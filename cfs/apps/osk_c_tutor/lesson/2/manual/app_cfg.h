/* LESSON 2 - Manual Updates
** 
** Make the changes below to osk_c_tutor/fsw/src/app_cfg.h created
** in lesson 1.
**
*/

/* >>>>>>>>>>>>>>> STEP 1 <<<<<<<<<<<<<<<
**
** Update minor version to the lesson number. Helps confirm lesson updates
** sucessfully built and loaded.  
*/

#define  OSK_C_DEMO_MINOR_VER   2  /* Lesson ID */


/* >>>>>>>>>>>>>>> STEP 2 <<<<<<<<<<<<<<<
**
**  In the "Command Macro" section add the following command function codes
**  for the Message Log commands. The child comments indicate these commands
**  will be performed by a child task. This is done in a later lesson.
*/

#define MSGLOG_START_LOG_CMD_FC     (CMDMGR_APP_START_FC + 2) /* Child */
#define MSGLOG_STOP_LOG_CMD_FC      (CMDMGR_APP_START_FC + 3) /* Child */
#define MSGLOG_START_PLAYBK_CMD_FC  (CMDMGR_APP_START_FC + 4) /* Child */
#define MSGLOG_STOP_PLAYBK_CMD_FC   (CMDMGR_APP_START_FC + 5) /* Child */


/* >>>>>>>>>>>>>>> STEP 3 <<<<<<<<<<<<<<<
**
**  In the "Event Macro" section add the following definition. This defines
**  the base ID for events generated by the Messge Log object.
*/

#define MSGLOG_BASE_EID      (OSK_C_FW_APP_BASE_EID + 20)

