/******************************************************************************/
/*                   CONEXANT PROPRIETARY AND CONFIDENTIAL                    */
/*                        SOFTWARE FILE/MODULE HEADER                         */
/*                 Copyright Conexant Systems Inc. 2004-2006                  */
/*                                 Austin, TX                                 */
/*                            All Rights Reserved                             */
/******************************************************************************/
/*
 * Filename:        trace_types.h
 *
 *
 * Description:     Public header file containing type definitions and labels
 *                  related to debug trace functions in the KAL
 *
 *
 * Author:          Dave Wilson
 *
 ******************************************************************************/
/* $Id: trace_types.h 143558 2010-01-30 02:36:12Z gargn $
 ******************************************************************************/

#ifndef _TRACE_TYPES_H_
#define _TRACE_TYPES_H_

/* Flag indicating a fatal error - top bit of error_num */
#define CNXT_ERROR_FATAL              0x80000000
#define CNXT_ERROR_WARNING            0x00000000

/* Flags for trace levels */
#define CNXT_TRACE_MASK_MODULE        0x0FFFFFFF
#define CNXT_TRACE_MASK_LEVEL         0x70000000
#define CNXT_TRACE_MASK_TIMESTAMP     0x80000000

#define CNXT_TRACE_SHIFT_LEVEL        28
#define CNXT_TRACE_SHIFT_TIMESTAMP    31

#define CNXT_TRACE_LEVEL_NEVER        0x00000000
#define CNXT_TRACE_LEVEL_1            0x10000000
#define CNXT_TRACE_LEVEL_2            0x20000000
#define CNXT_TRACE_LEVEL_3            0x30000000
#define CNXT_TRACE_LEVEL_4            0x40000000
#define CNXT_TRACE_LEVEL_5            0x50000000
#define CNXT_TRACE_LEVEL_6            0x60000000
#define CNXT_TRACE_LEVEL_ALWAYS       0x70000000

/* Disable the timestamp */
#define CNXT_TRACE_NO_TIMESTAMP       0x80000000

/* Driver module identifiers */
#define CNXT_TRACE_ANALOG_TV          0x00000001
#define CNXT_TRACE_ATA                0x00000002
#define CNXT_TRACE_AUDIO              0x00000003
#define CNXT_TRACE_CA                 0x00000004
#define CNXT_TRACE_CABLE_MODEM        0x00000005
#define CNXT_TRACE_CBUF               0x00000006
#define CNXT_TRACE_CCHANGE            0x00000007
#define CNXT_TRACE_CLK_REC            0x00000008
#define CNXT_TRACE_CLOSED_CAPTION     0x00000009
#define CNXT_TRACE_COMMON_INTERFACE   0x0000000A
#define CNXT_TRACE_CONFIG_MGR         0x0000000B
#define CNXT_TRACE_DEINTERLACER       0x0000000C
#define CNXT_TRACE_DEMO               0x0000000D
#define CNXT_TRACE_DEMOD              0x0000000E
#define CNXT_TRACE_DEMOD_SATELLITE    0x0000000F
#define CNXT_TRACE_DEMOD_CABLE        0x00000010
#define CNXT_TRACE_DEMOD_TERRESTRIAL  0x00000011
#define CNXT_TRACE_DEMOD_OTHER        0x00000012
#define CNXT_TRACE_DEMUX              0x00000013
#define CNXT_TRACE_DRM                0x00000014
#define CNXT_TRACE_VPP                CNXT_TRACE_DRM
#define CNXT_TRACE_DVBLIB             0x00000015
#define CNXT_TRACE_PVR_SYSTEM         0x00000016
#define CNXT_TRACE_PVR_REC            0x00000017
#define CNXT_TRACE_PVR_PLAY           0x00000018
#define CNXT_TRACE_EEPROM             0x00000019
#define CNXT_TRACE_ENCRYPT            0x0000001A
#define CNXT_TRACE_ETHERNET           0x0000001B
#define CNXT_TRACE_FILE_SYSTEM        0x0000001C
#define CNXT_TRACE_FLASH              0x0000001D
#define CNXT_TRACE_GPIO               0x0000001E
#define CNXT_TRACE_GRAPHICS           0x0000001F
#define CNXT_TRACE_GRAPHICS_HELPER    0x00000020
#define CNXT_TRACE_HANDLE             0x00000021
#define CNXT_TRACE_HSDP               0x00000022
#define CNXT_TRACE_HWLIB              0x00000023
#define CNXT_TRACE_I2C                0x00000024
#define CNXT_TRACE_IMAGE              0x00000025
#define CNXT_TRACE_IOLIB              0x00000026
#define CNXT_TRACE_JPEG               0x00000027
#define CNXT_TRACE_KAL                0x00000028
#define CNXT_TRACE_MEMMOVE            0x00000029
#define CNXT_TRACE_MODEM              0x0000002A
#define CNXT_TRACE_NETWORK            0x0000002B
#define CNXT_TRACE_PCM                0x0000002C
#define CNXT_TRACE_PLL                0x0000002D
#define CNXT_TRACE_PNG                0x0000002E
#define CNXT_TRACE_POWER              0x0000002F
#define CNXT_TRACE_TEMPLATE           0x00000030
#define CNXT_TRACE_PANEL_BUTTONS      0x00000031
#define CNXT_TRACE_PANEL_LCDS         0x00000032
#define CNXT_TRACE_PANEL_LEDS         0x00000033
#define CNXT_TRACE_REMOTE_CTL         0x00000034
#define CNXT_TRACE_RF_MOD             0x00000035
#define CNXT_TRACE_REAL_TIME_CLOCK    0x00000036
#define CNXT_TRACE_SCART              0x00000037
#define CNXT_TRACE_SERIAL             0x00000038
#define CNXT_TRACE_SMARTCARD          0x00000039
#define CNXT_TRACE_SOAR               0x0000003A
#define CNXT_TRACE_STARTUP            0x0000003B
#define CNXT_TRACE_SVCLIST_DTV        0x0000003C
#define CNXT_TRACE_SVCLIST_DVB        0x0000003D
#define CNXT_TRACE_TELETEXT           0x0000003E
#define CNXT_TRACE_TVENC              0x0000003F
#define CNXT_TRACE_VIDEO              0x00000040
#define CNXT_TRACE_TSROUTE            0x00000041
#define CNXT_TRACE_AVS                0x00000042
#define CNXT_TRACE_DEMUX_CLKREC       0x00000043
#define CNXT_TRACE_UI_CONTROLS        0x00000044
#define CNXT_TRACE_DMA                0x00000045
#define CNXT_TRACE_TRICKMODE          0x00000046
#define CNXT_TRACE_CC                 0x00000047
#define CNXT_TRACE_MIPIDLE            0x00000048
#define CNXT_TRACE_CRYPTO             0x00000049
#define CNXT_TRACE_CI                 0x0000004A       /* For CableCard Interface*/
#define CNXT_TRACE_IMGPORT            0x0000004B
#define CNXT_TRACE_EXTXPRT            0x0000004C
#define CNXT_TRACE_SSP                0x0000004D
#define CNXT_TRACE_PVR_FS             0x0000004E      /* For PVR File system */
#define CNXT_TRACE_COMPOSITOR         0x0000004F      /* Graphics compositor */
#define CNXT_TRACE_MCARD              0x00000050      /* MCARD */
#define CNXT_TRACE_MOSAIC             0x00000051      /* MOSAIC */
#define CNXT_TRACE_PINKY_DBGR         0x00000052      /* Pinky Debugger */
#define CNXT_TRACE_SPI                0x00000053      /* SPI */
#define CNXT_TRACE_DTCP               0x00000054
#define CNXT_TRACE_MEM                0x00000055
#define CNXT_TRACE_SCD                0x00000056
#define CNXT_TRACE_VSM_DMA            0x00000057
#define CNXT_TRACE_FUSECTL            0x00000058

/* Additional defines for CM traces */
#define CNXT_TRACE_CABLE_MODEM_MUX    CNXT_TRACE_CABLE_MODEM
#define CNXT_TRACE_TSTHOSTIF          CNXT_TRACE_CABLE_MODEM
#define CNXT_TRACE_TSTPCI             CNXT_TRACE_CABLE_MODEM

/* Additional IDs for application and middleware use */
#define CNXT_TRACE_MIDDLEWARE         0x0000005C
#define CNXT_TRACE_CONTROL_TASK       0x0000005D
#define CNXT_TRACE_APPLICATION        0x0000005E
#define CNXT_TRACE_TEST               0x0000005F

/* IDs for middleware adaptation drivers */
#define CNXT_TRACE_MWARE_AUDIO        0x00000060
#define CNXT_TRACE_MWARE_VIDEO        0x00000061
#define CNXT_TRACE_MWARE_DEMOD        0x00000062
#define CNXT_TRACE_MWARE_DEMUX        0x00000063
#define CNXT_TRACE_MWARE_CONTROL      0x00000064
#define CNXT_TRACE_MWARE_NVRAM        0x00000065
#define CNXT_TRACE_MWARE_BUTTONS      0x00000066
#define CNXT_TRACE_MWARE_LEDS         0x00000067
#define CNXT_TRACE_MWARE_REMOTE_CTL   0x00000068
#define CNXT_TRACE_MWARE_IMAGE        0x00000069
#define CNXT_TRACE_MWARE_OSD          0x0000006A
#define CNXT_TRACE_MWARE_MODEM        0x0000006B
#define CNXT_TRACE_MWARE_SERIAL       0x0000006C
#define CNXT_TRACE_MWARE_LOADER       0x0000006D
#define CNXT_TRACE_MWARE_AV_CONTROL   0x0000006E
#define CNXT_TRACE_MWARE_RF_MOD       0x0000006F
#define CNXT_TRACE_MWARE_TVENC        0x00000070
#define CNXT_TRACE_MWARE_CA           0x00000071
#define CNXT_TRACE_MWARE_PVR          0x00000072
#define CNXT_TRACE_MWARE_PANEL        0x00000073
#define CNXT_TRACE_MWARE_CC           0x00000074
#define CNXT_TRACE_MWARE_SCARD        0x00000075
#define CNXT_TRACE_MWARE_HDMI         0x00000076
#define CNXT_TRACE_MWARE_MISC         0x00000077

/* IDs between CNXT_TRACE_USER and CNXT_TRACE_MAX are free for use by customer
   applications */
#define CNXT_TRACE_USER               0x00000078
#define CNXT_TRACE_VBI                0x00000079
#define CNXT_TRACE_STC                0x0000007A
#define CNXT_TRACE_STC_CLKREC         0x0000007B

/* The upper supported value of a trace module identifier. Make sure that
   there are no trace IDs defined above this value since it is used to
   determine the amount of storage required for trace flags! */
#define CNXT_TRACE_MAX                0x000000A0

/* ID indicating that message should be displayed regardless of the
   modules which are currently enabled. */
#define CNXT_TRACE_ANY                0x00000000

/* Trace message color codes */

#define CNXT_TRACE_FG_LIGHT_BLUE            "\033[1;34m"
#define CNXT_TRACE_FG_LIGHT_GREEN           "\033[1;32m"
#define CNXT_TRACE_FG_LIGHT_CYAN            "\033[1;36m"
#define CNXT_TRACE_FG_LIGHT_RED             "\033[1;31m"
#define CNXT_TRACE_FG_WHITE                 "\033[1;37m"
#define CNXT_TRACE_FG_NORMAL                CNXT_TRACE_FG_WHITE
#define CNXT_TRACE_BG_GRAY                  "\033[0;47m"
#define CNXT_TRACE_BG_BLACK                 "\033[0;40m"

/* The number of 32bit words needed to store our trace flags */
#define CNXT_TRACE_FLAGS_WORDS ((CNXT_TRACE_MAX+31)/32)

/* Data structure used to store trace flags */
typedef struct
{
   u_int32 Flags[CNXT_TRACE_FLAGS_WORDS];
} CNXT_TRACE_FLAGS;


#endif /* _TRACE_TYPES_H_ */

/******************************************************************************
 * Modifications:
 * $Log$
 *
 ******************************************************************************/


