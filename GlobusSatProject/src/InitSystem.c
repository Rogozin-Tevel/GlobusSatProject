#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <hal/Drivers/I2C.h>
#include <hal/Drivers/SPI.h>
#include <hal/Timing/Time.h>
#include <at91/utility/exithandler.h>
#include <string.h>
#include "GlobalStandards.h"
#include "SubSystemModules/PowerManagment/EPS.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/Maintenance/Maintenance.h"
#include "InitSystem.h"
#include "TLM_management.h"
#include <hal/Storage/FRAM.h>
#ifdef GOMEPS
	#include <satellite-subsystems/GomEPS.h>
#endif
#ifdef ISISEPS
	#include <satellite-subsystems/IsisEPS.h>
#endif
#define I2c_SPEED_Hz 100000
#define I2c_Timeout 10
#define I2c_TimeoutTest portMAX_DELAY

Boolean isFirstActivation()
{
	return FALSE;
}
//mamtim 0/5 hour + open anat.
void firstActivationProcedure()
{
//	vTaskDelay(SECONDS_TO_TICKS(1800)); // 0.5 hour delay
	 time_unix curr_time ;
     Time_getUnixEpoch(&curr_time);   //saving the current time

     time_unix deploy_time ;
     Time_getUnixEpoch(&deploy_time);

     int err= FRAM_write((unsigned char *)&curr_time, MOST_UPDATED_SAT_TIME_ADDR, MOST_UPDATED_SAT_TIME_SIZE);

     int err1= FRAM_write((unsigned char *)&deploy_time, DEPLOYMENT_TIME_ADDR, DEPLOYMENT_TIME_SIZE);

      while ((curr_time-deploy_time)<MINUTES_TO_SECONDS(30))
      {
	 	  Time_getUnixEpoch(&curr_time);

	 	  err= FRAM_write((unsigned char *)&curr_time,  MOST_UPDATED_SAT_TIME_ADDR, MOST_UPDATED_SAT_TIME_SIZE);
      }

}
void WriteDefaultValuesToFRAM()
{
}

int StartFRAM()
{
//	return 0;
	int err=FRAM_start();
	return err;
}

int StartI2C()
{
	//return 0;

	return ( I2C_start(I2c_SPEED_Hz, I2c_Timeout));

}

int StartSPI()
{
	//return 0;
	int err=  SPI_start(bus1_spi,slave1_spi);
	return err;

}
#define SATELLITE_LAUNCH_TIME {\
    .seconds=0,         \
    .minutes=35,        \
	.hours=13,         \
	.day=4,            \
	.date=1,           \
	.month=8,           \
	.year=19,            \
	.secondsOfYear=0}

int StartTIME()
{
   Time curr_time = SATELLITE_LAUNCH_TIME;

	int err= Time_start(&curr_time, 0);
 if (0!=err){
	return err;
 }
 if (!isFirstActivation()){
	 time_unix time_before_restart=0;
	 FRAM_read((unsigned char*)&time_before_restart,MOST_UPDATED_SAT_TIME_ADDR,MOST_UPDATED_SAT_TIME_SIZE);
	 err=Time_setUnixEpoch(time_before_restart);
 }
 return err;
 }


int DeploySystem()
{

	int err1=0, err2=0;
	Boolean an=FALSE;
	Boolean flag = isFirstActivation();

	if (flag)
	{

		 firstActivationProcedure();
		 time_unix  time_act=0;
	     Time_getUnixEpoch((unsigned int *)&time_act);
	     err1= FRAM_write((unsigned char *)&time_act, DEPLOYMENT_TIME_ADDR, DEPLOYMENT_TIME_SIZE);
	     err2= FRAM_write((unsigned char *)&an, FIRST_ACTIVATION_FLAG_ADDR, FIRST_ACTIVATION_FLAG_SIZE);

	}

	if (err1!=0 || err2!=0)
  	 return -1;
   else
	  return 0;
}

#define PRINT_IF_ERR(method) if(0 != err)printf("error in '" #method  "' err = %d\n",err);
int InitSubsystems()
{
	return 0;
}

