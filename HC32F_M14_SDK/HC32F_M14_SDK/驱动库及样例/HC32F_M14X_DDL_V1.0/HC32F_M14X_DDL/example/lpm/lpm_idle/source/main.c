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
 ** \brief This example demonstrates the process of idle mode.
 **
 **   - 2017-08-08  1.0  HeChun        version 1.0
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "lpm.h"

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Use P50 to enter low power mode. */
#define MODE_SWTICH_TRIGGER_INIT()      Gpio_InitIO(5,4,GpioDirIn,FALSE,FALSE)
/* Wait util the P54 is "L" */
#define MODE_SWTICH_TRIGGER_CHECK()     (bM0P_GPIO_GET5_P54 == 1 ? TRUE : FALSE)

/* Use P24 to test gpio. */
#define TEST_GPIO_INIT()                Gpio_InitIO(2,4,GpioDirOut,TRUE,FALSE)

/* ClkHICR/ClkHSXT/ClkMPLL/ClkLICR/ClkLSXT */
#define IDLE_CLOCK                      ClkHSXT
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

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief exint interrupt initial
 **
 ** \return none
 **
 ******************************************************************************/
static void ExitInt00En(void)
{
    /* set INT00_1 as the input pin of EINT ch.0 pin0 */
    Gpio_SetFunc_INT00_2(0u);

    M0P_EXTINT->INTLVL_f.LVLL0 = 1;
    /* Detects down edge. */
    M0P_EXTINT->INTLVL_f.LVLH0 = 1;
    /* Clears an external interrupt factor of INTx pin corresponding to the relevant bit. */
    M0P_EXTINT->INTCLR = 0;
    /* Enable exint INT00 */
    M0P_EXTINT->INTEN_f.EN0 = 1;

    NVIC_ClearPendingIRQ(EXINT0_7_IRQn);
    NVIC_SetPriority(EXINT0_7_IRQn, 0);
    NVIC_EnableIRQ(EXINT0_7_IRQn);
}

/**
 *******************************************************************************
 ** \brief  exint interrupt 07 handle
 **
 ** \return none
 **
 ******************************************************************************/
void Exti_07IrqHandler(uint8_t u8Param)
{
    /* clear interrupt flag */
    M0P_EXTINT->INTCLR = 0;
}

/**
 *******************************************************************************
 ** \brief  Main function of project
 **
 ** \return int32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    /* config system clock */
    stc_clk_config_t    stcCfg;
    DDL_ZERO_STRUCT(stcCfg);

    stcCfg.enBaseClkDiv = ClkBaseDiv1;
    stcCfg.enAPB1Div = ClkApb1Div1;
    stcCfg.enClkWaitMain = ClkCswtMain1ms;
    stcCfg.enAPB0Div = ClkApb0Div1;
    stcCfg.enClkWaitSub = ClkCswtSub256ms;
    stcCfg.enClkSrc = IDLE_CLOCK;

    if(ClkMPLL == IDLE_CLOCK)
    {
        stcCfg.bPllEn = TRUE;
        stcCfg.enPllSrc = HICR2PLL;
        stcCfg.u8PllMulti = 2;
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
    Clk_Init(&stcCfg);
    ExitInt00En();

    TEST_GPIO_INIT();
    MODE_SWTICH_TRIGGER_INIT();
    /* open led */
    Gpio_SetIO(2,4,0);
    while(TRUE == Gpio_GetIO(5,4))
    {
        delay1ms(1);
    }
    /* close led */
    Gpio_SetIO(2,4,1);
    delay1ms(1);

    /* enter idle mode */
    Lpm_GoToStandByMode(IdleMode, TRUE);
    /* wake up open led */
    Gpio_SetIO(2,4,0);
    while(1)
    {
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
