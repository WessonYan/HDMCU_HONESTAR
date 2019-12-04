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
/** \file uart.h
 **
 ** A detailed description is available at
 ** @link UartleGroup Uart description @endlink
 **
 **   - 2017-6-10  1.0  HeChun First version for Device Driver Library of uart.
 **
 ******************************************************************************/

#ifndef _UART_H_
#define _UART_H_

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32f_m14x.h"

/* C binding of definitions if building with C++ compiler */
#ifdef _cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup UartGroup Universal Asynchronous Receiver/Transmitter(UART)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Uart channel value
 **
 ** \note set uart channel
 **
 ******************************************************************************/
typedef enum en_uart_channel
{
    UARTCH4 = 4u,    ///< uart channel 4
    UARTCH5 = 5u,    ///< uart channel 5
    UARTCH6 = 6u     ///< uart channel 6
}en_uart_channel_t;

/**
 *******************************************************************************
 ** \brief Uart work mode
 **
 ** \note set uart work mode
 **
 ******************************************************************************/
typedef enum en_uart_mode
{
    UartNormal = 0u,    ///< work in Normal mode
    UartMulti  = 1u     ///< work in Multi-Processor mode
} en_uart_mode_t;

/**
 *******************************************************************************
 ** \brief Uart date length
 **
 ** \note set uart date length
 **
 ******************************************************************************/
typedef enum en_uart_data_len
{
    UartEightBits      = 0u,    ///< 8 Bit character length
    UartFiveBits       = 1u,    ///< 5 Bit character length
    UartSixBits        = 2u,    ///< 6 Bit character length
    UartSevenBits      = 3u,    ///< 7 Bit character length
    UartNineBits       = 4u     ///< 9 Bit character length
} en_uart_data_len_t;

/**
 *******************************************************************************
 ** \brief Uart parity format
 **
 ** \note set uart parity format
 **
 ******************************************************************************/
typedef enum en_uart_parity
{
    UartParityNone  = 0u,    ///< No parity bit is used
    UartParityEven  = 1u,    ///< Even parity bit is used
    UartParityOdd   = 2u     ///< Odd parity bit is used
} en_uart_parity_t;

/**
 *******************************************************************************
 ** \brief Uart stop bits length
 **
 ** \note set uart stop bits length
 **
 ******************************************************************************/
typedef enum en_uart_stop_bit
{
    UartOneStopBit    = 0u,    ///< 1 Stop Bit
    UartTwoStopBits   = 1u,    ///< 2 Stop Bits
    UartThreeStopBits = 2u,    ///< 3 Stop Bits
    UartFourStopBits  = 3u     ///< 4 Stop Bits
} en_uart_stop_bit_t;

/**
 *******************************************************************************
 ** \brief Uart data direction
 **
 ** \note set uart data direction
 **
 ******************************************************************************/
typedef enum en_uart_data_dir
{
    UartDataLsbFirst = 0u,    ///< LSB first
    UartDataMsbFirst = 1u     ///< MSB first
}en_uart_data_dir_t;

/**
 *******************************************************************************
 ** \brief Uart functions
 **
 ** \note set uart functions
 **
 ******************************************************************************/
typedef enum en_uart_func
{
    UartTx = 0u,     ///< UART TX
    UartRx = 1u      ///< UART RX
}en_uart_func_t;

/**
 *******************************************************************************
 ** \brief Uart interrupt enable structure
 **
 ** \note enable  relevance interrupt
 **
 ******************************************************************************/
typedef struct stc_uart_irq_en
{
    boolean_t bTxIrq;          ///< UART TX interrupt
    boolean_t bRxIrq;          ///< UART RX interrupt
    boolean_t bTxIdleIrq;      ///< UART TX idle interrupt
}stc_uart_irq_en_t;

/**
 *******************************************************************************
 ** \brief Uart interrupt selection
 **
 ** \note Uart interrupt selection
 **
 ******************************************************************************/
typedef enum en_uart_irq_sel
{
    UartTxIrq       = 0u,      ///< UART TX interrupt
    UartRxIrq       = 1u,      ///< UART RX interrupt
    UartTxIdleIrq   = 2u       ///< UART TX idle interrupt
}en_uart_irq_sel_t;

/**
 *******************************************************************************
 ** \brief Uart interrupt selection
 **
 ** \note Uart interrupt selection
 **
 ******************************************************************************/
typedef struct stc_uart_irq_cb
{
    func_ptr_t pfnTxIrqCb;      ///< UART TX interrupt callback function pointer
    func_ptr_t pfnRxErrIrqCb;   ///< UART RX Error interrupt callback function pointer
    func_ptr_t pfnRxIrqCb;      ///< UART RX interrupt callback function pointer
    func_ptr_t pfnTxIdleCb;     ///< UART TX idle interrupt callback function pointer
}stc_uart_irq_cb_t;

/**
 *******************************************************************************
 ** \brief Uart status types
 **
 ** \note Uart status types get
 **
 ******************************************************************************/
typedef enum en_uart_status
{
    UartParityError     = 0u,   ///< Parity error
    UartFrameError      = 1u,   ///< Frame error
    UartOverrunError    = 2u,   ///< Overrun error
    UartRxFull          = 3u,   ///< RX completion
    UartTxEmpty         = 4u,   ///< TX buffer empty
    UartTxIdle          = 5u,   ///< TX idle
}en_uart_status_t;

/**
 ******************************************************************************
 ** \brief UART configuration structure
 ******************************************************************************/
 /**
 *******************************************************************************
 ** \brief Uart configuration structure
 **
 ** \note You can accorrding to configuration structure set uart
 **
 ******************************************************************************/
typedef struct stc_uart_config
{
    en_uart_mode_t      enMode;           ///< UART mode
    uint32_t            u32BaudRate;      ///< Baud rate (bps)
    en_uart_parity_t    enParity;         ///< Parity format
    en_uart_stop_bit_t  enStopBit;        ///< Stop bit
    en_uart_data_len_t  enDataLength;     ///< 5..9 Bit Character Length
    en_uart_data_dir_t  enBitDirection;   ///< UART data direction
    boolean_t bInvertData;                ///< FALSE: NRZ, TRUE : Inverted NRZ
    boolean_t bHwFlow;                    ///< FALSE: Not use Hardware Flow, TRUE : Use Hardware Flow
    boolean_t bUseExtClk;                 ///< FALSE: use internal clock, TRUE: use external clock which input via SCK pin
    boolean_t bCarryEnable;               ///< FALSE: not use carry function, TRUE: use carry function
    boolean_t bCarryPolarity;             ///< FALSE: normal polarity, TRUE: intervted polarity
    boolean_t bCompensation;              ///< FALSE: not compenstaion, TRUE: compensation
    uint16_t  u16CompenConf;              ///< Compensation configuration
    boolean_t bIrFunc;                    ///< FALSE: disable IrDA function, TRUE: enable IrDA function

    stc_uart_irq_en_t *pstcIrqEn;         ///< Pointer to UART interrupt enable structure, if set to NULL, no interrupt enabled.
    stc_uart_irq_cb_t *pstcIrqCb;         ///< Pointer to UART interrupt callback functions structurei, f set to NULL, no interrupt callback initialized.
    boolean_t         bTouchNvic;         ///< TRUE: enable NVIC, FALSE: don't enable NVIC
} stc_uart_config_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/

en_result_t Uart_EnableIrq(uint8_t u8Idx,en_uart_irq_sel_t enIrqSel);
en_result_t Uart_DisableIrq(uint8_t u8Idx,en_uart_irq_sel_t enIntSel);
en_result_t Uart_Init(uint8_t u8Idx, stc_uart_config_t* pstcConfig);
en_result_t Uart_DeInit(uint8_t u8Idx, boolean_t bTouchNvic);
en_result_t Uart_SetBaudRate(uint8_t u8Idx,uint32_t u32BaudRate);
en_result_t Uart_EnableFunc(uint8_t u8Idx, en_uart_func_t enFunc);
en_result_t Uart_DisableFunc(uint8_t u8Idx, en_uart_func_t enFunc);
boolean_t Uart_GetStatus(uint8_t u8Idx, en_uart_status_t enStatus);
en_result_t Uart_ClrStatus(uint8_t u8Idx, en_uart_status_t enStatus);
en_result_t Uart_SendData(uint8_t u8Idx, uint16_t Data);
en_result_t Uart_ReceiveData(uint8_t u8Idx,uint16_t *pu16Data);
//@} // UartGroup

#ifdef _cplusplus
}
#endif

#endif /* _UART_H_ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
