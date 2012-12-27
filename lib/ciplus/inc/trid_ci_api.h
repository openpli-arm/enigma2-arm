/****************************************************************************/
/*                                                                          */
/*           Copyright (c) 2003 Trident Technologies, Inc.                  */
/*                       All rights reserved                                */
/*                                                                          */
/* The content of this file or document is CONFIDENTIAL and PROPRIETARY     */
/* to Trident Microsystems, Inc.  It is subject to the terms of a           */
/* License Agreement between Licensee and Trident Technologies, Inc.        */
/* restricting among other things, the use, reproduction, distribution      */
/* and transfer.  Each of the embodiments, including this information and   */
/* any derivative work shall retain this copyright notice                   */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/* File:            trid_ciapi.h                                            */
/*                                                                          */
/* Purpose:         Header File for CI API interface                        */
/*                                                                          */
/* Copyright (C) 2004-2007, Trident Technologies, Inc.                      */
/*                                                                          */
/* modification history                                                     */
/*  --------------------                                                    */
/* 001. 11/18/2006 calvin create.                                           */
/* 002.  May.2008   Taolin transfer to new Trident style                    */                         
/****************************************************************************/


#ifndef _TRID_DVB_CI_API_H_
#define _TRID_DVB_CI_API_H_

#if defined __cplusplus || defined __cplusplus__
extern "C" {
#endif

#include <lib/ciplus/inc/trid_datatype.h>
#include <lib/ciplus/inc/trid_errno.h>
#include <lib/ciplus/inc/trid_ci_types.h>

//#include    "dvbci.h"

int cnxt_kal_initialize ( void );
int cnxt_dvbci_drv_init(void);
int cnxt_kal_terminate(void);

void cnxt_kal_trace(unsigned int uFlags, char *string, ...);

/******************************************************************************
 * Trid_CI_Set_Pcmcia_Func                                                    *
 *    to set pcmcia low level functions                                       *
 * Arguments:                                                                 *
 *    phys_funcs:                                                             *
 *    		IN, function which will be called when card communicates with host  *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    should be called before Trid_CI_Start below being called.              *
 ******************************************************************************/
RETURN_TYPE Trid_CI_Set_Pcmcia_Func(CI_phys_driver *phys_funcs);

/******************************************************************************
 * Trid_CI_Start                                                              *
 *    to initialize the whole CI stack                                        *
 * Arguments:                                                                 *
 *    CardStatusChangeNotifyCallback:                                         *
 *    		IN, callback function which will be called when card status       *
 *    		changes in order to notify the upper layer. Its parameter is      *
 *    		the card status.                                                  *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    should be called before any other APIs below being called.              *
 ******************************************************************************/
RETURN_TYPE Trid_CI_Start(trid_sint32 (*CardStatusChangeNotifyCallback)(Trid_CI_CardStatus_t status),
			  trid_sint32 (*CardGetHostLanguageCountryCallback)(trid_uint8 language_code[3], trid_uint8 country_code[3]),
			  trid_sint32 (*CardGetHostAVPIDCallback)(trid_uint16 *AudioPID, trid_uint16 *VideoPID),
			  trid_sint32 (*CardTuneChannelCallback)(trid_uint16 network_id, trid_uint16 orignal_network_id, trid_uint16 ts_id, trid_uint16 service_id),
	          void (*CardSetDesKeyCallback)(trid_uint8 key[8],trid_uint8 odd_even),
	          void (*CardSetAesKeyCallback)(trid_uint8 key[16],trid_uint8 iv[16],trid_uint8 odd_even)
);

/******************************************************************************
 * Trid_CI_GetCardStatus                                                      *
 *    to get the card status at any time.                                     *
 * Arguments:                                                                 *
 *    1. status: OUT, pointer to where the returned card status to be stored. *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *                                                                            *
 ******************************************************************************/
RETURN_TYPE Trid_CI_GetCardStatus(Trid_CI_CardStatus_t *status);

/******************************************************************************
 * Trid_CI_SendCAPmt                                                          *
 * Argument:                                                                  *
 *    1. pmt_data: IN, points to a buffer where PMT data is stored.           *
 *    2. pmt_length: IN, equals to the length of PMT data.                    *
 *    3. ca_system_id_match: OUT, value returned indicating whether the       *
 *       ca_system_id in the PMT data matches the current CI module,          *
 *       0: match and -1: mismatch                                            *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 * 	  this API converts the raw PMT data into CA_PMT format and then send     *
 * 	  CA_PMT data to CI card.                                                 *
 ******************************************************************************/
RETURN_TYPE Trid_CI_SendCAPmt(trid_uint8* pmt_data, trid_sint32 pmt_length, trid_sint32* ca_system_id_match);

/******************************************************************************
 * Trid_CI_GetCASystemId                                                      *
 * Argument:                                                                  *
 *    1. ca_system_id: OUT, points to a buffer where ca_system_id is stored.  *
 *    2. num: OUT, number of ca_system_id returned.                           *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 * 	  get the ca_system_ids that the current CI module can support.           *
 ******************************************************************************/
RETURN_TYPE Trid_CI_GetCASystemId(trid_uint16 ca_system_id[], trid_sint32* num);

/******************************************************************************
 * Trid_CI_AppInfo_GetMenuStr                                                 *
 * Argument:                                                                  *
 *    1. menu_str: INOUT, points to a buffer where menu string is stored.     *
 *    2. menu_str_len: INOUT, points to a variable where menu string length   *
 *                     is stored.                                             *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    To get the Menu String of the current CI card                           *
 ******************************************************************************/
RETURN_TYPE Trid_CI_AppInfo_GetMenuStr(trid_uint8 menu_str[],trid_uint8 *menu_str_len);

/******************************************************************************
 * Trid_CI_AppInfo_EnterMenu                                                  *
 * Argument:                                                                  *
 *    None.                                                                   *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 * 	  To send an enter menu packet to CI card, which is the entry point to    *
 * 	  open MMI menu of the CI module.                                         *
 ******************************************************************************/
RETURN_TYPE Trid_CI_AppInfo_EnterMenu(trid_void);

/******************************************************************************
 * Trid_CI_MMI_SendCloseMMI                                                   *
 * Argument:                                                                  *
 *    None.                                                                   *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    to close MMI menu and exit to the root CI menu                          *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MMI_SendCloseMMI(trid_void);


/******************************************************************************
 * Trid_CI_MMI_RegisterMenuDataNotify                                         *
 * Argument:                                                                  *
 *    1. callback: IN, callback function which will be called when MMI menu   *
 *       data is received from CI module in order to notify upper layer. Its  *
 *       parameter indicates the menu context which is of type T_Menu         *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    once the CI lib recieves MMI menu data from CI module, it will call the *
 *    callback function (registered with this API) to notify the upperlayer(  *
 *    usually OSD layer). Then upper layer can draw MMI Menu accordingly.     *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MMI_RegisterMenuDataNotify(trid_sint32 (*callback)(Trid_T_Menu* menu));

/******************************************************************************
 * Trid_CI_MMI_RegisterListDataNotify                                         *
 * Argument:                                                                  *
 *    1. callback: IN, callback function which will be called when MMI List   *
 *       data is received from CI module in order to notify upper layer. Its  *
 *       parameter indicates the list context which is of type T_List.        *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    once the CI lib receives MMI list data from CI module, it will call the *
 *    callback function (registered with this API) to notify the upperlayer ( *
 *    usually OSD layer). Then upper layer can draw MMI List accordingly.     *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MMI_RegisterListDataNotify(trid_sint32 (*callback)(Trid_T_List* list));

/******************************************************************************
 * Trid_CI_MMI_RegisterEnqDataNotify                                          *
 * Argument:                                                                  *
 *     1. callback: IN, callback function which will be called when MMI Enq   *
 *        data is received from CI module in order to notify upper layer. Its *
 *        parameter indicate the Enq context which is of type T_Enq.          *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *     once the CI lib receives MMI Enq data from CI module, it will call the *
 *     callback function (registered with this API) to notify the upper layer *
 *     (usually OSD layer). Then upper layer can draw MMI Enq accordingly.    *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MMI_RegisterEnqDataNotify(trid_sint32 (*callback)(Trid_T_Enq* enq));

/******************************************************************************
 * Trid_CI_MMI_RegisterCloseMMINotify                                         *
 * Argument:                                                                  *
 *     1. callback: IN, callback function which will be called when MMI is    *
 *        closed due to the request from CI card or card reset because of IO  *
 *        error or card remove, etc.                                          *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *     once the upper layer (e.g: OSD) receives the CloseMMINotify from CI    *
 *     lib, the upper layer knows the MMI sub menus exit and should return    *
 *     to the CI root menu or other correct menu position.                    *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MMI_RegisterCloseMMINotify(trid_sint32 (*callback)());

/******************************************************************************
 * Trid_CI_MMI_SendMenuAnsw                                                   *
 * Argument:                                                                  *
 *    1. choice_ref: IN, the choice number indicates which menu item (user)   *
 *       selected.                                                            *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    to make the CI lib send out a menu answer packet to the CI module, in   *
 *    which the menu selection is told.                                       *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MMI_SendMenuAnsw(trid_uint8 choice_ref);

/******************************************************************************
 * Trid_CI_MMI_SendListAnsw                                                   *
 * Argument:                                                                  *
 *    1. choice_ref: IN, the choice number incicates which list item (user)   *
 *       selected.                                                            *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    to make the CI lib send out a list answer packet to the CI module, in   *
 *    which the list selection is told.                                       *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MMI_SendListAnsw(trid_uint8 choice_ref);

/******************************************************************************
 * Trid_CI_MMI_SendEnqAnsw                                                    *
 * Argument:                                                                  *
 *    1. answ_id: IN, must be 0 or 1. 0 means cancel and 1 means answer.      *
 *    2. length: IN, length ot the text user input.                           *
 *    3. text: IN, point to the text user acutally input.                     *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    to make the CI lib send out an Enq Answer packet to the CI module, in   *
 *    which the user input is told.                                           *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MMI_SendEnqAnsw(trid_uint8 answ_id, trid_uint8 length, trid_uint8* text);


/*APIs below for MHEG engine */
/******************************************************************************
 * Trid_CI_MHEG_RegisterAppQuerySupport                                       *
 * Argument:                                                                  *
 *     1. callback: IN,                                                       *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *     Register a query callback function which CI+ Stack calls to determine  *
 *     whether specific application is currently supported by MHEG engine.    *
 *     The Callback functions take several parameters:                        *
 *         app_domain_id[]:   IN                                              *
 *         app_domain_id_len: IN                                              *
 *         init_object[]:     IN                                              *
 *         init_object_len:   IN                                              *
 *         *p_ack_code:       OUT                                             *
 *     the callback function shall return TRUE if App is supported and FALSE  *
 *     if otherwise, in this case, p_ack_code specifis the reason why not     *
 *     supported.                                                             *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MHEG_RegisterAppQuerySupport
(
    MHEGAppQuerySupportCallback_t callback
);

/******************************************************************************
 * Trid_CI_MHEG_RegisterFileAckNotify                                         *
 * Argument:                                                                  *
 *     1. callback: IN,                                                       *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    Register a callback function which CI+ Stack will call to whenever MHEG *
 *    engine requested file is ready from CI Module.                          *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MHEG_RegisterFileAckNotify
(
    MHEGFileAckNotifyCallback_t callback
);

/******************************************************************************
 * Trid_CI_MHEG_RegisterDataAckNotify                                         *
 * Argument:                                                                  *
 *     1. callback: IN,                                                       *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    Register a callback function which CI+ Stack will call to whenever MHEG *
 *    engine requested data is ready from CI Module.                          *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MHEG_RegisterDataAckNotify
(
    MHEGDataAckNotifyCallback_t callback
);


/******************************************************************************
 * Trid_CI_MHEG_RegisterAppAbortNotify                                        *
 * Argument:                                                                  *
 *     1. callback: IN,                                                       *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    Register a callback function which CI+ Stack will call to notify MHEG   *
 *    engine when it receives the App Abort Request from CI+ Module.          *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MHEG_RegisterAppAbortNotify
(
    MHEGAppAbortNotifyCallback_t callback
);

/******************************************************************************
 * Trid_CI_MHEG_RequestFile                                                   *
 * Argument:                                                                  *
 *     1. file_name: IN                                                       *
 *     2. file_name_len: IN                                                   *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    Request a specific file from CI+ Module                                 *
 *                                                                            *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MHEG_RequestFile
(
    trid_uint8  file_name[],
    trid_uint8  file_name_len
);

/******************************************************************************
 * Trid_CI_MHEG_RequestData                                                   *
 * Argument:                                                                  *
 *     1. data: IN                                                            *
 *     2. data_len: IN                                                        *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    Request Data from CI+ Module                                            *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MHEG_RequestData
(
    trid_uint8  data[],
    trid_uint32 data_len
);


/******************************************************************************
 * Trid_CI_MHEG_RequestAppAbort                                               *
 * Argument:                                                                  *
 *    None                                                                    *
 * Return:                                                                    *
 *    SYS_NOERROR for success and SYS_FAILED otherwise.                       *
 * Description:                                                               *
 *    Request to abort application from Host side.                            *
 ******************************************************************************/
RETURN_TYPE Trid_CI_MHEG_RequestAppAbort
(
    trid_void
);
#if defined __cplusplus || defined __cplusplus__
}
#endif

#endif


/******************************************************************************
 *                 End Of File                                                *
 ******************************************************************************/
