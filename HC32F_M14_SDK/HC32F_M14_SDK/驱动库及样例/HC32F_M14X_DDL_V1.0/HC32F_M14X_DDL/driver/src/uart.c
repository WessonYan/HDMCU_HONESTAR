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
/** \file uart.c
 **
 ** A detailed description is available at
 ** @link UartGroup Uart @endlink
 **
 **   - 2017-08-07  1.0  HeChun First version for Device Driver Library of uart.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "uart.h"

/**
 *******************************************************************************
 ** \addtogroup UartGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \breif UART instance data type
 ******************************************************************************/
typedef struct stc_uart_instance_data
{
    uint32_t                    u32Idx;               /*channel index*/
    M0P_MSC_UART_TypeDef       *pstcInstance;         /*pointer to registers of an instance*/
    stc_uart_irq_cb_t           stcUartInternIrqCb;   /*Uart internal interrupt callback function*/
} stc_uart_instance_data_t;

/**
 *******************************************************************************
 ** \breif UART structure of each channel
 ******************************************************************************/
static stc_uart_instance_data_t m_astcUartInstanceDataLut[] =
{
    {
        UARTCH4,
        M0P_MSC4_UART,       /* pstcInstance */
        {NULL,NULL,NULL,NULL}
    },
    {
        UARTCH5,
        M0P_MSC5_UART,       /* pstcInstance */
        {NULL,NULL,NULL,NULL}
    },
    {
        UARTCH6,
        M0P_MSC6_UART,       /* pstcInstance */
        {NULL,NULL,NULL,NULL}
    },
};

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_EN(x)                  \
(   (TRUE == (x)) ||                    \
    (FALSE == (x)) )
#define IS_VALID_CH(x)                  \
(   (UARTCH4 == (x)) ||                 \
    (UARTCH5 == (x)) ||                 \
    (UARTCH6 == (x)) )
#define IS_VALID_DSCH(x)                \
(   (UARTCH4 == (x)) ||                 \
    (UARTCH5 == (x)) ||                 \
    (UARTCH6 == (x)) )
#define IS_VALID_IRQSEL(x)              \
(   (UartTxIrq <= (x)) &&               \
    (UartTxIdleIrq >= (x)) )
#define IS_VALID_FUNC(x)                \
(   (UartTx == (x)) ||                  \
    (UartRx == (x)) )
#define IS_VALID_STATUS(x)              \
(   (UartParityError <= (x)) &&         \
    (UartTxIdle >= (x)) )

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static stc_uart_instance_data_t* UartGetInternDataPtr(uint8_t u8Idx);


/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 ******************************************************************************
 ** \brief Device dependent initialization of transmit interrupts
 **
 ** \param u8Idx  UART channel
 ** \param bRxTx  FALSE: RX IRQ, TRUE: TX IRQ
 **
 ******************************************************************************/
static void UartInitNvic(uint8_t u8Idx, boolean_t bRxTx)
{
    IRQn_Type enIrqIndex;

    /* check the input param */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_EN(bRxTx));

    /* channel 4-6 */
    if((UARTCH4 <= u8Idx) && (UARTCH6 >= u8Idx))
    {
        if(FALSE == bRxTx)
        {
            enIrqIndex = (IRQn_Type)(MSC4RX_IRQn + (2*(u8Idx - UARTCH4)));
        }
        else
        {
            enIrqIndex = (IRQn_Type)(MSC4TX_IRQn + (2*(u8Idx - UARTCH4)));
        }
    }

    NVIC_ClearPendingIRQ(enIrqIndex);
    switch(u8Idx)
    {
        case UARTCH4:
        {
            if(FALSE == bRxTx)
            {
                NVIC_SetPriority(enIrqIndex, DDL_IRQ_LEVEL_MSC4RX);
            }
            else
            {
                NVIC_SetPriority(enIrqIndex, DDL_IRQ_LEVEL_MSC4TX);
            }
            break;
        }
        case UARTCH5:
        {
            if(FALSE == bRxTx)
            {
                NVIC_SetPriority(enIrqIndex, DDL_IRQ_LEVEL_MSC5RX);
            }
            else
            {
                NVIC_SetPriority(enIrqIndex, DDL_IRQ_LEVEL_MSC5TX);
            }
            break;
        }
        case UARTCH6:
        {
            if(FALSE == bRxTx)
            {
                NVIC_SetPriority(enIrqIndex, DDL_IRQ_LEVEL_MSC6RX);
            }
            else
            {
                NVIC_SetPriority(enIrqIndex, DDL_IRQ_LEVEL_MSC6TX);
            }
            break;
        }
        default:
            break;
    }
    NVIC_EnableIRQ(enIrqIndex);
}

/**
 *******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS
 **
 ** \param u8Idx   UART channel
 ** \param bRxTx   FALSE: RX IRQ, TRUE: TX IRQ
 **
 ******************************************************************************/
static void UartDeInitNvic(uint8_t u8Idx, boolean_t bRxTx)
{
    IRQn_Type enIrqIndex;

    /* check the input param */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_EN(bRxTx));

    /*channel 4-6 */
    if((UARTCH4 <= u8Idx) && (UARTCH6 >= u8Idx))
    {
        if(FALSE == bRxTx)
        {
            enIrqIndex = (IRQn_Type)(MSC4RX_IRQn + (2*(u8Idx - UARTCH4)));
        }
        else
        {
            enIrqIndex = (IRQn_Type)(MSC4TX_IRQn + (2*(u8Idx - UARTCH4)));
        }
    }

    NVIC_ClearPendingIRQ(enIrqIndex);
    NVIC_DisableIRQ(enIrqIndex);
    NVIC_SetPriority(enIrqIndex, DDL_IRQ_LEVEL_DEFAULT);

}

/**
 *******************************************************************************
 ** \brief Return the internal data for a certain UART instance.
 **
 ** \param pstcUart Pointer to UART instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_uart_instance_data_t* UartGetInternDataPtr(uint8_t u8Idx)
{
    stc_uart_instance_data_t* pstcData = NULL;
    uint8_t i = 0;

    /* Get ptr to internal data struct ... */
    for (i = 0; i < ARRAY_SZ(m_astcUartInstanceDataLut); i++)
    {
        if (u8Idx == m_astcUartInstanceDataLut[i].u32Idx)
        {
            pstcData = &m_astcUartInstanceDataLut[i];
            break;
        }
    }

    return (pstcData);
}

/**
 ******************************************************************************
 ** \brief UART receive interrupt service routine.
 **
 ** \param u8Idx    UART channel
 **
 ** This function is called on Receive Interrupt set by the UART.
 **
 ******************************************************************************/
void Uart_Rx_IrqHandler(uint8_t u8Idx)
{
    stc_uart_instance_data_t *pstcData = NULL;

    /* check the input param */
    ASSERT(IS_VALID_CH(u8Idx));

    /*get Corresponding channel address */
    pstcData = UartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return;
    }

    /* Exception handling */
    if((1u == pstcData->pstcInstance->SR_f.ORF) ||
       (1u == pstcData->pstcInstance->SR_f.FEF) ||
       (1u == pstcData->pstcInstance->SR_f.PEF))
    {
        if(NULL != pstcData->stcUartInternIrqCb.pfnRxErrIrqCb)
        {
            pstcData->stcUartInternIrqCb.pfnRxErrIrqCb();
        }
    }

    /* Normal case */
    if(1u == pstcData->pstcInstance->SR_f.RDFF)
    {
        if(NULL != pstcData->stcUartInternIrqCb.pfnRxIrqCb)
        {
            pstcData->stcUartInternIrqCb.pfnRxIrqCb();
        }
    }

}

/**
 *******************************************************************************
 ** \brief UART transmit interrupt service routine.
 **
 ** \param u8Idx    UART channel
 **
 ** This function is called on Transmit Interrupt set by the UART.
 **
 ******************************************************************************/
void Uart_Tx_IrqHandler(uint8_t u8Idx)
{
    stc_uart_instance_data_t *pstcData = NULL;

    /* check the input param */
    ASSERT(IS_VALID_CH(u8Idx));

    /* get Corresponding channel address */
    pstcData = UartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return;
    }

    /* TXDR empty interrupt funciton */
    if(1u == pstcData->pstcInstance->SR_f.TDEF)
    {
        if(NULL != pstcData->stcUartInternIrqCb.pfnTxIrqCb)
        {
            pstcData->stcUartInternIrqCb.pfnTxIrqCb();
        }
    }

    /* uart send bus empty interrupt funciton */
    if(1u == pstcData->pstcInstance->SR_f.TBIF)
    {
        if(NULL != pstcData->stcUartInternIrqCb.pfnTxIdleCb)
        {
            pstcData->stcUartInternIrqCb.pfnTxIdleCb();
        }
    }
}

/**
 *******************************************************************************
 ** \brief Enable one interrupt source of UART
 **
 ** \param u8Idx           UART channel
 ** \param [in] enIrqSel   Pointer to the selection structure of UART interrupt
 ** \arg UartTxIrq         UART TX interrupt
 ** \arg UartRxIrq         UART RX interrupt
 ** \arg UartTxIdleIrq     UART TX idle interrupt
 **
 ** \retval Ok                    Interrupts has been enabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcUart == NULL
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Uart_EnableIrq(uint8_t u8Idx,
                           en_uart_irq_sel_t enIrqSel)
{
    stc_uart_instance_data_t *pstcData = NULL;

    /* check the input param */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_IRQSEL(enIrqSel));

    /* get Corresponding channel address */
    pstcData = UartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    /* enable be related to interrupt */
    switch (enIrqSel)
    {
        case UartTxIrq:
            pstcData->pstcInstance->CR_f.TXIE = 1u;
            break;
        case UartRxIrq:
            pstcData->pstcInstance->CR_f.RXIE = 1u;
            break;
        case UartTxIdleIrq:
            pstcData->pstcInstance->CR_f.TBIE = 1u;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable one interrupt source of UART
 **
 ** \param u8Idx           UART channel
 ** \param [in] enIrqSel   Pointer to the selection structure of UART interrupt
 ** \arg UartTxIrq         UART TX interrupt
 ** \arg UartRxIrq         UART RX interrupt
 ** \arg UartTxIdleIrq     UART TX idle interrupt
 **
 ** \retval Ok                    Interrupts has been disabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcUart == NULL
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Uart_DisableIrq(uint8_t u8Idx,
                            en_uart_irq_sel_t enIrqSel)
{
    stc_uart_instance_data_t *pstcData = NULL;

    /* check the input param */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_IRQSEL(enIrqSel));

    /* get Corresponding channel address */
    pstcData = UartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    /* disable be related to interrupt */
    switch (enIrqSel)
    {
        case UartTxIrq:
            pstcData->pstcInstance->CR_f.TXIE = 0u;
            break;
        case UartRxIrq:
            pstcData->pstcInstance->CR_f.RXIE = 0u;
            break;
        case UartTxIdleIrq:
            pstcData->pstcInstance->CR_f.TBIE = 0u;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Set the baudrate of UART
 **
 ** \param [in]   u8Idx           UART channel
 ** \param [in] u32BaudRate Baudrate value [bps]
 **
 ** \retval Ok                    UART baud rate has been successfully modified
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcUart == NULL
 **                               - u32BaudRate < 4
 ** \note
 ** The UART baud rate can be initialized in the Uart_Init() and be modified
 ** in the funciton.
 **
 ******************************************************************************/
en_result_t Uart_SetBaudRate(uint8_t u8Idx,
                             uint32_t u32BaudRate)
{
    uint32_t u32Pclk1;
    stc_uart_instance_data_t *pstcData = NULL;

    /* check the input param */
    ASSERT(IS_VALID_CH(u8Idx));

    /* get Corresponding channel address */
    pstcData = UartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    /* get current clock */
    u32Pclk1 = Clk_GetPeripheralClk(ClkPeripheralMsc);

    /* set baud rate */
    pstcData->pstcInstance->BRS_f.BRS = (u32Pclk1/u32BaudRate) - 1;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Initialization of a UART module.
 **
 ** Set registers to active UART.
 **
 ** \param [in]   u8Idx           UART channel
 ** \param [in]   pstcConfig      MFS UART configuration
 **
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcUart == NULL
 **             - pstcConfig == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit)
 **
 ******************************************************************************/
en_result_t Uart_Init(uint8_t u8Idx,
                      stc_uart_config_t* pstcConfig)
{
    stc_uart_instance_data_t *pstcData = NULL;

    /* Preset local register variables to zero */
    stc_msc_uart_mr_field_t  stcMR  = { 0u };
    stc_msc_uart_cr_field_t  stcCR  = { 0u };
    stc_msc_uart_ecr_field_t stcECR = { 0u };
    stc_msc_uart_comp_field_t stcCOMP = { 0u };

    /* check the input param */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_EN(pstcConfig->bInvertData));
    ASSERT(IS_VALID_EN(pstcConfig->bHwFlow));
    ASSERT(IS_VALID_EN(pstcConfig->bUseExtClk));
    ASSERT(IS_VALID_EN(pstcConfig->bCarryEnable));
    ASSERT(IS_VALID_EN(pstcConfig->bCarryPolarity));
    ASSERT(IS_VALID_EN(pstcConfig->bCompensation));
    ASSERT(IS_VALID_EN(pstcConfig->bIrFunc));
    ASSERT(IS_VALID_EN(pstcConfig->bTouchNvic));

    /* get Corresponding channel address */
    pstcData = UartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    /* Check for valid pointer and get pointer to internal data struct ... */
    /* Parameter check and get ptr to internal data struct */
    if(NULL == pstcConfig)
    {
        return (ErrorInvalidParameter);
    }

    /* Set the UART mode in the internal structure */
    /* Set UART function mode */
    switch (pstcConfig->enMode)
    {
        case UartNormal:
            /* Normal mode */
            stcMR.MODE = 0u;
            break;
        case UartMulti:
            /* Multi-processor mode */
            stcMR.MODE = 1u;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* Enable SOE */
    stcMR.DOE = TRUE;

    /* Set Parity */
    switch(pstcConfig->enParity)
    {
        case UartParityNone:
            /* Parity type selection (0-even/1-odd) */
            stcECR.PS = FALSE;
            /* Parity disable */
            stcECR.PE = FALSE;
            break;
        case UartParityEven:
            /* Parity type selection (0-even/1-odd) */
            stcECR.PS = FALSE;
            /* Parity Enable */
            stcECR.PE = TRUE;
            break;
        case UartParityOdd:
            /* Parity type selection (0-even/1-odd) */
            stcECR.PS = TRUE;
            /* Parity Enable */
            stcECR.PE = TRUE;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* Set Stop bit length */
    switch (pstcConfig->enStopBit)
    {
        case UartOneStopBit:
            stcMR.SBLS   = FALSE;
            stcECR.ESBLS = FALSE;
            break;
        case UartTwoStopBits:
            stcMR.SBLS   = TRUE;
            stcECR.ESBLS = FALSE;
            break;
        case UartThreeStopBits:
            stcMR.SBLS   = FALSE;
            stcECR.ESBLS = TRUE;
            break;
        case UartFourStopBits:
            stcMR.SBLS   = TRUE;
            stcECR.ESBLS = TRUE;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* Set Data bit length (5 - 9bits) */
    switch(pstcConfig->enDataLength)
    {
        case UartFiveBits:
            stcECR.DL = UartFiveBits;
            break;
        case UartSixBits:
            stcECR.DL = UartSixBits;
            break;
        case UartSevenBits:
            stcECR.DL = UartSevenBits;
            break;
        case UartEightBits:
            stcECR.DL = UartEightBits;
            break;
        case UartNineBits:
            stcECR.DL = UartNineBits;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* Set Bit direction (LSB/MSB) */
    switch(pstcConfig->enBitDirection)
    {
        case UartDataLsbFirst:
            stcMR.TDS = 0;
            break;
        case UartDataMsbFirst:
            stcMR.TDS = 1;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* HW Flow */
    if (TRUE == pstcConfig->bHwFlow)
    {
        stcECR.FCEB = TRUE;
    }
    else
    {
        stcECR.FCEB = FALSE;
    }

    /* Set Signal system (NRZ/Int-NRZ) */
    if (TRUE == pstcConfig->bInvertData)
    {
        stcECR.INV = TRUE;
    }
    else
    {
        stcECR.INV = FALSE;
    }

    if((UARTCH4 <= u8Idx) && (UARTCH6 >= u8Idx))
    {
        /* Compensation configuration */
        if(TRUE == pstcConfig->bCompensation)
        {
            stcCOMP.CE = TRUE;
            stcCOMP.COMP = pstcConfig->u16CompenConf;
        }
        else
        {
            stcCOMP.CE = FALSE;
        }
    }

    /* Clear UART by setting the Software Reset bit */
    pstcData->pstcInstance->CR_f.PCL = TRUE;

    /* Set Baudrate */
    (void)Uart_SetBaudRate(u8Idx, pstcConfig->u32BaudRate);

    /* Set registers value */
    pstcData->pstcInstance->MR_f  = stcMR;
    pstcData->pstcInstance->CR_f  = stcCR;
    pstcData->pstcInstance->ECR_f = stcECR;

    if((UARTCH4 <= u8Idx) && (UARTCH6 >= u8Idx))
    {
        pstcData->pstcInstance->COMP_f = stcCOMP;
    }

    /* Set external clock */
    pstcData->pstcInstance->BRS_f.EXT = ((pstcConfig->bUseExtClk == TRUE) ? 1u : 0u);

    /* Configue interrupts */
    if(NULL != pstcConfig->pstcIrqEn)
    {
        if(TRUE == pstcConfig->pstcIrqEn->bRxIrq)
        {
            pstcData->pstcInstance->CR_f.RXIE = 1u;
        }

        if(TRUE == pstcConfig->pstcIrqEn->bTxIrq)
        {
            pstcData->pstcInstance->CR_f.TXIE = 1u;
        }

        if(TRUE == pstcConfig->pstcIrqEn->bTxIdleIrq)
        {
            pstcData->pstcInstance->CR_f.TBIE = 1u;
        }
    }

    /* Configure callback functions */
    if(NULL != pstcConfig->pstcIrqCb)
    {
        pstcData->stcUartInternIrqCb.pfnRxIrqCb = pstcConfig->pstcIrqCb->pfnRxIrqCb;
        pstcData->stcUartInternIrqCb.pfnTxIrqCb = pstcConfig->pstcIrqCb->pfnTxIrqCb;
        pstcData->stcUartInternIrqCb.pfnTxIdleCb = pstcConfig->pstcIrqCb->pfnTxIdleCb;
        pstcData->stcUartInternIrqCb.pfnRxErrIrqCb = pstcConfig->pstcIrqCb->pfnRxErrIrqCb;
    }

    /* Configure NVIC */
    if(TRUE == pstcConfig->bTouchNvic)
    {
        UartInitNvic(u8Idx, TRUE);
        UartInitNvic(u8Idx, FALSE);
    }

    return (Ok);
}

/**
 *******************************************************************************
 ** \brief Deinitialisation of a UART module.
 **
 ** All used uart register are reset to their default values.
 **
 ** \param [in]   u8Idx         UART channel
 ** \param [in]   bTouchNvic    Touch NVIC or not
 **
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - u8Idx undefined
 **
 ******************************************************************************/
en_result_t Uart_DeInit(uint8_t u8Idx, boolean_t bTouchNvic)
{
    stc_uart_instance_data_t *pstcData = NULL;

    /* check the input param */
    ASSERT(IS_VALID_CH(u8Idx));

    /* get Corresponding channel address */
    pstcData = UartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    pstcData->pstcInstance->CR_f.TXE = FALSE;
    pstcData->pstcInstance->CR_f.RXE = FALSE;

    /* Baud Rate Generation Reload Reset */
    pstcData->pstcInstance->BRS = 0u;

    /* Serial Mode Register clear all bits (valid in any mode) */
    pstcData->pstcInstance->MR = 0u;

    /* Reset Mfs receive and transmit bit to default */
    /* and clear all error flags */
    pstcData->pstcInstance->CR = 0u;

    /* Clear reception Errors */
    pstcData->pstcInstance->SR_f.RECLR = TRUE;

    /* Reset all other used register to default value */
    pstcData->pstcInstance->SR  = 0u;
    pstcData->pstcInstance->ECR = 0u;

    /* Clear MFS by setting the Software Reset bit */
    pstcData->pstcInstance->CR_f.PCL = TRUE;

    if((UARTCH4 <= u8Idx) && (UARTCH6 >= u8Idx))
    {
        pstcData->pstcInstance->COMP = 0u;
    }

    /* Disable NVIC */
    if(TRUE == bTouchNvic)
    {
        UartDeInitNvic(u8Idx, TRUE);
        UartDeInitNvic(u8Idx, FALSE);
    }

    return (Ok);
}

/**
 *******************************************************************************
 ** \brief Enable UART functions
 **
 ** \param [in] u8Idx      UART channel
 ** \param [in] enFunc     UART function types
 ** \arg   UartTx    UART transfer function
 ** \arg   UartRx    UART receive function
 **
 ** \retval Ok                    Function has been enabled normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - u8Idx undefined
 **
 ******************************************************************************/
en_result_t Uart_EnableFunc(uint8_t u8Idx, en_uart_func_t enFunc)
{
    stc_uart_instance_data_t *pstcData = NULL;

    /* check the input param */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_FUNC(enFunc));

    /* get Corresponding channel address */
    pstcData = UartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    /* enable interrupt */
    switch(enFunc)
    {
        case UartTx:
            pstcData->pstcInstance->CR_f.TXE = 1u;
            break;
        case UartRx:
            pstcData->pstcInstance->CR_f.RXE = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable UART functions
 **
 ** \param [in] u8Idx      UART channel
 ** \param [in] enFunc     UART function types
 ** \arg   UartTx    UART transfer function
 ** \arg   UartRx    UART receive function
 **
 ** \retval Ok                    Function has been disabled normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - u8Idx undefined
 **
 ******************************************************************************/
en_result_t Uart_DisableFunc(uint8_t u8Idx, en_uart_func_t enFunc)
{
    stc_uart_instance_data_t *pstcData = NULL;

    /* check the input param */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_FUNC(enFunc));

    /* get Corresponding channel address */
    pstcData = UartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    /* disable interrupt */
    switch(enFunc)
    {
        case UartTx:
            pstcData->pstcInstance->CR_f.TXE = 0u;
            break;
        case UartRx:
            pstcData->pstcInstance->CR_f.RXE = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get status of UART according to status type
 **
 ** \param [in] u8Idx        UART channel
 ** \param [in] enStatus     UART status type
 ** \arg   UartParityError   UART parity error
 ** \arg   UartFrameError    UART frame error
 ** \arg   UartOverrunError  UART overrun error
 ** \arg   UartRxFull        UART receive buffer full
 ** \arg   UartTxEmpty       UART tranfer buffer empty
 ** \arg   UartTxIdle        UART tranfer idle status
 **
 ** \retval FALSE            If one of following conditions are met:
 **                          - No UART parity error [enStatus = UartParityError]
 **                          - No UART frame error  [enStatus = UartFrameError]
 **                          - UART receive buffer is not full [enStatus = UartRxFull]
 **                          - UART tranfer buffer is not empty [enStatus = UartTxEmpty]
 **                          - UART tranfer is on-going [enStatus = UartTxIdle]
 ** \retval TRUE             If one of following conditions are met:
 **                          - UART parity error occurs [enStatus = UartParityError]
 **                          - UART frame error occurs  [enStatus = UartFrameError]
 **                          - UART receive buffer is full [enStatus = UartRxFull]
 **                          - UART tranfer buffer is empty [enStatus = UartTxEmpty]
 **                          - UART tranfer is idle [enStatus = UartTxIdle]
 **
 ******************************************************************************/
boolean_t Uart_GetStatus(uint8_t u8Idx,
                         en_uart_status_t enStatus)
{
    boolean_t bResult = FALSE;
    stc_uart_instance_data_t *pstcData = NULL;

    /* check the input param */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_STATUS(enStatus));

    /* get Corresponding channel address */
    pstcData = UartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    /* get the status of uart */
    switch(enStatus)
    {
        case UartParityError:
            bResult = (pstcData->pstcInstance->SR_f.PEF == 1u) ? TRUE : FALSE;
            break;
        case UartFrameError:
            bResult = (pstcData->pstcInstance->SR_f.FEF == 1u) ? TRUE : FALSE;
            break;
        case UartOverrunError:
            bResult = (pstcData->pstcInstance->SR_f.ORF == 1u) ? TRUE : FALSE;
            break;
        case UartRxFull:
            bResult = (pstcData->pstcInstance->SR_f.RDFF == 1u) ? TRUE : FALSE;
            break;
        case UartTxEmpty:
            bResult = (pstcData->pstcInstance->SR_f.TDEF == 1u) ? TRUE : FALSE;
            break;
        case UartTxIdle:
            bResult = (pstcData->pstcInstance->SR_f.TBIF == 1u) ? TRUE : FALSE;
            break;
        default:
            break;
    }

    return bResult;
}

/**
 *******************************************************************************
 ** \brief Clear status of UART according to status type
 **
 ** \param [in] u8Idx        UART channel
 ** \param [in] enStatus     UART status type
 ** \arg   UartParityError   UART parity error
 ** \arg   UartFrameError    UART frame error
 ** \arg   UartOverrunError  UART overrun error
 ** \arg   UartRxFull        UART receive buffer full
 ** \arg   UartTxEmpty       UART tranfer buffer empty
 ** \arg   UartTxIdle        UART tranfer idle status
 **
 ** \retval Ok                    Status has been cleared normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **             - u8Idx undefined
 ** \note  The following status can only be cleared by hardware behavior:
 **        - UartRxFull
 **        - UartTxEmpty
 **        - UartTxIdle
 **
 ******************************************************************************/
en_result_t Uart_ClrStatus(uint8_t u8Idx,
                           en_uart_status_t enStatus)
{
    stc_uart_instance_data_t *pstcData = NULL;

    /* check the input param */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_STATUS(enStatus));

    /* get Corresponding channel address */
    pstcData = UartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    /* clear the status */
    switch(enStatus)
    {
        case UartParityError:
        case UartFrameError:
        case UartOverrunError:
            pstcData->pstcInstance->SR_f.RECLR = 1u;
            break;
        case UartRxFull:
        case UartTxEmpty:
        case UartTxIdle:
            break;
        default:
            return ErrorInvalidParameter;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Write UART data buffer
 **
 ** \param [in] u8Idx      UART channel
 ** \param [in] u16Data    Send data
 **
 ** \retval Ok                    Data has been successfully sent
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcData == NULL
 **
 ******************************************************************************/
en_result_t Uart_SendData(uint8_t u8Idx, uint16_t u16Data)
{
    stc_uart_instance_data_t *pstcData = NULL;

    /* get Corresponding channel address */
    pstcData = UartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    /* Transmit Data */
    pstcData->pstcInstance->TXDR = u16Data;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Read UART data buffer
 **
 ** \param [in]  u8Idx      UART channel
 ** \param [out] pu16Data  Read data
 **
 ** \retval Ok                    Data has been successfully received
 **         ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcData == NULL
 **         Received data
 **
 ******************************************************************************/
en_result_t Uart_ReceiveData(uint8_t u8Idx,uint16_t *pu16Data)
{
    stc_uart_instance_data_t *pstcData = NULL;

    ASSERT(IS_VALID_CH(u8Idx));
    /* get Corresponding channel address */
    pstcData = UartGetInternDataPtr(u8Idx);

    if(NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }

    *pu16Data = (pstcData->pstcInstance->RXDR)&(uint16_t)0x1FF;
    /* Receive Data */
    return Ok;
}

//@} // UartGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
