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
/** \file timer4_pwm.c
 **
 ** A detailed description is available at
 ** @link Timer4PwmGroup Timer4Pwm description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     Timer4Pwm
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "timer4_pwm.h"

/**
 *******************************************************************************
 ** \addtogroup Timer4AdctGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/


/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_POINTER(x)  (NULL != (x))  ///< Parameter validity check for pointer

/*! Parameter validity check for pwm couple channel  */
#define IS_VALID_COUPLE_CH(x)           \
(   (TIMER4_PWM_COUPLE_CH10 == (x)) ||  \
    (TIMER4_PWM_COUPLE_CH32 == (x)) ||  \
    (TIMER4_PWM_COUPLE_CH54 == (x)))

/*! Parameter validity check for pwm channel  */
#define IS_VALID_CH(x)                  \
(   (TIMER4_PWM_CH0 == (x)) ||          \
    (TIMER4_PWM_CH1 == (x)) ||          \
    (TIMER4_PWM_CH2 == (x)) ||          \
    (TIMER4_PWM_CH3 == (x)) ||          \
    (TIMER4_PWM_CH4 == (x)) ||          \
    (TIMER4_PWM_CH5 == (x)))

/*! Parameter validity check for pwm clock division  */
#define IS_VALID_PWM_CLK(x)             \
(   (PwmPlckDiv1 == (x))  ||            \
    (PwmPlckDiv2 == (x))  ||            \
    (PwmPlckDiv4 == (x))  ||            \
    (PwmPlckDiv8 == (x))  ||            \
    (PwmPlckDiv16 == (x)) ||            \
    (PwmPlckDiv32 == (x)) ||            \
    (PwmPlckDiv64 == (x)) ||            \
    (PwmPlckDiv128 == (x)))

/*! Parameter validity check for pwm output level bits  */
#define IS_VALID_LVLS_BITS(x)           \
(  (LvlsBits00B == (x)) ||              \
   (LvlsBits01B == (x)) ||              \
   (LvlsBits10B == (x)) ||              \
   (LvlsBits11B == (x)))

/*! Parameter validity check for pwm mode  */
#define IS_VALID_PWM_MODE(x)            \
(   (PwmThroughMode == (x))   ||        \
    (PwmDeadTimerMode == (x)) ||        \
    (PwmDeadTimerFilterMode == (x)))

/*! Parameter validity check for enable flag  */
#define IS_VALID_EN(x)                  \
(   (TRUE == (x)) ||                    \
    (FALSE == (x)))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/


/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Timer4PwmEnableNvic(void);
static void Timer4PwmDisableNvic(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static stc_timer4_pwm_timer_cb_t m_stcPwmTimerCb = {NULL, NULL, NULL};

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
********************************************************************************
** \brief Device dependent initialization of interrupts according CMSIS with
**        level defined in interrupt_xxx.h
**
******************************************************************************/
static void Timer4PwmEnableNvic(void)
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
static void Timer4PwmDisableNvic(void)
{
    NVIC_ClearPendingIRQ(TIM4PWM_EMI_IRQn);
    NVIC_DisableIRQ(TIM4PWM_EMI_IRQn);
    NVIC_SetPriority(TIM4PWM_EMI_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 *******************************************************************************
 ** \brief  PWM interrupt handler function
 **
 ** \param [in] u8CoupleCh        Couple channel
 **
 ** \retval                       None
 **
 ******************************************************************************/
void Timer4_Pwm_IrqHandler(uint8_t u8CoupleCh)
{
    ASSERT(IS_VALID_COUPLE_CH(u8CoupleCh));

    switch(u8CoupleCh)
    {
        case TIMER4_PWM_COUPLE_CH10:
            if(1u == bM0P_TIMER4_RCSR_RTIF10)
            {
                bM0P_TIMER4_RCSR_RTIC10 = 1u;
                if(m_stcPwmTimerCb.pfnPwm10TimerIrqCallback)
                {
                    m_stcPwmTimerCb.pfnPwm10TimerIrqCallback();
                }
            }
            break;
        case TIMER4_PWM_COUPLE_CH32:
            if(1u == bM0P_TIMER4_RCSR_RTIF32)
            {
                bM0P_TIMER4_RCSR_RTIC32 = 1u;
                if(m_stcPwmTimerCb.pfnPwm32TimerIrqCallback)
                {
                    m_stcPwmTimerCb.pfnPwm32TimerIrqCallback();
                }
            }
            break;
        case TIMER4_PWM_COUPLE_CH54:
            if(1u == bM0P_TIMER4_RCSR_RTIF54)
            {
                bM0P_TIMER4_RCSR_RTIC54 = 1u;
                if(m_stcPwmTimerCb.pfnPwm54TimerIrqCallback)
                {
                    m_stcPwmTimerCb.pfnPwm54TimerIrqCallback();
                }
            }
            break;
        default:
            break;
    }
}

/**
 *******************************************************************************
 ** \brief  Initialize a couple PWM channels
 **
 ** \param [in] u8CoupleCh        channel of PWM couple
 ** \arg  TIMER4_PWM_CH10 ~ TIMER4_PWM_CH54
 ** \param [in] pstcConfig        Pointer to PWM configuration
 ** \arg  see the struct #stc_timer4_pwm_config_t
 **
 ** \retval Ok                    PWM mode is configured
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8CoupleCh out of range
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Timer4_Pwm_Init(uint8_t u8CoupleCh,
                         stc_timer4_pwm_config_t* pstcConfig)
{
    volatile stc_timer4_pocr10_field_t* pstcPOCR_f;
    volatile stc_timer4_rcsr_field_t* pstcRCSR_f;

    /* Check parameters */
    ASSERT(IS_VALID_POINTER(pstcConfig));
    ASSERT(IS_VALID_COUPLE_CH(u8CoupleCh));
    ASSERT(IS_VALID_PWM_MODE(pstcConfig->enMode));
    ASSERT(IS_VALID_LVLS_BITS(pstcConfig->enLvlsBits));
    ASSERT(IS_VALID_PWM_CLK(pstcConfig->enClk));
    ASSERT(IS_VALID_EN(pstcConfig->bPwmTimerIrqMask));
    ASSERT(IS_VALID_EN(pstcConfig->bTouchNvic));

    if((u8CoupleCh > TIMER4_PWM_MAX_COUPLE_CH) ||
        (NULL == pstcConfig))
    {
        return ErrorInvalidParameter;
    }

    /* Get pointer of current channel PWM register address */
    pstcPOCR_f = (volatile stc_timer4_pocr10_field_t*)((volatile uint8_t*)&M0P_TIMER4->POCR10 + u8CoupleCh*4u);
    pstcRCSR_f = (volatile stc_timer4_rcsr_field_t*)((volatile uint8_t*)&M0P_TIMER4->RCSR);

    /* Configure PWM mode */
    switch(pstcConfig->enMode)
    {
        case PwmThroughMode:
            pstcPOCR_f->WMD = 0u;
            break;
        case PwmDeadTimerMode:
            pstcPOCR_f->WMD = 1u;
            break;
        case PwmDeadTimerFilterMode:
            pstcPOCR_f->WMD = 2u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    /* Configure PWM mode */
    switch(pstcConfig->enLvlsBits)
    {
        case LvlsBits00B:
            pstcPOCR_f->LVLS = 0u;
            break;
        case LvlsBits01B:
            pstcPOCR_f->LVLS = 1u;
            break;
        case LvlsBits10B:
            pstcPOCR_f->LVLS = 2u;
            break;
        case LvlsBits11B:
            pstcPOCR_f->LVLS = 3u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    /* Set timer clock division */
    switch (pstcConfig->enClk)
    {
        case PwmPlckDiv1:
            pstcPOCR_f->DIVCK = 0u;
            break;
        case PwmPlckDiv2:
            pstcPOCR_f->DIVCK = 1u;
            break;
        case PwmPlckDiv4:
            pstcPOCR_f->DIVCK = 2u;
            break;
        case PwmPlckDiv8:
            pstcPOCR_f->DIVCK = 3u;
            break;
        case PwmPlckDiv16:
            pstcPOCR_f->DIVCK = 4u;
            break;
        case PwmPlckDiv32:
            pstcPOCR_f->DIVCK = 5u;
            break;
        case PwmPlckDiv64:
            pstcPOCR_f->DIVCK = 6u;
            break;
        case PwmPlckDiv128:
            pstcPOCR_f->DIVCK = 7u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    /* Set interrupt mask */
    switch (u8CoupleCh)
    {
        case TIMER4_PWM_COUPLE_CH10:
            pstcRCSR_f->RTID10 = (TRUE == pstcConfig->bPwmTimerIrqMask) ? 1u:0u;
            break;
        case TIMER4_PWM_COUPLE_CH32:
            pstcRCSR_f->RTID32 = (TRUE == pstcConfig->bPwmTimerIrqMask) ? 1u:0u;
            break;
        case TIMER4_PWM_COUPLE_CH54:
            pstcRCSR_f->RTID54 = (TRUE == pstcConfig->bPwmTimerIrqMask) ? 1u:0u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    /* Set interrupt callback functions */
    switch (u8CoupleCh)
    {
        case TIMER4_PWM_COUPLE_CH10:
            m_stcPwmTimerCb.pfnPwm10TimerIrqCallback = pstcConfig->pfnPwmTimerIrqCb;
            break;
        case TIMER4_PWM_COUPLE_CH32:
            m_stcPwmTimerCb.pfnPwm32TimerIrqCallback = pstcConfig->pfnPwmTimerIrqCb;
            break;
        case TIMER4_PWM_COUPLE_CH54:
            m_stcPwmTimerCb.pfnPwm54TimerIrqCallback = pstcConfig->pfnPwmTimerIrqCb;
            break;
        default:
            return ErrorInvalidParameter;
    }

    if(TRUE == pstcConfig->bTouchNvic)
    {
        Timer4PwmEnableNvic();
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  De-Initialize a couple PWM channels
 **
 ** \param [in] pstcWfg           Pointer to PWM instance
 ** \param [in] u8CoupleCh        Couple channel of PWM couple
 ** \arg  TIMER4_PWM_CH10 ~ TIMER4_PWM_CH54
 ** \param [in] bTouchNvic        Touch NVIC or not
 **
 ** \retval Ok                    PWM mode is configured
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8CoupleCh out of range
 **
 ******************************************************************************/
en_result_t Timer4_Pwm_DeInit(uint8_t u8CoupleCh, boolean_t bTouchNvic)
{
    volatile uint16_t *pu16PDRA;
    volatile uint16_t *pu16PDRB;
    volatile uint16_t *pu16PFSR;
    volatile stc_timer4_pocr10_field_t* pstcPOCR_f;
    volatile stc_timer4_rcsr_field_t* pstcRCSR_f;

    /* Check parameters */
    ASSERT(IS_VALID_COUPLE_CH(u8CoupleCh));
    ASSERT(IS_VALID_EN(bTouchNvic));

    if(u8CoupleCh > TIMER4_PWM_MAX_COUPLE_CH)
    {
        return ErrorInvalidParameter;
    }

    /* Get pointer of current channel PWM register address */
    pu16PDRA = (volatile uint16_t*)((volatile uint8_t*)&M0P_TIMER4->PDRA10 + u8CoupleCh*8u);
    pu16PDRB = (volatile uint16_t*)((volatile uint8_t*)&M0P_TIMER4->PDRB10 + u8CoupleCh*8u);
    pu16PFSR = (volatile uint16_t*)((volatile uint8_t*)&M0P_TIMER4->PFSR10 + u8CoupleCh*8u);
    pstcPOCR_f = (volatile stc_timer4_pocr10_field_t*)((volatile uint8_t*)&M0P_TIMER4->POCR10 + u8CoupleCh*4u);
    pstcRCSR_f = (volatile stc_timer4_rcsr_field_t*)((volatile uint8_t*)&M0P_TIMER4->RCSR);

    *pu16PDRA = 0u;
    *pu16PDRB = 0u;
    *pu16PFSR = 0u;
    pstcPOCR_f->DIVCK = 0u;
    pstcPOCR_f->LVLS = 0u;
    pstcPOCR_f->WMD = 0u;

    switch (u8CoupleCh)
    {
        case TIMER4_PWM_COUPLE_CH10:

            m_stcPwmTimerCb.pfnPwm10TimerIrqCallback = NULL;
            pstcRCSR_f->RTID10 = 0u;
            break;
        case TIMER4_PWM_COUPLE_CH32:
            m_stcPwmTimerCb.pfnPwm32TimerIrqCallback = NULL;
            pstcRCSR_f->RTID32 = 0u;
            break;
        case TIMER4_PWM_COUPLE_CH54:
            m_stcPwmTimerCb.pfnPwm54TimerIrqCallback = NULL;
            pstcRCSR_f->RTID54 = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    if(TRUE == bTouchNvic)
    {
        Timer4PwmDisableNvic();
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Start PWM timer
 **
 ** \param [in]  u8CoupleCh       channel of PWM couple
 ** \arg TIMER4_PWM_CH10 ~ TIMER4_PWM_CH54
 **
 ** \retval Ok                    Start timer successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8CoupleCh out of range
 **
 ******************************************************************************/
en_result_t Timer4_Pwm_StartTimer(uint8_t u8CoupleCh)
{
    volatile stc_timer4_rcsr_field_t* pstcRCSR_f;

    /* Check parameters */
    ASSERT(IS_VALID_COUPLE_CH(u8CoupleCh));

    if(u8CoupleCh > TIMER4_PWM_MAX_COUPLE_CH)
    {
        return ErrorInvalidParameter;
    }

    pstcRCSR_f = (volatile stc_timer4_rcsr_field_t*)((volatile uint8_t*)&M0P_TIMER4->RCSR);

    switch(u8CoupleCh)
    {
        case TIMER4_PWM_COUPLE_CH10:
            pstcRCSR_f->RTE10 = 1u;
            break;
        case TIMER4_PWM_COUPLE_CH32:
            pstcRCSR_f->RTE32 = 1u;
            break;
        case TIMER4_PWM_COUPLE_CH54:
            pstcRCSR_f->RTE54 = 1u;
            break;
        default:
            break;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Stop PWM timer
 **
 ** \param [in]  u8CoupleCh       channel of PWM couple
 ** \arg TIMER4_PWM_CH10 ~ TIMER4_PWM_CH54
 **
 ** \retval Ok                    Stop timer successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8CoupleCh out of range
 **
 ******************************************************************************/
en_result_t Timer4_Pwm_StopTimer(uint8_t u8CoupleCh)
{
    volatile stc_timer4_rcsr_field_t* pstcRCSR_f;

    /* Check parameters */
    ASSERT(IS_VALID_COUPLE_CH(u8CoupleCh));

    if(u8CoupleCh > TIMER4_PWM_MAX_COUPLE_CH)
    {
        return ErrorInvalidParameter;
    }

    pstcRCSR_f = (volatile stc_timer4_rcsr_field_t*)((volatile uint8_t*)&M0P_TIMER4->RCSR);

    switch(u8CoupleCh)
    {
        case TIMER4_PWM_COUPLE_CH10:
            pstcRCSR_f->RTS10 = 1u;
            break;
        case TIMER4_PWM_COUPLE_CH32:
            pstcRCSR_f->RTS32 = 1u;
            break;
        case TIMER4_PWM_COUPLE_CH54:
            pstcRCSR_f->RTS54 = 1u;
            break;
        default:
            break;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Get PWM timer interrupt flag
 **
 ** \param [in]  u8CoupleCh       channel of PWM couple
 ** \arg  TIMER4_PWM_CH10 ~ TIMER4_PWM_CH54
 **
 ** \retval DdlSet                PWM Interrupt flag is set
 ** \retval DdlClr                PWM Interrupt flag is clear
 **
 ******************************************************************************/
en_irq_flag_t Timer4_Pwm_GetTimerIrqFlag(uint8_t u8CoupleCh)
{
    en_irq_flag_t enRetVal = DdlClr;
    volatile stc_timer4_rcsr_field_t* pstcRCSR_f;

    /* Check parameters */
    ASSERT(IS_VALID_COUPLE_CH(u8CoupleCh));

    pstcRCSR_f = (volatile stc_timer4_rcsr_field_t*)((volatile uint8_t*)&M0P_TIMER4->RCSR);

    switch(u8CoupleCh)
    {
        case TIMER4_PWM_COUPLE_CH10:
            enRetVal = (1u == pstcRCSR_f->RTIF10) ? DdlSet:DdlClr;
            break;
        case TIMER4_PWM_COUPLE_CH32:
            enRetVal = (1u == pstcRCSR_f->RTIF32) ? DdlSet:DdlClr;
            break;
        case TIMER4_PWM_COUPLE_CH54:
            enRetVal = (1u == pstcRCSR_f->RTIF32) ? DdlSet:DdlClr;
            break;
        default:
            break;
    }

    return enRetVal;
}

/**
 *******************************************************************************
 ** \brief  Clear PWM timer interrupt flag
 **
 ** \param [in] u8CoupleCh        channel of PWM couple
 ** \arg  TIMER4_PWM_CH10 ~ TIMER4_PWM_CH54
 **
 ** \retval Ok                    PWM timer interrupt flag is clear
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8CoupleCh out of range
 **
 ******************************************************************************/
en_result_t Timer4_Pwm_ClrTimerIrqFlag(uint8_t u8CoupleCh)
{
    volatile stc_timer4_rcsr_field_t* pstcRCSR_f;

    /* Check parameters */
    ASSERT(IS_VALID_COUPLE_CH(u8CoupleCh));

    if(u8CoupleCh > TIMER4_PWM_MAX_COUPLE_CH)
    {
        return ErrorInvalidParameter;
    }

    pstcRCSR_f = (volatile stc_timer4_rcsr_field_t*)((volatile uint8_t*)&M0P_TIMER4->RCSR);

    /* set register value */
    switch(u8CoupleCh)
    {
        case TIMER4_PWM_COUPLE_CH10:
            pstcRCSR_f->RTIC10 = 1u;
            break;
        case TIMER4_PWM_COUPLE_CH32:
            pstcRCSR_f->RTIC10 = 1u;
            break;
        case TIMER4_PWM_COUPLE_CH54:
            pstcRCSR_f->RTIC10 = 1u;
            break;
        default:
            break;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Write timer count cycle
 **
 ** \param [in]  u8CoupleCh       channel of PWM couple
 ** \arg  TIMER4_PWM_CH10 ~ TIMER4_PWM_CH54
 ** \param [in] u16CycleA         PDRA value
 ** \arg  0~65535
 ** \param [in]  u16CycleB        PDRB value
 ** \arg  0~65535
 **
 ** \retval Ok                    Timer count cycle is written
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8CoupleCh out of range
 **
 ******************************************************************************/
en_result_t Timer4_Pwm_WriteDeadRegionValue(uint8_t u8CoupleCh,
                                         uint16_t u16PDRA,
                                         uint16_t u16PDRB)
{
    volatile uint16_t* pu16PDRA;
    volatile uint16_t* pu16PDRB;

    /* Check parameters */
    ASSERT(IS_VALID_COUPLE_CH(u8CoupleCh));

    if(u8CoupleCh > TIMER4_PWM_MAX_COUPLE_CH)
    {
        return ErrorInvalidParameter;
    }

    /* Get pointer of current channel PWM register address */
    pu16PDRA = (volatile uint16_t*)((volatile uint8_t*)&M0P_TIMER4->PDRA10 + u8CoupleCh*8u);
    pu16PDRB = (volatile uint16_t*)((volatile uint8_t*)&M0P_TIMER4->PDRB10 + u8CoupleCh*8u);

    /* set the register */
    *pu16PDRA = u16PDRA;
    *pu16PDRB = u16PDRB;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Write dead region count value
 **
 ** \param [in]  u8CoupleCh       channel of PWM couple
 ** \arg  TIMER4_PWM_CH10 ~ TIMER4_PWM_CH54
 ** \param [in] pu16PDRA          pointer of uint16_t type
 ** \arg  0~65535
 ** \param [in]  pu16PDRB         pointer of uint16_t type
 ** \arg  0~65535
 **
 ** \retval Ok                    Timer count cycle is written
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8CoupleCh out of range
 **
 ******************************************************************************/
en_result_t Timer4_Pwm_ReadDeadRegionValue(uint8_t u8CoupleCh,
                                         uint16_t* pu16PDRA,
                                         uint16_t* pu16PDRB)
{
    /* Check parameters */
    ASSERT(IS_VALID_COUPLE_CH(u8CoupleCh));

    if(u8CoupleCh > TIMER4_PWM_MAX_COUPLE_CH)
    {
        return ErrorInvalidParameter;
    }

    /* Get pointer of current channel PWM register address */
    *pu16PDRA = *((volatile uint16_t*)((volatile uint8_t*)&M0P_TIMER4->PDRA10 + u8CoupleCh*8u));
    *pu16PDRB = *((volatile uint16_t*)((volatile uint8_t*)&M0P_TIMER4->PDRB10 + u8CoupleCh*8u));

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Set cycle of PWM timer
 **
 ** \param [in] u8CoupleCh        channel of PWM couple
 ** \arg  TIMER4_PWM_CH10 ~ TIMER4_PWM_CH54
 ** \param [in]  u16Count         pwm pulse counter value
 ** \arg   0~65535
 **
 ** \retval Ok                    cycle of PWM timer is set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - u8CoupleCh out of range
 **
 ******************************************************************************/
en_result_t Timer4_Pwm_SetFilterCountValue(uint8_t u8CoupleCh,
                                             uint16_t u16Count)
{
    volatile uint16_t* pu16PFSR;

    /* Check parameters */
     ASSERT(IS_VALID_COUPLE_CH(u8CoupleCh));

    if(u8CoupleCh > TIMER4_PWM_MAX_COUPLE_CH)
    {
        return ErrorInvalidParameter;
    }

    /* Get pointer of current channel PWM register address */
    pu16PFSR = (volatile uint16_t*)((volatile uint8_t*)&M0P_TIMER4->PFSR10 + u8CoupleCh*8u);

    *pu16PFSR =u16Count;

    return Ok;
}

//@} // Timer4PwmGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
