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
/** \file clk.h
 **
 ** A detailed description is available at
 ** @link ClkGroup Clk description @endlink
 **
 **   - 2017-08-09  1.0  HongJH First version for Device Driver Library of Clock.
 **
 ******************************************************************************/

#ifndef __CLK_H__
#define __CLK_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32f_m14x.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup ClkGroup Clock(Clk)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
*******************************************************************************/
/**
 *******************************************************************************
 ** \brief clk input source enum
 **
 ******************************************************************************/
typedef enum en_clk_source
{
    ClkHICR = 0u,                       ///< Internal High CR Clock Oscillator
    ClkHSXT = 1u,                       ///< External High CR Clock Oscillator
    ClkMPLL = 2u,                       ///< Main PLL Clock
    ClkLICR = 4u,                       ///< Internal Low CR Clock Oscillator
    ClkLSXT = 5u,                       ///< external low CR Clock Oscillator
}en_clk_source_t;

/**
 *******************************************************************************
 ** \brief pll source clock selection enum
 **
 ******************************************************************************/
typedef enum en_clk_pllsource
{
    MAIN2PLL = 0u,                      ///< Select external High CR Clock as pll source
    HICR2PLL = 1u,                      ///< Select internal High CR Clock as pll source
}en_clk_pllsource_t;

/**
 *******************************************************************************
 ** \brief all peripheral device enum
 **
 ******************************************************************************/
typedef enum en_clk_periph_idx
{
    /* apb0 */
    ClkPeripheralClock = 0u,            ///< Clock module
    ClkPeripheralReset = 1u,            ///< Reset module
    ClkPeripheralHwwdt = 2u,            ///< Hwwdt module
    ClkPeripheralSwwdt = 3u,            ///< Swwdt module
    ClkPeripheralDt    = 4u,            ///< Dt module
    /* apb1 */
    ClkPeripheralCt    = 5u,            ///< Ct module
    ClkPeripheralAdc   = 6u,            ///< Adc module
    ClkPeripheralExti  = 7u,            ///< Exti module
    ClkPeripheralLcd   = 8u,            ///< Lcd module
    ClkPeripheralGpio  = 9u,            ///< Gpio module
    ClkPeripheralLvd   = 10u,           ///< Lvd module
    ClkPeripheralMsc   = 11u,           ///< Msc module
    ClkPeripheralCc    = 12u,           ///< Cc module
    ClkPeripheralVc    = 13u,           ///< Vc module
    ClkPeripheralOpa   = 14u,           ///< Opa module
    /* ahb */
    ClkPeripheralDma   = 15u,           ///< Dma module
    ClkPeripheralFlash = 16u,           ///< Flash module
    ClkPeripheralMax   = 17u,           ///< Max index
}en_clk_periph_idx_t;

/**
 *******************************************************************************
 ** \brief high cr stable wait time
 **
 ******************************************************************************/
typedef enum en_clk_waitmain
{
    ClkCswtMain64us  = 0u,              ///< external high cr wait time: ~64us
    ClkCswtMain256us = 1u,              ///< external high cr wait time: ~256us
    ClkCswtMain1ms   = 2u,              ///< external high cr wait time: ~1ms
    ClkCswtMain4ms   = 3u,              ///< external high cr wait time: ~4ms
    ClkCswtMain8ms   = 4u,              ///< external high cr wait time: ~8ms
    ClkCswtMain16ms  = 5u,              ///< external high cr wait time: ~16ms
    ClkCswtMain32ms  = 6u,              ///< external high cr wait time: ~32ms
    ClkCswtMain65ms  = 7u,              ///< external high cr wait time: ~65ms
    ClkCswtMain131ms = 8u,              ///< external high cr wait time: ~131ms
    ClkCswtMain262ms = 9u,              ///< external high cr wait time: ~262ms
    ClkCswtMain524ms = 10u,             ///< external high cr wait time: ~524ms
    ClkCswtMain1s    = 11u,             ///< external high cr wait time: ~1s
    ClkCswtMain2s    = 12u,             ///< external high cr wait time: ~2s
    ClkCswtMain1us   = 13u,             ///< external high cr wait time: ~1us
    ClkCswtMain4us   = 14u,             ///< external high cr wait time: ~4us
    ClkCswtMain16us  = 15u,             ///< external high cr wait time: ~16us
}en_clk_waitmain_t;

/**
 *******************************************************************************
 ** \brief low cr stable wait time
 **
 ******************************************************************************/
typedef enum en_clk_waitsub
{
    ClkCswtSub01ms  = 0u,               ///< sub cr wait time: ~1ms
    ClkCswtSub05ms  = 1u,               ///< sub cr wait time: ~5ms
    ClkCswtSub2ms   = 2u,               ///< sub cr wait time: ~2ms
    ClkCswtSub8ms   = 3u,               ///< sub cr wait time: ~8ms
    ClkCswtSub16ms  = 4u,               ///< sub cr wait time: ~16ms
    ClkCswtSub32ms  = 5u,               ///< sub cr wait time: ~32ms
    ClkCswtSub64ms  = 6u,               ///< sub cr wait time: ~64ms
    ClkCswtSub128ms = 7u,               ///< sub cr wait time: ~128ms
    ClkCswtSub256ms = 8u,               ///< sub cr wait time: ~256ms
    ClkCswtSub512ms = 9u,               ///< sub cr wait time: ~512ms
    ClkCswtSub1s    = 10u,              ///< sub cr wait time: ~1s
    ClkCswtSub2s    = 11u,              ///< sub cr wait time: ~2s
    ClkCswtSub4s    = 12u,              ///< sub cr wait time: ~4s
    ClkCswtSub8s    = 13u,              ///< sub cr wait time: ~8s
    ClkCswtSub16s   = 14u,              ///< sub cr wait time: ~16s
    ClkCswtSub32s   = 15u,              ///< sub cr wait time: ~32s
}en_clk_waitsub_t;

/**
 *******************************************************************************
 ** \brief pll stable wait time
 **
 ******************************************************************************/
typedef enum en_clk_waitpll
{
    ClkPswtPll85us  = 0u,               ///< pll clock wait time: ~85us
    ClkPswtPll170us = 1u,               ///< pll clock wait time: ~170us
    ClkPswtPll340us = 2u,               ///< pll clock wait time: ~340us
    ClkPswtPll680us = 3u,               ///< pll clock wait time: ~680us
}en_clk_waitpll_t;

/**
 *******************************************************************************
 ** \brief clock base division value enum
 **
 ** \note osc source -> base div -> (base clock)cpu
 ******************************************************************************/
typedef enum en_clk_basediv
{
    ClkBaseDiv1  = 0u,                  ///< HCLK Division 1/1
    ClkBaseDiv2  = 1u,                  ///< HCLK Division 1/2
    ClkBaseDiv3  = 2u,                  ///< HCLK Division 1/3
    ClkBaseDiv4  = 3u,                  ///< HCLK Division 1/4
    ClkBaseDiv6  = 4u,                  ///< HCLK Division 1/6
    ClkBaseDiv8  = 5u,                  ///< HCLK Division 1/8
    ClkBaseDiv16 = 6u,                  ///< HCLK Division 1/16
}en_clk_basediv_t;

/**
 *******************************************************************************
 ** \brief apb0 bus division value enum
 **
 ** \note  base clock -> apb0 div -> apb0 clock
 ******************************************************************************/
typedef enum en_clk_apb0div
{
    ClkApb0Div1 = 0u,                   ///< PCLK0 Division 1/1
    ClkApb0Div2 = 1u,                   ///< PCLK0 Division 1/2
    ClkApb0Div4 = 2u,                   ///< PCLK0 Division 1/4
    ClkApb0Div8 = 3u,                   ///< PCLK0 Division 1/8
}en_clk_apb0div_t;

/**
 *******************************************************************************
 ** \brief apb1 bus division value enum
 **
 ** \note  base clock -> apb1 div -> apb1 clock
 ******************************************************************************/
typedef enum en_clk_apb1div
{
    ClkApb1Div1 = 0u,                   ///< PCLK1 Division 1/1
    ClkApb1Div2 = 1u,                   ///< PCLK1 Division 1/2,
    ClkApb1Div4 = 2u,                   ///< PCLK1 Division 1/4
    ClkApb1Div8 = 3u,                   ///< PCLK1 Division 1/8,
}en_clk_apb1div_t;

/**
 *******************************************************************************
 ** \brief Clcok IRQ select
 **
 ** \note  Clock IRQ source select
 ******************************************************************************/
typedef enum en_clk_irq_sel
{
    ClkIrqAfd     = 0u,                 ///< Clock afd interrupt
    ClkIrqPLLRdy  = 1u,                 ///< PLL ready interrupt
    ClkIrqLSXTRdy = 2u,                 ///< LSXT ready interrupt
    ClkIrqHSXTRdy = 3u,                 ///< HSXT ready interrupt
}en_clk_irq_sel_t;

/**
 *******************************************************************************
 ** \brief swwdt division setting
 **
 ******************************************************************************/
typedef enum en_clk_swwdt_div
{
    ClkSwwdtDiv1 = 0u,                       ///< swwdt division 1/1
    ClkSwwdtDiv2 = 1u,                       ///< swwdt division 1/2,
    ClkSwwdtDiv4 = 2u,                       ///< swwdt division 1/4,
    ClkSwwdtDiv8 = 3u,                       ///< swwdt division 1/8,
}en_clk_swwdt_div_t;

/**
 *******************************************************************************
 ** \brief afd division setting
 **
 ******************************************************************************/
typedef enum en_clk_afd_div
{
    ClkAfdDiv256  = 5u,                 ///< 1/256 frequency of high-speed CR oscillation
    ClkAfdDiv512  = 6u,                 ///< 1/512 frequency of high-speed CR oscillation
    ClkAfdDiv1024 = 7u,                 ///< 1/1024 frequency of high-speed CR oscillation
}en_clk_afd_div_t;

/**
 *******************************************************************************
 ** \brief clock type csv status
 **
 ******************************************************************************/
typedef enum en_clk_cfd_stat
{
    ClkCfdLSXT = 0u,                    ///< Cfd LSXT
    ClkCfdHSXT = 1u,                    ///< Cfd HSXT
}en_clk_cfd_stat_t;

/**
 *******************************************************************************
 ** \brief clock module function setting event enumeration
 **
 ******************************************************************************/
typedef enum en_clk_func
{
    ClkFuncAfd         = 0u,            ///< clock function for Afd
    ClkFuncAfdReset    = 1u,            ///< clock function for Afd reset
    ClkFuncLSXTCfd     = 2u,            ///< clock function for LSXT
    ClkFuncHSXTCfd     = 3u,            ///< clock function for HSXT
    ClkFuncLSXTCfdRest = 4u,            ///< clock function for LSXT reset
    ClkFuncHSXTCfdRest = 5u,            ///< clock function for HSXT reset
}en_clk_func_t;

/**
 *******************************************************************************
 ** \brief system reset cause flag
 **
 ** \note  may contain multiple event
 ******************************************************************************/
typedef struct stc_clk_rstcause
{
    boolean_t   bSoftware;              ///< software reset
    boolean_t   bAfd;                   ///< Anomalous frequence dectection reset
    boolean_t   bCfd;                   ///< Clock failure dectction reset
    boolean_t   bHwwdt;                 ///< Hardware watchdog timer reset
    boolean_t   bSwwdt;                 ///< Software watchdog timer reset
    boolean_t   bLvd;                   ///< Low voltage Dectection reset
    boolean_t   bRstPin;                ///< Reset pin
    boolean_t   bPowerOn;               ///< Power on
}stc_clk_rstcause_t;

/**
 *******************************************************************************
 ** \brief Clock configuration
 **
 ******************************************************************************/
typedef struct stc_clk_config
{
    en_clk_basediv_t      enBaseClkDiv;     ///< See description of #en_clk_basediv_t
    en_clk_apb0div_t      enAPB0Div;        ///< See description of #en_clk_apb0div_t
    en_clk_apb1div_t      enAPB1Div;        ///< See description of #en_clk_apb1div_t
    boolean_t             bAPB1En;          ///< FALSE: Disables APB1 regardless of divider settings
    en_clk_waitmain_t     enClkWaitMain;    ///< See description of #en_clk_waitmain_t
    en_clk_waitsub_t      enClkWaitSub;     ///< See description of #en_clk_waitsub_t
    en_clk_waitpll_t      enClkWaitPll;     ///< See description of #en_clk_waitpll_t

    boolean_t             bHicrEn;          ///< TRUE: Enables HICR
    boolean_t             bMainEn;          ///< TRUE: Enables HXST
    boolean_t             bSubEn;           ///< TRUE: Enables LXST
    boolean_t             bPllEn;           ///< TRUE: Enables PLL
    uint8_t               u8PllMulti;       ///< PLL Frquency = u8PllMulti*main clock
    en_clk_pllsource_t    enPllSrc;         ///< See description of #en_clk_pllsource_t
    en_clk_source_t       enClkSrc;         ///< See description of #en_clk_source_t
    boolean_t             bPLLIrq;          ///< TRUE: Enables PLL ready interrupt
    boolean_t             bHSXTIrq;         ///< TRUE: Enables HSXT ready interrupt
    boolean_t             bLSXTIrq;         ///< TRUE: Enables LXST ready interrupt
    func_ptr_t            pfnPLLRdyCb;      ///< PLL ready interrupt callback function
    func_ptr_t            pfnHSXTRdyCb;     ///< HSXT ready interrupt callback function
    func_ptr_t            pfnLSXTRdyCb;     ///< LXST ready interrupt callback function
} stc_clk_config_t;

/**
 *******************************************************************************
 ** \brief structure to enable the afd interrupts
 ******************************************************************************/
typedef struct stc_clk_afd_irq_sel
{
    boolean_t           bAfdIrqEn;      ///< TRUE: Enables afd interrupt
}stc_clk_afd_irq_sel_t;

/**
 *******************************************************************************
 ** \brief structure to set the afd interrupt callback function
 ******************************************************************************/
typedef struct stc_clk_afd_irq_cb
{
    func_ptr_t          pfnAfdIrqCb;    ///< AFD interrupt callback function
}stc_clk_afd_irq_cb_t;

/**
 *******************************************************************************
 ** \brief Clock csv configuration
 ******************************************************************************/
typedef struct stc_clk_csv_config
{
    en_clk_afd_div_t        enAfdDiv;           ///< See description of #en_clk_afd_div_t
    boolean_t               bAfdEn;             ///< TRUE: Enables afd function
    boolean_t               bAfdResetEn;        ///< TRUE: Enables afd reset function
    boolean_t               bLSXTFailResetEn;   ///< TRUE: Enables cfd reset function for LSXT
    boolean_t               bHSXTFailResetEn;   ///< TRUE: Enables cfd reset function for HSXT
    boolean_t               bLSXTFailDetectEn;  ///< TRUE: Enables cfd function for LSXT
    boolean_t               bHSXTFailDetectEn;  ///< TRUE: Enables cfd function for HSXT
    stc_clk_afd_irq_sel_t   *pstcAfdIrqSel;     ///< See description of #stc_clk_afd_irq_sel_t
    stc_clk_afd_irq_cb_t    *pstcAfdIrqCb;      ///< See description of #stc_clk_afd_irq_cb_t
}stc_clk_csv_config_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*! LICR fix frequency, don't change this value */
#define CLK_LICR_VAL  (32*1000)

/*! LSXT fix frequency, don't change this value */
#define CLK_LSXT_VAL  (32768)

/*! HSXT fix frequency, don't change this value */
#define CLK_HICR_VAL  (4*1000*1000)

/*! please, set this value according acctual hsxt frequency */
#define CLK_HSXT_VAL  (4*1000*1000)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/


/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/* clk init/deinit */
extern en_result_t Clk_Init(stc_clk_config_t *pstcCfg);
extern en_result_t Clk_DeInit(boolean_t bTouchNvic);

/* clk source basic function */
extern en_result_t Clk_Enable(en_clk_source_t enSource, boolean_t bFlag);
extern en_result_t Clk_SetSource(en_clk_source_t enSource);
extern boolean_t Clk_GetClkRdy(en_clk_source_t enSource);
extern en_clk_source_t Clk_GetCurSource(void);

/* clk switch advanced function */
extern en_result_t Clk_SwitchTo(en_clk_source_t enSource);

extern uint32_t Clk_GetSourceFreq(en_clk_source_t enSrc);

/* clk frequency function */
extern uint32_t Clk_GetSystemClk(void);

/* abp function */
extern uint32_t Clk_GetPeripheralClk(en_clk_periph_idx_t enIdx);
extern en_result_t Clk_EnableAPB1(boolean_t bFlag);

/* clock function enable/disable */
extern en_result_t Clk_EnableFunc(en_clk_func_t enFunc);
extern en_result_t Clk_DisableFunc(en_clk_func_t enFunc);

/* clock csv setting */
extern en_result_t Clk_SetSwwdtDiv(en_clk_swwdt_div_t  enDiv);

extern en_result_t Clk_CsvInit(stc_clk_csv_config_t *pstcCfg);
extern en_result_t Clk_CsvDeInit(boolean_t bTouchNvic);
extern en_result_t Clk_SetAfdDiv(en_clk_afd_div_t enDiv);
extern boolean_t Clk_GetCfdStat(en_clk_cfd_stat_t enType);
extern en_result_t Clk_SetAfdWindow(uint16_t u16Low, uint16_t u16High);
extern uint16_t Clk_GetAfdFreq(void);

extern en_result_t Clk_GetResetCause(stc_clk_rstcause_t *pstcData);

extern en_result_t Clk_EnableIrq(en_clk_irq_sel_t enSel);
extern en_result_t Clk_DisableIrq(en_clk_irq_sel_t enSel);
extern en_result_t Clk_GetIrqStat(en_clk_irq_sel_t enSel, boolean_t *pbIrqStat);
extern en_result_t Clk_ClearIrq(en_clk_irq_sel_t enSel);

//@} // ClkGroup

#ifdef __cplusplus
}
#endif


#endif /* __CLK_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
