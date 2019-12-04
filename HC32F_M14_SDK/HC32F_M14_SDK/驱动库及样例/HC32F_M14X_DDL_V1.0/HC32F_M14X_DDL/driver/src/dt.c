/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */
/******************************************************************************/
/** \file Dt.c
 **
 ** A detailed description is available at
 ** @link SampleGroup Dt description @endlink
 **
 **   - 2017-08-07  1.0  HeChun First version for Device Driver Library of Dt.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "dt.h"

/**
 *******************************************************************************
 ** \addtogroup DtGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief DT instance data type
 ******************************************************************************/
typedef struct stc_dt_instance_data
{
    uint32_t            u32Idx;             ///< channel index
    M0P_DTIM_TypeDef    *pstcInstance;      ///< pointer to registers of an instance
    func_ptr_t          pfnIrqCallback;     ///< module internal data of instance
} stc_dt_instance_data_t;

/**
 *******************************************************************************
 ** \brief Look-up table for all DT instances
 ******************************************************************************/
static stc_dt_instance_data_t m_astcDtInstanceDataLut[] =
{
    {
        1,
        M0P_DTIM1,
        NULL
    },
    {
        2,
        M0P_DTIM2,
        NULL
    }
};

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_CH(x)                  \
(   (DtChannel1 == (x)) ||              \
    (DtChannel2 == (x)) )

#define IS_VALID_MODE(x)                \
(   (DtModeFreeRun == (x)) ||           \
    (DtModeOneShot == (x)) ||           \
    (DtModePeriodic == (x)) )

#define IS_VALID_PRESCALER(x)           \
(   (DtPrescalerDiv1 == (x)) ||         \
    (DtPrescalerDiv16 == (x)) ||        \
    (DtPrescalerDiv256 == (x)) )

#define IS_VALID_CNT_SZ(x)              \
(   (DtCounterSize16 == (x)) ||         \
    (DtCounterSize32 == (x)) )


/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void DtEnableNvic(void);
static void DtDisableNvic(void);
static stc_dt_instance_data_t* DtGetInternDataPtr(uint8_t u8ch);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief
 **
 ** \param [in] u8ch Channel index
 **
 ** \retval stc_dt_instance_data_t Pointer to a Instance
 **         data
 **         NULL fail to get a instance.
 **
 ******************************************************************************/
static stc_dt_instance_data_t* DtGetInternDataPtr(uint8_t u8ch)
{
    stc_dt_instance_data_t *pstcData = NULL;
    uint32_t i = 0;

    for (i = 0; i < ARRAY_SZ(m_astcDtInstanceDataLut); i++)
    {
        if (u8ch == m_astcDtInstanceDataLut[i].u32Idx)
        {
            pstcData = &m_astcDtInstanceDataLut[i];
            break;
        }
    }

    return pstcData;
}

/**
 *******************************************************************************
 ** \brief DtEnableNvic
 **        Enable chip NVIC of DT
 **
 ******************************************************************************/
static void DtEnableNvic(void)
{
    NVIC_ClearPendingIRQ(DTIM_IRQn);
    NVIC_EnableIRQ(DTIM_IRQn);
    NVIC_SetPriority(DTIM_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 *******************************************************************************
 ** \brief DtDisableNvic 
 **        Disable chip NVIC of DT 
 **
 ******************************************************************************/
static void DtDisableNvic(void)
{
    NVIC_ClearPendingIRQ(DTIM_IRQn);
    NVIC_DisableIRQ(DTIM_IRQn);
    NVIC_SetPriority(DTIM_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 *******************************************************************************
 ** \brief ISR callback for DT (channel 1 and 2)
 **         DT isr callback function. clear the irq flag and call user callback
 **         function
 **
 ** \param [in] u8ch Channel number
 **
 ******************************************************************************/
void Dt_IrqHandler(uint8_t u8ch)
{
    stc_dt_instance_data_t *pstcData = NULL;
    pstcData = DtGetInternDataPtr(u8ch);
    ASSERT(NULL != pstcData);
    Dt_ClrIrqFlag(u8ch);
    if (NULL != pstcData->pfnIrqCallback)
    {
        pstcData->pfnIrqCallback();
    }
}

/**
 *******************************************************************************
 ** \brief Initialize DT
 **
 ** This function initializes the specified channel of Dual Timer.
 **
 ** \param [in]  u8ch Channel index
 ** \param [in]  pstcConfig Pointer to config data
 **
 ** \retval Ok Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - pstcConfig == NULL
 **                             - u8ch is invalid
 **                             - pstcConfig has invalid data
 **
 ******************************************************************************/
en_result_t Dt_Init(uint8_t u8ch, stc_dt_channel_config_t *pstcConfig)
{
    en_result_t enResult = Error;
    M0P_DTIM_TypeDef *pstcDt = NULL;
    stc_dt_instance_data_t *pstcData = NULL;
    stc_dtim_timerctrl_field_t stcTIMERCTRL = { 0u };

    ASSERT(NULL != pstcConfig);
    ASSERT(IS_VALID_CH(u8ch));
    ASSERT(IS_VALID_MODE(pstcConfig->enMode));
    ASSERT(IS_VALID_PRESCALER(pstcConfig->enClkDiv));
    ASSERT(IS_VALID_CNT_SZ(pstcConfig->enCounterSize));

    pstcData = DtGetInternDataPtr(u8ch);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    pstcDt = pstcData->pstcInstance;

    /* Disable everything */
    pstcDt->TIMERCTRL = 0u;

    /* Set Timer Mode */
    switch (pstcConfig->enMode)
    {
        /* Free run mode */
        case DtModeFreeRun:
            stcTIMERCTRL.TMRMODE = FALSE;
            stcTIMERCTRL.ONESHOT = FALSE;
            break;
        /* Periodic mode */
        case DtModePeriodic:
            stcTIMERCTRL.TMRMODE = TRUE;
            stcTIMERCTRL.ONESHOT = FALSE;
            break;
        /* One shot mode */
        case DtModeOneShot:
            stcTIMERCTRL.TMRMODE = FALSE;
            stcTIMERCTRL.ONESHOT = TRUE;
            break;
        default:
            break;
    }

    stcTIMERCTRL.TMRPRE = pstcConfig->enClkDiv;
    stcTIMERCTRL.TMRSIZE = pstcConfig->enCounterSize;

    if (TRUE == pstcConfig->bIrqEnable )
    {
        stcTIMERCTRL.INTEN = 1u;
    }

    /* Set interrupt callback function */
    pstcData->pfnIrqCallback = pstcConfig->pfnIrqCallback;

    /* Set NVIC */
    if (TRUE == pstcConfig->bTouchNvic )
    {
        DtEnableNvic();
    }

    /* Set control register */
    pstcDt->TIMERCTRL_f = stcTIMERCTRL;

    enResult = Ok;

    return enResult;
}

/**
 *******************************************************************************
 ** \brief De-Initialize DT
 **
 ** This function de-initializes the specified channel of Dual Timer.
 ** Dt-DeInit() accesses the DT hardware register. They are reset.
 **
 ** \param [in]  u8ch Channel index
 ** \param [in]  bTouchNvic Touch NVIC or not
 **
 ** \retval Ok Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **         - u8ch is invalid
 **
 ******************************************************************************/
en_result_t Dt_DeInit(uint8_t u8ch, boolean_t bTouchNvic)
{
    en_result_t enResult = Ok;
    M0P_DTIM_TypeDef *pstcDt = NULL;
    stc_dt_instance_data_t *pstcData = NULL;

    pstcData = DtGetInternDataPtr(u8ch);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    pstcDt = pstcData->pstcInstance;

    pstcDt->TIMERCTRL    = 0u;
    pstcDt->TIMERLOAD    = 0u;
    pstcDt->TIMERINTCLR  = 1u;
    pstcDt->TIMERBGLOAD  = 0u;

    pstcData->pfnIrqCallback = NULL;

    if (TRUE == bTouchNvic)
    {
        DtDisableNvic();
    }

    return enResult;
}

/**
 *******************************************************************************
 ** \brief Enable Timer Counter
 **
 ** This function enables the timer counter.
 **
 ** \param [in]  u8ch Channel index
 **
 ** \retval Ok  Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8ch is invalid
 **
 ******************************************************************************/
en_result_t Dt_EnableCount(uint8_t u8ch)
{
    en_result_t enResult = Ok;
    stc_dt_instance_data_t *pstcData = NULL;

    pstcData = DtGetInternDataPtr(u8ch);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    pstcData->pstcInstance->TIMERCTRL_f.TMREN = TRUE;

    return enResult;
}

/**
 *******************************************************************************
 ** \brief Disable Timer Counter
 **
 ** This function disables the timer counter.
 **
 ** \param [in]  u8ch Channel index
 **
 ** \retval Ok Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                                 - u8ch is invalid
 **
 ******************************************************************************/
en_result_t Dt_DisableCount(uint8_t u8ch)
{
    en_result_t enResult = Ok;
    stc_dt_instance_data_t *pstcData = NULL;

    pstcData = DtGetInternDataPtr(u8ch);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    pstcData->pstcInstance->TIMERCTRL_f.TMREN = FALSE;

    return enResult;
}

/**
 *******************************************************************************
 ** \brief Enable Interrupt
 **
 ** This function enables the interruption.
 **
 ** \param [in]  u8ch Channel index
 **
 ** \retval Ok Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8ch is invalid
 **
 ******************************************************************************/
en_result_t Dt_EnableIrq(uint8_t u8ch)
{
    en_result_t enResult = Ok;
    stc_dt_instance_data_t *pstcData = NULL;

    pstcData = DtGetInternDataPtr(u8ch);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    pstcData->pstcInstance->TIMERCTRL_f.INTEN = TRUE;

    return enResult;
}

/**
 *******************************************************************************
 ** \brief Disable Interrupt
 **
 ** This function disables the interruption.
 **
 ** \param [in]  u8Id Channel index
 **
 ** \retval Ok Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                                 - u8ch is invalid.
 **
 ******************************************************************************/
en_result_t Dt_DisableIrq(uint8_t u8ch)
{
    en_result_t enResult = Ok;
    stc_dt_instance_data_t *pstcData = NULL;

    pstcData = DtGetInternDataPtr(u8ch);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    pstcData->pstcInstance->TIMERCTRL_f.INTEN = FALSE;

    return enResult;
}

/**
 *******************************************************************************
 ** \brief Get interrupt status
 ** The Function can return the interrupt status (TimerXRIS)
 **
 ** \param [in]  u8ch Channel index
 **
 ** \retval boolean_t the interrupt status
 **
 ******************************************************************************/
boolean_t Dt_GetIrqFlag(uint8_t u8ch)
{
    stc_dt_instance_data_t *pstcData = NULL;
    boolean_t bRetVal = FALSE;

    pstcData = DtGetInternDataPtr(u8ch);
    if (NULL == pstcData)
    {
        return FALSE;
    }

    bRetVal = pstcData->pstcInstance->TIMERRIS_f.TMRINTST;

    return bRetVal;

}

/**
 *******************************************************************************
 ** \brief Get mask interrupt status
 ** The Function can return the mask interrupt status (TimerXMIS)
 **
 ** \param [in] u8ch Channel index
 **
 ** \retval boolean_t:the mask interrupt status
 **
 ******************************************************************************/
boolean_t Dt_GetMaskIrqFlag(uint8_t u8ch)
{
    stc_dt_instance_data_t *pstcData = NULL;
    boolean_t bRetVal = FALSE;

    pstcData = DtGetInternDataPtr(u8ch);
    if (NULL == pstcData)
    {
        return FALSE;
    }

    bRetVal = pstcData->pstcInstance->TIMERMIS_f.TMRMINTST;

    return bRetVal;
}

/**
 *******************************************************************************
 ** \brief Clear interrupt status
 ** The Function clears the interrupt status
 **
 ** \param [in]  u8ch Channel index
 **
 ** \retval Ok Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8ch is invalid
 **
 ******************************************************************************/
en_result_t Dt_ClrIrqFlag(uint8_t u8ch)
{
    en_result_t enResult  = Ok;
    stc_dt_instance_data_t *pstcData = NULL;

    pstcData = DtGetInternDataPtr(u8ch);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    pstcData->pstcInstance->TIMERINTCLR = TRUE;

    return enResult;
}

/**
 *******************************************************************************
 ** \brief Write load value
 ** The Function writes the load value to load register
 **
 ** \param [in]  u8ch Channel index
 ** \param [in]  u32Val Load value to set to load
 **        register
 **
 ** \retval Ok Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8ch is invalid
 **
 ******************************************************************************/
en_result_t Dt_WriteLoadVal(uint8_t u8ch, uint32_t u32Val)
{
    en_result_t enResult = Ok;
    stc_dt_instance_data_t *pstcData = NULL;

    pstcData = DtGetInternDataPtr(u8ch);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    /* if 16bit mode */
    if (FALSE == pstcData->pstcInstance->TIMERCTRL_f.TMRSIZE)
    {
        u32Val &= 0x0000FFFFu;
    }

    pstcData->pstcInstance->TIMERLOAD = u32Val;

    return enResult;

}

/**
 *******************************************************************************
 ** \brief Write back-ground load value
 ** The Function writes the load value to back-ground load register
 **
 ** \param [in]  u8ch Channel index
 ** \param [in]  u32Val Load value to set to back-ground
 **        load register
 **
 ** \retval Ok Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8ch is invalid
 **
 ******************************************************************************/
en_result_t Dt_WriteBgLoadVal(uint8_t  u8ch,uint32_t u32Val)
{
    en_result_t enResult = Ok;
    stc_dt_instance_data_t *pstcData = NULL;

    pstcData = DtGetInternDataPtr(u8ch);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    /* if 16bit mode */
    if (FALSE == pstcData->pstcInstance->TIMERCTRL_f.TMRSIZE)
    {
        u32Val &= 0x0000FFFFu;
    }

    pstcData->pstcInstance->TIMERBGLOAD = u32Val;

    return enResult;
}

/**
 *******************************************************************************
 ** \brief Read current count value
 ** The Function reads the value from value register
 **
 ** \param [in]  u8ch Channel index
 **
 ** \retval uint32_t:current counter value
 **
 ******************************************************************************/
uint32_t Dt_ReadCurCntVal(uint8_t u8ch)
{
    stc_dt_instance_data_t *pstcData = NULL;
    uint32_t u32Val = 0;

    pstcData = DtGetInternDataPtr(u8ch);
    if (NULL == pstcData)
    {
        return 0;
    }

    u32Val = pstcData->pstcInstance->TIMERVALUE;
    /* 16bit mode */
    if (FALSE == pstcData->pstcInstance->TIMERCTRL_f.TMRSIZE)
    {
        u32Val &= 0x0000FFFFu;
    }

    return u32Val;
}

//@} // DtGroup


/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
