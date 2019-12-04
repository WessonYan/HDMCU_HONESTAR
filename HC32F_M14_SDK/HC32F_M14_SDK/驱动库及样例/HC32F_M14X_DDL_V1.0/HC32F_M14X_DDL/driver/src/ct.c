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
/** \file ct.c
 **
 ** A detailed description is available at
 ** @link CtGroup Ct description @endlink
 **
 **   - 2017-8-10  1.0  WangM First version for Device Driver Library of ct.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ct.h"

/**
 *******************************************************************************
 ** \addtogroup CtGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define STA_REG_BITBAND(CtInstance) ((uint32_t)(&(bM0P_CT0_PWM_STA_UDFIR)) + \
                               ((uint32_t)(CtInstance) - M0P_CT0_PWM_BASE) * 32)

#define IS_VALID_POINTER(x)         (NULL != (x))
#define IS_VALID_CH(x)                  \
(   (CtChannel0  <= (x))                \
    && ((x) <= CtChannel3))
#define IS_VALID_EVEN_CH(x)             \
(   (CtChannel0  == (x))                \
    || (CtChannel2 == (x)))
#define IS_VALID_IO_MODE(x)             \
(   (CtIoMode0   <= (x))                \
    && ((x) <= CtIoMode8))


#define IS_VALID_PWM_DIV(x)             \
(   (PwmPresNone <= (x))                \
    && ((x) <= PwmPres1Div2048))
#define IS_VALID_PWM_RESTART(x)         \
(   (PwmRestartDisable == (x))          \
    || (PwmRestartEnable  == (x)))
#define IS_VALID_PWM_OUTPUT_MASK(x)     \
(   (PwmOutputNormal == (x))            \
    || (PwmOutputMask   == (x)))
#define IS_VALID_PWM_OUTPUT_POL(x)      \
(   (PwmPolarityLow == (x))             \
    || (PwmPolarityHigh   == (x)))
#define IS_VALID_PWM_EXTTRIG(x)         \
(   (PwmExtTrigDisable == (x))          \
    || (PwmExtTrigBoth == (x))          \
    || (PwmExtTrigRising == (x))        \
    || (PwmExtTrigFalling == (x)))
#define IS_VALID_PWM_MODE(x)            \
(   (PwmContinuous == (x))              \
    || (PwmOneshot    == (x)))
#define IS_VALID_PWM_IRQ_SEL(x)         \
(   (PwmTrigIrq <= (x))                 \
    && ((x) <= PwmUnderflowIrq))

#define IS_VALID_PPG_DIV(x)             \
(   (PpgPresNone <= (x))                \
    && ((x) <= PpgPres1Div2048))
#define IS_VALID_PPG_RESTART(x)         \
(   (PpgRestartDisable == (x))          \
    || (PpgRestartEnable  == (x)))
#define IS_VALID_PPG_OUTPUT_MASK(x)     \
(   (PpgOutputNormal == (x))            \
    || (PpgOutputMask   == (x)))
#define IS_VALID_PPG_OUTPUT_POL(x)      \
(   (PpgPolarityLow == (x))             \
    || (PpgPolarityHigh   == (x)))
#define IS_VALID_PPG_EXTTRIG(x)         \
(   (PpgExtTrigDisable <= (x))          \
    && ((x) <= PpgExtTrigBoth))
#define IS_VALID_PPG_MODE(x)            \
(   (PpgContinuous == (x))              \
    || (PpgOneshot    == (x)))
#define IS_VALID_PPG_IRQ_SEL(x)         \
(   (PpgTrigIrq <= (x))                 \
    && ((x) <= PpgUnderflowIrq))

#define IS_VALID_RT_DIV(x)              \
(   (RtPresNone <= (x))                 \
    && ((x) <= RtPres1Div2048))
#define IS_VALID_RT_SIZE(x)             \
(   (RtSize16Bit == (x))                \
    || ((x) == RtSize32Bit))
#define IS_VALID_RT_OUTPUT_POL(x)       \
(   (RtPolarityLow == (x))              \
    || ((x) == RtPolarityHigh))
#define IS_VALID_RT_EXTTRIG(x)          \
(   (RtExtTiggerDisable <= (x))         \
    && ((x) <= RtExtTiggerHighLevel))
#define IS_VALID_RT_MODE(x)             \
(   (RtReload == (x))                   \
    || (RtOneshot    == (x)))
#define IS_VALID_RT_IRQ_SEL(x)          \
(   (RtTrigIrq <= (x))                  \
    && ((x) <= RtUnderflowIrq))


#define IS_VALID_PWC_DIV(x)             \
(   (PwcPresNone <= (x))                \
    && ((x) <= PwcPres1Div2048))
#define IS_VALID_PWC_SIZE(x)            \
(   (PwcSize16Bit == (x))               \
    || ((x) == PwcSize32Bit))
#define IS_VALID_PWC_MEASURE_EDGE(x)    \
(   (PwcMeasureRisingToFalling <= (x))  \
    && ((x) <= PwcMeasureFallingToRising))
#define IS_VALID_PWC_MODE(x)            \
(   (PwcContinuous == (x))              \
    || (PwcOneshot    == (x)))
#define IS_VALID_PWC_IRQ_SEL(x)         \
(   (PwcMeasureCompleteIrq == (x))      \
    || (PwcMeasureOverflowIrq == (x)))

#define CT0       (*((volatile stc_ctn_t *) M0P_CT0_PWM_BASE))
#define CT1       (*((volatile stc_ctn_t *) M0P_CT1_PWM_BASE))
#define CT2       (*((volatile stc_ctn_t *) M0P_CT2_PWM_BASE))
#define CT3       (*((volatile stc_ctn_t *) M0P_CT3_PWM_BASE))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static stc_ct_instance_data_t* CtGetInstanceDataPtr(uint8_t u8Idx);
static void CtEnableNvic(void);
static void CtDisableNvic(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/* Look-up table for all enabled CT instances and their internal data  */
static stc_ct_instance_data_t m_astcCtInstanceDataLut[4] =
{
    { 
        CtChannel0,           /* CT channel 0 */
        &CT0,                 /* pstcInstance */
        {NULL, NULL, NULL}    /* stcInternData (not initialized yet) */
    },
    { 
        CtChannel1,           /* CT channel 1 */
        &CT1,                 /* pstcInstance */
        {NULL, NULL, NULL}    /* stcInternData (not initialized yet) */
    },
    { 
        CtChannel2,           /* CT channel 2 */
        &CT2,                 /* pstcInstance */
        {NULL, NULL, NULL}    /* stcInternData (not initialized yet) */
    },
    { 
        CtChannel3,           /* CT channel 3 */
        &CT3,                 /* pstcInstance */
        {NULL, NULL, NULL}    /* stcInternData (not initialized yet) */
    }
};

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief Return the data for a certain CT instance.
 **
 ** \param u8Idx             Channel index
 **
 ** \return Pointer to instance data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_ct_instance_data_t* CtGetInstanceDataPtr(uint8_t u8Idx) 
{
    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Idx));

    if (u8Idx == m_astcCtInstanceDataLut[u8Idx].u32Idx)
    {
        return &m_astcCtInstanceDataLut[u8Idx];
    }

    return NULL;
}     

/**
 *******************************************************************************
 ** \brief  Device dependent initialization of interrupts according CMSIS with
 **        level defined in interrupt_xxx.h
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void CtEnableNvic(void)
{
    NVIC_ClearPendingIRQ(CTIM0_3_FLASH_IRQn); 
    NVIC_EnableIRQ(CTIM0_3_FLASH_IRQn); 
    NVIC_SetPriority(CTIM0_3_FLASH_IRQn, DDL_IRQ_LEVEL_CT03);
}

/**
 *******************************************************************************
 ** \brief  Device dependent de-initialization of interrupts according CMSIS
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void CtDisableNvic(void)
{
    NVIC_ClearPendingIRQ(CTIM0_3_FLASH_IRQn);
    NVIC_DisableIRQ(CTIM0_3_FLASH_IRQn); 
    NVIC_SetPriority(CTIM0_3_FLASH_IRQn, DDL_IRQ_LEVEL_DEFAULT);
}


/**
 *******************************************************************************
 ** \brief  CT irq0  interrupt handler function
 **
 ** \param [in] u8Idx CT channel index
 **
 ** \retval None
 **
 ******************************************************************************/
void Ct_Irq0Handler(uint8_t u8Idx)
{
    uint8_t u8Irq = 0;
    uint32_t u32BitBandCtStaReg = 0u;
    stc_ct_instance_data_t *pstcCtInstanceData = NULL;

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Idx));

    /* Get actual address of register list of current channel */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return;
    }
    
    /* Get Interrupt Request */
    u8Irq = pstcCtInstanceData->pstcInstance->STA;
    u32BitBandCtStaReg = STA_REG_BITBAND(pstcCtInstanceData->pstcInstance);

    /* Duty Match Interrupt (PWM) */
    if (0x22u == (u8Irq & 0x22u))
    {
        /* Clear Interrupt */
        *((volatile unsigned int*)(u32BitBandCtStaReg + 4 * 1)) = 0;

        if (NULL != pstcCtInstanceData->stcInternData.pfnBit1IrqCb)
        {
            pstcCtInstanceData->stcInternData.pfnBit1IrqCb();  /* Call CallBackIrq */
        }
    }
    
    /* 
     * Underflow Interrupt (PWM,PPG,Reload timer)
     * Overflow Interrupt (PWC)
     */
    if (0x11u == (u8Irq & 0x11u))
    {
        /* Clear Interrupt */
        *((volatile unsigned int*)(u32BitBandCtStaReg + 4 * 0)) = 0;
        
        if (NULL != pstcCtInstanceData->stcInternData.pfnBit0IrqCb)
        {
            pstcCtInstanceData->stcInternData.pfnBit0IrqCb();  /* Call CallBackIrq */
        }
    }
}

/**
 *******************************************************************************
 ** \brief  CT irq1  interrupt handler function
 **
 ** \param [in] u8Idx CT channel index
 **
 ** \retval void
 **
 ******************************************************************************/
void Ct_Irq1Handler(uint8_t u8Idx)
{
    uint8_t u8Irq = 0;
    uint32_t u32BitBandCtStaReg = 0u;
    stc_ct_instance_data_t *pstcCtInstanceData = NULL;

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Idx));

    /* Get actual address of register list of current channel */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return;
    }
    
    /* Get Interrupt Request */
    u8Irq = pstcCtInstanceData->pstcInstance->STA;

    /*
     * Trigger Interrupt (PWM,PPG,Reload timer)
     * Measurement completion Interrupt (PWC)
     */
    if (u8Irq & 4ul)
    {
        u32BitBandCtStaReg = STA_REG_BITBAND(pstcCtInstanceData->pstcInstance);

        /* Clear Interrupt */
        *((volatile unsigned int*)(u32BitBandCtStaReg + 4 * 2)) = 0;

        if (NULL != pstcCtInstanceData->stcInternData.pfnBit2IrqCb)
        {
            pstcCtInstanceData->stcInternData.pfnBit2IrqCb();  /* Call CallBackIrq */
        }
    }
}

/**
 *******************************************************************************
 ** \brief Configure CT IO mode
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIoMode      CT IO mode
 ** \arg         CtIoMode0        CT IO mode 0
 ** \arg         CtIoMode1        CT IO mode 1
 ** \arg         CtIoMode2        CT IO mode 2
 ** \arg         CtIoMode3        CT IO mode 3
 ** \arg         CtIoMode4        CT IO mode 4
 ** \arg         CtIoMode5        CT IO mode 5
 ** \arg         CtIoMode6        CT IO mode 6
 ** \arg         CtIoMode7        CT IO mode 7
 ** \arg         CtIoMode8        CT IO mode 8
 **
 ** \retval Ok             CT IO mode has been set successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - enIoMode is invalid
 **
 ******************************************************************************/
en_result_t Ct_ConfigIOMode(uint8_t u8Idx, en_ct_io_mode_t enIoMode)
{
    /* Pointer to internal data */
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;
     
    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_IO_MODE(enIoMode));
 
    /* Get pointer to internal data structure  */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    if((CtChannel0 == u8Idx) || (CtChannel1 == u8Idx))
    {
        M0P_CTIOSEL03->CTSEL0123_f.SEL01 = (uint8_t)enIoMode;
    }
    else if((CtChannel2 == u8Idx) || (CtChannel3 == u8Idx))
    {
        M0P_CTIOSEL03->CTSEL0123_f.SEL23 = (uint8_t)enIoMode;
    }
    else
    {
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Set the Simultaneous Start register of Composite timer
 **
 ** This function is only valid in CTIO mode 5 and mode 6, otherwise don't 
 ** use this function!
 **
 ** \param [in]  u16Value      Bit0 ----- Ch.0
 **                            Bit1 ----- Ch.1
 **                                  ...
 **                            Bit3 ----- Ch.3
 **
 ** \retval None
 **
 ******************************************************************************/
void Ct_SetSimultaneousStart(uint8_t u8Value)
{
    /* Check parameters */
    ASSERT(u8Value <= 0x0F);  /* only ct ch.0~ch.3 */
    
    M0P_CTSSSR->CTSSSR = u8Value;
}

/**
 *******************************************************************************
 ** \brief Initialize PWM function of CT
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  pstcPwmConfig    Pointer to PWM configuration
 **
 ** \retval Ok             PWM function has been configured successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - pstcPwmConfig == NULL
 **                             - config data is invalid
 **
 ******************************************************************************/
en_result_t Ct_Pwm_Init(uint8_t u8Idx, stc_ct_pwm_config_t* pstcPwmConfig)
{
    uint16_t u16Pres = 0;
    volatile M0P_CT_PWM_TypeDef* pstcCtPwm = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to instance data */

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_POINTER(pstcPwmConfig));
    ASSERT(IS_VALID_PWM_DIV(pstcPwmConfig->enPres));
    ASSERT(IS_VALID_PWM_RESTART(pstcPwmConfig->enRestartEn));
    ASSERT(IS_VALID_PWM_OUTPUT_MASK(pstcPwmConfig->enOutputMask));
    ASSERT(IS_VALID_PWM_OUTPUT_POL(pstcPwmConfig->enOutputPolarity));
    ASSERT(IS_VALID_PWM_EXTTRIG(pstcPwmConfig->enExtTrig));
    ASSERT(IS_VALID_PWM_MODE(pstcPwmConfig->enMode));

    if(NULL == pstcPwmConfig)
    {
        return ErrorInvalidParameter;
    }
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPwm = (volatile M0P_CT_PWM_TypeDef*)(pstcCtInstanceData->pstcInstance);
    /* Reset Composite Timer */
    pstcCtPwm->TCR  = 0u;  
    pstcCtPwm->TCR2 = 0u;
    pstcCtPwm->STA  = 0u;
    /* Set Composite Timer to PWM mode */
    pstcCtPwm->TCR_f.FUNC = CtPwmMode;

    do
    {
        /* Clock prescaler setting */
        u16Pres = pstcPwmConfig->enPres;
        switch (pstcPwmConfig->enPres)
        {
            case PwmPresNone:
            case PwmPres1Div4:
            case PwmPres1Div16:
            case PwmPres1Div128:
            case PwmPres1Div256:
            case PwmPres1ExtClkRising:
            case PwmPres1ExtClkFalling:
            case PwmPres1ExtClkBoth: 
                pstcCtPwm->TCR2_f.CLKS3 = 0u;
                pstcCtPwm->TCR_f.CLKS = u16Pres;
                break;
            case PwmPres1Div512:
            case PwmPres1Div1024:
            case PwmPres1Div2048:
                pstcCtPwm->TCR2_f.CLKS3 = 1u;
                pstcCtPwm->TCR_f.CLKS = (u16Pres & 0x07u);
                break;
            default:
              break;
        }
        /* Restart enable setting */
        switch (pstcPwmConfig->enRestartEn)
        {
            case PwmRestartDisable:
                pstcCtPwm->TCR_f.RSTEN = 0u;
                break;
            case PwmRestartEnable:
                pstcCtPwm->TCR_f.RSTEN = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }
        /* Output Mask setting */
        switch (pstcPwmConfig->enOutputMask)
        {
            case PwmOutputNormal:
                pstcCtPwm->TCR_f.MASK = 0u;
                break;
            case PwmOutputMask:
                pstcCtPwm->TCR_f.MASK = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }
        /* Output polarity setting */
        switch (pstcPwmConfig->enOutputPolarity)
        {
            case PwmPolarityLow:
                pstcCtPwm->TCR_f.PLRT = 0u;
                break;
            case PwmPolarityHigh:
                pstcCtPwm->TCR_f.PLRT = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }
        /* External trigger setting */
        switch (pstcPwmConfig->enExtTrig)
        {
            case PwmExtTrigDisable:
                pstcCtPwm->TCR_f.EDGS =  0u;
                break;
            case PwmExtTrigRising:
                pstcCtPwm->TCR_f.EDGS = 1u;
                break;
            case PwmExtTrigFalling:
                pstcCtPwm->TCR_f.EDGS = 2u;
                break;
            case PwmExtTrigBoth:
                pstcCtPwm->TCR_f.EDGS = 3u;
                break;
            default:
                return ErrorInvalidParameter;
        }
        /* Mode setting */
        switch (pstcPwmConfig->enMode)
        {
            case PwmContinuous:
                pstcCtPwm->TCR_f.MODS = 0u;
                break;
            case PwmOneshot:
                pstcCtPwm->TCR_f.MODS = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }   
            
        /* Interrupt enable setting */
        if(NULL != pstcPwmConfig->pstcPwmIrqEn)
        {
            pstcCtPwm->STA_f.TRGIE = (TRUE == pstcPwmConfig->pstcPwmIrqEn->bPwmTrigIrq) ? 1u:0u;
            pstcCtPwm->STA_f.DTMIE = (TRUE == pstcPwmConfig->pstcPwmIrqEn->bPwmDutyMatchIrq) ? 1u:0u;
            pstcCtPwm->STA_f.UDFIE = (TRUE == pstcPwmConfig->pstcPwmIrqEn->bPwmUnderflowIrq) ? 1u:0u;
        }
        
        /* Set interrupt callback function */
        if(NULL != pstcPwmConfig->pstcPwmIrqCb)
        {
            pstcCtInstanceData->stcInternData.pfnBit2IrqCb = pstcPwmConfig->pstcPwmIrqCb->pfnPwmTrigIrqCb;
            pstcCtInstanceData->stcInternData.pfnBit1IrqCb = pstcPwmConfig->pstcPwmIrqCb->pfnPwmDutyMatchIrqCb;
            pstcCtInstanceData->stcInternData.pfnBit0IrqCb = pstcPwmConfig->pstcPwmIrqCb->pfnPwmUnderflowIrqCb;
        }
        
        /* NVIC setting */
        (TRUE == pstcPwmConfig->bTouchNvic) ? CtEnableNvic():CtDisableNvic();
    } while (0);
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief De-Initialize PWM function of CT
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  bTouchNvic    Touch NVIC or not
 **
 ** \retval Ok             PWM function has been de-initialized successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **
 ******************************************************************************/
en_result_t Ct_Pwm_DeInit(uint8_t u8Idx, boolean_t bTouchNvic)
{
    volatile M0P_CT_PWM_TypeDef* pstcCtPwm = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }

    pstcCtPwm = (volatile M0P_CT_PWM_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtPwm->TCR  = 0u;
    pstcCtPwm->TCR2 = 0u;
    pstcCtPwm->CSR = 0u;
    pstcCtPwm->STA = 0u;
    pstcCtPwm->DUT = 0u;

    /* Clear callback */
    pstcCtInstanceData->stcInternData.pfnBit0IrqCb = NULL;
    pstcCtInstanceData->stcInternData.pfnBit1IrqCb = NULL;
    pstcCtInstanceData->stcInternData.pfnBit2IrqCb = NULL;
    
    if(TRUE == bTouchNvic)
    {
        CtDisableNvic();
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Enable PWM timer counting
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Ok             Enable PWM timer counting successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **
 ******************************************************************************/
en_result_t Ct_Pwm_EnableCount(uint8_t u8Idx)
{
    volatile M0P_CT_PWM_TypeDef* pstcCtPwm = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure . */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }

    pstcCtPwm = (volatile M0P_CT_PWM_TypeDef*)(pstcCtInstanceData->pstcInstance);
    ((volatile M0P_CT_PWM_TypeDef*)pstcCtPwm)->TCR_f.ENCNT = 1u;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable PWM timer counting
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Ok             Disable PWM timer counting successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **
 ******************************************************************************/
en_result_t Ct_Pwm_DisableCount(uint8_t u8Idx)
{
    volatile M0P_CT_PWM_TypeDef* pstcCtPwm = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPwm = (volatile M0P_CT_PWM_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtPwm->TCR_f.ENCNT = 0u;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Start PWM timer by software
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Ok             Start PWM timer successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **
 ******************************************************************************/
en_result_t Ct_Pwm_EnableSwTrig(uint8_t u8Idx)
{
    volatile M0P_CT_PWM_TypeDef* pstcCtPwm = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPwm = (volatile M0P_CT_PWM_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtPwm->TCR_f.START = 1u;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Write Cycle value of PWM timer 
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  u16Cycle       Cycle value
 **
 ** \retval Ok             Write cycle value successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **
 ******************************************************************************/
en_result_t Ct_Pwm_WriteCycleVal(uint8_t u8Idx, uint16_t u16Cycle)
{
    volatile M0P_CT_PWM_TypeDef* pstcCtPwm = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure . */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }

    pstcCtPwm = (volatile M0P_CT_PWM_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtPwm->CSR = u16Cycle;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Write duty value of PWM timer 
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  u16Duty        Duty value
 **
 ** \retval Ok             Write duty value successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **
 ******************************************************************************/
en_result_t Ct_Pwm_WriteDutyVal(uint8_t u8Idx, uint16_t u16Duty)
{
    volatile M0P_CT_PWM_TypeDef* pstcCtPwm = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPwm = (volatile M0P_CT_PWM_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtPwm->DUT = u16Duty;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Read current count value of PWM timer 
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Current count value
 **
 ******************************************************************************/
uint16_t Ct_Pwm_ReadCurCnt(uint8_t u8Idx)
{ 
    volatile M0P_CT_PWM_TypeDef* pstcCtPwm = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    
    pstcCtPwm = (volatile M0P_CT_PWM_TypeDef*)(pstcCtInstanceData->pstcInstance);
    return pstcCtPwm->TCNT;
}

/**
 *******************************************************************************
 ** \brief Enable PWM timer interrupt
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIrqSel        PWM timer interrupt type
 ** \arg         PwmTrigIrq           PWM trigger detection interrupt
 ** \arg         PwmDutyMatchIrq  PWM duty match interrupt
 ** \arg         PwmUnderflowIrq   PWM underflow interrupt
 **
 ** \retval Ok             Enable PWM timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - enIrqSel is invalid
 **
 ******************************************************************************/
en_result_t Ct_Pwm_EnableIrq(uint8_t u8Idx, en_pwm_irq_sel_t enIrqSel)
{
    volatile M0P_CT_PWM_TypeDef* pstcCtPwm = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_PWM_IRQ_SEL(enIrqSel));
    
    /* Get pointer to internal data structure */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }

    pstcCtPwm = (volatile M0P_CT_PWM_TypeDef*)(pstcCtInstanceData->pstcInstance);
    
    switch (enIrqSel)
    {
        case PwmTrigIrq:
            pstcCtPwm->STA_f.TRGIE = 1u;
            break;
        case PwmDutyMatchIrq:
            pstcCtPwm->STA_f.DTMIE = 1u;
            break;
        case PwmUnderflowIrq:
            pstcCtPwm->STA_f.UDFIE = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable PWM timer interrupt
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIrqSel        PWM timer interrupt type    
 ** \arg         PwmTrigIrq            PWM trigger detection interrupt
 ** \arg         PwmDutyMatchIrq  PWM duty match interrupt 
 ** \arg         PwmUnderflowIrq   PWM underflow interrupt   
 **
 ** \retval Ok             Disable PWM timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - enIrqSel is invalid
 **
 ******************************************************************************/
en_result_t Ct_Pwm_DisableIrq(uint8_t u8Idx, en_pwm_irq_sel_t enIrqSel)
{
    volatile M0P_CT_PWM_TypeDef* pstcCtPwm = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_PWM_IRQ_SEL(enIrqSel));

    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }

    pstcCtPwm = (volatile M0P_CT_PWM_TypeDef*)(pstcCtInstanceData->pstcInstance);
    
    switch (enIrqSel)
    {
        case PwmTrigIrq:
            pstcCtPwm->STA_f.TRGIE = 0u;
            break;
        case PwmDutyMatchIrq:
            pstcCtPwm->STA_f.DTMIE = 0u;
            break;
        case PwmUnderflowIrq:
            pstcCtPwm->STA_f.UDFIE = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get interrupt flag of PWM timer 
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIrqSel        PWM timer interrupt type
 ** \arg         PwmTrigIrq            PWM trigger detection interrupt
 ** \arg         PwmDutyMatchIrq  PWM duty match interrupt 
 ** \arg         PwmUnderflowIrq   PWM underflow interrupt   
 **
 ** \retval     boolean_t    the interrupt status
 ******************************************************************************/
boolean_t Ct_Pwm_GetIrqFlag(uint8_t u8Idx, en_pwm_irq_sel_t enIrqSel)
{
    boolean_t bFlag; 
    volatile M0P_CT_PWM_TypeDef* pstcCtPwm = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_PWM_IRQ_SEL(enIrqSel));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    
    pstcCtPwm = (volatile M0P_CT_PWM_TypeDef*)(pstcCtInstanceData->pstcInstance);
    
    switch (enIrqSel)
    {
        case PwmTrigIrq:
            bFlag = (1u == pstcCtPwm->STA_f.TRGIR) ? TRUE : FALSE;
            break;
        case PwmDutyMatchIrq:
            bFlag = (1u == pstcCtPwm->STA_f.DTMIR) ? TRUE : FALSE;
            break;
        case PwmUnderflowIrq:
            bFlag = (1u == pstcCtPwm->STA_f.UDFIR) ? TRUE : FALSE;
            break;
        default:
            break;
    }
    
    return bFlag;
}

/**
 *******************************************************************************
 ** \brief Clear interrupt flag of PWM timer 
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIrqSel        PWM timer interrupt type
 ** \arg         PwmTrigIrq           PWM trigger detection interrupt
 ** \arg         PwmDutyMatchIrq  PWM duty match interrupt
 ** \arg         PwmUnderflowIrq   PWM underflow interrupt
 **
 ** \retval Ok             Clear interrupt flag successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - enIrqSel is invalid
 **
 ******************************************************************************/
en_result_t Ct_Pwm_ClrIrqFlag(uint8_t u8Idx, en_pwm_irq_sel_t enIrqSel)
{
    volatile M0P_CT_PWM_TypeDef* pstcCtPwm = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_PWM_IRQ_SEL(enIrqSel));
    
    /* Get pointer to internal data structure .*/
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }

    pstcCtPwm = (volatile M0P_CT_PWM_TypeDef*)(pstcCtInstanceData->pstcInstance);
    
    switch (enIrqSel)
    {
        case PwmTrigIrq:
            pstcCtPwm->STA_f.TRGIR = 0u;
            break;
        case PwmDutyMatchIrq:
            pstcCtPwm->STA_f.DTMIR = 0u;
            break;
        case PwmUnderflowIrq:
            pstcCtPwm->STA_f.UDFIR = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Initialize PPG function of CT
 **
 ** \param [in]  u8Idx                Channel index
 ** \param [in]  pstcPpgConfig    Pointer to PPG configuration
 **
 ** \retval Ok             PPG function has been configured successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - pstcPpgConfig == NULL
 **                             - config data is invalid
 **
 ******************************************************************************/
en_result_t Ct_Ppg_Init(uint8_t u8Idx, stc_ct_ppg_config_t* pstcPpgConfig)
{
    uint16_t u16Pres;
    volatile M0P_CT_PPG_TypeDef* pstcCtPpg = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data  */

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_POINTER(pstcPpgConfig));
    ASSERT(IS_VALID_PPG_DIV(pstcPpgConfig->enPres));
    ASSERT(IS_VALID_PPG_RESTART(pstcPpgConfig->enRestartEn));
    ASSERT(IS_VALID_PPG_OUTPUT_MASK(pstcPpgConfig->enOutputMask));
    ASSERT(IS_VALID_PPG_OUTPUT_POL(pstcPpgConfig->enOutputPolarity));
    ASSERT(IS_VALID_PPG_EXTTRIG(pstcPpgConfig->enExtTrig));
    ASSERT(IS_VALID_PPG_MODE(pstcPpgConfig->enMode));

    /* Get pointer to internal data structure .*/
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    
    if((NULL == pstcPpgConfig) || (NULL == pstcCtInstanceData))
    {
        return ErrorInvalidParameter;
    }

    pstcCtPpg = (volatile M0P_CT_PPG_TypeDef*)(pstcCtInstanceData->pstcInstance);

    /* Reset Composite Timer */
    pstcCtPpg->TCR  = 0u;
    pstcCtPpg->TCR2 = 0u;
    pstcCtPpg->STA  = 0u;
    /* Set Composite Timer to PPG mode */
    pstcCtPpg->TCR_f.FUNC = CtPpgMode;

    do
    {
        /* Clock prescaler setting */
        u16Pres = pstcPpgConfig->enPres;
        switch (pstcPpgConfig->enPres)
        {
            case PpgPresNone:
            case PpgPres1Div4:
            case PpgPres1Div16:
            case PpgPres1Div128:
            case PpgPres1Div256:
            case PpgPres1ExtClkRising:
            case PpgPres1ExtClkFalling:
            case PpgPres1ExtClkBoth:
                pstcCtPpg->TCR2_f.CLKS3 = 0u;
                pstcCtPpg->TCR_f.CLKS = u16Pres;
                break;
            case PpgPres1Div512:
            case PpgPres1Div1024:
            case PpgPres1Div2048:
                pstcCtPpg->TCR2_f.CLKS3 = 1u;
                pstcCtPpg->TCR_f.CLKS = (u16Pres & 0x7u);
                break;
            default:
                return ErrorInvalidParameter ;
        }
        /* Restart enable setting */
        switch (pstcPpgConfig->enRestartEn)
        {
            case PpgRestartDisable:
                pstcCtPpg->TCR_f.RSTEN = 0u;
                break;
            case PpgRestartEnable:
                pstcCtPpg->TCR_f.RSTEN = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }
        /* Output Mask setting */
        pstcCtPpg->TCR_f.MASK = (uint8_t)pstcPpgConfig->enOutputMask;
        switch (pstcPpgConfig->enOutputMask)
        {
            case PpgOutputNormal:
                pstcCtPpg->TCR_f.MASK = 0u;
                break;
            case PpgOutputMask:
                pstcCtPpg->TCR_f.MASK = 1u;
                break;
            default:   
                return ErrorInvalidParameter;
        }       
        /* Output polarity setting */
        switch (pstcPpgConfig->enOutputPolarity)
        {
            case PpgPolarityLow:
                pstcCtPpg->TCR_f.PLRT = 0u;
                break;
            case PpgPolarityHigh:
                pstcCtPpg->TCR_f.PLRT = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }
        /* External trigger setting  */   
        switch (pstcPpgConfig->enExtTrig)
        {
            case PpgExtTrigDisable:
                pstcCtPpg->TCR_f.EDGS = 0u;
                break;
            case PpgExtTrigRising:
                pstcCtPpg->TCR_f.EDGS = 1u;
                break;
            case PpgExtTrigFalling:
                pstcCtPpg->TCR_f.EDGS = 2u;
                break;
            case PpgExtTrigBoth:
                pstcCtPpg->TCR_f.EDGS = 3u;
                break;
            default:
                return ErrorInvalidParameter;
        }
        /* Mode setting */
        switch (pstcPpgConfig->enMode)
        {
            case PpgContinuous:
                pstcCtPpg->TCR_f.MODS = 0u;
                break;  
            case PpgOneshot:
                pstcCtPpg->TCR_f.MODS = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }

        /* Interrupt enable setting */
        if (NULL != pstcPpgConfig->pstcPpgIrqEn)
        {
            pstcCtPpg->STA_f.TRGIE = (TRUE == pstcPpgConfig->pstcPpgIrqEn->bPpgTrigIrq) ? 1u:0u;
            pstcCtPpg->STA_f.UDFIE = (TRUE == pstcPpgConfig->pstcPpgIrqEn->bPpgUnderflowIrq) ? 1u:0u;
        }

        /* Interrupt callback functions */
        if (NULL != pstcPpgConfig->pstcPpgIrqCb)
        {
            pstcCtInstanceData->stcInternData.pfnBit2IrqCb = pstcPpgConfig->pstcPpgIrqCb->pfnPpgTrigIrqCb;
            pstcCtInstanceData->stcInternData.pfnBit1IrqCb = NULL; 
            pstcCtInstanceData->stcInternData.pfnBit0IrqCb = pstcPpgConfig->pstcPpgIrqCb->pfnPpgUnderflowIrqCb;
        }
                
        /* NVIC setting */
        (TRUE == pstcPpgConfig->bTouchNvic) ? CtEnableNvic() : CtDisableNvic();
    } while (0);
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief De-Initialize PPG function of CT
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  bTouchNvic    Touch NVIC or not
 **
 ** \retval Ok             PPG function has been de-initialized successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **
 ******************************************************************************/
en_result_t Ct_Ppg_DeInit(uint8_t u8Idx, boolean_t bTouchNvic)
{
    volatile M0P_CT_PPG_TypeDef* pstcCtPpg = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data  */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ...  */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }

    pstcCtPpg = (volatile M0P_CT_PPG_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtPpg->TCR  = 0u;
    pstcCtPpg->TCR2 = 0u;
    pstcCtPpg->STA  = 0u;
    pstcCtPpg->LWRR = 0u;
    pstcCtPpg->HWRR = 0u;
    
    /* Clear callback */
    pstcCtInstanceData->stcInternData.pfnBit0IrqCb = NULL;
    pstcCtInstanceData->stcInternData.pfnBit1IrqCb = NULL;
    pstcCtInstanceData->stcInternData.pfnBit2IrqCb = NULL;
    
    if(TRUE == bTouchNvic)
    {
        CtDisableNvic();
    }
        
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Enable PPG timer counting
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Ok             Enable PPG timer counting successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **
 ******************************************************************************/
en_result_t Ct_Ppg_EnableCount(uint8_t u8Idx)
{
    volatile M0P_CT_PPG_TypeDef* pstcCtPpg = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }

    pstcCtPpg = (volatile M0P_CT_PPG_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtPpg->TCR_f.ENCNT = 1u;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable PPG timer counting
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Ok             Disable PPG timer counting successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **
 ******************************************************************************/
en_result_t Ct_Ppg_DisableCount(uint8_t u8Idx)
{
    volatile M0P_CT_PPG_TypeDef* pstcCtPpg = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ...  */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPpg = (volatile M0P_CT_PPG_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtPpg->TCR_f.ENCNT = 0u;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Start PPG timer by software
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Ok             Start PPG timer successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             
 ******************************************************************************/
en_result_t Ct_Ppg_EnableSwTrig(uint8_t u8Idx)
{
    volatile M0P_CT_PPG_TypeDef* pstcCtPpg = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ...  */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPpg = (volatile M0P_CT_PPG_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtPpg->TCR_f.START = 1u;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Write low width count value of PPG timer 
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  u16Val           Low width count value
 **
 ** \retval Ok             Write low width count value successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             
 ******************************************************************************/
en_result_t Ct_Ppg_WriteLowWidthVal(uint8_t u8Idx, uint16_t u16Val)
{
    volatile M0P_CT_PPG_TypeDef* pstcCtPpg = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPpg = (volatile M0P_CT_PPG_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtPpg->LWRR = u16Val;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Write high width count value of PPG timer
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  u16Val           High width count value
 **
 ** \retval Ok             Write high width count value successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             
 ******************************************************************************/
en_result_t Ct_Ppg_WriteHighWidthVal(uint8_t u8Idx, uint16_t u16Val)
{
    volatile M0P_CT_PPG_TypeDef* pstcCtPpg = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPpg = (volatile M0P_CT_PPG_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtPpg->HWRR = u16Val;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Read current count value of PPG timer 
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Current count value
 ******************************************************************************/
uint16_t Ct_Ppg_ReadCurCnt(uint8_t u8Idx)
{ 
    volatile M0P_CT_PPG_TypeDef* pstcCtPpg = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    
    pstcCtPpg = (volatile M0P_CT_PPG_TypeDef*)(pstcCtInstanceData->pstcInstance);
    return pstcCtPpg->TCNT;
}

/**
 *******************************************************************************
 ** \brief Enable PPG timer interrupt
 **
 ** \param [in]  u8Idx              Channel index
 ** \param [in]  enIrqSel          PPG timer interrupt type 
 ** \arg         PpgTrigIrq           PPG trigger detection interrupt
 ** \arg         PpgUnderflowIrq  PPG underflow interrupt
 **
 ** \retval Ok             Enable PPG timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - enIrqSel is invalid
 **
 ******************************************************************************/
en_result_t Ct_Ppg_EnableIrq(uint8_t u8Idx, en_ppg_irq_sel_t enIrqSel)
{
    volatile M0P_CT_PPG_TypeDef* pstcCtPpg = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_PPG_IRQ_SEL(enIrqSel));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPpg = (volatile M0P_CT_PPG_TypeDef*)(pstcCtInstanceData->pstcInstance);
    
    switch (enIrqSel)
    {
        case PpgTrigIrq:
            pstcCtPpg->STA_f.TRGIE = 1u;
            break;
        case PpgUnderflowIrq:
            pstcCtPpg->STA_f.UDFIE = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable PPG timer interrupt
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIrqSel        PPG timer interrupt type 
 ** \arg         PpgTrigIrq           PPG trigger detection interrupt
 ** \arg         PpgUnderflowIrq  PPG underflow interrupt
 **
 ** \retval Ok             Disable PPG timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - enIrqSel is invalid
 **
 ******************************************************************************/
en_result_t Ct_Ppg_DisableIrq(uint8_t u8Idx, en_ppg_irq_sel_t enIrqSel)
{
    volatile M0P_CT_PPG_TypeDef* pstcCtPpg = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_PPG_IRQ_SEL(enIrqSel)); 
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPpg = (volatile M0P_CT_PPG_TypeDef*)(pstcCtInstanceData->pstcInstance);
    
    switch (enIrqSel)
    {
        case PpgTrigIrq:
            pstcCtPpg->STA_f.TRGIE = 0u;
            break;
        case PpgUnderflowIrq:
            pstcCtPpg->STA_f.UDFIE = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
     
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get interrupt flag of PPG timer
 **
 ** \param [in]  u8Idx              Channel index
 ** \param [in]  enIrqSel          PPG timer interrupt type
 ** \arg         PpgTrigIrq           PPG trigger detection interrupt
 ** \arg         PpgUnderflowIrq  PPG underflow interrupt
 **
 ** \retval     boolean_t    the interrupt status
 **
 ******************************************************************************/
boolean_t Ct_Ppg_GetIrqFlag(uint8_t u8Idx, en_ppg_irq_sel_t enIrqSel)
{
    boolean_t bFlag; 
    volatile M0P_CT_PPG_TypeDef* pstcCtPpg = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_PPG_IRQ_SEL(enIrqSel));  
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    
    pstcCtPpg = (volatile M0P_CT_PPG_TypeDef*)(pstcCtInstanceData->pstcInstance);
    
    switch (enIrqSel)
    {
        case PpgTrigIrq:
            bFlag = (1u == pstcCtPpg->STA_f.TRGIR) ? TRUE : FALSE;
            break;
        case PpgUnderflowIrq:
            bFlag = (1u == pstcCtPpg->STA_f.UDFIR) ? TRUE : FALSE;
            break;
        default:
            break;
    }
    
    return bFlag;
}

/**
 *******************************************************************************
 ** \brief Clear interrupt flag of PPG timer 
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIrqSel        PPG timer interrupt type
 ** \arg         PpgTrigIrq           PPG trigger detection interrupt
 ** \arg         PpgUnderflowIrq  PPG underflow interrupt
 **
 ** \retval Ok             Clear interrupt flag successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - enIrqSel is invalid
 **
 ******************************************************************************/
en_result_t Ct_Ppg_ClrIrqFlag(uint8_t u8Idx, en_ppg_irq_sel_t enIrqSel)
{
    volatile M0P_CT_PPG_TypeDef* pstcCtPpg = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL;/* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_PPG_IRQ_SEL(enIrqSel)); 
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPpg = (volatile M0P_CT_PPG_TypeDef*)(pstcCtInstanceData->pstcInstance);
    
    switch (enIrqSel)
    {
        case PpgTrigIrq:
            pstcCtPpg->STA_f.TRGIR = 0u;
            break;
        case PpgUnderflowIrq:
            pstcCtPpg->STA_f.UDFIR = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Initialize RT function of CT
 **
 ** \param [in]  u8Idx               Channel index
 ** \param [in]  pstcRtConfig     Pointer to RT configuration
 **
 ** \retval Ok             RT function has been configured successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - pstcRtConfig == NULL
 **                             - Configuration data is invalid
 **
 ******************************************************************************/
en_result_t Ct_Rt_Init(uint8_t u8Idx, stc_ct_rt_config_t* pstcRtConfig)
{
    uint16_t u16Pres, u16ExtTrig;
    volatile M0P_CT_RT_TypeDef* pstcCtRt = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */
    
    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_POINTER(pstcRtConfig));
    ASSERT(IS_VALID_RT_DIV(pstcRtConfig->enPres));
    ASSERT(IS_VALID_RT_SIZE(pstcRtConfig->enSize));
    ASSERT(IS_VALID_RT_OUTPUT_POL(pstcRtConfig->enOutputPolarity));
    ASSERT(IS_VALID_RT_EXTTRIG(pstcRtConfig->enExtTrig));
    ASSERT(IS_VALID_RT_MODE(pstcRtConfig->enMode));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    
    if((NULL == pstcRtConfig) || (NULL == pstcCtInstanceData))
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtRt = (volatile M0P_CT_RT_TypeDef*)(pstcCtInstanceData->pstcInstance);
    /* Reset Composite Timer */
    pstcCtRt->TCR  = 0u;  
    pstcCtRt->TCR2 = 0u;
    pstcCtRt->STA  = 0u;
    /* Set Composite Timer to Reload Timer mode */
    pstcCtRt->TCR_f.FUNC = CtRtMode;

    do
    {
        /* Clock prescaler setting */
        u16Pres = pstcRtConfig->enPres; 
        switch (pstcRtConfig->enPres)
        {
            case RtPresNone:
            case RtPres1Div4:
            case RtPres1Div16:
            case RtPres1Div128:
            case RtPres1Div256:
            case RtPres1ExtClkRising:
            case RtPres1ExtClkFalling:
            case RtPres1ExtClkBoth:
                pstcCtRt->TCR2_f.CLKS3 = 0u;
                pstcCtRt->TCR_f.CLKS = u16Pres;
                break;  
            case RtPres1Div512:
            case RtPres1Div1024:
            case RtPres1Div2048:
                pstcCtRt->TCR2_f.CLKS3 = 1u;
                pstcCtRt->TCR_f.CLKS = (u16Pres & 0x7u);
                break;
            default:
                return ErrorInvalidParameter;
        }
        /* Timer size setting */
        switch (pstcRtConfig->enSize)
        {
            case RtSize16Bit:
                pstcCtRt->TCR_f.T32 = 0u;
                break;
            case RtSize32Bit:
                pstcCtRt->TCR_f.T32 = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }
        /* Output polarity setting */
        switch (pstcRtConfig->enOutputPolarity)
        {
            case RtPolarityLow:
                pstcCtRt->TCR_f.PLRT = 0u;
                break;
            case RtPolarityHigh:
                pstcCtRt->TCR_f.PLRT = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }
        
        /* External trigger setting */
        u16ExtTrig = pstcRtConfig->enExtTrig; 
        switch (pstcRtConfig->enExtTrig)
        {
            case RtExtTiggerDisable:
            case RtExtTiggerRisingEdge:
            case RtExtTiggerFallingEdge:
            case RtExtTiggerBothEdge:
                pstcCtRt->TCR2_f.GTIE = 0u;
                pstcCtRt->TCR_f.EDGS = u16ExtTrig;
                break;
            case RtExtTiggerLowLevel:
                pstcCtRt->TCR2_f.GTIE = 1u;
                pstcCtRt->TCR_f.EDGS = 0u;
                break;
            case RtExtTiggerHighLevel:
                pstcCtRt->TCR2_f.GTIE = 1u;
                pstcCtRt->TCR_f.EDGS = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }
       
        /* Mode setting */
        switch (pstcRtConfig->enMode)
        {
            case RtReload:
                pstcCtRt->TCR_f.MODS = 0u;
                break;
            case RtOneshot:
                pstcCtRt->TCR_f.MODS = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }
        
        /* Interrupt enable setting */
        if(NULL != pstcRtConfig->pstcRtIrqEn)
        {
            pstcCtRt->STA_f.TRGIE = (TRUE == pstcRtConfig->pstcRtIrqEn->bRtTrigIrq) ? 1u:0u;
            pstcCtRt->STA_f.UDFIE = (TRUE == pstcRtConfig->pstcRtIrqEn->bRtUnderflowIrq) ? 1u:0u;
        }
        /* Interrupt callback functions setting */
        if(NULL != pstcRtConfig->pstcRtIrqCb)
        {
            pstcCtInstanceData->stcInternData.pfnBit2IrqCb = pstcRtConfig->pstcRtIrqCb->pfnRtTrigIrqCb;
            pstcCtInstanceData->stcInternData.pfnBit1IrqCb = NULL;
            pstcCtInstanceData->stcInternData.pfnBit0IrqCb = pstcRtConfig->pstcRtIrqCb->pfnRtUnderflowIrqCb;
        }
                
        /* NVIC setting */
        (TRUE == pstcRtConfig->bTouchNvic) ? CtEnableNvic() :  CtDisableNvic();
    } while (0);
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief De-Initialize RT function of CT
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  bTouchNvic    Touch NVIC or not
 **
 ** \retval Ok             RT function has been de-initialized successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             
 ******************************************************************************/
en_result_t Ct_Rt_DeInit(uint8_t u8Idx, boolean_t bTouchNvic)
{
    volatile M0P_CT_RT_TypeDef* pstcCtRt = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtRt = (volatile M0P_CT_RT_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtRt->TCR  = 0u;
    pstcCtRt->TCR2 = 0u;
    pstcCtRt->STA = 0u;
    pstcCtRt->CSR = 0u;
    
    /* Clear callback */
    pstcCtInstanceData->stcInternData.pfnBit0IrqCb = NULL;
    pstcCtInstanceData->stcInternData.pfnBit1IrqCb = NULL;
    pstcCtInstanceData->stcInternData.pfnBit2IrqCb = NULL;
    
    if(TRUE == bTouchNvic)
    {
        CtDisableNvic();
    }
        
    return Ok;
}
        
/**
 *******************************************************************************
 ** \brief Enable Reload timer counting
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Ok             Enable RT timer counting successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             
 ******************************************************************************/
en_result_t Ct_Rt_EnableCount(uint8_t u8Idx)
{
    volatile M0P_CT_RT_TypeDef* pstcCtRt = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtRt = (volatile M0P_CT_RT_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtRt->TCR_f.ENCNT = 1u;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable Reload timer counting
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Ok             Disable RT timer counting successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             
 ******************************************************************************/
en_result_t Ct_Rt_DisableCount(uint8_t u8Idx)
{
    volatile M0P_CT_RT_TypeDef* pstcCtRt = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtRt = (volatile M0P_CT_RT_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtRt->TCR_f.ENCNT = 0u;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Start Reload timer by software
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Ok             Start Reload timer successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             
 ******************************************************************************/
en_result_t Ct_Rt_EnableSwTrig(uint8_t u8Idx)
{
    volatile M0P_CT_RT_TypeDef* pstcCtRt = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtRt = (volatile M0P_CT_RT_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtRt->TCR_f.START = 1u;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Write count cycle of Reload timer 
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  u16Val           Cycle value
 **
 ** \retval Ok             Write count cycle successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **
 ******************************************************************************/
en_result_t Ct_Rt_WriteCycleVal(uint8_t u8Idx, uint16_t u16Val)
{
    volatile M0P_CT_RT_TypeDef* pstcCtRt = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtRt = (volatile M0P_CT_RT_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtRt->CSR = u16Val;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Read current count value of Reload timer 
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Current count value
 ******************************************************************************/
uint16_t Ct_Rt_ReadCurCnt(uint8_t u8Idx)
{ 
    volatile M0P_CT_RT_TypeDef* pstcCtRt = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    
    pstcCtRt = (volatile M0P_CT_RT_TypeDef*)(pstcCtInstanceData->pstcInstance);
    return pstcCtRt->TCNT;
}

/**
 *******************************************************************************
 ** \brief Enable Reload timer interrupt
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIrqSel        Reload Timer interrupt type
 ** \arg         RtTrigIrq           Reload timer trigger detection flag
 ** \arg         RtUnderflowIrq  Reload timer underflow interrupt flag
 **
 ** \retval Ok             Enable Reload timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - enIrqSel is invalid
 **
 ******************************************************************************/
en_result_t Ct_Rt_EnableIrq(uint8_t u8Idx, en_rt_irq_sel_t enIrqSel)
{
    volatile M0P_CT_RT_TypeDef* pstcCtRt = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_RT_IRQ_SEL(enIrqSel));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtRt = (volatile M0P_CT_RT_TypeDef*)(pstcCtInstanceData->pstcInstance);

    switch (enIrqSel)
    {
        case RtTrigIrq:
            pstcCtRt->STA_f.TRGIE = 1u;
            break;
        case RtUnderflowIrq:
            pstcCtRt->STA_f.UDFIE = 1u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable Reload timer interrupt
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIrqSel        Reload Timer interrupt type
 ** \arg         RtTrigIrq           Reload timer trigger detection flag
 ** \arg         RtUnderflowIrq  Reload timer underflow interrupt flag
 **
 ** \retval Ok             Disable Reload timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - enIrqSel is invalid
 **
 ******************************************************************************/
en_result_t Ct_Rt_DisableIrq(uint8_t u8Idx, en_rt_irq_sel_t enIrqSel)
{
    volatile M0P_CT_RT_TypeDef* pstcCtRt = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_RT_IRQ_SEL(enIrqSel));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtRt = (volatile M0P_CT_RT_TypeDef*)(pstcCtInstanceData->pstcInstance);
    
    switch (enIrqSel)
    {
        case RtTrigIrq:
            pstcCtRt->STA_f.TRGIE = 0u;
            break;
        case RtUnderflowIrq:
            pstcCtRt->STA_f.UDFIE = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get interrupt flag of Reload timer 
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIrqSel        Reload timer interrupt type
 ** \arg         RtTrigIrq           Reload timer trigger detection flag
 ** \arg         RtUnderflowIrq  Reload timer underflow interrupt flag
 **
 ** \retval      boolean_t         the interrupt status
 ******************************************************************************/
boolean_t Ct_Rt_GetIrqFlag(uint8_t u8Idx, en_rt_irq_sel_t enIrqSel)
{
    boolean_t bFlag; 
    volatile M0P_CT_RT_TypeDef* pstcCtRt = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_RT_IRQ_SEL(enIrqSel));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtRt = (volatile M0P_CT_RT_TypeDef*)(pstcCtInstanceData->pstcInstance);
    
    switch (enIrqSel)
    {
        case RtTrigIrq:
            bFlag = (1u == pstcCtRt->STA_f.TRGIR) ? TRUE : FALSE;
            break;
        case RtUnderflowIrq:
            bFlag = (1u == pstcCtRt->STA_f.UDFIR) ? TRUE : FALSE;
            break;
        default:
            break;
    }
    
    return bFlag;
}

/**
 *******************************************************************************
 ** \brief Clear interrupt flag of Reload timer 
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIrqSel        Reload timer interrupt type
 ** \arg         RtTrigIrq           Reload timer trigger detection flag
 ** \arg         RtUnderflowIrq  Reload timer underflow interrupt flag
 **
 ** \retval Ok             Clear interrupt flag successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - enIrqSel is invalid
 **
 ******************************************************************************/
en_result_t Ct_Rt_ClrIrqFlag(uint8_t u8Idx, en_rt_irq_sel_t enIrqSel)
{
    volatile M0P_CT_RT_TypeDef* pstcCtRt = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_RT_IRQ_SEL(enIrqSel));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtRt = (volatile M0P_CT_RT_TypeDef*)(pstcCtInstanceData->pstcInstance);
    
    switch (enIrqSel)
    {
        case RtTrigIrq:
            pstcCtRt->STA_f.TRGIR = 0u;
            break;
        case RtUnderflowIrq:
            pstcCtRt->STA_f.UDFIR = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Initialize PWC function of CT
 **
 ** \param [in]  u8Idx                Channel index
 ** \param [in]  pstcPwcConfig    Pointer to PWC configuration
 **
 ** \retval Ok             PWC function has been configured successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - pstcPwcConfig == NULL
 **                             - Configuration data is invalid
 **
 ******************************************************************************/
en_result_t Ct_Pwc_Init(uint8_t u8Idx, stc_ct_pwc_config_t* pstcPwcConfig)
{
    uint16_t u16Pres;
    volatile M0P_CT_PWC_TypeDef* pstcCtPwc = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */

    /* Check parameters */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_POINTER(pstcPwcConfig));
    ASSERT(IS_VALID_PWC_DIV(pstcPwcConfig->enPres));
    ASSERT(IS_VALID_PWC_SIZE(pstcPwcConfig->enSize));
    ASSERT(IS_VALID_PWC_MEASURE_EDGE(pstcPwcConfig->enMeasureEdge));
    ASSERT(IS_VALID_PWC_MODE(pstcPwcConfig->enMode));

    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    
    if((NULL == pstcPwcConfig) || (NULL == pstcCtInstanceData))
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPwc = (volatile M0P_CT_PWC_TypeDef*)(pstcCtInstanceData->pstcInstance);
    /* Reset Composite Timer */
    pstcCtPwc->TCR  = 0u;
    pstcCtPwc->TCR2 = 0u;
    pstcCtPwc->STA  = 0u;
    /* Set Composite Timer to Reload Timer mode */
    pstcCtPwc->TCR_f.FUNC = CtPwcMode;

    do
    {
        /* Clock prescaler setting */
        u16Pres = pstcPwcConfig->enPres;
        switch (pstcPwcConfig->enPres)
        {
            case PwcPresNone:
            case PwcPres1Div4:
            case PwcPres1Div16:
            case PwcPres1Div128:
            case PwcPres1Div256:
                pstcCtPwc->TCR2_f.CLKS3 = 0u;
                pstcCtPwc->TCR_f.CLKS = u16Pres;
                break;
            case PwcPres1Div512:
            case PwcPres1Div1024:
            case PwcPres1Div2048:
                pstcCtPwc->TCR2_f.CLKS3 = 1u;
                pstcCtPwc->TCR_f.CLKS = (u16Pres & 0x07u);
                break;
            default:
                return ErrorInvalidParameter ;
        }
        /* Timer size setting */
        switch (pstcPwcConfig->enSize)
        {
            case PwcSize16Bit:
                pstcCtPwc->TCR_f.T32 = 0u;
                break;
            case PwcSize32Bit:
                pstcCtPwc->TCR_f.T32 = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }
        /* Measurement mode setting */
        switch (pstcPwcConfig->enMeasureEdge)
        {
            case PwcMeasureRisingToFalling:
                pstcCtPwc->TCR_f.EDGS  = 0u;
                break;  
            case PwcMeasureRisingToRising:
                pstcCtPwc->TCR_f.EDGS  = 1u;
                break;
            case PwcMeasureFallingToFalling:
                pstcCtPwc->TCR_f.EDGS  = 2u;
                break;
            case PwcMeasureEitherToEither:
                pstcCtPwc->TCR_f.EDGS  = 3u;
                break;
            case PwcMeasureFallingToRising:
                pstcCtPwc->TCR_f.EDGS  = 4u;
                break;
            default:
                return ErrorInvalidParameter;
        }
        /* Mode setting */
        switch (pstcPwcConfig->enMode)
        {
            case PwcContinuous:
                pstcCtPwc->TCR_f.MODS = 0u;
                break;
            case PwcOneshot:
                pstcCtPwc->TCR_f.MODS = 1u;
                break;
            default:
                return ErrorInvalidParameter;
        }
        
        /* Interrupt enable setting */
        if(NULL != pstcPwcConfig->pstcPwcIrqEn)
        {
            pstcCtPwc->STA_f.MCIE = (TRUE == pstcPwcConfig->pstcPwcIrqEn->bPwcMeasureCompleteIrq) ? 1u:0u;
            pstcCtPwc->STA_f.OVIE = (TRUE == pstcPwcConfig->pstcPwcIrqEn->bPwcMeasureOverflowIrq) ? 1u:0u;
        }
        
        /* Interrupt callback functions */
        if (NULL != pstcPwcConfig->pstcPwcIrqCb)
        {
            pstcCtInstanceData->stcInternData.pfnBit2IrqCb = pstcPwcConfig->pstcPwcIrqCb->pfnPwcMeasureCompleteIrqCb;
            pstcCtInstanceData->stcInternData.pfnBit1IrqCb = NULL;
            pstcCtInstanceData->stcInternData.pfnBit0IrqCb = pstcPwcConfig->pstcPwcIrqCb->pfnPwcMeasureOverflowIrqCb;
        }
              
        /* NVIC setting */
        (TRUE == pstcPwcConfig->bTouchNvic) ? CtEnableNvic() : CtDisableNvic();
    } while (0);
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief De-Initialize PWC function of CT
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  bTouchNvic    Touch NVIC or not
 **
 ** \retval Ok             PWC function has been de-initialized successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             
 ******************************************************************************/
en_result_t Ct_Pwc_DeInit(uint8_t u8Idx, boolean_t bTouchNvic)
{
    volatile M0P_CT_PWC_TypeDef* pstcCtPwc = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */

    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPwc = (volatile M0P_CT_PWC_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtPwc->TCR  = 0u;
    pstcCtPwc->TCR2 = 0u;
    pstcCtPwc->STA  = 0u;
    
    /* Clear callback */
    pstcCtInstanceData->stcInternData.pfnBit0IrqCb = NULL;
    pstcCtInstanceData->stcInternData.pfnBit1IrqCb = NULL;
    pstcCtInstanceData->stcInternData.pfnBit2IrqCb = NULL;
    
    if(TRUE == bTouchNvic)
    {
        CtDisableNvic();
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Enable PWC timer counting
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Ok             Enable PWC timer counting successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             
 ******************************************************************************/
en_result_t Ct_Pwc_EnableCount(uint8_t u8Idx)
{
    volatile M0P_CT_PWC_TypeDef* pstcCtPwc = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */

    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPwc = (volatile M0P_CT_PWC_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtPwc->TCR_f.ENCNT = 1u;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable PWC timer counting
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Ok             Disable PWC timer counting successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             
 ******************************************************************************/
en_result_t Ct_Pwc_DisableCount(uint8_t u8Idx)
{
    volatile M0P_CT_PWC_TypeDef* pstcCtPwc = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */

    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPwc = (volatile M0P_CT_PWC_TypeDef*)(pstcCtInstanceData->pstcInstance);
    pstcCtPwc->TCR_f.ENCNT = 0u;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get 16 bits measure data of PWC timer
 **
 ** This function reads measure data in 16-bit timer mode.
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Measure data       
 ******************************************************************************/
uint16_t Ct_Pwc_Get16BitMeasureData(uint8_t u8Idx)
{ 
    uint16_t u16RegDtbf;
    volatile M0P_CT_PWC_TypeDef* pstcCtPwc = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */

    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    
    pstcCtPwc = (volatile M0P_CT_PWC_TypeDef*)(pstcCtInstanceData->pstcInstance);
    u16RegDtbf = pstcCtPwc->DBUF;
    
    return u16RegDtbf;
}

/**
 *******************************************************************************
 ** \brief Get 32 bits measure data of PWC timer
 **
 ** This function reads measure data in 32-bit timer mode, the input CT instance
 ** should point to register base address of even channel. 
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval Measure data 
 ******************************************************************************/
uint32_t Ct_Pwc_Get32BitMeasureData(uint8_t u8Idx)
{ 
    uint16_t u16RegLow, u16RegHigh;
    volatile M0P_CT_PWC_TypeDef* pstcCtPwc = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */
    
    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_EVEN_CH(u8Idx)); 
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    
    pstcCtPwc = (volatile M0P_CT_PWC_TypeDef*)(pstcCtInstanceData->pstcInstance);
    u16RegLow = pstcCtPwc->DBUF;
    u16RegHigh = ((volatile M0P_CT_PWC_TypeDef*)((volatile uint8_t*)pstcCtPwc + 0x40u))->DBUF;
    return (((uint32_t)u16RegHigh<<16) | (uint32_t)u16RegLow);
}

/**
 *******************************************************************************
 ** \brief Enable PWC timer interrupt
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIrqSel        PWC timer interrupt type
 ** \arg         PwcMeasureCompleteIrq   PWC measure complete interrupt 
 ** \arg         PwcMeasureOverflowIrq   PWC overflow interrupt
 **
 ** \retval Ok             Enable PWC timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - enIrqSel is invalid
 **
 ******************************************************************************/
en_result_t Ct_Pwc_EnableIrq(uint8_t u8Idx, en_pwc_irq_sel_t enIrqSel)
{
    volatile M0P_CT_PWC_TypeDef* pstcCtPwc = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */

    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_PWC_IRQ_SEL(enIrqSel)); 
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPwc = (volatile M0P_CT_PWC_TypeDef*)(pstcCtInstanceData->pstcInstance);

    switch (enIrqSel)
    {
        case PwcMeasureCompleteIrq:
            pstcCtPwc->STA_f.MCIE = 1u;
            break;
        case PwcMeasureOverflowIrq:
            pstcCtPwc->STA_f.OVIE = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Disable PWC timer interrupt
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIrqSel        PWC timer interrupt type
 ** \arg         PwcMeasureCompleteIrq   PWC measure complete interrupt 
 ** \arg         PwcMeasureOverflowIrq   PWC overflow interrupt
 **
 ** \retval Ok             Disable PWC timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - enIrqSel is invalid
 **
 ******************************************************************************/
en_result_t Ct_Pwc_DisableIrq(uint8_t u8Idx, en_pwc_irq_sel_t enIrqSel)
{
    volatile M0P_CT_PWC_TypeDef* pstcCtPwc = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */

    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_PWC_IRQ_SEL(enIrqSel));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPwc = (volatile M0P_CT_PWC_TypeDef*)(pstcCtInstanceData->pstcInstance);
    
    switch (enIrqSel)
    {
        case PwcMeasureCompleteIrq:
            pstcCtPwc->STA_f.MCIE = 0u;
            break;
        case PwcMeasureOverflowIrq:
            pstcCtPwc->STA_f.OVIE = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get interrupt flag of PWC timer 
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIrqSel        PWC timer interrupt type
 ** \arg         PwcMeasureCompleteIrq   PWC measure complete interrupt 
 ** \arg         PwcMeasureOverflowIrq    PWC overflow interrupt
 **
 ** \retval      boolean_t         the interrupt status
 **
 ******************************************************************************/
boolean_t Ct_Pwc_GetIrqFlag(uint8_t u8Idx, en_pwc_irq_sel_t enIrqSel)
{
    boolean_t bFlag; 
    volatile M0P_CT_PWC_TypeDef* pstcCtPwc = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */

    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_PWC_IRQ_SEL(enIrqSel));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    
    pstcCtPwc = (volatile M0P_CT_PWC_TypeDef*)(pstcCtInstanceData->pstcInstance);
    
    switch (enIrqSel)
    {
        case PwcMeasureCompleteIrq:
            bFlag = (1u == pstcCtPwc->STA_f.MCIR) ? TRUE : FALSE;
            break;
        case PwcMeasureOverflowIrq:
            bFlag = (1u == pstcCtPwc->STA_f.OVIR) ? TRUE : FALSE;
            break;
        default:
            break;
    }
    
    return bFlag;
}

/**
 *******************************************************************************
 ** \brief Clear interrupt flag of PWC timer 
 **
 ** \param [in]  u8Idx            Channel index
 ** \param [in]  enIrqSel        PWC timer interrupt type
 ** \arg         PwcMeasureCompleteIrq   PWC measure complete interrupt
 ** \arg         PwcMeasureOverflowIrq    PWC overflow interrupt
 **
 ** \retval Ok              Clear interrupt flag successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                             - u8Idx is invalid
 **                             - Pointer to instance data is NULL
 **                             - enIrqSel is invalid
 **
 ******************************************************************************/
en_result_t Ct_Pwc_ClrIrqFlag(uint8_t u8Idx, en_pwc_irq_sel_t enIrqSel)
{
    volatile M0P_CT_PWC_TypeDef* pstcCtPwc = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */

    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_PWC_IRQ_SEL(enIrqSel));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    if(NULL == pstcCtInstanceData)
    {
        return ErrorInvalidParameter;
    }
    
    pstcCtPwc = (volatile M0P_CT_PWC_TypeDef*)(pstcCtInstanceData->pstcInstance);
  
    switch (enIrqSel)
    {
        case PwcMeasureCompleteIrq:
            pstcCtPwc->STA_f.MCIR = 0u;
            break;
        case PwcMeasureOverflowIrq:
            pstcCtPwc->STA_f.OVIR = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get error flag of PWC timer
 **
 ** If the measured data is not read out when the next data is coming,
 ** the error flag will be set.
 **
 ** \param [in]  u8Idx            Channel index
 **
 ** \retval         boolean_t       the error flag status
 ******************************************************************************/
boolean_t Ct_Pwc_GetErrorFlag(uint8_t u8Idx)
{
    boolean_t bFlag; 
    volatile M0P_CT_PWC_TypeDef* pstcCtPwc = NULL;
    stc_ct_instance_data_t* pstcCtInstanceData = NULL; /* Pointer to internal data */

    /* Check for CT channel */
    ASSERT(IS_VALID_CH(u8Idx));
    
    /* Get pointer to internal data structure ... */
    pstcCtInstanceData = CtGetInstanceDataPtr(u8Idx);
    ASSERT(IS_VALID_POINTER(pstcCtInstanceData));
    
    pstcCtPwc = (volatile M0P_CT_PWC_TypeDef*)(pstcCtInstanceData->pstcInstance);
    bFlag = (1u == pstcCtPwc->STA_f.ERR) ? TRUE : FALSE;
    
    return bFlag;
}

//@} // CtGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
