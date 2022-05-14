/*************************************************************************
** File:
**   $Id: mm_sbed.h
**
** Purpose: 
**   Specification for the CFS Memory Manager SBED_Detect ground commands.
** 
*************************************************************************/
#ifndef _mm_storebits_
#define _mm_storebits_

/*************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "mm_msg.h"
#include "mm_filedefs.h"

/************************************************************************/
/** \brief Process single bit error detect command
**  
**  \par Description
**       Detects whether a single bit error occurred.
**		
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   msg   A #CFE_SB_MsgPtr_t pointer that
**                      references the software bus message 
**
**  \sa #MM_SBED_DETECT_CC
**
*************************************************************************/
void MM_STOREBITS_Cmd(CFE_SB_MsgPtr_t MessagePtr);

uint32 MM_createInitialBits(CFE_SB_MsgPtr_t MessagePtr);

void MM_writeToMem(CFE_SB_MsgPtr_t MessagePtr, uint32 byteValue);
/************************************************************************/
/** \brief conversion to Binary
**  
**  \par Description
**       Support function for #MM_SBED_DetectCMD.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   value     The value is the data read from SBEI_AppData 
**
**
**  \param [in]   DestAddress   The source address for the flip operation 
** 
*************************************************************************/
char* MM_encodeBits(uint8 ByteValue);

#endif /* _mm_storebits_ */

/************************/
/*  End of File Comment */
/************************/