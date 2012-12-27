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
/* File:            trid_ci_types.h                                         */
/*                                                                          */
/* Purpose:         Head file for definitions of Types used in CI           */
/*                                                                          */
/* Copyright (C) 2004-2007, Trident Technologies, Inc.                      */
/*                                                                          */
/* modification history                                                     */
/*  --------------------                                                    */
/*                                                                          */
/****************************************************************************/


/********************************************************************************
 * trid_ci_types.h
 *
 * type definintions, for Common Interface Library 
 * 
 * Edition History:
 *
 * #    Date      Who     Comments
 * --- ---------- -----   -------------------------------------------------------
 * 001 11/18/2006 calvin  Created.
 * 002 05/20/2008 Taolin  updated for new trident style
 ********************************************************************************/

#ifndef _TRID_CI_TYPES_H_
#define _TRID_CI_TYPES_H_

#if defined __cplusplus || defined __cplusplus__
extern "C" {
#endif

#include <lib/ciplus/inc/trid_datatype.h>

#define TRID_MMI_TEXT_MAX_LENGTH	(255)
#define TRID_MMI_MAX_MENU_ITEMS  (50)

typedef enum _Trid_CI_CardStatus_t
{
    _TRID_CARD_REMOVE_ = 0,     /* card is removed from the slot, no card available */
    _TRID_CARD_INSERT_,         /* card is inserted inot the slot, but maybe not a valid CI card */
    _TRID_CARD_INVALID_CARD_,   /* card in the slot is not a valid CI card */
    _TRID_CARD_MMI_READY_,      /* card is ready for MMI menu access */
    _TRID_CARD_CA_READY_,       /* card is ready for CA access,eg: ready to accept CA_PMT*/
    _TRID_CARD_MMI_CA_READY_,   /* card in ready for both CA and MMI access */
    _TRID_CARD_UPGRADE_START_,
    _TRID_CARD_UPGRADE_FINISH_,
    _TRID_CARD_RESET_START_,
    _TRID_CARD_IO_ERROR_        /* error occurs while accessing the card */
}Trid_CI_CardStatus_t;

typedef struct _Trid_T_Text_
{
    trid_uint8 len;   /* total number of bytes in strings*/	
    trid_uint8 strings[TRID_MMI_TEXT_MAX_LENGTH];
}Trid_T_Text;

typedef struct _Trid_T_Menu_
{
    trid_uint8 choice_nb;
    Trid_T_Text title;
    Trid_T_Text sub_title;
    Trid_T_Text bottom;
    Trid_T_Text texts[TRID_MMI_MAX_MENU_ITEMS];
}Trid_T_Menu;

typedef struct _Trid_T_List_
{
    trid_uint8 item_nb;
    Trid_T_Text title;
    Trid_T_Text sub_title;
    Trid_T_Text bottom;
    Trid_T_Text texts[TRID_MMI_MAX_MENU_ITEMS];
}Trid_T_List;

typedef struct _Trid_T_Enq_
{
    trid_uint8 blind_answ;
    trid_uint8 answ_len;
    Trid_T_Text text;
}Trid_T_Enq;

/* below definitions are for MHEG engeine interface communications */

typedef enum
{
    _OK_                        = 1,
    _WRONG_API_                 = 2, 
    _API_BUSY_                  = 3,
    _Domain_Specific_API_BUSY_  = 0x80
}Trid_CI_MHEG_StartAckCode_e;

typedef trid_bool (*MHEGAppQuerySupportCallback_t)
(
    trid_uint8                  app_domain_id[],
    trid_uint8                  app_domain_id_len,
    trid_uint8                  init_object[],
    trid_uint8                  init_object_len,
    Trid_CI_MHEG_StartAckCode_e *p_ack_code
);

typedef void (*MHEGFileAckNotifyCallback_t)
(
    trid_uint8 file_name[], 
    trid_uint32  file_name_len,
    trid_uint8 file_data[], 
    trid_uint32 file_data_len
);

typedef void (*MHEGDataAckNotifyCallback_t)
(
    trid_uint8 data[], 
    trid_uint32 data_len
);

typedef void (*MHEGAppAbortNotifyCallback_t)(void);

struct _CI_phys_driver
{
    /*
        --------------- common fields -----------------
    */
	trid_sint32 (*pcmcia_init) ( void *pCfg );
	trid_sint32 (*pcmcia_term) ( void );
	trid_sint32 (*pcmcia_open) ( void    **pHandle,
										void 	 *pCaps,
										void *pNotifyFn,
										void				 *pUserData);
	trid_sint32 (*pcmcia_close) ( void *Handle );
	trid_sint32 (*pcmcia_module_init)( void* Handle );
	trid_sint32 (*pcmcia_register_read)( void *Handle,
												trid_sint32	   Reg,
												trid_uint8		   *pData,
												trid_uint16 		   NumBytes );
	trid_sint32 (*pcmcia_register_write)( void *Handle,
												 trid_sint32 	Reg,
												 trid_uint8 			*pData,
												 trid_uint16			NumBytes );
	trid_sint32 (*pcmcia_attribute_mem_read)( void *Handle,
													 trid_uint32		   AttribAddr,
													 trid_uint16		   NumBytes,
													 trid_uint8 		   *pData);
	trid_sint32 (*pcmcia_attribute_mem_write)( void *Handle,
													  trid_uint32			AttribAddr,
													  trid_uint16			NumBytes,
													  trid_uint8			*pData);
	trid_sint32 (*pcmcia_signal_get_state)( void *Handle,
												   trid_sint32 Signal,
												   trid_bool 			 *pbActive );
	trid_sint32 (*pcmcia_signal_set_state)( void *Handle,
												   trid_sint32 Signal,
												   trid_bool 			 bActive );
	trid_sint32 (*pcmcia_enable_ts)( void *Handle );
	trid_sint32 (*pcmcia_disable_ts)( void *Handle );
};

typedef struct _CI_phys_driver CI_phys_driver;

#if defined __cplusplus || defined __cplusplus__
}
#endif

#endif

/******************************************************************************
 *                      End Of File                                           *
 ******************************************************************************/
