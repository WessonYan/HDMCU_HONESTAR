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
/** \file timer4_cnt.c
 **
 ** A detailed description is available at
 ** @link Timer4CntGroup Timer4Cnt description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     Timer4Cnt
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "timer4_cnt.h"


/**
 *******************************************************************************
 ** \addtogroup Timer4CntGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/


/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_POINTER(x)  (NULL != (x))  ///< Parameter validity check for pointer

/*! Parameter validity check for CNT pclk division  */
#define IS_VALID_CNT_CLK_DIV(x)         \
(   (CntPclkDiv1    == (x)) ||          \
    (CntPclkDiv2    == (x)) ||          \
    (CntPclkDiv4    == (x)) ||          \
    (CntPclkDiv8    == (x)) ||          \
    (CntPclkDiv16   == (x)) ||          \
    (CntPclkDiv32   == (x)) ||          \
    (CntPclkDiv64   == (x)) ||          \
    (CntPclkDiv128  == (x)) ||          \
    (CntPclkDiv256  == (x)) ||          \
    (CntPclkDiv512  == (x)) ||          \
    (CntPclkDiv1024 == (x)))

/*! Parameter validity check for CNT mode  */
#define IS_VALID_CNT_MODE(x)            \
(   (UpCnt == (x)) ||                   \
    (UpDownCnt == (x)))

/*! Parameter validity check for CNT interrupt mask  */
#define IS_VALID_CNT_INT_MSK(x)    ((x) < 16u)

/*! Parameter validity check for CNT start mode  */
#define IS_VALID_CNT_START_MODE(x)      \
(   (CntStartFromCurValue == (x))  ||   \
    (CntStartFromZeroValue == (x)))

/*! Parameter validity check for CNT stop mode  */
#define IS_VALID_CNT_STOP_MODE(x)       \
(   (CntStopKeepCISR == (x)) ||         \
    (CntStopClearCISR == (x)))

/*! Parameter validity check for CNT match interrupt type  */
#define IS_VALID_CNT_IRQ_TYPE(x)        \
(   (CntZeroMatchIrq == (x)) ||         \
    (CntPeakMatchIrq == (x)))

/*! Parameter validity check for CNT clock source  */
#define IS_VALID_CNT_CLK(x)             \
(   (CntClockPclk == (x)) ||            \
    (CntClockExck == (x)))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/


/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Timer4CntEnableNvic(void);
static void Timer4CntDisableNvic(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static stc_cnt_irq_cb_t m_stcCntIrqCb = {NULL, NULL};

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
********************************************************************************
** \brief Device dependent initialization of interrupts according CMSIS with
**        level defined in interrupt_xxx.h
**
******************************************************************************/
static void Timer4CntEnableNvic(void)
{
    NVIC_ClearPendingIRQ(TIM4CNT_IRQn);
    NVIC_EnableIRQ(TIM4CNT_IRQn);
    NVIC_SetPriority(TIM4CNT_IRQn, DDL_IRQ_LEVEL_CNT);
}

/**
 *******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS
 **
 ******************************************************************************/
static void Timer4CntDisableNvic(void)
{
    NVIC_ClearPendingIRQ(TIM4CNT_IRQn);
    NVIC_DisableIRQ(TIM4CNT_IRQn);
    NVIC_SetPriority(TIM4CNT_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 *******************************************************************************
 ** \brief  CNT peak match interrupt handler function
 **
 ** \param [in] u8Dummy           Dummy parameter
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Cnt_PeakIrqHandler(uint8_t u8Dummy)
{
    stc_timer4_ccsr_field_t stcCCSR_f;

    stcCCSR_f = M0P_TIMER4->CCSR_f;

    if(1u == stcCCSR_f.ICLR)   /* Peak match Interrupt */
    {
        bM0P_TIMER4_CCSR_ICLR = 0u; /* Clear peak match Interrupt */
        if(NULL != m_stcCntIrqCb.pfnCntPeakIrqCb)
        {
            m_stcCntIrqCb.pfnCntPeakIrqCb();
        }
    }
}

/**
 *******************************************************************************
 ** \brief  CNT zero match interrupt handler function
 **
 ** \param [in] u8Dummy           Dummy parameter
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Cnt_ZeroIrqHandler(uint8_t u8Dummy)
{
    stc_timer4_ccsr_field_t stcCCSR_f;

    stcCCSR_f = M0P_TIMER4->CCSR_f;

    if(1u == stcCCSR_f.IRQZF) /* Zero match Interrupt */
    {
        bM0P_TIMER4_CCSR_IRQZF = 0u; /* Clear zero match Interrupt */
        if(NULL != m_stcCntIrqCb.pfnCntZeroIrqCb)
        {
            m_stcCntIrqCb.pfnCntZeroIrqCb();
        }
    }
}

/**
 *******************************************************************************
 ** \brief  Init Cnt
 **
 ** \param [in] pstcCntConfig Pointer to CNT configure structure
 **
 ** \retval Ok                    Internal data has been setup
 **
 ******************************************************************************/
en_result_t Timer4_Cnt_Init(stc_timer4_cnt_config_t *pstcCntConfig)
{
    /* Check parameters */
    ASSERT(IS_VALID_POINTER(pstcCntConfig));
    ASSERT(IS_VALID_CNT_CLK(pstcCntConfig->enCntClock));
    ASSERT(IS_VALID_CNT_CLK_DIV(pstcCntConfig->enCntClockDiv));
    ASSERT(IS_VALID_CNT_MODE(pstcCntConfig->enCntMode));
    ASSERT(IS_VALID_CNT_INT_MSK(pstcCntConfig->u8ZeroIntMsk));
    ASSERT(IS_VALID_CNT_INT_MSK(pstcCntConfig->u8PeakIntMsk));

    /* Set default value */
    M0P_TIMER4->CCSR = 0x0040;
    M0P_TIMER4->CIMR = 0x0000;
    M0P_TIMER4->CPSR = 0xFFFF;
    M0P_TIMER4->CISR = 0x0000;

    /* set count clock div of CNT */
    M0P_TIMER4->CCSR_f.DCLK = (uint8_t)pstcCntConfig->enCntClockDiv;

    /* set cnt mode */
    M0P_TIMER4->CCSR_f.CNTMD = (uint8_t)pstcCntConfig->enCntMode;

    /* set buffer enable bit */
    M0P_TIMER4->CCSR_f.BUFEN = (TRUE == pstcCntConfig->bEnBuffer) ? 1u : 0u;

    /* set external clock enable bit */
    M0P_TIMER4->CCSR_f.ECKEN = (CntClockExck == pstcCntConfig->enCntClock) ? 1u : 0u;

    /* set intterrupt mask times */
    M0P_TIMER4->CIMR_f.ZIM = (uint8_t)pstcCntConfig->u8ZeroIntMsk;
    M0P_TIMER4->CIMR_f.PIM = (uint8_t)pstcCntConfig->u8PeakIntMsk;

    /* Set interrupt enable */
    if (NULL != pstcCntConfig->pstcIrqEn)
    {
        M0P_TIMER4->CCSR_f.IRQZEN = (TRUE == pstcCntConfig->pstcIrqEn->bCntZeroMatchIrq)?1u:0u;
        M0P_TIMER4->CCSR_f.ICLEN  = (TRUE == pstcCntConfig->pstcIrqEn->bCntPeakMatchIrq)?1u:0u;
    }

    /* Set interrupt callback function */
    if (NULL != pstcCntConfig->pstcIrqCb)
    {
        m_stcCntIrqCb.pfnCntZeroIrqCb = pstcCntConfig->pstcIrqCb->pfnCntZeroIrqCb;
        m_stcCntIrqCb.pfnCntPeakIrqCb = pstcCntConfig->pstcIrqCb->pfnCntPeakIrqCb;
    }
    else
    {
        m_stcCntIrqCb.pfnCntZeroIrqCb = NULL;
        m_stcCntIrqCb.pfnCntPeakIrqCb = NULL;
    }

    if(TRUE == pstcCntConfig->bTouchNvic)
    {
        Timer4CntEnableNvic();
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  DeInit CNT
 **
 ** \param [in] bTouchNvic        Touch NVIC or not
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Cnt_DeInit(boolean_t bTouchNvic)
{
    if(TRUE == bTouchNvic)
    {
        Timer4CntDisableNvic();
    }

    /* Set default value */
    M0P_TIMER4->CCSR = 0x0040;
    M0P_TIMER4->CIMR = 0x0000;
    M0P_TIMER4->CPSR = 0xFFFF;
    M0P_TIMER4->CISR = 0x0000;

    m_stcCntIrqCb.pfnCntZeroIrqCb = NULL;
    m_stcCntIrqCb.pfnCntPeakIrqCb = NULL;
}

/**
 *******************************************************************************
 ** \brief  Set CNT clock source
 **
 ** \param [in] enCntClock        CNT clock source
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Cnt_SetClock(en_cnt_clock_t enCntClock)
{
    /* Check parameters */
    ASSERT(IS_VALID_CNT_CLK(enCntClock));

    M0P_TIMER4->CCSR_f.ECKEN = (uint8_t)enCntClock;
}

/**
 *******************************************************************************
 ** \brief  Set CNT clock division
 **
 ** \param [in] enClockDiv        CNT clock division
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Cnt_SetClockDiv(en_cnt_clock_div_t enClockDiv)
{
    /* Check parameters */
    ASSERT(IS_VALID_CNT_CLK_DIV(enClockDiv));

    M0P_TIMER4->CCSR_f.DCLK = (uint8_t)enClockDiv;
}

/**
 *******************************************************************************
 ** \brief  Set CNT mode
 **
 ** \param [in] enMode            CNT mode
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Cnt_SetMode(en_cnt_mode_t enMode)
{
    /* Check parameters */
    ASSERT(IS_VALID_CNT_MODE(enMode));

    M0P_TIMER4->CCSR_f.DCLK = (uint8_t)enMode;
}

/**
 *******************************************************************************
 ** \brief  Start CNT
 **
 ** \param [in] enStartMode       Start CNT mode
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Cnt_Start(en_cnt_start_mode_t enStartMode)
{
    /* Check parameters */
    ASSERT(IS_VALID_CNT_START_MODE(enStartMode));

    if(CntStartFromZeroValue == enStartMode)
    {
        M0P_TIMER4->CCSR_f.STOP = 0u;
        M0P_TIMER4->CCSR_f.CLEAR = 1u;
    }
    else if(CntStartFromCurValue == enStartMode)
    {
        M0P_TIMER4->CCSR_f.STOP = 0u;
        M0P_TIMER4->CCSR_f.CLEAR = 0u;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }
}

/**
 *******************************************************************************
 ** \brief  Stop CNT
 **
 ** \param [in] enStopMode        Stop CNT mode
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Cnt_Stop(en_cnt_stop_mode_t enStopMode)
{
    /* Check parameters */
    ASSERT(IS_VALID_CNT_STOP_MODE(enStopMode));

    if(CntStopClearCISR == enStopMode)
    {
        M0P_TIMER4->CCSR_f.STOP = 1u;
        M0P_TIMER4->CCSR_f.CLEAR = 1u;
    }
    else if(CntStopKeepCISR == enStopMode)
    {
        M0P_TIMER4->CCSR_f.STOP = 1u;
        M0P_TIMER4->CCSR_f.CLEAR = 0u;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }
}

/**
 *******************************************************************************
 ** \brief  enable CNT interrupt
 **
 ** \param [in] enIrqType         the type of interrupt
 ** \arg enCntZeroMatchIrq        Zero match interrupt of CNT
 ** \arg enCntPeakMatchIrq        Peak match interrupt of CNT
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Cnt_EnableIrq(en_cnt_irq_t enIrqType)
{
    /* Check parameters */
    ASSERT(IS_VALID_CNT_IRQ_TYPE(enIrqType));

    if(CntZeroMatchIrq == enIrqType) /* zero detect interrupt */
    {
        M0P_TIMER4->CCSR_f.IRQZEN = 1u;
    }
    else if(CntPeakMatchIrq == enIrqType) /* peak detect interrupt */
    {
        M0P_TIMER4->CCSR_f.ICLEN = 1u;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }
}

/**
 *******************************************************************************
 ** \brief enable CNT interrupt
 **
 ** \param [in] enIrqType         the type of interrupt
 ** \arg enCntZeroMatchIrq        Zero match interrupt of CNT
 ** \arg enCntPeakMatchIrq        Peak match interrupt of CNT
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Cnt_DisableIrq(       en_cnt_irq_t enIrqType)
{
    /* Check parameters */
    ASSERT(IS_VALID_CNT_IRQ_TYPE(enIrqType));

    if(CntZeroMatchIrq == enIrqType) /* zero detect interrupt */
    {
        M0P_TIMER4->CCSR_f.IRQZEN = 0u;
    }
    else if(CntPeakMatchIrq == enIrqType) /* peak detect interrupt */
    {
        M0P_TIMER4->CCSR_f.ICLEN = 0u;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }
}

/**
 *******************************************************************************
 ** \brief  Get CNT interrupt flag
 **
 ** \param [in] enIrqType         Interrupt type
 ** \arg enCntZeroMatchIrq        Zero match interrupt of CNT
 ** \arg enCntPeakMatchIrq        Peak match interrupt of CNT
 **
 ** \retval DdlClr                None interrupt request on CNT
 ** \retval DdlSet                Detection interrupt request on CNT
 **
 ******************************************************************************/
en_irq_flag_t Timer4_Cnt_GetIrqFlag(       en_cnt_irq_t enIrqType)
{
    en_irq_flag_t enIrqFlag;

    /* Check parameters */
    ASSERT(IS_VALID_CNT_IRQ_TYPE(enIrqType));

    if(CntZeroMatchIrq == enIrqType) /* zero detect interrupt */
    {
        enIrqFlag = (1u == M0P_TIMER4->CCSR_f.IRQZF) ? DdlSet : DdlClr;
    }
    else if(CntPeakMatchIrq == enIrqType) /* peak detect interrupt */
    {
        enIrqFlag = (1u == M0P_TIMER4->CCSR_f.ICLR) ? DdlSet : DdlClr;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }
    return enIrqFlag;
}

/**
 *******************************************************************************
 ** \brief  Clear CNT interrupt falg
 **
 ** \param [in] enIrqType         the type of interrupt
 ** \arg enCntZeroMatchIrq        Zero match interrupt of CNT
 ** \arg enCntPeakMatchIrq        Peak match interrupt of CNT
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Cnt_ClrIrqFlag(      en_cnt_irq_t enIrqType)
{
    /* Check parameters */
    ASSERT(IS_VALID_CNT_IRQ_TYPE(enIrqType));

    if(CntZeroMatchIrq == enIrqType) /*zero detect interrupt */
    {
        M0P_TIMER4->CCSR_f.IRQZF = 0u;
    }
    else if(CntPeakMatchIrq == enIrqType) /* peak detect interrupt */
    {
        M0P_TIMER4->CCSR_f.ICLR = 0u;
    }
    else
    {
        /* do-nothing : only avoid compiler error for MISRA-C:2004 rule */
    }
}

/**
 *******************************************************************************
 ** \brief  Set CNT cycle value
 **
 ** \param [in] u16Cycle          the cycle value
 ** \arg number of 16bit
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Cnt_SetCycleVal(uint16_t u16Cycle)
{
    M0P_TIMER4->CPSR = u16Cycle;
}

/**
 *******************************************************************************
 ** \brief  Get CNT cycle value
 **
 ** \param [in]                   None
 **
 ** \retval the cycle value
 **
 ******************************************************************************/
uint16_t Timer4_Cnt_GetCycleVal(void)
{
    return M0P_TIMER4->CPSR;
}

/**
 *******************************************************************************
 ** \brief  Set CNT count value
 **
 ** \param [in] u16Count          the count value
 ** \arg number of 16bit
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Cnt_SetCountVal(uint16_t u16Count)
{
    M0P_TIMER4->CISR = u16Count;
}

/**
 *******************************************************************************
 ** \brief Get CNT current count
 **
 ** \param [in]                   None
 **
 ** \retval current count of CNT
 **
 ******************************************************************************/
uint16_t Timer4_Cnt_GetCountVal(void)
{
    return M0P_TIMER4->CISR;
}

/**
 *******************************************************************************
 ** \brief Set mask zero times
 **
 ** \param [in]                   u8MaskTimes mask times
 ** \arg alue of 0~15
 **
 ** \retval None
 **
 ******************************************************************************/
void Timer4_Cnt_SetMaskZeroTimes(uint8_t u8MaskTimes)
{
    /* Check parameters */
    ASSERT(IS_VALID_CNT_INT_MSK(u8MaskTimes));

    M0P_TIMER4->CIMR_f.ZIM = u8MaskTimes;
}

/**
 *******************************************************************************
 ** \brief  Get current mask zero  times
 **
 ** \param [in]                   None
 **
 ** \retval Current mask zero times
 **
 ******************************************************************************/
uint8_t Timer4_Cnt_GetCurMaskZeroTimes(void)
{
    return M0P_TIMER4->CIMR_f.ZIM;
}

/**
 *******************************************************************************
 ** \brief Set mask peak  times
 **
 ** \param [in] u8MaskTimes mask times
 ** \arg value of 0~15
 **
 ** \retval None
 **
 ******************************************************************************/
void Timer4_Cnt_SetMaskPeakTimes(uint8_t u8MaskTimes)
{
    /* Check parameters */
    ASSERT(IS_VALID_CNT_INT_MSK(u8MaskTimes));

    M0P_TIMER4->CIMR_f.PIM = u8MaskTimes;
}

/**
 *******************************************************************************
 ** \brief Get mask peak  times
 **
 ** \param [in]                   None
 **
 ** \retval Current mask peak times
 **
 ******************************************************************************/
uint8_t Timer4_Cnt_GetCurMaskPeakTimes(void)
{
    return M0P_TIMER4->CIMR_f.PIM;
}

//@} // Timer4CntGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
