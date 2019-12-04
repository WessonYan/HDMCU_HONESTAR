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
/** \file dmac.c
 **
 ** A detailed description is available at
 ** @link DmacGroup Dmac description @endlink
 **
 **   - 2017-6-7  1.0  Zhangxl First version for Device Driver Library of Dmac
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "dmac.h"

/**
 *******************************************************************************
 ** \addtogroup DmacGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*! Parameter valid check for Dmac Channel */
#define IS_VALID_CH(x)                  \
(   DMACH0 == (x) ||                    \
    DMACH1 == (x))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static en_result_t DmaInitNvic(uint8_t u8DmaChannel);
static en_result_t DmaDeInitNvic(uint8_t u8DmaChannel);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static stc_dma_intern_data_t stcDmaInternData;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief DMA interrupt handler
 **
 ** \param [in]  u8DmaChannel     Channel number
 **
 ** \note depending on the DMA stop status the DmaCallback function is called
 **       on successful transfer or DmaErrorCallback function on errornous
 **       transfer. The ErrorCallback has the error code as an (uint8_t)
 **       argument which reflects the 3 bits of the stop status.
 ******************************************************************************/
void Dmac_IrqHandler(uint8_t u8DmaChannel)
{
    uint8_t u8StopStatus;

    u8StopStatus = (uint8_t)Dma_GetStopCause(u8DmaChannel);
    Dma_ClrStopCause(u8DmaChannel);     /* clear interrupt cause */

    if (0x5 == u8StopStatus)            /* Successful DMA transfer */
    {
        stcDmaInternData.apfnDmaIrqCompletionCb[u8DmaChannel]();
    }else                               /* Errornous DMA transfer */
    {
        stcDmaInternData.apfnDmaErrorIrqCb[u8DmaChannel](u8StopStatus);
    }
}

/**
 *******************************************************************************
 ** \brief Init DMA interrupt IRQ
 **
 ** \param [in]  u8DmaChannel     Channel number
 **
 ** \retval Ok                    init successful
 **
 ******************************************************************************/
static en_result_t DmaInitNvic(uint8_t u8DmaChannel)
{
    ASSERT(IS_VALID_CH(u8DmaChannel));

    if (DMACH0 == u8DmaChannel)
    {
        NVIC_ClearPendingIRQ(MSC6RX_DMAC0_IRQn);
        NVIC_EnableIRQ(MSC6RX_DMAC0_IRQn);
        NVIC_SetPriority(MSC6RX_DMAC0_IRQn, DDL_IRQ_LEVEL_DMAC0);
    }else if (DMACH1 == u8DmaChannel)
    {
        NVIC_ClearPendingIRQ(MSC6TX_DMAC1_IRQn);
        NVIC_EnableIRQ(MSC6TX_DMAC1_IRQn);
        NVIC_SetPriority(MSC6TX_DMAC1_IRQn, DDL_IRQ_LEVEL_DMAC1);
    }else
    {
        return ErrorInvalidParameter;
    }
    return Ok;
}

/**
 *******************************************************************************
 ** \brief De-Init DMA interrupt
 **
 ** \param [in]  u8DmaChannel     Channel number
 **
 ** \retval Ok                    de-init successful
 **
 ******************************************************************************/
static en_result_t DmaDeInitNvic(uint8_t u8DmaChannel)
{
    ASSERT(IS_VALID_CH(u8DmaChannel));

    if (DMACH0 == u8DmaChannel)
    {
        NVIC_ClearPendingIRQ(MSC6RX_DMAC0_IRQn);
        NVIC_DisableIRQ(MSC6RX_DMAC0_IRQn);
        NVIC_SetPriority(MSC6RX_DMAC0_IRQn, DDL_IRQ_LEVEL_DEFAULT);
    }else if (DMACH1 == u8DmaChannel)
    {
        NVIC_ClearPendingIRQ(MSC6TX_DMAC1_IRQn);
        NVIC_DisableIRQ(MSC6TX_DMAC1_IRQn);
        NVIC_SetPriority(MSC6TX_DMAC1_IRQn, DDL_IRQ_LEVEL_DEFAULT);
    }else
    {
        return ErrorInvalidParameter;
    }
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Enable interrupt of a DMA channel
 **
 ** Enable interrupt and set interrupt callback function of a DMA channel
 **
 ** \param [in]  u8DmaChannel    DMA channel index
 ** \param [in]  stcIrqSel       DMA interrupt selection structure
 **
 ** \retval Ok                    Interrupt enabled normally
 ** \retval ErrorInvalidParameter u8DmaChannel > DMA_CH_CNT
 **                               Other invalid configuration
 **
 ******************************************************************************/
en_result_t Dma_EnableIrq(uint8_t u8DmaChannel,
                          stc_dma_irq_sel_t* pstcIrqSel)
{
    stc_dmac_dmacb0_field_t *stcDmacDmacb;

    ASSERT(IS_VALID_CH(u8DmaChannel));

    if ((u8DmaChannel + 1u) > DMA_CH_CNT)
    {
        return ErrorInvalidParameter;
    }

    stcDmacDmacb = (stc_dmac_dmacb0_field_t*)(M0P_DMAC_BASE + u8DmaChannel * 0x10 + DMA_DMACB_OFFSET);

    /* Update EI and CI bit */
    if (pstcIrqSel->bCompleteIrq == TRUE)
    {
        stcDmacDmacb->CI = 1u;
    }

    if (pstcIrqSel->bErrorIrq == TRUE)
    {
        stcDmacDmacb->EI = 1u;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable interrupt of a DMA channel
 **
 ** \param [in]  u8DmaChannel    DMA channel index
 ** \param [in]  stcIrqSel       DMA interrupt selection structure
 **
 ** \retval Ok                    Interrupt disabled normally
 ** \retval ErrorInvalidParameter u8DmaChannel > DMA_CH_CNT
 **                               Other invalid configuration
 **
 ******************************************************************************/
en_result_t Dma_DisableIrq(uint8_t u8DmaChannel,
                           stc_dma_irq_sel_t* pstcIrqSel)
{
    stc_dmac_dmacb0_field_t *stcDmacDmacb;

    ASSERT(IS_VALID_CH(u8DmaChannel));

    if ((u8DmaChannel + 1u) > DMA_CH_CNT)
    {
        return ErrorInvalidParameter;
    }

    stcDmacDmacb = (stc_dmac_dmacb0_field_t*)(M0P_DMAC_BASE + u8DmaChannel * DMA_CH_OFFSET + DMA_DMACB_OFFSET);

    /* Update EI and CI bit */
    if (pstcIrqSel->bCompleteIrq == TRUE)
    {
        stcDmacDmacb->CI = 0u;
    }

    if (pstcIrqSel->bErrorIrq == TRUE)
    {
        stcDmacDmacb->EI = 0u;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Initializes a DMA channel
 **
 ** Sets up an DMA channel without starting immediate DMA transfer.
 ** Set_Dma_Channel() is used for starting DMA transfer.
 **
 ** \param [in]  u8DmaChannel     DMA channel number
 ** \param [in]  pstcConfig       DMA module configuration
 **
 ** \retval Ok                    init successful
 ** \retval ErrorInvalidParameter pstcAdc == NULL or other invalid configuration
 ** \retval OperationInProgress   DMA channel already in use
 **
 ******************************************************************************/
en_result_t Dma_InitChannel(uint8_t u8DmaChannel, volatile stc_dma_config_t* pstcConfig)
{
    stc_dmac_dmaca0_field_t *stcDmacDmaca;
    stc_dmac_dmacb0_field_t *stcDmacDmacb;
    stc_dmac_dmacsa0_field_t *stcDmacDmacsa;
    stc_dmac_dmacda0_field_t *stcDmacDmacda;

    uint32_t u32DrqselBit;

    ASSERT(IS_VALID_CH(u8DmaChannel));
    ASSERT(NULL != pstcConfig);

    /* Check for NULL pointer */
    if (NULL == pstcConfig)
    {
        return ErrorInvalidParameter;
    }

    stcDmacDmaca = (stc_dmac_dmaca0_field_t*)(M0P_DMAC_BASE + u8DmaChannel * DMA_CH_OFFSET + DMA_DMACA_OFFSET);
    stcDmacDmacb = (stc_dmac_dmacb0_field_t*)(M0P_DMAC_BASE + u8DmaChannel * DMA_CH_OFFSET + DMA_DMACB_OFFSET);
    stcDmacDmacsa = (stc_dmac_dmacsa0_field_t*)(M0P_DMAC_BASE + u8DmaChannel * DMA_CH_OFFSET + DMA_DMASA_OFFSET);
    stcDmacDmacda = (stc_dmac_dmacda0_field_t*)(M0P_DMAC_BASE + u8DmaChannel * DMA_CH_OFFSET + DMA_DMADA_OFFSET);

    /* Setup DMACA */
    /* Block Count */
    stcDmacDmaca->BC = 0x0Fu & (uint32_t)(pstcConfig->u8BlockCount - 1u);

    /* Transfer Count */
    stcDmacDmaca->TC = 0xFFFFu & (uint32_t)(pstcConfig->u16TransferCount - 1u);

    /* Interrupt Request Channel */
    if (Software == pstcConfig->enDmaIdrq)
    {
        stcDmacDmaca->IS = 0u;
    }else
    {
        stcDmacDmaca->IS = 0x20u | (0x1Fu & (uint32_t)(pstcConfig->enDmaIdrq));
    }

    /* Setup DMACB */
    /* First Transfer mode */
    switch (pstcConfig->enTransferMode)
    {
        case DmaBlockTransfer:
            stcDmacDmacb->MS = 0u;
            break;
        case DmaBurstTransfer:
            stcDmacDmacb->MS = 1u;
            break;
        case DmaDemandTransfer:
            stcDmacDmacb->MS = 2u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    /* Transfer width */
    switch (pstcConfig->enTransferWdith)
    {
        case Dma8Bit:
            stcDmacDmacb->TW = 0u;
            break;
        case Dma16Bit:
            stcDmacDmacb->TW = 1u;
            break;
        case Dma32Bit:
            stcDmacDmacb->TW = 2u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    /* Fixed source */
    stcDmacDmacb->FS = (uint8_t)((pstcConfig->bFixedSource == TRUE) ? 1u : 0u);

    /* Fixed destination */
    stcDmacDmacb->FD = (uint8_t)((pstcConfig->bFixedDestination == TRUE) ? 1u : 0u);

    /* Reload Count (BC/TC reload) */
    stcDmacDmacb->RC = (uint8_t)((pstcConfig->bReloadCount == TRUE) ? 1u : 0u);

    /* Reload Source */
    stcDmacDmacb->RS = (uint8_t)((pstcConfig->bReloadSource == TRUE) ? 1u : 0u);

    /* Reload Destination */
    stcDmacDmacb->RD = (uint8_t)((pstcConfig->bReloadDestination == TRUE) ? 1u : 0u);

    /* Enable bit mask */
    stcDmacDmacb->EM = (uint8_t)((pstcConfig->bEnableBitMask == TRUE) ? 1u : 0u);

    /* Set interrupt */
    if (NULL != pstcConfig->pstcIrqEn)
    {
        if (TRUE == pstcConfig->pstcIrqEn->bCompleteIrq)
        {
            stcDmacDmacb->CI = 1;
        }

        if (TRUE == pstcConfig->pstcIrqEn->bErrorIrq)
        {
            stcDmacDmacb->EI = 1;
        }
    }

    /* Setup source address */
    stcDmacDmacsa->DMACSA = pstcConfig->u32SourceAddress;

    /* Setup destination address */
    stcDmacDmacda->DMACDA = pstcConfig->u32DestinationAddress;

    /* Switch resource interrupt to DMA (except software DMA) */
    if (pstcConfig->enDmaIdrq != Software)
    {
        u32DrqselBit = 1ul << (uint32_t)(pstcConfig->enDmaIdrq);
        M0P_IRQC->IRQ_DSEL |= u32DrqselBit;
    }

    /* Set interrupt callback functions */
    if (NULL != pstcConfig->pstcIrqCb)
    {
        stcDmaInternData.apfnDmaIrqCompletionCb[u8DmaChannel] = pstcConfig->pstcIrqCb->pfnDmaCompletionIrqCb;
        stcDmaInternData.apfnDmaErrorIrqCb[u8DmaChannel] = pstcConfig->pstcIrqCb->pfnDmaErrorIrqCb;
    }

    /* Set NVIC */
    if (TRUE == pstcConfig->bTouchNvic)
    {
        DmaInitNvic(u8DmaChannel);
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief De-Initializes a DMA channel
 **
 ** Clears an DMA channel.
 **
 ** \param [in]  u8DmaChannel     DMA channel number
 **
 ** \retval Ok                    init successful
 ** \retval ErrorInvalidParameter pstcAdc == NULL or other invalid configuration
 **
 ******************************************************************************/
en_result_t Dma_DeInitChannel(uint8_t u8DmaChannel)
{
    ASSERT(IS_VALID_CH(u8DmaChannel));

    *(uint32_t*)((uint32_t)(M0P_DMAC_BASE + u8DmaChannel * DMA_CH_OFFSET + DMA_DMACA_OFFSET)) = 0;
    *(uint32_t*)((uint32_t)(M0P_DMAC_BASE + u8DmaChannel * DMA_CH_OFFSET + DMA_DMACB_OFFSET)) = 0;
    *(uint32_t*)((uint32_t)(M0P_DMAC_BASE + u8DmaChannel * DMA_CH_OFFSET + DMA_DMASA_OFFSET)) = 0;
    *(uint32_t*)((uint32_t)(M0P_DMAC_BASE + u8DmaChannel * DMA_CH_OFFSET + DMA_DMADA_OFFSET)) = 0;

    DmaDeInitNvic(u8DmaChannel);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Enable, disable, pause, or trigger a DMA channel via configuration
 **
 ** This function enables, disables, pauses or triggers a DMA transfer according
 ** to the settings in the configuration bits for EB (Enable), PB (Pause) and
 ** ST (Software Trigger)
 **
 ** \param [in]  u8DmaChannel     DMA channel number
 ** \param [in]  bEnable          Enable the channel or not
 ** \param [in]  bPause           Pause the channel or not
 ** \param [in]  bSwTrigger       Pause the channel or not
 **
 ** \retval Ok                    Setting finished
 **
 ******************************************************************************/
en_result_t Dma_SetChannel(uint8_t u8DmaChannel,
                           boolean_t bEnable,
                           boolean_t bPause,
                           boolean_t bSwTrigger)
{
    stc_dmac_dmaca0_field_t *stcDmacDmaca;

    ASSERT(IS_VALID_CH(u8DmaChannel));

    stcDmacDmaca = (stc_dmac_dmaca0_field_t*)(M0P_DMAC_BASE + u8DmaChannel * DMA_CH_OFFSET + DMA_DMACA_OFFSET);
    stcDmacDmaca->EB = bEnable;
    stcDmacDmaca->PB = bPause;
    stcDmacDmaca->ST = bSwTrigger;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Read stop cause of a DMA channel
 **
 ** \param [in]  u8DmaChannel       DMA channel
 **
 ** \retval DmaStopByAddrOverflow      DMA stops by address overflow
 ** \retval DmaStopByTransferStopRqst  DMA stops by transfer stop request
 ** \retval DmaStopBySrcAccessError    DMA stops by source access error
 ** \retval DmaStopByDstAccessError    DMA stops by destination access error
 ** \retval DmaStopByTransferComplete  DMA stops by transfer completion
 ** \retval DmaStopByTransferPause     DMA stops by transfer pause
 **
 ******************************************************************************/
dma_stop_cause_t Dma_GetStopCause(uint8_t u8DmaChannel)
{
    stc_dmac_dmacb0_field_t *stcDmacDmacb;

    ASSERT(IS_VALID_CH(u8DmaChannel));

    stcDmacDmacb = (stc_dmac_dmacb0_field_t*)(M0P_DMAC_BASE + u8DmaChannel * DMA_CH_OFFSET + DMA_DMACB_OFFSET);

    return (dma_stop_cause_t)(stcDmacDmacb->SS);
}

/**
 *******************************************************************************
 ** \brief Clear stop cause of a DMA channel
 **
 ** \retval Ok                    Enable finished
 **
 ******************************************************************************/
en_result_t Dma_ClrStopCause(uint8_t u8DmaChannel)
{
    stc_dmac_dmacb0_field_t *stcDmacDmacb;

    ASSERT(IS_VALID_CH(u8DmaChannel));

    stcDmacDmacb = (stc_dmac_dmacb0_field_t*)(M0P_DMAC_BASE + u8DmaChannel * DMA_CH_OFFSET + DMA_DMACB_OFFSET);

    stcDmacDmacb->SS = 0;

    return Ok;
}


/**
 *******************************************************************************
 ** \brief Enable DMA globally
 **
 ** \retval Ok                    Enable finished
 **
 ******************************************************************************/
en_result_t Dma_Enable(void)
{
    M0P_DMAC->DMACR = 0x80000000ul; /* Set DE Bit (DMA enable all channels) */

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable DMA globally
 **
 ** \retval Ok                    Disable finished
 **
 ******************************************************************************/
en_result_t Dma_Disable(void)
{
    M0P_DMAC->DMACR = 0u; /* Clear everything */

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Pause DMA globally
 **
 ** \retval Ok                    Pause finished
 **
 ******************************************************************************/
en_result_t Dma_Pause(void)
{
    M0P_DMAC->DMACR_f.DH = 0x0Fu;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Resume DMA globally
 **
 ** \retval Ok                    Resume from pause status finished
 **
 ******************************************************************************/
en_result_t Dma_Resume(void)
{
    M0P_DMAC->DMACR_f.DH = 0x0000u;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Set priority of DMA transfer
 **
 ** \param [in]  bPriority        fix order or not
 **
 ** \retval Ok                    Set finished
 **
 ******************************************************************************/
en_result_t Dma_SetPri(boolean_t bPriority)
{
    M0P_DMAC->DMACR_f.PR = bPriority;
    return Ok;
}

//@} // DmacGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
