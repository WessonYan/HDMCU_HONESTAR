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
 ** \brief The example for priority of interrupt
 **
 **   - 2017-08-10  1.0  WangM First version for sample of I2C module.
 **
 ******************************************************************************/
/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "exint.h"
#include "swwdt.h"
#include "dt.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Dt1IrqCbk(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
uint32_t u32ExtInt0Count = 0u, u32ExtInt1Count = 0u;
uint32_t u32NmiCount = 0u;

static  stc_dt_channel_config_t stcDtChannelConfig1 = {
    DtModePeriodic,             /* Periodic mode */
    DtPrescalerDiv256,          /* Prescaler dividor f/256 */
    DtCounterSize32,            /* 32bits counter size */
    TRUE,                       /* irq enable */
    Dt1IrqCbk,                  /* callback function */
    TRUE,                       /* enable nvic */
};

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief ExtInt0 callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
void Main_ExtInt0Callback(void)
{
    u32ExtInt0Count++;
    while(1)
    {
    }
}

/**
 *******************************************************************************
 ** \brief ExtInt1 callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
void Main_ExtInt1Callback(void)
{
    u32ExtInt1Count++;
    while(1)
    {
    }
}

/**
 *******************************************************************************
 ** \brief NMI callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
void Main_NmiCallback(void)
{
    u32NmiCount++;
    Exint_Nmi_IrqFlagClear();
    while(1)
    {
    }
}

/**
 *******************************************************************************
 ** \brief Software Watchdog interrupt callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void WdtSwCallback(void)
{
    Swwdt_Feed();   /* Clear Irq and Reset Timer */
    Swwdt_Stop();
    while(1)
    {
    }
}

/**
 *******************************************************************************
 ** \brief Dual Timer interrupt callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void Dt1IrqCbk(void)
{
    static uint8_t u8Flag = 0u;
    u8Flag = !u8Flag;
    while(1)
    {
    }
}

/**
 *******************************************************************************
 ** \brief  main function for demonstration of interrupt priority
 **
 ** \param [in]  None
 **
 ** \return int32_t Return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_exint_config_t stcExintConfig;
    stc_swwdt_config_t stcSwwdtConfig;
    uint32_t u32Hz = 0;
    stc_exint_nmi_config_t stcNmiConfig;

    DDL_ZERO_STRUCT(stcExintConfig);

    /* Pin Function: INT00_2 */
    Gpio_SetFunc_INT00_2(0u);
    /* Enable channel 0 EXINT, interrupt priority level 0 */
    stcExintConfig.bEnable = 1u;
    stcExintConfig.enLevel = ExIntFallingEdge;
    stcExintConfig.pfnExintCallback = Main_ExtInt0Callback;
    stcExintConfig.bTouchNvic = TRUE;
    Exint_Init(ExintInstanceIndexExint0, &stcExintConfig);

    /* Pin Function: INT08_2 */
    Gpio_SetFunc_INT08_2(0u);
    /* Enable channel 15 EXINT, interrupt priority level 1 */
    stcExintConfig.bEnable = 1u;
    stcExintConfig.enLevel = ExIntFallingEdge;
    stcExintConfig.pfnExintCallback = Main_ExtInt1Callback;
    stcExintConfig.bTouchNvic = TRUE;
    Exint_Init(ExintInstanceIndexExint8, &stcExintConfig);

    /* Initialize Software watchdog, interrupt priority level 2 */
    Swwdt_EnableDbgBrkWdtCtl();
    DDL_ZERO_STRUCT(stcSwwdtConfig);
    stcSwwdtConfig.u32LoadValue = 5000000ul;   /* Timer interval, note that software watchdog's clock is related with SWC_PSR_Val */
    stcSwwdtConfig.bResetEnable = TRUE;        /* Enables SW watchdog reset */
    stcSwwdtConfig.pfnSwwdtIrqCb = WdtSwCallback;
    if (Ok != Swwdt_Init((stc_swwdt_config_t *)&stcSwwdtConfig))
    {
        while(1)
        {
        }
    }
    else
    {
        /* Start Software watchdog */
        Swwdt_Start();
    }

    /* Initialize Dual timer, interrupt priority level 3 */
    if (Ok != Dt_Init(DtChannel1, (stc_dt_channel_config_t *)&stcDtChannelConfig1))
    {
        return Error;
    }

    u32Hz = Clk_GetPeripheralClk(ClkPeripheralDt);
    u32Hz = (u32Hz / 256u) / 100u;        /* 100hz */

    Dt_WriteLoadVal(DtChannel1, u32Hz);
    Dt_WriteBgLoadVal(DtChannel1, u32Hz);
    Dt_EnableCount(DtChannel1);

    /* enable NMI interrupt, system request */
    DDL_ZERO_STRUCT(stcNmiConfig);
    stcNmiConfig.pfnNmiCallback = Main_NmiCallback;
    Exint_Nmi_Init(&stcNmiConfig);
    Gpio_SetFunc_NMIX(0u);

    while(1)
    {}
}
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
