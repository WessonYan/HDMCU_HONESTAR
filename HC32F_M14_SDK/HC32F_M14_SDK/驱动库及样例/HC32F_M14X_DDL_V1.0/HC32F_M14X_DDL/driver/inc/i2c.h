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
/** \file i2c.h
 **
 ** A detailed description is available at
 ** @link I2cGroup i2c description @endlink
 **
 **   - 2017-08-10  1.0  WangM First version for Device Driver Library of i2c.
 **
 ******************************************************************************/

#ifndef __I2C_H_
#define __I2C_H_

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32f_m14x.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup I2cGroup Inter-Integrated Circuit (I2C)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief I2C Channel
 ******************************************************************************/
typedef enum en_i2c_channel
{
    I2CCH4 = 4u,
    I2CCH5 = 5u,
    I2CCH6 = 6u,
    I2CCHMAX = 6u,
}en_i2c_channel_t;

/**
 *******************************************************************************
 ** \brief I2C mode
 ******************************************************************************/
typedef enum en_i2c_mode
{
    I2cMaster = 0u,                     ///< I2C Master mode
    I2cSlave = 1u,                      ///< I2C Slave mode
}en_i2c_mode_t;

/**
 *******************************************************************************
 ** \brief I2C ACK types
 ******************************************************************************/
typedef enum en_i2c_ack
{
    I2cAck = 0u,                        ///< I2C normal ACK
    I2cNAck = 1u,                       ///< I2C NACK
}en_i2c_ack_t;

/**
 *******************************************************************************
 ** \brief I2C interrupt selection
 ******************************************************************************/
typedef enum en_i2c_irq_sel
{
    I2cTxIrq = 0u,                      ///< I2C TX interrupt
    I2cRxIrq = 1u,                      ///< I2C RX interrupt
    I2cTxRxIrq = 2u,                    ///< I2C TX and RX interrupt
    I2cStopDetectIrq = 3u,              ///< I2C stop condition interrupt
}en_i2c_irq_sel_t;

/**
 *******************************************************************************
 ** \brief I2C interrupt config type
 ******************************************************************************/
typedef enum en_i2c_irq_type
{
    I2cIrqTxEn = 0u,
    I2cIrqRxEn = 1u,
}en_i2c_irq_type_t;

/**
 *******************************************************************************
 ** \brief I2C interrupt enable structure
 ******************************************************************************/
typedef struct stc_i2c_irq_en
{
    boolean_t bTxIrq;                   ///< I2C TX interrupt
    boolean_t bRxIrq;                   ///< I2C RX interrupt
    boolean_t bTxRxIrq;                 ///< I2C TX and RX interrupt
    boolean_t bStopDetectIrq;           ///< I2C stop condition interrupt
}stc_i2c_irq_en_t;

/**
 *******************************************************************************
 ** \brief I2C interrupt callback function
 ******************************************************************************/
typedef struct stc_i2c_irq_cb
{
    func_ptr_t pfnTxIrqCb;              ///< I2C TX interrupt callback function pointer
    func_ptr_t pfnRxIrqCb;              ///< I2C RX interrupt callback function pointer
    func_ptr_t pfnTxRxIrqCb;            ///< I2C TX and RX completion interrupt callback function
    func_ptr_t pfnStopDetectIrqCb;      ///< I2C stop condition interrupt
}stc_i2c_irq_cb_t;

/**
 *******************************************************************************
 ** \brief I2C status types
 ******************************************************************************/
typedef enum en_i2c_status
{
    I2cOverrunError = 0u,               ///< I2C overrun error
    I2cRxFull = 1u,                     ///< I2C RX buffer full
    I2cTxEmpty = 2u,                    ///< I2C TX buffer empty
    I2cFirstByteDetect = 3u,            ///< I2C First byte detection
    I2cReservedByteDetect = 4u,         ///< I2C reserved byte detection
    I2cStopDetect = 5u,                 ///< I2C stop condition detection
    I2cBusStatus = 6u,                  ///< I2C Bus status
    I2cBusErr = 7u,                     ///< I2C Bus error
    I2cRxTxIrq = 8u,                    ///< I2C transimission or reception interrupt flag
    I2cDevAddrMatch = 9u,               ///< I2C received slave address matchs with pre-set address
}en_i2c_status_t;

/**
 *******************************************************************************
 ** \brief I2C data direction(used in slave mode)
 ******************************************************************************/
typedef enum en_i2c_data_dir
{
    i2c_master_tx_slave_rx = 0u,        ///< Data from master to slave
    i2c_slave_tx_master_rx = 1u,        ///< Data from slave to master
}en_i2c_data_dir_t;

/**
 *******************************************************************************
 ** \brief I2C configuration structure
 ******************************************************************************/
typedef struct stc_i2c_config
{
    en_i2c_mode_t enMsMode;             ///< I2C master mode or slave mode
    uint32_t u32BaudRate;               ///< Baud rate (bps)
    uint8_t u8SlaveAddr;                ///< Slave address (This is effective on slave mode.)
    uint8_t u8SlaveMaskAddr;            ///< Slave Mask address (This is effective on slave mode.)
    boolean_t bWaitSelection;           ///< FALSE: generate interrupt after ACK, TRUE: generate interrupt before ACK
    stc_i2c_irq_en_t* pstcIrqEn;        ///< Pointer to I2C interrupt enable structure, if set to NULL, no interrupt enabled.
    stc_i2c_irq_cb_t* pstcIrqCb;        ///< Pointer to I2C interrupt callback functions structure, if set to NULL, no interrupt callback initialized.
    boolean_t bTouchNvic;               ///< TRUE: enable NVIC, FALSE: don't enable NVIC
} stc_i2c_config_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define stc_mscn_i2c_t  M0P_MSC_I2C_TypeDef

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
extern en_result_t I2c_EnableIrq(uint8_t u8Channel, en_i2c_irq_sel_t enIrqSel);
extern en_result_t I2c_DisableIrq(uint8_t u8Channel, en_i2c_irq_sel_t enIrqSel);

extern en_result_t I2c_Init(uint8_t u8Channel, stc_i2c_config_t* pstcConfig);
extern en_result_t I2c_DeInit(uint8_t u8Channel);

extern en_result_t I2c_SetBaudRate(uint8_t u8Channel, uint32_t u32BaudRate);

extern en_result_t I2c_GenerateStart(uint8_t u8Channel);
extern en_result_t I2c_GenerateRestart(uint8_t u8Channel);
extern en_result_t I2c_GenerateStop(uint8_t u8Channel);

extern en_result_t I2c_SendData(uint8_t u8Channel, uint8_t u8Data);
extern en_result_t I2c_ReceiveData(uint8_t u8Channel, uint8_t* pu8data);

extern en_result_t I2c_ConfigAck(uint8_t u8Channel, en_i2c_ack_t enAck);
extern en_i2c_ack_t I2c_GetAck(uint8_t u8Channel);

extern boolean_t I2c_GetStatus(uint8_t u8Channel, en_i2c_status_t enStatus);
extern en_result_t I2c_ClrStatus(uint8_t u8Channel, en_i2c_status_t enStatus);
extern en_i2c_data_dir_t I2c_GetDataDir(uint8_t u8Channel);

#ifdef __cplusplus
}
#endif

#endif /*__I2C_H_*/
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
