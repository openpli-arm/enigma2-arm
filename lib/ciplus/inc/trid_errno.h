
#ifndef _TRID_ERRNO_H_
#define _TRID_ERRNO_H_
#if defined __cplusplus || defined __cplusplus__
extern "C" {
#endif

#include <lib/ciplus/inc/trid_datatype.h>

typedef trid_sint32 RETURN_TYPE;
#define	SYS_NOERROR				0
#define	SYS_FAILED				1
#define	SYS_NULL_POINTER		2
#define	SYS_SMALL_BUFFER		3
#define	SYS_NO_MEMORY			4
#define	SYS_BAD_PARAM			5

//Demux: 0x2000 ~ 0x4000 
/*******************************************************************************
* Demux Error definition
*******************************************************************************/
#define	EID_DmxCommError			10000

#define	EID_DmxApiError 			10001
#define	EID_DmxApiInvalidParam  	10002
#define	EID_DmxInvalidDevice  		10003
#define	EID_DmxDeviceInit     		10004
#define	EID_DmxDeviceNotReady		10005
#define	EID_DmxBuffNotAvailable   	10006
#define	EID_DmxHandleERROR			10007
#define	EID_DmxFilterNotAvailable   10008
#define	EID_DmxDeviceIDErr          10009
//IO
#define EID_DmxIOError              10010  
//PES Filter
#define	EID_DmxPESErrStart          10020
#define	EID_DmxPESErrStop           10021
//Section Filter
#define	EID_DmxSecErrInit           10030
#define	EID_DmxSecErrStart          10031
#define	EID_DmxSecNotAllocate		10032
#define	EID_DmxSecNotStart			10033 
#define	EID_DmxSecErrStop           10034
#define	EID_DmxSecInUse				10035
#define	EID_DmxSecNoFree			10036
#define	EID_DmxSecErrParam			10037
#define	EID_DmxSecErrState          10038
#define	EID_DmxErrSectionPattern    10039
#define	EID_DmxErrSectionFilter     10040
//TS Filter
#define	EID_DmxMainTSErrStart       10050
#define	EID_DmxPVRErrStart          10051
#define	EID_DmxMinorTSErrStart      10052
#define	EID_DmxTSErrStop            10053
//Debug IO
#define	EID_DmxSVESOutErrStart      10060
#define	EID_DmxSVESOutErrStop       10061
#define	EID_DmxSVESInErrStart       10062
#define	EID_DmxSVESInErrStop        10063
#define	EID_DmxPVRInErrStart        10064
#define	EID_DmxPVRInErrStop         10065
//Data Buffer
#define	EID_DmxInputBufNotAvailable  10070
#define	EID_DmxSVESInBufNotAvailable 10071
#define	EID_DmxInputQueueFull        10072
#define	EID_DmxSVESInputQueueFull    10073
#define	EID_DmxNoData                10074
#define	EID_DmxNoSecData             10075
//DVD
#define	EID_DmxDVDErrSetDecss        10080
#define	EID_DmxDVDErrKeyNum          10081
#define	EID_DmxDVDErrSetKey          10082
#define	EID_DmxDVDErrSetDiscMode     10083
// DES
#define	EID_DmxDesErrSetMode         10090
#define	EID_DmxDesNotAvailable       10091
#define	EID_DmxDesErrStart           10092
#define	EID_DmxDesErrStop            10093
#define	EID_DmxDesErrSetKey          10094
//PTS
#define	EID_DmxPTSErrGet             10100 
#define	EID_DmxPTSErrGetRp           10101
#define	EID_DmxPTSErrGetWp           10102


//PSIP: 0x4001 ~ 0x6000
#define PSIP_STT_EMPTY			0x4001
#define PSIP_STT_NO_MEMORY		0x4002
#define PSIP_STT_NO_TIME		0x4003
#define PSIP_STT_TOO_SHORT		0x4004
#define PSIP_STT_PARSED			0x4005
#define PSIP_STT_BAD_BUF        0x4006
#define PSIP_STT_BAD_HEAD       0x4007
#define PSIP_STT_PACK_NEXT		0x4008
#define PSIP_STT_BAD_PROT		0x4009
#define PSIP_STT_SCAN_TIMEOUT	0x4010
#define PSIP_STT_OOB_BAD_HEAD	0x4011
#define PSIP_STT_OOB_TOO_SHORT  0x4012

#define PSIP_ETT_NO_MEMORY		0x4100
#define PSIP_ETT_TOO_SHORT		0x4101
#define PSIP_ETT_PARSED			0x4102
#define PSIP_ETT_BAD_HEAD		0x4103
#define PSIP_ETT_TIME_OUT		0x4104
#define PSIP_ETT_NO_INFO		0x4105
#define PSIP_ETT_BAD_LENGTH		0x4106	
#define PSIP_ETT_NEXT_INDICATOR	0x4107
#define PSIP_ETT_PROTO_VERSION	0x4108
#define PSIP_ETT_NOT_READY		0x4109


#define PSIP_RRT_EMPTY			0x4200
#define PSIP_RRT_NO_MEMORY		0x4201
#define PSIP_RRT_TOO_SHORT		0x4202
#define PSIP_RRT_PARSED			0x4203
#define PSIP_RRT_BAD_REGION		0x4204
#define PSIP_RRT_NO_SPACE		0x4205
#define PSIP_RRT_BAD_BUFFER		0x4206
#define PSIP_RRT_BAD_HEAD		0x4207
#define PSIP_RRT_TIME_OUT		0x4208
#define PSIP_RRT_PARSE_ERROR	0x4209
#define PSIP_RRT_NEXT_INDICATOR	0x4210
#define PSIP_RRT_PRO_VERSION	0x4211
#define PSIP_RRT_NO_INFO		0x4212
#define PSIP_RRT_ONCE_ERROR		0x4213

#define PSIP_EIT_TOO_SHORT		0x4300
#define PSIP_EIT_NO_MEMORY		0x4301
#define PSIP_EIT_PARSED			0x4302
#define PSIP_EIT_EMPTY			0x4303
#define PSIP_EIT_CONT_SHORT		0x4304
#define PSIP_EIT_CONT_EMPTY		0x4305
#define PSIP_EIT_CAPT_SHORT		0x4306
#define PSIP_EIT_HAS_CAPT		0x4307
#define PSIP_EIT_ZERO_CAPT		0x4308
#define PSIP_EIT_CC_NO_ROOM		0x4309
#define PSIP_EIT_DESC_NULL		0x4310
#define PSIP_EIT_NO_EVENT		0x4311
#define PSIP_EIT_NO_INFO		0x4312
#define PSIP_EIT_BAD_HEAD       0x4313
#define PSIP_EIT_BAD_DATAFILE   0x4314
#define PSIP_EIT_MMAP_FAIL      0x4315
#define PSIP_TVCT_WFILE_FAIL 	0x4316
#define PSIP_EIT_NO_EVENT_MEM   0x4317
#define PSIP_EIT_NO_TS_ID		0x4318
#define PSIP_EIT_NO_CHAN_SPACE	0x4319
#define PSIP_EIT_BAD_LENGTH		0x4320
#define PSIP_EIT_NEXT_INDICATOR	0x4321
#define PSIP_EIT_PROTO_VERSION	0x4322
#define PSIP_EIT_NO_CHANNEL		0x4323
#define PSIP_EIT_CLEAR_FAIL		0x4324
#define PSIP_EIT_BAD_SID		0x4325
#define PSIP_EIT_BAD_SYNTAX		0x4326
#define PSIP_EIT_ZERO_EVENT		0x4327
#define PSIP_BAD_EVENT_TYPE		0x4328


#define PSIP_TVCT_SOURCE_ERROR	0x4403
#define PSIP_GET_PATNUM_ERROR	0x4404
#define PSIP_SET_PATNUM_ERROR	0x4405
#define PSIP_TVCT_INDIC_ERROR	0x4406
#define PSIP_TVCT_PROTO_ERROR	0x4407
#define PSIP_TVCT_NO_PPLACE		0x4408
#define PSIP_TVCT_NO_PLACE		0x4409
#define PSIP_TVCT_TOO_SHORT		0x4410
#define PSIP_TVCT_NO_MEMORY		0x4411
#define PSIP_TVCT_PARSED		0x4412
#define PSIP_TVCT_EMPTY			0x4413
#define PSIP_TVCT_SERV_SHORT    0x4414
#define PSIP_TVCT_HAS_AUDIO		0x4415
#define PSIP_TVCT_DESC_NULL		0x4416
#define PSIP_TVCT_BAD_TS_ID		0x4417
#define PSIP_TVCT_BAD_BUFFER	0x4418
#define PSIP_TVCT_NO_INFO       0x4419
#define PSIP_TVCT_BAD_HEAD      0x4420
#define PSIP_TVCT_NO_PCSPACE    0x4421
#define PSIP_TVCT_SET_TSID		0x4422
#define PSIP_TVCT_NO_TSID		0x4423
#define PSIP_TVCT_STOP_ERROR	0x4424	
#define PSIP_TVCT_COUNT_PTR		0x4425
#define PSIP_TVCT_SET_CHAN		0x4426
#define PSIP_TVCT_GET_CHAN		0x4427
#define PSIP_TVCT_TIME_OUT		0x4428

#define PSIP_MGT_TOO_SHORT		0x4500
#define PSIP_MGT_NO_MEMORY		0x4501
#define PSIP_MGT_PARSED			0x4502
#define PSIP_MGT_EMPTY			0x4503
#define PSIP_MGT_ZERO_ENTRY		0x4504
#define PSIP_MGT_NO_INFO		0x4505
#define PSIP_MGT_SLOW_TABLE		0x4506
#define PSIP_MGT_FAST_TABLE		0x4507
#define PSIP_MGT_BAD_HEAD		0x4508
#define PSIP_MGT_NEXT_INDICATOR	0x4509 
#define PSIP_MGT_PROTO_VERSION	0x4510
#define PSIP_PROCESS_MGT_ERROR	0x4511
#define PSIP_MGT_STOP_FAIL		0x4512


#define PSIP_PAT_NO_DATA		0x4600
#define PSIP_PAT_NO_NUMBER		0x4601
#define PSIP_PAT_TOO_SHORT		0x4602
#define PSIP_PAT_NO_MEMORY		0x4603
#define PSIP_PAT_NO_PHYCHAN		0x4604
#define PSIP_PAT_EMPTY			0x4605
#define PSIP_PAT_PACK_ERROR		0x4606
#define PSIP_PAT_TIME_OUT		0x4607
#define PSIP_START_PMT_ERROR	0x4608
#define PSIP_START_PAT_ERROR	0x4609
#define PSIP_PAT_BAD_TABLE_ID	0x4610


#define PSIP_CAT_TOO_SHORT		0x4700
#define PSIP_CAT_NO_MEMORY		0x4701
#define PSIP_CAT_PARSED			0x4702
#define PSIP_CAT_EMPTY			0x4703
#define PSIP_CAT_PACK_ERROR		0x4704

#define PSIP_PMT_TOO_SHORT		0x4800
#define PSIP_PMT_NO_MEMORY		0x4801
#define PSIP_PMT_PARSED			0x4802
#define PSIP_PMT_EMPTY			0x4803
#define PSIP_PMT_PACK_ERROR		0x4804
#define PSIP_PMT_NULL_PTR		0x4805

#define PSIP_NO_STRING			0x4900
#define PSIP_TOO_SHORT_STRING	0x4901
#define PSIP_SCAN_SET_EIT_ERROR	0x4902
#define PSIP_CETT_TIME_OUT		0x4903
#define PSIP_SCAN_SET_CETT		0x4904
#define PSIP_LONG_STRING_OLD	0x4905
#define PSIP_SHORT_STRING_OLD	0x4906	
#define PSIP_NO_PAT_TSID		0x4907
#define PSIP_SET_ANALOG_FAIL	0x4908
#define PSIP_SET_TSID_FAIL		0x4909
#define PSIP_STRING_ERROR		0x4910

#define PSIP_EVENT_HAS_RATE		0x4a00
#define PSIP_EVENT_LIST_NULL	0x4a01
#define PSIP_EVENT_PTR_NULL		0x4a02
#define PSIP_WRONG_TS_ID		0x4a03

#define PSIP_CHAN_LIST_NULL		0x4b00
#define PSIP_MULTI_STRING_NULL	0x4b01
#define PSIP_NO_CHAN_SPACE		0x4b02
#define PSIP_CHAN_ARRAY_NULL	0x4b04
#define PSIP_PHY_CHAN_NULL		0x4b05
#define PSIP_NOTHING_PRINT		0x4b06
#define PSIP_SCANPMT_NO_TSID	0x4b07
#define PSIP_SCAN_CHAN_FAIL		0x4b08
#define PSIP_CLEAR_NO_TSID		0x4b09
#define PSIP_CLEAR_PTR_NULL		0x4b10	
#define PSIP_CLEAR_NOTHING		0x4b11
#define PSIP_CHANGE_ERROR		0x4b12
#define PSIP_SCAN_CCHAN_FAIL	0x4b13
#define PSIP_NO_CCHAN_SPACE		0x4b14
#define PSIP_SMALL_AUDIO_SPACE	0x4b15
#define PSIP_NOT_SUPPORT		0x4b16
#define PSIP_NO_START_FILTER	0x4b17
#define PSIP_DELETE_ARI_FAIL	0x4b18
#define PSIP_DELETE_CABLE_FAIL	0x4b19
#define PSIP_SET_CABLE_FAIL		0x4b20
#define PSIP_SET_AIR_FAIL		0x4b21
#define PSIP_FIND_CABLE_FAIL	0x4b22
#define PSIP_CANCEL_SCAN_FAIL	0x4b23
#define PSIP_NO_START_SCAN		0x4b24
#define PSIP_CANCEL_SCAN_CHAN 	0x4b25
#define PSIP_NO_DESC_ROOM		0x4b26
#define PSIP_NO_SUCH_DESC		0x4b27

#define PAT_TOO_SHORT			0x4b30
#define PAT_NO_MEMORY			0x4b31
#define PAT_PARSED				0x4b32
#define PAT_EMPTY		        0x4b33
#define PAT_PACK_ERROR			0x4b34
#define PAT_NO_ROOM				0x4b35
#define PAT_NO_PROGRAM			0x4b36
#define PAT_PMT_PID_ERROR		0x4b37
#define PAT_NO_CA_INFO			0x4b38
#define PAT_BAD_BUFFER			0x4b39


#define CAT_TOO_SHORT			0x4c00
#define CAT_NO_MEMORY			0x4c01
#define CAT_PARSED				0x4c02
#define CAT_EMPTY				0x4c03
#define CAT_PACK_ERROR			0x4c04
#define CAT_NO_EMM_INFO			0x4c05


#define PMT_TOO_SHORT			0x4d00
#define PMT_NO_MEMORY			0x4d01
#define PMT_PARSED				0x4d02
#define PMT_PROG_EMPTY			0x4d03
#define PMT_PACK_NEXT			0x4d04
#define PMT_NO_PAT				0x4d05
#define PMT_ES_EMPTY			0x4d06
#define PMT_TIME_OUT			0x4d07
#define PMT_PARSE_ERROR			0x4d08
#define PMT_FILTER_ERROR		0x4d09
#define PMT_BAD_TABLE_ID		0x4d10

#define PSIP_GET_SOURCE_ALL		0x4e02
#define PSIP_BAD_PARAMETER		0x4e03
#define PSIP_CHECK_EIT_DONE		0x4e04
#define PSIP_THREAD_ERROR		0x4e05
#define PSIP_COMBINE_ERROR		0x4e06
#define PSIP_REQUEST_ERROR		0x4e07
#define PSIP_SEMAPHORE_ERROR	0x4e08
#define PSIP_FILTERS_OVERFLOW	0x4e09
#define PSIP_FILTERS_REQ_ERROR	0x4e10
#define PSIP_BAD_CRC_CHECKING	0x4e11
#define PSIP_FIRST_CHAN_ERROR	0x4e12
#define PSIP_BAD_CHAN_UP		0x4e13
#define PSIP_BAD_CHAN_DOWN		0x4e14
#define PSIP_PARSER_STOP_ERROR	0x4e15
#define PSIP_STOP_ERROR			0x4e16
#define PSIP_FILE_ERROR			0x4e17
#define PSIP_POINTER_NULL		0x4e18
#define PSIP_NO_AUDIO_PID		0x4e19
#define PSIP_BAD_FILTER_ID		0x4e20
#define PSIP_OPEN_FILE_ERROR	0x4e21
#define PSIP_CLOSE_FILE_ERROR	0x4e22
#define PSIP_CHANGE_CHAN_FAIL	0x4e23
#define PSIP_NO_CHAN_TSID		0x4e24
#define PSIP_BAD_TSID_PTR		0x4e25
#define PSIP_LIB_POINTER_NULL	0x4e26
#define PSIP_LIB_NEED_EROOM		0x4e27
#define PSIP_OUT_OF_BOUNDARY	0x4e28

#define PSIP_EVENT_SORT_ERROR   0x4f00
#define PSIP_FREE_OLD_ERROR     0x4f01
#define PSIP_COMP_PTR_ERROR     0x4f02
#define PSIP_ALL_EVENTS_ERROR	0x4f03
#define PSIP_SORT_EVENT_ERROR	0x4f04
#define PSIP_NEED_CHAN_ROOM		0x4f05
#define PSIP_START_EPG_FAIL		0x4f06
#define PSIP_SCAN_MGT_FAIL		0x4f07
#define PSIP_EPG_STOP_FAIL		0x4f08
#define PSIP_EPG_MGT_FAIL		0x4f09
#define PSIP_PHY_INFO_FAIL		0x4f10
#define PSIP_NO_PHY_INFO		0x4f11
#define PSIP_SCAN_EIT_FAIL		0x4f12
#define PSIP_SCAN_EIT_TIMEOUT	0x4f13
#define PSIP_PAT_NO_INFO		0x4f14
#define PSIP_BAD_PHY_NUM		0x4f15


#define PSIP_BANNER_NO_TIME		0x5000
#define PSIP_BANNER_NO_MGT		0x5001
#define PSIP_BANNER_NO_EIT		0x5002
#define PSIP_EVENT_SEARCH_FAIL	0x5003
#define PSIP_BANNER_RATING_FAIL	0x5004
#define PSIP_BANNER_SEARCH_FAIL	0x5005
#define PSIP_BANNER_NO_RATING	0x5006
#define PSIP_BANNER_NO_CC		0x5007
#define PSIP_EPG_EVENT_FAIL		0x5008
#define PSIP_EVENT_LOOK_FAIL	0x5009
#define PSIP_EVENT_BAD_TIME		0x5010
#define PSIP_GET_EVENT_FAIL		0x5011
#define PSIP_CHECK_ETT_FAIL		0x5012
#define PSIP_BAD_TUNER_MODE		0x5013


#define PSIP_CVCT_BAD_HEAD		0x5100
#define PSIP_CVCT_NO_PLACE		0x5101
#define PSIP_CVCT_TOO_SHORT		0x5102
#define PSIP_CVCT_NO_MEMORY		0x5103
#define PSIP_CVCT_PARSED		0x5104
#define PSIP_CVCT_EMPTY			0x5105
#define PSIP_CVCT_SERV_SHORT    0x5106
#define PSIP_CVCT_HAS_AUDIO		0x5107
#define PSIP_CVCT_DESC_NULL		0x5108
#define PSIP_CVCT_BAD_TS_ID		0x5109
#define PSIP_CVCT_BAD_BUFFER	0x5110
#define PSIP_CVCT_NO_INFO       0x5111
#define PSIP_CVCT_NO_PCSPACE    0x5112
#define PSIP_CVCT_SET_TSID		0x5113
#define PSIP_CVCT_NO_TSID		0x5114
#define PSIP_CVCT_STOP_ERROR	0x5115	
#define PSIP_CVCT_COUNT_PTR		0x5116
#define PSIP_CVCT_SET_CHAN		0x5117
#define PSIP_CVCT_GET_CHAN		0x5118
#define PSIP_CVCT_INDIC_ERROR	0x5119
#define PSIP_CVCT_PROTO_ERROR	0x5120

#define PSIP_EAS_BAD_TABLE_ID	0x5200
#define EAS_TOO_SHORT           0x5201
#define EAS_DISCARD             0x5202
#define PSIP_CHECK_EAS_FAIL		0x5203	
//DemuxSec: 0x6001 ~ 0x8000

//Audio AC3: 0x8001~ 0x10000
 
/*******************************************************************************
* AC3 Error definition
*******************************************************************************/
#define AUDIO_DECODER_ERROR			11500
#define AUDIO_MANAGER_ERROR			11501
#define AUDIO_ApiInvalidDevice		11502
#define AUDIO_ApiInvalidParam		11503
#define AUDIO_ApiDeviceInUse		11504
#define AUDIO_ApiDeviceNotReady		11505
#define AUDIO_ApiSystemError		11506
#define AUDIO_ApiInvalidState		11507


//Audio AAC:0x10000 ~ 0x12000
	
//AVCore:0x12000 ~ 0x14000
	
//Mpeg: 0x14001 ~ 0x17000
/*******************************************************************************
* MPEG Video Error definition
*******************************************************************************/
#define	MPEG_INITERROR				10500
#define	MPEG_ApiOK					10505
#define	MPEG_ApiError				10506
#define	MPEG_ApiInvalidDevice		10507
#define	MPEG_ApiInvalidParam		10508
#define	MPEG_ApiDeviceInuse			10509
#define	MPEG_ApiDeviceInit			10510
#define	MPEG_ApiDeviceNotReady		10511
#define	MPEG_ApiNoFilterAvailable	10512



//HiDTV_GPIO:0x17001 ~ 0x17400
	
//HiDTV_RTC: 0x17401 ~ 0x17800
	
//SoftI2C: 0x17801 ~ 0x18000 

//VChip: 0x18001 ~ 0x18200

//SI: 0x18201 ~ 0x19800
	
//TvtoolKit: 0x19801 ~ 0x21000
#define TTK_ERRNO_BASE 			(0x19800)
#define TTK_VDL_CALL_FAIL		(TTK_ERRNO_BASE+1)
#define TTK_CC_CALL_FAIL		(TTK_ERRNO_BASE+3)
#define TTK_PSIP_CALL_FAIL		(TTK_ERRNO_BASE+4)

#define TTK_INVALID_CHAN		(TTK_ERRNO_BASE+5)
#define TTK_NOT_NORMAL_CHAN		(TTK_ERRNO_BASE+6)
#define TTK_NOT_ATV_CHAN		(TTK_ERRNO_BASE+7)
#define TTK_NOT_DTV_CHAN		(TTK_ERRNO_BASE+8)
#define TTK_NOT_TV_CHAN			(TTK_ERRNO_BASE+9)
#define TTK_BAD_CUR_STT			(TTK_ERRNO_BASE+10)
#define TTK_NOT_ATV_WIN			(TTK_ERRNO_BASE+11)
#define TTK_NOT_DTV_WIN			(TTK_ERRNO_BASE+12)
#define TTK_NOT_TV_WIN			(TTK_ERRNO_BASE+13)

#define TTK_RATING_INVALID		(TTK_ERRNO_BASE+14)
#define TTK_NO_RATING			(TTK_ERRNO_BASE+15)
#define TTK_VCHIP_NOT_SUPPORT (TTK_ERRNO_BASE+16)
#define TTK_INPUT_NOT_SUPPORT	(TTK_ERRNO_BASE+17)
#define TTK_INPUT_DISABLED		(TTK_ERRNO_BASE+18)

#define TTK_BAD_CHANTYPE		(TTK_ERRNO_BASE+19)
#define TTK_NOT_SUPPORT			(TTK_ERRNO_BASE+20)
#define TTK_CHANTYPE_MISMATCH	(TTK_ERRNO_BASE+21)

#define TTK_BAD_BG_ID			(TTK_ERRNO_BASE+22)
//Utility: 0x21000 ~ 0x23000

//Display: 0x23000 ~ 0x30000
/*******************************************************************************
* Display Error definition
*******************************************************************************/
#define DISP_DEVBUSY			0x8100
#define DISP_DEVOPENERR			0x8101
#define DISP_DEVOPERERR			0x8102
#define DISP_DEVSUROPENERR		0x8103
#define DISP_DEVSURCLOSEERR		0x8104
#define DISP_GETPALETTEERR		0x8105
#define DISP_SETPALETTEERR		0x8106
#define DISP_HSSETERR			0x8107
#define DISP_UNKNOWNSIGNAL		0x8108
#define DISP_OPINVALID			0x8109
#define DISP_MULTIWINERR		0x810A



//VDL: 0x30000 ~ 0x33000
#define VDL_ERRNO_BASE 			(0x30000)
#define VDL_NOT_SUPPORT			(VDL_ERRNO_BASE + 1)
#define VDL_TUNER_INIT_FAIL		(VDL_ERRNO_BASE + 2)
#define VDL_DRIVER_CALL_FAIL	(VDL_ERRNO_BASE + 3)
#define VDL_AUDIO_NOT_READY		(VDL_ERRNO_BASE + 4)

//TGE: 0x33000 ~ 0x43000


/*******************************************************************************
* Register Map Error definition
*******************************************************************************/
#define REG_MAPFAIL      		0x8000			//start from 32768
#define REG_UNMAPFAIL    		0x8001
#define REG_DEVFAIL      		0x8002


/*******************************************************************************
* CC Error definition
*******************************************************************************/
#define DTVCC_POINTER_NULL		11800
#define DTVCC_DATA_ERROR		11801
#define DTVCC_LENGTH_ERROR		11802
#define DTVCC_BUFFER_FULL		11803
#define DTVCC_ApiError			11804
#define DTVCC_ApiInvalidModule	11805
#define DTVCC_ApiModuleInit		11806
#define DTVCC_ANCHOR_INVALID    11807
#define DTVCC_INVALID_PARAMETER 11808


#if defined __cplusplus || defined __cplusplus__
}
#endif
#endif

