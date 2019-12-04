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
 ** \brief This example shows how to use the clock counter
 **
 ** History:
 **   - 2017-08-08  1.0  HeChun        version 1.0
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "cc.h"

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define TESTPORT    (0)
#define TESTPIN     (0)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void TestPinInit(void);
static void SetTestPin(uint8_t u8Flag);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IDLE_CLOCK ClkHSXT

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief  Cc test IO Configuration function init
 **
 ******************************************************************************/
static void TestPinInit(void)
{
    Gpio_InitIO(TESTPORT, TESTPIN, GpioDirOut, FALSE, FALSE);
}

/**
 *******************************************************************************
 ** \brief  set test IO polarity
 **
 ******************************************************************************/
static void SetTestPin(uint8_t u8Flag)
{
    Gpio_SetIO(TESTPORT, TESTPIN, u8Flag);
}

/**
 *******************************************************************************
 ** \brief  Cc interrupt callback function
 **
 ******************************************************************************/
void Cc1IrqCbk(void)
{
    static uint8_t u8Flag = 0;
    u8Flag = !u8Flag;
    SetTestPin(u8Flag);
}

static  stc_cc_config_t stcCcChannelConfig = {
    HSXT,                ///< Periodic mode
    TRUE,                ///< Prescaler dividor
    Modulus2,            ///< set dividor mode
    Ck0,                 ///< set Prescaler
    TRUE,                ///< irq enable
    Cc1IrqCbk,           ///< set interrupt callback
    TRUE,                ///< enable Nvic
};

/**
 *******************************************************************************
 ** \brief  Main function of project
 **
 ** \return int32_t return value, if needed
 **
 ** \note This example show the clock init, do clock prescaler.
 **         as a result ,Cc init and how to do clock counter.
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_clk_config_t    stcCfg;
    DDL_ZERO_STRUCT(stcCfg);

    /* close clock monitor */
    M0P_CLOCK->FCM_CTL = 0u;
    stcCfg.enBaseClkDiv = ClkBaseDiv1;
    stcCfg.enAPB1Div = ClkApb1Div1;
    stcCfg.enClkWaitMain = ClkCswtMain1ms;
    stcCfg.enAPB0Div = ClkApb0Div1;
    stcCfg.enClkWaitSub = ClkCswtSub256ms;
    stcCfg.enClkWaitPll = ClkPswtPll680us;

    stcCfg.enClkSrc = IDLE_CLOCK;

    if(ClkMPLL == IDLE_CLOCK)
    {
        stcCfg.bPllEn = TRUE;
        stcCfg.enPllSrc = MAIN2PLL;
        if(stcCfg.enPllSrc == MAIN2PLL)
        {
           stcCfg.bMainEn = TRUE;
        }
        else
        {
          stcCfg.bHicrEn = TRUE;
        }
        stcCfg.u8PllMulti = 8u;
    }
    else if(ClkHICR == IDLE_CLOCK)
    {
        stcCfg.bHicrEn = TRUE;
    }
    else if(ClkHSXT == IDLE_CLOCK)
    {
        stcCfg.bMainEn = TRUE;
    }
    else if(ClkLSXT == IDLE_CLOCK)
    {
        stcCfg.bSubEn = TRUE;
    }
    else
    {
    }
    /* init main clock */
    Clk_Init(&stcCfg);
    /* init test io */
    TestPinInit();
    /* init cc */
    Cc_Init(&stcCcChannelConfig);
    /* set count number */
    Cc_WriteLoadVal(2);
    Cc_EnableCount();

    while (1)
    {
    }

}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
