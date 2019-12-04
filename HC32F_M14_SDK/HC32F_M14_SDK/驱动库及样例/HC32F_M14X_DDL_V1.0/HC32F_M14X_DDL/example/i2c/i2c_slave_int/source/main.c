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
 ** \brief The example demonstrate i2c slave interrupt transmit
 **
 **   - 2017-08-10  1.0  WangM First version for sample of I2C module.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "i2c.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief I2C read and write mode
 ******************************************************************************/
typedef enum en_i2c_rw_mode
{
    I2cTxMode = 0,                      ///< I2C TX only
    I2cRxMode,                          ///< I2C RX only
    I2cTxRxMode,                        ///< I2C TX first and RX at the following

}en_i2c_rw_mode_t;

/**
 *******************************************************************************
 ** \brief I2C transfer sequence
 ******************************************************************************/
typedef enum en_i2c_seq
{
    I2cSeqStart = 0u,                   ///< I2C Start process
    I2cSeqWrite,                        ///< I2C Write process
    I2cSeqRestart,                      ///< I2C Restart process
    I2cSeqRead,                         ///< I2C Read process
    I2cSeqStop,                         ///< I2C Stop process

}en_i2c_seq_t;

/**
 *******************************************************************************
 ** \brief I2C transfer information structure
 ******************************************************************************/
typedef struct stc_i2c_info
{
    boolean_t bI2cErrorFlag;            ///< Error flag
    uint8_t*  pu8TxBuf;                 ///< Pointer to TX buffer
    uint32_t  u32TxSize;                ///< TX buffer size
    uint8_t*  pu8RxBuf;                 ///< Pointer to RX buffer
    uint32_t  u32RxSize;                ///< RX buffer size
    uint32_t  u32Count;                 ///< Transfer count
    en_i2c_seq_t     enSequence;        ///< Transfer Sequence
    uint8_t          u8DevAddr;         ///< Device address
    en_i2c_rw_mode_t enRwMode;          ///< R/W Flag
    uint32_t  u32TxRxGap;               ///< The period from end of TX and begin of restart
}stc_i2c_info_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define SAMPLE_I2C_MASTER_TX_BUFFSIZE   sizeof(au8I2cMasterTxBuf)/sizeof(char)
#define SAMPLE_I2C_MASTER_RX_BUFFSIZE   SAMPLE_I2C_MASTER_TX_BUFFSIZE

#define I2C_DEV_ADDR              (0x3Au)
#define I2C_DEV_ADDR_W            ((I2C_DEV_ADDR<<1) | 0u)
#define I2C_DEV_ADDR_R            ((I2C_DEV_ADDR<<1) | 1u)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t au8I2cMasterTxBuf[10] = "0123456789";
static uint8_t au8I2cMasterRxBuf[SAMPLE_I2C_MASTER_RX_BUFFSIZE] = {0};
static uint8_t u8I2cCh = I2CCH5;
static stc_i2c_info_t stcI2cInfo;
static stc_i2c_irq_cb_t stcI2cIntCb;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief I2C TX callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void I2cTxCallback(void)
{
    if(stcI2cInfo.enSequence == I2cSeqStart)
    {
        if(TRUE == I2c_GetStatus(u8I2cCh, I2cDevAddrMatch))
        {
            stcI2cInfo.enSequence = I2cSeqWrite;
        }
    }
    else if(stcI2cInfo.enSequence == I2cSeqRestart)
    {
    }
    else if(stcI2cInfo.enSequence == I2cSeqWrite)
    {
        /* Wait for end of transmission */
        while(1)
        {
            if(I2c_GetStatus(u8I2cCh, I2cRxTxIrq) == TRUE)
            {
                break;
            }
        }
        while(1)
        {
            if(I2c_GetStatus(u8I2cCh, I2cTxEmpty) == TRUE)
            {
                break;
            }
        }

        if(I2c_GetAck(u8I2cCh) == I2cAck)
        {
            /* continue to send */
            I2c_SendData(u8I2cCh, *stcI2cInfo.pu8TxBuf++);
            I2c_ClrStatus(u8I2cCh, I2cRxTxIrq);

            stcI2cInfo.u32Count++;
        }
        else
        {
            /* should be enable I2cRxIrq for stop irq*/
            I2c_DisableIrq(u8I2cCh, I2cTxIrq);
            I2c_EnableIrq(u8I2cCh, I2cRxIrq);
            /* Master tells that it is last data he wants to read, check the stop signal then */
            I2c_ClrStatus(u8I2cCh, I2cRxTxIrq); /* Release bus */
        }
    }
    else
    {
    }
}

/**
 *******************************************************************************
 ** \brief I2C RX callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void I2cRxCallback(void)
{
    uint32_t u32Cnt = SystemCoreClock/4000000u;

    if(stcI2cInfo.enSequence == I2cSeqRead)
    {
        I2c_ReceiveData(u8I2cCh, (uint8_t*)stcI2cInfo.pu8RxBuf++);

        if(stcI2cInfo.u32Count == stcI2cInfo.u32RxSize - 1u)
        {
           I2c_ConfigAck(u8I2cCh, I2cNAck); /* Last byte send a NACK */
        }

        /* wait some cycles for slave to prepare the data */
        while(u32Cnt--){}

        I2c_ClrStatus(u8I2cCh, I2cRxTxIrq);

        stcI2cInfo.u32Count++;
    }
}

/**
 *******************************************************************************
 ** \brief I2C interrupt callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void I2cIntCallback(void)
{
    uint32_t u32Cnt = SystemCoreClock/4000000u;

    if(TRUE == I2c_GetStatus(u8I2cCh, I2cRxTxIrq))
    {
        /* Check error status of I2C */
        if((TRUE == I2c_GetStatus(u8I2cCh, I2cBusErr)) || (TRUE == I2c_GetStatus(u8I2cCh, I2cOverrunError)))
        {
            stcI2cInfo.bI2cErrorFlag = 1u;
            return;
        }
        /* In read or write sequence, clear INT at TX/RX callback */
        if((stcI2cInfo.enSequence != I2cSeqStart) && (stcI2cInfo.enSequence != I2cSeqRestart))
        {
            return;
        }

        if(stcI2cInfo.enSequence == I2cSeqStart)
        {
            if(stcI2cInfo.enRwMode == I2cRxMode)
            {
                /* wait some cycles for slave to prepare the data */
                while(u32Cnt--){}

                /* Receive first data here and receive following data at RX callback */
                I2c_ClrStatus(u8I2cCh, I2cRxTxIrq);

                stcI2cInfo.u32Count++;

                stcI2cInfo.enSequence = I2cSeqRead; /* Start read data */
            }
            else
            {
                stcI2cInfo.enSequence = I2cSeqWrite; /* Start write data */
            }
        }

        if(stcI2cInfo.enSequence == I2cSeqRestart)
        {
            if(stcI2cInfo.u32Count == stcI2cInfo.u32RxSize-1)
            {
                I2c_ConfigAck(u8I2cCh, I2cNAck); /* Last byte send a NACK */
            }
            /* Receive first data here and receive following data at RX callback */
            I2c_ClrStatus(u8I2cCh, I2cRxTxIrq);

            stcI2cInfo.u32Count++;

            stcI2cInfo.enSequence = I2cSeqRead; /* Start read data */
        }
    }
}

/**
 *******************************************************************************
 ** \brief I2C stop detection callback function
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void I2cStopDetectCallback(void)
{
    if(TRUE == I2c_GetStatus(u8I2cCh, I2cStopDetect))
    {
        I2c_ClrStatus(u8I2cCh, I2cStopDetect);
        I2c_ClrStatus(u8I2cCh, I2cRxTxIrq);

        stcI2cInfo.enSequence = I2cSeqStop;

        I2c_DisableIrq(u8I2cCh, I2cTxIrq);
        I2c_DisableIrq(u8I2cCh, I2cRxIrq);
        I2c_DisableIrq(u8I2cCh, I2cTxRxIrq);
        I2c_DisableIrq(u8I2cCh, I2cStopDetectIrq);
    }
}


/**
 *******************************************************************************
 ** \brief  Initialize I2C salve
 **
 ** \param  none
 **
 ** \return none
 ******************************************************************************/
static void InitI2cSlave(void)
{
    static stc_i2c_config_t stcI2c0Config;

    Gpio_SetFunc_SOT5_1(0u);
    Gpio_SetFunc_SCK5_0(0u);

    DDL_ZERO_STRUCT(stcI2c0Config);
    DDL_ZERO_STRUCT(stcI2cIntCb);

    /* Init interrupt callback function */
    stcI2cIntCb.pfnTxIrqCb = I2cTxCallback;
    stcI2cIntCb.pfnRxIrqCb = I2cRxCallback;
    stcI2cIntCb.pfnStopDetectIrqCb = I2cStopDetectCallback;
    stcI2cIntCb.pfnTxRxIrqCb = I2cIntCallback;

    /* Init configuration structure */
    stcI2c0Config.enMsMode = I2cSlave;
    stcI2c0Config.u32BaudRate = 100000u;
    stcI2c0Config.u8SlaveAddr = I2C_DEV_ADDR;
    stcI2c0Config.u8SlaveMaskAddr = 0x7Fu;
    stcI2c0Config.bWaitSelection = FALSE;
    stcI2c0Config.pstcIrqEn = NULL;
    stcI2c0Config.pstcIrqCb = &stcI2cIntCb;
    stcI2c0Config.bTouchNvic = TRUE;
    I2c_Init(u8I2cCh, &stcI2c0Config);

}

/**
 *******************************************************************************
 ** \brief Write I2C data
 **
 ** \param [in]  u8SlaveAddr  7-bit slave address
 ** \param [in]  pu8Data      Pointer to data buffer
 ** \param [in]  u32Size      Write buffer size
 **
 ** \retval Ok      Data are transferred normally
 ** \retval Error   Data transferred are ended with error
 ******************************************************************************/
static en_result_t I2cTxData(uint8_t u8SlaveAddr, uint8_t *pu8Data, uint32_t u32Size)
{
    stcI2cInfo.enSequence = I2cSeqStart;
    stcI2cInfo.enRwMode = I2cTxMode;
    stcI2cInfo.u8DevAddr = u8SlaveAddr;
    stcI2cInfo.pu8TxBuf = pu8Data;
    stcI2cInfo.u32TxSize = u32Size;
    stcI2cInfo.u32Count = 0u;
    stcI2cInfo.bI2cErrorFlag = 0u;

    I2c_EnableIrq(u8I2cCh, I2cStopDetectIrq);
    I2c_EnableIrq(u8I2cCh, I2cTxRxIrq);
    I2c_EnableIrq(u8I2cCh, I2cTxIrq);

    while(stcI2cInfo.enSequence < I2cSeqStop)
    {
        if(stcI2cInfo.bI2cErrorFlag == 1u)
        {
            return Error;
        }
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Read I2C data
 **
 ** \param [in]  u8SlaveAddr  7-bit device address
 ** \param [out] pu8RxData    Pointer to data buffer
 ** \param [in]  u32RxSize    Read buffer size
 **
 ** \retval Ok      Data are read normally
 ** \retval Error   Data read are ended with error
 ******************************************************************************/
static en_result_t I2cRxData(uint8_t u8SlaveAddr, uint8_t* pu8RxData, uint32_t u32RxSize)
{
    stcI2cInfo.enSequence = I2cSeqStart;
    stcI2cInfo.enRwMode = I2cRxMode;
    stcI2cInfo.u8DevAddr = u8SlaveAddr;
    stcI2cInfo.pu8RxBuf = pu8RxData;
    stcI2cInfo.u32RxSize = u32RxSize;
    stcI2cInfo.u32Count = 0u;
    stcI2cInfo.bI2cErrorFlag = 0u;

    I2c_EnableIrq(u8I2cCh, I2cRxIrq);
    I2c_EnableIrq(u8I2cCh, I2cTxRxIrq);
    I2c_EnableIrq(u8I2cCh, I2cStopDetectIrq);

    while(stcI2cInfo.enSequence < I2cSeqStop)
    {
        if(stcI2cInfo.bI2cErrorFlag == 1u)
        {
            return Error;
        }
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief  Main function of project
 **
 ** \param [in]  None
 **
 ** \return uint32_t Return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    /* Clock init */
    Clk_SwitchTo(ClkHICR);

    I2c_DeInit(u8I2cCh);

    while(1)
    {
        /* I2C master init */
        InitI2cSlave();

        /* I2C master receive data */
        I2cRxData(I2C_DEV_ADDR, au8I2cMasterRxBuf, sizeof(au8I2cMasterRxBuf));

        /* I2C master send data */
        I2cTxData(I2C_DEV_ADDR, au8I2cMasterTxBuf, sizeof(au8I2cMasterTxBuf));

        /* DeInit I2C Bus */
        I2c_DeInit(u8I2cCh);
    }

}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
