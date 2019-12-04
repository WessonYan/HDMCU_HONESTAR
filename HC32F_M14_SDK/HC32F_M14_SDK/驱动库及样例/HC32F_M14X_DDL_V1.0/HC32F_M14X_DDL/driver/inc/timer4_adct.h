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
/** \file timer4_adct.h
 **
 ** A detailed description is available at
 ** @link Timer4AdctGroup Timer4Adct description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of
 **     Timer4Adct
 **
 ******************************************************************************/

#ifndef __TIMER4_ADCT_H__
#define __TIMER4_ADCT_H__

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
 ** \defgroup Timer4AdctGroup Timer4 ADC Trigger(Timer4Adct)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Define Adct buffer type
 ******************************************************************************/
typedef enum en_adct_buf
{
    AdctBufDisable     = 0u,            ///< disable Adct buffer function
    AdctBufCntZero     = 1u,            ///< transfer buffer of ATVR and ATMR when counter value of Cnt connected= 0x0000
    AdctBufCntPeak     = 2u,            ///< transfer buffer of ATVR and ATMR when counter value of Cnt connected= TCCP
    AdctBufCntZeroPeak = 3u,            ///< transfer buffer of ATVR and ATMR both when counter value of Cnt connected= 0x0000 and TCCP
    AdctBufCntZicZero  = 4u,            ///< transfer buffer of ATVR and ATMR when counter value of Cnt connected= 0x0000 and zero value detection mask counter value is 0
    AdctBufCntPicZero  = 5u,            ///< transfer buffer of ATVR and ATMR when counter value of Cnt connected= peak and peak value detection mask counter value is 0
    AdctBufCntZicZeroOrCntPicZero = 6u, ///< transfer buffer of ATVR and ATMR when counter value of Cnt connected= 0x0000 and zero value detection mask counter value is 0
                                        ///< or counter value of Cnt connected= peak and peak value detection mask counter value is 0
}en_adct_buf_t;

/**
 *******************************************************************************
 ** \brief Define Timer4 Adct trig mode
 ******************************************************************************/
typedef enum en_adct_trig_unit
{
    AdctTrigAdc0Scan = 0u,              ///< AdctStartTrig0
    AdctTrigAdc0Prio = 1u,              ///< AdctStartTrig1
    AdctTrigAdc1Scan = 2u,              ///< AdctStartTrig2
    AdctTrigAdc1Prio = 3u,              ///< AdctStartTrig3
    AdctTrigAdc2Scan = 4u,              ///< AdctStartTrig4
    AdctTrigAdc2Prio = 5u,              ///< AdctStartTrig5
}en_adct_trig_unit_t;

/**
 *******************************************************************************
 ** \brief Define Occr select type
 ******************************************************************************/
typedef enum en_adct_occr_sel
{
    AdctSelOccr0 = 0u,                  ///< select Occr0 channel
    AdctSelOccr1 = 1u,                  ///< select Occr1 channel
}en_adct_occr_sel_t;

/**
 *******************************************************************************
 ** \brief Define Adct running mode
 ******************************************************************************/
typedef enum en_adct_mode
{
    AdctCmpTrigMode   = 0u,             ///< select Adct Normal mode
    AdctDelayTrigMode = 1u,             ///< select Adct Offset mode
}en_adct_mode_t;

/**
 *******************************************************************************
 ** \brief Define Timer4_adct configure parameters
 ******************************************************************************/
typedef struct stc_timer4_adct_config
{
    en_adct_buf_t        enBuf;         ///< configure Adct Buffer transfer type
    en_adct_trig_unit_t  enTrigAdcUnit; ///< configure Adct Trigger type
    en_adct_mode_t       enMode;        ///< configure Adct Running mode
    en_adct_occr_sel_t   enOccrSel;     ///< select Adct Occr channel
    boolean_t            bCompareCntZeroMaskCntVal; ///< TRUE:  Comparison is performed with the CNT zero interrupt mask counter.
                                                    ///< FALSE: Comparison is not performed with the CNT zero interrupt mask counter.
    boolean_t            bCompareCntPeakMaskCntVal; ///< TRUE:  Comparison is performed with the CNT peak interrupt mask counter.
                                                    ///< FALSE: Comparison is not performed with the CNT peak interrupt mask counter.
    uint8_t              u8CompareVal;              ///< Specifies the AD conversion start time by the value to be compared with the CNT interrupt mask counter.
}stc_timer4_adct_config_t;

/**
 *******************************************************************************
 ** \brief Define Timer4_adct functions
 ******************************************************************************/
typedef struct stc_timer4_adct_trig_cond
{
    boolean_t   bDownEn;                ///< Enable Adct Down function
    boolean_t   bPeakEn;                ///< Enable Adct Peak function
    boolean_t   bUpEn;                  ///< Enable Adct Up function
    boolean_t   bZeroEn;                ///< Enable Adct Zero function
}stc_timer4_adct_trig_cond_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define TIMER4_ADCT_CH0  (0u)           ///< Timer4 ADCT channel NO.0
#define TIMER4_ADCT_CH1  (1u)           ///< Timer4 ADCT channel NO.1
#define TIMER4_ADCT_CH2  (2u)           ///< Timer4 ADCT channel NO.2
#define TIMER4_ADCT_CH3  (3u)           ///< Timer4 ADCT channel NO.3
#define TIMER4_ADCT_CH4  (4u)           ///< Timer4 ADCT channel NO.4
#define TIMER4_ADCT_CH5  (5u)           ///< Timer4 ADCT channel NO.5

#define TIMER4_ADCT_COUPLE_CH10  (0u)   ///< Timer4 ADCT couple channel NO.1&0
#define TIMER4_ADCT_COUPLE_CH32  (1u)   ///< Timer4 ADCT couple channel NO.3&2
#define TIMER4_ADCT_COUPLE_CH54  (2u)   ///< Timer4 ADCT couple channel NO.5&4

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/


/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/* Init */
extern en_result_t Timer4_Adct_Init (uint8_t u8Ch,
                                      stc_timer4_adct_config_t *pstcConfig);
extern en_result_t Timer4_Adct_DeInit(uint8_t u8Ch);
/* Func enable/disable */
extern en_result_t Timer4_Adct_EnableOperation(uint8_t u8Ch,
                                     stc_timer4_adct_trig_cond_t* pstcTrigCond);
extern en_result_t Timer4_Adct_DisableOperation(uint8_t u8Ch,
                                     stc_timer4_adct_trig_cond_t* pstcTrigCond);
/* Count write/read */
extern en_result_t Timer4_Adct_WriteAtvr(uint8_t u8Ch, uint16_t u16AtvrVal);
extern uint16_t    Timer4_Adct_ReadAtvr(uint8_t u8Ch);

//@} // Timer4AdctGroup

#ifdef __cplusplus
}
#endif

#endif /* __TIMER4_ADCT_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
