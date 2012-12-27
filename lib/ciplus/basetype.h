/****************************************************************************/
/*                   CONEXANT PROPRIETARY AND CONFIDENTIAL                  */
/*                       SOFTWARE FILE/MODULE HEADER                        */
/*                   Copyright Conexant Systems Inc. 2008                   */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:       basetype.h
 *
 *
 * Description:    Base data types used by Conexant driver software
 *
 ****************************************************************************/
/* $Id: basetype.h 172358 2010-09-15 00:25:47Z gargn $
 ****************************************************************************/

#ifndef _BASETYPE_H_
#define _BASETYPE_H_

#ifndef __OPTVX_H__ /* This def conflicts with OpenTV definition */
typedef unsigned long long u_int64;
#endif

typedef unsigned long u_int32;
typedef unsigned short u_int16;
typedef unsigned char u_int8;
#if !defined(_LINUX_TYPES_H) && !defined(__OPTVX_H__) && !defined(__DIRECT__TYPES_H__) && !defined(__cplusplus) && !defined(__ARMCC_VERSION)
typedef _Bool bool;
#elif defined(__ARMCC_VERSION)
typedef unsigned int bool;
#endif
typedef void *voidF;

/* This pointer type is needed such that kernel-headers can be shared with */
/* user-space code. Note that kernel and physical pointer types will NEVER */
/* be used in user space so are not included here.                         */
typedef void *UVMPTR;

#if !defined(TYPEDEF_H) /* added to keep softmodem code common */
#ifndef __OPTVX_H__ /* This def conflicts with OpenTV definition */
typedef signed long long int64;
#endif
typedef signed long int32;
typedef signed short int16;
typedef signed char int8;

typedef unsigned long uint64;
typedef unsigned long uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#endif /* _BASETYPE_H_ */

/****************************************************************************
 * Modifications:
 * $Log$
 *
 ****************************************************************************/


