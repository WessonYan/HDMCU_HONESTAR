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
/** \file dmac.h
 **
 ** A detailed description is available at
 ** @link DmacGroup Dmac description @endlink
 **
 **   - 2017-6-5  1.0  Zhangxl First version for Device Driver Library of Dmac.
 **
 ******************************************************************************/

#ifndef __DMAC_H__
#define __DMAC_H__

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
 ** \defgroup DmacGroup Direct Memory Access (DMA)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief DMA transfer data width
 ******************************************************************************/
typedef enum en_dma_transfermode
{
    DmaBlockTransfer = 0u,              ///< Block Transfer
    DmaBurstTransfer = 1u,              ///< Burst transfer
    DmaDemandTransfer = 2u              ///< Demand transfer
} en_dma_transfermode_t;

/**
 *******************************************************************************
 ** \brief DMA transfer data width
 ******************************************************************************/
typedef enum en_dma_transferwidth
{
    Dma8Bit = 0u,                       ///< 8 bit transfer via DMA
    Dma16Bit = 1u,                      ///< 16 bit transfer via DMA
    Dma32Bit = 2u                       ///< 32 bit transfer via DMA
} en_dma_transferwidth_t;

/**
 *******************************************************************************
 ** \brief DMA IDREQ number
 **
 ******************************************************************************/
typedef enum en_dma_idreq
{
    //               0u,   ///< reserved
    //               1u,   ///< reserved
    //               2u,   ///< reserved
    //               3u,   ///< reserved
    //               4u,   ///< reserved
    Adc0 = 5u,             ///< Scan conversion interrupt signal from A/D converter unit0
    //               6u,   ///< reserved
    //               7u,   ///< reserved
    Ct0Irq0 = 8u,          ///< Interrupt signal from IRQ0 of composite timer ch.0
    Ct2Irq0 = 9u,          ///< Interrupt signal from IRQ0 of composite timer ch.2
    //               10u,  ///reserved
    //               11u,  ///reserved
    //               12u,  ///reserved
    //               13u,  ///reserved
    //               14u,  ///reserved
    //               15u,  ///reserved
    //               16u,  ///reserved
    //               17u,  ///reserved
    //               18u,  ///reserved
    //               19u,  ///reserved
    MscRx4 = 20u,          ///< Receiving interrupt signal from MSC ch.4
    MscTx4 = 21u,          ///< Sending interrupt signal from MSC ch.4
    MscRx5 = 22u,          ///< Receiving interrupt signal from MSC ch.5
    MscTx5 = 23u,          ///< Sending interrupt signal from MSC ch.5
    MscRx6 = 24u,          ///< Receiving interrupt signal from MSC ch.6
    MscTx6 = 25u,          ///< Sending interrupt signal from MSC ch.6
    //               26u,  ///reserved
    //               27u,  ///reserved
    ExtIrq0 = 28u,         ///< Interrupt signal from external interrupt unit ch.0
    ExtIrq1 = 29u,         ///< Interrupt signal from external interrupt unit ch.1
    ExtIrq2 = 30u,         ///< Interrupt signal from external interrupt unit ch.2
    ExtIrq3 = 31u,         ///< Interrupt signal from external interrupt unit ch.4
    Software = 9999u       ///< Software Demand (just a high number)
} en_dma_idreq_t;

/**
 *******************************************************************************
 ** \brief DMA stop cause
 ******************************************************************************/
typedef enum dma_stop_cause
{
    DmaStopByAddrOverflow = 0u,         ///< DMA stops by address overflow
    DmaStopByTransferStopRqst = 1u,     ///< DMA stops by transfer stop request
    DmaStopBySrcAccessError = 2u,       ///< DMA stops by source access error
    DmaStopByDstAccessError = 3u,       ///< DMA stops by destination access error
    DmaStopByTransferComplete = 4u,     ///< DMA stops by transfer completion
    DmaStopByTransferPause = 5u         ///< DMA stops by transfer pause
}dma_stop_cause_t;

/**
 *******************************************************************************
 ** \brief DMA interrupt selection
 ******************************************************************************/
typedef struct stc_dma_irq_sel
{
    boolean_t bCompleteIrq;   ///< Select DMA transfer completion interrupt
    boolean_t bErrorIrq;      ///< Select DMA transfer error interrupt
}stc_dma_irq_sel_t, stc_dma_irq_en_t;

/**
 *******************************************************************************
 ** \brief DMA interrupt callback function
 ******************************************************************************/
typedef struct stc_dma_irq_cb
{
    func_ptr_t pfnDmaCompletionIrqCb;   ///< Pointer to DMA transfer completion interrupt callback function
    func_ptr_arg1_t pfnDmaErrorIrqCb;   ///< Pointer to DMA transfer error interrupt callback function
}stc_dma_irq_cb_t;

/**
 *******************************************************************************
 ** \brief Datatype for holding internal data needed for DMA
 ******************************************************************************/
typedef struct stc_dma_intern_data
{
    func_ptr_t apfnDmaIrqCompletionCb[2];  ///< DMA transfer completion callback pointer array
    func_ptr_arg1_t apfnDmaErrorIrqCb[2];  ///< DMA error callback pointer array
} stc_dma_intern_data_t;

/**
 *******************************************************************************
 ** \brief DMA configuration
 ******************************************************************************/
typedef struct stc_dma_config
{
    en_dma_idreq_t enDmaIdrq;                       ///< ID Request number (see #en_dma_idreq_t for details)
    uint8_t u8BlockCount;                           ///< Block counter
    uint16_t u16TransferCount;                      ///< Transfer counter
    en_dma_transfermode_t enTransferMode;           ///< DMA transfer mode (see #en_dma_transfermode_t for details)
    en_dma_transferwidth_t enTransferWdith;         ///< DMA transfer width (see #en_dma_transferwidth_t for details)
    uint32_t u32SourceAddress;                      ///< Source address
    uint32_t u32DestinationAddress;                 ///< Destination address
    boolean_t bFixedSource;                         ///< TRUE = source address not increased
    boolean_t bFixedDestination;                    ///< TRUE = destiantion address not increased
    boolean_t bReloadCount;                         ///< TRUE = count is reloaded
    boolean_t bReloadSource;                        ///< TRUE = source address is reloaded
    boolean_t bReloadDestination;                   ///< TRUE = destination address is reloaded
    boolean_t bEnableBitMask;                       ///< FALSE = Clear EB (bEnable) bit on completion (mandatory for transfer end!)
    stc_dma_irq_en_t*      pstcIrqEn;               ///< Pointer to DMA interrupt enable structure
    stc_dma_irq_cb_t*      pstcIrqCb;               ///< Pointer to DMA interrupt callback function structure
    boolean_t bTouchNvic;                           ///< TRUE: enable NVIC, FALSE: disable NVIC
} stc_dma_config_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define DMA_CH_CNT              2       ///< DMAC max. channel count
#define DMACH0                  0       ///< DMAC channel 0
#define DMACH1                  1       ///< DMAC channel 1

#define DMA_DMACA_OFFSET        0x10ul  ///< Offset of register DMACA
#define DMA_DMACB_OFFSET        0x14ul  ///< Offset of register DMACB
#define DMA_DMASA_OFFSET        0x18ul  ///< Offset of register DMASA
#define DMA_DMADA_OFFSET        0x1Cul  ///< Offset of register DMADA

#define DMA_CH_OFFSET           0x10ul  ///< Offset of each DMA channel

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/* DMA IRQ functions */
en_result_t Dma_EnableIrq(uint8_t u8DmaChannel, stc_dma_irq_sel_t* pstcIrqSel);
en_result_t Dma_DisableIrq(uint8_t u8DmaChannel, stc_dma_irq_sel_t* pstcIrqSel);

/* Init/DeInit a DMA channel */
en_result_t Dma_InitChannel(uint8_t u8DmaChannel, volatile stc_dma_config_t* pstcConfig);
en_result_t Dma_DeInitChannel(uint8_t u8DmaChannel);

/* Activate a DMA channel */
en_result_t Dma_SetChannel(uint8_t u8DmaChannel,
                           boolean_t bEnable,
                           boolean_t bPause,
                           boolean_t bSwTrigger);

/* Stop casue read/clear */
dma_stop_cause_t Dma_GetStopCause(uint8_t u8DmaChannel);
en_result_t Dma_ClrStopCause(uint8_t u8DmaChannel);
/* Globally enable/disable all channels */
en_result_t Dma_Enable(void);
en_result_t Dma_Disable(void);
/* Globally pause/resume all channels */
en_result_t Dma_Pause(void);
en_result_t Dma_Resume(void);
/* Globally transfer priority set all channels */
en_result_t Dma_SetPri(boolean_t bPriority);

//@} // DmacGroup

#ifdef __cplusplus
}
#endif

#endif /* __DMAC_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
