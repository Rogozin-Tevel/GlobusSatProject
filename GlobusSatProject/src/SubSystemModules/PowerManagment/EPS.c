
#include <satellite-subsystems/IsisSolarPanelv2.h>
#include <hal/errors.h>

#include <string.h>

#include "EPS.h"
#ifdef ISISEPS
	#include <satellite-subsystems/IsisEPS.h>
#endif
#ifdef GOMEPS
	#include <satellite-subsystems/GomEPS.h>
#endif

// y[i] = a * x[i] +(1-a) * y[i-1]
voltage_t prev_avg = 0;		// y[i-1]
float alpha = 0;			//<! smoothing constant

voltage_t eps_threshold_voltages[NUMBER_OF_THRESHOLD_VOLTAGES];	// saves the current EPS logic threshold voltages
//done
int GetBatteryVoltage(voltage_t *vbatt)
{
	int err;

	ieps_enghk_data_cdb_t p_raenghk_data_cdb;

	unsigned char index= EPS_I2C_BUS_INDEX ;

	ieps_board_t board= ieps_board_cdb1;

	ieps_statcmd_t p_rsp_code;

	//get house keeping  data

	err= IsisEPS_getRAEngHKDataCDB(index, board, &p_raenghk_data_cdb, &p_rsp_code);

	*vbatt=p_raenghk_data_cdb.fields.bat_voltage; //saving the batt voltage

	if(err!=E_NO_SS_ERR)
	    return 0;
	else
		return -1;
}
//done
int EPS_Init()
{
	//a*******************************************************************************************************
    int err;
    unsigned char eps_addr= EPS_I2C_ADDR;

	err= IsisEPS_initialize(&eps_addr, 1);
	//b*******************************************************************************************************
	if (err!=E_NO_SS_ERR)
		return -1;
	//c*******************************************************************************************************
	err= IsisSolarPanelv2_initialize( slave0_spi);
	//d*******************************************************************************************************
	if (err==ISIS_SOLAR_PANEL_STATE_NOINIT)
			return -2;
	//loss batt
	IsisSolarPanelv2_sleep();
	//e*******************************************************************************************************
	err=GetThresholdVoltages(eps_threshold_voltages);
	if (0!=err)
	{
		voltage_t temp[]=DEFAULT_EPS_THRESHOLD_VOLTAGES ;
		memcpy(eps_threshold_voltages,temp,sizeof(temp));
		return -3;
	}
	err=GetAlpha(&alpha);
	if (0!=err)
	{
		alpha=DEFAULT_ALPHA_VALUE;
		return -4;
	}
    GetBatteryVoltage(&prev_avg);
    EPS_Conditioning();
    return 0;
}

int EPS_Conditioning()
{
	//******done by diana*****
	voltage_t vbatt;
	voltage_t curr_avg;

	//voltage_t prev_avg = 0;

	int an=0;

	an = GetBatteryVoltage(&vbatt);

	if (an ==-1)
	 {
		 printf("error to get Battery Voltages\n");
	 }

	an =FRAM_read((unsigned char *)&alpha,EPS_ALPHA_FILTER_VALUE_ADDR,sizeof(EPS_ALPHA_FILTER_VALUE_SIZE)); //or using:  GetAlpha(&alpha)

	if (an!=0)
		{
		 printf("cannot read alpha\n");
		}

	 an= GetThresholdVoltages(eps_threshold_voltages);  // adding the levels for eps

	curr_avg= alpha*prev_avg + (1-alpha)*vbatt; // funcation

	if (curr_avg > prev_avg)
	{
		if (prev_avg > eps_threshold_voltages[INDEX_UP_FULL])
		{
			an= EnterFullMode();
			if (an ==-1)
				 {
					 printf("error to go Full Mode\n");
				 }
			else
			{
				printf("Entering to Full Mode\n");
			}

		}
		else
			if (prev_avg > eps_threshold_voltages[INDEX_UP_CRUISE])
			{
			  an=  EnterCruiseMode();
			  if (an ==-1)
			  	{
			  	  printf("error to go Cruise Mode\n");
			  	}
			  else
			  {
				  printf("Entering to Cruise Mode\n");
			  }

			}
			else
				if (prev_avg > eps_threshold_voltages[INDEX_UP_SAFE])
				{
				  an=  EnterSafeMode();
				  if (an ==-1)
				  	{
				  	  printf("error to go Safe Mode\n");
				  	}
				  else
				   {
				    printf("Entering to Safe Mode\n");
				   }
				}
	  }
	else
	{
		/*if (prev_avg < eps_threshold_voltages[INDEX_DOWN_SAFE])
		 {
			 an= EnterCriticalMode();
			 if (an ==-1)
				 {
					printf("error to go Critical Mode\n");
				 }
			else
				{
					printf("Entering to Critical Mode\n");
				}

				}
				else*/
					if (prev_avg < eps_threshold_voltages[INDEX_DOWN_SAFE])
					{
					  an= EnterSafeMode();
					  if (an ==-1)
					  	{
					  	  printf("error to go Safe Mode\n");
					  	}
					  else
					  {
						  printf("Entering to Safe Mode\n");
					  }

					}
					else
						if (prev_avg < eps_threshold_voltages[INDEX_DOWN_CRUISE])
						{
						  an=  EnterCruiseMode();
						  if (an ==-1)
						  	{
						  	  printf("error to go Cruise Mode");
						  	}
						  else
						   {
						    printf("Entering to Cruise Mode\n");
						   }
						}
	}


        prev_avg=curr_avg;


	return an;
}

int UpdateAlpha(float new_alpha)
{
	//******done by diana*****
    if (new_alpha <0)
    {
    	printf("error: the new paramater is nor right\n");
    	return -2;
    }
	if(FRAM_write((unsigned char*)&new_alpha,EPS_ALPHA_FILTER_VALUE_ADDR,EPS_ALPHA_FILTER_VALUE_SIZE)!=0) //כשלון הכתיבה לפראם
		 	{
		     printf("error: cannot to write alpha to fram\n");
		     return -1;
			}
	else
	{
		printf("succed:to write alpha to fram\n");
		return 0;
	}
}

int UpdateThresholdVoltages(voltage_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES])
{
	//******done by diana*****
	//eps_threshold_voltages
	return 0;
}

int GetThresholdVoltages(voltage_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES])
{
	return 0;
}

int GetAlpha(float *alpha)
{
	int an =0;

	an = FRAM_read((unsigned char *)&alpha,EPS_ALPHA_FILTER_VALUE_ADDR,sizeof(EPS_ALPHA_FILTER_VALUE_SIZE));

	if (an==0 )
	{
		printf("success to get alpha");

	}

	else
		if (an ==-1)
		{
          printf("obtaining lock for FRAM access fails/ NULL input array");
          an=-2;
		}

		else
			if (an==-2)
			{
				an=-1;
	            printf("NULL input array");
			}

	return an;
}

int RestoreDefaultAlpha()
{
	alpha=DEFAULT_ALPHA_VALUE;
	if(FRAM_write((unsigned char*)&alpha,EPS_ALPHA_FILTER_VALUE_ADDR,EPS_ALPHA_FILTER_VALUE_SIZE)!=0) //כשלון הכתיבה לפראם
			 	{
			     printf("error: cannot to write alpha to fram\n");
			     return -1;
				}
	else
	{
      printf("success to Restore Default Alpha \n");
	  return 0;
	}
}

int RestoreDefaultThresholdVoltages()
{
	//to do memcpy
	//we need to do pointerim
	//memcpy(eps_threshold_voltages,DEFAULT_EPS_THRESHOLD_VOLTAGES,NUMBER_OF_THRESHOLD_VOLTAGES);
	return 0;
}

