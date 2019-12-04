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
 ** \brief This example shows how to use the UART low power recive function to
 **   wake up the MCU from low power mode.
 **
 **   - 2017-08-10  1.0  HeChun        version 1.0
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "uart.h"
#include "lpm.h"
#include "clk.h"

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define UART_BAUDRATE   (9600) /*< define uart bps 9600 */

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
uint8_t u8TxData[10] = "HUADA MCU!"; /* define send buf */
uint16_t u16RxData[10];              /* define receive buf */
uint8_t u8TxCnt = 0;                 /* send count */
uint8_t u8RxCnt = 0;                 /* receive count */

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief  Uart IO Configuration function
 **
 ******************************************************************************/
static void UartIoConfig(void)
{
    Gpio_SetFunc_SIN5_0(0u);
    Gpio_SetFunc_SOT5_0(0u);
}

/**
 *******************************************************************************
 ** \brief  UART5 RX interrupt callback function
 ******************************************************************************/
void Uart5RxIntCallback(void)
{
    /* disable channel 5 receive interrupt */
    Uart_DisableIrq(UARTCH5,UartRxIrq);
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
    /* define uart configure structure */
    stc_uart_config_t stcUart5Config;
    /* define uart interrupt callback function structure */
    stc_uart_irq_cb_t stcUart5IrqCb;
    /* define uart interrupt configure structure */
    stc_uart_irq_en_t stcIrqEn;

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
    stcCfg.enClkSrc = ClkLSXT;
    stcCfg.bSubEn = TRUE;
    Clk_Init(&stcCfg);
    /* set clock work on ClkLSXT */
    Clk_SwitchTo(ClkLSXT);

    /* init test gpio */
    Gpio_InitIO(2, 4, GpioDirOut, FALSE, FALSE);
    Gpio_InitIO(0, 0, GpioDirIn, FALSE, FALSE);
    /* open test led */
    Gpio_SetIO(2,4,0);
    /* Check the IO input value */
    while(1 == Gpio_GetIO(0,0))
    {

    }

    /* clear structure */
    DDL_ZERO_STRUCT(stcUart5Config);
    DDL_ZERO_STRUCT(stcUart5IrqCb);
    DDL_ZERO_STRUCT(stcIrqEn);

    /* Config the UART IO */
    UartIoConfig();

    /* Init the UART */
    stcUart5IrqCb.pfnRxIrqCb = Uart5RxIntCallback;
    stcIrqEn.bRxIrq = TRUE;

    /* Config the UART */
    /* disable carry */
    stcUart5Config.bCarryEnable = FALSE;
    /* set Carry Polarity is high */
    stcUart5Config.bCarryPolarity = FALSE;
    /* enable bps Compensation */
    stcUart5Config.bCompensation = TRUE;
    /* disable hardware control */
    stcUart5Config.bHwFlow = FALSE;
    /* disable the data Invert */
    stcUart5Config.bInvertData = FALSE;
    /* enable interrupt NVIC */
    stcUart5Config.bTouchNvic = TRUE;
    stcUart5Config.bUseExtClk = FALSE;
    stcUart5Config.enBitDirection = UartDataLsbFirst;
    stcUart5Config.enDataLength = UartEightBits;
    stcUart5Config.enMode = UartNormal;
    stcUart5Config.enParity = UartParityNone;
    stcUart5Config.enStopBit = UartOneStopBit;
    stcUart5Config.u32BaudRate = 9600;
    stcUart5Config.pstcIrqCb = &stcUart5IrqCb;
    stcUart5Config.pstcIrqEn = &stcIrqEn;
    /* enable bps Compensation bits */
    stcUart5Config.u16CompenConf = 0x0155;

    Uart_Init(UARTCH5, &stcUart5Config);

    /* Enable the RX function */
    Uart_EnableFunc(UARTCH5, UartRx);
    /* Enable RX interrupt */
    Uart_EnableIrq(UARTCH5, UartRxIrq);
    /* Enter the IdleMode mode */
    Lpm_GoToStandByMode(IdleMode , TRUE);

    /* Wakeup by receive the data close led */
    Gpio_SetIO(2,4,1);

    while(1){}
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
