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
 ** \brief The example demonstrate i2c slave polling transmit
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

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define SAMPLE_I2C_MASTER_TX_BUFFSIZE   sizeof(au8I2cSlaveTxBuf)/sizeof(char)
#define SAMPLE_I2C_MASTER_RX_BUFFSIZE   sizeof(au8I2cSlaveRxBuf)/sizeof(char)

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
static uint8_t au8I2cSlaveTxBuf[50];
static uint8_t au8I2cSlaveRxBuf[50];
static uint8_t u8I2cCh = I2CCH5;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

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
    stc_i2c_config_t stcI2c0Config;

    Gpio_SetFunc_SOT5_1(0u);
    Gpio_SetFunc_SCK5_0(0u);

    DDL_ZERO_STRUCT(stcI2c0Config);

    stcI2c0Config.enMsMode = I2cSlave;
    stcI2c0Config.u32BaudRate = 100000u;
    stcI2c0Config.u8SlaveAddr = I2C_DEV_ADDR;
    stcI2c0Config.u8SlaveMaskAddr = 0x7Fu;
    stcI2c0Config.bWaitSelection = FALSE;

    I2c_Init(u8I2cCh, &stcI2c0Config);
}

/**
 *******************************************************************************
 ** \brief Transmit the dedicated size data to device
 **
 ** \param [in]  pu8TxData Transmit data pointer
 ** \param [in]  pu32Size the number of transmitted data
 **
 ** \retval None
 **
 ******************************************************************************/
void I2c_SlaveWriteData(uint8_t *pu8TxData, uint32_t *pu32Size)
{
    uint8_t i=0;
    while(1)
    {
        /* Transmit the data */
        I2c_SendData(u8I2cCh, pu8TxData[i++]);
        I2c_ClrStatus(u8I2cCh, I2cRxTxIrq);

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
            ; /* continue to send */
        }
        else
        {
            /* Master tells that it is last data he wants to read, check the stop signal then */
            I2c_ClrStatus(u8I2cCh, I2cRxTxIrq); /* Release bus */
            break;
        }
    }

    /* Check the stop condition */
    while(1)
    {
        if(I2c_GetStatus(u8I2cCh, I2cStopDetect) == TRUE)
        {
            I2c_ClrStatus(u8I2cCh, I2cStopDetect);
            I2c_ClrStatus(u8I2cCh, I2cRxTxIrq);
            break;
        }
    }

    *pu32Size = i;
}

/**
 *******************************************************************************
 ** \brief Receive the dedicated size data to device
 **
 ** \param [out] pu8RxData The receive buffer pointer
 ** \param [in]  pu32Sizethe number of receiving data
 **
 ** \retval None
 ******************************************************************************/
void I2c_SlaveReadData(uint8_t *pu8RxData, uint32_t *pu32Size)
{
    uint8_t i = 0;
    I2c_ClrStatus(u8I2cCh, I2cRxTxIrq);
    while(1)
    {
        /* Stop condition and NACK */
        if(I2c_GetStatus(u8I2cCh, I2cStopDetect) == TRUE)
        {
            I2c_ClrStatus(u8I2cCh, I2cStopDetect);
            I2c_ClrStatus(u8I2cCh, I2cRxTxIrq);

            break;
        }else
        {

        }

        /* Bus Error */
        if(I2c_GetStatus(u8I2cCh, I2cBusErr) == TRUE)
        {
            I2c_ClrStatus(u8I2cCh, I2cRxTxIrq);
            break;
        }
        else
        {
        }
        
        if(I2c_GetStatus(u8I2cCh, I2cRxFull) == TRUE)
        {
            I2c_ReceiveData(u8I2cCh, (uint8_t*)&pu8RxData[i++]);
            I2c_ConfigAck(u8I2cCh, I2cAck);
            I2c_ClrStatus(u8I2cCh, I2cRxTxIrq);
        }
        else
        {
        }
    }

    *pu32Size = i;
}

/**
 *******************************************************************************
 ** \brief  Main function of project
 **
 ** \param [in]  None
 **
 ** \return int32_t Return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    uint32_t WriteLength, ReadLength;

    stc_clk_config_t stcCfg;
    en_result_t enRet = Ok;

    Gpio_InitIO(0u, 0u, GpioDirIn, FALSE, FALSE);
    while(TRUE == Gpio_GetIO(0u, 0u)){}

    /* Set System clock */
    DDL_ZERO_STRUCT(stcCfg);
    stcCfg.u8PllMulti = 4u;
    stcCfg.enPllSrc = MAIN2PLL;

    stcCfg.enClkSrc = ClkHSXT;
    stcCfg.enBaseClkDiv = ClkBaseDiv1;
    stcCfg.enAPB0Div = ClkApb0Div1;
    stcCfg.enAPB1Div = ClkApb1Div1;
    stcCfg.bAPB1En = TRUE;
    stcCfg.enClkWaitMain = ClkCswtMain524ms;
    stcCfg.enClkWaitSub = ClkCswtSub512ms;
    stcCfg.enClkWaitPll = ClkPswtPll680us;

    stcCfg.bHicrEn = TRUE;
    stcCfg.bMainEn = TRUE;
    stcCfg.bSubEn  = FALSE;
    stcCfg.bPllEn  = TRUE;

    enRet = Clk_Init(&stcCfg);
    if(Ok != enRet)
    {
        while(1){}
    }

    /* DeInit I2C Bus */
    I2c_DeInit(u8I2cCh);
    
    /* I2C slave init */
    InitI2cSlave();

    while(1)
    {
        /* polling flag of first data */
        if(TRUE == I2c_GetStatus(u8I2cCh, I2cDevAddrMatch))
        {
            if(i2c_slave_tx_master_rx == I2c_GetDataDir(u8I2cCh))/* Tx */
            {
                delay1ms(1);
                I2c_SlaveWriteData(au8I2cSlaveTxBuf, &WriteLength);
            }
            else/* Rx */
            {
                I2c_SlaveReadData(au8I2cSlaveRxBuf,&ReadLength);
                memset(au8I2cSlaveTxBuf, 0, sizeof(au8I2cSlaveTxBuf));
                memcpy(au8I2cSlaveTxBuf, au8I2cSlaveRxBuf, ReadLength);
            }
        }
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
