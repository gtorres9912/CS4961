/*************************************************************************
** File:
**   $Id: mm_storebits.c
**
** Purpose: 
**   Functions used to detect the Single Bit Error Anomaly
**
*************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/
#include "mm_app.h"
#include "mm_events.h"
#include "mm_storebits.h"
#include "cfs_utils.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/*************************************************************************
** External Data
*************************************************************************/
extern MM_AppData_t MM_AppData;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Single Bit Error Detect Command                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MM_STOREBITS_Cmd(CFE_SB_MsgPtr_t msg) {
    // Create initial parity bits
    uint32 createInitialBits = MM_createInitialBits(msg);
    // Store inital parity bits in SBED_AppData
    MM_writeToMem(msg, createInitialBits);

} /* end MM_SBED_DetectCmd */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Converts the byte at the data address to binary                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint32 MM_createInitialBits(CFE_SB_MsgPtr_t MessagePtr) {
    //Read the data from SBEI_AppData size of 8 bits
    uint8 sbeiByteValue = 0;
    uint32 sbeiDestAddress;
    CFS_SymAddr_t sbeiDestSymAddress;
    char sbeiAddress[] = "SBEI_AppData";

    strcpy(sbeiDestSymAddress.SymName, sbeiAddress);

    CFS_ResolveSymAddr(&sbeiDestSymAddress, &sbeiDestAddress);
    CFE_PSP_MemRead8(sbeiDestAddress, &sbeiByteValue);

    //Create encoded bits from the data read (encoded bits will total 12 bits)
    char* encodedBits = MM_encodeBits(sbeiByteValue);
    // CFE_EVS_SendEvent(MM_STOREBITS_EID, CFE_EVS_INFORMATION, "SBED COMMAND: encoded bits %s", encodedBits); 

    //In 32 bits change the 16th - 19th bits if the partiy bit at a location is 1
    //These will be the bits written to SBEI_AppData
    uint32 sbedData = 0;
    uint32 flip = 0;
    for (int i = 11; i >= 0; i--) {
        if (i == 11 && encodedBits[i] == '1') {
            flip = 65536;
            sbedData ^= flip;
        }
        else if (i == 10 && encodedBits[i] == '1') {
            flip = 131072;
            sbedData ^= flip;
        }
        else if (i == 8 && encodedBits[i] == '1') {
            flip = 262144;
            sbedData ^= flip;
        }
        else if (i == 4 && encodedBits[i] == '1') {
            flip = 524288;
            sbedData ^= flip;
        }
    }

    // CFE_EVS_SendEvent(MM_STOREBITS_EID, CFE_EVS_INFORMATION, "SBED COMMAND: parity bits %X", sbedData);
    return sbedData;
}

void MM_writeToMem(CFE_SB_MsgPtr_t MessagePtr, uint32 byteValue) {
    // Write the parity bits to SBED_AppData
    int32      PSP_Status;
    uint32 DestAddress;
    CFS_SymAddr_t     DestSymAddress;
    char address[] = "SBED_AppData";
    strcpy(DestSymAddress.SymName, address);
    CFS_ResolveSymAddr(&DestSymAddress, &DestAddress);
    PSP_Status = CFE_PSP_MemWrite32(DestAddress, byteValue);

    if (PSP_Status == CFE_PSP_SUCCESS) {
        CFE_EVS_SendEvent(MM_STOREBITS_EID, CFE_EVS_INFORMATION, "SBED COMMAND: Current parity bits stored in memory");
    }
    else {
        CFE_EVS_SendEvent(MM_STOREBITS_EID, CFE_EVS_INFORMATION, "SBED COMMAND: failed");
    }
    return;
}

char* MM_encodeBits(uint8 ByteValue) {
    // Algorithm to create the encoded bits
    char *encodedBits = malloc (sizeof (char) * 13);
    int bitIndex = 0;
    for (int i = 11; i >= 0; i--) {
        //Indices for parity will be set to 0
        if (i == 11 || i == 10 || i == 8 || i == 4) {
            encodedBits[i] = '0';
            continue;
        }
        //shift bits by 1 to the right 
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
        //Check every other bit
        if (i <= 11 && p1Counter < 2) {
            if (p1Counter < 1 && encodedBits[i] == '1') {
                parity1++;
            }
            p1Counter++;
        }

        //Check every other 2 bits
        if (i <= 10 && p2Counter < 4) {
           if (p2Counter < 2 && encodedBits[i] == '1') {
                parity2++;
            }
            p2Counter++;
        }

        //Check every other 4 bits
        if (i <= 8 && p4Counter < 8) {
           if (p4Counter < 4 && encodedBits[i] == '1') {
               parity4++;
            }
            p4Counter++;
        }

        //Check every other 8 bits
        if (i <= 4 && p8Counter < 16) {
            if (p8Counter < 8 && encodedBits[i] == '1') {
                parity8++;
            }
            p8Counter++;
        }

        //Resetting the counters 
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

    //If the counter variable for a certain parity bit is even then in the encoded bits it'll be 1 if it's even it'll be 0
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


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Converts the bit to a Hex value		                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/************************/
/*  End of File Comment */
/************************/