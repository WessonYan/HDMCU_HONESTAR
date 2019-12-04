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
/** \file main.c
 **
 ** \brief This example demonstrates how to use the reload timer mode function
 **      of the timer4-pwm.
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     timer4-pwm.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "timer4_cnt.h"
#include "timer4_oco.h"
#include "timer4_pwm.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Timer4 PWM */
#define PWM_COUPLE_CH  TIMER4_PWM_COUPLE_CH10
#define PWM_RT_VAL     (10000u)

/* FAST I/O */
#define FAST_IO_PORT   (7)
#define FAST_IO_PIN    (0)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void ToggleFastIo(uint8_t u8Port, uint8_t u8Pin);
static void PwmCoupleChIrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief pwm couple channel0&1 reload timer interrupt handler
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Toggle fast gpio.
 **
 ** \param [in] u8Port                  Fast IO port
 ** \param [in] u8Pin                   Fast IO pin
 **
 ** \retval                             None
 **
 ******************************************************************************/
static void ToggleFastIo(uint8_t u8Port, uint8_t u8Pin)
{
    if(FALSE == FGpio_GetIO(u8Port, u8Pin))
    {
        FGpio_SetIO(u8Port, u8Pin, TRUE);
    }
    else
    {
        FGpio_SetIO(u8Port, u8Pin, FALSE);
    }

    if(FALSE == FGpio_GetIO(u8Port, u8Pin))
    {
        FGpio_SetIO(u8Port, u8Pin, TRUE);
    }
    else
    {
        FGpio_SetIO(u8Port, u8Pin, FALSE);
    }
}

/**
 *******************************************************************************
 ** \brief pwm couple channel reload timer interrupt handler
 ******************************************************************************/
static void PwmCoupleChIrqCallback(void)
{
    ToggleFastIo(FAST_IO_PORT, FAST_IO_PIN);
}

/**
 *******************************************************************************
 ** \brief  Main function of project
 **
 ** \retval int32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_timer4_pwm_config_t stcPwmChConfig;

    /* Clear structures */
    DDL_ZERO_STRUCT(stcPwmChConfig);

    /* Initialize I/O */
    FGpio_InitIO(FAST_IO_PORT, FAST_IO_PIN, GpioDirOut);
    FGpio_SetIO(FAST_IO_PORT, FAST_IO_PIN, FALSE);

    /* Timer4 PWM: Initialize PWM couple channel configuration structure */
    stcPwmChConfig.bPwmTimerIrqMask = FALSE;
    stcPwmChConfig.pfnPwmTimerIrqCb = PwmCoupleChIrqCallback;
    stcPwmChConfig.bTouchNvic = TRUE;
    stcPwmChConfig.enClk = PwmPlckDiv1;
    stcPwmChConfig.enLvlsBits = LvlsBits00B;
    stcPwmChConfig.enMode = PwmThroughMode;  /* Change: PwmDeadTimerMode  PwmThroughMode */
    Timer4_Pwm_Init(PWM_COUPLE_CH, &stcPwmChConfig);  /* Initialize PWM channel */
    Timer4_Pwm_SetFilterCountValue(PWM_COUPLE_CH, PWM_RT_VAL);

    /* Start pwm count */
    Timer4_Pwm_StartTimer(PWM_COUPLE_CH);

    /* Toggle Fast I/O */
    ToggleFastIo(FAST_IO_PORT, FAST_IO_PIN);

    while(1)
    {
        /* nop */
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
