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
/** \file lcd.h
 **
 ** A detailed description is available at
 ** @link LcdGroup Lcd description @endlink
 **
 **   - 2017-8-9  1.0  Jason First version for Device Driver Library of lcd.
 **
 ******************************************************************************/

#ifndef __LCD_H__
#define __LCD_H__

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
 ** \defgroup LcdGroup Liquid Crystal Display (LCD)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef  M0P_LCDC_TypeDef         stc_lcdc_regtyptedef_t;

typedef  stc_lcdc_lcdse0_field_t  stc_lcd_se0typedef_t;
typedef  stc_lcdc_lcdse1_field_t  stc_lcd_se1typedef_t;
typedef  stc_lcdc_lcdse2_field_t  stc_lcd_se2typedef_t;
typedef  stc_lcdc_lcdse3_field_t  stc_lcd_se3typedef_t;
typedef  stc_lcdc_lcdse4_field_t  stc_lcd_se4typedef_t;

/**
 *******************************************************************************
 ** \brief Clock selection.
 **
 ** \note Set the lcd clock source.
 **
 ******************************************************************************/
typedef enum en_lcd_clk_sel
{
    LcdClkLSXT                  = 0u,   ///< The lcd clock source is lsxt.
    LcdClkLICR                  = 1u,   ///< The lcd clock source is licr.
} en_lcd_clk_sel_t;

/**
 *******************************************************************************
 ** \brief Frame refresh rate selection.
 **
 ** \note Set the frame refresh rate.
 **
 ******************************************************************************/
typedef enum en_lcd_refresh_rate_sel
{
    LcdRefreshRate128Hz         = 0u,   ///< The frame refresh rate is 128hz.
    LcdRefreshRate256Hz         = 1u,   ///< The frame refresh rate is 256hz.
    LcdRefreshRate512Hz         = 2u,   ///< The frame refresh rate is 512hz.
    LcdRefreshRate1024Hz        = 3u,   ///< The frame refresh rate is 1024hz.
} en_lcd_refresh_rate_sel_t;

/**
 *******************************************************************************
 ** \brief Bias selection.
 **
 ** \note Set the bias.
 **
 ******************************************************************************/
typedef enum en_lcd_bias_sel
{
    LcdBias1Div2                = 0u,   ///< LCD bias is 1/2.
    LcdBias1Div3                = 1u,   ///< LCD bias is 1/3.
    LcdBias1Div4                = 2u,   ///< LCD bias is 1/4.
} en_lcd_bias_sel_t;

/**
 *******************************************************************************
 ** \brief Display mode selection.
 **
 ** \note Set the display mode.
 **
 ******************************************************************************/
typedef enum en_lcd_display_mode_sel
{
    LcdDisModeDutyStatic        = 0u,   ///< LCD display mode duty is Static.
    LcdDisModeDuty1Div2         = 1u,   ///< LCD display mode duty is 1/2.
    LcdDisModeDuty1Div3         = 2u,   ///< LCD display mode duty is 1/3.
    LcdDisModeDuty1Div4         = 3u,   ///< LCD display mode duty is 1/4.
    LcdDisModeDuty1Div6         = 5u,   ///< LCD display mode duty is 1/6.
    LcdDisModeDuty1Div8         = 7u,   ///< LCD display mode duty is 1/8.
} en_lcd_display_mode_sel_t;

/**
 *******************************************************************************
 ** \brief Contrast selection.
 **
 ** \note Only in internal resistance \ref en_lcd_bias_circuit_sel_t circuit.
 **
 ******************************************************************************/
typedef enum en_lcd_contrast_sel
{
    LcdContrast60per            = 0u,   ///< LCD contrast is 60%.
    LcdContrast65per            = 1u,   ///< LCD contrast is 65%.
    LcdContrast70per            = 2u,   ///< LCD contrast is 70%.
    LcdContrast75per            = 3u,   ///< LCD contrast is 75%.
    LcdContrast80per            = 4u,   ///< LCD contrast is 80%.
    LcdContrast85per            = 5u,   ///< LCD contrast is 85%.
    LcdContrast90per            = 6u,   ///< LCD contrast is 90%.
    LcdContrast100per           = 7u,   ///< LCD contrast is 100%.
} en_lcd_contrast_sel_t;

/**
 *******************************************************************************
 ** \brief Charging time selection in fast charging mode.
 **
 ** \note Only in internal resistance \ref en_lcd_bias_circuit_sel_t circuit.
 **
 ******************************************************************************/
typedef enum en_lcd_charge_time_sel
{
    LcdChargeTime1Clk           = 0u,   ///< LCD Charge time is 1 lcd clock.
    LcdChargeTime2Clk           = 1u,   ///< LCD Charge time is 2 lcd clock.
    LcdChargeTime3Clk           = 2u,   ///< LCD Charge time is 3 lcd clock.
    LcdChargeTime4Clk           = 3u,   ///< LCD Charge time is 4 lcd clock.
    LcdChargeTime5Clk           = 4u,   ///< LCD Charge time is 5 lcd clock.
    LcdChargeTime6Clk           = 5u,   ///< LCD Charge time is 6 lcd clock.
    LcdChargeTime7Clk           = 6u,   ///< LCD Charge time is 7 lcd clock.
    LcdChargeTime8Clk           = 7u,   ///< LCD Charge time is 8 lcd clock.
} en_lcd_charge_time_sel_t;

/**
 *******************************************************************************
 ** \brief Charging mode selection.
 **
 ** \note Only in internal resistance \ref en_lcd_bias_circuit_sel_t circuit.
 **
 ******************************************************************************/
typedef enum en_lcd_charge_mode_sel
{
    LcdChargeModeFast           = 0u,   ///< Fast charging.
    LcdChargeModeLargeCur       = 1u,   ///< Large current charging.
} en_lcd_charge_mode_sel_t;

/**
 *******************************************************************************
 ** \brief Internal charge pump charging frequence selection.
 **
 ** \note Only in external capacitance \ref en_lcd_bias_circuit_sel_t circuit.
 **
 ******************************************************************************/
typedef enum en_lcd_charge_pump_freq_sel
{
    LcdChargePumpFreq2kHz       = 0u,   ///< Frequence is 2kHz.
    LcdChargePumpFreq4kHz       = 1u,   ///< Frequence is 4kHz.
    LcdChargePumpFreq8kHz       = 2u,   ///< Frequence is 8kHz.
    LcdChargePumpFreq16kHz      = 3u,   ///< Frequence is 16kHz.
} en_lcd_charge_pump_freq_sel_t;

/**
 *******************************************************************************
 ** \brief Bias circuit selection.
 **
 ** \note Set bias circuit.
 **
 ******************************************************************************/
typedef enum en_lcd_bias_circuit_sel
{
    LcdBiasCircuitInRes         = 0u,   ///< LCD bias circuit is internal resistance.
    LcdBiasCircuitExCap         = 1u,   ///< LCD bias circuit is external capacitance.
    LcdBiasCircuitExRes         = 2u,   ///< LCD bias circuit is external resistance.
} en_lcd_bias_circuit_sel_t;

/**
 *******************************************************************************
 ** \brief Drive waveform selection.
 **
 ** \note Set drive waveform.
 **
 ******************************************************************************/
typedef enum en_lcd_drive_wave_sel
{
    LcdDriveWaveTypeA           = 0u,   ///< LCD drive waveform is type A.
    LcdDriveWaveTypeB           = 1u,   ///< LCD drive waveform is type B.
} en_lcd_drive_wave_sel_t;

/**
 *******************************************************************************
 ** \brief Drive capability selection.
 **
 ** \note Set drive capability.
 **
 ******************************************************************************/
typedef enum en_lcd_drive_capability_sel
{
    LcdDriveCapabilityNormal    = 0u,   ///< LCD drive capability is Normal.
    LcdDriveCapability2Times    = 1u,   ///< LCD drive capability is 2 times.
    LcdDriveCapability3Times    = 2u,   ///< LCD drive capability is 3 times.
    LcdDriveCapability4Times    = 3u,   ///< LCD drive capability is 4 times.
} en_lcd_drive_capability_sel_t;

/**
 *******************************************************************************
 ** \brief R03~R43 Port function selection.
 **
 ** \note LcdPortFuncGpio:R03~R43 port is GPIO only in internal resistance
 **       circuit.If the drive circuit is not internal resistance,the R03~R43
 **       port is only use to drive voltage input.
 **
 ** \note LcdPortFuncVolIn:R03~R43 port is drive voltage input.
 ******************************************************************************/
typedef enum en_lcd_port_func_sel
{
    LcdPortFuncGpio             = 0u,   ///< GPIO.
    LcdPortFuncVolIn            = 1u,   ///< Drive voltage input.
} en_lcd_port_func_sel_t;

/**
 *******************************************************************************
 ** \brief R03~R43 Port type.
 **
 ******************************************************************************/
typedef enum en_lcd_port_type_sel
{
    LcdPortTypeR03              = 0u,   ///< Port type is R03.
    LcdPortTypeR13              = 1u,   ///< Port type is R13.
    LcdPortTypeR23              = 2u,   ///< Port type is R23.
    LcdPortTypeR33              = 3u,   ///< Port type is R33.
    LcdPortTypeR43              = 4u,   ///< Port type is R43.
} en_lcd_port_type_sel_t;

/**
 *******************************************************************************
 ** \brief Com port function selection.
 **
 ** \note LcdComFuncGpio:Com0~com3 port is GPIO.Com4~com5 port is GPIO or segxx
 **       according to the segxx port in 1/8 or 1/6 duty.Com6~com7 port is GPIO
 **       or segxx according to the segxx port in 1/8 duty.
 ** \note LcdComFuncOutput:Com0~com3 port is lcd output.Com4~com5 port is lcd
 **       output in 1/8 or 1/6 duty.Com6~com7 port is lcd output in 1/8 duty.
 **
 ******************************************************************************/
typedef enum en_lcd_com_func_sel
{
    LcdComFuncGpio              = 0u,   ///< Gpio.
    LcdComFuncOutput            = 1u,   ///< Lcd output.
} en_lcd_com_func_sel_t;

/**
 *******************************************************************************
 ** \brief Com Port type.
 **
 ** \note The number of com port.
 **
 ******************************************************************************/
typedef enum en_lcd_com_type_sel
{
    LcdComType0                 = 0u,   ///< Com0.
    LcdComType1                 = 1u,   ///< Com1.
    LcdComType2                 = 2u,   ///< Com2.
    LcdComType3                 = 3u,   ///< Com3.
    LcdComType4                 = 4u,   ///< Com4.
    LcdComType5                 = 5u,   ///< Com5.
    LcdComType6                 = 6u,   ///< Com6.
    LcdComType7                 = 7u,   ///< Com7.
} en_lcd_com_type_sel_t;

/**
 *******************************************************************************
 ** \brief Working mode.
 **
 ** \note Include com, duty, bias, waveform.
 **
 ******************************************************************************/
typedef enum  en_lcd_work_mode
{
    /* 8 COM mode, pin SEG0 ~ SEG35 as segment signal output pin. */
    Lcd_8Com_1Div8Duty_1Div3bias_TypeA_Max36Seg  = 0,
    Lcd_8Com_1Div8Duty_1Div3bias_TypeB_Max36Seg  = 1,
    Lcd_8Com_1Div8Duty_1Div4bias_TypeA_Max36Seg  = 2,
    Lcd_8Com_1Div8Duty_1Div4bias_TypeB_Max36Seg  = 3,

    /* 6 COM mode, pin SEG0 ~ SEG35, SEG38 and SEG39 as segment signal output pin. */
    Lcd_6Com_1Div6Duty_1Div3bias_TypeA_Max38Seg  = 4,
    Lcd_6Com_1Div6Duty_1Div3bias_TypeB_Max38Seg  = 5,
    Lcd_6Com_1Div6Duty_1Div4bias_TypeA_Max38Seg  = 6,
    Lcd_6Com_1Div6Duty_1Div4bias_TypeB_Max38Seg  = 7,

    /* 4 COM mode, pin SEG0 ~ SEG39 as segment signal output pin. */
    Lcd_4Com_1Div2Duty_1Div2bias_TypeA_Max40Seg  = 8,
    Lcd_4Com_1Div2Duty_1Div2bias_TypeB_Max40Seg  = 9,
    Lcd_4Com_1Div2Duty_1Div3bias_TypeA_Max40Seg  = 10,
    Lcd_4Com_1Div2Duty_1Div3bias_TypeB_Max40Seg  = 11,
    Lcd_4Com_1Div3Duty_1Div2bias_TypeA_Max40Seg  = 12,
    Lcd_4Com_1Div3Duty_1Div2bias_TypeB_Max40Seg  = 13,
    Lcd_4Com_1Div3Duty_1Div3bias_TypeA_Max40Seg  = 14,
    Lcd_4Com_1Div3Duty_1Div3bias_TypeB_Max40Seg  = 15,
    Lcd_4Com_1Div4Duty_1Div3bias_TypeA_Max40Seg  = 16,
    Lcd_4Com_1Div4Duty_1Div3bias_TypeB_Max40Seg  = 17,
    Lcd_4Com_1Div4Duty_1Div4bias_TypeA_Max40Seg  = 18,
    Lcd_4Com_1Div4Duty_1Div4bias_TypeB_Max40Seg  = 19,

    /* Static mode, pin SEG0 ~ SEG39 as segment signal output pin. */
    Lcd_Com0_StaticMode_Max40Seg = 20,
} en_lcd_work_mode_t;

/**
 *******************************************************************************
 ** \brief Private configuration structure of internal resistance circuit.
 **
 ** \note Configure the contrast,charging time,charging mode.
 **
 ******************************************************************************/
typedef struct stc_lcd_inr_cfg
{
    en_lcd_contrast_sel_t           enContrast;     ///< Contrast.

    en_lcd_charge_time_sel_t        enChargingTime; ///< Charging time.

    en_lcd_charge_mode_sel_t        enChargingMode; ///< Charging mode.
} stc_lcd_inr_cfg_t;

/**
 *******************************************************************************
 ** \brief Private configuration structure of external capacotance circuit.
 **
 ** \note Configure the contrast,charging time,charging mode.
 **
 ******************************************************************************/
typedef struct stc_lcd_excap_cfg
{
    en_lcd_charge_pump_freq_sel_t   enChargePumpFreq;///< Charge pump frequence.
} stc_lcd_excap_cfg_t;

/**
 *******************************************************************************
 ** \brief Configuration structure of lcd drive circuit.
 **
 ** \note Configure drive circuit.
 **
 ******************************************************************************/
typedef struct stc_lcd_drive_circuit_cfg
{
    en_lcd_bias_circuit_sel_t       enDriveCircuit; ///< Drive circuit.

    union
    {
        stc_lcd_inr_cfg_t           stcLcdInrCfg;   ///< Internal resistance circuit configuration structure.

        stc_lcd_excap_cfg_t         stcLcdExcapCfg; ///< External capacitance circuit configuration structure.

        /* There is no configuration structure for external resistance. */
    };
} stc_lcd_drive_circuit_cfg_t;

/**
 ******************************************************************************
 ** \brief LCD configuration struct
 *****************************************************************************/
/**
 *******************************************************************************
 ** \brief Lcd configuration structure.
 **
 ** \note Configure the these function for lcd module if needed.
 **
 ******************************************************************************/
typedef struct stc_lcd_cfg
{
    en_lcd_clk_sel_t                enLcdClk;           ///< Set clock.

    en_lcd_refresh_rate_sel_t       enLcdRefreshRate;   ///< Set frame refresh rate.

    en_lcd_work_mode_t              enLcdWorkMode;      ///< Set work mode.Include com,bias,duty,waveform.

    en_lcd_drive_capability_sel_t   enLcdDriveCap;      ///< Set drive capability.

    stc_lcd_drive_circuit_cfg_t     stcLcdDriveCircuit; ///< Set drive circuit.

    /* u8LcdSe0~u8LcdSe4 Regist configuration */
    /* 8 COM mode, pin SEG0 ~ SEG35 as segment signal output pin. */
    /* 6 COM mode, pin SEG0 ~ SEG35, SEG38 and SEG39 as segment signal output pin. */
    /* 4 COM mode, pin SEG0 ~ SEG39 as segment signal output pin. */
    /* Static mode, pin SEG0 ~ SEG39 as segment signal output pin. */
    /* SEG0~SEG7 SET. */
    union {
        volatile  uint8_t           u8Seg0_Seg7Config;
        stc_lcd_se0typedef_t        stcSeg0_Seg7Config;
    };
    /* SEG8~SEG15 SET. */
    union {
        volatile  uint8_t           u8Seg8_Seg15Config;
        stc_lcd_se1typedef_t        stcSeg8_Seg15Config;
    };
    /* SEG16~SEG23 SET. */
    union {
        volatile  uint8_t           u8Seg16_Seg23Config;
        stc_lcd_se2typedef_t        stcSeg16_Seg23Config;
    };
    /* SEG24~SEG31 SET. */
    union {
        volatile  uint8_t           u8Seg24_Seg31Config;
        stc_lcd_se3typedef_t        stcSeg24_Seg31Config;
    };
    /* SEG32~SEG39 SET. */
    union {
        volatile  uint8_t           u8Seg32_Seg39Config;
        stc_lcd_se4typedef_t        stcSeg32_Seg39Config;
    };
} stc_lcd_cfg_t;


/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#ifndef ENABLE
        #define ENABLE TRUE             ///< Enable definition.
#endif

#ifndef DISABLE
        #define DISABLE FALSE           ///< Disable definition.
#endif

#ifndef LCDRAMSIZE
        #define LCDRAMSIZE  39u         ///< LCD ram max size.
#endif

#ifndef LCDRAMu8ListSIZE
        #define LCDRAMu8ListSIZE 7u     ///< LCD ram u8List max size.
#endif

#ifndef LCDFULLWRITE
        #define LCDFULLWRITE  0XFFu     ///< LCD ram full write 0XFF.
#endif

#ifndef LCDFULLCLEAR
        #define LCDFULLCLEAR  0X00u     ///< LCD ram full clear 0X00
#endif

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/* LCD INTI */
en_result_t Lcd_InitConfig(stc_lcd_cfg_t* pstcLcdInitconfig);
/* LCD ENABLE OR DISABLE */
void Lcd_Cmd(boolean_t bNewState);
/* LCD FULL Write */
void Lcd_FullWrite(void);
/* LCD FULL Clear */
void Lcd_FullClear(void);
/* LCD Write RAM Index bit */
en_result_t Lcd_WriteDdramIndexBit(uint8_t u8Row, uint8_t u8List, boolean_t bDate);
/* LCD Read RAM Index bit */
boolean_t Lcd_ReadDdramIndexBit(uint8_t u8Row, uint8_t u8List);
/* LCD Write RAM Index byte */
en_result_t LCD_WriteDdramIndexByte(uint8_t u8Row, uint8_t bData);
/* LCD Read RAM Index byte */
uint8_t LCD_ReadDdramIndexByte(uint8_t u8Row );


//@} // LcdGroup
#ifdef __cplusplus
}
#endif

#endif /* __LCD_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
