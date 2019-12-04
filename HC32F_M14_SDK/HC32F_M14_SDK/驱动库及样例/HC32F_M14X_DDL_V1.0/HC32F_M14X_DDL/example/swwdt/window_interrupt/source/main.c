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
 ** \brief This example shows how to use the window interrupt function of
 **      the software watchdog timer.
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     swwdt.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "swwdt.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* LED I/O */
#define Led_Init() Gpio_InitIO(2, 4, GpioDirOut, FALSE, FALSE)
#define Led_On()   Gpio_SetIO(2, 4, FALSE)
#define Led_Off()  Gpio_SetIO(2, 4, TRUE)

/* Feed swwdt in window when this value is 0 ;
   Feed swwdt out window when this value is 1 */
#define TRIGGER_OUT_WINDOW_IRQ  (1)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/


/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SwwdtIrqCb(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static volatile uint32_t m_u32CountWdt = 0u;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief  Software watchdog interrupt callback function
 **
 ******************************************************************************/
static void SwwdtIrqCb(void)
{
    /* Software watchdog timer has to be stopped since interruption occurs,
       when interruption cause was cleared and watchdog counter was reloaded. */
    Swwdt_Stop();  /* Stop software watchdog timer */
    Swwdt_Feed();  /* Feed software watchdog timer and clear interrupt */
    Swwdt_Start(); /* Start software watchdog timer */

    if(++m_u32CountWdt % 2)
    {
        Led_On();   /* Led on */
    }
    else
    {
        Led_Off();  /* Led off */
    }
}

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \retval int32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    uint32_t u32CmpVal = 0u;
    volatile uint32_t u32CurCount = 0;
    stc_swwdt_config_t stcSwwdtConfig;

    /* Clear structure */
    DDL_ZERO_STRUCT(stcSwwdtConfig);

    /* The following setting only used for debugging */
    Swwdt_EnableDbgBrkWdtCtl();

    /* Initialize IO */
    Led_Init();

    /* Initialize SWWDT configure structure */
    stcSwwdtConfig.u32LoadValue = 2000000u; /* Timer interval */
    stcSwwdtConfig.bWinWdtEnable = TRUE;    /* Enables Window watchdog timer mode */
    stcSwwdtConfig.bWinWdtResetEnable = FALSE;
    stcSwwdtConfig.enTimingWindow = en_swwdt_timing_window_75;
    stcSwwdtConfig.pfnSwwdtIrqCb = SwwdtIrqCb;
    /* Set SWWDT configure structure load value*/
    if(en_swwdt_timing_window_100 == stcSwwdtConfig.enTimingWindow)
    {
        u32CmpVal = stcSwwdtConfig.u32LoadValue * 95/100;
    }
    else if(en_swwdt_timing_window_75 == stcSwwdtConfig.enTimingWindow)
    {
        u32CmpVal = stcSwwdtConfig.u32LoadValue * 3/4;
    }
    else if(en_swwdt_timing_window_50 == stcSwwdtConfig.enTimingWindow)
    {
        u32CmpVal = stcSwwdtConfig.u32LoadValue * 1/2;
    }
    else if(en_swwdt_timing_window_25 == stcSwwdtConfig.enTimingWindow)
    {
        u32CmpVal = stcSwwdtConfig.u32LoadValue * 1/4;
    }
    else
    {

    }

    /* Initialize Software watchdog timer */
    if (Ok != Swwdt_Init(&stcSwwdtConfig))
    {
        while (1)
        {
            /* nop */
        }
    }

    Swwdt_Start(); /* Start software watchdog timer */

#if (0 != TRIGGER_OUT_WINDOW_IRQ) /* Feed swwdt out window when TRIGGER_OUT_WINDOW_IRQ value is 1 */
    u32CmpVal += 200;
#endif

    do
    {
        u32CurCount = Swwdt_ReadWdtValue();
        if(u32CurCount < u32CmpVal)
        {
            Swwdt_Feed();  /* Feed software watchdog timer */
        }
    }while(1);
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
