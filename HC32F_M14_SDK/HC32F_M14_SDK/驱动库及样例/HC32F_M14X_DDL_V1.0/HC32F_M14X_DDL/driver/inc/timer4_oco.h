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
/** \file timer4_oco.h
 **
 ** A detailed description is available at
 ** @link Timer4OcoGroup Timer4Oco description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     Timer4Oco
 **
 ******************************************************************************/

#ifndef __TIMER4_OCO_H__
#define __TIMER4_OCO_H__

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
 ** \defgroup Timer4OcoGroup Timer4 Output Compare & Output(Timer4Oco)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief output level of the OP pin state
 ******************************************************************************/
typedef enum en_oco_op_out_state
{
    OpLowLevel  = 0u,                   ///< output low level to OP pin
    OpHighLevel = 1u,                   ///< output high level to OP pin
}en_oco_op_out_state_t;

/**
 *******************************************************************************
 ** \brief buffer register function of OCCR
 ******************************************************************************/
typedef enum en_oco_occr_buf
{
    OccrBufDisable           = 0u,      ///< disable the buffer function
    OccrBufTrsfByCntZero     = 1u,      ///< buffer transfer when counter value is 0x0000
    OccrBufTrsfByCntPeak     = 2u,      ///< buffer transfer when counter value is CPSR
    OccrBufTrsfByCntZeroPeak = 3u,      ///< buffer transfer when the value is both 0 and CPSR
    OccrBufTrsfByCntZeroMszcZero = 4u,  ///< buffer transfer when counter value is 0x0000 and zero value detection mask counter value is 0
    OccrBufTrsfByCntPeakMspcZero = 5u,  ///< buffer transfer when counter value is CPSR and peak value detection mask counter value is 0
    OccrBufTrsfByCntZeroMszcZeroOrCntPeakMspcZero = 6u, ///< buffer transfer when counter value is 0x0000 and zero value detection mask counter value is 0 or
                                                        ///< when counter value is CPSR and peak value detection mask counter value is 0
}en_oco_occr_buf_t;

/**
 *******************************************************************************
 ** \brief buffer register function of OCMR
 ******************************************************************************/
typedef enum en_oco_ocmr_buf
{
    OcmrBufDisable           = 0u,      ///< disable the buffer function
    OcmrBufTrsfByCntZero     = 1u,      ///< buffer transfer when counter value is 0x0000
    OcmrBufTrsfByCntPeak     = 2u,      ///< buffer transfer when counter value is CPSR
    OcmrBufTrsfByCntZeroPeak = 3u,      ///< buffer transfer when the value is both 0 and CPSR
    OcmrBufTrsfByCntZeroMszcZero = 4u,  ///< buffer transfer when CNT counter value is 0x0000 and zero value detection mask counter value is 0
    OcmrBufTrsfByCntPeakMspcZero = 5u,  ///< buffer transfer when CNT counter value is CPSR and peak value detection mask counter value is 0
    OcmrBufTrsfByCntZeroMszcZeroOrCntPeakMspcZero = 6u, ///< buffer transfer when CNT counter value is 0x0000 and zero value detection mask counter value is 0 or
                                                        ///< when CNT counter value is CPSR and peak value detection mask counter value is 0
}en_oco_ocmr_buf_t;

/**
 *******************************************************************************
 ** \brief OCO configure
 ******************************************************************************/
typedef struct stc_timer4_oco_config
{
    en_oco_ocmr_buf_t   enOcmrBufMode;  ///< buffer register function of OCMR
    en_oco_occr_buf_t   enOccrBufMode;  ///< buffer register function of OCCR
    en_oco_op_out_state_t enPinState;   ///< OP output level state

    boolean_t  bIrqEnable;              ///< TRUE: enable OCO interrupt, FALSE: disable OCO interrupt
    func_ptr_t pfnIrqCallback;          ///< Pointer to OCO interrupt callback function.
    boolean_t  bTouchNvic;              ///< TRUE: enable NVIC, FALSE: disable NVIC
}stc_timer4_oco_config_t;

/**
 *******************************************************************************
 ** \brief OP output status
 ******************************************************************************/
typedef enum en_op_status
{
    OpOutputHold     = 0u,             ///< OP output hold
    OpOutputHigh     = 1u,             ///< OP output high
    OpOutputLow      = 2u,             ///< OP output low
    OpOutputReverse  = 3u,             ///< OP output reverse
}en_op_even_status_t, en_op_odd_status_t;

/**
 *******************************************************************************
 ** \brief The condition for OCF set
 ******************************************************************************/
typedef enum en_ocf_set_condition
{
    OcfHold = 0u,                       ///< OCF hold
    OcfSet  = 1u,                       ///< OCF set
}en_ocf_even_t, en_ocf_odd_t;

/**
 *******************************************************************************
 ** \brief The compare configuration of even OCO channel
 ******************************************************************************/
typedef struct stc_oco_even_compare_config
{
    en_op_even_status_t enCntZeroEvenMatchEvenChOpStatus;       ///< Even channel's OP output status when even channel match occurs at the condition of CNT count=0x0000
    en_op_even_status_t enCntZeroEvenNotMatchEvenChOpStatus;    ///< Even channel's OP output status when even channel match doesn't occur at the condition of CNT count=0x0000

    en_op_even_status_t enCntUpCntEvenMatchEvenChOpStatus;      ///< Even channel's OP output status when even channel match occurs at the condition of CNT is counting up

    en_op_even_status_t enCntPeakEvenMatchEvenChOpStatus;       ///< Even channel's OP output status when even channel match occurs at the condition of CNT count=Peak
    en_op_even_status_t enCntPeakEvenNotMatchEvenChOpStatus;    ///< Even channel's OP output status when even channel match doesn't occur at the condition of CNT count=Peak

    en_op_even_status_t enCntDownCntEvenMatchEvenChOpStatus;    ///< Even channel's OP output status when even channel match occurs at the condition of CNT is counting down

    en_ocf_even_t enOcfWhenCntZeroEvenMatch;    ///< Even channel OCO's OCF status when even channel match occurs at the condition of CNT count=0x0000
    en_ocf_even_t enOcfWhenCntUpCntEvenMatch;   ///< Even channel OCO's OCF status when even channel match occurs at the condition of CNT is counting up
    en_ocf_even_t enOcfWhenCntPeakEvenMatch;    ///< Even channel OCO's OCF status when even channel match occurs at the condition of CNT count=Peak
    en_ocf_even_t enOcfWhenCntDownCntEvenMatch; ///< Even channel OCO's OCF status when even channel match occurs at the condition of CNT is counting down

    boolean_t bCntZeroPeakExtendMatchCondition; ///< TRUE:  The condition of that OCCR > CNT cycle value will be treated as comparison match
                                                ///< FALSE: The condition of that OCCR > CNT counter value will be treated as comparison not match
}stc_oco_even_compare_config_t;

/**
 *******************************************************************************
 ** \brief The compare configuration of odd OCO channel
 ******************************************************************************/
typedef struct stc_oco_odd_compare_config
{
    en_op_odd_status_t enCntZeroOddMatchEvenMatchOddChOpStatus;         ///< Odd channel's OP output status when even channel and odd channel match occurs at the condition of CNT count=0x0000
    en_op_odd_status_t enCntZeroOddMatchEvenNotMatchOddChOpStatus;      ///< Odd channel's OP output status when even channel not match and odd channel match occurs at the condition of CNT count=0x0000
    en_op_odd_status_t enCntZeroOddNotMatchEvenMatchOddChOpStatus;      ///< Odd channel's OP output status when even channel match and odd channel not match occurs at the condition of CNT count=0x0000
    en_op_odd_status_t enCntZeroOddNotMatchEvenNotMatchOddChOpStatus;   ///< Odd channel's OP output status when even channel not match and odd channel not match occurs at the condition of CNT count=0x0000

    en_op_odd_status_t enCntUpCntOddMatchEvenMatchOddChOpStatus;        ///< Odd channel's OP output status when even channel and odd channel match occurs at the condition of CNT is counting up
    en_op_odd_status_t enCntUpCntOddMatchEvenNotMatchOddChOpStatus;     ///< Odd channel's OP output status when even channel not match and odd channel match occurs at the condition of CNT is counting up
    en_op_odd_status_t enCntUpCntOddNotMatchEvenMatchOddChOpStatus;     ///< Odd channel's OP output status when even channel match and odd channel not match occurs at the condition of CNT is counting up

    en_op_odd_status_t enCntPeakOddMatchEvenMatchOddChOpStatus;         ///< Odd channel's OP output status when even channel and odd channel match occurs at the condition of CNT count=Peak
    en_op_odd_status_t enCntPeakOddMatchEvenNotMatchOddChOpStatus;      ///< Odd channel's OP output status when even channel not match and odd channel match occurs at the condition of CNT count=Peak
    en_op_odd_status_t enCntPeakOddNotMatchEvenMatchOddChOpStatus;      ///< Odd channel's OP output status when even channel match and odd channel not match occurs at the condition of CNT count=Peak
    en_op_odd_status_t enCntPeakOddNotMatchEvenNotMatchOddChOpStatus;   ///< Odd channel's OP output status when even channel not match and odd channel not match occurs at the condition of CNT count=Peak

    en_op_odd_status_t enCntDownOddMatchEvenMatchOddChOpStatus;         ///< Odd channel's OP output status when even channel and odd channel match occurs at the condition of CNT is counting down
    en_op_odd_status_t enCntDownOddMatchEvenNotMatchOddChOpStatus;      ///< Odd channel's OP output status when even channel not match and odd channel match occurs at the condition of CNT is counting down
    en_op_odd_status_t enCntDownOddNotMatchEvenMatchOddChOpStatus;      ///< Odd channel's OP output status when even channel match and odd channel not match occurs at the condition of CNT is coutning down

    en_ocf_odd_t enOcfWhenCntZeroOddMatch;      ///< Odd channel OCO's OCF status when Odd channel match occurs at the condition of CNT count=0x0000
    en_ocf_odd_t enOcfWhenCntUpCntOddMatch;     ///< Odd channel OCO's OCF status when Odd channel match occurs at the condition of CNT is counting up
    en_ocf_odd_t enOcfWhenCntPeakOddMatch;      ///< Odd channel OCO's OCF status when Odd channel match occurs at the condition of CNT count=Peak
    en_ocf_odd_t enOcfWhenCntDownCntOddMatch;   ///< Odd channel OCO's OCF status when Odd channel match occurs at the condition of CNT is counting down

    boolean_t bCntZeroPeakExtendMatchCondition; ///< TRUE:  The condition of that OCCR > CNT cycle value will be treated as comparison match
                                                ///< FALSE: The condition of that OCCR > CNT counter value will be treated as comparison not match
}stc_oco_odd_compare_config_t;

/**
 *******************************************************************************
 ** \brief The compare mode of OCO couple channels (FM3 mode)
 **
 ** \note The OCO channel with 1 change mode or 2 change mode must connect
 **       with CNT with up count mode.
 **       The OCO channel with active high mode or active low mode must
 **       connect with CNT with up/down count mode.
 **
 ******************************************************************************/
typedef enum en_oco_compare_mode
{
    OcoOdd1ChangeEven1Change    = 0u,       ///< Odd OCO channel is set to 1 change mode, even OCO channel is set to 1 change mode.
    OcoOdd2ChangeEven1Change    = 1u,       ///< Odd OCO channel is set to 2 change mode, even OCO channel is set to 1 change mode.
    OcoOdd1ChangeEvenActiveHigh = 2u,       ///< Odd OCO channel is set to 1 change mode, even OCO channel is set to active high mode.
    OcoOddActiveHighEven1Change = 3u,       ///< Odd OCO channel is set to active high mode, even OCO channel is set to 1 change mode.
    OcoOddActiveLowEven1Change  = 4u,       ///< Odd OCO channel is set to active low mode, even OCO channel is set to 1 change mode.
    OcoOddActiveHighEvenActiveHigh = 5u,    ///< Odd OCO channel is set to active high mode, even OCO channel is set to active high mode.
    OcoOddActiveLowEvenActiveLow   = 6u,    ///< Odd OCO channel is set to active low mode, even OCO channel is set to active low mode.
}en_oco_compare_mode_t;

/**
 *******************************************************************************
 ** \brief Structure of OCO internal data
 ******************************************************************************/
typedef struct stc_oco_irq_cb
{
    func_ptr_t  pfnOco0IrqCallback;     ///< Callback function pointer of OCO0 interrupt
    func_ptr_t  pfnOco1IrqCallback;     ///< Callback function pointer of OCO1 interrupt
    func_ptr_t  pfnOco2IrqCallback;     ///< Callback function pointer of OCO2 interrupt
    func_ptr_t  pfnOco3IrqCallback;     ///< Callback function pointer of OCO3 interrupt
    func_ptr_t  pfnOco4IrqCallback;     ///< Callback function pointer of OCO4 interrupt
    func_ptr_t  pfnOco5IrqCallback;     ///< Callback function pointer of OCO5 interrupt
}stc_oco_irq_cb_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define  TIMER4_OCO_CH0  (0u)           ///< Timer4 OCO channel NO.0
#define  TIMER4_OCO_CH1  (1u)           ///< Timer4 OCO channel NO.1
#define  TIMER4_OCO_CH2  (2u)           ///< Timer4 OCO channel NO.2
#define  TIMER4_OCO_CH3  (3u)           ///< Timer4 OCO channel NO.3
#define  TIMER4_OCO_CH4  (4u)           ///< Timer4 OCO channel NO.4
#define  TIMER4_OCO_CH5  (5u)           ///< Timer4 OCO channel NO.5
#define  TIMER4_OCO_MAX_CH (TIMER4_OCO_CH5)     ///< Timer4 OCO max channel NO.

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/


/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/* 1. Timer4-OCO configuration */
extern en_result_t Timer4_Oco_Init(uint8_t u8Ch,
                              stc_timer4_oco_config_t*  pstcOcoConfig);
extern en_result_t Timer4_Oco_DeInit(uint8_t u8Ch, boolean_t bTouchNvic);

/* 2. Write/read compare value/mode */
extern en_result_t Timer4_Oco_WriteOccr(uint8_t u8Ch, uint16_t u16Occr);
extern uint16_t Timer4_Oco_ReadOccr(uint8_t u8Ch);
extern en_result_t Timer4_Oco_SetOccrBuf(uint8_t u8Ch,
                                         en_oco_occr_buf_t enOccrBufMode);
extern en_result_t Timer4_Oco_SetOcmrBuf(uint8_t u8Ch,
                                         en_oco_ocmr_buf_t enOcmrBufMode);
extern en_result_t Timer4_Oco_SetEvenChCompareMode(uint8_t u8EvenCh,
                                     stc_oco_even_compare_config_t* pstcConfig);
extern en_result_t Timer4_Oco_SetOddChCompareMode (uint8_t u8OddCh,
                                     stc_oco_odd_compare_config_t* pstcConfig);

/* 3. Function Enable/Disable */
extern en_result_t Timer4_Oco_EnableOperation(uint8_t u8Ch);
extern en_result_t Timer4_Oco_DisableOperation(uint8_t u8Ch);

/* 4. Irq Enable/Disable */
extern en_result_t Timer4_Oco_EnableIrq(uint8_t u8Ch,
                                                  func_ptr_t  pfnCallback);
extern en_result_t Timer4_Oco_DisableIrq(uint8_t u8Ch);

/* 5. Interrupt flag read/clear */
extern en_irq_flag_t Timer4_Oco_GetIrqFlag(uint8_t u8Ch);
extern en_result_t Timer4_Oco_ClrIrqFlag(uint8_t u8Ch);

/* 6. Set/get pin level */
extern en_result_t Timer4_Oco_SetOpPinLevel(uint8_t u8Ch,
                                               en_oco_op_out_state_t enLevel);
extern en_oco_op_out_state_t Timer4_Oco_GetOpPinLevel(uint8_t u8Ch);

//@} // Timer4OcoGroup

#ifdef __cplusplus
}
#endif

#endif /* __TIMER4_OCO_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
