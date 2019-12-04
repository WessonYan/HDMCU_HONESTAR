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
 ** \brief This example shows how to use the polling mode to TX/RX data through
 ** UART.
 **
 ** History:
 **   - 2017-08-08  1.0  HeChun        version 1.0
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "uart.h"
#include "string.h"

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
uint8_t u8TxData[10] = "HUADA MCU!";
uint16_t u16RxData[10];
uint8_t u8TxCnt = 0, u8RxCnt = 0;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define UARTTX  UARTCH5
#define UARTRX  UARTCH6

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
    Gpio_SetFunc_SIN5_1();
    Gpio_SetFunc_SOT5_1();

    Gpio_SetFunc_SIN6_1();
    Gpio_SetFunc_SOT6_1();
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
    stc_uart_config_t stcUartTxConfig;
    stc_uart_config_t stcUartRxConfig;
    uint8_t u8Cnt = 0;
    uint8_t u8Times;

    DDL_ZERO_STRUCT(stcUartTxConfig);
    DDL_ZERO_STRUCT(stcUartRxConfig);

    /* Config the UART IO */
    UartIoConfig();

    /* Config the channle5 */
    stcUartTxConfig.bCarryEnable = FALSE;
    stcUartTxConfig.bCarryPolarity = FALSE;
    stcUartTxConfig.bCompensation = FALSE;
    stcUartTxConfig.bHwFlow = FALSE;
    stcUartTxConfig.bInvertData = FALSE;
    stcUartTxConfig.bTouchNvic = FALSE;
    stcUartTxConfig.bUseExtClk = FALSE;
    stcUartTxConfig.enBitDirection = UartDataLsbFirst;
    stcUartTxConfig.enDataLength = UartEightBits;
    stcUartTxConfig.enMode = UartNormal;
    stcUartTxConfig.enParity = UartParityNone;
    stcUartTxConfig.enStopBit = UartOneStopBit;
    stcUartTxConfig.u32BaudRate = 9600;

    Uart_Init(UARTTX, &stcUartTxConfig);

    Uart_EnableFunc(UARTTX, UartTx);

    /* Config the channle6 */
    stcUartRxConfig.bCarryEnable = FALSE;
    stcUartRxConfig.bCarryPolarity = FALSE;
    stcUartRxConfig.bCompensation = FALSE;
    stcUartRxConfig.bHwFlow = FALSE;
    stcUartRxConfig.bInvertData = FALSE;
    stcUartRxConfig.bTouchNvic = FALSE;
    stcUartRxConfig.bUseExtClk = FALSE;
    stcUartRxConfig.enBitDirection = UartDataLsbFirst;
    stcUartRxConfig.enDataLength = UartEightBits;
    stcUartRxConfig.enMode = UartNormal;
    stcUartRxConfig.enParity = UartParityNone ;
    stcUartRxConfig.enStopBit = UartOneStopBit;
    stcUartRxConfig.u32BaudRate = 9600;

    Uart_Init(UARTRX, &stcUartRxConfig);

    Uart_EnableFunc(UARTRX, UartRx);

    /* Send/receive the data between Channel 5 and 6
    Channel 5 send and channle 6 receive */
    for(u8Times=0;u8Times<10;u8Times++)
    {
        while(u8Cnt < 10)
        {
            while (TRUE != Uart_GetStatus(UARTTX, UartTxEmpty)); /* wait until TX buffer empty */
            Uart_SendData(UARTTX, u8TxData[u8Cnt]);

            while(TRUE != Uart_GetStatus(UARTRX, UartRxFull));  /* wait until RX buffer full */
            Uart_ReceiveData(UARTRX,&u16RxData[u8Cnt]);
            u8Cnt++;
        }
        if(0 != memcmp(u8TxData,u16RxData,10))
        {
            while(1);
        }
    }

    Uart_DeInit(UARTTX, TRUE);
    Uart_DeInit(UARTRX, TRUE);

    while(1);
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
