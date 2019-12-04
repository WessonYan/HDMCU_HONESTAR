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
 ** \brief This sample demonstrates software block & burst transfer function of
 **        Dmac
 **
 **   - 2017-6-7  1.0  Zhangxl First version for Device Driver Library of Dmac
 **     software block & burst transfer samples.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "dmac.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief DMA transfer sequence
 ******************************************************************************/
typedef enum
{
    trans_idle = 0,
    trans_done
}en_dma_transfer_stat_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define DMAC0_BC        5               ///< DMA Ch.0 Block count
#define DMAC0_TC        3               ///< DMA Ch.0 Transfer count

#define DMAC1_BC        5               ///< DMA Ch.1 Block count
#define DMAC1_TC        3               ///< DMA Ch.1 Transfer count

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint8_t const au8Src[] = "0123456789abcdef";
uint8_t volatile au8Des[sizeof(au8Src) * 3] = { 0 };
en_dma_transfer_stat_t enDmaTransStat;

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief  DMA Ch.0 callback function
 ******************************************************************************/
void Dma0CompleteCb(void)
{
    enDmaTransStat = trans_done;
}

/**
 *******************************************************************************
 ** \brief  DMA Ch.1 callback function
 ******************************************************************************/
void Dma1CompleteCb(void)
{
    enDmaTransStat = trans_done;
}

/**
 *******************************************************************************
 ** \brief  Main function of project
 **
 ** \return int32_t return value, if needed
 **
 ** \note This sample use DMA Ch.0 software block transfer, DMA Ch.1 in software
 **       burst transfer.
 **
 ******************************************************************************/
int32_t main(void)
{
    /* DMA Ch.0, software block transfer */
    stc_dma_config_t stcDma0Cfg, stcDma1Cfg;
    stc_dma_irq_sel_t stcIrqSel;
    stc_dma_irq_cb_t stcIrqCb;
    uint8_t i = 0;
    enDmaTransStat = trans_idle;

    DDL_ZERO_STRUCT(stcDma0Cfg);
    DDL_ZERO_STRUCT(stcIrqSel);
    DDL_ZERO_STRUCT(stcIrqCb);

    stcIrqSel.bCompleteIrq = TRUE;
    stcIrqCb.pfnDmaCompletionIrqCb = Dma0CompleteCb;

    stcDma0Cfg.bFixedDestination = FALSE;
    stcDma0Cfg.bFixedSource = FALSE;
    stcDma0Cfg.bReloadCount = TRUE;
    stcDma0Cfg.bReloadDestination = TRUE;
    stcDma0Cfg.bReloadSource = TRUE;
    stcDma0Cfg.bTouchNvic = TRUE;
    stcDma0Cfg.enDmaIdrq = Software;
    stcDma0Cfg.enTransferMode = DmaBlockTransfer;
    stcDma0Cfg.enTransferWdith = Dma8Bit;
    stcDma0Cfg.pstcIrqCb = &stcIrqCb;
    stcDma0Cfg.pstcIrqEn = &stcIrqSel;
    stcDma0Cfg.u16TransferCount = DMAC0_TC;
    stcDma0Cfg.u8BlockCount = DMAC0_BC;
    stcDma0Cfg.u32DestinationAddress = (uint32_t)&au8Des[0];
    stcDma0Cfg.u32SourceAddress = (uint32_t)&au8Src[0];

    Dma_InitChannel(DMACH0, &stcDma0Cfg);
    Dma_SetChannel(DMACH0, TRUE, FALSE, TRUE);

    Dma_Enable();

    /* wait transfer done */
    while (trans_done != enDmaTransStat)
    {
        __asm ("nop");
    }

    for (i = 0; i < DMAC0_BC * DMAC0_TC; i++)
    {
        if (au8Src[i] != au8Des[i])
        {
            while (1)
            {
                ;                       /* error transfer */
            }
        }
    }

    Dma_Disable();
    enDmaTransStat = trans_idle;

    /* DMA Ch.1, software burst tranfer */
    DDL_ZERO_STRUCT(stcDma1Cfg);
    DDL_ZERO_STRUCT(stcIrqSel);
    DDL_ZERO_STRUCT(stcIrqCb);
    DDL_ZERO_STRUCT(au8Des);

    stcIrqSel.bCompleteIrq = TRUE;
    stcIrqCb.pfnDmaCompletionIrqCb = Dma1CompleteCb;

    stcDma1Cfg.bFixedDestination = FALSE;
    stcDma1Cfg.bFixedSource = FALSE;
    stcDma1Cfg.bReloadCount = TRUE;
    stcDma1Cfg.bReloadDestination = TRUE;
    stcDma1Cfg.bReloadSource = TRUE;
    stcDma1Cfg.bTouchNvic = TRUE;
    stcDma1Cfg.enDmaIdrq = Software;
    stcDma1Cfg.enTransferMode = DmaBurstTransfer;
    stcDma1Cfg.enTransferWdith = Dma8Bit;
    stcDma1Cfg.pstcIrqCb = &stcIrqCb;
    stcDma1Cfg.pstcIrqEn = &stcIrqSel;
    stcDma1Cfg.u16TransferCount = DMAC1_TC;
    stcDma1Cfg.u8BlockCount = DMAC1_BC;
    stcDma1Cfg.u32DestinationAddress = (uint32_t)&au8Des[0];
    stcDma1Cfg.u32SourceAddress = (uint32_t)&au8Src[0];

    Dma_InitChannel(DMACH1, &stcDma1Cfg);
    Dma_SetChannel(DMACH1, TRUE, FALSE, TRUE);

    Dma_Enable();

    /* wait transfer done */
    while (trans_done != enDmaTransStat)
    {
        __asm ("nop");
    }

    for (i = 0; i < DMAC1_BC * DMAC1_TC; i++)
    {
        if (au8Src[i] != au8Des[i])
        {
            while (1)
            {
                ;               // error transfer
            }
        }
    }
    while (1)
    {
        ;
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
