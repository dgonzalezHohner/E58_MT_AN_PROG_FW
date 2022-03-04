/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _PUSH_BUTTONS_H    /* Guard against multiple inclusion */
#define _PUSH_BUTTONS_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */
    #define NUMBER_PUSH_BUTTONS 4       //2 push buttons + 2 cable inputs
    #define DEB_TIME_ms         50
    #define DEB_TIMER_set       (((DEB_TIME_ms*1000)/(SYSTICK_INTERRUPT_PERIOD_IN_US))+1)
    
    //Push Buttons scaling timers
    #define PB_SCALE_CFG_T_ms   15000
    #define PB_SCALE_CFG_T_set  (((PB_SCALE_CFG_T_ms*1000)/(SYSTICK_INTERRUPT_PERIOD_IN_US))+1)

    #define PB_DEF_CFG_T_ms     30000
    #define PB_DEF_CFG_T_set    (((PB_DEF_CFG_T_ms*1000)/(SYSTICK_INTERRUPT_PERIOD_IN_US))+1)

    #define PB_SET_CFG_T_ms     2000
    #define PB_SET_CFG_T_set    (((PB_SET_CFG_T_ms*1000)/(SYSTICK_INTERRUPT_PERIOD_IN_US))+1)
    
    //Contact scaling timers
//    #define CT_SCALE_CFG_T_ms   1500
//    #define CT_SCALE_CFG_T_set  (((CT_SCALE_CFG_T_ms*1000)/(SYSTICK_INTERRUPT_PERIOD_IN_US))+1)

    #define CT_DEF_CFG_T_ms     3000
    #define CT_DEF_CFG_T_set    (((CT_DEF_CFG_T_ms*1000)/(SYSTICK_INTERRUPT_PERIOD_IN_US))+1)

    #define CT_SET_CFG_T_ms     1500
    #define CT_SET_CFG_T_set    (((CT_SET_CFG_T_ms*1000)/(SYSTICK_INTERRUPT_PERIOD_IN_US))+1)
    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************

    /** Descriptive Data Type Name

      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
    typedef enum
    {
        WAIT_PB_CT = (uint8_t)0,
        BOTH_PBS_ON,
        BOTH_PBS_OFF,
        PB1_ON_FIRST,
        PB1_OFF_FIRST,
        PB2_ON_LAST,
        PB2_OFF_LAST,
        PB2_ON_FIRST,
        PB2_OFF_FIRST,
        PB1_ON_LAST,
        PB1_OFF_LAST,
                
        CT1_ON_FIRST,
        CT1_OFF_FIRST,
        CT2_ON_LAST,
        CT2_OFF_LAST,
        CT2_ON_FIRST,
        CT2_OFF_FIRST,
        CT1_ON_LAST,
        CT1_OFF_LAST,
        BOTH_CTS_ON,
        WAIT_BOTH_CTS_OFF,
        RESTORE_DEF
    }PBfsmType;
    
    typedef struct
    {
        /* Describe structure member. */
        uint8_t DebTimer;
        union _PBStatus
        {
            uint8_t Value;
            struct _Bitfield
            {
                uint8_t PinStat :1;
                uint8_t DebStat :1;
            }Bitfield;
        }PBStatus;
    } PBType;

    typedef struct __PBGroup
    {
        uint8_t length;
        PBType* pPBData;
        void (*pPBGroupInit)(struct __PBGroup* pPBData);
        void (*pPBGroupDeInit)(struct __PBGroup* pPBData);
    }PBGroupType;
    
    PBGroupType* pPushButtons;

    #define PB1_CURRENT_VAL pPushButtons->pPBData[0].PBStatus.Bitfield.PinStat
    #define PB1_DEB_VAL pPushButtons->pPBData[0].PBStatus.Bitfield.DebStat
    #define PB2_CURRENT_VAL pPushButtons->pPBData[1].PBStatus.Bitfield.PinStat
    #define PB2_DEB_VAL pPushButtons->pPBData[1].PBStatus.Bitfield.DebStat
    #define SET1_CURRENT_VAL pPushButtons->pPBData[2].PBStatus.Bitfield.PinStat
    #define SET1_DEB_VAL pPushButtons->pPBData[2].PBStatus.Bitfield.DebStat
    #define SET2_CURRENT_VAL pPushButtons->pPBData[3].PBStatus.Bitfield.PinStat
    #define SET2_DEB_VAL pPushButtons->pPBData[3].PBStatus.Bitfield.DebStat
    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************
    /**
      @Function
        int ExampleFunctionName ( int param1, int param2 ) 

      @Summary
        Brief one-line description of the function.

      @Description
        Full description, explaining the purpose and usage of the function.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Precondition
        List and describe any required preconditions. If there are no preconditions,
        enter "None."

      @Parameters
        @param param1 Describe the first parameter to the function.
    
        @param param2 Describe the second parameter to the function.

      @Returns
        List (if feasible) and describe the return values of the function.
        <ul>
          <li>1   Indicates an error occurred
          <li>0   Indicates an error did not occur
        </ul>

      @Remarks
        Describe any special behavior not described above.
        <p>
        Any additional remarks.

      @Example
        @code
        if(ExampleFunctionName(1, 2) == 0)
        {
            return 3;
        }
     */

//    int ExampleFunction(int param1, int param2);

    void InitPushButtonData (PBGroupType* pPBGroup);
    void DenitPushButtonData (PBGroupType* pPBGroup);
    void PBInit (PBGroupType** pPBGroup, uint8_t length);
    void PBFree (PBGroupType** pPBGroup);
    void PushButtonsTask();
    void PBDebTimerTask();
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _PUSH_BUTTONS_H */

/* *****************************************************************************
 End of File
 */
