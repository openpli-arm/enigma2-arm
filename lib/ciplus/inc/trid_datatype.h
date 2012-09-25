/*************************************************************************
 * File: trid_datatype.h
 * 
 * Copyright (C) Trident Digital Multimedia Technologies Co.,
 *          2007  All rights reserved.
 * 
 * This file contains basic data type use in DTV library
 * 
 * Revision:
 *  05/04/2007     Created by Keven 
 *  05/19/2007     Blance update 
 * 
 *************************************************************************/

#ifndef __TRID_DATATYPE_H__
#define __TRID_DATATYPE_H__

#if defined __cplusplus || defined __cplusplus__
extern "C" {
#endif
//#include "typedef.h"


typedef void (*Data_Callback) (int id,char *data, int size, int reason);
typedef void (*Status_Callback) (int id,void *pStatus,int reason);
typedef int  (*Callback)(int id, int filterid, int reason);
typedef void (*CC_Callback) (int id,char *data, int size, int reason,unsigned long disp_ms, void *pInfo);
/*****************************************************************************
 *                          basic data type definition
 ****************************************************************************/

typedef unsigned long long	trid_uint64;
typedef unsigned long		trid_uint32;
typedef unsigned short		trid_uint16;
typedef unsigned char		trid_uint8;
typedef long long			trid_sint64;
typedef signed long			trid_sint32;
typedef signed short		trid_sint16;
typedef signed char 		trid_sint8;
typedef char 				trid_char;
//typedef void 				trid_void;
typedef unsigned char		trid_bool;

#define trid_void void

typedef struct _tagTridExtString
{
	trid_uint16 BufSize;	// whole buf size
	trid_uint16 DataSize;	// valid data size in buffer
	trid_uint8 EnCodeType;	// Data Encodetype in Buffer
	trid_uint8 Buf[0];
}TridExtString_t;

#ifndef trid_false
#define trid_false 	(0)
#endif

#ifndef trid_true
#define trid_true (!trid_false)
#endif


#define     _TRID_BIT_0_         1 << 0
#define     _TRID_BIT_1_         1 << 1
#define     _TRID_BIT_2_         1 << 2
#define     _TRID_BIT_3_         1 << 3
#define     _TRID_BIT_4_         1 << 4
#define     _TRID_BIT_5_         1 << 5
#define     _TRID_BIT_6_         1 << 6
#define     _TRID_BIT_7_         1 << 7
#define     _TRID_BIT_8_         1 << 8
#define     _TRID_BIT_9_         1 << 9
#define     _TRID_BIT_10_        1 << 10
#define     _TRID_BIT_11_        1 << 11
#define     _TRID_BIT_12_        1 << 12
#define     _TRID_BIT_13_        1 << 13
#define     _TRID_BIT_14_        1 << 14
#define     _TRID_BIT_15_        1 << 15
#define     _TRID_BIT_16_        1 << 16
#define     _TRID_BIT_17_        1 << 17
#define     _TRID_BIT_18_        1 << 18
#define     _TRID_BIT_19_        1 << 19
#define     _TRID_BIT_20_        1 << 20
#define     _TRID_BIT_21_        1 << 21
#define     _TRID_BIT_22_        1 << 22
#define     _TRID_BIT_23_        1 << 23
#define     _TRID_BIT_24_        1 << 24
#define     _TRID_BIT_25_        1 << 25
#define     _TRID_BIT_26_        1 << 26
#define     _TRID_BIT_27_        1 << 27
#define     _TRID_BIT_28_        1 << 28
#define     _TRID_BIT_29_        1 << 29
#define     _TRID_BIT_30_        1 << 30
#define     _TRID_BIT_31_        1 << 31

#if defined __cplusplus || defined __cplusplus__
}
#endif

#endif


/*****************************************************************************
*                       End Of File
*****************************************************************************/



