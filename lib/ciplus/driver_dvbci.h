/****************************************************************************/
/*                   CONEXANT PROPRIETARY AND CONFIDENTIAL                  */
/*                       SOFTWARE FILE/MODULE HEADER                        */
/*                Copyright Conexant Systems Inc. 2004                      */
/*                         All Rights Reserved                              */
/****************************************************************************/
/*
 * Filename:        dvbci.h
 *
 *
 * Description:     Public header file for dvbci (Common Interface) driver
 *
 *
 * Author:          Steve Glennon
 *
 ****************************************************************************/
/* $Header: dvbci.h, 1, 2007-6-13 15:16:01, Jie Yu$
 * $Id: dvbci.h,v 1.0, 2007-06-13 07:16:00Z, Jie Yu$
 ****************************************************************************/

#ifndef _DVBCI_H_
#define _DVBCI_H_

#if defined __cplusplus || defined __cplusplus__
extern "C" {
#endif

#include "status_codes.h"
#include "basetype.h"
#include "trace_types.h"
/*-------------------------- HEADER FILE INCLUDES --------------------------*/
/*-------------------- Use of this section is discouraged ------------------*/

/*------------------------------ CONSTANTS ---------------------------------*/
#define TL_DVBCI_ERROR  (CNXT_TRACE_LEVEL_ALWAYS)
#define TL_DVBCI_FUNC   (CNXT_TRACE_LEVEL_3|CNXT_TRACE_DVBCI)
#define TL_DVBCI_INFO   (CNXT_TRACE_LEVEL_3|CNXT_TRACE_DVBCI)
#define TL_DVBCI_ISR    (CNXT_TRACE_LEVEL_1|CNXT_TRACE_DVBCI)

#define PLL_PIN_GPIO_MUX0_PIO_IO_REG       (1UL<<6)

/* Bit difinitions for DVB CI I/O Mode Control Register */
#define DVBCI_IO_CTRL_RSVD_80      0x80
#define DVBCI_IO_CTRL_RSVD_40      0x40
#define DVBCI_IO_CTRL_RSVD_20      0x20
#define DVBCI_IO_CTRL_RSVD_10      0x10
#define DVBCI_IO_CTRL_RS           0x08
#define DVBCI_IO_CTRL_SR           0x04
#define DVBCI_IO_CTRL_SW           0x02
#define DVBCI_IO_CTRL_HC           0x01
#define DVBCI_IO_CTRL_NULL         0x00

#define DVBCI_IO_CTRL_SR_CLEAR     0x00
#define DVBCI_IO_CTRL_SW_CLEAR     0x00

/* Bit difinitions for DVBCI I/O Mode Status Register */
#define DVBCI_IO_STAT_DA           0x80
#define DVBCI_IO_STAT_FR           0x40
#define DVBCI_IO_STAT_RSVD_20      0x20
#define DVBCI_IO_STAT_RSVD_10      0x10
#define DVBCI_IO_STAT_RSVD_08      0x08
#define DVBCI_IO_STAT_RSVD_04      0x04
#define DVBCI_IO_STAT_WE           0x02
#define DVBCI_IO_STAT_RE           0x01
#define DVBCI_IO_STAT_NULL         0x00

/* Time values (ms) for card ready */
#define MAX_PCCARD_READY_TIMEOUT 100
#define PCCARD_READY_WAIT_TIME   100

#define DVBCI_RESET_WAIT_TIME      10    /* Each wait = 20mS, a total = 5 Sec */
#define DVBCI_RESET_RETRY          500   /* Each wait = 20mS, a total = 5 Sec */

#define DVBCI_CDx_DEBOUNCE_RETRY   5     /* Try debouce 5 times               */
#define DVBCI_CDx_DEBOUNCE_TIME    20    /* 20 mS each time                   */


/*Enable shifting of address lines to increase address range*/
#define DVBCI_NO_SHIFT_ADDRESS TRUE
/*----------------------------- ENUMERATIONS -------------------------------*/
/** This enumerates the all four registers in the DVBCI
 ** interface for I/O mode operations. 
 **/
typedef enum
{
   CNXT_DATA_REG       = 0,
   CNXT_CTRL_STAT_REG,
   CNXT_SIZE_LS_REG,
   CNXT_SIZE_MS_REG,
   CNXT_REG_MAX = 10
} CNXT_DVBCI_REG;

/** This enumerates some PC CARD signals that are used in this API.
 ** when calling API cnxt_dvbci_signal_get_state( ) or
 ** cnxt_dvbci_signal_set_state( ), these enums should be used as 
 ** signal argument. 
 **/ 
typedef enum _CNXT_DVBCI_SIGNAL 
{

   CNXT_DVBCI_CARD_DETECT_1   = 0,
   CNXT_DVBCI_CARD_DETECT_2,
   CNXT_DVBCI_CARD_READY_IREQ,
   CNXT_DVBCI_RESET,
   CNXT_DVBCI_VS_1,
   CNXT_DVBCI_VS_2,
   CNXT_DVBCI_VPP_ENABLE,
   CNXT_DVBCI_SIGNAL_INVALID

} CNXT_DVBCI_SIGNAL;

#if 0     
/* These are the return values of APIs */
typedef enum
{
   CNXT_DVBCI_OK           = 0,
   CNXT_DVBCI_ALREADY_INIT,
   CNXT_DVBCI_NOT_INIT,
   CNXT_DVBCI_BAD_UNIT,
   CNXT_DVBCI_BAD_HANDLE,
   CNXT_DVBCI_BAD_PARAMETER,
   CNXT_DVBCI_CARD_ERROR,
   CNXT_DVBCI_RESOURCE_ERROR,
   CNXT_DVBCI_INTERNAL_ERROR,
   CNXT_DVBCI_NOT_AVAILABLE,
   CNXT_DVBCI_NO_CARD
   /* Note: list all specific error here */
} CNXT_DVBCI_STATUS;
#endif

/** These events should be used for communications between the DVBCI-HOST
 ** interface driver and the applications. The driver and some of the APIs
 ** must invoke the CNXT_DVBCI_PFNNOTIFY function to pass in the desired event.
 **
 ** NOTE: Please note that the CNXT_DVBCI_PFNNOTIFY function is running in
 ** the driver conext. The actual implementation of this notify function
 ** muat invoke a system call, either queue message send or event send, to
 ** relinguish the program control. 
 **/
typedef enum
{
   CNXT_DVBCI_EVENT_NULL = 0,
   CNXT_DVBCI_EVENT_INSERTED,
   CNXT_DVBCI_EVENT_REMOVED,
   CNXT_DVBCI_EVENT_TERMINATED,
   CNXT_DVBCI_EVENT_INTERRUPT,
   CNXT_DVBCI_MAX_EVENT
   /* Note: list all specific events here */
} CNXT_DVBCI_EVENT;

/** Two types of interrupt sources in the HOST-DVBCI interface are supported
 ** 1. CD1#/CD2# interrupts -- Used for PC CARD detection
 ** 2. DVBCI IREQ# interrupt  -- for DVBCI data I/O operation, DA and FR bits
 **/
typedef enum
{
   CNXT_DVBCI_INT_CD          = 0x0F,
   CNXT_DVBCI_INT_DA          = 0x80,
   CNXT_DVBCI_INT_FR          = 0x40,
   CNXT_DVBCI_INT_IO          = 0xC0,
   CNXT_DVBCI_MAX_INT_SOURCE  = 0x04
} CNXT_DVBCI_INTERRUPT;  

/** All HOST-DVBCI interrupts are level trigger.
 **/
typedef enum
{
   CNXT_DVBCI_INT_LOW   = 0,
   CNXT_DVBCI_INT_HGH,
   CNXT_DVBCI_MAX_INT_LEVEL
} CNXT_DVBCI_INT_LEVEL; 

/** CD1# and CD2# are edge triggered
 **/
typedef enum
{
   CNXT_DVBCI_RISING_EDGE   = 1,
   CNXT_DVBCI_FAILING_EDGE,
   CNXT_DVBCI_BOTH_EDGES,
   CNXT_DVBCI_MAX_INT_EDGE
} CNXT_DVBCI_INT_EDGE; 

/*--------------------------- BASIC DATA TYPES -----------------------------*/
/** The device handle is derived from cnxt_dvbci_inst object that is internal
 ** to the API. Please refer to dvbci_priv.h
 **/
typedef struct cnxt_dvbci_inst *CNXT_DVBCI_HANDLE;

/** This function is used for communications between the DVBCI-HOST
 ** interface driver and the applications. The driver and some of the APIs
 ** can invoke the CNXT_DVBCI_PFNNOTIFY function.
 **
 ** NOTE: Please note that the CNXT_DVBCI_PFNNOTIFY function is running in
 ** the driver conext. The actual implementation of this notify function
 ** muat invoke a system call, either queue message send or event send, to
 ** relinguish the program control. 
 **/
typedef CNXT_STATUS (*CNXT_DVBCI_PFNNOTIFY) ( CNXT_DVBCI_HANDLE Handle, 
                                                    void              *pUserData,
                                                    CNXT_DVBCI_EVENT  Event,
                                                    void              *pData,
                                                    void              *Tag );
/* Note: This is notification callback for asynchronous options.       */
/* If no asynchronous operation in the driver, change it as needed.    */

/** This function is passed as part of a slot config structure and is called
 ** whenever it is necessary to activate the slot.
 ** If a NULL is passed in the slot config structure then not slot-specific
 ** function needs to be called prior to accessing the slot.
 ** This allows a more hardware independent implementation of the DVBCI driver.
 **/
typedef void (*CNXT_DVBCI_PFNSETSLOT) ( u_int32 uSlotNum );


/*---------------------- STRUCTURE/UNION DATA TYPES ------------------------*/

/** The main configuration structure contains an array of slot config 
 ** structures. The slot config structures contain everything you need to 
 ** know about each slot.
 **/
typedef struct _CNXT_DVBCI_SLOT_CONFIG 
{
   
   u_int32  uDescNum;      /* ISA/ROM descriptor number to use       */
   u_int32  uCE1CS;        /* Chip Select number (0-5) for CE1#      */
   u_int32  uAddressOff;   /* Address offset from base of descriptor */
                           /* to access/address this slot.           */
   u_int32  uCD1GPIO;      /* GPIO identifier for CD1#               */
   u_int32  uCD2GPIO;      /* GPIO identifier for CD2#               */
   u_int32  uIREQGPIO;     /* GPIO identifier for IREQ               */
   u_int32  uResetGPIO;    /* GPIO identifier for RESET              */
   u_int32  uVS1GPIO;      /* Voltage Sense 1 GPIO                   */
   u_int32  uVS2GPIO;      /* Voltage Sense 2 GPIO                   */
   u_int32  uTSThruGPIO;   /* GPIO to set to route TS through module */
   bool     bUseXOE;       /* Use XOE (DVBCI device behind buffer)   */
   u_int32  uVppEnableGPIO;/* GPIO identifier for Vpp enable         */
	 u_int32  uSignalIntID[CNXT_DVBCI_SIGNAL_INVALID];
   CNXT_DVBCI_PFNSETSLOT pfnSetSlot; 
                           /* Function which is called to activate   */
                           /* this slot                              */
   
} CNXT_DVBCI_SLOT_CONFIG, *PCNXT_DVBCI_SLOT_CONFIG;

typedef struct _CNXT_DVBCI_CONFIG
{
   u_int32                uNumSlots;      
                           /* Number of slots to configure           */ 
   CNXT_DVBCI_SLOT_CONFIG SlotConfig[2];
                           /* Array of slot configuration structures */
} CNXT_DVBCI_CONFIG, *PCNXT_DVBCI_CONFIG;                           
 

/** It is a device capability structure. The uSlotNum is the return value
 ** in the object CNXT_DVBCI_CONFIG from the API call cnxt_dvbci_init()
 ** The DVBCI APIs use this to indentify the unit instance. 
 **/
typedef struct _CNXT_DVBCI_CAPS 
{

   u_int32 uLength;		   /* Length of the caps structure in bytes.  */
                           /* Normally set to sizeof(CNXT_DVBCI_CAPS) */
   bool    bExclusive;     /* Set to TRUE on query to indicate only   */
                           /* supports exclusive opening. Set to TRUE */
                           /* on open to indicate want exclusive      */
                           /* access.                                 */
   u_int32 uSlotNum;       /* 0 based numerical index of slot         */
                           /* associated with this instance.          */
} CNXT_DVBCI_CAPS, *PCNXT_DVBCI_CAPS;

/*------------------------ FUNCTION PROTOTYPE(S) -----------------------------*/

/** These are the available DVBCI API calls:
 **/
CNXT_STATUS cnxt_dvbci_init ( CNXT_DVBCI_CONFIG *pCfg );

CNXT_STATUS cnxt_dvbci_term ( void );
CNXT_STATUS cnxt_dvbci_get_units ( u_int32 *puCount );
CNXT_STATUS cnxt_dvbci_get_unit_caps ( u_int32         uUnitNumber, 
                                             CNXT_DVBCI_CAPS *pCaps );

CNXT_STATUS cnxt_dvbci_open ( CNXT_DVBCI_HANDLE    *pHandle,
                                    CNXT_DVBCI_CAPS      *pCaps,
                                    CNXT_DVBCI_PFNNOTIFY pNotifyFn,
                                    void                 *pUserData);

CNXT_STATUS cnxt_dvbci_close ( CNXT_DVBCI_HANDLE Handle );

CNXT_STATUS cnxt_dvbci_module_init( CNXT_DVBCI_HANDLE Handle );

CNXT_STATUS cnxt_dvbci_register_read( CNXT_DVBCI_HANDLE  Handle,
                                            CNXT_DVBCI_REG     Reg,
                                            u_int8             *pData,
                                            u_int16            NumBytes );
                                            
CNXT_STATUS cnxt_dvbci_register_write( CNXT_DVBCI_HANDLE  Handle,
                                             CNXT_DVBCI_REG     Reg,
                                             u_int8             *pData,
                                             u_int16            NumBytes );

CNXT_STATUS cnxt_dvbci_attribute_mem_read( CNXT_DVBCI_HANDLE Handle,
                                                 u_int32           AttribAddr,
                                                 u_int16           NumBytes,
                                                 u_int8            *pData);

CNXT_STATUS cnxt_dvbci_attribute_mem_write( CNXT_DVBCI_HANDLE Handle,
                                                  u_int32           AttribAddr,
                                                  u_int16           NumBytes,
                                                  u_int8            *pData);

CNXT_STATUS cnxt_dvbci_signal_get_state( CNXT_DVBCI_HANDLE Handle,
                                               CNXT_DVBCI_SIGNAL Signal,
                                               bool              *pbActive );
                                               
CNXT_STATUS cnxt_dvbci_signal_set_state( CNXT_DVBCI_HANDLE Handle,
                                               CNXT_DVBCI_SIGNAL Signal,
                                               bool              bActive );

CNXT_STATUS cnxt_dvbci_enable_ts( CNXT_DVBCI_HANDLE Handle );

CNXT_STATUS cnxt_dvbci_disable_ts( CNXT_DVBCI_HANDLE Handle );

CNXT_STATUS cnxt_dvbci_get_event_length(
               CNXT_DVBCI_HANDLE hHandle,
               CNXT_DVBCI_EVENT Event,
               u_int32         *puLength);

CNXT_STATUS cnxt_dvbci_enable_int(CNXT_DVBCI_HANDLE     Handle,
                                        bool                  bEnable);

/** This prototype are the HOOK function for DVBCI demo.
 **/
void DVBCITSK_Init(void); 

/*----------------------------- GLOBAL DATA ----------------------------------*/
/*-------------------- Use of this section is discouraged --------------------*/
#if defined __cplusplus || defined __cplusplus__
}
#endif

#endif   /* _DVBCI_H_ */

/****************************************************************************
 * Modifications:
 * $Log: 
 *  1    mpeg      1.0         4/20/04 6:29:56 PM     Steve Glennon   CR(s) 
 *        8901 8902 : Public Header file for DVBCI driver
 *        
 * $
 ****************************************************************************/

