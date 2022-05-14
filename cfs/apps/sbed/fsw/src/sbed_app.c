/*******************************************************************************
** File: sbed_app.c
**
** Purpose:
**   This file contains the source code for the Sbed App.
**
*******************************************************************************/

/*
**   Include Files:
*/

#include "sbed_app.h"
#include "sbed_app_perfids.h"
#include "sbed_app_msgids.h"


/*
** System Header files
*/
#include <string.h>

/*
** SBED global data
*/
SBED_AppData_t SBED_AppData;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SBED_AppMain() -- Application entry point and main process loop   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SBED_AppMain(void)
{
    int32 Status;
    
    /*
    ** Register the Application with Executive Services
    */
    CFE_ES_RegisterApp();

    /*
    ** Create the first Performance Log entry
    */
    CFE_ES_PerfLogEntry(SBED_APPMAIN_PERF_ID);

    /*
    ** Perform application specific initialization
    ** If the Intialization fails, set the RunStatus to CFE_ES_APP_ERROR
    ** and the App will not enter the RunLoop.
    */
    Status = SBED_AppInit();
    if ( Status != CFE_SUCCESS )
    {
        SBED_AppData.RunStatus = CFE_ES_APP_ERROR;
    }

    /*
    ** Application Main Loop. Call CFE_ES_RunLoop to check for changes
    ** in the Application's status. If there is a request to kill this
    ** App, it will be passed in through the RunLoop call.
    */
    while ( CFE_ES_RunLoop(&SBED_AppData.RunStatus) == TRUE )
    {
    
        /*
        ** Performance Log Exit Stamp.
        */
        CFE_ES_PerfLogExit(SBED_APPMAIN_PERF_ID);
   
        /*
        ** Pend on the arrival of the next Software Bus message.
        */
        Status = CFE_SB_RcvMsg(&SBED_AppData.MsgPtr, SBED_AppData.CmdPipe, CFE_SB_PEND_FOREVER);
        
        /*
        ** Performance Log Entry Stamp.
        */      
        CFE_ES_PerfLogEntry(SBED_APPMAIN_PERF_ID);

        /*
        ** Check the return status from the Software Bus.
        */        
        if (Status == CFE_SUCCESS)
        {
            /*
            ** Process Software Bus message. If there are fatal errors
            ** in command processing the command can alter the global 
            ** RunStatus variable to exit the main event loop.
            */
            SBED_AppPipe(SBED_AppData.MsgPtr);
            
            /* 
            ** Update Critical Data Store. Because this data is only updated
            ** in one command, this could be moved the command processing function.
            */
            CFE_ES_CopyToCDS(SBED_AppData.CDSHandle, &SBED_AppData.WorkingCriticalData);

        }
        else
        {
            /*
            ** This is an example of exiting on an error.
            ** Note that a SB read error is not always going to 
            ** result in an app quitting. 
            */
            CFE_EVS_SendEvent(SBED_PIPE_ERR_EID, CFE_EVS_ERROR,
                               "SBED: SB Pipe Read Error, SBED App Will Exit.");
            
            SBED_AppData.RunStatus = CFE_ES_APP_ERROR;
         }
                    
    } /* end while */

    /*
    ** Performance Log Exit Stamp.
    */
    CFE_ES_PerfLogExit(SBED_APPMAIN_PERF_ID);

    /*
    ** Exit the Application.
    */
    CFE_ES_ExitApp(SBED_AppData.RunStatus);
    
} /* End of SBED_AppMain() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SBED_AppInit() -- SBED initialization                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 SBED_AppInit(void)
{
    int32  Status;
    int32  ResetType;
    uint32 ResetSubType;

    /* 
    ** Determine Reset Type
    */
    ResetType = CFE_ES_GetResetType(&ResetSubType);
    
    /*
    ** For a Poweron Reset, initialize the Critical variables.
    ** If it is a Processor Reset, these variables will be restored from
    ** the Critical Data Store later in this function.
    */
    if ( ResetType == CFE_ES_POWERON_RESET )
    {
       SBED_AppData.WorkingCriticalData.DataPtOne   = 1;
       SBED_AppData.WorkingCriticalData.DataPtTwo   = 2;
       SBED_AppData.WorkingCriticalData.DataPtThree = 3;
       SBED_AppData.WorkingCriticalData.DataPtFour  = 4;
       SBED_AppData.WorkingCriticalData.DataPtFive  = 5;
    }
    
    /*
    ** Setup the RunStatus variable
    */
    SBED_AppData.RunStatus = CFE_ES_APP_RUN;
    
    /*
    ** Initialize app command execution counters.
    */
    SBED_AppData.CmdCounter = 0;
    SBED_AppData.ErrCounter = 0;

    /*
    ** Initialize app configuration data.
    */
    strcpy(SBED_AppData.PipeName, "SBED_CMD_PIPE");
    SBED_AppData.PipeDepth = SBED_PIPE_DEPTH;

    SBED_AppData.LimitHK   = SBED_LIMIT_HK;
    SBED_AppData.LimitCmd  = SBED_LIMIT_CMD;

    /*
    ** Initialize event filter table for envents we want to filter.
    */
    SBED_AppData.EventFilters[0].EventID = SBED_PROCESS_INF_EID;
    SBED_AppData.EventFilters[0].Mask    = CFE_EVS_EVERY_FOURTH_ONE;
    
    SBED_AppData.EventFilters[1].EventID = SBED_RESET_INF_EID;
    SBED_AppData.EventFilters[1].Mask    = CFE_EVS_NO_FILTER;
    
    SBED_AppData.EventFilters[2].EventID = SBED_CC1_ERR_EID;
    SBED_AppData.EventFilters[2].Mask    = CFE_EVS_EVERY_OTHER_TWO;
    
    SBED_AppData.EventFilters[3].EventID = SBED_LEN_ERR_EID;
    SBED_AppData.EventFilters[3].Mask    = CFE_EVS_FIRST_8_STOP;


    /*
    ** Register event filter table.
    */
    Status = CFE_EVS_Register(SBED_AppData.EventFilters, 4,
                               CFE_EVS_BINARY_FILTER);
    
    if ( Status != CFE_SUCCESS )
    {
       CFE_ES_WriteToSysLog("SBED App: Error Registering Events, RC = 0x%08X\n", Status);
       return ( Status );
    }
             
    /*
    ** Initialize housekeeping packet (clear user data area).
    */
    CFE_SB_InitMsg(&SBED_AppData.HkPacket, SBED_HK_TLM_MID, sizeof(SBED_HkPacket_t), TRUE);
   
    /*
    ** Create Software Bus message pipe.
    */
    Status = CFE_SB_CreatePipe(&SBED_AppData.CmdPipe, SBED_AppData.PipeDepth, SBED_AppData.PipeName);
    if ( Status != CFE_SUCCESS )
    {
       /*
       ** Could use an event at this point
       */
       CFE_ES_WriteToSysLog("SBED App: Error Creating SB Pipe, RC = 0x%08X\n", Status);
       return ( Status );
    }

    /*
    ** Subscribe to Housekeeping request commands
    */
    Status = CFE_SB_Subscribe(SBED_SEND_HK_MID,SBED_AppData.CmdPipe);
    if ( Status != CFE_SUCCESS )
    {
       CFE_ES_WriteToSysLog("SBED App: Error Subscribing to HK Request, RC = 0x%08X\n", Status);
       return ( Status );
    }

    /*
    ** Subscribe to SBED ground command packets
    */
    Status = CFE_SB_Subscribe(SBED_CMD_MID,SBED_AppData.CmdPipe);
    if ( Status != CFE_SUCCESS )
    {
       CFE_ES_WriteToSysLog("SBED App: Error Subscribing to SBED Command, RC = 0x%08X\n", Status);
       return ( Status );
    }

    /*
    ** Register tables with cFE and load default data
    */
    Status = CFE_TBL_Register(&SBED_AppData.TblHandles[0], "MyFirstTbl",
                              sizeof(SBED_Tbl_1_t), CFE_TBL_OPT_DEFAULT,
                              SBED_FirstTblValidationFunc);
    if ( Status != CFE_SUCCESS )
    {
       CFE_ES_WriteToSysLog("SBED App: Error Registering Table 1, RC = 0x%08X\n", Status);
       return ( Status );
    }
    else
    {
       Status = CFE_TBL_Load(SBED_AppData.TblHandles[0], CFE_TBL_SRC_FILE, SBED_FIRST_TBL_FILE);
    }
    
    Status = CFE_TBL_Register(&SBED_AppData.TblHandles[1], "MySecondTbl",
                              sizeof(SBED_Tbl_2_t), CFE_TBL_OPT_DEFAULT,
                              SBED_SecondTblValidationFunc);
    if ( Status != CFE_SUCCESS )
    {
       CFE_ES_WriteToSysLog("SBED App: Error Registering Table 2, RC = 0x%08X\n", Status);
       return ( Status );
    }
    else
    {
      Status = CFE_TBL_Load(SBED_AppData.TblHandles[1], CFE_TBL_SRC_FILE, SBED_SECOND_TBL_FILE);
    }
                 

    /*
    ** Create and manage the Critical Data Store 
    */
    Status = CFE_ES_RegisterCDS(&SBED_AppData.CDSHandle, sizeof(SBED_CdsDataType_t), SBED_CDS_NAME);
    if(Status == CFE_SUCCESS)
    {
       /* 
       ** Setup Initial contents of Critical Data Store 
       */
       CFE_ES_CopyToCDS(SBED_AppData.CDSHandle, &SBED_AppData.WorkingCriticalData);
       
    }
    else if(Status == CFE_ES_CDS_ALREADY_EXISTS)
    {
       /* 
       ** Critical Data Store already existed, we need to get a copy 
       ** of its current contents to see if we can use it
       */
       Status = CFE_ES_RestoreFromCDS(&SBED_AppData.WorkingCriticalData, SBED_AppData.CDSHandle);
       if(Status == CFE_SUCCESS)
       {
          /*
          ** Perform any logical verifications, if necessary, to validate data 
          */
          CFE_ES_WriteToSysLog("SBED App CDS data preserved\n");
       }
       else
       {
          /* 
          ** Restore Failied, Perform baseline initialization 
          */
          SBED_AppData.WorkingCriticalData.DataPtOne   = 1;
          SBED_AppData.WorkingCriticalData.DataPtTwo   = 2;
          SBED_AppData.WorkingCriticalData.DataPtThree = 3;
          SBED_AppData.WorkingCriticalData.DataPtFour  = 4;
          SBED_AppData.WorkingCriticalData.DataPtFive  = 5;
          CFE_ES_WriteToSysLog("Failed to Restore CDS. Re-Initialized CDS Data.\n");
       }
    }
    else 
    {
       /* 
       ** Error creating my critical data store 
       */
       CFE_ES_WriteToSysLog("SBED: Failed to create CDS (Err=0x%08x)", Status);
       return(Status);
    }

    /*
    ** Application startup event message.
    */
    CFE_EVS_SendEvent(SBED_INIT_INF_EID,CFE_EVS_INFORMATION, "SBED: Application Initialized");
                         
    return(CFE_SUCCESS);

} /* End of SBED_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SBED_AppPipe() -- Process command pipe message                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SBED_AppPipe(CFE_SB_MsgPtr_t msg)
{
    CFE_SB_MsgId_t MessageID;
    uint16 CommandCode;

    MessageID = CFE_SB_GetMsgId(msg);
    switch (MessageID)
    {
        /*
        ** Housekeeping telemetry request.
        */
        case SBED_SEND_HK_MID:
            SBED_HousekeepingCmd(msg);
            break;

        /*
        ** SBED ground commands.
        */
        case SBED_CMD_MID:

            CommandCode = CFE_SB_GetCmdCode(msg);
            switch (CommandCode)
            {
                case SBED_NOOP_CC:
                    SBED_NoopCmd(msg);
                    break;

                case SBED_RESET_CC:
                    SBED_ResetCmd(msg);
                    break;
                    
                case SBED_PROCESS_CC:
                    SBED_RoutineProcessingCmd(msg);
                    break;

                default:
                    CFE_EVS_SendEvent(SBED_CC1_ERR_EID, CFE_EVS_ERROR,
                     "Invalid ground command code: ID = 0x%X, CC = %d",
                                      MessageID, CommandCode);
                    break;
            }
            break;

        default:

            CFE_EVS_SendEvent(SBED_MID_ERR_EID, CFE_EVS_ERROR,
                             "Invalid command pipe message ID: 0x%X",
                              MessageID);
            break;
    }

    return;

} /* End of SBED_AppPipe() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SBED_HousekeepingCmd() -- On-board command (HK request)           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SBED_HousekeepingCmd(CFE_SB_MsgPtr_t msg)
{
    uint16 ExpectedLength = sizeof(SBED_NoArgsCmd_t);
    uint16 i;

    /*
    ** Verify command packet length
    */
    if (SBED_VerifyCmdLength(msg, ExpectedLength))
    {
        /*
        ** Get command execution counters
        */
        SBED_AppData.HkPacket.CmdCounter = SBED_AppData.CmdCounter;
        SBED_AppData.HkPacket.ErrCounter = SBED_AppData.ErrCounter;

        /*
        ** Send housekeeping telemetry packet
        */
        CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &SBED_AppData.HkPacket);
        CFE_SB_SendMsg((CFE_SB_Msg_t *) &SBED_AppData.HkPacket);

        /*
        ** Manage any pending table loads, validations, etc.
        */
        for (i=0; i<SBED_NUM_TABLES; i++)
        {
            CFE_TBL_Manage(SBED_AppData.TblHandles[i]);
        }
        
        /*
        ** This command does not affect the command execution counter
        */
        
    } /* end if */

    return;

} /* End of SBED_HousekeepingCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SBED_NoopCmd() -- SBED ground command (NO-OP)                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SBED_NoopCmd(CFE_SB_MsgPtr_t msg)
{
    uint16 ExpectedLength = sizeof(SBED_NoArgsCmd_t);

    /*
    ** Verify command packet length...
    */
    if (SBED_VerifyCmdLength(msg, ExpectedLength))
    {
        SBED_AppData.CmdCounter++;

        CFE_EVS_SendEvent(SBED_NOOP_INF_EID, CFE_EVS_INFORMATION,
                         "SBED Version 1.0.0: No-op command");
    }

    return;

} /* End of SBED_NoopCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SBED_ResetCmd() -- SBED ground command (reset counters)             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SBED_ResetCmd(CFE_SB_MsgPtr_t msg)
{
    uint16 ExpectedLength = sizeof(SBED_NoArgsCmd_t);

    /*
    ** Verify command packet length...
    */
    if (SBED_VerifyCmdLength(msg, ExpectedLength))
    {
        SBED_AppData.CmdCounter = 0;
        SBED_AppData.ErrCounter = 0;

        CFE_EVS_SendEvent(SBED_RESET_INF_EID, CFE_EVS_INFORMATION,
                         "SBED: Reset Counters command");
    }

    return;

} /* End of SBED_ResetCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  SBED_RoutineProcessingCmd() - Common Processing for each cmd.    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SBED_RoutineProcessingCmd(CFE_SB_MsgPtr_t msg)
{
    uint16 ExpectedLength = sizeof(SBED_NoArgsCmd_t);
    SBED_Tbl_1_t   *MyFirstTblPtr;
    SBED_Tbl_2_t   *MySecondTblPtr;

    /*
    ** Verify command packet length
    */
    if (SBED_VerifyCmdLength(msg, ExpectedLength))
    {
        /* Obtain access to table data addresses */
        CFE_TBL_GetAddress((void *)&MyFirstTblPtr, SBED_AppData.TblHandles[0]);
        CFE_TBL_GetAddress((void *)&MySecondTblPtr, SBED_AppData.TblHandles[1]);
        
        /* Perform routine processing accessing table data via pointers */
        /*                            .                                 */
        /*                            .                                 */
        /*                            .                                 */
        
        /* Once completed with using tables, release addresses          */
        CFE_TBL_ReleaseAddress(SBED_AppData.TblHandles[0]);
        CFE_TBL_ReleaseAddress(SBED_AppData.TblHandles[1]);
        
        /*
        ** Update Critical variables. These variables will be saved
        ** in the Critical Data Store and preserved on a processor reset.
        */
        SBED_AppData.WorkingCriticalData.DataPtOne++;
        SBED_AppData.WorkingCriticalData.DataPtTwo++;
        SBED_AppData.WorkingCriticalData.DataPtThree++;
        SBED_AppData.WorkingCriticalData.DataPtFour++;
        SBED_AppData.WorkingCriticalData.DataPtFive++;
        
        CFE_EVS_SendEvent(SBED_PROCESS_INF_EID,CFE_EVS_INFORMATION, "SBED: Routine Processing Command");
        
        SBED_AppData.CmdCounter++;
    }

    return;

} /* End of SBED_RoutineProcessingCmd() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SBED_VerifyCmdLength() -- Verify command packet length            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

boolean SBED_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength)
{
    boolean result = TRUE;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(msg);

    /*
    ** Verify the command packet length...
    */
    if (ExpectedLength != ActualLength)
    {
        CFE_SB_MsgId_t MessageID = CFE_SB_GetMsgId(msg);
        uint16 CommandCode = CFE_SB_GetCmdCode(msg);

        CFE_EVS_SendEvent(SBED_LEN_ERR_EID, CFE_EVS_ERROR,
           "SBED: Invalid cmd pkt: ID = 0x%X,  CC = %d, Len = %d",
                          MessageID, CommandCode, ActualLength);
        result = FALSE;
        SBED_AppData.ErrCounter++;
    }

    return(result);

} /* End of SBED_VerifyCmdLength() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SBED_FirstTblValidationFunc() -- Verify contents of First Table   */
/*                                buffer contents                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 SBED_FirstTblValidationFunc(void *TblData)
{
    int32              ReturnCode = CFE_SUCCESS;
    SBED_Tbl_1_t *TblDataPtr = (SBED_Tbl_1_t *)TblData;
    
    if (TblDataPtr->TblElement1 > SBED_TBL_ELEMENT_1_MAX)
    {
        /* First element is out of range, return an appropriate error code */
        ReturnCode = SBED_TBL_1_ELEMENT_OUT_OF_RANGE_ERR_CODE;
    }
    
    return ReturnCode;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SBED_SecondTblValidationFunc() -- Verify contents of Second Table */
/*                                 buffer contents                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 SBED_SecondTblValidationFunc(void *TblData)
{
    int32               ReturnCode = CFE_SUCCESS;
    SBED_Tbl_2_t *TblDataPtr = (SBED_Tbl_2_t *)TblData;
    
    if (TblDataPtr->TblElement3 > SBED_TBL_ELEMENT_3_MAX)
    {
        /* Third element is out of range, return an appropriate error code */
        ReturnCode = SBED_TBL_2_ELEMENT_OUT_OF_RANGE_ERR_CODE;
    }
    

    return ReturnCode;
}

/************************/
/*  End of File Comment */
/************************/
