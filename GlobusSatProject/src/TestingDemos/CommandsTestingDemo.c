#include "CommandsTestingDemo.h"

#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/Communication/ActUponCommand.h"
#include "SubSystemModules/Communication/CommandDictionary.h"
#include "SubSystemModules/Communication/AckHandler.h"
#include "SubSystemModules/Communication/SPL.h"
#include "SubSystemModules/Communication/SatCommandHandler.h"

#include <stdio.h>
#include <string.h>
#include <hal/Utility/util.h>

Boolean TestActUponCommand()
{
	printf("\nPlease insert number of minutes to test(1 to 10)\n");
	int minutes = 0;
	int err = 0;
	while(UTIL_DbguGetIntegerMinMax((unsigned int*)&minutes,1,10) == 0);

	portTickType curr_time = xTaskGetTickCount();
	portTickType end_time = MINUTES_TO_TICKS(minutes) + curr_time;

	sat_packet_t cmd = {0};
	while(end_time - curr_time > 0)
	{
		curr_time = xTaskGetTickCount();
		err = GetOnlineCommand(&cmd);
		if(command_found == err){
			printf("Online Command Found!!\n");
			err = ActUponCommand(&cmd);
			if(0 != err){
				printf("error in 'ActUponCommand' = %d\n",err);
			}
		}
		vTaskDelay(100);
	}
	return TRUE;
}

Boolean TestAssmbleCommand()
{
	unsigned char data[] = {1,2,3,4,5,6,7,8,9,10,
							11,12,13,14,15,16,17,18,19,20};
	unsigned int length = sizeof(data);
	unsigned char type = 0x42;
	unsigned char subtype = 0x43;
	unsigned int id = 0x12345678;

	sat_packet_t cmd = {0};
	int err = AssmbleCommand(data,length,type,subtype,id,&cmd);
	if(0 != err){
		printf("error in 'AssmbleCommand' = %d\n",err);
		return TRUE;
	}
	printf("id = %d\n",			cmd.ID);
	printf("type = %d\n",		cmd.cmd_type);
	printf("subtype = %d\n",	cmd.cmd_subtype);
	printf("data length = %d\n",cmd.length);

	printf("data =\t");
	for(unsigned int i= 0;i<cmd.length;i++)
	{
		printf("%X\t",cmd.data[i]);
	}
	printf("\n");
	return TRUE;
}

Boolean TestGetDelayedCommand()
{
	sat_packet_t cmd = {0};
	int err = GetDelayedCommand(&cmd);
	if(err == command_found)
	{
		printf("command found:\n");
		for(unsigned int i = 0; i < sizeof(sat_packet_t);i++){
			printf("%x\t",((unsigned char*)(&cmd))[i]);
		}
	}
	return TRUE;
}

Boolean TestAddDelayedCommand()
{
	sat_packet_t cmd = {0};
	cmd.ID = 0x12345678;
	cmd.cmd_type = 0x42;
	cmd.cmd_subtype = 0x43;
	unsigned char data[] ={1,2,3,4,5,6,7,8,9,10};
	memcpy(cmd.data,data,sizeof(data));
	cmd.length = sizeof(data);
	int err = AddDelayedCommand(&cmd);
	if(0 != err){
		printf("error in 'AddDelayedCommand' = %d",err);
	}
	return TRUE;
}

Boolean TestGetDelayedCommandBufferCount()
{
	int err = GetDelayedCommandBufferCount();
	if(-1 == err){
		printf("error in 'GetDelayedCommandBufferCount'\n");
		return TRUE;
	}
	printf("number of delayed commands in the buffer = %d",err);
	return TRUE;
}

Boolean TestGetOnlineCommand()
{
	sat_packet_t cmd = {0};
	int err = 0;
	err = GetOnlineCommand(&cmd);
	if(command_found != err){
		printf("error in 'GetOnlineCommand' = %d",err);
		return TRUE;
	}

	printf("data of the online command:\n");
	for(unsigned int i = 0; i < sizeof(sat_packet_t);i++){
		printf("%x\t",((unsigned char*)(&cmd))[i]);
	}
	printf("\n");
	return TRUE;
}

Boolean TestDeleteDelayedCommandByIndex()
{
	printf("Please choose index to delete at\n");
	unsigned int index = 0;
	while(UTIL_DbguGetIntegerMinMax(&index,0,MAX_NUM_OF_DELAYED_CMD));

	int err = 0;
	sat_packet_t cmd = {0};
	err = GetDelayedCommandByIndex(index,&cmd);
	if(err != 0){
		printf("error in 'GetDelayedCommandByIndex' = %d\n",err);
		return TRUE;
	}
	printf("data of command at index %d:\n",index);
	for(unsigned int i = 0; i < sizeof(sat_packet_t);i++){
		printf("%x\t",((unsigned char*)(&cmd))[i]);
	}
	printf("\n");

	err =  DeleteDelayedCommandByIndex(index);
	if(0 != err){
		printf("error in 'DeleteDelayedCommandByIndex' = %d",err);
		return TRUE;
	}

	char res = 0;
	for(unsigned int i = 0; i < sizeof(sat_packet_t); i++){
		res += ((unsigned char*)(&cmd))[i];
	}

	if(0 != res){
		printf("error in zero'ing command at index %d\n",index);
		return TRUE;
	}
	printf("Delayed Buffer Deletion Succsess\n");
	return TRUE;
}

Boolean TestDeleteDelayedBuffer()
{
	int err = DeleteDelayedBuffer();
	if(0 != err){
		printf("Error in 'DeleteDelayedBuffer' = %d\n",err);
		return TRUE;
	}
	err = GetDelayedCommandBufferCount();
	if(-1 == err){
		printf("error in 'GetDelayedCommanBufferCount' == %d\n",err);
		return TRUE;
	}
	err = 0;
	sat_packet_t cmd = {0};
	err = GetDelayedCommandByIndex(0,&cmd);
	if(err != 0){
		printf("error in 'GetDelayedCommandByIndex' = %d\n",err);
		return TRUE;
	}

	char res = 0;
	for(unsigned int i = 0; i < sizeof(sat_packet_t); i++){
		res += ((unsigned char*)(&cmd))[i];
	}

	if(0 != res){
		printf("error in setting buffer to zero\n");
		return TRUE;
	}
	printf("Delayed Buffer Deletion Succsess\n");
	return TRUE;
}

Boolean selectAndExecuteCommandsDemoTest()
{
	unsigned int selection = 0;
	Boolean offerMoreTests = TRUE;

	printf("\n\r Select a test to perform: \n\r");
	printf("\t 0) Return to main menu \n\r");
	printf("\t 1) Test Act Upon Command\n\r");
	printf("\t 2) Test Assmble Command\n\r");
	printf("\t 3) Get Delayed Command\n\r");
	printf("\t 4) Add Delayed Command\n\r");
	printf("\t 5) Get Delayed Command Buffer Count\n\r");
	printf("\t 6) Get Online Command\n\r");
	printf("\t 7) Delete Delayed Command By Index\n\r");
	printf("\t 8) Delete Delayed Buffer\n\r");

	unsigned int number_of_tests = 8;
	while(UTIL_DbguGetIntegerMinMax(&selection, 0, number_of_tests) == 0);

	switch(selection) {
	case 0:
		offerMoreTests = FALSE;
		break;
	case 1:
		offerMoreTests = TestActUponCommand();
		break;
	case 2:
		offerMoreTests = TestAssmbleCommand();
		break;
	case 3:
		offerMoreTests = TestGetDelayedCommand();
		break;
	case 4:
		offerMoreTests = TestAddDelayedCommand();
		break;
	case 5:
		offerMoreTests = TestGetDelayedCommandBufferCount();
		break;
	case 6:
		offerMoreTests = TestGetOnlineCommand();
		break;
	case 7:
		offerMoreTests = TestDeleteDelayedCommandByIndex();
		break;
	case 8:
		offerMoreTests = TestDeleteDelayedBuffer();
		break;
	}
	return offerMoreTests;
}

Boolean MainCommandsTestBench()
{
	Boolean offerMoreTests = FALSE;

	while(1)
	{
		offerMoreTests = selectAndExecuteCommandsDemoTest();

		if(offerMoreTests == FALSE)
		{
			break;
		}
	}
	return FALSE;
}
