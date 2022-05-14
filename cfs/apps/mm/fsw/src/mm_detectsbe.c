/*************************************************************************
** File:
**   $Id: mm_detectsbe.c
**
** Purpose: 
**   Reoslves SBE if it has occurred
**
*************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/
#include "mm_app.h"
#include "mm_events.h"
#include "cfs_utils.h"
#include "mm_detectsbe.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

/*************************************************************************
** External Data
*************************************************************************/
extern MM_AppData_t MM_AppData;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Single Bit Error Inject Command                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MM_DETECTSBE_Cmd(CFE_SB_MsgPtr_t msg, uint8 sbei) {  
    return;
} /* end MM_SBEI_InjectCmd */


uint16 MM_readSBEDMem(CFE_SB_MsgPtr_t msg) {
    //Read bit data from SBED_AppData
    int32 PSP_Status;
    uint16 byte = 0;
    uint32 DestAddress;
    CFS_SymAddr_t     DestSymAddress;

    char address[] = "SBED_AppData";

    strcpy(DestSymAddress.SymName, address);
    CFS_ResolveSymAddr(&DestSymAddress, &DestAddress);

    PSP_Status = CFE_PSP_MemRead16(DestAddress, &byte);
    return byte;
}

uint8 MM_readSBEIMem(CFE_SB_MsgPtr_t msg) {
    //Read bit data from SBEI_AppData
    int32 PSP_Status;
    uint8 byte = 0;
    //uint32 newValue = 0;
    uint32 DestAddress;
    CFS_SymAddr_t     DestSymAddress;

    char address[] = "SBEI_AppData";

    strcpy(DestSymAddress.SymName, address);
    CFS_ResolveSymAddr(&DestSymAddress, &DestAddress);

    PSP_Status = CFE_PSP_MemRead8(DestAddress, &byte);
    return byte;
}

void MM_isChanged(CFE_SB_MsgPtr_t msg, uint8 sbei, uint16 sbed) {
    //Check to see if there has been a change in the bits
    uint8 parityBits = MM_retrieveParityBits(sbei);
    CFE_EVS_SendEvent(MM_DETECTSBE_EID, CFE_EVS_INFORMATION, "DETECTSBE COMMAND: current parity bits - %X", parityBits);
    CFE_EVS_SendEvent(MM_DETECTSBE_EID, CFE_EVS_INFORMATION, "DETECTSBE COMMAND: stored parity bits  - %X", sbed);
    if (parityBits == sbed) {
        CFE_EVS_SendEvent(MM_DETECTSBE_EID, CFE_EVS_INFORMATION, "DETECTSBE COMMAND: no flip has occurred");
        return;
    }
    CFE_EVS_SendEvent(MM_DETECTSBE_EID, CFE_EVS_INFORMATION, "DETECTSBE COMMAND: flip has occurred");
    return;
}

uint8 MM_retrieveParityBits(uint8 sbei) {
    //Create encoded bits
    char* encodedBits = MM_encodeBitsAlgo(sbei);

    //Flip bits in sbedData depending on the parity bits and return them
    uint8 sbedData = 0;
    uint8 flip = 0;
    for (int i = 0; i < strlen(encodedBits); i++) {
        if (i == 11 && encodedBits[i] == '1') {
            flip = 1;
            sbedData ^= flip;
        }
        else if (i == 10 && encodedBits[i] == '1') {
            flip = 2;
            sbedData ^= flip;
        }
        else if (i == 8 && encodedBits[i] == '1') {
            flip = 4;
            sbedData ^= flip;
        }
        else if (i == 4 && encodedBits[i] == '1') {
            flip = 8;
            sbedData ^= flip;
        }
    }
    return sbedData;
}


//Check mm_storebits.c
char* MM_encodeBitsAlgo(uint8 ByteValue) {
    char *encodedBits = malloc (sizeof (char) * 13);
    int bitIndex = 0;
    for (int i = 11; i >= 0; i--) {
        if (i == 11 || i == 10 || i == 8 || i == 4) {
            encodedBits[i] = '0';
            continue;
        }
        int bit = (ByteValue >> bitIndex) & 1;
        if (bit) {
            encodedBits[i] = '1';
        }
        else {
            encodedBits[i] = '0';
        }
        bitIndex++;
    }
    // CFE_EVS_SendEvent(MM_STOREBITS_EID, CFE_EVS_INFORMATION, "SBED COMMAND: different approach bits %s", encodedBits);

    int parity1 = 0;
    int parity2 = 0;
    int parity4 = 0;
    int parity8 = 0;

    int p1Counter = 0;
    int p2Counter = 0;
    int p4Counter = 0;
    int p8Counter = 0;

    for (int i = 11; i >= 0; i--) {
        if (i <= 11 && p1Counter < 2) {
            if (p1Counter < 1 && encodedBits[i] == '1') {
                parity1++;
            }
            p1Counter++;
        }

        if (i <= 10 && p2Counter < 4) {
           if (p2Counter < 2 && encodedBits[i] == '1') {
                parity2++;
            }
            p2Counter++;
        }

        if (i <= 8 && p4Counter < 8) {
           if (p4Counter < 4 && encodedBits[i] == '1') {
               parity4++;
            }
            p4Counter++;
        }

        if (i <= 4 && p8Counter < 16) {
            if (p8Counter < 8 && encodedBits[i] == '1') {
                parity8++;
            }
            p8Counter++;
        }

        if (p1Counter == 2) {
            p1Counter = 0;
        }

        if (p2Counter == 4) {
            p2Counter = 0;
        }

        if (p4Counter == 8) {
            p4Counter = 0;
        }

        if (p8Counter == 16) {
            p8Counter = 0;
        }
    }

    if (parity1 % 2 != 0) {
        encodedBits[11] = '1';
    }

    if (parity2 % 2 != 0) {
        encodedBits[10] = '1';
    }

    if (parity4 % 2 != 0) {
        encodedBits[8] = '1';
   }

    if (parity8 % 2 != 0) {
        encodedBits[4] = '1';
   }
    
    return encodedBits;
} 

/************************/
/*  End of File Comment */
/************************/