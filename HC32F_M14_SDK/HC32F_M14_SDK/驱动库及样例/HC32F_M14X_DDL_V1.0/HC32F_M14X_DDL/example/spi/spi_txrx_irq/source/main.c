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
 ** \brief The example for spi tx and rx by interrupt demonstration.
 **        Get more details from readme text.
 **
 **   - 2017-8-16  1.0  Jason First version for Device Driver Library of spi.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "spi.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define     SPI_TX_CH           (6)
#define     SPI_RX_CH           (5)

#define     BUFF_SZ             (20)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t u8TxBuff[] = "1234567890abcdedf";
static uint8_t __attribute__((unused)) u8RxBuff[BUFF_SZ];

static volatile uint8_t u8RxCnt = 0;
static volatile uint8_t u8TxCnt = 0;
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief Spi tx callback function
 **
 ** \retval None
 **
 ******************************************************************************/
void SpiTxIrqCbk(void)
{
    Spi_SendData(SPI_TX_CH, u8TxBuff[u8TxCnt++]);
    Spi_DisableIrq(SPI_TX_CH,SpiIrqTx);
}

/**
 *******************************************************************************
 ** \brief Spi status callback function
 **
 ** \retval None
 **
 ******************************************************************************/
void SpiStatIrqCbk(void)
{
    ;
}

/**
 *******************************************************************************
 ** \brief Spi rx callback function
 **
 ** \retval None
 **
 ******************************************************************************/
void SpiRxIrqCbk(void)
{
    en_result_t ret = Ok;
    uint16_t u16Data = 0;

    if(TRUE == Spi_GetStatus(SPI_RX_CH,SpiRxFull))
    {
        ret = Spi_ReceiveData(SPI_RX_CH, FALSE, (uint16_t* )&u16Data);

        if(Ok == ret)
        {
            u8RxBuff[u8RxCnt] = (u16Data & 0xff);
            u8RxCnt++;
        }
    }

    if (u8RxCnt < strlen((const char* )u8TxBuff))
    {
        Spi_EnableIrq(SPI_TX_CH,SpiIrqTx);
    }
}

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_spi_config_t    stcConfigTx;
    stc_spi_config_t    stcConfigRx;
    stc_spi_irq_sel_t   stcIrqsel;
    stc_spi_irq_cb_t    stcIrqCb;


    DDL_ZERO_STRUCT(stcConfigTx);
    DDL_ZERO_STRUCT(stcConfigRx);
    DDL_ZERO_STRUCT(stcIrqsel);
    DDL_ZERO_STRUCT(stcIrqCb);

    stcConfigRx.enSckMarkLevel = SpiSckMarkLevel_High;
    stcConfigRx.bMSB = TRUE;
    stcConfigRx.bOutputData = TRUE;
    stcConfigRx.bOutputSCK = FALSE;
    stcConfigRx.bSOTHigh = FALSE;
    stcConfigRx.bSPIMode = TRUE;
    stcConfigRx.enDataLen = SpiDataLen8;
    stcConfigRx.bSlaveMode = TRUE;
    stcConfigRx.u32BaudRate = 100*1000;
    stcConfigRx.u8DataOuputDly = 3;

    stcIrqsel.bRxIrq = TRUE;
    stcConfigRx.pstcIrqEn = &stcIrqsel;
    stcIrqCb.pfnRxIrqCb = SpiRxIrqCbk;
    stcConfigRx.pstcIrqCb = &stcIrqCb;

    /* Goio init for tx. */
    Gpio_SetFunc_SCK6_1();
    Gpio_SetFunc_SOT6_1();

    /* Goio init for rx. */
    Gpio_SetFunc_SCK5_0();
    Gpio_SetFunc_SIN5_0();

    Spi_Init(SPI_RX_CH,&stcConfigRx);

    DDL_ZERO_STRUCT(stcConfigTx);
    DDL_ZERO_STRUCT(stcIrqsel);
    DDL_ZERO_STRUCT(stcIrqCb);

    stcConfigTx = stcConfigRx;
    stcConfigTx.bSlaveMode = FALSE;
    stcConfigTx.bOutputSCK = TRUE;

    stcIrqsel.bTxIrq = TRUE;
    stcConfigTx.pstcIrqEn = &stcIrqsel;
    stcIrqCb.pfnTxIrqCb = SpiTxIrqCbk;
    stcConfigTx.pstcIrqCb = &stcIrqCb;

    Spi_Init(SPI_TX_CH,&stcConfigTx);

    Spi_EnableFunc(SPI_RX_CH,SpiFuncRx);

    Spi_EnableFunc(SPI_TX_CH,SpiFuncTx);

    while (u8RxCnt < (strlen((const char*)u8TxBuff)-1)){}

    Spi_DeInit(SPI_TX_CH);
    Spi_DeInit(SPI_RX_CH);

    while (1){}
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
