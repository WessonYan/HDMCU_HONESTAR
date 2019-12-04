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
 ** \brief The example demonstrate i2c master polling transmit
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
#define SAMPLE_I2C_MASTER_TX_BUFFSIZE   sizeof(au8I2cMasterTxBuf)/sizeof(char)
#define SAMPLE_I2C_MASTER_RX_BUFFSIZE   SAMPLE_I2C_MASTER_TX_BUFFSIZE

#define I2C_RET_OK                0u
#define I2C_RET_ERROR             1u

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

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief Initialize I2C Master
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
static void InitI2cMaster(void)
{
    stc_i2c_config_t stcI2c0Config;

    Gpio_SetFunc_SOT5_1(0u);
    Gpio_SetFunc_SCK5_0(0u);

    DDL_ZERO_STRUCT(stcI2c0Config);

    stcI2c0Config.enMsMode = I2cMaster;
    stcI2c0Config.u32BaudRate = 100000u;
    stcI2c0Config.bWaitSelection = FALSE;

    I2c_Init(u8I2cCh, &stcI2c0Config);
}

/**
 *******************************************************************************
 ** \brief Config the start condition
 **
 ** \param [in]  u8Addr       device address and read write information
 **
 ** \retval I2C_RET_ERROR     Generate Start signal failed
 ** \retval I2C_RET_OK        Generate Start signal success
 **
 ******************************************************************************/
static uint8_t I2c_Start(uint8_t u8Addr)
{
    /* Prepare I2C device address */
    I2c_SendData(u8I2cCh, u8Addr);
    /* Generate I2C start signal */
    if(Ok != I2c_GenerateStart(u8I2cCh))
    {
        return I2C_RET_ERROR; /* Timeout or other error */
    }

    while(1)
    {
        if(TRUE == I2c_GetStatus(u8I2cCh, I2cRxTxIrq))
        {
            break;
        }
    }

    if(I2cNAck == I2c_GetAck(u8I2cCh))
    {
        return I2C_RET_ERROR;   /* NACK */
    }

    if(TRUE == I2c_GetStatus(u8I2cCh, I2cBusErr))
    {
        return I2C_RET_ERROR; /* Bus error occurs? */
    }

    if(TRUE == I2c_GetStatus(u8I2cCh, I2cOverrunError))
    {
        return I2C_RET_ERROR; /* Overrun error occurs? */
    }

    return I2C_RET_OK;
}

/**
 *******************************************************************************
 ** \brief Transmit the dedicated size data to device
 **
 ** \param [in]  pTxData Transmit data pointer
 ** \param [in]  u8Size the number of transmitted data
 **
 ** \retval I2C_RET_ERROR     Data send failed
 ** \retval I2C_RET_OK        Data send success
 **
 ******************************************************************************/
static uint8_t I2c_Write(uint8_t *pTxData, uint8_t u8Size)
{
    uint8_t i;

    for(i=0;i<u8Size;i++)
    {
        /* Transmit the data */
        I2c_SendData(u8I2cCh, pTxData[i]);
        I2c_ClrStatus(u8I2cCh, I2cRxTxIrq);
        /* Wait for end of transmission */
        while(1)
        {
            if(TRUE == I2c_GetStatus(u8I2cCh, I2cRxTxIrq))
            {
                break;
            }
        }

        while(1)
        {
            if(TRUE == I2c_GetStatus(u8I2cCh, I2cTxEmpty))
            {
                break;
            }
        }

        if(I2cNAck == I2c_GetAck(u8I2cCh))
        {
            return I2C_RET_ERROR;   /* NACK */
        }

        if(TRUE == I2c_GetStatus(u8I2cCh, I2cBusErr))
        {
            return I2C_RET_ERROR; /* Bus error occurs? */
        }

        if(TRUE == I2c_GetStatus(u8I2cCh, I2cOverrunError))
        {
            return I2C_RET_ERROR; /* Overrun error occurs? */
        }
    }

    return I2C_RET_OK;
}

/**
 *******************************************************************************
 ** \brief Receive the dedicated size data to device
 **
 ** \param [out] pRxData The receive buffer pointer
 ** \param [in]  u8Size The number of receiving data
 **
 ** \retval I2C_RET_ERROR     Data read failed
 ** \retval I2C_RET_OK        Data read success
 **
 ******************************************************************************/
static uint8_t I2c_Read(uint8_t *pRxData, uint8_t u8Size)
{
    uint8_t i = 0;

    while(i < u8Size)
    {
        /* Config ACK or NACK */
        if(i == u8Size-1)
        {
            I2c_ConfigAck(u8I2cCh, I2cNAck); /* Last byte send a NACK */
        }
        else
        {
            I2c_ConfigAck(u8I2cCh, I2cAck);
        }

        /* Clear interrupt flag and receive data to RX buffer */
        I2c_ClrStatus(u8I2cCh, I2cRxTxIrq);

        while(1)
        {
            if(TRUE == I2c_GetStatus(u8I2cCh, I2cRxTxIrq))
            {
                break;
            }
        }

        /* Wait for the receive data */
        while(1)
        {
            if(TRUE == I2c_GetStatus(u8I2cCh, I2cRxFull))
            {
                break;
            }
        }

        if(TRUE == I2c_GetStatus(u8I2cCh, I2cBusErr))
        {
            return I2C_RET_ERROR;   /* Bus error occurs? */
        }

        if(TRUE == I2c_GetStatus(u8I2cCh, I2cOverrunError))
        {
            return I2C_RET_ERROR;  /* Overrun error occurs? */
        }

        I2c_ReceiveData(u8I2cCh, (uint8_t*)&pRxData[i++]);
    }
    return I2C_RET_OK;
}

/**
 *******************************************************************************
 ** \brief Config the stop condition
 **
 ** \param [in]  None
 **
 ** \retval I2C_RET_ERROR     Generate stop signal failed
 ** \retval I2C_RET_OK        Generate stop signal success
 **
 ******************************************************************************/
static uint8_t I2c_Stop(void)
{
    /* Generate I2C stop signal */
    if(Ok != I2c_GenerateStop(u8I2cCh))
    {
        return I2C_RET_ERROR; /* Timeout or other error */
    }
    /* Clear Stop condition flag */
    while(1)
    {
        if(TRUE == I2c_GetStatus(u8I2cCh, I2cStopDetect))
        {
            break;
        }
    }

    if(TRUE == I2c_GetStatus(u8I2cCh, I2cBusErr))
    {
        return I2C_RET_ERROR;
    }

    I2c_ClrStatus(u8I2cCh, I2cStopDetect);
    I2c_ClrStatus(u8I2cCh, I2cRxTxIrq);

    return I2C_RET_OK;
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

    stc_clk_config_t stcCfg;
    en_result_t enRet = Ok;

    /* Set System clock */
    DDL_ZERO_STRUCT(stcCfg);
    stcCfg.u8PllMulti = 4;
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
    else
    {
    }

    /* I2C master init */
    InitI2cMaster();

    /* I2C master send data */
    I2c_Start(I2C_DEV_ADDR_W);
    I2c_Write(au8I2cMasterTxBuf, SAMPLE_I2C_MASTER_TX_BUFFSIZE);
    I2c_Stop();

    delay1ms(1);

    /* I2C master receive data */
    I2c_Start(I2C_DEV_ADDR_R);
    I2c_Read(au8I2cMasterRxBuf, SAMPLE_I2C_MASTER_RX_BUFFSIZE);
    I2c_Stop();

    /* DeInit I2C Bus */
    I2c_DeInit(u8I2cCh);
    if(Ok != memcmp(au8I2cMasterTxBuf, au8I2cMasterRxBuf, SAMPLE_I2C_MASTER_TX_BUFFSIZE))
    {
        while(1){} /* Communication fails if code runs here. */
    }

    while(1){} /* Data is normally sent and received */
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
