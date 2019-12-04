/******************************************************************************
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
/** \file spi.h
 **
 ** A detailed description is available at
 ** @link SpiGroup Spi description @endlink
 **
 **   - 2017-8-15  1.0  Jason First version for Device Driver Library of spi.
 **
 ******************************************************************************/

#ifndef __SPI_H__
#define __SPI_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32f_m14x.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup SpiGroup Serial Peripheral Interface(SPI)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Spi channel.
 ******************************************************************************/
typedef enum en_spi_channel
{
    SpiCh4 = 4u,                        ///< Spi channel 4.
    SpiCh5 = 5u,                        ///< Spi channel 5.
    SpiCh6 = 6u,                        ///< Spi channel 6.
} en_spi_channel_t;

/**
 *******************************************************************************
 ** \brief Spi signal mark level enumeration
 ******************************************************************************/
typedef enum en_spi_sck_mark_level
{
    SpiSckMarkLevel_High = 0u,          ///< Spi mark level is high.
    SpiSckMarkLevel_Low = 1u,           ///< Spi mark level is low.
}en_spi_sck_mark_level_t;

/**
 *******************************************************************************
 ** \brief Spi data length enumeration
 ******************************************************************************/
typedef enum en_spi_datalen
{
    SpiDataLen8 = 0u,                   ///< 8-bit length.
    SpiDataLen5,                        ///< 5-bit length.
    SpiDataLen6,                        ///< 6-bit length.
    SpiDataLen7,                        ///< 7-bit length.
    SpiDataLen9,                        ///< 9-bit length.
    SpiDataLen10,                       ///< 10-bit length.
    SpiDataLen11,                       ///< 11-bit length.
    SpiDataLen12,                       ///< 12-bit length.
    SpiDataLen13,                       ///< 13-bit length.
    SpiDataLen14,                       ///< 14-bit length.
    SpiDataLen15,                       ///< 15-bit length.
    SpiDataLen16,                       ///< 16-bit length.
    SpiDataLenMax,
}en_spi_datalen_t;

/**
 *******************************************************************************
 ** \brief Spi serial timer clock division
 ******************************************************************************/
typedef enum en_spi_clk_div
{
    SpiClkDiv1   = 0u,                  ///< Serial Timer clock = PCLK.
    SpiClkDiv2   = 1u,                  ///< Serial Timer clock = PCLK/2.
    SpiClkDiv4   = 2u,                  ///< Serial Timer clock = PCLK/4.
    SpiClkDiv8   = 3u,                  ///< Serial Timer clock = PCLK/8.
    SpiClkDiv16  = 4u,                  ///< Serial Timer clock = PCLK/16.
    SpiClkDiv32  = 5u,                  ///< Serial Timer clock = PCLK/32.
    SpiClkDiv64  = 6u,                  ///< Serial Timer clock = PCLK/64.
    SpiClkDiv128 = 7u,                  ///< Serial Timer clock = PCLK/128.
    SpiClkDiv256 = 8u,                  ///< Serial Timer clock = PCLK/256.
    SpiClkDivMax = 9u,                  ///< Invalid.
}en_spi_clk_div_t;

/**
 *******************************************************************************
 ** \brief Spi chip selection pin
 ******************************************************************************/
typedef enum en_spi_cspin_sel
{
    SpiCsPin0 = 0u,                     ///< Use SCS0 as chip selection pin
    SpiCsPin1 = 1u,                     ///< Use SCS1 as chip selection pin
    SpiCsPin2 = 2u,                     ///< Use SCS2 as chip selection pin
    SpiCsPin3 = 3u,                     ///< Use SCS3 as chip selection pin
    SpiCsPinMax = 0xFFu,
}en_spi_cspin_sel_t;

/**
 *******************************************************************************
 ** \brief Spi function
 ******************************************************************************/
typedef enum en_spi_func_sel
{
    SpiFuncTx = 0u,                     ///< Spi TX
    SpiFuncRx,                          ///< Spi RX
    SPiFuncSyncTx,                      ///< Spi sync transmit
    SpiFuncCsFormat,                    ///< Spi cs format
    SpiFuncSerialTimer,                 ///< Spi Serial Timer
}en_spi_func_sel_t;

/**
 *******************************************************************************
 ** \brief Spi interrupt selection
 ******************************************************************************/
typedef enum en_spi_irq_sel
{
    SpiIrqTx           = 0u,            ///< Spi TX interrupt
    SpiIrqRx           = 1u,            ///< Spi RX interrupt
    SpiIrqTxIdle       = 2u,            ///< Spi TX idle interrupt
    SpiIrqCsErr        = 4u,            ///< Spi cs interrupt
    SpiIrqSerialTimer  = 5u,            ///< Spi serial timer interrupt
}en_spi_irq_sel_t;

/**
 *******************************************************************************
 ** \brief Spi status types
 ******************************************************************************/
typedef enum en_spi_status
{
    SpiOverrunError = 0u,               ///< Spi overrun error
    SpiRxFull,                          ///< Spi RX completion
    SpiTxEmpty,                         ///< Spi TX buffer empty
    SpiTxIdle,                          ///< Spi TX bus idle
    SpiCsErrIntFlag,                    ///< Spi cs error interrupt flag
    SpiSerialTimerIntFlag,              ///< Spi serial timer interrupt flag
}en_spi_status_t;

/**
 *******************************************************************************
 ** \brief Spi interrupt enable structure
 ******************************************************************************/
typedef struct stc_spi_irq_sel
{
    boolean_t bTxIrq;                   ///< Spi TX interrupt
    boolean_t bRxIrq;                   ///< Spi RX interrupt
    boolean_t bCsErrIrq;                ///< Spi chip selection interrupt
    boolean_t bTxIdleIrq;               ///< Spi TX idle interrupt
    boolean_t bSerialTimerIrq;          ///< Spi serial timer interrupt
}stc_spi_irq_sel_t;

/**
 *******************************************************************************
 ** \brief Spi interrupt callback function
 ******************************************************************************/
typedef struct stc_spi_irq_cb
{
    func_ptr_t pfnTxIrqCb;              ///< Spi TX interrupt callback function
    func_ptr_t pfnRxIrqCb;              ///< Spi RX interrupt callback function
    func_ptr_t pfnTxIdleCb;             ///< Spi TX idle interrupt callback function
    func_ptr_t pfnRxErrIrqCb;           ///< Spi RX error interrupt callback function
    func_ptr_t pfnCsErrIrqCb;           ///< Spi chip selection interrupt callback function
    func_ptr_t pfnSerialTimerIrqCb;     ///< Spi serial timer interrupt callback function
}stc_spi_irq_cb_t;

/**
 *******************************************************************************
 ** \brief Spi chip selection configuration
 ******************************************************************************/
typedef struct stc_spi_cspin_config
{
    boolean_t           bEn;            ///< pin enable
    boolean_t           bSckInv;        ///< sck invert
    boolean_t           bCsInActiveHigh;///< cs inactive high
    boolean_t           bSPIMode;       ///< spi mode
    boolean_t           bMSB;           ///< big/little endian
    en_spi_datalen_t    enDataLen;      ///< 5 ~ 16 bit Length
}stc_spi_cspin_config_t;

/**
 *******************************************************************************
 ** \brief SPI cs configure data
 ******************************************************************************/
typedef struct stc_spi_cs_config
{
    en_spi_cspin_sel_t      enCsStartPin;       ///< CS start pin
    en_spi_cspin_sel_t      enCsEndPin;         ///< CS end pin
    boolean_t               bActiveHold;        ///< active hold
    en_spi_clk_div_t        enCsClkDiv;         ///< CS clock division
    boolean_t               bCsIdleLow;         ///< CS pin idle level
    boolean_t               bCsOutputEn;        ///< CS output enable
    stc_spi_cspin_config_t  stcCsPincfg[4];     ///< cs pin configuration
    uint8_t                 u8CsDelayTime;      ///< cs to sck start delay time
    uint8_t                 u8CsHoldTime;       ///< sck end to cs delay time
    uint16_t                u16CsDeselectTime;  ///< cs min idle time
}stc_spi_cs_config_t;

/**
 *******************************************************************************
 ** \brief SPI configuration structure
 ******************************************************************************/
typedef struct stc_spi_config
{
    boolean_t               bSlaveMode;         ///< Master or slave mode
    boolean_t               bSPIMode;           ///< spi or normal mode
    boolean_t               bOutputSCK;         ///< SCK output
    boolean_t               bOutputData;        ///< SOT output
    boolean_t               bSOTHigh;           ///< SOT pin state
    boolean_t               bMSB;               ///< MSB or LSB
    uint8_t                 u8DataOuputDly;     ///< max to 3 bits time delay.
    uint32_t                u32BaudRate;        ///< Baud rate (bps)
    en_spi_datalen_t        enDataLen;          ///< 5->16 bit Length
    en_spi_sck_mark_level_t enSckMarkLevel;     ///< SCK mark level
    stc_spi_irq_sel_t       *pstcIrqEn;         ///< Pointer to SPI interrupt enable structure, if set to NULL, no interrupt enabled.
    stc_spi_irq_cb_t        *pstcIrqCb;         ///< Pointer to SPI interrupt callback functions structurei, f set to NULL, no interrupt callback initialized.
} stc_spi_config_t;


/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/* Irq enable/disable. */
en_result_t Spi_EnableIrq(uint8_t u8Channel, en_spi_irq_sel_t enIrqSel);
en_result_t Spi_DisableIrq(uint8_t u8Channel, en_spi_irq_sel_t enIrqSel);

/* Spi get status. */
boolean_t Spi_GetStatus(uint8_t u8Channel, en_spi_status_t enStatus);
en_result_t Spi_ClrStatus(uint8_t u8Channel,en_spi_status_t enStatus);

/* Spi Init/Deinit. */
en_result_t Spi_Init(uint8_t u8Channel,stc_spi_config_t* pstcConfig);
en_result_t Spi_DeInit(uint8_t u8Channel);
en_result_t Spi_CSCconfig(uint8_t u8Channel,stc_spi_cs_config_t *pstcConfig);
en_result_t Spi_SerialTimerConfig(uint8_t u8Channel,
                               en_spi_clk_div_t enDiv,
                               uint16_t u16Val);

/* Spi function. */
en_result_t Spi_EnableFunc(uint8_t u8Channel, en_spi_func_sel_t enFunc);
en_result_t Spi_DisableFunc(uint8_t u8Channel, en_spi_func_sel_t enFunc);

/* Spi data setting. */
en_result_t Spi_SetBaudRate(uint8_t u8Channel,uint32_t u32BaudRate);
en_result_t Spi_SetTxBytes(uint8_t u8Channel,uint8_t u8Cspin, uint8_t u8Val);

/* Spi TX/RX data function. */
en_result_t Spi_SendData(uint8_t u8Channel, uint16_t u16Data);
en_result_t Spi_ReceiveData(uint8_t u8Channel, boolean_t bSendSck, uint16_t* pu16Data);

//@} // SpiGroup

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
