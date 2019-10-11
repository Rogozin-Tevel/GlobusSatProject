/*
 * TM_managment.h
 *
 *  Created on: Apr 8, 2019
 *      Author: Hoopoe3n
 */

#ifndef TM_MANAGMENT_H_
#define TM_MANAGMENT_H_

#include <hal/Boolean.h>
#include "../Global/Global.h"

#define MAX_F_FILE_NAME_SIZE 7
#define FIRST_ELEMENT_IN_C_FILE 0
#define LAST_ELEMENT_IN_C_FILE 0
#define DEFAULT_NUM_OF_FILES 0

#define FS_FILE_ENDING	"TLM"
#define FS_FILE_ENDING_SIZE	3

#ifndef FSFRAM
#define FSFRAM 0x20000
#endif

typedef enum
{
	FS_SUCCSESS,
	FS_DUPLICATED,
	FS_LOCKED,
	FS_TOO_LONG_NAME,
	FS_BUFFER_OVERFLOW,
	FS_NOT_EXIST,
	FS_ALLOCATION_ERROR,
	FS_FRAM_FAIL,
	FS_FAT_API_FAIL,
	FS_FAIL
} FileSystemResult;

/*
 *
 */
void delete_allTMFilesFromSD();
/*!
 * Initializes the file system.
 * @note call once for boot and after DeInitializeFS.
 * @return FS_FAIL if Initializing the FS failed,
 * FS_ALLOCATION_ERROR on malloc error,
 * FS_SUCCSESS on success.
 */
FileSystemResult InitializeFS();

/*!
 * DeInitializes the file system.
 */
void DeInitializeFS();

/*!
 * Create new c_file.
 * @param c_file_name the name of the c_file.
 * @param size_of_element size of the structure.
 * DEFAULT_NUM_OF_FILES for default(recommended).
 * @return FS_TOO_LONG_NAME if c_file_name size is bigger then MAX_F_FILE_NAME_SIZE,
 * FS_FRAM_FAIL,
 * FS_SUCCSESS on success.
 */
FileSystemResult c_fileCreate(char* c_file_name,
		int size_of_element);
/*!
 * Write element to c_file.
 * @param c_file_name the name of the c_file.
 * @param element the structure of the telemetry/data.
 * @return FS_NOT_EXIST if c_file not exist,
 * FS_LOCKED if c_file used by other thread,
 * FS_SUCCSESS on success.
 */
FileSystemResult c_fileWrite(char* c_file_name, void* element);

/*!
 * Delete elements from c_file from "from_time" to "to_time".
 * @param c_file_name the name of the c_file.
 * @param from_time time of first element, FIRST_ELEMENT_IN_C_FILE to first element.
 * @param to_time time of last element, LAST_ELEMENT_IN_C_FILE to last element.
 * @return FS_NOT_EXIST if c_file not exist,
 * FS_LOCKED if c_file used by other thread,
 * FS_SUCCSESS on success.
 */
FileSystemResult c_fileDeleteElements(char* c_file_name, time_unix from_time,
		time_unix to_time);
/*!
 * Find number of elements from "from_time" to "to_time"
 * @param c_file_name the name of the c_file.
 * @param from_time time of first element, FIRST_ELEMENT_IN_C_FILE to first element.
 * @param to_time time of last element, LAST_ELEMENT_IN_C_FILE to last element.
 * @return num of elements.
 */
int c_fileGetNumOfElements(char* c_file_name,time_unix from_time
		,time_unix to_time);
/*!
 * Read elements from c_file to buffer
 * @param c_file_name the name of the c_file.
 * @param buffer.
 * @param size_of_buffer.
 * @param read[out] number of elements read.
 * @param from_time time of first element, FIRST_ELEMENT_IN_C_FILE to first element.
 * @param to_time time of last element, LAST_ELEMENT_IN_C_FILE to last element.
 * @return FS_BUFFER_OVERFLOW if size_of_buffer too small,
 * @return FS_NOT_EXIST if c_file not exist,
 * FS_SUCCSESS on success.
 */
FileSystemResult c_fileRead(char* c_file_name, byte* buffer, int size_of_buffer,
		time_unix from_time, time_unix to_time, int* read,time_unix* last_read_time);

//print c_file for testing
void print_file(char* c_file_name);
FileSystemResult c_fileReset(char* c_file_name);
int FS_test();
void test_i();
#endif /* TM_MANAGMENT_H_ */
