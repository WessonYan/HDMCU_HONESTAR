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
 **
 ** \brief This example shows how to use the dt work on interrupt
 **
 ** History:
 **   - 2017-08-08  1.0  HeChun        version 1.0
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "dt.h"

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define TESTPIN_PORT        (0)
#define TESTPIN_PIN         (0)

#define TESTPIN_PORT1       (5)
#define TESTPIN_PIN1        (4)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void Dt1IrqCbk(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief  Configuration for channel 1
 **
 ******************************************************************************/
static stc_dt_channel_config_t stcDtChannelConfig1 = {
    DtModeFreeRun,              ///< Periodic mode
    DtPrescalerDiv1,            ///< Prescaler dividor f/256
    DtCounterSize16,            ///< 32bits counter size
    TRUE,                       ///< irq enable
    Dt1IrqCbk,                  ///< callback function
    TRUE,                       ///< enable nvic
};

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief  init test io
 **
 ******************************************************************************/
void TestPinInit(void)
{
    Gpio_InitIO(TESTPIN_PORT, TESTPIN_PIN, GpioDirOut, FALSE, FALSE);
    Gpio_InitIO(TESTPIN_PORT1, TESTPIN_PIN1, GpioDirOut, FALSE, FALSE);
}

/**
 *******************************************************************************
 ** \brief  set io polarity
 **
 ******************************************************************************/
void SetTestPin(uint8_t u8Flag)
{
    Gpio_SetIO(TESTPIN_PORT, TESTPIN_PIN, u8Flag);
}

/**
 *******************************************************************************
 ** \brief  DT callback
 **
 ******************************************************************************/
void Dt1IrqCbk(void)
{
    static uint8_t u8Flag = 0;
    u8Flag = !u8Flag;
    SetTestPin(u8Flag);
}

/**
 *******************************************************************************
 ** \brief  Main function of project
 **
 ** \return int32_t return value, if needed
 **
 ** This sample use Channel 1 in interrupt mode. when met the interrupt, switch
 ** a IO level ouput.
 **
 ******************************************************************************/
int32_t main(void)
{
    uint32_t u32Hz = 0;
    stc_clk_config_t    stcCfg;

    stcCfg.enBaseClkDiv = ClkBaseDiv1;
    stcCfg.enAPB0Div = ClkApb0Div1;
    stcCfg.enAPB1Div = ClkApb1Div1;

    Clk_Init(&stcCfg);
    Clk_SwitchTo(ClkHICR);
    TestPinInit();

    if (Ok != Dt_Init(DtChannel1,
                      (stc_dt_channel_config_t *)&stcDtChannelConfig1))
    {
        return Error;
    }

    u32Hz = Clk_GetPeripheralClk(ClkPeripheralDt);
    u32Hz = (u32Hz / 1) / 100;

    Dt_WriteLoadVal(DtChannel1, u32Hz);
    Dt_WriteBgLoadVal(DtChannel1, u32Hz);
    Dt_EnableCount(DtChannel1);

    while (1)
    {
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
