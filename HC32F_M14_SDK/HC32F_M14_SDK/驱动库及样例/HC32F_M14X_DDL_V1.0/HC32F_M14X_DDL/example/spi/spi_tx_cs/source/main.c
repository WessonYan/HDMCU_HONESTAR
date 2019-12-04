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
 ** \brief The example for spi chip selection demonstration.
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
#define     SPI_TX_CH           (4)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t __attribute__((unused)) u8TxBuff[] = "1234567890abcdedf";

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_spi_config_t    stcSpiCfgTx;
    stc_spi_cs_config_t stcCsconfig;

    DDL_ZERO_STRUCT(stcSpiCfgTx);
    DDL_ZERO_STRUCT(stcCsconfig);

    stcSpiCfgTx.enSckMarkLevel = SpiSckMarkLevel_High;
    stcSpiCfgTx.bMSB = TRUE;
    stcSpiCfgTx.bOutputData = TRUE;
    stcSpiCfgTx.bOutputSCK = TRUE;
    stcSpiCfgTx.bSOTHigh = FALSE;
    stcSpiCfgTx.bSPIMode = TRUE;
    stcSpiCfgTx.enDataLen = SpiDataLen8;
    stcSpiCfgTx.bSlaveMode = FALSE;
    stcSpiCfgTx.u32BaudRate = 100*1000;
    stcSpiCfgTx.u8DataOuputDly = 3;

    stcCsconfig.bActiveHold = FALSE;
    stcCsconfig.bCsIdleLow = TRUE;
    stcCsconfig.bCsOutputEn = TRUE;
    stcCsconfig.enCsClkDiv = SpiClkDiv1;
    stcCsconfig.enCsEndPin = SpiCsPin1;
    stcCsconfig.enCsStartPin = SpiCsPin0;
    stcCsconfig.u8CsDelayTime = 3;
    stcCsconfig.u8CsHoldTime = 3;
    stcCsconfig.u16CsDeselectTime = 5;
    stcCsconfig.stcCsPincfg[0].bEn = TRUE;
    stcCsconfig.stcCsPincfg[0].bMSB = TRUE;
    stcCsconfig.stcCsPincfg[0].bSPIMode = TRUE;
    stcCsconfig.stcCsPincfg[0].bSckInv = TRUE;
    stcCsconfig.stcCsPincfg[0].bCsInActiveHigh = TRUE;

    stcCsconfig.stcCsPincfg[1].bEn = TRUE;
    stcCsconfig.stcCsPincfg[1].bMSB = TRUE;
    stcCsconfig.stcCsPincfg[1].bSckInv = TRUE;
    stcCsconfig.stcCsPincfg[1].bSPIMode = TRUE;
    stcCsconfig.stcCsPincfg[1].bCsInActiveHigh = TRUE;

    /* Gpio init. */
    Gpio_SetFunc_SCK4_1(0u);
    Gpio_SetFunc_SOT4_1(0u);

    Gpio_SetFunc_SCS40_1(0u);
    Gpio_SetFunc_SCS41_1(0u);

    Spi_Init(SPI_TX_CH,&stcSpiCfgTx);
    Spi_CSCconfig(SPI_TX_CH,&stcCsconfig);
    Spi_EnableFunc(SPI_TX_CH,SpiFuncCsFormat);

    Spi_SetTxBytes(SPI_TX_CH,0,3);
    Spi_SetTxBytes(SPI_TX_CH,1,3);

    Spi_EnableFunc(SPI_TX_CH,SpiFuncTx);

    while (1)
    {
        while (FALSE == Spi_GetStatus(SPI_TX_CH,SpiTxEmpty)){}
        Spi_SendData(SPI_TX_CH,0x33);
    }

    Spi_DeInit(SPI_TX_CH);

    while (1){}
}

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
