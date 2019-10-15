/*
 * filesystem.c
 *
 *  Created on: 20 áîøõ 2019
 *      Author: Idan
 */


#include <satellite-subsystems/GomEPS.h>
#include <hal/Timing/Time.h>
#include <hcc/api_fat.h>
#include <hal/errors.h>
#include <hcc/api_hcc_mem.h>
#include <string.h>
#include <hcc/api_mdriver_atmel_mcipdc.h>
#include <hal/Storage/FRAM.h>
#include <at91/utility/trace.h>
#include "TLM_management.h"
#include <stdlib.h>

#define SKIP_FILE_TIME_SEC 1000000
#define _SD_CARD 0
#define FIRST_TIME -1
#define FILE_NAME_WITH_INDEX_SIZE MAX_F_FILE_NAME_SIZE+sizeof(int)*2

//struct for filesystem info
typedef struct
{
	int num_of_files;
} FS;
//TODO remove all 'PLZNORESTART' from code!!
#define PLZNORESTART() gom_eps_hk_basic_t myEpsTelemetry_hk_basic;	(GomEpsGetHkData_basic(0, &myEpsTelemetry_hk_basic)); //todo delete

//struct for chain file info
typedef struct
{
	int size_of_element;
	char name[FILE_NAME_WITH_INDEX_SIZE];
	unsigned int creation_time;
	unsigned int last_time_modified;
	int num_of_files;

} C_FILE;
#define C_FILES_BASE_ADDR (FSFRAM+sizeof(FS))


void delete_allTMFilesFromSD()
{
	F_FIND find;
	if (!f_findfirst("A:/*.*",&find))
	{
		do
		{
			int count = 0;
			while (find.filename[count] != '.' && find.filename[count] != '\0' && count < 30)
				count++;
			count++;
			if (!memcmp(find.filename + count, FS_FILE_ENDING, (int)FS_FILE_ENDING_SIZE))
			{
				f_delete(find.filename);
			}

		} while (!f_findnext(&find));
	}
}
// return -1 for FRAM fail
static int getNumOfFilesInFS()
{
	FS fs;

	if(FRAM_read((unsigned char*)&fs,FSFRAM,sizeof(FS))!=0)
	{
		return -1;
	}
	return fs.num_of_files;
}
//return -1 on fail
static int setNumOfFilesInFS(int new_num_of_files)
{
	FS fs;
	fs.num_of_files = new_num_of_files;
	if(FRAM_write((unsigned char*)&fs,FSFRAM,sizeof(FS))!=0)
	{
		return -1;
	}
	return 0;
}
FileSystemResult InitializeFS(Boolean first_time)
{

	int ret;
	hcc_mem_init(); /* Initialize the memory to be used by filesystem */

	ret = fs_init(); /* Initialize the filesystem */
	if(ret != F_NO_ERROR )
	{
		TRACE_ERROR("fs_init pb: %d\n\r", ret);
		return FS_FAT_API_FAIL;
	}

	ret = f_enterFS(); /* Register this task with filesystem */
	if(ret != F_NO_ERROR )
	{
		TRACE_ERROR("f_enterFS pb: %d\n\r", ret);
		return FS_FAT_API_FAIL;
	}

	ret = f_initvolume( 0, atmel_mcipdc_initfunc, _SD_CARD ); /* Initialize volID as safe */

	if( F_ERR_NOTFORMATTED == ret )
	{
		TRACE_ERROR("Filesystem not formated!\n\r");
		return FS_FAT_API_FAIL;
	}
	else if( F_NO_ERROR != ret)
	{
		TRACE_ERROR("f_initvolume pb: %d\n\r", ret);
		return FS_FAT_API_FAIL;
	}
	if(first_time)
	{
		delete_allTMFilesFromSD();
		FS fs = {0};
		if(FRAM_write((unsigned char*)&fs,FSFRAM,sizeof(FS))!=0)
		{
			return FS_FAT_API_FAIL;
		}
	}

	F_SPACE space;
	/* get free space on current drive */
	ret = f_getfreespace(f_getdrive(),&space);
	if(!ret)
	{
	printf("There are %lu bytes total, %lu bytes free, \
	%lu bytes used, %lu bytes bad.",
	space.total, space.free, space.used, space.bad);
	}
	else
	{
	printf("\nError %d reading drive\n", ret);
	}

	return FS_SUCCSESS;
}

//only register the chain, files will create dynamically
FileSystemResult c_fileCreate(char* c_file_name,
		int size_of_element)
{
	if(strlen(c_file_name)>MAX_F_FILE_NAME_SIZE)//check len
	{
		return FS_TOO_LONG_NAME;
	}

	C_FILE c_file; //chain file descriptor
	strcpy(c_file.name,c_file_name);
	Time_getUnixEpoch(&c_file.creation_time);//get current time
	c_file.size_of_element =size_of_element;
	c_file.last_time_modified = FIRST_TIME;//no written yet
	int num_of_files_in_FS = getNumOfFilesInFS();
	if(num_of_files_in_FS == -1)
	{
		return FS_FRAM_FAIL;
	}
	int c_file_address =C_FILES_BASE_ADDR+num_of_files_in_FS*sizeof(C_FILE);
	if(FRAM_write((unsigned char*)&c_file,
			c_file_address,sizeof(C_FILE))!=0)//write c_file struct in FRAM
	{
			return FS_FRAM_FAIL;
	}
	if(setNumOfFilesInFS(num_of_files_in_FS+1)!=0)//TODO change to c_fil
	{
		return FS_FRAM_FAIL;
	}

	return FS_SUCCSESS;
}
//write element with timestamp to file
static void writewithEpochtime(F_FILE* file, byte* data, int size,unsigned int time)
{
	PLZNORESTART();

	int number_of_writes;
	number_of_writes = f_write( &time,sizeof(unsigned int),1, file );
	number_of_writes += f_write( data, size,1, file );
	//printf("writing element, time is: %u\n",time);
	if(number_of_writes!=2)
	{
		printf("writewithEpochtime error\n");
	}
	f_flush( file ); /* only after flushing can data be considered safe */
	f_close( file ); /* data is also considered safe when file is closed */
}
// get C_FILE struct from FRAM by name
static Boolean get_C_FILE_struct(char* name,C_FILE* c_file,unsigned int *address)
{

	PLZNORESTART();

	int i;
	unsigned int c_file_address = 0;
	int err_read=0;
	int num_of_files_in_FS = getNumOfFilesInFS();
	for(i =0; i < num_of_files_in_FS; i++)			//search correct c_file struct
	{
		c_file_address= C_FILES_BASE_ADDR+sizeof(C_FILE)*(i);
		err_read = FRAM_read((unsigned char*)c_file,c_file_address,sizeof(C_FILE));
		if(0 != err_read)
		{
			printf("FRAM error in 'get_C_FILE_struct()' error = %d\n",err_read);
			return FALSE;
		}

		if(!strcmp(c_file->name,name))
		{
			if(address != NULL)
			{
				*address = c_file_address;
			}
			return TRUE;//stop when found
		}
	}
	return FALSE;
}
//calculate index of file in chain file by time
static int getFileIndex(unsigned int creation_time, unsigned int current_time)
{
	PLZNORESTART();
	return ((current_time-creation_time)/SKIP_FILE_TIME_SEC);
}
//write to curr_file_name
void get_file_name_by_index(char* c_file_name,int index,char* curr_file_name)
{
	PLZNORESTART();
	sprintf(curr_file_name,"%s%d.%s", c_file_name, index, FS_FILE_ENDING);
}
FileSystemResult c_fileReset(char* c_file_name)
{
	C_FILE c_file;
	unsigned int addr;//FRAM ADDRESS
	//F_FILE *file;
	char curr_file_name[MAX_F_FILE_NAME_SIZE+sizeof(int)*2];
	PLZNORESTART();
	unsigned int curr_time;
	Time_getUnixEpoch(&curr_time);
	if(get_C_FILE_struct(c_file_name,&c_file,&addr)!=TRUE)//get c_file
	{
		return FS_NOT_EXIST;
	}
	for(int i =0; i<c_file.num_of_files;i++)
	{
		get_file_name_by_index(c_file_name,i,curr_file_name);
		f_delete(c_file_name);
	}
	c_file.last_time_modified=-1;
	c_file.creation_time =curr_time;
	return FS_SUCCSESS;
}

FileSystemResult c_fileWrite(char* c_file_name, void* element)
{
	C_FILE c_file;
	unsigned int addr;//FRAM ADDRESS
	F_FILE *file;
	char curr_file_name[MAX_F_FILE_NAME_SIZE+sizeof(int)*2];
	PLZNORESTART();
	unsigned int curr_time;
	Time_getUnixEpoch(&curr_time);
	if(get_C_FILE_struct(c_file_name,&c_file,&addr)!=TRUE)//get c_file
	{
		return FS_NOT_EXIST;
	}
	int index_current = getFileIndex(c_file.creation_time,curr_time);
	get_file_name_by_index(c_file_name,index_current,curr_file_name);
	int error = f_enterFS();
	check_int("c_fileWrite, f_enterFS", error);
	file = f_open(curr_file_name,"a+");
	writewithEpochtime(file,element,c_file.size_of_element,curr_time);
	c_file.last_time_modified= curr_time;
	if(FRAM_write((unsigned char *)&c_file,addr,sizeof(C_FILE))!=0)//update last written
	{
		return FS_FRAM_FAIL;
	}
	f_close(file);
	f_releaseFS();
	return FS_SUCCSESS;
}
FileSystemResult fileWrite(char* file_name, void* element,int size)
{
	F_FILE *file;
	unsigned int curr_time;
	Time_getUnixEpoch(&curr_time);
	file= f_open(file_name,"a+");
	writewithEpochtime(file,element,size,curr_time);
	f_flush(file);
	f_close(file);
	return FS_SUCCSESS;
}
static FileSystemResult deleteElementsFromFile(char* file_name,unsigned long from_time,
		unsigned long to_time,int full_element_size)
{
	F_FILE* file = f_open(file_name,"r");
	F_FILE* temp_file = f_open("temp","a+");
	char* buffer = malloc(full_element_size);
	for(int i = 0; i<f_filelength(file_name); i++)
	{

		f_read(buffer,1,full_element_size,file);
		unsigned int element_time = *((unsigned int*)buffer);
		if(element_time>=from_time&&element_time<=to_time)
		{
			f_write(buffer,1,full_element_size,temp_file);
		}
	}
	f_close(file);
	f_close(temp_file);
	free(buffer);
	f_delete(file_name);
	f_rename("temp",file_name);
	return FS_SUCCSESS;

}
FileSystemResult c_fileDeleteElements(char* c_file_name, time_unix from_time,
		time_unix to_time)
{
	C_FILE c_file;
	unsigned int addr;//FRAM ADDRESS
	char curr_file_name[MAX_F_FILE_NAME_SIZE+sizeof(int)*2];
	PLZNORESTART();
	unsigned int curr_time;
	Time_getUnixEpoch(&curr_time);
	if(get_C_FILE_struct(c_file_name,&c_file,&addr)!=TRUE)//get c_file
	{
		return FS_NOT_EXIST;
	}
	int first_file_index = getFileIndex(c_file.creation_time,from_time);
	int last_file_index = getFileIndex(c_file.creation_time,to_time);
	if(first_file_index+1<last_file_index)//delete all files between first to kast file
	{
		for(int i =first_file_index+1; i<last_file_index;i++)
		{
			get_file_name_by_index(c_file_name,i,curr_file_name);
			f_delete(curr_file_name);
		}
	}
	get_file_name_by_index(c_file_name,first_file_index,curr_file_name);
	deleteElementsFromFile(curr_file_name,from_time,to_time,c_file.size_of_element+sizeof(int));
	if(first_file_index!=last_file_index)
	{
		get_file_name_by_index(c_file_name,last_file_index,curr_file_name);
		deleteElementsFromFile(curr_file_name,from_time,to_time,c_file.size_of_element+sizeof(int));
	}
	return FS_SUCCSESS;
}
FileSystemResult fileRead(char* c_file_name,byte* buffer, int size_of_buffer,
		time_unix from_time, time_unix to_time, int* read, int element_size)
{
	*read=0;
	F_FILE* current_file= f_open(c_file_name,"r+");
	int buffer_index = 0;
	void* element;
	unsigned int size_elementWithTimeStamp = element_size+sizeof(unsigned int);
	element = malloc(size_elementWithTimeStamp);//store element and his timestamp
	unsigned int length =f_filelength(c_file_name)/(size_elementWithTimeStamp);//number of elements in currnet_file
	int err_fread=0;

	f_seek( current_file, 0L , SEEK_SET );
	for(unsigned int j=0;j < length;j++)
	{
		err_fread = f_read(element,(size_t)size_elementWithTimeStamp,(size_t)1,current_file);
		(void)err_fread;
		unsigned int element_time = *((unsigned int*)element);
		printf("read element, time is %u\n",element_time);
		if(element_time > to_time)
		{
			break;
		}

		if(element_time >= from_time)
		{
			if((unsigned int)buffer_index>(unsigned int)size_of_buffer)
			{
				return FS_BUFFER_OVERFLOW;
			}
			(*read)++;
			memcpy(buffer + buffer_index,element,size_elementWithTimeStamp);
			buffer_index += size_elementWithTimeStamp;
		}
	}
	f_close(current_file);


	free(element);

	return FS_SUCCSESS;
}
FileSystemResult c_fileRead(char* c_file_name,byte* buffer, int size_of_buffer,
		time_unix from_time, time_unix to_time, int* read,time_unix* last_read_time)
{
	C_FILE c_file;
	unsigned int addr;//FRAM ADDRESS
	//F_FILE *file;
	char curr_file_name[MAX_F_FILE_NAME_SIZE+sizeof(int)*2];
	PLZNORESTART();

	int buffer_index = 0;
	void* element;
	if(get_C_FILE_struct(c_file_name,&c_file,&addr)!=TRUE)//get c_file
	{
		return FS_NOT_EXIST;
	}
	if(from_time<c_file.creation_time)
	{
		from_time=c_file.creation_time;
	}
	F_FILE* current_file;
	int index_current = getFileIndex(c_file.creation_time,from_time);
	get_file_name_by_index(c_file_name,index_current,curr_file_name);
	unsigned int size_elementWithTimeStamp = c_file.size_of_element+sizeof(unsigned int);
	element = malloc(size_elementWithTimeStamp);//store element and his timestamp
	do
	{
		get_file_name_by_index(c_file_name,index_current++,curr_file_name);
		int error = f_enterFS();
		check_int("c_fileWrite, f_enterFS", error);
		current_file= f_open(curr_file_name,"r");
		if (current_file == NULL)
			return FS_NOT_EXIST;
		unsigned int length =f_filelength(curr_file_name)/(size_elementWithTimeStamp);//number of elements in currnet_file
		int err_fread=0;
		(void)err_fread;
		f_seek( current_file, 0L , SEEK_SET );
		for(unsigned int j=0;j < length;j++)
		{
			err_fread = f_read(element,(size_t)size_elementWithTimeStamp,(size_t)1,current_file);

			unsigned int element_time = *((unsigned int*)element);
			//printf("read element, time is %u\n",element_time);
			if(element_time > to_time)
			{
				break;
			}

			if(element_time >= from_time)
			{
				*last_read_time = element_time;
				if((unsigned int)buffer_index>(unsigned int)size_of_buffer)
				{
					free(element);
					return FS_BUFFER_OVERFLOW;
				}
				(*read)++;
				memcpy(buffer + buffer_index,element,size_elementWithTimeStamp);
				buffer_index += size_elementWithTimeStamp;
			}
		}
		f_close(current_file);
		f_releaseFS();
	}while(getFileIndex(c_file.creation_time,c_file.last_time_modified)>=index_current);


	free(element);

	return FS_SUCCSESS;
}
void print_file(char* c_file_name)
{
	C_FILE c_file;
	F_FILE* current_file;
	int i = 0;
	void* element;
	char curr_file_name[FILE_NAME_WITH_INDEX_SIZE];//store current file's name
	//int temp[2];//use to append name with index
	//temp[1] = '\0';
	if(get_C_FILE_struct(c_file_name,&c_file,NULL)!=TRUE)
	{
		printf("print_file_error\n");
	}
	PLZNORESTART();
	element = malloc(c_file.size_of_element+sizeof(unsigned int));//store element and his timestamp
	for(i=0;i<c_file.num_of_files;i++)
	{
		printf("file %d:\n",i);//print file index
		get_file_name_by_index(c_file_name,i,curr_file_name);
		current_file=f_open(curr_file_name,"r");
		for(int j=0;j<f_filelength(curr_file_name)/((int)c_file.size_of_element+(int)sizeof(unsigned int));j++)
		{
			f_read(element,c_file.size_of_element,(size_t)c_file.size_of_element+sizeof(unsigned int),current_file);
			printf("time: %d\n data:",*((int*)element));//print element timestamp
			for(j =0;j<c_file.size_of_element;j++)
			{
				printf("%d ",*((byte*)(element+sizeof(int)+j)));//print data
			}
			printf("\n");
		}
		f_close(current_file);
	}
}

void DeInitializeFS( void )
{
	printf("deinitializig file system \n");
	int err = f_delvolume( _SD_CARD ); /* delete the volID */

	printf("1\n");
	PLZNORESTART();
	if(err != 0)
	{
		printf("f_delvolume err %d\n", err);
	}

	f_releaseFS(); /* release this task from the filesystem */

	printf("2\n");

	err = fs_delete(); /* delete the filesystem */

	printf("3\n");

	if(err != 0)
	{
		printf("fs_delete err , %d\n", err);
	}
	err = hcc_mem_delete(); /* free the memory used by the filesystem */

	printf("4\n");

	if(err != 0)
	{
		printf("hcc_mem_delete err , %d\n", err);
	}
	printf("deinitializig file system end \n");

}
typedef struct{
	int a;
	int b;
}TestStruct ;
/*void test_i()
{

	time_unix curr_time;
	time_unix read=0;
	time_unix last_read_time=0;
	f_delete("idan0");
	TestStruct test_struct_arr[8]={0};
	Time_getUnixEpoch(&curr_time);
	time_unix start_time_unix=curr_time;
	InitializeFS(TRUE);
	TestStruct test_struct = {3,4};
	c_fileCreate("idan",sizeof(TestStruct));
	curr_time+=200;
	Time_setUnixEpoch(curr_time);
	c_fileWrite("idan",&test_struct);
	Time_setUnixEpoch(++curr_time);
	c_fileWrite("idan",&test_struct);
	Time_setUnixEpoch(++curr_time);
	c_fileWrite("idan",&test_struct);
	c_fileRead("idan",test_struct_arr,8,start_time_unix,++curr_time,&read,&last_read_time);
	c_fileReset("idan");


}

*/
