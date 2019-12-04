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
/** \file exint.c
 **
 ** A detailed description is available at
 ** @link ExintGroup Exint description @endlink
 **
 **   - 2017-08-10  1.0  WangM First version for Device Driver Library of exint
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "exint.h"

/**
 *******************************************************************************
 ** \addtogroup ExintGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Datatype for holding internal data needed for EXINT
 ******************************************************************************/
typedef struct stc_exint_intern_data
{
    func_ptr_t apfnExintCallback[DDL_EXINT_CHANNELS]; ///< Callback pointer array
} stc_exint_intern_data_t ;

/**
 *******************************************************************************
 ** \brief Datatype for holding internal data needed for NMI
 ******************************************************************************/
typedef struct stc_exint_nmi_intern_data
{
    func_ptr_t    pfnNmiCallback;       ///< Callback pointer
} stc_exint_nmi_intern_data_t ;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_CH(x) (ExintInstanceIndexMax > (x))
#define IS_NULL_POINT(x) (NULL != (x))

#define IS_VALID_LEVEL(x)               \
(   ExIntLowLevel == (x) ||             \
    ExIntHighLevel == (x) ||            \
    ExIntRisingEdge == (x) ||           \
    ExIntFallingEdge == (x))

#define IS_VALID_CNT_SZ(x)              \
(   DtCounterSize16 == (x) ||           \
    DtCounterSize32 == (x))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static stc_exint_intern_data_t stcExintInternData;
static stc_exint_nmi_intern_data_t stcExintNMIInternData;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief ISR for EXINT for channel 0 ~ channel 7
 **
 ** \param [in]  u8Channel  EXINT channel number
 **
 ** \retval None
 **
 ** \note This function clears the cause bit and calls the callback function, if
 **       specified.
 ******************************************************************************/
void Exti_07IrqHandler(uint8_t u8Channel)
{  
    ASSERT(IS_VALID_CH(u8Channel));

    M0P_EXTINT->INTCLR &= (0xFFFFFFFFu ^ (1ul << u8Channel));

    if (NULL != stcExintInternData.apfnExintCallback[u8Channel])
    {
      stcExintInternData.apfnExintCallback[u8Channel]();
    }
}

/**
 *******************************************************************************
 ** \brief ISR for EXINT for channel 8 ~ channel 15
 **
 ** \param [in]  u8Channel  EXINT channel number
 **
 ** \retval None
 **
 ** \note This function clears the cause bit and calls the callback function, if
 **       specified.
 ******************************************************************************/
void Exti_815IrqHandler(uint8_t u8Channel)
{
    ASSERT(IS_VALID_CH(u8Channel));

    M0P_EXTINT->INTCLR &= (0xFFFFFFFFu ^ (1ul << u8Channel));

    if (NULL != stcExintInternData.apfnExintCallback[u8Channel])
    {
        stcExintInternData.apfnExintCallback[u8Channel]();
    }
}

/**
 *******************************************************************************
 ** \brief ISR for NMI
 **
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
void Nmi_IrqHandler(uint8_t u8Dummy)
{
    stcExintNMIInternData.pfnNmiCallback();
}

/**
 *******************************************************************************
 ** \brief Initialize External Interrupts
 **
 ** This function initializes the external interrupts according the given
 ** configuration.
 ** 
 ** \param [in]  u8Channel   EXINT channel number
 ** \param [in]  pstcConfig  EXINT configuration parameters
 **
 ** \retval  Ok                       EXINT initialized
 **          ErrorInvalidParameter    pstcConfig == NULL or Illegal mode
 **
 ******************************************************************************/
en_result_t Exint_Init(en_exint_channel_index_t enChannel, stc_exint_config_t* pstcConfig)
{
    uint32_t u32INTLVL;
    uint32_t u32Enir;

    ASSERT(IS_VALID_CH(enChannel));
    ASSERT(IS_NULL_POINT(pstcConfig));
    
    if(NULL == pstcConfig)
    {
        return ErrorInvalidParameter;
    }

    u32INTLVL  = M0P_EXTINT->INTLVL;

    u32Enir  = M0P_EXTINT->INTEN;

    if(1u == pstcConfig->bEnable)
    {
        if (enChannel < ExintInstanceIndexMax)
        {
            switch (pstcConfig->enLevel)
            {
                case ExIntLowLevel:
                u32INTLVL &= (~(3u << (2u * enChannel)));
                break;
            case ExIntHighLevel:
                u32INTLVL |= 1u << (2u * enChannel);
                break;
            case ExIntRisingEdge:
                u32INTLVL |= 2u << (2u * enChannel);
                break;
            case ExIntFallingEdge:
                u32INTLVL |= 3u << (2u * enChannel);
                break;
            default:
                return ErrorInvalidParameter;
            }
        }
        u32Enir |= 1ul << enChannel;
    }
    else
    {
        u32Enir &= ~(1ul << enChannel);
    }
    stcExintInternData.apfnExintCallback[enChannel] = pstcConfig->pfnExintCallback;

    /* update hardware */
    M0P_EXTINT->INTLVL  = u32INTLVL;

    M0P_EXTINT->INTCLR = 0u;   /* Clear all possible interrupts due to level change */
    M0P_EXTINT->INTEN = u32Enir;
    
    if(TRUE == pstcConfig->bTouchNvic)
    {
        /* enable NVIC function */
        ExintEnableNvic(enChannel);
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable EXINT interrupt function of single channel
 **
 ** \param [in]  u8Channel  EXINT channel number
 **
 ** \retval Ok  Disable done
 **
 ******************************************************************************/
en_result_t Exint_DeInit(en_exint_channel_index_t enChannel, boolean_t bTouchNvic)
{
    ASSERT(IS_VALID_CH(enChannel));
    M0P_EXTINT->INTEN &= ~(1ul << enChannel);   /* Disable all channels */
    
    if(TRUE == bTouchNvic)
    {
        /* Disable NVIC function */
        ExintDisableNvic(enChannel);
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Enable Exint NVIC function
 **
 ** \param [in]  u8Channel   EXINT channel number
 **
 ** \retval  Ok                       enable done
 **          ErrorInvalidParameter    u8Channel not valid
 **
 ** \note  if u8Channel is channel0~chanbel7 enable NVIC EXINT0_7_IRQn
 **        if u8Channel is channel8~chanbel15 enable NVIC EXINT8_15_IRQn
 **
 ******************************************************************************/
en_result_t ExintEnableNvic(en_exint_channel_index_t enChannel)
{
    ASSERT(IS_VALID_CH(enChannel));
    
    if(enChannel <= ExintInstanceIndexExint7)
    {
        /* NVIC index EXINT0_7_IRQn*/
        NVIC_ClearPendingIRQ(EXINT0_7_IRQn); 
        NVIC_SetPriority(EXINT0_7_IRQn, DDL_IRQ_LEVEL_EXT07);
        NVIC_EnableIRQ(EXINT0_7_IRQn);
    }
    else if((enChannel >= ExintInstanceIndexExint7) && (enChannel <= ExintInstanceIndexExint15))
    {
        /* NVIC index EXINT8_15_IRQn*/
        NVIC_ClearPendingIRQ(EXINT8_15_IRQn);
        NVIC_SetPriority(EXINT8_15_IRQn, DDL_IRQ_LEVEL_EXT815);
        NVIC_EnableIRQ(EXINT8_15_IRQn);
    }
    else
    {
        return ErrorInvalidParameter;
    }
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable Exint NVIC function
 **
 ** \param [in]  u8Channel   EXINT channel number
 **
 ** \retval  Ok                       disable done
 **          ErrorInvalidParameter    u8Channel not valid
 **
 ** \note  if u8Channel is channel0~chanbel7 disable NVIC EXINT0_7_IRQn
 **        if u8Channel is channel8~chanbel15 disable NVIC EXINT8_15_IRQn
 **
 ******************************************************************************/
en_result_t ExintDisableNvic(en_exint_channel_index_t enChannel)
{
    ASSERT(IS_VALID_CH(enChannel));
    
    if(enChannel <= ExintInstanceIndexExint7)
    {
        /* NVIC index EXINT0_7_IRQn*/
        NVIC_ClearPendingIRQ(EXINT0_7_IRQn);
        NVIC_DisableIRQ(EXINT0_7_IRQn);
        NVIC_SetPriority(EXINT0_7_IRQn, DDL_IRQ_LEVEL_DEFAULT);
    }
    else if((enChannel >= ExintInstanceIndexExint7) && (enChannel <= ExintInstanceIndexExint15))
    {
        /* NVIC index EXINT8_15_IRQn*/
        NVIC_ClearPendingIRQ(EXINT8_15_IRQn);
        NVIC_DisableIRQ(EXINT8_15_IRQn);
        NVIC_SetPriority(EXINT8_15_IRQn, DDL_IRQ_LEVEL_DEFAULT);
    }
    else
    {
        return ErrorInvalidParameter;
    }
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Enable EXINT interrupt function of single channel
 **
 ** \param [in]  u8Channel  EXINT channel number
 **
 ** \retval Ok Enable done
 **
 ******************************************************************************/
en_result_t Exint_EnableChannel(en_exint_channel_index_t enChannel)
{
    ASSERT(IS_VALID_CH(enChannel));

    M0P_EXTINT->INTCLR &= 0xFFFFFFFFu ^ (1u << enChannel);
    M0P_EXTINT->INTEN |= 1u << enChannel;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable EXINT function of single channel
 **
 ** \param [in]  u8Channel  EXINT channel number
 **
 ** \retval Ok  Disable done
 **
 ******************************************************************************/
en_result_t Exint_DisableChannel(en_exint_channel_index_t enChannel)
{
    ASSERT(IS_VALID_CH(enChannel));

    M0P_EXTINT->INTEN &= (1u << enChannel) ^ 0xFFFFFFFFu;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Set external interrupt detect mode
 **
 ** \param [in]  u8Channel  EXINT channel number
 ** \param [in]  enLevel
 ** \arg ExIntLowLevel       Low level is active edge for interrupt detection
 ** \arg ExIntHighLevel      High level is active edge for interrupt detection
 ** \arg ExIntRisingEdge     Rising edge is active edge for interrupt detection
 ** \arg ExIntFallingEdge    Falling edge is active edge for interrupt detection
 ** 
 ** \retval  Ok                     Interreupt detection edge set normmally
 ** \retval  ErrorInvalidParameter  Invalid paremeter
 ******************************************************************************/
en_result_t Exint_SetDetectMode(en_exint_channel_index_t enChannel, en_exint_level_t enLevel)
{
    ASSERT(IS_VALID_CH(enChannel));
    ASSERT(IS_VALID_LEVEL(enLevel));

    if(enLevel <= ExIntFallingEdge)
    {
        M0P_EXTINT->INTLVL &= ~(3ul << (2u * enChannel));
        M0P_EXTINT->INTLVL |= (uint32_t)(((uint32_t)enLevel<<(2u * enChannel)));
    }
    else
    {
        return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get the detection mode of EXINT
 **
 ** \param u8Channel   EXINT channel number
 **
 ** \retval ExIntLowLevel       Low level is active edge for interrupt detection
 ** \retval ExIntHighLevel      High level is active edge for interrupt detection
 ** \retval ExIntRisingEdge     Rising edge is active edge for interrupt detection
 ** \retval ExIntFallingEdge    Falling edge is active edge for interrupt detection
 **
 ******************************************************************************/
en_exint_level_t Exint_GetDetectMode(en_exint_channel_index_t enChannel)
{
    uint8_t u8Level;
    ASSERT(IS_VALID_CH(enChannel));
    u8Level = ( M0P_EXTINT->INTLVL & (3u<<(2u * enChannel))) >> (2u * enChannel);
    return (en_exint_level_t)u8Level;
}

/**
 *******************************************************************************
 ** \brief Init Non-Maskable Interrupt (NMI)
 **
 ** This function initializes the NMI according to the given configuration.
 ** 
 ** \param [in]  pstcConfig        NMI configuration parameters
 ** 
 ** \retval Ok                     NMI initialized
 ** \retval ErrorInvalidParameter  pstcConfig == NULL

 ******************************************************************************/
en_result_t Exint_Nmi_Init(stc_exint_nmi_config_t* pstcConfig)
{
    if ( pstcConfig == NULL )
    {
        return ErrorInvalidParameter;
    }

    /* Set internal data */
    stcExintNMIInternData.pfnNmiCallback = pstcConfig->pfnNmiCallback;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief De-Init Non-Maskable Interrupt (NMI)
 **
 ** This function de-initializes the NMI according to the given
 ** configuration.
 **
 ** \param [in]  pstcConfig        NMI configuration parameters
 ** 
 ** \retval Ok                     NMI initialized
 **
 ******************************************************************************/
en_result_t Exint_Nmi_DeInit(void)
{
    /* Set internal data */
    stcExintNMIInternData.pfnNmiCallback = NULL;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Clear NMI interrupt flag
 **
 ** \param [in]  None
 ** 
 ** \retval Ok  Clear done
 **
 ******************************************************************************/
en_result_t Exint_Nmi_IrqFlagClear(void)
{
    M0P_EXTINT->NMICLR = 0;

    return Ok;
}

//@} // ExintGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
