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
 ** \brief This example demonstrates how to use the interrupt function of the
 **        hardware watchdog timer.
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     hwwdt.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hwwdt.h"

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

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void HwwdtIrqCb(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint32_t m_u32CountWdt = 0;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief  Hardware watchdog interrupt callback function
 **
 ******************************************************************************/
static void HwwdtIrqCb(void)
{
    Hwwdt_Feed(0x55, 0xAA);   /* Clear Irq and Reset Timer */

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
 *******************************************************************************
 ** \brief  Main function of project
 **
 ** \retval int32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_hwwdt_config_t stcHwwdtConfig;

    /* Clear structure */
    DDL_ZERO_STRUCT(stcHwwdtConfig);

    /* The following setting only used for debugging */
    Hwwdt_EnableDbgBrkWdtCtl();

    /* Initialize IO */
    Led_Init();

    /* Initialize hwwdt configration structure */
    stcHwwdtConfig.u32LoadValue = 0x0FFFu;
    stcHwwdtConfig.bResetEnable = FALSE;     /* Disables Hardware watchdog timer reset */
    stcHwwdtConfig.pfnHwwdtIrqCb = HwwdtIrqCb;
    if (Ok != Hwwdt_Init(&stcHwwdtConfig))  /* Initialize hardware watchdog  timer */
    {
        while(1)
        {
            /* nop */
        }
    }
    else
    {
        Hwwdt_Start();  /* Start hardware watchdog timer */
    }

    /* wait for interrupts */
    while(1)
    {
        /* nop */
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
