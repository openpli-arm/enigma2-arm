/****************************************************************************/
/*                   CONEXANT PROPRIETARY AND CONFIDENTIAL                  */
/*                       SOFTWARE FILE/MODULE HEADER                        */
/*                   Conexant Systems Inc. (c) 1998 - 2004                  */
/*                               Austin, TX                                 */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:       status_codes.h
 *
 *
 * Description:    Definitions of all status codes returned by 
 *                 Conexant driver APIs.
 *
 *
 * Author:         Dave Wilson
 *
 ****************************************************************************/
/* $Id: status_codes.h 121092 2009-09-29 14:29:32Z paruphs $
 ****************************************************************************/
#ifndef _STATUS_CODES_H_
#define _STATUS_CODES_H_

/*****************************************************/
/* Status codes returned by all Conexant driver APIs */
/*****************************************************/

/* NB: If you add a member to this enum, please make sure you 
       also add a string to pszStatusCodes at the end of this
       file, making sure that it is in the same position in
       both lists! */
       
typedef enum
{
   /*   0  0x00  */   CNXT_STATUS_OK = 0,
   /*   1  0x01  */   CNXT_STATUS_ALREADY_INIT,
   /*   2  0x02  */   CNXT_STATUS_NOT_INIT,
   /*   3  0x03  */   CNXT_STATUS_INTERNAL_ERROR,
   /*   4  0x04  */   CNXT_STATUS_BAD_HANDLE,
   /*   5  0x05  */   CNXT_STATUS_BAD_PARAMETER,
   /*   6  0x06  */   CNXT_STATUS_BAD_LENGTH,
   /*   7  0x07  */   CNXT_STATUS_BAD_UNIT,
   /*   8  0x08  */   CNXT_STATUS_RESOURCE_ERROR,
   /*   9  0x09  */   CNXT_STATUS_CLOSED_HANDLE,
   /*  10  0x0A  */   CNXT_STATUS_TIMEOUT,
   /*  11  0x0B  */   CNXT_STATUS_NOT_ATTACHED,
   /*  12  0x0C  */   CNXT_STATUS_NOT_SUPPORTED,
   /*  13  0x0D  */   CNXT_STATUS_REOPENED_HANDLE,
   /*  14  0x0E  */   CNXT_STATUS_INVALID,
   /*  15  0x0F  */   CNXT_STATUS_DESTROYED,
   /*  16  0x10  */   CNXT_STATUS_DISCONNECTED,
   /*  17  0x11  */   CNXT_STATUS_BUSY,
   /*  18  0x12  */   CNXT_STATUS_IN_USE,
   /*  19  0x13  */   CNXT_STATUS_CANCELLED,
   /*  20  0x14  */   CNXT_STATUS_UNDEFINED,
   /*  21  0x15  */   CNXT_STATUS_UNKNOWN,
   /*  22  0x16  */   CNXT_STATUS_NOT_FOUND,
   /*  23  0x17  */   CNXT_STATUS_NOT_AVAILABLE,
   /*  24  0x18  */   CNXT_STATUS_NOT_COMPATIBLE,
   /*  25  0x19  */   CNXT_STATUS_NOT_IMPLEMENTED,
   /*  26  0x1A  */   CNXT_STATUS_EMPTY,
   /*  27  0x1B  */   CNXT_STATUS_FULL,
   /*  28  0x1C  */   CNXT_STATUS_FAILURE,
   /*  29  0x1D  */   CNXT_STATUS_ALREADY_ATTACHED,
   /*  30  0x1E  */   CNXT_STATUS_ALREADY_DONE,
   /*  31  0x1F  */   CNXT_STATUS_ASLEEP,
   /*  32  0x20  */   CNXT_STATUS_BAD_ATTACHMENT,
   /*  33  0x21  */   CNXT_STATUS_BAD_COMMAND,
   /*  34  0x22  */   CNXT_STATUS_BAD_GPIO,
   /*  35  0x23  */   CNXT_STATUS_BAD_INDEX,
   /*  36  0x24  */   CNXT_STATUS_BAD_MODE,
   /*  37  0x25  */   CNXT_STATUS_BAD_PID,
   /*  38  0x26  */   CNXT_STATUS_BAD_PLANE,
   /*  39  0x27  */   CNXT_STATUS_BAD_PTR,
   /*  40  0x28  */   CNXT_STATUS_BAD_RECT,
   /*  41  0x29  */   CNXT_STATUS_BAD_RGN_HANDLE,
   /*  42  0x2A  */   CNXT_STATUS_BAD_SIZE,
   /*  43  0x2B  */   CNXT_STATUS_INT_HANDLED,
   /*  44  0x2C  */   CNXT_STATUS_INT_NOT_HANDLED,
   /*  45  0x2D  */   CNXT_STATUS_NOT_SET,
   /*  46  0x2E  */   CNXT_STATUS_NOT_HOOKED,
   /*  47  0x2F  */   CNXT_STATUS_CC_NOT_ENABLED,
   /*  48  0x30  */   CNXT_STATUS_CLOSED_RGN,
   /*  49  0x31  */   CNXT_STATUS_COMPLETE,
   /*  50  0x32  */   CNXT_STATUS_DEMOD_ERROR,
   /*  51  0x33  */   CNXT_STATUS_INVALID_NODE,
   /*  52  0x34  */   CNXT_STATUS_DUPLICATE_NODE,
   /*  53  0x35  */   CNXT_STATUS_HARDWARE_NOT_FOUND,
   /*  54  0x36  */   CNXT_STATUS_HDCP_AUTH_FAILED,
   /*  55  0x37  */   CNXT_STATUS_HDCP_BAD_BKSV,
   /*  56  0x38  */   CNXT_STATUS_ILLEGAL_OPERATION,
   /*  57  0x39  */   CNXT_STATUS_INCOMPATIBLE_FORMATS,
   /*  58  0x3A  */   CNXT_STATUS_INVALID_DEVICE,
   /*  59  0x3B  */   CNXT_STATUS_INVALID_EDGE,
   /*  60  0x3C  */   CNXT_STATUS_INVALID_NUMBER,
   /*  61  0x3D  */   CNXT_STATUS_INVALID_STATE,
   /*  62  0x3E  */   CNXT_STATUS_INVALID_TYPE,
   /*  63  0x3F  */   CNXT_STATUS_NO_BUFFER,
   /*  64  0x40  */   CNXT_STATUS_NO_DESTINATION_BUF,
   /*  65  0x41  */   CNXT_STATUS_NO_OSD,
   /*  66  0x42  */   CNXT_STATUS_NO_PALETTE,
   /*  67  0x43  */   CNXT_STATUS_NO_ACK,
   /*  68  0x44  */   CNXT_STATUS_RECEIVER_HDMI_INCAPABLE,
   /*  69  0x45  */   CNXT_STATUS_RECEIVER_NOT_ATTACHED,
   /*  70  0x46  */   CNXT_STATUS_ADJUSTED,
   /*  71  0x47  */   CNXT_STATUS_CLIPPED,
   /*  72  0x48  */   CNXT_STATUS_CLIPRECT_ADJUSTED,
   /*  73  0x49  */   CNXT_STATUS_NOT_ALIGNED,
   /*  74  0x4A  */   CNXT_STATUS_FIXUP_OK,
   /*  75  0x4B  */   CNXT_STATUS_FIXUP_OPTION_ERROR,
   /*  76  0x4C  */   CNXT_STATUS_FIXUP_ZERO_RECT,
   /*  77  0x4D  */   CNXT_STATUS_UNABLE_TO_FIXUP_AND_PRESERVE,
   /*  78  0x4E  */   CNXT_STATUS_UNABLE_TO_FIXUP_X,
   /*  79  0x4F  */   CNXT_STATUS_UNABLE_TO_FIXUP_Y,
   /*  80  0x50  */   CNXT_STATUS_OUT_OF_BOUNDS,
   /*  81  0x51  */   CNXT_STATUS_OUTSIDE_CLIP_RECT,
   /*  82  0x52  */   CNXT_STATUS_RECT_CLIPPED,
   /*  83  0x53  */   CNXT_STATUS_RECT_ENCLOSED,
   /*  84  0x54  */   CNXT_STATUS_RECT_FIXED_UP,
   /*  85  0x55  */   CNXT_STATUS_RECT_INCLUDES,
   /*  86  0x56  */   CNXT_STATUS_RECT_NO_OVERLAP,
   /*  87  0x57  */   CNXT_STATUS_RECT_OVERLAP,
   /*  88  0x58  */   CNXT_STATUS_RECT_ZERO_AREA,
   /*  89  0x59  */   CNXT_STATUS_SERVICE_LIST_NOT_READY,
   /*  90  0x5A  */   CNXT_STATUS_SERVICE_LIST_READY,
   /*  91  0x5B  */   CNXT_STATUS_STOPPED,
   /*  92  0x5C  */   CNXT_STATUS_SUSPENDED,
   /*  93  0x5D  */   CNXT_STATUS_TERMINATED,
   /*  94  0x5E  */   CNXT_STATUS_TOO_MUCH_DATA,
   /*  95  0x5F  */   CNXT_STATUS_WIPE_NONE,
   /*  96  0x60  */   CNXT_STATUS_NOT_STOPPED,
   /*  97  0x61  */   CNXT_STATUS_INT_NOT_COMPLETE,
   /*  98  0x62  */   CNXT_STATUS_NOT_ALLOWED,
   /*  99  0x63  */   CNXT_STATUS_DUPLICATE_PID,
   /* 100  0x64  */   CNXT_STATUS_MAX_FILTERS_ATTACHED,
   /* 101  0x65  */   CNXT_STATUS_HW_NOT_READY,
   /* 102  0x66  */   CNXT_STATUS_OUTPUT_BUF_FULL,
   /* 103  0x67  */   CNXT_STATUS_REJECTED,
   /* 104  0x68  */   CNXT_STATUS_INVALID_PID,
   /* 105  0x69  */   CNXT_STATUS_EOF,
   /* 106  0x6A  */   CNXT_STATUS_BOF,
   /* 107  0x6B  */   CNXT_STATUS_MISSING_DATA,
   /* 108  0x6C  */   CNXT_STATUS_PM_ACK,
   /* 109  0x6D  */   CNXT_STATUS_PM_NAK,
   /* 110  0x6E  */   CNXT_STATUS_PM_ACCEPTED,
   /* 111  0x6F  */   CNXT_STATUS_PM_REJECTED,
   /* 112  0x70  */   CNXT_STATUS_PM_PENDING,
               CNXT_STATUS_CODE_LAST = CNXT_STATUS_PM_PENDING
} CNXT_STATUS;

/* Optionally, instantiate an array of status codes which can be used
   in, for example, debug trace messages. */
#ifdef INSTANTIATE_STATUS_CODE_STRINGS
const char *pszStatusCodes[] =
{
   /*   0  0x00  */   "CNXT_STATUS_OK",
   /*   1  0x01  */   "CNXT_STATUS_ALREADY_INIT",
   /*   2  0x02  */   "CNXT_STATUS_NOT_INIT",
   /*   3  0x03  */   "CNXT_STATUS_INTERNAL_ERROR",
   /*   4  0x04  */   "CNXT_STATUS_BAD_HANDLE",
   /*   5  0x05  */   "CNXT_STATUS_BAD_PARAMETER",
   /*   6  0x06  */   "CNXT_STATUS_BAD_LENGTH",
   /*   7  0x07  */   "CNXT_STATUS_BAD_UNIT",
   /*   8  0x08  */   "CNXT_STATUS_RESOURCE_ERROR",
   /*   9  0x09  */   "CNXT_STATUS_CLOSED_HANDLE",
   /*  10  0x0A  */   "CNXT_STATUS_TIMEOUT",
   /*  11  0x0B  */   "CNXT_STATUS_NOT_ATTACHED",
   /*  12  0x0C  */   "CNXT_STATUS_NOT_SUPPORTED",
   /*  13  0x0D  */   "CNXT_STATUS_REOPENED_HANDLE",
   /*  14  0x0E  */   "CNXT_STATUS_INVALID",
   /*  15  0x0F  */   "CNXT_STATUS_DESTROYED",
   /*  16  0x10  */   "CNXT_STATUS_DISCONNECTED",
   /*  17  0x11  */   "CNXT_STATUS_BUSY",
   /*  18  0x12  */   "CNXT_STATUS_IN_USE",
   /*  19  0x13  */   "CNXT_STATUS_CANCELLED",
   /*  20  0x14  */   "CNXT_STATUS_UNDEFINED",
   /*  21  0x15  */   "CNXT_STATUS_UNKNOWN",
   /*  22  0x16  */   "CNXT_STATUS_NOT_FOUND",
   /*  23  0x17  */   "CNXT_STATUS_NOT_AVAILABLE",
   /*  24  0x18  */   "CNXT_STATUS_NOT_COMPATIBLE",
   /*  25  0x19  */   "CNXT_STATUS_NOT_IMPLEMENTED",
   /*  26  0x1A  */   "CNXT_STATUS_EMPTY",
   /*  27  0x1B  */   "CNXT_STATUS_FULL",
   /*  28  0x1C  */   "CNXT_STATUS_FAILURE",
   /*  29  0x1D  */   "CNXT_STATUS_ALREADY_ATTACHED",
   /*  30  0x1E  */   "CNXT_STATUS_ALREADY_DONE",
   /*  31  0x1F  */   "CNXT_STATUS_ASLEEP",
   /*  32  0x20  */   "CNXT_STATUS_BAD_ATTACHMENT",
   /*  33  0x21  */   "CNXT_STATUS_BAD_COMMAND",
   /*  34  0x22  */   "CNXT_STATUS_BAD_GPIO",
   /*  35  0x23  */   "CNXT_STATUS_BAD_INDEX",
   /*  36  0x24  */   "CNXT_STATUS_BAD_MODE",
   /*  37  0x25  */   "CNXT_STATUS_BAD_PID",
   /*  38  0x26  */   "CNXT_STATUS_BAD_PLANE",
   /*  39  0x27  */   "CNXT_STATUS_BAD_PTR",
   /*  40  0x28  */   "CNXT_STATUS_BAD_RECT",
   /*  41  0x29  */   "CNXT_STATUS_BAD_RGN_HANDLE",
   /*  42  0x2A  */   "CNXT_STATUS_BAD_SIZE",
   /*  43  0x2B  */   "CNXT_STATUS_INT_HANDLED",
   /*  44  0x2C  */   "CNXT_STATUS_INT_NOT_HANDLED",
   /*  45  0x2D  */   "CNXT_STATUS_NOT_SET",
   /*  46  0x2E  */   "CNXT_STATUS_NOT_HOOKED",
   /*  47  0x2F  */   "CNXT_STATUS_CC_NOT_ENABLED",
   /*  48  0x30  */   "CNXT_STATUS_CLOSED_RGN",
   /*  49  0x31  */   "CNXT_STATUS_COMPLETE",
   /*  50  0x32  */   "CNXT_STATUS_DEMOD_ERROR",
   /*  51  0x33  */   "CNXT_STATUS_INVALID_NODE",
   /*  52  0x34  */   "CNXT_STATUS_DUPLICATE_NODE",
   /*  53  0x35  */   "CNXT_STATUS_HARDWARE_NOT_FOUND",
   /*  54  0x36  */   "CNXT_STATUS_HDCP_AUTH_FAILED",
   /*  55  0x37  */   "CNXT_STATUS_HDCP_BAD_BKSV",
   /*  56  0x38  */   "CNXT_STATUS_ILLEGAL_OPERATION",
   /*  57  0x39  */   "CNXT_STATUS_INCOMPATIBLE_FORMATS",
   /*  58  0x3A  */   "CNXT_STATUS_INVALID_DEVICE",
   /*  59  0x3B  */   "CNXT_STATUS_INVALID_EDGE",
   /*  60  0x3C  */   "CNXT_STATUS_INVALID_NUMBER",
   /*  61  0x3D  */   "CNXT_STATUS_INVALID_STATE",
   /*  62  0x3E  */   "CNXT_STATUS_INVALID_TYPE",
   /*  63  0x3F  */   "CNXT_STATUS_NO_BUFFER",
   /*  64  0x40  */   "CNXT_STATUS_NO_DESTINATION_BUF",
   /*  65  0x41  */   "CNXT_STATUS_NO_OSD",
   /*  66  0x42  */   "CNXT_STATUS_NO_PALETTE",
   /*  67  0x43  */   "CNXT_STATUS_NO_ACK",
   /*  68  0x44  */   "CNXT_STATUS_RECEIVER_HDMI_INCAPABLE",
   /*  69  0x45  */   "CNXT_STATUS_RECEIVER_NOT_ATTACHED",
   /*  70  0x46  */   "CNXT_STATUS_ADJUSTED",
   /*  71  0x47  */   "CNXT_STATUS_CLIPPED",
   /*  72  0x48  */   "CNXT_STATUS_CLIPRECT_ADJUSTED",
   /*  73  0x49  */   "CNXT_STATUS_NOT_ALIGNED",
   /*  74  0x4A  */   "CNXT_STATUS_FIXUP_OK",
   /*  75  0x4B  */   "CNXT_STATUS_FIXUP_OPTION_ERROR",
   /*  76  0x4C  */   "CNXT_STATUS_FIXUP_ZERO_RECT",
   /*  77  0x4D  */   "CNXT_STATUS_UNABLE_TO_FIXUP_AND_PRESERVE",
   /*  78  0x4E  */   "CNXT_STATUS_UNABLE_TO_FIXUP_X",
   /*  79  0x4F  */   "CNXT_STATUS_UNABLE_TO_FIXUP_Y",
   /*  80  0x50  */   "CNXT_STATUS_OUT_OF_BOUNDS",
   /*  81  0x51  */   "CNXT_STATUS_OUTSIDE_CLIP_RECT",
   /*  82  0x52  */   "CNXT_STATUS_RECT_CLIPPED",
   /*  83  0x53  */   "CNXT_STATUS_RECT_ENCLOSED",
   /*  84  0x54  */   "CNXT_STATUS_RECT_FIXED_UP",
   /*  85  0x55  */   "CNXT_STATUS_RECT_INCLUDES",
   /*  86  0x56  */   "CNXT_STATUS_RECT_NO_OVERLAP",
   /*  87  0x57  */   "CNXT_STATUS_RECT_OVERLAP",
   /*  88  0x58  */   "CNXT_STATUS_RECT_ZERO_AREA",
   /*  89  0x59  */   "CNXT_STATUS_SERVICE_LIST_NOT_READY",
   /*  90  0x5A  */   "CNXT_STATUS_SERVICE_LIST_READY",
   /*  91  0x5B  */   "CNXT_STATUS_STOPPED",
   /*  92  0x5C  */   "CNXT_STATUS_SUSPENDED",
   /*  93  0x5D  */   "CNXT_STATUS_TERMINATED",
   /*  94  0x5E  */   "CNXT_STATUS_TOO_MUCH_DATA",
   /*  95  0x5F  */   "CNXT_STATUS_WIPE_NONE",
   /*  96  0x60  */   "CNXT_STATUS_NOT_STOPPED",
   /*  97  0x61  */   "CNXT_STATUS_INT_NOT_COMPLETE",
   /*  98  0x62  */   "CNXT_STATUS_NOT_ALLOWED",
   /*  99  0x63  */   "CNXT_STATUS_DUPLICATE_PID",
   /* 100  0x64  */   "CNXT_STATUS_MAX_FILTERS_ATTACHED"
   /* 101  0x65  */   "CNXT_STATUS_HW_NOT_READY",
   /* 102  0x66  */   "CNXT_STATUS_OUTPUT_BUF_FULL",
   /* 103  0x67  */   "CNXT_STATUS_REJECTED",
   /* 104  0X68  */   "CNXT_STATUS_INVALID_PID",
   /* 105  0x69  */   "CNXT_STATUS_EOF",
   /* 106  0x6A  */   "CNXT_STATUS_BOF",
   /* 107  0x6B  */   "CNXT_STATUS_MISSING_DATA",
   /* 108  0x6C  */   "CNXT_STATUS_PM_ACK",
   /* 109  0x6D  */   "CNXT_STATUS_PM_NAK",
   /* 110  0x6E  */   "CNXT_STATUS_PM_ACCEPTED",
   /* 111  0x6F  */   "CNXT_STATUS_PM_REJECTED",
   /* 112  0x70  */   "CNXT_STATUS_PM_PENDING"
};
#else
extern const char *pszStatusCodes[];
#endif

#endif /* _STATUS_CODES_H_ */

/****************************************************************************
 * Modifications:
 * $Log$
 *
 ****************************************************************************/

