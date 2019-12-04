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
/** \file ct.h
 **
 ** A detailed description is available at
 ** @link CtGroup ct description @endlink
 **
 **   - 2017-8-10  1.0  WangM First version for Device Driver Library of ct.
 **
 ******************************************************************************/

#ifndef __CT_H__
#define __CT_H__

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
 ** \defgroup CtGroup Composite Timer (CT)
 **
 ******************************************************************************/

//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief Redefinition of CT register structure
 ******************************************************************************/
typedef M0P_CT_PWM_TypeDef stc_ctn_t;

/**
 *******************************************************************************
 ** \brief Ct channel number
 ******************************************************************************/
typedef enum en_ct_channel
{
    CtChannel0     = 0u,                ///< channel 0
    CtChannel1     = 1u,                ///< channel 1
    CtChannel2     = 2u,                ///< channel 2
    CtChannel3     = 3u,                ///< channel 3
    CtChannelMax   = 4u,                ///< channel max
} en_ct_channel_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define CT IO mode
 ******************************************************************************/
typedef enum en_ct_io_mode
{
    CtIoMode0 = 0u,                     ///< CT IO mode 0
    CtIoMode1 = 1u,                     ///< CT IO mode 1
    CtIoMode2 = 2u,                     ///< CT IO mode 2
    CtIoMode3 = 3u,                     ///< CT IO mode 3
    CtIoMode4 = 4u,                     ///< CT IO mode 4
    CtIoMode5 = 5u,                     ///< CT IO mode 5
    CtIoMode6 = 6u,                     ///< CT IO mode 6
    CtIoMode7 = 7u,                     ///< CT IO mode 7
    CtIoMode8 = 8u,                     ///< CT IO mode 8
}en_ct_io_mode_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define timer mode of CT
 ******************************************************************************/
typedef enum en_ct_timer_mode
{
    CtResetMode = 0u,                   ///< Reset mode
    CtPwmMode   = 1u,                   ///< PWM timer mode
    CtPpgMode   = 2u,                   ///< PPG timer mode
    CtRtMode    = 3u,                   ///< Reload timer mode
    CtPwcMode   = 4u,                   ///< PWC timer mode
}en_ct_timer_mode_t;

/**
 *******************************************************************************
 ** \brief CT internal data
 ******************************************************************************/
typedef struct stc_ct_intern_data
{
    func_ptr_t  pfnBit0IrqCb;           ///< Pointer to callback function the interrupt reflected by bit0 of STC
    func_ptr_t  pfnBit1IrqCb;           ///< Pointer to callback function the interrupt reflected by bit1 of STC
    func_ptr_t  pfnBit2IrqCb;           ///< Pointer to callback function the interrupt reflected by bit2 of STC
} stc_ct_intern_data_t ;

/**
 *******************************************************************************
 ** \brief CT instance data
 ******************************************************************************/
typedef struct stc_ct_instance_data
{
    const uint32_t       u32Idx;        ///< channel index
    volatile stc_ctn_t*  pstcInstance;  ///< pointer to registers of an instance
    stc_ct_intern_data_t stcInternData; ///< module internal data of instance
} stc_ct_instance_data_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define clock prescaler of PWM timer
 ******************************************************************************/
typedef enum en_pwm_clock_pres
{
    PwmPresNone            = 0u,        ///< PWM prescaler clock: no division
    PwmPres1Div4           = 1u,        ///< PWM prescaler clock: 1/4
    PwmPres1Div16          = 2u,        ///< PWM prescaler clock: 1/16
    PwmPres1Div128         = 3u,        ///< PWM prescaler clock: 1/128
    PwmPres1Div256         = 4u,        ///< PWM prescaler clock: 1/256
    PwmPres1ExtClkRising   = 5u,        ///< Use external clock and count at rising edge
    PwmPres1ExtClkFalling  = 6u,        ///< Use external clock and count at falling edge
    PwmPres1ExtClkBoth     = 7u,        ///< Use external clock and count at both edge
    PwmPres1Div512         = 8u,        ///< PWM prescaler clock: 1/512
    PwmPres1Div1024        = 9u,        ///< PWM prescaler clock: 1/1024
    PwmPres1Div2048        = 10u,       ///< PWM prescaler clock: 1/2048
}en_pwm_clock_pres_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define the configuration of PWM restart option
 ******************************************************************************/
typedef enum en_pwm_restart_enable
{
    PwmRestartDisable = 0u,             ///< Disable PWM restart
    PwmRestartEnable  = 1u,             ///< Enable PWM restart
}en_pwm_restart_enable_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define the configuration of PWM output mask
 ******************************************************************************/
typedef enum en_pwm_output_mask
{
    PwmOutputNormal = 0u,               ///< Output normal PWM wave
    PwmOutputMask   = 1u,               ///< Mask PWM wave
}en_pwm_output_mask_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define external trigger of PWM
 ******************************************************************************/
typedef enum en_pwm_ext_trig
{
    PwmExtTrigDisable  = 0u,            ///< Disable external trigger
    PwmExtTrigRising   = 1u,            ///< Enable external trigger with rising edge
    PwmExtTrigFalling  = 2u,            ///< Enable external trigger with falling edge
    PwmExtTrigBoth     = 3u,            ///< Enable external trigger with both edge
}en_pwm_ext_trig_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define PWM initial polarity
 ******************************************************************************/
typedef enum en_pwm_output_polarity
{
    PwmPolarityLow    = 0u,             ///< Initial polarity: low
    PwmPolarityHigh   = 1u,             ///< Initial polarity: high
}en_pwm_output_polarity_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define PWM timer mode
 ******************************************************************************/
typedef enum en_pwm_mode
{
    PwmContinuous = 0u,                 ///< Continuous mode
    PwmOneshot    = 1u,                 ///< One-shot mode
}en_pwm_mode_t;

/**
 *******************************************************************************
 ** \brief structure to enable the PWM interrupts
 ******************************************************************************/
typedef struct stc_pwm_irq_en
{
    boolean_t bPwmTrigIrq;              ///< Trigger interrupt selection
    boolean_t bPwmDutyMatchIrq;         ///< Duty match interrupt selection
    boolean_t bPwmUnderflowIrq;         ///< Underflow interrupt selection
} stc_pwm_irq_en_t;

/**
 *******************************************************************************
 ** \brief enumeration to select the PWM interrupt
 ******************************************************************************/
typedef enum en_pwm_irq_sel
{
    PwmTrigIrq          = 0u,           ///< PWM trigger interrupt
    PwmDutyMatchIrq     = 1u,           ///< PWM duty match interrupt
    PwmUnderflowIrq     = 2u,           ///< PWM underflow interrupt
} en_pwm_irq_sel_t;

/**
 *******************************************************************************
 ** \brief structure to set the PWM interrupt callback function
 ******************************************************************************/
typedef struct stc_pwm_irq_cb
{
    func_ptr_t pfnPwmTrigIrqCb;         ///< Pointer to trigger interrupt callback function
    func_ptr_t pfnPwmDutyMatchIrqCb;    ///< Pointer to duty match interrupt callback function
    func_ptr_t pfnPwmUnderflowIrqCb;    ///< Pointer to underflow interrupt callback function
}stc_pwm_irq_cb_t;

/**
 *******************************************************************************
 ** \brief Structure of PWM configuration
 ******************************************************************************/
typedef struct stc_ct_pwm_config
{
    en_pwm_clock_pres_t      enPres;          ///< Clock prescaler
    en_pwm_restart_enable_t  enRestartEn;     ///< Restart enable setting
    en_pwm_output_mask_t     enOutputMask;    ///< PWM output mask setting
    en_pwm_ext_trig_t        enExtTrig;       ///< PWM external trigger setting
    en_pwm_output_polarity_t enOutputPolarity;///< PWM output polarity setting
    en_pwm_mode_t            enMode;          ///< Continuous mode or one-shot mode
    stc_pwm_irq_en_t*        pstcPwmIrqEn;    ///< Pointer to interrupt request enable setting structure of Composite Timer with PWM mode
    stc_pwm_irq_cb_t*        pstcPwmIrqCb;    ///< Pointer to interrupt request callback functions structure of Composite Timer with PWM mode
    boolean_t                bTouchNvic;      ///< TRUE: enable NVIC, FALSE: don't enable NVIC
}stc_ct_pwm_config_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define clock prescaler of PPG timer
 ******************************************************************************/
typedef enum en_ppg_clock_pres
{
    PpgPresNone              = 0u,      ///< PPG prescaler clock: no division
    PpgPres1Div4             = 1u,      ///< PPG prescaler clock: 1/4
    PpgPres1Div16            = 2u,      ///< PPG prescaler clock: 1/16
    PpgPres1Div128           = 3u,      ///< PPG prescaler clock: 1/128
    PpgPres1Div256           = 4u,      ///< PPG prescaler clock: 1/256
    PpgPres1ExtClkRising     = 5u,      ///< Use external clock and count at rising edge
    PpgPres1ExtClkFalling    = 6u,      ///< Use external clock and count at falling edge
    PpgPres1ExtClkBoth       = 7u,      ///< Use external clock and count at both edge
    PpgPres1Div512           = 8u,      ///< PPG prescaler clock: 1/512
    PpgPres1Div1024          = 9u,      ///< PPG prescaler clock: 1/1024
    PpgPres1Div2048          = 10u,     ///< PPG prescaler clock: 1/2048
}en_ppg_clock_pres_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define the configuration of PPG restart option
 ******************************************************************************/
typedef enum en_ppg_restart_enable
{
    PpgRestartDisable = 0u,             ///< Disable PPG restart
    PpgRestartEnable  = 1u,             ///< Enable PPG restart
}en_ppg_restart_enable_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define the configuration of PPG output mask
 ******************************************************************************/
typedef enum en_ppg_output_mask
{
    PpgOutputNormal = 0u,               ///< Output normal PPG wave
    PpgOutputMask   = 1u,               ///< Mask PPG wave
}en_ppg_output_mask_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define external trigger of PPG
 ******************************************************************************/
typedef enum en_ppg_ext_trig
{
    PpgExtTrigDisable   = 0u,           ///< Disable external trigger
    PpgExtTrigRising    = 1u,           ///< Enable external trigger with rising edge
    PpgExtTrigFalling   = 2u,           ///< Enable external trigger with falling edge
    PpgExtTrigBoth      = 3u,           ///< Enable external trigger with both edge
}en_ppg_ext_trig_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define PWM initial polarity
 ******************************************************************************/
typedef enum en_ppg_output_polarity
{
    PpgPolarityLow   = 0u,              ///< Initial polarity: low
    PpgPolarityHigh  = 1u,              ///< Initial polarity: high
}en_ppg_output_polarity_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define PWM timer mode
 ******************************************************************************/
typedef enum en_ppg_mode
{
    PpgContinuous = 0u,                 ///< Continuous mode
    PpgOneshot    = 1u,                 ///< One-shot mode
}en_ppg_mode_t;

/**
 *******************************************************************************
 ** \brief structure to enable the PPG interrupts
 ******************************************************************************/
typedef struct stc_ppg_irq_en
{
    boolean_t bPpgTrigIrq;              ///< PPG trigger interrupt selection
    boolean_t bPpgUnderflowIrq;         ///< PPG underflow interrupt selection
} stc_ppg_irq_en_t;

/**
 *******************************************************************************
 ** \brief enumeration to select the PPG interrupt
 ******************************************************************************/
typedef enum en_ppg_irq_sel
{
    PpgTrigIrq      = 0u,               ///< PPG trigger interrupt
    PpgUnderflowIrq = 1u,               ///< PPG underflow interrupt
} en_ppg_irq_sel_t;

/**
 *******************************************************************************
 ** \brief structure to set the PPG interrupt callback function
 ******************************************************************************/
typedef struct stc_ppg_irq_cb
{
    func_ptr_t pfnPpgTrigIrqCb;         ///< Pointer to PPG trigger interrupt callback function
    func_ptr_t pfnPpgUnderflowIrqCb;    ///< Pointer to PPG underflow interrupt callback function
}stc_ppg_irq_cb_t;

/**
 *******************************************************************************
 ** \brief Structure of PPG configuration
 ******************************************************************************/
typedef struct stc_ct_ppg_config
{
    en_ppg_clock_pres_t      enPres;          ///< Clock prescaler
    en_ppg_restart_enable_t  enRestartEn;     ///< Restart enable setting
    en_ppg_output_mask_t     enOutputMask;    ///< PPG output mask setting
    en_ppg_ext_trig_t        enExtTrig;       ///< PPG external trigger setting
    en_ppg_output_polarity_t enOutputPolarity;///< PPG output polarity setting
    en_ppg_mode_t            enMode;          ///< Continuous mode or one-shot mode
    stc_ppg_irq_en_t*        pstcPpgIrqEn;    ///< Pointer to interrupt request enable setting structure of Composite Timer with PPG mode
    stc_ppg_irq_cb_t*        pstcPpgIrqCb;    ///< Pointer to interrupt request callback functions structure of Composite Timer with PPG mode
    boolean_t                bTouchNvic;      ///< TRUE: enable NVIC, FALSE: don't enable NVIC
}stc_ct_ppg_config_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define clock prescaler of Reload timer
 ******************************************************************************/
typedef enum en_rt_clock_pres
{
    RtPresNone              = 0u,       ///< Reload timer prescaler clock: no division
    RtPres1Div4             = 1u,       ///< Reload timer prescaler clock: 1/4
    RtPres1Div16            = 2u,       ///< Reload timer prescaler clock: 1/16
    RtPres1Div128           = 3u,       ///< Reload timer prescaler clock: 1/128
    RtPres1Div256           = 4u,       ///< Reload timer prescaler clock: 1/256
    RtPres1ExtClkRising     = 5u,       ///< Use external clock and count at rising edge
    RtPres1ExtClkFalling    = 6u,       ///< Use external clock and count at falling edge
    RtPres1ExtClkBoth       = 7u,       ///< Use external clock and count at both edge
    RtPres1Div512           = 8u,       ///< Reload timer prescaler clock: 1/512
    RtPres1Div1024          = 9u,       ///< Reload timer prescaler clock: 1/1024
    RtPres1Div2048          = 10u,      ///< Reload timer prescaler clock: 1/2048
}en_rt_clock_pres_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define external trigger of Reload timer
 ******************************************************************************/
typedef enum en_rt_ext_trigger
{
    RtExtTiggerDisable      = 0u,       ///< Disable external trigger
    RtExtTiggerRisingEdge   = 1u,       ///< Enable external trigger with rising edge
    RtExtTiggerFallingEdge  = 2u,       ///< Enable external trigger with falling edge
    RtExtTiggerBothEdge     = 3u,       ///< Enable external trigger with both edge
    RtExtTiggerLowLevel     = 4u,       ///< Enable external trigger with low level
    RtExtTiggerHighLevel    = 5u,       ///< Enable external trigger with high level
}en_rt_ext_trigger_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define Reload timer output initial polarity
 ******************************************************************************/
typedef enum en_rt_output_polarity
{
    RtPolarityLow   = 0u,               ///< Initial polarity: low
    RtPolarityHigh  = 1u,               ///< Initial polarity: high
}en_rt_output_polarity_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define Reload timer mode
 ******************************************************************************/
typedef enum en_rt_mode
{
    RtReload    = 0u,                   ///< Reload mode
    RtOneshot   = 1u,                   ///< One-shot mode
}en_rt_mode_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define Reload timer size
 ******************************************************************************/
typedef enum en_rt_timer_size
{
    RtSize16Bit = 0u,                   ///< 16-bit mode
    RtSize32Bit = 1u,                   ///< 32-bit mode
}en_rt_timer_size_t;

/**
 *******************************************************************************
 ** \brief structure to enable the Reload timer interrupts
 ******************************************************************************/
typedef struct stc_rt_irq_en
{
    boolean_t bRtTrigIrq;               ///< Trigger interrupt selection
    boolean_t bRtUnderflowIrq;          ///< Underflow interrupt selection
} stc_rt_irq_en_t;

/**
 *******************************************************************************
 ** \brief enumeration to select the Reload timer interrupt
 ******************************************************************************/
typedef enum en_rt_irq_sel
{
    RtTrigIrq      = 0u,                ///< Reload Timer trigger interrupt
    RtUnderflowIrq = 1u,                ///< Reload Timer underflow interrupt
} en_rt_irq_sel_t;

/**
 *******************************************************************************
 ** \brief structure to set the Reload timer interrupt callback function
 ******************************************************************************/
typedef struct stc_rt_irq_cb
{
    func_ptr_t pfnRtTrigIrqCb;          ///< Pointer to trigger interrupt callback function
    func_ptr_t pfnRtUnderflowIrqCb;     ///< Pointer to underflow interrupt callback function
}stc_rt_irq_cb_t;

/**
 *******************************************************************************
 ** \brief Structure of Reload timer configuration
 ******************************************************************************/
typedef struct stc_ct_rt_config
{
    en_rt_clock_pres_t      enPres;          ///< Clock prescaler
    en_rt_timer_size_t      enSize;          ///< 16-bit or 32-bit
    en_rt_ext_trigger_t     enExtTrig;       ///< External trigger setting
    en_rt_output_polarity_t enOutputPolarity;///< Output polarity setting
    en_rt_mode_t            enMode;          ///< Reload mode or one-shot mode
    stc_rt_irq_en_t*        pstcRtIrqEn;     ///< Pointer to interrupt request enable setting structure of Composite Timer with Reload TImer mode
    stc_rt_irq_cb_t*        pstcRtIrqCb;     ///< Pointer to interrupt request callback functions structure of Composite Timer with Reload TImer mode
    boolean_t               bTouchNvic;      ///< TRUE: enable NVIC, FALSE: don't enable NVIC
}stc_ct_rt_config_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define clock prescaler of PWC timer
 ******************************************************************************/
typedef enum en_pwc_clock_pres
{
    PwcPresNone     = 0u,               ///< PWC timer prescaler clock: no division
    PwcPres1Div4    = 1u,               ///< PWC timer prescaler clock: 1/4
    PwcPres1Div16   = 2u,               ///< PWC timer prescaler clock: 1/16
    PwcPres1Div128  = 3u,               ///< PWC timer prescaler clock: 1/128
    PwcPres1Div256  = 4u,               ///< PWC timer prescaler clock: 1/256
    PwcPres1Div512  = 8u,               ///< PWC timer prescaler clock: 1/512
    PwcPres1Div1024 = 9u,               ///< PWC timer prescaler clock: 1/1024
    PwcPres1Div2048 = 10u,              ///< PWC timer prescaler clock: 1/2048
}en_pwc_clock_pres_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define measure mode of PWC timer
 ******************************************************************************/
typedef enum en_pwc_measure_edge
{
    PwcMeasureRisingToFalling   = 0u,   ///< Measure between rising edge with falling edge
    PwcMeasureRisingToRising    = 1u,   ///< Measure between rising edge with rising edge
    PwcMeasureFallingToFalling  = 2u,   ///< Measure between falling edge with falling edge
    PwcMeasureEitherToEither    = 3u,   ///< Measure between either edge with either edge
    PwcMeasureFallingToRising   = 4u,   ///< Measure between falling edge with falling edge
}en_pwc_measure_edge_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define PWC timer mode
 ******************************************************************************/
typedef enum en_pwc_mode
{
    PwcContinuous = 0u,                 ///< Continuous mode
    PwcOneshot    = 1u,                 ///< One-shot mode
}en_pwc_mode_t;

/**
 *******************************************************************************
 ** \brief Enumeration to define Reload timer size
 ******************************************************************************/
typedef enum en_pwc_timer_size
{
    PwcSize16Bit = 0u,                  ///< 16-bit mode
    PwcSize32Bit = 1u,                  ///< 32-bit mode
}en_pwc_timer_size_t;

/**
 *******************************************************************************
 ** \brief structure to enable the PWC timer interrupts
 ******************************************************************************/
typedef struct stc_pwc_irq_en
{
    boolean_t bPwcMeasureCompleteIrq;   ///< PWC measure completion IRQ
    boolean_t bPwcMeasureOverflowIrq;   ///< PWC measure overflow IRQ
}stc_pwc_irq_en_t;

/**
 *******************************************************************************
 ** \brief structure to select the PWC timer interrupt
 ******************************************************************************/
typedef enum en_pwc_irq_sel
{
    PwcMeasureCompleteIrq = 0u,         ///< PWC measure completion IRQ
    PwcMeasureOverflowIrq = 1u,         ///< PWC measure overflow IRQ
} en_pwc_irq_sel_t;

/**
 *******************************************************************************
 ** \brief structure to set the PWC timer interrupt callback function
 ******************************************************************************/
typedef struct stc_pwc_irq_cb
{
    func_ptr_t pfnPwcMeasureCompleteIrqCb;///< Poiter to PWC measure completion callback function
    func_ptr_t pfnPwcMeasureOverflowIrqCb;///< Poiter to PWC measure overflow callback function
}stc_pwc_irq_cb_t;

/**
 *******************************************************************************
 ** \brief Structure of PWC timer configuration
 ******************************************************************************/
typedef struct stc_ct_pwc_config
{
    en_pwc_clock_pres_t   enPres;       ///< Clock prescaler
    en_pwc_timer_size_t   enSize;       ///< 16-bit or 32-bit
    en_pwc_measure_edge_t enMeasureEdge;///< Measure mode setting
    en_pwc_mode_t         enMode;       ///< Continuous mode or one-shot mode
    stc_pwc_irq_en_t*     pstcPwcIrqEn; ///< Pointer to interrupt request enable setting structure of Composite Timer with PWC mode
    stc_pwc_irq_cb_t*     pstcPwcIrqCb; ///< Pointer to interrupt request callback functions structure of Composite Timer with PWC mode
    boolean_t             bTouchNvic;   ///< TRUE: enable NVIC, FALSE: don't enable NVIC
}stc_ct_pwc_config_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/

/* 1. Configure CT IO mode  */
extern en_result_t Ct_ConfigIOMode(uint8_t u8Idx, en_ct_io_mode_t enIoMode);

/* 2. Start/stop CT simultaneously  */
extern void Ct_SetSimultaneousStart(uint8_t u8Value);

/* 3. Timer configuration */
/* 3.1 PWM timer */
/* Init */
extern en_result_t Ct_Pwm_Init(uint8_t u8Idx, stc_ct_pwm_config_t* pstcPwmConfig);
extern en_result_t Ct_Pwm_DeInit(uint8_t u8Idx, boolean_t bTouchNvic);
/* Function Enable/Disable */
extern en_result_t Ct_Pwm_EnableCount(uint8_t u8Idx);
extern en_result_t Ct_Pwm_DisableCount(uint8_t u8Idx);
extern en_result_t Ct_Pwm_EnableSwTrig(uint8_t u8Idx);
/* Write/read count */
extern en_result_t Ct_Pwm_WriteCycleVal(uint8_t u8Idx, uint16_t u16Cycle);
extern en_result_t Ct_Pwm_WriteDutyVal(uint8_t u8Idx, uint16_t u16Duty);
extern uint16_t Ct_Pwm_ReadCurCnt(uint8_t u8Idx);
/* Irq Enable/Disable */
extern en_result_t Ct_Pwm_EnableIrq(uint8_t u8Idx, en_pwm_irq_sel_t enIrqSel);
extern en_result_t Ct_Pwm_DisableIrq(uint8_t u8Idx, en_pwm_irq_sel_t enIrqSel);
/* Interrupt flag read/clear */
extern boolean_t Ct_Pwm_GetIrqFlag(uint8_t u8Idx, en_pwm_irq_sel_t enIrqSel);
extern en_result_t Ct_Pwm_ClrIrqFlag(uint8_t u8Idx, en_pwm_irq_sel_t enIrqSel);

/* 3.2 PPG timer */
/* Init */
extern en_result_t Ct_Ppg_Init(uint8_t u8Idx, stc_ct_ppg_config_t* pstcPpgConfig);
extern en_result_t Ct_Ppg_DeInit(uint8_t u8Idx, boolean_t bTouchNvic);
/* Function Enable/Disable */
extern en_result_t Ct_Ppg_EnableCount(uint8_t u8Idx);
extern en_result_t Ct_Ppg_DisableCount(uint8_t u8Idx);
extern en_result_t Ct_Ppg_EnableSwTrig(uint8_t u8Idx);
/* Write/read count */
extern en_result_t Ct_Ppg_WriteLowWidthVal(uint8_t u8Idx, uint16_t u16Val);
extern en_result_t Ct_Ppg_WriteHighWidthVal(uint8_t u8Idx, uint16_t u16Val);
extern uint16_t Ct_Ppg_ReadCurCnt(uint8_t u8Idx);
/* Irq Enable/Disable */
extern en_result_t Ct_Ppg_EnableIrq(uint8_t u8Idx, en_ppg_irq_sel_t enIrqSel);
extern en_result_t Ct_Ppg_DisableIrq(uint8_t u8Idx, en_ppg_irq_sel_t enIrqSel);
/* Interrupt flag read/clear */
extern boolean_t Ct_Ppg_GetIrqFlag(uint8_t u8Idx, en_ppg_irq_sel_t enIrqSel);
extern en_result_t Ct_Ppg_ClrIrqFlag(uint8_t u8Idx, en_ppg_irq_sel_t enIrqSel);

/* 3.3 RT timer */
/* Init */
extern en_result_t Ct_Rt_Init(uint8_t u8Idx, stc_ct_rt_config_t* pstcRtConfig);
extern en_result_t Ct_Rt_DeInit(uint8_t u8Idx, boolean_t bTouchNvic);
/* Function/Irq Enable/Disable */
extern en_result_t Ct_Rt_EnableCount(uint8_t u8Idx);
extern en_result_t Ct_Rt_DisableCount(uint8_t u8Idx);
extern en_result_t Ct_Rt_EnableSwTrig(uint8_t u8Idx);
/* Write/read count */
extern en_result_t Ct_Rt_WriteCycleVal(uint8_t u8Idx, uint16_t u16Val);
extern uint16_t Ct_Rt_ReadCurCnt(uint8_t u8Idx);
/* Irq Enable/Disable */
extern en_result_t Ct_Rt_EnableIrq(uint8_t u8Idx, en_rt_irq_sel_t enIrqSel);
extern en_result_t Ct_Rt_DisableIrq(uint8_t u8Idx, en_rt_irq_sel_t enIrqSel);
/* Interrupt flag read/clear */
extern boolean_t Ct_Rt_GetIrqFlag(uint8_t u8Idx, en_rt_irq_sel_t enIrqSel);
extern en_result_t Ct_Rt_ClrIrqFlag(uint8_t u8Idx, en_rt_irq_sel_t enIrqSel);

/* 3.4 PWC timer */
/* Init */
extern en_result_t Ct_Pwc_Init(uint8_t u8Idx, stc_ct_pwc_config_t* pstcPwcConfig);
extern en_result_t Ct_Pwc_DeInit(uint8_t u8Idx, boolean_t bTouchNvic);
/* Function/Irq Enable/Disable */
extern en_result_t Ct_Pwc_EnableCount(uint8_t u8Idx);
extern en_result_t Ct_Pwc_DisableCount(uint8_t u8Idx);
/* Read count */
extern uint16_t Ct_Pwc_Get16BitMeasureData(uint8_t u8Idx);
extern uint32_t Ct_Pwc_Get32BitMeasureData(uint8_t u8Idx);
/* Irq Enable/Disable */
extern en_result_t Ct_Pwc_EnableIrq(uint8_t u8Idx, en_pwc_irq_sel_t  enIrqSel);
extern en_result_t Ct_Pwc_DisableIrq(uint8_t u8Idx, en_pwc_irq_sel_t enIrqSel);
/* Interrupt flag read/clear */
extern boolean_t Ct_Pwc_GetIrqFlag(uint8_t u8Idx, en_pwc_irq_sel_t enIrqSel);
extern en_result_t Ct_Pwc_ClrIrqFlag(uint8_t u8Idx, en_pwc_irq_sel_t enIrqSel);
extern boolean_t Ct_Pwc_GetErrorFlag(uint8_t u8Idx);

//@} // CtGroup

#ifdef __cplusplus
}
#endif

#endif // #if  __CT_H__
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
