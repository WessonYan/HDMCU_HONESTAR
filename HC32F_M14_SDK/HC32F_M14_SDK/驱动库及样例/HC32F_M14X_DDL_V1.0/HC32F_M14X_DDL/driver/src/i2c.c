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

/******************************************************************************/
/** \file i2c.c
 **
 ** A detailed description is available at
 ** @link I2cGroup I2c description @endlink
 **
 **   - 2017-08-10  1.0  WangM First version for Device Driver Library of i2c.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "i2c.h"

/**
 *******************************************************************************
 ** \addtogroup I2cGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief I2C instance data structure define
 ******************************************************************************/
typedef struct stc_i2c_instance_data
{
    uint8_t u8Idx;                      ///< Channel
    volatile stc_mscn_i2c_t* pstcInstance;///< pointer to registers of an instance
    stc_i2c_irq_cb_t stcI2cInternIrqCb; ///< I2C internal interrupt callback function
} stc_i2c_instance_data_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
#define MSC_I2C_MODE        4
#define I2CMAXBPS           400000ul
#define I2C_INSTANCE_COUNT  3

#define IS_VALID_POINTER(x)         (NULL != (x))

#define IS_VALID_CH(x)                  \
(   I2CCH4 == (x) ||                    \
    I2CCH5 == (x) ||                    \
    I2CCH6 == (x))

#define IS_VALID_IRQ(x)                 \
(   I2cTxIrq == (x) ||                  \
    I2cRxIrq == (x) ||                  \
    I2cTxRxIrq == (x) ||                \
    I2cStopDetectIrq == (x))

#define IS_VALID_STAT(x)                \
(   I2cOverrunError == (x) ||           \
    I2cRxFull == (x) ||                 \
    I2cTxEmpty == (x) ||                \
    I2cReservedByteDetect == (x) ||     \
    I2cStopDetect == (x) ||             \
    I2cBusStatus == (x) ||              \
    I2cBusErr == (x) ||                 \
    I2cRxTxIrq == (x) ||                \
    I2cDevAddrMatch == (x))

#define IS_VALID_IRQTYPE(x)             \
(   I2cIrqRxEn == (x) ||                \
    I2cIrqTxEn == (x))

#define IS_VALID_I2CMODE(x)             \
(   I2cMaster == (x) ||                 \
    I2cSlave == (x))

#define IS_VALID_ACK(x)                 \
(   I2cAck == (x) ||                    \
    I2cNAck == (x))

#define IS_VALID_EN(x)                  \
(   (TRUE == (x)) ||                    \
    (FALSE == (x)))


stc_i2c_instance_data_t m_astcI2cDataLut[I2C_INSTANCE_COUNT] =
{
    {
        I2CCH4,                         /* Ch.4 */
        M0P_MSC4_I2C,                   /* pstcInstance */
        { NULL, NULL, NULL, NULL }      /* stcInternData (not initialized yet) */
    },
    {
        I2CCH5,                         /* Ch.5 */
        M0P_MSC5_I2C,                   /* pstcInstance */
        { NULL, NULL, NULL, NULL }      /* stcInternData (not initialized yet) */
    },
    {
        I2CCH6,                         /* Ch.6 */
        M0P_MSC6_I2C,                   /* pstcInstance */
        { NULL, NULL, NULL, NULL }      /* stcInternData (not initialized yet) */
    }
};

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
 ** \brief Return the internal data for a certain I2C instance.
 **
 ** \param [in] u8Idx Channel of the I2C
 **
 ** \retval Pointer to internal data or NULL if instance is not enabled 
 **        (or not known)
 **
 ******************************************************************************/
static stc_i2c_instance_data_t* I2cGetInternDataPtr(uint8_t u8Idx)
{
    stc_i2c_instance_data_t* pstcInternDataPtr = NULL;
    uint32_t i = 0u;

    ASSERT(IS_VALID_CH(u8Idx));
    
    for (i = 0u; i < ARRAY_SZ(m_astcI2cDataLut); i++)
    {
        if (u8Idx == m_astcI2cDataLut[i].u8Idx)
        {
            pstcInternDataPtr = &m_astcI2cDataLut[i];
            break;
        }
    }
    return (pstcInternDataPtr);
}

/**
 *******************************************************************************
 ** \brief Device dependent initialization of transmit interrupts according 
 **        CMSIS
 **
 ** \param [in]  u8Channel  I2C channel
 ** \param [in]  enIrqType  I2cIrqRxEn: RX IRQ, I2cIrqTxEn: TX IRQ
 ** 
 ** \retval None
 **
 ******************************************************************************/
static void I2cInitNvic(uint8_t u8Channel, en_i2c_irq_type_t enIrqType)
{
    IRQn_Type enIrq;
    uint8_t u8IrqLevRx, u8IrqLevTx;

    ASSERT(IS_VALID_CH(u8Channel));
    ASSERT(IS_VALID_IRQTYPE(enIrqType));
    
    switch (u8Channel)
    {
        case I2CCH4:
            u8IrqLevRx = DDL_IRQ_LEVEL_MSC4RX;
            u8IrqLevTx = DDL_IRQ_LEVEL_MSC4TX;
            break;
        case I2CCH5:
            u8IrqLevRx = DDL_IRQ_LEVEL_MSC5RX;
            u8IrqLevTx = DDL_IRQ_LEVEL_MSC5TX;
            break;
        case I2CCH6:
            u8IrqLevRx = DDL_IRQ_LEVEL_MSC6RX;
            u8IrqLevTx = DDL_IRQ_LEVEL_MSC6TX;
            break;
        default:
            return;
    }
    enIrq = (IRQn_Type)(MSC4RX_IRQn + (u8Channel - 4u) * 2u);
    
    /* rx */
    if (I2cIrqRxEn == enIrqType)
    {
        NVIC_ClearPendingIRQ(enIrq);
        NVIC_EnableIRQ(enIrq);
        NVIC_SetPriority(enIrq, u8IrqLevRx);
    }
    /* tx */
    if (I2cIrqTxEn == enIrqType)
    {
        enIrq = (IRQn_Type)(enIrq + 1u);
        NVIC_ClearPendingIRQ(enIrq);
        NVIC_EnableIRQ(enIrq);
        NVIC_SetPriority(enIrq, u8IrqLevTx);
    }
}

/**
 *******************************************************************************
 ** \brief Device dependent de-initialization of transmit interrupts according 
 **        CMSIS
 **
 ** \param [in]  u8Channel  I2C channel
 ** 
 ** \retval None
 **
 ******************************************************************************/
static void I2cDeInitNvic(uint8_t u8Channel)
{
    IRQn_Type enIrq;

    ASSERT(IS_VALID_CH(u8Channel));

    switch (u8Channel)
    {
        case I2CCH4:
        case I2CCH5:
        case I2CCH6:
            enIrq = (IRQn_Type)(MSC4RX_IRQn + u8Channel - 4u);
            break;
        default:
            return;
    }

    /* rx */
    NVIC_ClearPendingIRQ(enIrq);
    NVIC_DisableIRQ(enIrq);
    NVIC_SetPriority(enIrq, DDL_IRQ_LEVEL_DEFAULT);
    /* tx */
    enIrq = (IRQn_Type)(enIrq + 1u);
    NVIC_ClearPendingIRQ(enIrq);
    NVIC_DisableIRQ(enIrq);
    NVIC_SetPriority(enIrq, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 *******************************************************************************
 ** \brief 2C receive interrupt service routine. 
 **
 ** \param [in]  u8Channel  I2C channel
 ** 
 ** \retval None
 **
 ******************************************************************************/
void I2c_Rx_IrqHander(uint8_t u8Channel)
{
    stc_i2c_instance_data_t *pstcI2cInternData = NULL;

    ASSERT(IS_VALID_CH(u8Channel));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);

    if (NULL == pstcI2cInternData)
    {
        return;
    }

    if (NULL != pstcI2cInternData->stcI2cInternIrqCb.pfnRxIrqCb)
    {
        pstcI2cInternData->stcI2cInternIrqCb.pfnRxIrqCb();
    }
}

/**
 *******************************************************************************
 ** \brief I2C transfer interrupt service routine.
 **
 ** \param [in]  u8Channel  I2C channel
 ** 
 ** \retval None
 **
 ******************************************************************************/
void I2c_Tx_IrqHander(uint8_t u8Channel)
{
    stc_i2c_instance_data_t *pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_CH(u8Channel));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);

    if (NULL == pstcI2cInternData)
    {
        return;
    }

    pstcI2c = pstcI2cInternData->pstcInstance;

    if (1u == pstcI2c->SR_f.TDEF)
    {
        if (NULL != pstcI2cInternData->stcI2cInternIrqCb.pfnTxIrqCb)
        {
            pstcI2cInternData->stcI2cInternIrqCb.pfnTxIrqCb();
        }
    }
}

/**
 *******************************************************************************
 ** \brief I2C status interrupt service routine.
 **
 ** \param [in]  u8Channel  I2C channel
 ** 
 ** \retval None
 **
 ******************************************************************************/
void I2c_Stat_IrqHander(uint8_t u8Channel)
{
    stc_i2c_instance_data_t *pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_CH(u8Channel));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);

    if (NULL == pstcI2cInternData)
    {
        return;
    }

    pstcI2c = pstcI2cInternData->pstcInstance;

    if (1u == pstcI2c->BC_f.INTF)
    {
        if (NULL != pstcI2cInternData->stcI2cInternIrqCb.pfnTxRxIrqCb)
        {
            pstcI2cInternData->stcI2cInternIrqCb.pfnTxRxIrqCb();
        }
    }

    if (1u == pstcI2c->BS_f.STPCC)
    {
        if (NULL != pstcI2cInternData->stcI2cInternIrqCb.pfnStopDetectIrqCb)
        {
            pstcI2cInternData->stcI2cInternIrqCb.pfnStopDetectIrqCb();
        }
        pstcI2c->BS_f.STPCC = 0u;
    }
}

/**
 *******************************************************************************
 ** \brief Enable I2C interrupts
 **
 ** \param [in] u8Channel  Channel of I2C
 ** \param [in] enIrqSel   Enumeration of I2C interrupt selection
 ** \arg I2cTxIrq  TX interrupt of I2C
 ** \arg I2cRxIrq  RX interrupt of I2C
 ** \arg I2cTxRxIrq   TX and RX interrupt of I2C
 ** \arg I2cStopDetectIrq Stop detection interrupt of I2C
 **
 ** \retval Ok                    Interrupts has been enabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2cInternData == NULL
 **                               - enIrqSel not valid
 **
 ******************************************************************************/
en_result_t I2c_EnableIrq(uint8_t u8Channel, en_i2c_irq_sel_t enIrqSel)
{
    stc_i2c_instance_data_t *pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_CH(u8Channel));
    ASSERT(IS_VALID_IRQ(enIrqSel));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);

    if (NULL == pstcI2cInternData)
    {
        return (ErrorInvalidParameter);
    }

    pstcI2c = pstcI2cInternData->pstcInstance;

    switch (enIrqSel)
    {
        case I2cTxIrq:
            pstcI2c->MR_f.TXIE = 1u;
            break;
        case I2cRxIrq:
            pstcI2c->MR_f.RXIE = 1u;
            break;
        case I2cTxRxIrq:
            pstcI2c->BC_f.INTE = 1u;
            break;
        case I2cStopDetectIrq:
            pstcI2c->BC_f.CNDDIE = 1u;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    return (Ok);
}

/**
 *******************************************************************************
 ** \brief Disable I2C interrupts
 **
 ** \param [in] u8Channel  Channel of I2C
 ** \param [in] enIrqSel   Enumeration of I2C interrupt selection
 ** \arg I2cTxIrq  TX interrupt of I2C
 ** \arg I2cRxIrq  RX interrupt of I2C
 ** \arg I2cTxRxIrq   TX and RX interrupt of I2C
 ** \arg I2cStopDetectIrq Stop detection interrupt of I2C
 **
 ** \retval Ok                    Interrupts has been disabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2cInternData == NULL
 **                               - enIrqSel not valid
 **
 ******************************************************************************/
en_result_t I2c_DisableIrq(uint8_t u8Channel, en_i2c_irq_sel_t enIrqSel)
{
    stc_i2c_instance_data_t *pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_CH(u8Channel));
    ASSERT(IS_VALID_IRQ(enIrqSel));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);

    if (NULL == pstcI2cInternData)
    {
        return (ErrorInvalidParameter);
    }

    pstcI2c = pstcI2cInternData->pstcInstance;

    switch (enIrqSel)
    {
        case I2cTxIrq:
            pstcI2c->MR_f.TXIE = 0u;
            break;
        case I2cRxIrq:
            pstcI2c->MR_f.RXIE = 0u;
            break;
        case I2cTxRxIrq:
            pstcI2c->BC_f.INTE = 0u;
            break;
        case I2cStopDetectIrq:
            pstcI2c->BC_f.CNDDIE = 0u;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    return (Ok);
}

/**
 *******************************************************************************
 ** \brief Initialization of I2C module.
 **
 ** \param [in]   u8Channel       Channel of I2C
 ** \param [in]   pstcConfig      MSC I2C configuration
 **
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcConfig == NULL
 **             - pstcI2cInternData == NULL (invalid)
 **             - u8Channel not valid
 **
 ******************************************************************************/
en_result_t I2c_Init(uint8_t u8Channel, stc_i2c_config_t* pstcConfig)
{
    stc_i2c_instance_data_t *pstcI2cInternData;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    /* Preset local register variables to zero */
    stc_msc_i2c_mr_field_t stcMR = { 0u };
    stc_msc_i2c_bc_field_t stcBC = { 0u };
    stc_msc_i2c_sa_field_t stcSA = { 0u };
    stc_msc_i2c_samsk_field_t stcSAMSK = { 0u };

    ASSERT(IS_VALID_POINTER(pstcConfig));
    ASSERT(IS_VALID_CH(u8Channel));
    ASSERT(pstcConfig->u32BaudRate < I2CMAXBPS);
    ASSERT(IS_VALID_EN(pstcConfig->bWaitSelection));
    ASSERT(IS_VALID_I2CMODE(pstcConfig->enMsMode));

    if((u8Channel > I2CCHMAX) || (NULL == pstcConfig))
    {
        return (ErrorInvalidParameter);
    }

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);

    /* Parameter check and get ptr to internal data struct */
    if ((NULL == pstcI2cInternData) || (NULL == pstcConfig))
    {
        return (ErrorInvalidParameter);
    }

    pstcI2c = pstcI2cInternData->pstcInstance;

    /* Set I2C mode */
    switch (pstcConfig->enMsMode)
    {
        case I2cMaster:
        case I2cSlave:
            stcMR.MODE = MSC_I2C_MODE;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    /* Ack enable */
    stcBC.ACKE = TRUE;

    /* Slave mode */
    if (I2cSlave == pstcConfig->enMsMode)
    {
        /* Set slave address */
        stcSA.ADDR = pstcConfig->u8SlaveAddr;
        /* Enable slave address detection */
        stcSA.AE = TRUE;
        /* Set slave address bit mask */
        stcSAMSK.MSK = pstcConfig->u8SlaveMaskAddr;
    }

    /* Enable I2C*/
    stcSAMSK.EN = TRUE;

    /* now setup hardware with correct mode first and then go on with */
    /*   bit settings */
    pstcI2c->MR_f = stcMR;

    /* I2C disable before other registers are set. */
    pstcI2c->SAMSK = 0u;

    /* Set baud rate generation */
    (void)I2c_SetBaudRate(u8Channel, pstcConfig->u32BaudRate);

    /* Set registers value */
    pstcI2c->BC_f = stcBC;
    pstcI2c->SA_f = stcSA;
    pstcI2c->SAMSK_f = stcSAMSK;

    /* Configure interrupt */
    if (NULL != pstcConfig->pstcIrqEn)
    {
        if (TRUE == pstcConfig->pstcIrqEn->bTxIrq)
        {
            pstcI2c->MR_f.TXIE = 1u;
        }

        if (TRUE == pstcConfig->pstcIrqEn->bRxIrq)
        {
            pstcI2c->MR_f.RXIE = 1u;
        }

        if (TRUE == pstcConfig->pstcIrqEn->bTxRxIrq)
        {
            pstcI2c->BC_f.INTE = 1u;
        }

        if (TRUE == pstcConfig->pstcIrqEn->bStopDetectIrq)
        {
            pstcI2c->BC_f.CNDDIE = 1u;
        }
    }

    /* Configure interrupt */
    if (NULL != pstcConfig->pstcIrqCb)
    {
        pstcI2cInternData->stcI2cInternIrqCb.pfnTxIrqCb = pstcConfig->pstcIrqCb->pfnTxIrqCb;
        pstcI2cInternData->stcI2cInternIrqCb.pfnRxIrqCb = pstcConfig->pstcIrqCb->pfnRxIrqCb;
        pstcI2cInternData->stcI2cInternIrqCb.pfnTxRxIrqCb = pstcConfig->pstcIrqCb->pfnTxRxIrqCb;
        pstcI2cInternData->stcI2cInternIrqCb.pfnStopDetectIrqCb = pstcConfig->pstcIrqCb->pfnStopDetectIrqCb;
    }
    if (NULL != pstcConfig->bTouchNvic)
    {
        I2cInitNvic(u8Channel, I2cIrqTxEn);
        I2cInitNvic(u8Channel, I2cIrqRxEn);
    }
    return (Ok);
}

/**
 *******************************************************************************
 ** \brief Deinitialisation of I2C module.
 **
 ** All used I2C register are reset to their default values.
 **
 ** \param [in]   pstcI2c         Pointer to I2C instance register area
 ** \param [in]   bTouchNvic      Touch NVIC or not
 **
 ** \retval Ok                    Process successfully done.
 **
 ******************************************************************************/
en_result_t I2c_DeInit(uint8_t u8Channel)
{
    stc_i2c_instance_data_t *pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_CH(u8Channel));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);

    if (NULL == pstcI2cInternData)
    {
        return (ErrorInvalidParameter);
    }

    pstcI2c = pstcI2cInternData->pstcInstance;
    pstcI2c->SAMSK_f.EN = 0u;
    
    I2cDeInitNvic(u8Channel);
    return (Ok);
}

/**
 *******************************************************************************
 ** \brief Set the baudrate of I2C
 **
 ** \param [in] u8Channel    Channel to I2C instance
 ** \param [in] u32BaudRate  Baudrate value [bps]
 **
 ** \retval Ok                    I2C baud rate has been successfully modified
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2cInternData == NULL
 **                               - u32BaudRate invalid
 **
 ** The I2C baud rate can be initialized in the I2c_Init() and be modified
 ** with this function.
 **
 ******************************************************************************/
en_result_t I2c_SetBaudRate(uint8_t u8Channel, uint32_t u32BaudRate)
{
    uint32_t u32Pclk1;
    boolean_t bTempBit = FALSE;
    stc_i2c_instance_data_t* pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_CH(u8Channel));
    ASSERT(u32BaudRate <= I2CMAXBPS);

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);
    if ((NULL == pstcI2cInternData) || (u32BaudRate > I2CMAXBPS))
    {
        return ErrorInvalidParameter;
    }

    pstcI2c = pstcI2cInternData->pstcInstance;

    bTempBit = pstcI2c->SAMSK_f.EN;
    pstcI2c->SAMSK_f.EN = 0u;

    u32Pclk1 = Clk_GetPeripheralClk(ClkPeripheralMsc);

    pstcI2c->BRS = (u32Pclk1 / u32BaudRate) - 1u;
    pstcI2c->SAMSK_f.EN = bTempBit;

    return (Ok);
}

/**
 *******************************************************************************
 ** \brief Generate start signal of I2C
 **
 ** \param [in] u8Channel         Channel to I2C instance
 **
 ** \retval Ok                    Generate start signal successfully
 ** \retval ErrorInvalidParameter pstcI2cInternData == NULL
 ** \retval ErrorTimeout          Timeout when generating a start signal
 **
 ******************************************************************************/
en_result_t I2c_GenerateStart(uint8_t u8Channel)
{
    uint32_t u32TimeOut = SystemCoreClock;
    stc_i2c_instance_data_t* pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_CH(u8Channel));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);
    if (NULL == pstcI2cInternData)
    {
        return ErrorInvalidParameter;
    }

    pstcI2c = pstcI2cInternData->pstcInstance;

    pstcI2c->BC_f.MSS = 1u;
    while (u32TimeOut--)
    {
        if ((1u == pstcI2c->BC_f.MSS) && (1u == pstcI2c->BC_f.OFITS))
        {
            return (Ok);
        }
    }

    return ErrorTimeout;
}

/**
 *******************************************************************************
 ** \brief Generate restart signal of I2C
 **
 ** \param [in] u8Channel    Channel to I2C instance
 **
 ** \retval Ok                    Generate restart signal successfully
 ** \retval ErrorInvalidParameter pstcI2cInternData == NULL
 ** \retval ErrorTimeout          Timeout when generating a restart signal
 **
 ******************************************************************************/
en_result_t I2c_GenerateRestart(uint8_t u8Channel)
{
    uint32_t u32TimeOut = SystemCoreClock;
    stc_i2c_instance_data_t* pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_CH(u8Channel));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);
    if (NULL == pstcI2cInternData)
    {
        return ErrorInvalidParameter;
    }

    pstcI2c = pstcI2cInternData->pstcInstance;

    pstcI2c->BC_f.OFITS = 1u;

    while (u32TimeOut--)
    {
        if (1u == pstcI2c->BS_f.ITSCC)
        {
            return (Ok);
        }
    }
    return ErrorTimeout;
}

/**
 *******************************************************************************
 ** \brief Generate stop signal of I2C
 **
 ** \param [in] u8Channel         Channel to I2C instance
 **
 ** \retval Ok                    Generate stop signal successfully
 ** \retval ErrorInvalidParameter pstcI2cInternData == NULL
 ** \retval ErrorTimeout          Timeout when generating a stop signal
 **
 ******************************************************************************/
en_result_t I2c_GenerateStop(uint8_t u8Channel)
{
    uint32_t u32TimeOut = SystemCoreClock;
    stc_i2c_instance_data_t* pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_CH(u8Channel));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);
    if (NULL == pstcI2cInternData)
    {
        return ErrorInvalidParameter;
    }

    pstcI2c = pstcI2cInternData->pstcInstance;

    while (u32TimeOut--)
    {
        pstcI2c->BC &= ~0xc1u;
        if ((0u == pstcI2c->BC_f.MSS) && (0u == pstcI2c->BC_f.OFITS))
        {
            return (Ok);
        }
    }

    return ErrorTimeout;
}

/**
 *******************************************************************************
 ** \brief Write I2C data buffer
 **
 ** \param [in] u8Channel  Channel to I2C instance
 ** \param [in] u8Data     Data to be sent
 **
 ** \retval Ok                    Data has been successfully sent
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2cInternData == NULL
 **
 ******************************************************************************/
en_result_t I2c_SendData(uint8_t u8Channel, uint8_t u8Data)
{
    stc_i2c_instance_data_t* pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_CH(u8Channel));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);
    if (NULL == pstcI2cInternData)
    {
        return ErrorInvalidParameter;
    }

    pstcI2c = pstcI2cInternData->pstcInstance;

    pstcI2c->TXDR = u8Data;

    return (Ok);
}

/**
 *******************************************************************************
 ** \brief Read I2C data buffer
 **
 ** \param [in] u8Channel  Channel to I2C instance
 ** \param [in] pu8data    pointer to store received data
 **
 ** \retval Ok                    Data has been successfully received
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pu8data == NULL
 **                               - pstcI2cInternData == NULL
 **
 ******************************************************************************/
en_result_t I2c_ReceiveData(uint8_t u8Channel, uint8_t* pu8data)
{
    stc_i2c_instance_data_t* pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_POINTER(pu8data));
    ASSERT(IS_VALID_CH(u8Channel));
    
    if(NULL == pu8data)
    {
        return ErrorInvalidParameter;
    }

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);
    if (NULL == pstcI2cInternData)
    {
        return ErrorInvalidParameter;
    }

    pstcI2c = pstcI2cInternData->pstcInstance;

    *pu8data = pstcI2c->RXDR;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Configure ACK signal sent to master
 **
 ** \param [in] u8Channel  Channel to I2C instance
 ** \param [in] enAck      ACK to be sent
 ** \arg  I2cAck ACK will be sent as response signal
 ** \arg  I2cNAck NACK will be sent as response signal
 **
 ** \retval Ok                    ACK signal has been successfully configured
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2cInternData == NULL
 **
 ** This function is only meaningful when I2C is in slave mode
 **
 ******************************************************************************/
en_result_t I2c_ConfigAck(uint8_t u8Channel, en_i2c_ack_t enAck)
{
    stc_i2c_instance_data_t* pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;
    stc_msc_i2c_bc_field_t stcBc;

    ASSERT(IS_VALID_CH(u8Channel));
    ASSERT(IS_VALID_ACK(enAck));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);
    if (NULL == pstcI2cInternData)
    {
        return ErrorInvalidParameter;
    }

    pstcI2c = pstcI2cInternData->pstcInstance;

    stcBc = pstcI2c->BC_f;
    (I2cAck == enAck) ?  (stcBc.ACKE = 1u) :  (stcBc.ACKE = 0u);

    stcBc.OFITS = 0u; /* Set OFITS = 0 to avoid generating a restart */

    pstcI2c->BC_f = stcBc;

    return (Ok);
}

/**
 *******************************************************************************
 ** \brief Get the ACK signal from slave
 **
 ** \param [in] u8Channel  Channel to I2C instance
 **
 ** \retval I2cAck Receive the ACK  from I2C Slave
 ** \retval I2cNAck Receive the NACK from I2C Slave
 **
 ** This function applies in the I2C master mode.
 **
 ******************************************************************************/
en_i2c_ack_t I2c_GetAck(uint8_t u8Channel)
{
    en_i2c_ack_t enRet;
    stc_i2c_instance_data_t* pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_CH(u8Channel));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);

    pstcI2c = pstcI2cInternData->pstcInstance;
    enRet = (0u == pstcI2c->BS_f.ACKF) ? I2cAck : I2cNAck;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get status of I2C according to status type
 **
 ** \param [in] u8Channel    Channel to I2C instance
 ** \param [in] enStatus     I2C status type
 ** \arg   I2cOverrunError          I2C overrun error
 ** \arg   I2cRxFull                I2C receive buffer full
 ** \arg   I2cTxEmpty               I2C tranfer buffer empty
 ** \arg   I2cFirstByteDetect       I2C first byte detection
 ** \arg   I2cReservedByteDetect    I2C reserved address detection
 ** \arg   I2cStopDetect            I2C stop condition detection
 ** \arg   I2cBusStatus             I2C bus status
 ** \arg   I2cBusErr                I2C bus error
 ** \arg   I2cRxTxIrq               I2C transfer and receive interrupt flag
 **
 ** \retval FALSE                 If one of following conditions are met:
 **                               - No I2C overrun error [enStatus = I2cOverrunError]
 **                               - I2C receive buffer is not full [enStatus = I2cRxFull]
 **                               - I2C tranfer buffer is not empty [enStatus = I2cTxEmpty]
 **                               - I2C first byte is not detected [enStatus = I2cFirstByteDetect]
 **                               - I2C reserved address is not detected [enStatus = I2cReservedByteDetect]
 **                               - I2C stop condition is not detected [enStatus = I2cStopDetect]
 **                               - I2C bus is idle [enStatus = I2cBusStatus]
 **                               - I2C bus error doesn't occur [enStatus = I2cBusErr]
 **                               - I2C transfer or receive is not completed. [enStatus = I2cRxTxIrq]
 **
 ** \retval TRUE                 If one of following conditions are met:
 **                               - I2C overrun error occurs  [enStatus = I2cOverrunError]
 **                               - I2C receive buffer is full [enStatus = I2cRxFull]
 **                               - I2C tranfer buffer is empty [enStatus = I2cTxEmpty]
 **                               - I2C first byte is detected [enStatus = I2cFirstByteDetect]
 **                               - I2C reserved address is detected [enStatus = I2cReservedByteDetect]
 **                               - I2C stop condition is detected [enStatus = I2cStopDetect]
 **                               - I2C bus is busy [enStatus = I2cBusStatus]
 **                               - I2C bus error occurs [enStatus = I2cBusErr]
 **                               - I2C transfer or receive is completed. [enStatus = I2cRxTxIrq]
 **
 ******************************************************************************/
boolean_t I2c_GetStatus(uint8_t u8Channel, en_i2c_status_t enStatus)
{
    boolean_t bRet;
    stc_i2c_instance_data_t* pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_CH(u8Channel));
    ASSERT(IS_VALID_STAT(enStatus));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);
    if (NULL == pstcI2cInternData)
    {
        return ErrorInvalidParameter;
    }

    pstcI2c = pstcI2cInternData->pstcInstance;

    switch (enStatus)
    {
        case I2cOverrunError:
            bRet = (1u == pstcI2c->SR_f.ORF) ? TRUE : FALSE;
            break;
        case I2cRxFull:
            bRet = (1u == pstcI2c->SR_f.RDFF) ? TRUE : FALSE;
            break;
        case I2cTxEmpty:
            bRet = (1u == pstcI2c->SR_f.TDEF) ? TRUE : FALSE;
            break;
        case I2cFirstByteDetect:
            bRet = (1u == pstcI2c->BS_f.FBB) ? TRUE : FALSE;
            break;
        case I2cReservedByteDetect:
            bRet = (1u == pstcI2c->BS_f.RAD) ? TRUE : FALSE;
            break;
        case I2cStopDetect:
            bRet = (1u == pstcI2c->BS_f.STPCC) ? TRUE : FALSE;
            break;
        case I2cBusStatus:
            bRet = (1u == pstcI2c->BS_f.BSS) ? TRUE : FALSE;
            break;
        case I2cBusErr:
            bRet = (1u == pstcI2c->BC_f.BERF) ? TRUE : FALSE;
            break;
        case I2cRxTxIrq:
            bRet = (1u == pstcI2c->BC_f.INTF) ? TRUE : FALSE;
            break;
        case I2cDevAddrMatch:
            if ((0u == pstcI2c->BC_f.MSS) && (1u == pstcI2c->BC_f.OFITS))
            {
                bRet = TRUE;
            }else
            {
                bRet = FALSE;
            }
            break;
        default:
            break;
    }

    return bRet;
}


/**
 *******************************************************************************
 ** \brief Clear status of I2C according to status type
 **
 ** \param [in] u8Channel    Channel to I2C instance
 ** \param [in] enStatus     I2C status type
 ** \arg   I2cOverrunError          I2C overrun error
 ** \arg   I2cRxFull                I2C receive buffer full
 ** \arg   I2cTxEmpty               I2C tranfer buffer empty
 ** \arg   I2cFirstByteDetect       I2C first byte detection
 ** \arg   I2cReservedByteDetect    I2C reserved address detection
 ** \arg   I2cStopDetect            I2C stop condition detection
 ** \arg   I2cBusStatus             I2C bus status
 ** \arg   I2cBusErr                I2C bus error
 ** \arg   I2cRxTxIrq               I2C transfer and receive interrupt flag
 **
 ** \retval Ok                    Status has been cleared normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2cInternData == NULL
 **                               - enStatus not valid
 **
 ** \note  The following status can only be cleared by hardware behavior:
 **        - I2cRxFull
 **        - I2cTxEmpty
 **        - I2cFirstByteDetect
 **        - I2cReservedByteDetect
 **        - I2cBusStatus
 **        - I2cBusErr
 **
 ******************************************************************************/
en_result_t I2c_ClrStatus(uint8_t u8Channel, en_i2c_status_t enStatus)
{
    stc_i2c_instance_data_t* pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_CH(u8Channel));
    ASSERT(IS_VALID_STAT(enStatus));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);
    if (NULL == pstcI2cInternData)
    {
        return ErrorInvalidParameter;
    }

    pstcI2c = pstcI2cInternData->pstcInstance;

    switch (enStatus)
    {
        case I2cOverrunError:
            pstcI2c->SR_f.RECLR = 1u;
            break;
        case I2cRxFull:
        case I2cTxEmpty:
        case I2cDevAddrMatch:
            break;
        case I2cFirstByteDetect:
            break;
        case I2cReservedByteDetect:
            break;
        case I2cStopDetect:
            pstcI2c->BS_f.STPCC = 0u;
            break;
        case I2cBusStatus:
            break;
        case I2cBusErr:
            break;
        case I2cRxTxIrq:
            pstcI2c->BC &= ~0x41u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    return (Ok);
}

/**
 *******************************************************************************
 ** \brief Get direction of I2C data in slave mode
 **
 ** \param [in] u8Channel  Channel to I2C instance
 **
 ** \retval i2c_master_tx_slave_rx  Master will send data to slave
 ** \retval i2c_slave_tx_master_rx  Master will receive data from slave
 **
 ** This function can be called after receiving the device address from master
 ** in the slave mode.
 **
 ******************************************************************************/
en_i2c_data_dir_t I2c_GetDataDir(uint8_t u8Channel)
{
    en_i2c_data_dir_t enDir;
    stc_i2c_instance_data_t* pstcI2cInternData = NULL;
    volatile stc_mscn_i2c_t *pstcI2c = NULL;

    ASSERT(IS_VALID_CH(u8Channel));

    pstcI2cInternData = I2cGetInternDataPtr(u8Channel);

    pstcI2c = pstcI2cInternData->pstcInstance;
    (0u == pstcI2c->BS_f.TRX) ? (enDir = i2c_master_tx_slave_rx) : (enDir = i2c_slave_tx_master_rx);

    return enDir;
}

//@} // I2cGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
