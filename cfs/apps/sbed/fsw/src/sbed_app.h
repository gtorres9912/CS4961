/*******************************************************************************
** File: sbed_app.h
**
** Purpose:
**   This file is main hdr file for the SBED application.
**
**
*******************************************************************************/

#ifndef _sbed_app_h_
#define _sbed_app_h_

/*
**   Include Files:
*/

#include "cfe.h"
#include "sbed_tbldefs.h"


/*
** Event message ID's.
*/
#define SBED_INIT_INF_EID      1    /* start up message "informational" */

#define SBED_NOOP_INF_EID      2    /* processed command "informational" */
#define SBED_RESET_INF_EID     3
#define SBED_PROCESS_INF_EID   4
  
#define SBED_MID_ERR_EID       5    /* invalid command packet "error" */
#define SBED_CC1_ERR_EID       6
#define SBED_LEN_ERR_EID       7
#define SBED_PIPE_ERR_EID      8

#define SBED_EVT_COUNT         8    /* count of event message ID's */

/*
** Command packet command codes
*/
#define SBED_NOOP_CC           0    /* no-op command */
#define SBED_RESET_CC          1    /* reset counters */
#define SBED_PROCESS_CC        2    /* Perform Routine Processing */

/*
** Table defines
*/
#define SBED_NUM_TABLES        2    /* Number of Tables */

#define SBED_FIRST_TBL_FILE  "/cf/sbed_tbl_1.tbl"
#define SBED_SECOND_TBL_FILE "/cf/sbed_tbl_2.tbl"

#define SBED_TBL_1_ELEMENT_OUT_OF_RANGE_ERR_CODE    1  
#define SBED_TBL_2_ELEMENT_OUT_OF_RANGE_ERR_CODE   -1

#define SBED_TBL_ELEMENT_1_MAX  10  
#define SBED_TBL_ELEMENT_3_MAX  30  

/*
** Software Bus defines
*/
#define SBED_PIPE_DEPTH        12   /* Depth of the Command Pipe for Application */
#define SBED_LIMIT_HK          2    /* Limit of HouseKeeping Requests on Pipe for Application */
#define SBED_LIMIT_CMD         4    /* Limit of Commands on pipe for Application */

/*
** SBED Critical Data Store defines
*/
#define SBED_CDS_NAME            "SBEDCDS"


/*
** Type definition (Critical Data Store data)
*/
typedef struct
{
  uint32  DataPtOne;    /* Values stored in my CDS */
  uint32  DataPtTwo;
  uint32  DataPtThree;
  uint32  DataPtFour;
  uint32  DataPtFive;

} SBED_CdsDataType_t;


/*************************************************************************/

/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
  uint8                 CmdHeader[CFE_SB_CMD_HDR_SIZE];

} SBED_NoArgsCmd_t;

/*************************************************************************/

/*
** Type definition (SBED housekeeping)
*/
typedef struct
{
  uint8                 TlmHeader[CFE_SB_TLM_HDR_SIZE];

  /*
  ** Command interface counters
  */
  uint8                 CmdCounter;
  uint8                 ErrCounter;

} OS_PACK SBED_HkPacket_t;

/*************************************************************************/

/*
** Type definition (SBED app global data)
*/
typedef struct
{
  /*
  ** Command interface counters.
  */
  uint8                 CmdCounter;
  uint8                 ErrCounter;

  /*
  ** Housekeeping telemetry packet
  */
  SBED_HkPacket_t         HkPacket;

  /*
  ** Operational data (not reported in housekeeping).
  */
  CFE_SB_MsgPtr_t       MsgPtr;
  CFE_SB_PipeId_t       CmdPipe;
  
  /*
  ** RunStatus variable used in the main processing loop
  */
  uint32                RunStatus;

  /*
  ** Critical Data store variables
  */
  SBED_CdsDataType_t      WorkingCriticalData; /* Define a copy of critical data that can be used during Application execution */
  CFE_ES_CDSHandle_t    CDSHandle;           /* Handle to CDS memory block */

  /*
  ** Initialization data (not reported in housekeeping)
  */
  char                  PipeName[16];
  uint16                PipeDepth;

  uint8                 LimitHK;
  uint8                 LimitCmd;

  CFE_EVS_BinFilter_t   EventFilters[SBED_EVT_COUNT];
  CFE_TBL_Handle_t      TblHandles[SBED_NUM_TABLES];

} SBED_AppData_t;

/*************************************************************************/

/*
** Local function prototypes
**
** Note: Except for the entry point (SBED_AppMain), these
**       functions are not called from any other source module.
*/
void    SBED_AppMain(void);
int32   SBED_AppInit(void);
void    SBED_AppPipe(CFE_SB_MsgPtr_t msg);

void    SBED_HousekeepingCmd(CFE_SB_MsgPtr_t msg);

void    SBED_NoopCmd(CFE_SB_MsgPtr_t msg);
void    SBED_ResetCmd(CFE_SB_MsgPtr_t msg);
void    SBED_RoutineProcessingCmd(CFE_SB_MsgPtr_t msg);

boolean SBED_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength);

int32   SBED_FirstTblValidationFunc(void *TblData);
int32   SBED_SecondTblValidationFunc(void *TblData);


#endif /* _sbed_app_h_ */

/************************/
/*  End of File Comment */
/************************/



