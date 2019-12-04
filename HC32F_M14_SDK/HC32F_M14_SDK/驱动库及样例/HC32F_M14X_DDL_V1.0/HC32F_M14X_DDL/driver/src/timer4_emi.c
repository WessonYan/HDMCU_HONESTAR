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
/** \file timer4_emi.c
 **
 ** A detailed description is available at
 ** @link Timer4EmiGroup Timer4Emi description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     Timer4Emi
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "timer4_emi.h"

/**
 *******************************************************************************
 ** \addtogroup Timer4EmiGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/


/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_POINTER(x)  (NULL != (x))  ///< Parameter validity check for pointer

/*! Parameter validity check for enable flag  */
#define IS_VALID_EN(x)                  \
(   (TRUE == (x)) ||                    \
    (FALSE == (x)))

/*! Parameter validity check for emi filter width  */
#define IS_VALID_EMI_WIDTH(x)           \
(   (EmiNoFilter      == (x)) ||        \
    (EmiWidth4Cycle   == (x)) ||        \
    (EmiWidth8Cycle   == (x)) ||        \
    (EmiWidth16Cycle  == (x)) ||        \
    (EmiWidth32Cycle  == (x)) ||        \
    (EmiWidth64Cycle  == (x)) ||        \
    (EmiWidth128Cycle == (x)) ||        \
    (EmiWidth256Cycle == (x)))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/


/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Timer4EmiEnableNvic(void);
static void Timer4EmiDisableNvic(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static stc_emi_irq_cb_t m_stcEmiIrqCb = {NULL, NULL};

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
********************************************************************************
** \brief Device dependent initialization of interrupts according CMSIS with
**        level defined in interrupt_xxx.h
**
******************************************************************************/
static void Timer4EmiEnableNvic(void)
{
    NVIC_ClearPendingIRQ(TIM4PWM_EMI_IRQn);
    NVIC_EnableIRQ(TIM4PWM_EMI_IRQn);
    NVIC_SetPriority(TIM4PWM_EMI_IRQn, DDL_IRQ_LEVEL_TIM4PWMEMI);
}

/**
 *******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS
 **
 ******************************************************************************/
static void Timer4EmiDisableNvic(void)
{
    NVIC_ClearPendingIRQ(TIM4PWM_EMI_IRQn);
    NVIC_DisableIRQ(TIM4PWM_EMI_IRQn);
    NVIC_SetPriority(TIM4PWM_EMI_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 *******************************************************************************
 ** \brief  Emi interrupt handler function
 **
 ** \param [in] u8Dummy           Dummy parameter
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Emi_IrqHandler(uint8_t u8Dummy)
{
    if(1u == bM0P_TIMER4_ESCR_DIF)
    {
        if(NULL != m_stcEmiIrqCb.pfnDigitalPathIrqCb)
        {
            m_stcEmiIrqCb.pfnDigitalPathIrqCb();
        }
    }

    if(1u == bM0P_TIMER4_ESCR_AIF)
    {
        if(NULL != m_stcEmiIrqCb.pfnAnalogPathIrqCb)
        {
            m_stcEmiIrqCb.pfnAnalogPathIrqCb();
        }
    }
}

/**
 *******************************************************************************
 ** \brief  Initialize EMI
 **
 ** \param [in] pstcEmiConfig     Configure of EMI
 **
 ** \retval Ok                    EMI is configured
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcEmiConfig == NULL
 **                               - filter width is invalid
 **
 ******************************************************************************/
en_result_t Timer4_Emi_Init(stc_timer4_emi_config_t* pstcEmiConfig)
{
    /* Check parameters */
    ASSERT(IS_VALID_POINTER(pstcEmiConfig));
    ASSERT(IS_VALID_EN(pstcEmiConfig->bDigitalPathEnable));
    ASSERT(IS_VALID_EMI_WIDTH(pstcEmiConfig->enDigitalFilterWidth));
    ASSERT(IS_VALID_EN(pstcEmiConfig->bAnalogPathEnable));
    ASSERT(IS_VALID_EN(pstcEmiConfig->bHoldPwm));
    ASSERT(IS_VALID_EN(pstcEmiConfig->bSwitchToGpio));
    ASSERT(IS_VALID_EN(pstcEmiConfig->bDigitalPathIrqMskEnable));
    ASSERT(IS_VALID_EN(pstcEmiConfig->bAnalogPathIrqMskEnable));
    ASSERT(IS_VALID_EN(pstcEmiConfig->bTouchNvic));

    if(NULL == pstcEmiConfig)
    {
        return ErrorInvalidParameter;
    }

    M0P_TIMER4->ESCR = 0u;

    /* Set digital path for EMIIx pin */
    M0P_TIMER4->ESCR_f.DPE = ((TRUE == pstcEmiConfig->bDigitalPathEnable) ? 1u : 0u);

    /* Set analog path for EMIIx pin */
    M0P_TIMER4->ESCR_f.APE = ((TRUE == pstcEmiConfig->bAnalogPathEnable) ? 1u : 0u);

    /* Hold PWM pin when EMI interrupt occurs or not */
    M0P_TIMER4->ESCR_f.HOLD = ((TRUE == pstcEmiConfig->bHoldPwm) ? 1u : 0u);

    /* Set filter width of digital path  */
    switch (pstcEmiConfig->enDigitalFilterWidth)
    {
        case EmiNoFilter:
            M0P_TIMER4->ESCR_f.NCS = 0u;
            break;
        case EmiWidth4Cycle:
            M0P_TIMER4->ESCR_f.NCS = 1u;
            break;
        case EmiWidth8Cycle:
            M0P_TIMER4->ESCR_f.NCS = 2u;
            break;
        case EmiWidth16Cycle:
            M0P_TIMER4->ESCR_f.NCS = 3u;
            break;
        case EmiWidth32Cycle:
            M0P_TIMER4->ESCR_f.NCS = 4u;
            break;
        case EmiWidth64Cycle:
            M0P_TIMER4->ESCR_f.NCS = 5u;
            break;
        case EmiWidth128Cycle:
            M0P_TIMER4->ESCR_f.NCS = 6u;
            break;
        case EmiWidth256Cycle:
            M0P_TIMER4->ESCR_f.NCS = 7u;
            break;
        default:
            return ErrorInvalidParameter ;
    }

    bM0P_GPIO_FN_SEL01_EMII0C = pstcEmiConfig->bSwitchToGpio;

    M0P_TIMER4->ESCR_f.DID = ((TRUE == pstcEmiConfig->bDigitalPathIrqMskEnable) ? 1u : 0u);
    M0P_TIMER4->ESCR_f.AID = ((TRUE == pstcEmiConfig->bAnalogPathIrqMskEnable) ? 1u : 0u);

    m_stcEmiIrqCb.pfnDigitalPathIrqCb = pstcEmiConfig->stcIrqCb.pfnDigitalPathIrqCb;
    m_stcEmiIrqCb.pfnAnalogPathIrqCb = pstcEmiConfig->stcIrqCb.pfnAnalogPathIrqCb;

    if(TRUE == pstcEmiConfig->bTouchNvic)
    {
        Timer4EmiEnableNvic();
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  De-Initialize EMI
 **
 ** \param [in] bTouchNvic        Disable NVIC or not
 ** \arg TRUE                     Disable NVIC
 ** \arg FALSE                    Don't disable NVIC
 **
 ** \retval Ok                    EMI is cleared
 **
 ******************************************************************************/
en_result_t Timer4_Emi_DeInit(boolean_t bTouchNvic)
{
    ASSERT(IS_VALID_EN(bTouchNvic));

    M0P_TIMER4->ESCR = 0u;

    m_stcEmiIrqCb.pfnDigitalPathIrqCb = NULL;
    m_stcEmiIrqCb.pfnAnalogPathIrqCb = NULL;

    if(TRUE == bTouchNvic)
    {
        Timer4EmiDisableNvic();
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Set software trigger EMI
 **
 ** \param [in]                   None
 **
 ** \retval Ok                    EMI is triggered by software
 **
 ******************************************************************************/
en_result_t Timer4_Emi_SwTiggerEmi(void)
{
    M0P_TIMER4->ESCR_f.SDI = 1u;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Get Digital path interrupt flag
 **
 ** \param [in]                  None
 **
 ** \retval DdlClr               Digital path interrupt flag is clear
 ** \retval DdlSet               Digital path interrupt flag is set
 **
 ******************************************************************************/
en_irq_flag_t Timer4_Emi_GetDigitalPathIrqFlag(void)
{
    en_irq_flag_t enFlag = DdlClr;

    /* get interrupt flag via digital noise-canceler */
    enFlag = (1u == M0P_TIMER4->ESCR_f.DIF) ? DdlSet : DdlClr;

    return enFlag;
}

/**
 *******************************************************************************
 ** \brief  Clear Digital path interrupt flag
 **
 ** \param [in]                   None
 **
 ** \retval Ok                    Digital path interrupt flag is clear
 **
 ******************************************************************************/
en_result_t Timer4_Emi_ClrDigitalPathIrqFlag(void)
{
    M0P_TIMER4->ESCR_f.DIFC = 1u;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Get Analog path interrupt flag
 **
 ** \param [in]                   None
 **
 ** \retval DdlClr                Analog path interrupt flag is clear
 ** \retval DdlSet                Analog path interrupt flag is set
 **
 ******************************************************************************/
en_irq_flag_t Timer4_Emi_GetAnalogPathIrqFlag(void)
{
    en_irq_flag_t enFlag;

    /* get interrupt flag via analog noise filter */
    enFlag = (1u == M0P_TIMER4->ESCR_f.AIF) ? DdlSet : DdlClr;

    return enFlag;
}

/**
 *******************************************************************************
 ** \brief  Clear Analog path interrupt flag
 **
 ** \param [in]                   None
 **
 ** \retval Ok                    Analog path interrupt flag is clear
 **
 ******************************************************************************/
en_result_t Timer4_Emi_ClrAnalogPathIrqFlag(void)
{
    M0P_TIMER4->ESCR_f.AIFC = 1u;

    return Ok;
}

//@} // Timer4EmiGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
