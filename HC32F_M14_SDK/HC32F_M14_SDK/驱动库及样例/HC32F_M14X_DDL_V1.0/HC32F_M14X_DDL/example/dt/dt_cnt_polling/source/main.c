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
 ** \brief This example shows how to use the dt work on demand
 **
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
#define DT_CH               (DtChannel1)
#define TESTPIN_PORT        (0)
#define TESTPIN_PIN         (0)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

 /**
 *******************************************************************************
 ** \brief  Configuration for channel 1
 **
 ******************************************************************************/
static  stc_dt_channel_config_t stcDtConfig = {
    DtModePeriodic,             ///< Periodic mode
    DtPrescalerDiv256,          ///< Prescaler dividor f/256
    DtCounterSize32,            ///< 32bits counter size
    FALSE,                      ///< irq enable
    NULL,                       ///< callback function
    FALSE,                      ///< enable nvic
};

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
 ** \brief  Main function of project
 **
 ** \return int32_t return value, if needed
 **
 ** This sample use Channel 2 in polling mode. when met the interrupt, switch
 ** a IO level output.
 **
 ******************************************************************************/
int32_t main(void)
{
    uint8_t u8Flag = 0;
    uint32_t u32Hz = 0;

    TestPinInit();

    if (Ok != Dt_Init(DT_CH,
                      (stc_dt_channel_config_t *)&stcDtConfig))
    {
        return Error;
    }

    u32Hz = Clk_GetPeripheralClk(ClkPeripheralDt);
    u32Hz = (u32Hz / 256) / 100;

    Dt_WriteLoadVal(DT_CH, u32Hz);
    Dt_WriteBgLoadVal(DT_CH, u32Hz);
    Dt_EnableCount(DT_CH);

    while (1)
    {
        if (TRUE == Dt_GetIrqFlag(DT_CH))
        {
            Dt_ClrIrqFlag(DT_CH);
            u8Flag = !u8Flag;
            SetTestPin(u8Flag);
        }
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
