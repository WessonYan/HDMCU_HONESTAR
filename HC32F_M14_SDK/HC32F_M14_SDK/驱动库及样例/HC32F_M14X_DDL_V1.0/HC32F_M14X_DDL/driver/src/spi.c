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
/** \file spi.c
 **
 ** A detailed description is available at
 ** @link SpiGroup Spi description @endlink
 **
 **   - 2017-8-15  1.0  Jason First version for Device Driver Library of spi.
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 *****************************************************************************/
#include "spi.h"

/**
 ******************************************************************************
 ** \addtogroup SpiGroup
 *****************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
typedef struct stc_spi_instance_data
{
    uint8_t                 u8Ch;           ///< Channel index.
    M0P_MSC_SPI_TypeDef     *pstcInstance;  ///< Pointer to spi base registers
    stc_spi_irq_cb_t        stcIrqCb;       ///< Irq callback function

} stc_spi_instance_data_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define MSC_SPI_MODE            (2)     ///< hardware setting
#define SPI_DATA_OUT_DEALY_MAX  (3)     ///< max 3bit delay

/*! Parameter validity check for spi channel \a x. */
#define IS_VALID_CH(x)                  \
(   SpiCh4 == (x) ||                    \
    SpiCh5 == (x) ||                    \
    SpiCh6 == (x) )

/*! Parameter validity check for spi function \a x. */
#define IS_VALID_FUNC(x)                \
(   SpiFuncTx <= (x) &&                 \
    SpiFuncSerialTimer >= (x) )

/*! Parameter validity check for spi interrupt request \a x. */
#define IS_VALID_IRQ(x)                 \
(   SpiIrqTx <=(x) &&                   \
    SpiIrqSerialTimer >= (x) )

/*! Parameter validity check for spi status \a x. */
#define IS_VALID_STAT(x)                \
(   SpiOverrunError <= (x)||            \
    SpiSerialTimerIntFlag >= (x) )

/*! Parameter validity check for spi nvic level. */
#define SPI_NVIC_LEVEL(chn,txrx)    (SpiCh4 == chn ? DDL_IRQ_LEVEL_MSC4##txrx : \
                                    (SpiCh5 == chn ? DDL_IRQ_LEVEL_MSC5##txrx : \
                                                     DDL_IRQ_LEVEL_MSC6##txrx))

/*! Check the spi instance in debug mode. */
#define DEBUG_CHECK_INST(inst)                                                  \
do{                                                                             \
    ASSERT(NULL != inst);                                                       \
    ASSERT(NULL != inst->pstcInstance);                                         \
}while(0)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SpiEnableNvic(uint8_t u8ch, en_spi_irq_sel_t enSel);
static void SpiDisableNvic(uint8_t u8ch);
static en_result_t SpiEnableFunc(uint8_t u8Channel,
                                 en_spi_func_sel_t enFunc,
                                 boolean_t bFlag);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/* Look-up table for all SPI instances. */
static stc_spi_instance_data_t m_astcSpiDataLut[] =
{
    {
        SpiCh4,
        M0P_MSC4_SPI,
        {NULL,NULL,NULL,NULL,NULL},
    },
    {
        SpiCh5,
        M0P_MSC5_SPI,
        {NULL,NULL,NULL,NULL,NULL},
    },
    {
        SpiCh6,
        M0P_MSC6_SPI,
        {NULL,NULL,NULL,NULL,NULL},
    },
};

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Return the spi instance data by channel index
 **
 ** \param [in] u8Idx                   Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \retval pstcData                    Pointer to spi instance data
 ** \retval NULL                        If instance is not enabled
 **
 ******************************************************************************/
static stc_spi_instance_data_t* SpiGetInternDataPtr(uint8_t u8Idx)
{
    stc_spi_instance_data_t *pstcData = NULL;
    uint32_t i = 0;

    ASSERT(IS_VALID_CH(u8Idx));

    for (i = 0; i < ARRAY_SZ(m_astcSpiDataLut); i++)
    {
        if (u8Idx == m_astcSpiDataLut[i].u8Ch)
        {
            pstcData = &m_astcSpiDataLut[i];
            break;
        }
    }

    /* If there is no specified instance data, check the look up table. */
    ASSERT(NULL != pstcData);

    return pstcData;
}

/**
 *******************************************************************************
 ** \brief Enable the nvic.
 **
 ** \param [in] u8ch                    Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 ** \param [in] enSel                   Irq tx or rx
 ** \arg SpiIrqRx                       Spi rx interrupt request
 ** \arg SpiIrqTx                       Spi tx interrupt request
 **
 ** \retval None
 **
 ******************************************************************************/
static void SpiEnableNvic(uint8_t u8ch, en_spi_irq_sel_t enSel)
{
    uint8_t u8Level = DDL_IRQ_LEVEL_DEFAULT;
    IRQn_Type enIrq;

    switch (u8ch)
    {
        case SpiCh4:
        case SpiCh5:
        case SpiCh6:
            enIrq = (IRQn_Type)(MSC4RX_IRQn + (u8ch - 4) * 2);
            break;
        default:
            return;
    }

    /* rx. */
    if (SpiIrqRx == enSel)
    {
        u8Level = SPI_NVIC_LEVEL(u8ch, RX);
        NVIC_ClearPendingIRQ(enIrq);
        NVIC_EnableIRQ(enIrq);
        NVIC_SetPriority(enIrq, u8Level);
    }

    /* tx. */
    if (SpiIrqTx == enSel)
    {
        u8Level = SPI_NVIC_LEVEL(u8ch, TX);
        enIrq = (IRQn_Type)(enIrq + 1);
        NVIC_ClearPendingIRQ(enIrq);
        NVIC_EnableIRQ(enIrq);
        NVIC_SetPriority(enIrq, u8Level);
    }
}

/**
 *******************************************************************************
 ** \brief Disable the nvic.
 **
 ** \param [in] u8ch                    Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \retval None
 **
 ******************************************************************************/
static void SpiDisableNvic(uint8_t u8ch)
{
    IRQn_Type enIrq;

    switch (u8ch)
    {
        case SpiCh4:
        case SpiCh5:
        case SpiCh6:
            enIrq = (IRQn_Type)(MSC4RX_IRQn + u8ch - 4);
            break;
        default:
            return;
    }

    /* rx. */
    NVIC_ClearPendingIRQ(enIrq);
    NVIC_DisableIRQ(enIrq);
    NVIC_SetPriority(enIrq, DDL_IRQ_LEVEL_DEFAULT);

    /* tx. */
    enIrq = (IRQn_Type)(enIrq + 1);
    NVIC_ClearPendingIRQ(enIrq);
    NVIC_DisableIRQ(enIrq);
    NVIC_SetPriority(enIrq, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 *******************************************************************************
 ** \brief Enable or disable function.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] enFunc                  Spi func
 ** \arg SpiFuncTx                      Spi tx function
 ** \arg SpiFuncRx                      Spi rx function
 ** \arg SPiFuncSyncTx                  Spi synchronous tx function
 ** \arg SpiFuncCsFormat                Spi cs format function
 ** \arg SpiFuncSerialTimer             Spi serial timer function
 **
 ** \param [in] bFlag                   TRUE of FALSE
 ** \arg TRUE                           Enable spi function
 ** \arg FALSE                          Disable spi function
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcInst is null
 **                                     - pstcInst->pstcInstance is null
 **                                     - enFunc is invalid
 **
 ******************************************************************************/
static en_result_t SpiEnableFunc(uint8_t u8Channel,
                                 en_spi_func_sel_t enFunc,
                                 boolean_t bFlag)
{
    en_result_t enRet = Error;
    stc_spi_instance_data_t *pstcInst = NULL;
    M0P_MSC_SPI_TypeDef     *pstcSPI = NULL;

    ASSERT(IS_VALID_FUNC(enFunc));

    pstcInst = SpiGetInternDataPtr(u8Channel);
    DEBUG_CHECK_INST(pstcInst);

    if ((NULL == pstcInst) || (NULL == pstcInst->pstcInstance))
    {
        return ErrorInvalidParameter;
    }

    pstcSPI = pstcInst->pstcInstance;

    switch (enFunc)
    {
        case SpiFuncTx:
            pstcSPI->CR_f.TXE = bFlag;
            break;
        case SpiFuncRx:
            pstcSPI->CR_f.RXE = bFlag;
            break;
        case SPiFuncSyncTx:
            pstcSPI->EACR_f.SYNTE = bFlag;
            break;
        case SpiFuncCsFormat:
            pstcSPI->ECR_f.CSFE = bFlag;
            break;
        case SpiFuncSerialTimer:
            pstcSPI->EACR_f.TMRE = bFlag;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable irq.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] enIrqSel                Interrupt request
 ** \arg SpiIrqTx                       Spi tx interrupt request
 ** \arg SpiIrqRx                       Spi rx interrupt request
 ** \arg SpiIrqTxIdle                   Spi tx bus idle interrupt request
 ** \arg SpiIrqCsErr                    Spi cs error interrupt request
 ** \arg SpiIrqSerialTimer              Spi serial timer interrupt request
 **
 ** \param [in] bFlag                   TRUE of FALSE
 ** \arg TRUE                           Enable interrupt request
 ** \arg FALSE                          Disable interrupt request
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcInst is null
 **                                     - pstcInst->pstcInstance is null
 **                                     - enIrqSel is invalid
 **
 ******************************************************************************/
static en_result_t SpiEnableIrq(uint8_t u8Channel,
                                en_spi_irq_sel_t enIrqSel,
                                boolean_t bFlag)
{
    en_result_t enRet = Error;
    stc_spi_instance_data_t *pstcInst = NULL;
    M0P_MSC_SPI_TypeDef     *pstcSPI = NULL;

    ASSERT(IS_VALID_IRQ(enIrqSel));

    pstcInst = SpiGetInternDataPtr(u8Channel);
    DEBUG_CHECK_INST(pstcInst);

    if ((NULL == pstcInst) || (NULL == pstcInst->pstcInstance))
    {
        return ErrorInvalidParameter;
    }

    pstcSPI = pstcInst->pstcInstance;

    switch (enIrqSel)
    {
        case SpiIrqTx:
            pstcSPI->CR_f.TXIE = bFlag;
            break;
        case SpiIrqRx:
            pstcSPI->CR_f.RXIE = bFlag;
            break;
        case SpiIrqTxIdle:
            pstcSPI->CR_f.TBIE = bFlag;
            break;
        case SpiIrqCsErr:
            pstcSPI->EACR_f.CSEIE = bFlag;
            break;
        case SpiIrqSerialTimer:
            pstcSPI->EACR_f.TIE = bFlag;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief The interrupt service routine for rx.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \retval None
 **
 ******************************************************************************/
void Spi_Rx_IrqHandler(uint8_t u8Channel)
{
    stc_spi_instance_data_t *pstcInst = NULL;

    pstcInst = SpiGetInternDataPtr(u8Channel);

    if (NULL == pstcInst)
    {
        return;
    }

    if (1u == pstcInst->pstcInstance->SR_f.RDFF)
    {
        if (NULL != pstcInst->stcIrqCb.pfnRxIrqCb)
        {
            pstcInst->stcIrqCb.pfnRxIrqCb();
        }
    }

    if (1u == pstcInst->pstcInstance->SR_f.ORF)
    {
        if (NULL != pstcInst->stcIrqCb.pfnRxErrIrqCb)
        {
            pstcInst->stcIrqCb.pfnRxErrIrqCb();
        }

        pstcInst->pstcInstance->SR_f.RECLR = TRUE;
    }
}

/**
 *******************************************************************************
 ** \brief The interrupt service routine for tx.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \retval None
 **
 ******************************************************************************/
void Spi_Tx_IrqHandler(uint8_t u8Channel)
{
    stc_spi_instance_data_t *pstcInst = NULL;

    pstcInst = SpiGetInternDataPtr(u8Channel);

    if (NULL == pstcInst)
    {
        return;
    }

    /* TXDR empty interrupt function. */
    if (1u == pstcInst->pstcInstance->SR_f.TDEF)
    {
        if (NULL != pstcInst->stcIrqCb.pfnTxIrqCb)
        {
            pstcInst->stcIrqCb.pfnTxIrqCb();
        }
    }

    /* TX bus empty interrupt function. */
    if (1u == pstcInst->pstcInstance->SR_f.TBIF)
    {
        if (NULL != pstcInst->stcIrqCb.pfnTxIdleCb)
        {
            pstcInst->stcIrqCb.pfnTxIdleCb();
        }
    }

    if (1u == pstcInst->pstcInstance->EACR_f.CSER)
    {
        if (NULL != pstcInst->stcIrqCb.pfnCsErrIrqCb)
        {
            pstcInst->stcIrqCb.pfnCsErrIrqCb();
        }
        pstcInst->pstcInstance->EACR_f.CSER = 0u;
    }
}

/**
 *******************************************************************************
 ** \brief The interrupt service routine for status.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \retval None
 **
 ******************************************************************************/
void Spi_Stat_IrqHandler(uint8_t u8Channel)
{
    stc_spi_instance_data_t *pstcInst = NULL;

    pstcInst = SpiGetInternDataPtr(u8Channel);

    if (NULL == pstcInst)
    {
        return;
    }

    if (1u == pstcInst->pstcInstance->EACR_f.TIF)
    {
        if (NULL != pstcInst->stcIrqCb.pfnSerialTimerIrqCb)
        {
            pstcInst->stcIrqCb.pfnSerialTimerIrqCb();
        }
        pstcInst->pstcInstance->EACR_f.TIF = 0u;
    }
}

/**
 *******************************************************************************
 ** \brief Get spi status.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] enStatus                Spi status
 ** \arg SpiOverrunError                Spi overrun error flag
 ** \arg SpiRxFull                      Spi rx buffer full flag
 ** \arg SpiTxEmpty                     Spi tx buffer empty flag
 ** \arg SpiTxIdle                      Spi tx bus idle flag
 ** \arg SpiCsErrIntFlag                Spi cs error interrupt flag
 ** \arg SpiSerialTimerIntFlag          Spi serial timer interrupt flag
 **
 ** \retval boolean_t                   TRUE or FALSE
 **
 ******************************************************************************/
boolean_t Spi_GetStatus(uint8_t u8Channel, en_spi_status_t enStatus)
{
    volatile boolean_t bRet = FALSE;
    stc_spi_instance_data_t *pstcInst = NULL;
    M0P_MSC_SPI_TypeDef     *pstcSPI = NULL;

    ASSERT(IS_VALID_STAT(enStatus));

    pstcInst = SpiGetInternDataPtr(u8Channel);
    DEBUG_CHECK_INST(pstcInst);

    if ((NULL == pstcInst) || (NULL == pstcInst->pstcInstance))
    {
        return FALSE;
    }

    pstcSPI = pstcInst->pstcInstance;

    switch (enStatus)
    {
        case SpiOverrunError:
            bRet = pstcSPI->SR_f.ORF;
            break;
        case SpiRxFull:
            bRet = pstcSPI->SR_f.RDFF;
            break;
        case SpiTxEmpty:
            bRet = pstcSPI->SR_f.TDEF;
            break;
        case SpiTxIdle:
            bRet = pstcSPI->SR_f.TBIF;
            break;
        case SpiCsErrIntFlag:
            bRet = pstcSPI->EACR_f.CSER;
            break;
        case SpiSerialTimerIntFlag:
            bRet = pstcSPI->EACR_f.TIF;
            break;
        default:
            break;
    }

    return bRet;
}

/**
 *******************************************************************************
 ** \brief Clear spi status.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] enStatus                Spi status
 ** \arg SpiOverrunError                Spi overrun error flag
 ** \arg SpiRxFull                      Spi rx buffer full flag
 ** \arg SpiTxEmpty                     Spi tx buffer empty flag
 ** \arg SpiTxIdle                      Spi tx bus idle flag
 ** \arg SpiCsErrIntFlag                Spi cs error interrupt flag
 ** \arg SpiSerialTimerIntFlag          Spi serial timer interrupt flag
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - Get instance data fail
 **                                     - enStatus is invalid
 **
 ******************************************************************************/
en_result_t Spi_ClrStatus(uint8_t u8Channel, en_spi_status_t enStatus)
{
    en_result_t enRet = Ok;
    stc_spi_instance_data_t *pstcInst = NULL;
    M0P_MSC_SPI_TypeDef     *pstcSPI = NULL;

    ASSERT(IS_VALID_STAT(enStatus));

    pstcInst = SpiGetInternDataPtr(u8Channel);
    DEBUG_CHECK_INST(pstcInst);

    if ((NULL == pstcInst) || (NULL == pstcInst->pstcInstance))
    {
        return ErrorInvalidParameter;
    }

    pstcSPI = pstcInst->pstcInstance;

    switch (enStatus)
    {
        case SpiOverrunError:
            pstcSPI->SR_f.RECLR = TRUE;
            break;
        case SpiRxFull:
        case SpiTxEmpty:
        case SpiTxIdle:
            break;
        case SpiCsErrIntFlag:
            pstcSPI->EACR_f.CSER = FALSE;
            break;
        case SpiSerialTimerIntFlag:
            pstcSPI->EACR_f.TIF = FALSE;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Initialize a spi channel.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] pstcConfig              Spi configuration structure
 **
 ** \retval Ok                          Process successfully done
 ** \ErrorInvalidParameter              If one of following conditions are met:
 **                                     - pstcConfig is null
 **                                     - Get instance data fail
 **
 ******************************************************************************/
en_result_t Spi_Init(uint8_t u8Channel, stc_spi_config_t *pstcConfig)
{
    en_result_t enRet = Error;
    stc_spi_instance_data_t *pstcInst = NULL;
    stc_spi_irq_sel_t       *pstcIrqsel = NULL;
    M0P_MSC_SPI_TypeDef     *pstcSPI = NULL;

    /* Check parameter only in debug mode with assert. */
    ASSERT(NULL != pstcConfig);
    ASSERT(pstcConfig->u8DataOuputDly <= SPI_DATA_OUT_DEALY_MAX);
    ASSERT(pstcConfig->enDataLen < SpiDataLenMax);

    pstcInst = SpiGetInternDataPtr(u8Channel);
    DEBUG_CHECK_INST(pstcInst);

    if ((NULL == pstcInst) || (NULL == pstcInst->pstcInstance))
    {
        return ErrorInvalidParameter;
    }

    /* Check null pointer for config. */
    if (NULL == pstcConfig)
    {
        return ErrorInvalidParameter;
    }

    pstcSPI = pstcInst->pstcInstance;

    /* Reset. */
    pstcSPI->CR_f.PCL = TRUE;
    pstcSPI->MR_f.MODE = MSC_SPI_MODE;
    pstcSPI->CR_f.MSS = pstcConfig->bSlaveMode;
    pstcSPI->CR_f.SPIMODE = pstcConfig->bSPIMode;
    pstcSPI->MR_f.CINV = pstcConfig->enSckMarkLevel;
    pstcSPI->MR_f.TDS = pstcConfig->bMSB;
    pstcSPI->MR_f.CKE = pstcConfig->bOutputSCK;
    pstcSPI->ECR_f.DTRWS = pstcConfig->u8DataOuputDly;
    pstcSPI->ECR_f.SOPS = pstcConfig->bSOTHigh;
    pstcSPI->ECR_f.DL = pstcConfig->enDataLen & 0x7;
    if (pstcConfig->enDataLen >= SpiDataLen13)
    {
        pstcSPI->ECR_f.DL3 = 1;
    }

    enRet = Spi_SetBaudRate(u8Channel, pstcConfig->u32BaudRate);

    /* Interrupt setting. */
    if (NULL != pstcConfig->pstcIrqCb)
    {
        pstcInst->stcIrqCb = *pstcConfig->pstcIrqCb;
    }

    pstcIrqsel = pstcConfig->pstcIrqEn;
    if (NULL != pstcIrqsel)
    {
        SpiEnableIrq(u8Channel, SpiIrqRx, pstcIrqsel->bRxIrq);
        SpiEnableIrq(u8Channel, SpiIrqTx, pstcIrqsel->bTxIrq);
        SpiEnableIrq(u8Channel, SpiIrqTxIdle, pstcIrqsel->bTxIdleIrq);
        SpiEnableIrq(u8Channel, SpiIrqCsErr, pstcIrqsel->bCsErrIrq);
        SpiEnableIrq(u8Channel, SpiIrqSerialTimer, pstcIrqsel->bSerialTimerIrq);

        if (TRUE == pstcIrqsel->bTxIrq)
        {
            SpiEnableNvic(u8Channel, SpiIrqTx);
        }
        if (TRUE == pstcIrqsel->bRxIrq)
        {
            SpiEnableNvic(u8Channel, SpiIrqRx);
        }
    }

    enRet = Ok;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief De-initialize a spi channel.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t Spi_DeInit(uint8_t u8Channel)
{
    en_result_t enRet = Ok;

    SpiEnableIrq(u8Channel, SpiIrqTx, FALSE);
    SpiEnableIrq(u8Channel, SpiIrqRx, FALSE);
    SpiEnableIrq(u8Channel, SpiIrqTxIdle, FALSE);
    SpiEnableIrq(u8Channel, SpiIrqCsErr, FALSE);
    SpiEnableIrq(u8Channel, SpiIrqSerialTimer, FALSE);

    SpiEnableFunc(u8Channel, SpiFuncRx, FALSE);
    SpiEnableFunc(u8Channel, SpiFuncTx, FALSE);
    SpiEnableFunc(u8Channel, SPiFuncSyncTx, FALSE);
    SpiEnableFunc(u8Channel, SpiFuncCsFormat, FALSE);
    SpiEnableFunc(u8Channel, SpiFuncSerialTimer, FALSE);

    SpiDisableNvic(u8Channel);

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set spi output data baudrate.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] u32BaudRate             Spi baudrate
 **
 ** \retval Ok                          Process successfully done
 ** \ErrorInvalidParameter              If one of following conditions are met:
 **                                     - Get instance data fail
 **
 ******************************************************************************/
en_result_t Spi_SetBaudRate(uint8_t u8Channel, uint32_t u32BaudRate)
{
    stc_spi_instance_data_t *pstcInst = NULL;
    M0P_MSC_SPI_TypeDef     *pstcSPI = NULL;
    uint32_t u32Val = 0;

    pstcInst = SpiGetInternDataPtr(u8Channel);
    DEBUG_CHECK_INST(pstcInst);

    if ((NULL == pstcInst) || (NULL == pstcInst->pstcInstance))
    {
        return ErrorInvalidParameter;
    }

    pstcSPI = pstcInst->pstcInstance;

    u32Val = Clk_GetPeripheralClk(ClkPeripheralMsc);

    u32Val = u32Val / u32BaudRate;

    pstcSPI->BRS_f.BRS = u32Val - 1;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Enable spi function.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] enFunc                  Spi func
 ** \arg SpiFuncTx                      Spi tx function
 ** \arg SpiFuncRx                      Spi rx function
 ** \arg SPiFuncSyncTx                  Spi synchronous tx function
 ** \arg SpiFuncCsFormat                Spi cs format function
 ** \arg SpiFuncSerialTimer             Spi serial timer function
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - Get instance data fail
 **                                     - enFunc is invalid
 **
 ******************************************************************************/
en_result_t Spi_EnableFunc(uint8_t u8Channel, en_spi_func_sel_t enFunc)
{
    return SpiEnableFunc(u8Channel, enFunc, TRUE);
}

/**
 *******************************************************************************
 ** \brief Disable spi function.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] enFunc                  Spi func
 ** \arg SpiFuncTx                      Spi tx function
 ** \arg SpiFuncRx                      Spi rx function
 ** \arg SPiFuncSyncTx                  Spi synchronous tx function
 ** \arg SpiFuncCsFormat                Spi cs format function
 ** \arg SpiFuncSerialTimer             Spi serial timer function
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - Get instance data fail
 **                                     - enFunc is invalid
 **
 ******************************************************************************/
en_result_t Spi_DisableFunc(uint8_t u8Channel, en_spi_func_sel_t enFunc)
{
    return SpiEnableFunc(u8Channel, enFunc, FALSE);
}

/**
 *******************************************************************************
 ** \brief Enable spi interrupt request.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] enIrqSel                Interrupt request
 ** \arg SpiIrqTx                       Spi tx interrupt request
 ** \arg SpiIrqRx                       Spi rx interrupt request
 ** \arg SpiIrqTxIdle                   Spi tx bus idle interrupt request
 ** \arg SpiIrqCsErr                    Spi cs error interrupt request
 ** \arg SpiIrqSerialTimer              Spi serial timer interrupt request
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - Get instance data fail
 **                                     - enIrqSel is invalid
 **
 ******************************************************************************/
en_result_t Spi_EnableIrq(uint8_t u8Channel, en_spi_irq_sel_t enIrqSel)
{
    return SpiEnableIrq(u8Channel, enIrqSel, TRUE);
}

/**
 *******************************************************************************
 ** \brief Disable spi interrupt request.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] enIrqSel                Interrupt request
 ** \arg SpiIrqTx                       Spi tx interrupt request
 ** \arg SpiIrqRx                       Spi rx interrupt request
 ** \arg SpiIrqTxIdle                   Spi tx bus idle interrupt request
 ** \arg SpiIrqCsErr                    Spi cs error interrupt request
 ** \arg SpiIrqSerialTimer              Spi serial timer interrupt request
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - Get instance data fail
 **                                     - enIrqSel is invalid
 **
 ******************************************************************************/
en_result_t Spi_DisableIrq(uint8_t u8Channel, en_spi_irq_sel_t enIrqSel)
{
    return SpiEnableIrq(u8Channel, enIrqSel, FALSE);
}

/**
 *******************************************************************************
 ** \brief Send spi data.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] u16Data                 Data to send
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcInst is null
 **                                     - pstcInst->pstcInstance is null
 **
 ******************************************************************************/
en_result_t Spi_SendData(uint8_t u8Channel, uint16_t u16Data)
{
    stc_spi_instance_data_t *pstcInst = NULL;
    M0P_MSC_SPI_TypeDef     *pstcSPI = NULL;

    pstcInst = SpiGetInternDataPtr(u8Channel);
    DEBUG_CHECK_INST(pstcInst);

    if ((NULL == pstcInst) || (NULL == pstcInst->pstcInstance))
    {
        return ErrorInvalidParameter;
    }

    pstcSPI = pstcInst->pstcInstance;

    pstcSPI->MR_f.DOE = TRUE;
    pstcSPI->TXDR = u16Data;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Receive spi data.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] bSendSck                Send sck or not.
 ** \arg [in] TRUE                      Send sck if mode is master
 ** \arg [in] FALSE                     Do not send sck if mode is slave
 **
 ** \param [out] pu16Data               Data buffer
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcInst is null
 **                                     - pstcInst->pstcInstance is null
 **                                     - pu16Data is null
 **
 ******************************************************************************/
en_result_t Spi_ReceiveData(uint8_t u8Channel, boolean_t bSendSck, uint16_t* pu16Data)
{
    stc_spi_instance_data_t *pstcInst = NULL;
    M0P_MSC_SPI_TypeDef     *pstcSPI = NULL;

    ASSERT(NULL != pu16Data);

    pstcInst = SpiGetInternDataPtr(u8Channel);
    DEBUG_CHECK_INST(pstcInst);

    if ((NULL == pstcInst) || (NULL == pstcInst->pstcInstance))
    {
        return ErrorInvalidParameter;
    }

    if (NULL == pu16Data)
    {
        return ErrorInvalidParameter;
    }

    pstcSPI = pstcInst->pstcInstance;

    pstcSPI->MR_f.DOE = FALSE;

    /* Send Sck. */
    if (TRUE == bSendSck)
    {
        pstcSPI->TXDR = 0x0;
    }

    *pu16Data = pstcSPI->RXDR;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Set spi serial timer.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] enDiv                   Clock prescaler
 ** \arg SpiClkDiv1                     Serial Timer clock = PCLK
 ** \arg SpiClkDiv2                     Serial Timer clock = PCLK/2
 ** \arg SpiClkDiv4                     Serial Timer clock = PCLK/4
 ** \arg SpiClkDiv8                     Serial Timer clock = PCLK/8
 ** \arg SpiClkDiv16                    Serial Timer clock = PCLK/16
 ** \arg SpiClkDiv32                    Serial Timer clock = PCLK/32
 ** \arg SpiClkDiv64                    Serial Timer clock = PCLK/64
 ** \arg SpiClkDiv128                   Serial Timer clock = PCLK/128
 ** \arg SpiClkDiv256                   Serial Timer clock = PCLK/256
 **
 ** \param [in] u16Val                  Time count
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcInst is null
 **                                     - pstcInst->pstcInstance is null
 **
 ******************************************************************************/
en_result_t Spi_SerialTimerConfig(uint8_t u8Channel,
                                  en_spi_clk_div_t enDiv,
                                  uint16_t u16Val)
{
    stc_spi_instance_data_t *pstcInst = NULL;
    M0P_MSC_SPI_TypeDef     *pstcSPI = NULL;

    ASSERT(enDiv < SpiClkDivMax);

    pstcInst = SpiGetInternDataPtr(u8Channel);
    DEBUG_CHECK_INST(pstcInst);

    if ((NULL == pstcInst) || (NULL == pstcInst->pstcInstance))
    {
        return ErrorInvalidParameter;
    }

    pstcSPI = pstcInst->pstcInstance;

    pstcSPI->TMC_f.TMC = u16Val;
    pstcSPI->EACR_f.CLKDIV = enDiv;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Set tx byte for cs/sync mode.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] u8Cspin                 Cs pin index
 **
 ** \param [in] u8Val                   The value of tx byte
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - u8Cspin is invalid
 **                                     - pstcInst is null
 **                                     - pstcInst->pstcInstance is null
 **
 ******************************************************************************/
en_result_t Spi_SetTxBytes(uint8_t u8Channel, uint8_t u8Cspin, uint8_t u8Val)
{
    stc_spi_instance_data_t *pstcInst = NULL;
    M0P_MSC_SPI_TypeDef     *pstcSPI = NULL;
    en_result_t enRet = Ok;

    ASSERT(u8Cspin < SpiCsPinMax);

    pstcInst = SpiGetInternDataPtr(u8Channel);
    DEBUG_CHECK_INST(pstcInst);

    if ((NULL == pstcInst) || (NULL == pstcInst->pstcInstance))
    {
        return ErrorInvalidParameter;
    }

    pstcSPI = pstcInst->pstcInstance;

    switch (u8Cspin)
    {
        case SpiCsPin0:
            pstcSPI->TDTCNT0 = u8Val;
            break;
        case SpiCsPin1:
            pstcSPI->TDTCNT1 = u8Val;
            break;
        case SpiCsPin2:
            pstcSPI->TDTCNT2 = u8Val;
            break;
        case SpiCsPin3:
            pstcSPI->TDTCNT3 = u8Val;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set chip selection.
 **
 ** \param [in] u8Channel               Spi channel
 ** \arg SpiCh4                         Spi channel 4
 ** \arg SpiCh5                         Spi channel 5
 ** \arg SpiCh6                         Spi channel 6
 **
 ** \param [in] pstcConfig              Spi chip selection configuration structure
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcConfig is null
 **                                     - pstcInst is null
 **                                     - pstcInst->pstcInstance is null
 **
 ******************************************************************************/
en_result_t Spi_CSCconfig(uint8_t u8Channel, stc_spi_cs_config_t *pstcConfig)
{
    stc_spi_instance_data_t *pstcInst = NULL;
    M0P_MSC_SPI_TypeDef     *pstcSPI = NULL;

    ASSERT(NULL != pstcConfig);
    ASSERT(pstcConfig->enCsStartPin < SpiCsPinMax);
    ASSERT(pstcConfig->enCsEndPin < SpiCsPinMax);
    ASSERT(pstcConfig->enCsClkDiv <= SpiClkDiv64);

    pstcInst = SpiGetInternDataPtr(u8Channel);
    DEBUG_CHECK_INST(pstcInst);

    if ((NULL == pstcInst) || (NULL == pstcInst->pstcInstance))
    {
        return ErrorInvalidParameter;
    }

    /* Check null pointer for config. */
    if (NULL == pstcConfig)
    {
        return ErrorInvalidParameter;
    }

    pstcSPI = pstcInst->pstcInstance;

    pstcSPI->CSCR_f.CSAS = pstcConfig->enCsStartPin;

    /* Pin error. */
    pstcSPI->CSCR_f.CSAE = pstcConfig->enCsEndPin;

    pstcSPI->CSCR_f.CSAH = pstcConfig->bActiveHold;
    pstcSPI->CSCR_f.CSTODIV = pstcConfig->enCsClkDiv;

    /* CSLVS. */
    pstcSPI->CSCR_f.CSLVS = pstcConfig->bCsIdleLow;

    pstcSPI->CSCR_f.CSOE = pstcConfig->bCsOutputEn;

    pstcSPI->CSTMG1 = pstcConfig->u8CsDelayTime;
    pstcSPI->CSTMG0 = pstcConfig->u8CsHoldTime;
    pstcSPI->CSTMG32 = pstcConfig->u16CsDeselectTime;

    /* Each pin setting. */
    pstcSPI->CSCR_f.CSEN0 = pstcConfig->stcCsPincfg[0].bEn;
    pstcSPI->CSCR_f.CSEN1 = pstcConfig->stcCsPincfg[1].bEn;
    pstcSPI->CSCR_f.CSEN2 = pstcConfig->stcCsPincfg[2].bEn;
    pstcSPI->CSCR_f.CSEN3 = pstcConfig->stcCsPincfg[3].bEn;

    /* Inactive. */
    pstcSPI->CSFR0_f.CS1CSLVS = pstcConfig->stcCsPincfg[0].bCsInActiveHigh;
    pstcSPI->CSFR0_f.CS1CINV = pstcConfig->stcCsPincfg[0].bSckInv;
    pstcSPI->CSFR0_f.CS1SPI = pstcConfig->stcCsPincfg[0].bSPIMode;
    pstcSPI->CSFR0_f.CS1TDS = pstcConfig->stcCsPincfg[0].bMSB;
    pstcSPI->CSFR0_f.CS1DL = pstcConfig->stcCsPincfg[0].enDataLen;

    pstcSPI->CSFR1_f.CS2CSLVS = pstcConfig->stcCsPincfg[1].bCsInActiveHigh;
    pstcSPI->CSFR1_f.CS2CINV = pstcConfig->stcCsPincfg[1].bSckInv;
    pstcSPI->CSFR1_f.CS2SPI = pstcConfig->stcCsPincfg[1].bSPIMode;
    pstcSPI->CSFR1_f.CS2TDS = pstcConfig->stcCsPincfg[1].bMSB;
    pstcSPI->CSFR1_f.CS2DL = pstcConfig->stcCsPincfg[1].enDataLen;

    pstcSPI->CSFR2_f.CS3CSLVS = pstcConfig->stcCsPincfg[2].bCsInActiveHigh;
    pstcSPI->CSFR2_f.CS3CINV = pstcConfig->stcCsPincfg[2].bSckInv;
    pstcSPI->CSFR2_f.CS3SPI = pstcConfig->stcCsPincfg[2].bSPIMode;
    pstcSPI->CSFR2_f.CS3TDS = pstcConfig->stcCsPincfg[2].bMSB;
    pstcSPI->CSFR2_f.CS3DL = pstcConfig->stcCsPincfg[2].enDataLen;

    return Ok;
}

//@} // SpiGroup
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
