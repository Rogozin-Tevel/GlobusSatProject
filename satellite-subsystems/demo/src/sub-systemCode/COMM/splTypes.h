/*
 * splTypes.h
 *
 *  Created on: Dec 5, 2018
 *      Author: Hoopoe3n
 */

#ifndef SPLTYPES_H_
#define SPLTYPES_H_

//Types (TM)
#define BEACON_T 3
#define APRS 20
#define ACK_TYPE 13
#define DUMP_T 173
#define IMAGE_DUMP_T 11
#define ACK_ST 90
#define TM_ADCS_ST 	42
#define ADCS_SC_ST	55

//Types (TC)

#define NONE_APRS_T 33
#define COMM_T 		13
#define GENERAL_T 	20
#define TC_ADCS_T 		154
#define GENERALLY_SPEAKING_T		195
#define PAYLOAD_T	251
#define EPS_T		6
#define SOFTWARE_T 42
#define SPECIAL_OPERATIONS_T	69

//SubType (TM)
#define BEACON_ST 25
#define APRS_PACKET_FRAM 33
//dumps subTypes:
#define EPS_DUMP_ST 13
#define CAM_DUMP_ST 23
#define COMM_DUMP_ST 45
#define ADCS_DUMP_ST 78
#define SP_DUMP_ST	91

#define IMAGE_DUMP_THUMBNAIL4_ST	100
#define IMAGE_DUMP_THUMBNAIL3_ST	101
#define IMAGE_DUMP_THUMBNAIL2_ST	102
#define IMAGE_DUMP_THUMBNAIL1_ST	103
#define IMAGE_DUMP_RAW_ST			104
#define IMAGE_DUMP_JPG_ST			105
//ADCS science sub-types
#define ADCS_CSS_DATA_ST 				122
#define ADCS_MAGNETIC_FILED_ST			123
#define ADCS_CSS_SUN_VECTOR_ST			124
#define ADCS_WHEEL_SPEED_ST				125
#define ADCS_SENSORE_RATE_ST			126
#define ADCS_MAG_CMD_ST					127
#define ADCS_WHEEL_CMD_ST				128
#define ADCS_MAG_RAW_ST					129
#define ADCS_IGRF_MODEL_ST				130
#define ADCS_GYRO_BIAS_ST				131
#define ADCS_INNO_VEXTOR_ST				132
#define ADCS_ERROR_VEC_ST				133
#define ADCS_QUATERNION_COVARIANCE_ST 	134
#define ADCS_ANGULAR_RATE_COVARIANCE_ST 135
#define ADCS_ESTIMATED_ANGLES_ST		136
#define ADCS_ESTIMATED_AR_ST			137
#define ADCS_ECI_POS_ST					138
#define ADCS_SAV_VEL_ST					139
#define ADCS_ECEF_POS_ST				140
#define ADCS_LLH_POS_ST					141
#define ADCS_EST_QUATERNION_ST			142

//camm staff
#define IMAGE_DUMP_ST 122

//Subtypes (TC)
//comm
#define MUTE_ST						0
#define UNMUTE_ST					1
#define ACTIVATE_TRANS_ST			24
#define SHUT_TRANS_ST				25
#define CHANGE_TRANS_RSSI_ST		26
#define APRS_DUMP_ST				90
#define STOP_DUMP_ST				91
#define TIME_FREQUENCY_ST			5

//general
#define SOFT_RESET_ST			68
#define HARD_RESET_ST			66
#define RESET_SAT_ST 			67
#define GRACEFUL_RESET_ST		69
#define UPLOAD_TIME_ST 			17

//generally speaking
#define GENERIC_I2C_ST			0
#define DUMP_ST					33
#define DELETE_PACKETS_ST		35
#define RESET_FILE_ST			45
#define RESTSRT_FS_ST			46
#define DUMMY_FUNC_ST			122
#define REDEPLOY				56
#define ARM_DISARM				57

//payload
#define SEND_PIC_CHUNCK_ST		1
#define UPDATE_STN_PARAM_ST 	2
#define GET_IMG_DATA_BASE_ST	7
#define RESET_DATA_BASE_ST		11
#define DELETE_PIC_ST			42
#define UPD_DEF_DUR_ST			69
#define	OFF_CAM_ST				73
#define ON_CAM_ST				101
#define MOV_IMG_CAM_OBS_ST		123
#define TAKE_IMG_DEF_VAL_ST		213
#define TAKE_IMG_ST				255
#define CREATE_TB_FROM_IMAGE_ST	51

//eps
#define ALLOW_ADCS_ST			99
#define SHUT_ADCS_ST			100
#define ALLOW_CAM_ST			101
#define SHUT_CAM_ST			102
#define UPD_LOGIC_VOLT_ST		255
#define UPD_COMM_VOLTAGE		20
#define CHANGE_HEATER_TMP_ST	0

//adcs
#define ADCS_SET_BOOT_INDEX_ST			98
#define ADCS_RUN_SELECTED_BOOT_PROG_ST	99
#define ADCS_SET_TLE_ST					100
#define ADCS_SET_TLE_PART_0_ST				101
#define ADCS_SET_TLE_PART_1_ST				102
#define ADCS_SET_TLE_PART_2_ST				103
#define ADCS_SET_TLE_PART_3_ST				104
#define ADCS_SET_TLE_PART_4_ST				105
#define ADCS_SET_TLE_PART_5_ST				106
#define ADCS_SET_TLE_PART_6_ST				107
#define ADCS_SET_TLE_PART_7_ST				108
#define ADCS_RESET_BOOT_REGI_ST				109
#define ADCS_SAVE_CONFIG_ST					110
#define ADCS_SAVE_TLE_ST					111
#define ADCS_SET_UNIX_TIME_ST				112
#define ADCS_SET_CACHE_STATE_ST				113
#define ADCS_DEPLOY_BOOM_ST					114
#define ADCS_RUN_MODE_ST					115
#define ADCS_MAGNETORQUER_SET_CONFIG_ST					117
#define ADCS_MAGNETOMETER_MOUNT_ST			118
#define ADCS_MAGNETOMETER_OFF_SET_ST			119
#define ADCS_MAGNETOMETER_SENSITIVITY_ST		120
#define	ADCS_DETUMBLING_CONTROL_ST			121
#define ADCS_WHEEL_PARAM_ST					122
#define ADCS_WHEEL_GAIN_ST					123
#define ADCS_MOMENT_OFF_INERTIA_ST			124
#define ADCS_PRODUCTS_INERTIA_ST			125
#define ADCS_SET_ESTIM_PARAM1_ST			126
#define ADCS_SET_ESTIM_PARAM2_ST			127
#define ADCS_SET_STAGE_TABLE_ST				128
#define ADCS_SET_DELAY_ST					129
#define ADCS_SET_CM_ST						130
#define ADCS_SET_POWER_ST					131
#define ADCS_SET_EM_ST						132
#define ADCS_SET_TM_ST						133
#define ADCS_SET_CM_ST						130
//sw
#define RESET_DELAYED_CM_LIST_ST 	32
#define RESET_APRS_LIST_ST			15
#define RESET_FRAM_ST				42

//special operations
#define DELETE_UNF_CUF_ST		156
#define UPLOAD_CUF_ST			157
#define CON_UNF_CUF_ST			158
#define PAUSE_UP_CUF_ST			159
#define EXECUTE_CUF				160
#define REVERT_CUF				161
#endif /* SPLTYPES_H_ */
