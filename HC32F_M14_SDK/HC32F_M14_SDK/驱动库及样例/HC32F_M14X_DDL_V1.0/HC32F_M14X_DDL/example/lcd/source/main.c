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
/** \file main.c
 **
 ** \brief The example for lcd demonstration.
 **        Get more details from readme text.
 **
 **   - 2017-8-9  1.0  Jason First version for Device Driver Library of lcd.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "lcd.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
typedef enum en_lcd_digs
{
    lcd_dig1 = 0,
    lcd_dig2,
    lcd_dig3,
    lcd_dig4,
} en_lcd_digs_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/* Element assignment in a nixietube */
/*****************
        A
     ------
    |      |
   F|   G  | B
    |______|
    |      |
   E|      | C
    |______|
        D
******************/

/* Pin to LCD ram */
/********************************************************
    |----LCD-S401M16TF------|   |----------LCDC---------|
    |   pin 12  --- SEG8    |   |   pin 55  --- SEG 20  |
    |   pin 11  --- SEG7    |   |   pin 46  --- SEG 13  |
    |   pin 10  --- SEG6    |   |   pin 40  --- SEG 7   |
    |   pin 9   --- SEG5    |   |   pin 39  --- SEG 6   |
    |   pin 8   --- SEG4    |   |   pin 29  --- SEG 5   |
    |   pin 7   --- SEG3    |   |   pin 26  --- SEG 2   |
    |   pin 6   --- SEG2    |   |   pin 25  --- SEG 1   |
    |   pin 5   --- SEG1    |   |   pin 24  --- SEG 0   |
    |   pin 4   --- COM3    |   |   pin 35  --- COM3    |
    |   pin 3   --- COM2    |   |   pin 36  --- COM2    |
    |   pin 2   --- COM1    |   |   pin 37  --- COM1    |
    |   pin 1   --- COM0    |   |   pin 38  --- COM0    |
    |-----------------------|   |-----------------------|
********************************************************/

#define DPON                TRUE
#define DPOFF               FALSE

#define FIRST_SEG           1
#define SECOND_SEG          2
#define BOTH_SEG            3

#define COM0                0x01
#define COM1                0x02
#define COM2                0x04
#define COM3                0x08
#define A                   COM3
#define B                   COM2
#define C                   COM1
#define D                   COM0
#define E                   COM1
#define F                   COM3
#define G                   COM2
#define DP1                 COM0
#define DP2                 COM0
#define DP3                 COM0
#define COL                 COM0

#define NUM0                A | B | C | D | E | F
#define NUM1                E | F
#define NUM2                A | B | G | E | D
#define NUM3                A | B | C | D | G
#define NUM4                F | B | G | C
#define NUM5                A | F | G | C | D
#define NUM6                A | F | G | C | D | E
#define NUM7                A | B | C
#define NUM8                A | B | C | D | E | F | G
#define NUM9                A | B | C | D | F | G

#define VALID_DATA          0xFF
#define VALID_SEG           0xFF

#define N_FIRST_0           (D | E | F)
#define N_SECOND_0          (A | B | C)
#define N_FIRST_1           (E | F)
#define N_SECOND_1          (VALID_DATA)
#define N_FIRST_2           (D | E | G)
#define N_SECOND_2          (A | B)
#define N_FIRST_3           (D | G)
#define N_SECOND_3          (A | B | C)
#define N_FIRST_4           (F | G)
#define N_SECOND_4          (B | C)
#define N_FIRST_5           (D | F | G)
#define N_SECOND_5          (A | C)
#define N_FIRST_6           (D | E | F| G)
#define N_SECOND_6          (A | C)
#define N_FIRST_7           (VALID_DATA)
#define N_SECOND_7          (A | B | C)
#define N_FIRST_8           (D | E | F| G)
#define N_SECOND_8          (A | B | C)
#define N_FIRST_9           (D | F | G)
#define N_SECOND_9          (A | B | C)

#define N_FIRST(n)          N_FIRST_##n
#define N_SECOND(n)         N_SECOND_##n

#define SEG_DP(dig)         (dig == lcd_dig1 ? DP1 : \
                            (dig == lcd_dig2 ? DP2 : \
                            (dig == lcd_dig3 ? DP3 : COL)))

#define SEG_DATA(n,p1,p2)   do{                                     \
                                if(p1)                              \
                                {                                   \
                                    *(uint8_t* )p1 = N_FIRST(n);    \
                                }                                   \
                                if(p2)                              \
                                {                                   \
                                    *(uint8_t* )p2 = N_SECOND(n);   \
                                }                                   \
                            }while(0)

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
 ** \brief Convert digits from 0 to 9 to dispaly data
 **
 ** \param [in] u8Content               The digits from 0 to 9
 **
 ** \param [out] pSegUsed               Use one or two segs
 **
 ** \param [out] pFisrtSegData          The display data for the first seg
 **
 ** \param [out] pSecondSegData         The display data for the second seg
 **
 ** \retval None
 **
 ******************************************************************************/
static void segData(uint8_t u8Content, uint8_t* pSegUsed, uint8_t* pFisrtSegData, uint8_t* pSecondSegData)
{
    ASSERT(NULL != pSegUsed);
    ASSERT(NULL != pFisrtSegData);
    ASSERT(NULL != pSecondSegData);

    *pFisrtSegData = VALID_DATA;
    *pSecondSegData = VALID_DATA;

    switch(u8Content)
    {
        case 0:
            SEG_DATA(0, pFisrtSegData, pSecondSegData);
            *pSegUsed = BOTH_SEG;
            break;
        case 2:
            SEG_DATA(2, pFisrtSegData, pSecondSegData);
            *pSegUsed = BOTH_SEG;
            break;
        case 3:
            SEG_DATA(3, pFisrtSegData, pSecondSegData);
            *pSegUsed = BOTH_SEG;
            break;
        case 4:
            SEG_DATA(4, pFisrtSegData, pSecondSegData);
            *pSegUsed = BOTH_SEG;
            break;
        case 5:
            SEG_DATA(5, pFisrtSegData, pSecondSegData);
            *pSegUsed = BOTH_SEG;
            break;
        case 6:
            SEG_DATA(6, pFisrtSegData, pSecondSegData);
            *pSegUsed = BOTH_SEG;
            break;
        case 8:
            SEG_DATA(8, pFisrtSegData, pSecondSegData);
            *pSegUsed = BOTH_SEG;
            break;
        case 9:
            SEG_DATA(9, pFisrtSegData, pSecondSegData);
            *pSegUsed = BOTH_SEG;
            break;
        case 1:
            SEG_DATA(1, pFisrtSegData, NULL);
            *pSegUsed = FIRST_SEG;
            break;
        case 7:
            SEG_DATA(7, NULL, pSecondSegData);
            *pSegUsed = SECOND_SEG;
            break;
        default:
            break;
    }

    return;
}

/**
 *******************************************************************************
 ** \brief Get the seg pin index from lcdc
 **
 ** \param [in] enDig                   Get seg pin index for enDig
 ** \arg lcd_dig1                       Get seg pin index for dig1
 ** \arg lcd_dig2                       Get seg pin index for dig2
 ** \arg lcd_dig3                       Get seg pin index for dig3
 ** \arg lcd_dig4                       Get seg pin index for dig4
 **
 ** \param [out] pFirstSeg              Get the first seg pin index
 **
 ** \param [out] pSecondSeg             Get the second seg pin index
 **
 ** \retval None
 **
 ******************************************************************************/
static void segIndex(en_lcd_digs_t enDig, uint8_t* pFirstSeg, uint8_t* pSecondSeg)
{
    ASSERT(NULL != pFirstSeg);
    ASSERT(NULL != pSecondSeg);

    switch(enDig)
    {
        case lcd_dig1:
            *pFirstSeg = 0;
            *pSecondSeg = 1;
            break;
        case lcd_dig2:
            *pFirstSeg = 2;
            *pSecondSeg = 5;
            break;
        case lcd_dig3:
            *pFirstSeg = 6;
            *pSecondSeg = 7;
            break;
        case lcd_dig4:
            *pFirstSeg = 13;
            *pSecondSeg = 20;
            break;
        default:
            *pFirstSeg = 0xff;
            *pSecondSeg = 0xff;
            break;
    }

    return;
}

/**
 *******************************************************************************
 ** \brief Display dp
 **
 ** \param [in] enDig                   Display dp on enDig
 ** \arg lcd_dig1                       Display dp on dig1
 ** \arg lcd_dig2                       Display dp on dig2
 ** \arg lcd_dig3                       Display dp on dig3
 ** \arg lcd_dig4                       Display dp on dig4
 **
 ** \param [in] bOnOff                  Display dp or not
 **
 ** \retval None
 **
 ******************************************************************************/
static void showDpoint(en_lcd_digs_t enDig, boolean_t bOnOff)
{
    uint8_t u8firstSeg = VALID_SEG;
    uint8_t u8secondSeg = VALID_SEG;

    segIndex(enDig, &u8firstSeg, &u8secondSeg);

    Lcd_WriteDdramIndexBit(u8secondSeg, SEG_DP(enDig) - 1, bOnOff);
}

/**
 *******************************************************************************
 ** \brief Display digs
 **
 ** \param [in] enDig                   The dig to display
 ** \arg lcd_dig1                       Display dig1
 ** \arg lcd_dig2                       Display dig2
 ** \arg lcd_dig3                       Display dig3
 ** \arg lcd_dig4                       Display dig4
 **
 ** \param [in] u8Content               The digits of enDig to display
 **
 ** \retval None
 **
 ******************************************************************************/
static void showDig(en_lcd_digs_t enDig, uint8_t u8Content)
{
    uint8_t u8segUsed = 0;
    uint8_t u8firstSeg = VALID_SEG;
    uint8_t u8secondSeg = VALID_SEG;
    uint8_t u8firstSegData = VALID_DATA;
    uint8_t u8secondSegData = VALID_DATA;

    segData(u8Content, &u8segUsed, &u8firstSegData, &u8secondSegData);
    segIndex(enDig, &u8firstSeg, &u8secondSeg);

    if((u8firstSeg == VALID_SEG) || (u8secondSeg == VALID_SEG))
    {
        return;
    }

    if((u8firstSegData == VALID_DATA) && (u8secondSegData == VALID_DATA))
    {
        return;
    }

    if((u8segUsed != FIRST_SEG) && (u8segUsed != SECOND_SEG) && (u8segUsed != BOTH_SEG))
    {
        return;
    }

    if(u8segUsed == FIRST_SEG)
    {
        LCD_WriteDdramIndexByte(u8firstSeg, u8firstSegData);
    }
    else if(u8segUsed == SECOND_SEG)
    {
        LCD_WriteDdramIndexByte(u8secondSeg, u8secondSegData);
    }
    else
    {
        LCD_WriteDdramIndexByte(u8firstSeg, u8firstSegData);
        LCD_WriteDdramIndexByte(u8secondSeg, u8secondSegData);
    }
}

/**
 *******************************************************************************
 ** \brief Display lcd
 **
 ** \param [in] u8Dig1                  The digits of dig1 to display
 ** \param [in] u8Dig2                  The digits of dig2 to display
 ** \param [in] u8Dig3                  The digits of dig3 to display
 ** \param [in] u8Dig4                  The digits of dig4 to display
 **
 ** \param [in] bDp1OnOff               Display dp1 or not
 ** \param [in] bDp2OnOff               Display dp2 or not
 ** \param [in] bDp3OnOff               Display dp3 or not
 ** \param [in] bDp4OnOff               Display dp4 or not
 **
 ** \retval None
 **
 ******************************************************************************/
static void showLcd(uint8_t u8Dig1, uint8_t u8Dig2, uint8_t u8Dig3, uint8_t u8Dig4,
                    boolean_t bDp1OnOff, boolean_t bDp2OnOff, boolean_t bDp3OnOff, boolean_t bDp4OnOff)
{
    showDig(lcd_dig1, u8Dig1);
    showDig(lcd_dig2, u8Dig2);
    showDig(lcd_dig3, u8Dig3);
    showDig(lcd_dig4, u8Dig4);
    showDpoint(lcd_dig1, bDp1OnOff);
    showDpoint(lcd_dig2, bDp2OnOff);
    showDpoint(lcd_dig3, bDp3OnOff);
    showDpoint(lcd_dig4, bDp4OnOff);
}
/**
 *******************************************************************************
 ** \brief LCD initialization configuration
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t Lcd_InitFunc(void)
{
    stc_lcd_cfg_t  stcLcdInitconfig;
    en_result_t  enresult =  Error;

    DDL_ZERO_STRUCT(stcLcdInitconfig);

    /* LCD clock selected internal RC. */
    stcLcdInitconfig.enLcdClk = LcdClkLICR;
    /* The frame refresh rate select 128hz. */
    stcLcdInitconfig.enLcdRefreshRate =  LcdRefreshRate512Hz;
    /* LCD working mode set,include com、bias、duty、waveform、set. */
    stcLcdInitconfig.enLcdWorkMode  = Lcd_4Com_1Div4Duty_1Div3bias_TypeA_Max40Seg;

    /* LCD drive circuit select,in R. */
    stcLcdInitconfig.stcLcdDriveCircuit.enDriveCircuit = LcdBiasCircuitInRes;
    /* LCD contrast select,Contrast 100%. */
    stcLcdInitconfig.stcLcdDriveCircuit.stcLcdInrCfg.enContrast = LcdContrast100per;
    /* LCD Charge time select,Charging time 1 CLK. */
    stcLcdInitconfig.stcLcdDriveCircuit.stcLcdInrCfg.enChargingTime = LcdChargeTime1Clk;
    /* LCD Charge mode select,Quick charging mode. */
    stcLcdInitconfig.stcLcdDriveCircuit.stcLcdInrCfg.enChargingMode = LcdChargeModeFast;
    /* LCD drive power select Normal. */
    stcLcdInitconfig.enLcdDriveCap = LcdDriveCapabilityNormal;

    /* LCD Port config. */
    stcLcdInitconfig.stcSeg0_Seg7Config.SEGE0 = ENABLE;
    stcLcdInitconfig.stcSeg0_Seg7Config.SEGE1 = ENABLE;
    stcLcdInitconfig.stcSeg0_Seg7Config.SEGE2 = ENABLE;
    stcLcdInitconfig.stcSeg0_Seg7Config.SEGE5 = ENABLE;
    stcLcdInitconfig.stcSeg0_Seg7Config.SEGE6 = ENABLE;
    stcLcdInitconfig.stcSeg0_Seg7Config.SEGE7 = ENABLE;
    stcLcdInitconfig.stcSeg8_Seg15Config.SEGE13 = ENABLE;
    stcLcdInitconfig.stcSeg16_Seg23Config.SEGE20 = ENABLE;

    /* According to the initialization configuration structure LCD registers. */
    enresult = Lcd_InitConfig( &stcLcdInitconfig );

    return enresult;
}

/**
 *******************************************************************************
 ** \brief Main function of project
 **
 ******************************************************************************/
int32_t main(void)
{
    /* LCD initialization configuration. */
    Lcd_InitFunc();
    /* LCD Enable. */
    Lcd_Cmd( ENABLE );

    /* Display 8.8:8.8 */
    showLcd(8, 8, 8, 8, DPON, DPON, DPON, DPON);
    Lcd_FullClear();

    /* Display 0 1 2 3 */
    showLcd(0, 1, 2, 3, DPOFF, DPOFF, DPOFF, DPOFF);
    Lcd_FullClear();

    /* Display 4 5 6 7 */
    showLcd(4, 5, 6, 7, DPOFF, DPOFF, DPOFF, DPOFF);
    Lcd_FullClear();

    /* Display 8 9 0 1 */
    showLcd(8, 9, 0, 1, DPOFF, DPOFF, DPOFF, DPOFF);
    Lcd_FullClear();

    /* Full dispaly */
    Lcd_FullWrite();

    return 0;
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
