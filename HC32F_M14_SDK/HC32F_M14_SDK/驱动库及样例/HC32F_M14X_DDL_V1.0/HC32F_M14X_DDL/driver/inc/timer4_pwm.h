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
/** \file timer4_pwm.h
 **
 ** A detailed description is available at
 ** @link Timer4PwmGroup Timer4Pwm description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     Timer4Pwm
 **
 ******************************************************************************/

#ifndef __TIMER4_PWM_H__
#define __TIMER4_PWM_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32f_m14x.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup Timer4PwmGroup Timer4 Pulse Width Modulation(Timer4Pwm)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/*******************************************************************************
 * \brief PWM internal data
 ******************************************************************************/
typedef struct stc_timer4_pwm_timer_cb
{
    func_ptr_t  pfnPwm10TimerIrqCallback;   ///< Callback function pointer of PWM10 timer interrupt callback
    func_ptr_t  pfnPwm32TimerIrqCallback;   ///< Callback function pointer of PWM32 timer interrupt callback
    func_ptr_t  pfnPwm54TimerIrqCallback;   ///< Callback function pointer of PWM54 timer interrupt callback
}stc_timer4_pwm_timer_cb_t;


/*******************************************************************************
 * \brief PWM operation mode
 ******************************************************************************/
typedef enum en_timer4_pwm_mode
{
    PwmThroughMode         = 0u,        ///< through mode
    PwmDeadTimerMode       = 1u,        ///< Dead timer mode
    PwmDeadTimerFilterMode = 2u,        ///< Dead timer filter mode
}en_timer4_pwm_mode_t;

/*******************************************************************************
 * \brief Enumeration of DMOD bit setting
 ******************************************************************************/
typedef enum en_lvls_bits
{
    LvlsBits00B = 0u,                   ///< output PWM1 and PWM0 signals without changing the level
    LvlsBits01B = 1u,                   ///< output both PWM1 and PWM0 signals reversed
    LvlsBits10B = 2u,                   ///< Outputs the PWM(0) signal reversed, outputs the PWM(1) signal without changing the level.
    LvlsBits11B = 3u,                   ///< Outputs the PWM(0) signal without changing the level, Outputs the PWM(1 ) signal reversed.
}en_lvls_bits_t;

/*******************************************************************************
 * \brief Enumeration to set count clock prescaler
 ******************************************************************************/
typedef enum en_pwm_timer_clock
{
    PwmPlckDiv1   = 0u,                 ///< PWM timer clock prescaler: None
    PwmPlckDiv2   = 1u,                 ///< PWM timer clock prescaler: 1/2
    PwmPlckDiv4   = 2u,                 ///< PWM timer clock prescaler: 1/4
    PwmPlckDiv8   = 3u,                 ///< PWM timer clock prescaler: 1/8
    PwmPlckDiv16  = 4u,                 ///< PWM timer clock prescaler: 1/16
    PwmPlckDiv32  = 5u,                 ///< PWM timer clock prescaler: 1/32
    PwmPlckDiv64  = 6u,                 ///< PWM timer clock prescaler: 1/64
    PwmPlckDiv128 = 7u,                 ///< PWM timer clock prescaler: 1/128
}en_pwm_timer_clock_t;

/*******************************************************************************
 * \brief Configuration structure of PWM generator
 ******************************************************************************/
typedef struct stc_timer4_pwm_config
{
    en_timer4_pwm_mode_t enMode;        ///< select the output condition
    en_lvls_bits_t   enLvlsBits;        ///< polarity for PWM0 and PWM1 signal output

    en_pwm_timer_clock_t enClk;         ///< clock division of PWM timer

    boolean_t   bPwmTimerIrqMask;       ///< TRUE: mask timer interrupt, FALSE: don't mask timer interrupt
    func_ptr_t  pfnPwmTimerIrqCb;       ///< Poitner to PWM timer callback function
    boolean_t   bTouchNvic;             ///< TRUE: enable PWM NVIC; FALSE: don't enable PWM NVIC
}stc_timer4_pwm_config_t;


/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define TIMER4_PWM_CH0  (0u)            ///< Timer4 PWM channel NO.0
#define TIMER4_PWM_CH1  (1u)            ///< Timer4 PWM channel NO.1
#define TIMER4_PWM_CH2  (2u)            ///< Timer4 PWM channel NO.2
#define TIMER4_PWM_CH3  (3u)            ///< Timer4 PWM channel NO.3
#define TIMER4_PWM_CH4  (4u)            ///< Timer4 PWM channel NO.4
#define TIMER4_PWM_CH5  (5u)            ///< Timer4 PWM channel NO.5
#define TIMER4_PWM_MAX_CH  (TIMER4_PWM_CH5)     ///< Timer4 PWM max channel NO.

#define TIMER4_PWM_COUPLE_CH10  (0u)    ///< Timer4 PWM couple channel NO.1&0
#define TIMER4_PWM_COUPLE_CH32  (1u)    ///< Timer4 PWM couple channel NO.3&2
#define TIMER4_PWM_COUPLE_CH54  (2u)    ///< Timer4 PWM couple channel NO.5&4
#define TIMER4_PWM_MAX_COUPLE_CH  (TIMER4_PWM_COUPLE_CH54)      ///< Timer4 PWM max couple channel

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/


/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/* 1. PWM function configuration */
extern en_result_t Timer4_Pwm_Init(uint8_t u8CoupleCh,
                               stc_timer4_pwm_config_t* pstcConfig);
extern en_result_t Timer4_Pwm_DeInit(uint8_t u8CoupleCh, boolean_t bTouchNvic);

/* 2. Write/read clock/mode */
extern en_result_t Timer4_Pwm_SetClkDiv(uint8_t u8CoupleCh);
extern en_result_t Timer4_Pwm_GetClkDiv(uint8_t u8CoupleCh);
extern en_result_t Timer4_Pwm_SetMode(uint8_t u8CoupleCh);

/* 3. Function Enable/Disable */
extern en_result_t Timer4_Pwm_StartTimer(uint8_t u8CoupleCh);
extern en_result_t Timer4_Pwm_StopTimer(uint8_t u8CoupleCh);

/* 4. Interrupt flag read/clear */
extern en_irq_flag_t Timer4_Pwm_GetTimerIrqFlag(uint8_t u8CoupleCh);
extern en_result_t Timer4_Pwm_ClrTimerIrqFlag(uint8_t u8CoupleCh);

/* 5. Write/read dead timer */
extern en_result_t Timer4_Pwm_WriteDeadRegionValue(uint8_t u8CoupleCh,
                                         uint16_t u16PDRA,
                                         uint16_t u16PDRB);
extern en_result_t Timer4_Pwm_ReadDeadRegionValue(uint8_t u8CoupleCh,
                                         uint16_t* pu16PDRA,
                                         uint16_t* pu16PDRB);

/* 6. Write/read filter timer */
extern en_result_t Timer4_Pwm_SetFilterCountValue(uint8_t u8CoupleCh,
                                         uint16_t u16Count);
extern uint16_t Timer4_Pwm_GetFilterCountValue(uint8_t u8CoupleCh);

//@} // Timer4PwmGroup

#ifdef __cplusplus
}
#endif

#endif /* __TIMER4_PWM_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
