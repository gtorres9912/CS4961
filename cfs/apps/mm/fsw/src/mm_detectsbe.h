/*************************************************************************
** File:
**   $Id: mm_detectsbe.h
**
** Purpose: 
**   Specification for the CFS Memory Manager SBEI_Inject ground commands.
** 
*************************************************************************/
#ifndef _mm_detectsbe_
#define _mm_detectsbe_

/*************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "mm_msg.h"
#include "mm_filedefs.h"

/************************************************************************/
/** \brief Process single bit error inject command
**  
**  \par Description
**       Processes the single bit error inject command that will read 
**		 the SBEI memory location and flip 1 bit of the 8 bit sequence
**		
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   msg   A #CFE_SB_MsgPtr_t pointer that
**                      references the software bus message 
**
**  \sa #MM_DETECTSBE_CC
**
*************************************************************************/
void MM_DETECTSBE_Cmd(CFE_SB_MsgPtr_t MessagePtr, uint8 sbei);

uint16 MM_readSBEDMem(CFE_SB_MsgPtr_t MessagePtr);

uint8 MM_readSBEIMem(CFE_SB_MsgPtr_t MessagePtr);

void MM_isChanged(CFE_SB_MsgPtr_t MessagePtr, uint8 sbei, uint16 sbed);

void MM_writeToSBEI(CFE_SB_MsgPtr_t MessagePtr);

uint8 MM_retrieveParityBits(uint8 sbei);

char* MM_encodeBitsAlgo(uint8 ByteValue);

/************************************************************************/
/** \brief Bit Flip
**  
**  \par Description
**       Support function for #MM_SBEI_InjectCmd. This routine will flip
**		 a random bit in the SBEI
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   ByteValue     A #uint8 ByteValue that was read in the 
**								SBEI memory
**
**  \param [in]   DestAddress   The source address for the flip operation 
** 
*************************************************************************/

#endif /* _mm_detectsbe_ */

/************************/
/*  End of File Comment */
/************************/