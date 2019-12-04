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
 ** \brief This example shows how to use the interrupt mode to TX/RX data via
 **   UART.
 **
 **   - 2017-08-08  1.0  HeChun        version 1.0
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "uart.h"

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
uint8_t u8TxData[20] = "HUADA MCU!";
uint16_t u16RxData[10];
uint8_t u8TxCnt = 0, u8RxCnt = 0,uarttmp=0,uarttmp1=0,uarttmp2=0;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define UART0  UARTCH4
#define UART1  UARTCH5
#define UART2  UARTCH6

/*******************************************************************************
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
 ** \brief  UART1 TX interrupt callback function
 **
 ******************************************************************************/
void Tx1IntCallback(void)
{
    Uart_SendData(UART1, u8TxData[u8TxCnt]);
    u8TxCnt++;
    if(u8TxCnt > 9)
    {
      Uart_DisableIrq(UART1, UartTxIrq);
      return;
    }
}

/**
 *******************************************************************************
 ** \brief  UART1 RX interrupt callback function
 **
 ******************************************************************************/
void Rx1IntCallback(void)
{
    Uart_ReceiveData(UART1,&u16RxData[u8RxCnt]);
    u8RxCnt++;

    if(u8RxCnt > 9)
    {
      u8RxCnt = 0;
      Uart_EnableIrq(UART1, UartTxIrq);
      return;
    }
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
    stc_uart_config_t stcUartConfig;
    stc_uart_irq_cb_t stcUart1IrqCb;

    DDL_ZERO_STRUCT(stcUartConfig);
    DDL_ZERO_STRUCT(stcUart1IrqCb);


    Clk_SwitchTo(ClkHSXT);

    UartIoConfig();

    /* Init the UART interrupt structure */
    stcUart1IrqCb.pfnRxIrqCb = Rx1IntCallback;
    stcUart1IrqCb.pfnTxIrqCb = Tx1IntCallback;

    /* Config the UARTTX */
    stcUartConfig.bCarryEnable = FALSE;
    stcUartConfig.bCarryPolarity = FALSE;
    stcUartConfig.bCompensation = FALSE;
    stcUartConfig.bHwFlow = FALSE;
    stcUartConfig.bInvertData = FALSE;
    stcUartConfig.bTouchNvic = TRUE;
    stcUartConfig.bUseExtClk = FALSE;
    stcUartConfig.enBitDirection = UartDataLsbFirst;
    stcUartConfig.enDataLength = UartEightBits;
    stcUartConfig.enMode = UartNormal;
    stcUartConfig.enParity = UartParityNone;
    stcUartConfig.enStopBit = UartOneStopBit;
    stcUartConfig.u32BaudRate = 9600;
    stcUartConfig.pstcIrqCb = &stcUart1IrqCb;

    Uart_Init(UART1, &stcUartConfig);
    Uart_EnableFunc(UART1, UartRx);
    Uart_EnableFunc(UART1, UartTx);
    Uart_EnableIrq(UART1, UartRxIrq);
    while(1)
    {
        ;
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
