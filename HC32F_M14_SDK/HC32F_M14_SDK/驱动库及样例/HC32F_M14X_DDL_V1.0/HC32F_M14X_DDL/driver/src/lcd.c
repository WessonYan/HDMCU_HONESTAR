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
/** \file lcd.c
 **
 ** A detailed description is available at
 ** @link LcdGroup Lcd description @endlink
 **
 **   - 2017-8-14  1.0  Jason First version for Device Driver Library of lcd
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "lcd.h"

/**
 *******************************************************************************
 ** \addtogroup LcdGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*! Parameter validity check for frame refresh rate \a x. */
#define IS_VALID_REFRESH_RATE(x)        \
(   LcdRefreshRate128Hz     <= (x) &&   \
    LcdRefreshRate1024Hz    >= (x))

/*! Parameter validity check for display mode \a x. */
#define IS_VALID_DISPLAY_MODE(x)        \
(   LcdDisModeDutyStatic    <= (x) &&   \
    LcdDisModeDuty1Div8     >= (x))

/*! Parameter validity check for contrast \a x. */
#define IS_VALID_CONTRAST(x)            \
(   LcdContrast60per        <= (x) &&   \
    LcdContrast100per       >= (x))

/*! Parameter validity check for charge time \a x. */
#define IS_VALID_CHARGE_TIME(x)         \
(   LcdChargeTime1Clk       <= (x) &&   \
    LcdChargeTime8Clk       >= (x))

/*! Parameter validity check for charge mode \a x. */
#define IS_VALID_CHARGE_MODE(x)         \
(   LcdChargeModeFast       == (x) ||   \
    LcdChargeModeLargeCur   == (x))

/*! Parameter validity check for charge pump charging frequence \a x. */
#define IS_VALID_CHARGE_PUMP_FRE(x)     \
(   LcdChargePumpFreq2kHz   <= (x) &&   \
    LcdChargePumpFreq16kHz  >= (x))

/*! Parameter validity check for drive circuit \a x. */
#define IS_VALID_DRIVE_CIRCUIT(x)       \
(   LcdBiasCircuitInRes     <= (x) &&   \
    LcdBiasCircuitExRes     >= (x))

/*! Parameter validity check for drive capability \a x. */
#define IS_VALID_DRIVE_CAP(x)           \
(   LcdDriveCapabilityNormal<= (x) &&   \
    LcdDriveCapability4Times>= (x))

/*! Parameter validity check for working mode \a x. */
#define IS_VALID_WORK_MODE(x)           \
(   Lcd_8Com_1Div8Duty_1Div3bias_TypeA_Max36Seg <= (x) &&   \
    Lcd_Com0_StaticMode_Max40Seg >= (x))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

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
 ** \brief Initialize lcd module by configuration structure
 **
 ** \param [in] pstcLcdInitconfig       Pointer to lcd config
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t Lcd_InitConfig(stc_lcd_cfg_t* pstcLcdInitconfig)
{
    en_result_t  enresult =  Error;
    volatile stc_lcdc_regtyptedef_t *pLCDReg = M0P_LCDC;

    /* Parameter detection. */
    ASSERT(NULL != pstcLcdInitconfig);
    ASSERT(NULL != pLCDReg);
    ASSERT(IS_VALID_REFRESH_RATE(pstcLcdInitconfig->enLcdRefreshRate));
    ASSERT(IS_VALID_DRIVE_CIRCUIT(pstcLcdInitconfig->stcLcdDriveCircuit.enDriveCircuit));
    ASSERT(IS_VALID_DRIVE_CAP(pstcLcdInitconfig->enLcdDriveCap));
    ASSERT(IS_VALID_WORK_MODE(pstcLcdInitconfig->enLcdWorkMode));

    /* Setting function state return variables. */
    enresult =  Ok;

    /* LCD Display CLOCK Select. */
    pLCDReg->LCDCR0_f.CLKSEL = pstcLcdInitconfig->enLcdClk;
    /* LCD frame refresh rate. */
    pLCDReg->LCDCR0_f.LCD_FRAME  = pstcLcdInitconfig->enLcdRefreshRate;

    /* LCD working mode set,include com、bias、duty、waveform、set. */
    switch( pstcLcdInitconfig->enLcdWorkMode)
    {
        /* 8 COM mode, pin SEG0 ~ SEG35 as segment signal output pin. */
        case Lcd_8Com_1Div8Duty_1Div3bias_TypeA_Max36Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = TRUE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = TRUE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = TRUE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = TRUE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div3;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div8;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeA;

            break;

        case Lcd_8Com_1Div8Duty_1Div3bias_TypeB_Max36Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = TRUE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = TRUE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = TRUE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = TRUE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div3;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div8 ;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeB;

            break;

        case Lcd_8Com_1Div8Duty_1Div4bias_TypeA_Max36Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = TRUE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = TRUE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = TRUE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = TRUE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div4;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div8 ;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeA;

            break;

        case Lcd_8Com_1Div8Duty_1Div4bias_TypeB_Max36Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = TRUE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = TRUE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = TRUE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = TRUE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div4;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div8 ;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeB;

            break;

        /* 6 COM mode, pin SEG0 ~ SEG35, SEG38 and SEG39 as segment signal output pin. */
        case Lcd_6Com_1Div6Duty_1Div3bias_TypeA_Max38Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = TRUE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = TRUE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div3;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div6;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeA;

            break;

        case Lcd_6Com_1Div6Duty_1Div3bias_TypeB_Max38Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = TRUE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = TRUE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div3;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div6;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeB;

            break;

        case Lcd_6Com_1Div6Duty_1Div4bias_TypeA_Max38Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = TRUE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = TRUE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div4;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div6;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeA;

            break;

        case Lcd_6Com_1Div6Duty_1Div4bias_TypeB_Max38Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = TRUE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = TRUE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div4;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div6;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeB;

            break;

        /* 4 COM mode, pin SEG0 ~ SEG39 as segment signal output pin. */
        case Lcd_4Com_1Div2Duty_1Div2bias_TypeA_Max40Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = FALSE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = FALSE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div2;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div2;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeA;

            break;

        case Lcd_4Com_1Div2Duty_1Div2bias_TypeB_Max40Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = FALSE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = FALSE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div2;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div2;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeB;

            break;

        case Lcd_4Com_1Div2Duty_1Div3bias_TypeA_Max40Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = FALSE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = FALSE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div3;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div2;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeA;

            break;

        case Lcd_4Com_1Div2Duty_1Div3bias_TypeB_Max40Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = FALSE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = FALSE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div3;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div2;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeB;

            break;

        case Lcd_4Com_1Div3Duty_1Div2bias_TypeA_Max40Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = FALSE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = FALSE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div2;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div3;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeA;

            break;

        case Lcd_4Com_1Div3Duty_1Div2bias_TypeB_Max40Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = FALSE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = FALSE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div2;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div3;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeB;

            break;

        case Lcd_4Com_1Div3Duty_1Div3bias_TypeA_Max40Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = FALSE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = FALSE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div3;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div3;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeA;

            break;

        case Lcd_4Com_1Div3Duty_1Div3bias_TypeB_Max40Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = FALSE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = FALSE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div3;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div3;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeB;

            break;


        case Lcd_4Com_1Div4Duty_1Div3bias_TypeA_Max40Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = FALSE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = FALSE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div3;
            /* LCD display mode choice 1/4 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div4;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeA;

            break;


        case Lcd_4Com_1Div4Duty_1Div3bias_TypeB_Max40Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = FALSE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = FALSE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div3;
            /* LCD display mode choice 1/4 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div4;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeB;

            break;


        case Lcd_4Com_1Div4Duty_1Div4bias_TypeA_Max40Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = FALSE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = FALSE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div4;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div4 ;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeA;

            break;

        case Lcd_4Com_1Div4Duty_1Div4bias_TypeB_Max40Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = TRUE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = TRUE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = TRUE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = FALSE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = FALSE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div4;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDuty1Div4 ;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeB;

            break;

        /* Static mode, pin SEG0 ~ SEG39 as segment signal output pin. */
        case Lcd_Com0_StaticMode_Max40Seg:

            /* COME0 function select. */
            pLCDReg->LCDCE0_f.COME0 = TRUE;
            /* COME1 function select. */
            pLCDReg->LCDCE0_f.COME1 = FALSE;
            /* COME2 function select. */
            pLCDReg->LCDCE0_f.COME2 = FALSE;
            /* COME3 function select. */
            pLCDReg->LCDCE0_f.COME3 = FALSE;
            /* COME4 function select. */
            pLCDReg->LCDCE0_f.COME4 = FALSE;
            /* COME5 function select. */
            pLCDReg->LCDCE0_f.COME5 = FALSE;
            /* COME6 function select. */
            pLCDReg->LCDCE0_f.COME6 = FALSE;
            /* COME7 function select. */
            pLCDReg->LCDCE0_f.COME7 = FALSE;

            /* LCD bias Settings. */
            pLCDReg->LCDCR0_f.BIAS  = LcdBias1Div2;
            /* LCD display mode choice 1/8 duty. */
            pLCDReg->LCDCR0_f.LCD_MODE  = LcdDisModeDutyStatic ;
            /* LCD driver waveform selection. */
            pLCDReg->LCDCR2_f.WSEL = LcdDriveWaveTypeB;

            break;

        default:
            enresult = ErrorInvalidParameter;
            /* Provided parameter is not valid. */
            return enresult;
    }

    /* Drive circuit. */
    pLCDReg->LCDCR2_f.VSEL = pstcLcdInitconfig->stcLcdDriveCircuit.enDriveCircuit;
    /* LCD drive circuit selection,include in R、 Ex R 、 Ex C、 Register (R03、R13、R23、R33、R43)driving voltagesetting. */
    switch( pstcLcdInitconfig->stcLcdDriveCircuit.enDriveCircuit)
    {
        /* LCD bias circuit selection Internal resistance. */
        case LcdBiasCircuitInRes:

            /* LCD contrast to choose. */
            pLCDReg->LCDCR1_f.VLCD = pstcLcdInitconfig->stcLcdDriveCircuit.stcLcdInrCfg.enContrast;
            /* LCD Quick charge mode, small internal resistance charging time choice. */
            pLCDReg->LCDCR1_f.CT  = pstcLcdInitconfig->stcLcdDriveCircuit.stcLcdInrCfg.enChargingTime;
            /* LCD When using internal resistance, charging mode selection. */
            pLCDReg->LCDCR1_f.CMOD = pstcLcdInitconfig->stcLcdDriveCircuit.stcLcdInrCfg.enChargingMode;

            /* Choose a R03 port function AS GPIO. */
            pLCDReg->LCDDVC_f.VE0 =  FALSE;
            /* Choose a R13 port function AS GPIO. */
            pLCDReg->LCDDVC_f.VE1 =  FALSE;
            /* Choose a R23 port function AS GPIO. */
            pLCDReg->LCDDVC_f.VE2 =  FALSE;
            /* Choose a R33 port function AS GPIO. */
            pLCDReg->LCDDVC_f.VE3 =  FALSE;
            /* Choose a R43 port function AS GPIO. */
            pLCDReg->LCDDVC_f.VE4 =  FALSE;

            break;
        /* LCD bias circuit selection  external capacitance. */
        case LcdBiasCircuitExCap:

            /* LCD When using external capacitance, charge pump working frequency selection. */
            pLCDReg->LCDCR2_f.CP_FREQ = pstcLcdInitconfig->stcLcdDriveCircuit.stcLcdExcapCfg.enChargePumpFreq;

            /* Choose a R03 port function AS LCD drive voltage input. */
            pLCDReg->LCDDVC_f.VE0 =  TRUE;
            /* Choose a R13 port function AS LCD drive voltage input. */
            pLCDReg->LCDDVC_f.VE1 =  TRUE;
            /* Choose a R23 port function AS LCD drive voltage input. */
            pLCDReg->LCDDVC_f.VE2 =  TRUE;
            /* Choose a R33 port function AS LCD drive voltage input. */
            pLCDReg->LCDDVC_f.VE3 =  TRUE;
            /* Choose a R43 port function AS LCD drive voltage input. */
            pLCDReg->LCDDVC_f.VE4 =  TRUE;

            break;
        /* LCD bias circuit selection  external resistance. */
        case LcdBiasCircuitExRes:

            /* Choose a R03 port function AS LCD drive voltage input. */
            pLCDReg->LCDDVC_f.VE0 =  TRUE;
            /* Choose a R13 port function AS LCD drive voltage input. */
            pLCDReg->LCDDVC_f.VE1 =  TRUE;
            /* Choose a R23 port function AS LCD drive voltage input. */
            pLCDReg->LCDDVC_f.VE2 =  TRUE;
            /* Choose a R33 port function AS LCD drive voltage input. */
            pLCDReg->LCDDVC_f.VE3 =  TRUE;
            /* Choose a R43 port function AS LCD drive voltage input. */
            pLCDReg->LCDDVC_f.VE4 =  TRUE;

            break;

        default:
            enresult = ErrorInvalidParameter;
            /* Provided parameter is not valid. */
            return enresult;
    }

    /* LCD COM、Seg i/o drive ability set. */
    pLCDReg->LCDCR3_f.LCD_BUF = pstcLcdInitconfig->enLcdDriveCap;

    /* u8LcdSe0~u8LcdSe4 Regist configuration. */
    /* 8 COM mode, pin SEG0 ~ SEG35 as segment signal output pin. */
    /* 6 COM mode, pin SEG0 ~ SEG35, SEG38 and SEG39 as segment signal output pin. */
    /* 4 COM mode, pin SEG0 ~ SEG39 as segment signal output pin. */
    /* Static mode, pin SEG0 ~ SEG39 as segment signal output pin. */
    /* SEG0~SEG7 SET. */
    pLCDReg->LCDSE0 = pstcLcdInitconfig->u8Seg0_Seg7Config;

    /* SEG8~SEG15 SET. */
    pLCDReg->LCDSE1 = pstcLcdInitconfig->u8Seg8_Seg15Config;

    /* SEG16~SEG23 SET. */
    pLCDReg->LCDSE2 = pstcLcdInitconfig->u8Seg16_Seg23Config;

    /* SEG24~SEG31 SET. */
    pLCDReg->LCDSE3 = pstcLcdInitconfig->u8Seg24_Seg31Config;

    /* SEG32~SEG39 SET. */
    pLCDReg->LCDSE4 = pstcLcdInitconfig->u8Seg32_Seg39Config;

    return  enresult;
}

/**
 *******************************************************************************
 ** \brief Enable or disable lcd
 **
 ** \param [in] bNewState               Enable or disable
 ** \arg ENABLE                         Enable lcd
 ** \arg DISABLE                        Disable lcd
 **
 ** \retval None
 **
 ******************************************************************************/
void Lcd_Cmd(boolean_t bNewState)
{
    /* LCD Enable. */
    M0P_LCDC->LCDCR3_f.LCD_EN = bNewState;
}

/**
 *******************************************************************************
 ** \brief Lcd full display
 **
 ** \param None
 **
 ** \retval None
 **
 ******************************************************************************/
void Lcd_FullWrite(void)
{
    int32_t i=0;
    volatile uint8_t *ptemp = NULL;

    /* get LCD_RAMXX u8LcdRam00 Address. */
    ptemp = &M0P_LCDC->LCDRAM00;
    /* All members of the assignment. */
    for(i=0;i<=LCDRAMSIZE;i++)
    {
        *ptemp = LCDFULLWRITE;
        ptemp++;
    }

}

/**
 *******************************************************************************
 ** \brief Lcd full clear
 **
 ** \param None
 **
 ** \retval None
 **
 ******************************************************************************/
void Lcd_FullClear(void)
{
      int32_t i=0;
    volatile uint8_t *ptemp = NULL;
    /* get LCD_RAMXX u8LcdRam00 Address. */
    ptemp = &M0P_LCDC->LCDRAM00;
    /* All members of the assignment. */
    for(i=0;i<=LCDRAMSIZE;i++)
    {
        *ptemp = LCDFULLCLEAR;
        ptemp++;
    }
}

/**
 *******************************************************************************
 ** \brief Lcd write the specified bit in the ddram
 **
 ** \param [in] u8Row                   RAM00~RAM39
 ** \param [in] u8List                  Bit0~Bit7
 ** \param [in] bData                   1 or 0
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - u8Row or u8List is invalid
 **
 ******************************************************************************/
en_result_t Lcd_WriteDdramIndexBit(uint8_t u8Row, uint8_t u8List, boolean_t bData)
{
    /* Define function returns the state variables. */
    en_result_t  enresult = Error;
    /* Define a pointer variable. */
    volatile uint8_t *ptemp = NULL;

    /* Determine whether the input parameters conform to the limit. */
    if( (u8Row > LCDRAMSIZE) || (u8List > LCDRAMu8ListSIZE))
    {
        enresult = ErrorInvalidParameter;
        return enresult;
    }
    /* set function returns state. */
    enresult = Ok;
    /* get LCD_RAMXX u8LcdRam00 Address. */
    ptemp = &M0P_LCDC->LCDRAM00;
    /* Point to modify LCDRAM u8Row. */
    ptemp += u8Row;

    if( bData == TRUE)
    {
        /* SET bit. */
        *ptemp |= (uint8_t)(bData<<u8List);
    }
    else
    {
        /* clear bit. */
        *ptemp &= (uint8_t)~(uint8_t)(bData<<u8List);
    }

    return  enresult;
}

/**
 *******************************************************************************
 ** \brief Lcd read the specified bit from the ddram
 **
 ** \param [in] u8Row                   RAM00~RAM39
 ** \param [in] u8List                  Bit0~Bit7
 **
 ** \retval boolean_t                   The specified bit in the ddram
 **
 ******************************************************************************/
boolean_t Lcd_ReadDdramIndexBit(uint8_t u8Row, uint8_t u8List)
{
    /* Define a pointer variable. */
    volatile uint8_t *ptemp = NULL;
    uint8_t temp = 0;

    /* get LCD_RAMXX u8LcdRam00 Address. */
    ptemp = &M0P_LCDC->LCDRAM00;
    /* Point to modify LCDRAM u8Row. */
    ptemp += u8Row;
    /* Get the specified bit data. */
    temp = (0x00u | (1u<<u8List));
    temp &= (*ptemp);
    temp = temp>>u8List;

    /* return data. */
    return  (boolean_t)temp;
}

/**
 *******************************************************************************
 ** \brief Lcd write the specified byte in the lcdram
 **
 ** \param [in] u8Row                   RAM00~RAM39
 ** \param [in] bData                   Data
 **
 ** \retval Ok                          Process successfully done
 ** \ErrorInvalidParameter              If one of following conditions are met:
 **                                     - u8Row invalid
 **
 ******************************************************************************/
en_result_t LCD_WriteDdramIndexByte(uint8_t u8Row, uint8_t bData)
{
    /* Define function returns the state variables. */
    en_result_t  enresult = Error;
    /* Define a pointer variable. */
    volatile uint8_t *ptemp = NULL;

    /* Determine whether the input parameters conform to the limit. */
    if( u8Row > LCDRAMSIZE )
    {
        enresult = ErrorInvalidParameter;
        return enresult;
    }
    /* set function returns state. */
    enresult = Ok;
    /* get LCD_RAMXX u8LcdRam00 Address. */
    ptemp = &M0P_LCDC->LCDRAM00;
    /* Point to modify LCDRAM u8Row. */
    ptemp += u8Row;
    /* Write a specified byte. */
    *ptemp = bData;

    return  enresult;
}

/**
 *******************************************************************************
 ** \brief Lcd read the specified byte from the ddram
 **
 ** \param [in] u8Row                   RAM00~RAM39
 **
 ** \retval uint8_t                     The specified byte in the ddram
 **
 ******************************************************************************/
uint8_t LCD_ReadDdramIndexByte(uint8_t u8Row )
{
    /* Define a pointer variable. */
    volatile uint8_t *ptemp = NULL;

    /* get LCD_RAMXX u8LcdRam00 Address. */
    ptemp = &M0P_LCDC->LCDRAM00;
    /* Point to modify LCDRAM u8Row. */
    ptemp += u8Row;

    return  (*ptemp);
}

//@} // LcdGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
