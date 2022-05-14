/************************************************************************
** File:
**   $Id: sbed_app_msgids.h  $
**
** Purpose: 
**  Define Sbed App  Message IDs
**
** Notes:
**   1. Default to OSK test IDs. These IDs should be changed if the app
**      is integrated with the user's system.
**
*************************************************************************/
#ifndef _sbed_app_msgids_h_
#define _sbed_app_msgids_h_

/*
** Command Message IDs
*/

#define  SBED_CMD_MID        0x1FF0
#define  SBED_SEND_HK_MID    0x1A06   /* Generic 4s scheduled packet */


/*
** Telemetry Message IDs
*/

#define  SBED_HK_TLM_MID     0x0FF0

#endif /* _sbed_app_msgids_h_ */
