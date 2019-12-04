/*****************************************************************************
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
/** \file gpio.h
 **
 ** A detailed description is available at
 ** @link GpioGroup Gpio description @endlink
 **
 **   - 2018-08-11  1.0  chengy First version for Device Driver Library of Gpio.
 **
 ******************************************************************************/

#ifndef __GPIO_H__
#define __GPIO_H__

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
 ** \defgroup GpioGroup General Purpose Input/Output(GPIO)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief GPIO input or output seclected
 **
 ** \note To select GPIO input or output
 **
 ******************************************************************************/
typedef enum en_Gpio_dir
{
    GpioDirIn,                      ///< input.
    GpioDirOut,                     ///< output.
}en_Gpio_dir_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* pin function configure macro */
#define Gpio_SetFunc_VCM(x)         do{\
                                        bM0P_GPIO_ANALOG_ADE23 = x;\
                                    }while(0)

#define Gpio_SetFunc_VCP(x)         do{\
                                        bM0P_GPIO_ANALOG_ADE22 = x;\
                                    }while(0)

#define Gpio_SetFunc_OPA1O(x)       do{\
                                        bM0P_GPIO_ANALOG_ADE21 = x;\
                                    }while(0)

#define Gpio_SetFunc_OPA1M(x)       do{\
                                        bM0P_GPIO_ANALOG_ADE20 = x;\
                                    }while(0)

#define Gpio_SetFunc_OPA1P(x)       do{\
                                        bM0P_GPIO_ANALOG_ADE19 = x;\
                                    }while(0)

#define Gpio_SetFunc_OPA0O(x)       do{\
                                        bM0P_GPIO_ANALOG_ADE18 = x;\
                                    }while(0)

#define Gpio_SetFunc_OPA0M(x)       do{\
                                        bM0P_GPIO_ANALOG_ADE17 = x;\
                                    }while(0)

#define Gpio_SetFunc_OPA0P(x)       do{\
                                        bM0P_GPIO_ANALOG_ADE16 = x;\
                                    }while(0)

/* FN_SEL00 */
#define Gpio_SetFunc_NMIX(x)        do{\
                                        M0P_GPIO->FN0_f.P07 = 1;\
                                        M0P_GPIO->FN_SEL00_f.NMIS = 1;\
                                    }while(0)

#define Gpio_SetFunc_IRCO_0(x)      do{\
                                        M0P_GPIO->ANALOG_f.ADE13 = 0;\
                                        M0P_GPIO->FN7_f.P73 = 1;\
                                        M0P_GPIO->FN_SEL00_f.CROUTE = 1;\
                                    }while(0)

#define Gpio_SetFunc_IRCO_1(x)      do{\
                                        M0P_GPIO->FN6_f.P65 = 1;\
                                        M0P_GPIO->FN_SEL00_f.CROUTE = 2;\
                                    }while(0)

#define Gpio_SetFunc_IRCO_2(x)      do{\
                                        M0P_GPIO->FN6_f.P67 = 1;\
                                        M0P_GPIO->FN_SEL00_f.CROUTE = 3;\
                                    }while(0)

#define Gpio_SetFunc_SWDEN(x)       do{\
                                        M0P_GPIO->FN3_f.P30 = 1;\
                                        M0P_GPIO->FN3_f.P31 = 1;\
                                        M0P_GPIO->FN_SEL00_f.SWDEN = (x);\
                                    }while(0)

/* FN_SEL01 */
#define Gpio_SetFunc_PWM00_0(x)   do{\
                                        bM0P_GPIO_ANALOG_ADE10 = 0;\
                                        M0P_GPIO->FN7_f.P70 = 1;\
                                        M0P_GPIO->FN_SEL01_f.PWM00E = 1;\
                                    }while(0)

#define Gpio_SetFunc_PWM01_0(x)   do{\
                                        bM0P_GPIO_ANALOG_ADE11 = 0;\
                                        M0P_GPIO->FN7_f.P71 = 1;\
                                        M0P_GPIO->FN_SEL01_f.PWM01E = 1;\
                                    }while(0)

#define Gpio_SetFunc_PWM02_0(x)   do{\
                                        bM0P_GPIO_ANALOG_ADE12 = 0;\
                                        M0P_GPIO->FN7_f.P72 = 1;\
                                        M0P_GPIO->FN_SEL01_f.PWM02E = 1;\
                                    }while(0)

#define Gpio_SetFunc_PWM03_0(x)   do{\
                                        bM0P_GPIO_ANALOG_ADE13 = 0;\
                                        M0P_GPIO->FN7_f.P73 = 1;\
                                        M0P_GPIO->FN_SEL01_f.PWM03E = 1;\
                                    }while(0)

#define Gpio_SetFunc_PWM04_0(x)   do{\
                                        bM0P_GPIO_ANALOG_ADE14 = 0;\
                                        M0P_GPIO->FN7_f.P74 = 1;\
                                        M0P_GPIO->FN_SEL01_f.PWM04E = 1;\
                                    }while(0)

#define Gpio_SetFunc_PWM05_0(x)   do{\
                                        bM0P_GPIO_ANALOG_ADE15 = 0;\
                                        M0P_GPIO->FN7_f.P75 = 1;\
                                        M0P_GPIO->FN_SEL01_f.PWM05E = 1;\
                                    }while(0)

#define Gpio_SetFunc_EMII_EX(x)    do{\
                                        M0P_GPIO->FN0_f.P07 = 1;\
                                        M0P_GPIO->FN_SEL01_f.EMII0S = 0;\
                                    }while(0)

#define Gpio_SetFunc_EMII_VC(x)    do{\
                                        M0P_GPIO->FN0_f.P07 = 1;\
                                        M0P_GPIO->FN_SEL01_f.EMII0S = 1;\
                                    }while(0)

#define Gpio_SetFunc_EXCK0_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE08 = 0;\
                                        M0P_GPIO->FN6_f.P60 = 1;\
                                        M0P_GPIO->FN_SEL01_f.EXCK0S = 0;\
                                    }while(0)

#define Gpio_SetFunc_EXCK0_1(x)     do{\
                                        M0P_GPIO->FN6_f.P65 = 1;\
                                        M0P_GPIO->FN_SEL01_f.EXCK0S = 1;\
                                    }while(0)


/* FN_SEL02 */
#define Gpio_SetFunc_TIOA0_0_OUT(x) do{\
                                        bM0P_GPIO_ANALOG_ADE03 = 0;\
                                        bM0P_GPIO_ANALOG_ADE21 = 0;\
                                        M0P_GPIO->FN4_f.P46 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOA0E = 1;\
                                    }while(0)

#define Gpio_SetFunc_TIOA0_1_OUT(x) do{\
                                        bM0P_GPIO_ANALOG_ADE05 = 0;\
                                        M0P_GPIO->FN5_f.P50 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOA0E = 2;\
                                    }while(0)

#define Gpio_SetFunc_TIOA0_2_OUT(x) do{\
                                        M0P_GPIO->FN6_f.P63 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOA0E = 3;\
                                    }while(0)

#define Gpio_SetFunc_TIOB0_0_IN(x)  do{\
                                        bM0P_GPIO_ANALOG_ADE02 = 0;\
                                        bM0P_GPIO_ANALOG_ADE20 = 0;\
                                        M0P_GPIO->FN4_f.P45 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOB0S = 1;\
                                    }while(0)

#define Gpio_SetFunc_TIOB0_1_IN(x)  do{\
                                        bM0P_GPIO_ANALOG_ADE04 = 0;\
                                        M0P_GPIO->FN4_f.P47 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOB0S = 2;\
                                    }while(0)

#define Gpio_SetFunc_TIOB0_2_IN(x)  do{\
                                        M0P_GPIO->FN6_f.P62 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOB0S = 3;\
                                    }while(0)

#define Gpio_SetFunc_VC2TM0(x)      do{\
                                        M0P_GPIO->FN_SEL02_f.TIOB0S = 4;\
                                    }while(0)

#define Gpio_SetFunc_SUBOUT(x)      do{\
                                        M0P_GPIO->FN_SEL02_f.TIOB0S = 6;\
                                    }while(0)

#define Gpio_SetFunc_TRIMMING(x)    do{\
                                        M0P_GPIO->FN_SEL02_f.TIOB0S = 7;\
                                    }while(0)

#define Gpio_SetFunc_TIOA1_0_IN(x)  do{\
                                        bM0P_GPIO_ANALOG_ADE00 = 0;\
                                        M0P_GPIO->FN4_f.P43 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOA1S = 1;\
                                    }while(0)

#define Gpio_SetFunc_TIOA1_1_IN(x)  do{\
                                        bM0P_GPIO_ANALOG_ADE07 = 0;\
                                        M0P_GPIO->FN5_f.P52 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOA1S = 2;\
                                    }while(0)

#define Gpio_SetFunc_TIOA1_0_OUT(x) do{\
                                        bM0P_GPIO_ANALOG_ADE00 = 0;\
                                        M0P_GPIO->FN4_f.P43 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOA1E = 1;\
                                    }while(0)

#define Gpio_SetFunc_TIOA1_1_OUT(x) do{\
                                        bM0P_GPIO_ANALOG_ADE07 = 0;\
                                        M0P_GPIO->FN5_f.P52 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOA1E = 2;\
                                    }while(0)

#define Gpio_SetFunc_TIOB1_0_IN(x)  do{\
                                        bM0P_GPIO_ANALOG_ADE18 = 0;\
                                        M0P_GPIO->FN4_f.P42 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOB1S = 1;\
                                    }while(0)

#define Gpio_SetFunc_TIOB1_1_IN(x)  do{\
                                        bM0P_GPIO_ANALOG_ADE06 = 0;\
                                        M0P_GPIO->FN5_f.P51 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOB1S = 2;\
                                    }while(0)

#define Gpio_SetFunc_TIOB1_2_IN(x)  do{\
                                        bM0P_GPIO_ANALOG_ADE08 = 0;\
                                        M0P_GPIO->FN6_f.P60 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOB1S = 3;\
                                    }while(0)

#define Gpio_SetFunc_TIOA2_0_OUT(x) do{\
                                        bM0P_GPIO_ANALOG_ADE17 = 0;\
                                        M0P_GPIO->FN4_f.P41 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOA2E = 1;\
                                    }while(0)

#define Gpio_SetFunc_TIOA2_1_OUT(x) do{\
                                        M0P_GPIO->FN2_f.P24 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOA2E = 2;\
                                    }while(0)

#define Gpio_SetFunc_TIOA2_2_OUT(x) do{\
                                        M0P_GPIO->FN3_f.P32 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOA2E = 3;\
                                    }while(0)

#define Gpio_SetFunc_TIOB2_0_IN(x)  do{\
                                        bM0P_GPIO_ANALOG_ADE16 = 0;\
                                        M0P_GPIO->FN4_f.P40 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOB2S = 1;\
                                    }while(0)

#define Gpio_SetFunc_TIOB2_1_IN(x)  do{\
                                        M0P_GPIO->FN2_f.P23 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOB2S = 2;\
                                    }while(0)

#define Gpio_SetFunc_TIOB2_2_IN(x)  do{\
                                        bM0P_GPIO_ANALOG_ADE09 = 0;\
                                        M0P_GPIO->FN6_f.P61 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOB2S = 3;\
                                    }while(0)

#define Gpio_SetFunc_VC2TM2(x)      do{\
                                        M0P_GPIO->FN_SEL02_f.TIOB2S = 4;\
                                    }while(0)

#define Gpio_SetFunc_TIOA3_0_IN(x)  do{\
                                        M0P_GPIO->FN3_f.P31 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOA3S = 1;\
                                    }while(0)

#define Gpio_SetFunc_TIOA3_1_IN(x)  do{\
                                        M0P_GPIO->FN2_f.P21 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOA3S = 2;\
                                    }while(0)

#define Gpio_SetFunc_TIOA3_0_OUT(x) do{\
                                        M0P_GPIO->FN3_f.P31 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOA3E = 1;\
                                    }while(0)

#define Gpio_SetFunc_TIOA3_1_OUT(x) do{\
                                        M0P_GPIO->FN2_f.P21 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOA3E = 2;\
                                    }while(0)

#define Gpio_SetFunc_TIOB3_0_IN(x)  do{\
                                        M0P_GPIO->FN3_f.P30 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOB3S = 1;\
                                    }while(0)

#define Gpio_SetFunc_TIOB3_1_IN(x)  do{\
                                        M0P_GPIO->FN2_f.P22 = 1;\
                                        M0P_GPIO->FN_SEL02_f.TIOB3S = 2;\
                                    }while(0)

/* FN_SEL04 */
#define Gpio_SetFunc_INT00_0(x)     do{\
                                        M0P_GPIO->FN1_f.P10 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT00S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT00_1(x)     do{\
                                        M0P_GPIO->FN0_f.P03 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT00S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT00_2(x)     do{\
                                        M0P_GPIO->FN0_f.P00 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT00S = 3;\
                                    }while(0)


#define Gpio_SetFunc_INT01_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE22 = 0;\
                                        M0P_GPIO->FN1_f.P11 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT01S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT01_1(x)     do{\
                                        M0P_GPIO->FN0_f.P04 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT01S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT01_2(x)     do{\
                                        M0P_GPIO->FN0_f.P01 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT01S = 3;\
                                    }while(0)

#define Gpio_SetFunc_INT02_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE23 = 0;\
                                        M0P_GPIO->FN1_f.P12 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT02S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT02_1(x)     do{\
                                        M0P_GPIO->FN0_f.P05 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT02S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT02_2(x)     do{\
                                        M0P_GPIO->FN0_f.P02 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT02S = 3;\
                                    }while(0)

#define Gpio_SetFunc_INT03_0(x)     do{\
                                        M0P_GPIO->FN3_f.P32 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT03S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT03_1(x)     do{\
                                        M0P_GPIO->FN0_f.P06 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT03S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT03_2(x)     do{\
                                        M0P_GPIO->FN2_f.P25 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT03S = 3;\
                                    }while(0)

#define Gpio_SetFunc_INT04_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE00 = 0;\
                                        M0P_GPIO->FN4_f.P43 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT04S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT04_1(x)     do{\
                                        M0P_GPIO->FN2_f.P21 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT04S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT04_2(x)     do{\
                                        M0P_GPIO->FN2_f.P26 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT04S = 3;\
                                    }while(0)

#define Gpio_SetFunc_INT05_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE01 = 0;\
                                        bM0P_GPIO_ANALOG_ADE19 = 0;\
                                        M0P_GPIO->FN4_f.P44 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT05S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT05_1(x)     do{\
                                        M0P_GPIO->FN2_f.P22 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT05S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT05_2(x)     do{\
                                        M0P_GPIO->FN2_f.P27 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT05S = 3;\
                                    }while(0)

#define Gpio_SetFunc_INT06_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE02 = 0;\
                                        bM0P_GPIO_ANALOG_ADE20 = 0;\
                                        M0P_GPIO->FN4_f.P45 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT06S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT06_1(x)     do{\
                                        M0P_GPIO->FN2_f.P23 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT06S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT06_2(x)     do{\
                                        M0P_GPIO->FN5_f.P53 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT06S = 3;\
                                    }while(0)

#define Gpio_SetFunc_INT07_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE03 = 0;\
                                        bM0P_GPIO_ANALOG_ADE21 = 0;\
                                        M0P_GPIO->FN4_f.P46 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT07S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT07_1(x)     do{\
                                        M0P_GPIO->FN2_f.P24 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT07S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT07_2(x)     do{\
                                        M0P_GPIO->FN5_f.P54 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT07S = 3;\
                                    }while(0)

#define Gpio_SetFunc_INT08_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE08 = 0;\
                                        M0P_GPIO->FN6_f.P60 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT08S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT08_1(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE04 = 0;\
                                        M0P_GPIO->FN4_f.P47 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT08S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT08_2(x)     do{\
                                        M0P_GPIO->FN5_f.P55 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT08S = 3;\
                                    }while(0)

#define Gpio_SetFunc_INT09_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE09 = 0;\
                                        M0P_GPIO->FN6_f.P61 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT09S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT09_1(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE05 = 0;\
                                        M0P_GPIO->FN5_f.P50 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT09S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT09_2(x)     do{\
                                        M0P_GPIO->FN6_f.P66 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT09S = 3;\
                                    }while(0)

#define Gpio_SetFunc_INT10_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE10 = 0;\
                                        M0P_GPIO->FN7_f.P70 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT10S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT10_1(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE06 = 0;\
                                        M0P_GPIO->FN5_f.P51 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT10S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT10_2(x)     do{\
                                        M0P_GPIO->FN6_f.P67 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT10S = 3;\
                                    }while(0)

#define Gpio_SetFunc_INT11_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE11 = 0;\
                                        M0P_GPIO->FN7_f.P71 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT11S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT11_1(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE07 = 0;\
                                        M0P_GPIO->FN5_f.P52 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT11S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT11_2(x)     do{\
                                        M0P_GPIO->FN7_f.P76 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT11S = 3;\
                                    }while(0)


#define Gpio_SetFunc_INT12_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE12 = 0;\
                                        M0P_GPIO->FN7_f.P72 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT12S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT12_1(x)     do{\
                                        M0P_GPIO->FN6_f.P62 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT12S = 3;\
                                    }while(0)

#define Gpio_SetFunc_INT13_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE13 = 0;\
                                        M0P_GPIO->FN7_f.P73 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT13S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT13_1(x)     do{\
                                        M0P_GPIO->FN6_f.P63 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT13S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT14_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE14 = 0;\
                                        M0P_GPIO->FN7_f.P74 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT14S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT14_1(x)     do{\
                                        M0P_GPIO->FN6_f.P64 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT14S = 2;\
                                    }while(0)

#define Gpio_SetFunc_INT15_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE15 = 0;\
                                        M0P_GPIO->FN7_f.P75 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT15S = 1;\
                                    }while(0)

#define Gpio_SetFunc_INT15_1(x)     do{\
                                        M0P_GPIO->FN6_f.P65 = 1;\
                                        M0P_GPIO->FN_SEL04_f.EINT15S = 2;\
                                    }while(0)

/* FN_SEL07 */
#define Gpio_SetFunc_RTS4_0(x)      do{\
                                        bM0P_GPIO_ANALOG_ADE17 = 0;\
                                        M0P_GPIO->FN4_f.P41 = 1;\
                                        M0P_GPIO->FN_SEL07_f.RTS4E = 1;\
                                    }while(0)

#define Gpio_SetFunc_RTS4_1(x)      do{\
                                        M0P_GPIO->FN2_f.P25 = 1;\
                                        M0P_GPIO->FN_SEL07_f.RTS4E = 2;\
                                    }while(0)

#define Gpio_SetFunc_CTS4_0(x)      do{\
                                        bM0P_GPIO_ANALOG_ADE16 = 0;\
                                        M0P_GPIO->FN4_f.P40 = 1;\
                                        M0P_GPIO->FN_SEL07_f.CTS4S = 1;\
                                    }while(0)

#define Gpio_SetFunc_CTS4_1(x)      do{\
                                        M0P_GPIO->FN2_f.P24 = 1;\
                                        M0P_GPIO->FN_SEL07_f.CTS4S = 2;\
                                    }while(0)

#define Gpio_SetFunc_SIN4_0(x)      do{\
                                        M0P_GPIO->FN3_f.P30 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SIN4S = 1;\
                                    }while(0)

#define Gpio_SetFunc_SIN4_1(x)      do{\
                                        M0P_GPIO->FN2_f.P23 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SIN4S = 2;\
                                    }while(0)

#define Gpio_SetFunc_SOT4_0(x)      do{\
                                        M0P_GPIO->FN3_f.P31 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SOT4B = 1;\
                                    }while(0)

#define Gpio_SetFunc_SOT4_1(x)      do{\
                                        M0P_GPIO->FN2_f.P22 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SOT4B = 2;\
                                    }while(0)

#define Gpio_SetFunc_SCK4_0(x)      do{\
                                        M0P_GPIO->FN3_f.P32 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SCK4B = 1;\
                                    }while(0)

#define Gpio_SetFunc_SCK4_1(x)      do{\
                                        M0P_GPIO->FN2_f.P21 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SCK4B = 2;\
                                    }while(0)

#define Gpio_SetFunc_SCS40_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE00 = 0;\
                                        M0P_GPIO->FN4_f.P43 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SCS40B = 1;\
                                    }while(0)

#define Gpio_SetFunc_SCS40_1(x)     do{\
                                        M0P_GPIO->FN2_f.P27 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SCS40B = 2;\
                                    }while(0)

#define Gpio_SetFunc_SCS41_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE18 = 0;\
                                        M0P_GPIO->FN4_f.P42 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SCS41E = 1;\
                                    }while(0)

#define Gpio_SetFunc_SCS41_1(x)     do{\
                                        M0P_GPIO->FN2_f.P26 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SCS41E = 2;\
                                    }while(0)

#define Gpio_SetFunc_SIN5_0(x)      do{\
                                        bM0P_GPIO_ANALOG_ADE23 = 0;\
                                        M0P_GPIO->FN1_f.P12 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SIN5S = 1;\
                                    }while(0)

#define Gpio_SetFunc_SIN5_1(x)      do{\
                                        M0P_GPIO->FN6_f.P65 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SIN5S = 2;\
                                    }while(0)

#define Gpio_SetFunc_SIN5_2(x)      do{\
                                        M0P_GPIO->FN1_f.P14 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SIN5S = 3;\
                                    }while(0)

#define Gpio_SetFunc_SOT5_0(x)      do{\
                                        bM0P_GPIO_ANALOG_ADE22 = 0;\
                                        M0P_GPIO->FN1_f.P11 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SOT5B = 1;\
                                    }while(0)

#define Gpio_SetFunc_SOT5_1(x)      do{\
                                        M0P_GPIO->FN6_f.P64 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SOT5B = 2;\
                                    }while(0)

#define Gpio_SetFunc_SOT5_2(x)      do{\
                                        M0P_GPIO->FN1_f.P13 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SOT5B = 3;\
                                    }while(0)

#define Gpio_SetFunc_SCK5_0(x)      do{\
                                        M0P_GPIO->FN1_f.P10 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SCK5B = 1;\
                                    }while(0)

#define Gpio_SetFunc_SCK5_1(x)      do{\
                                        M0P_GPIO->FN6_f.P63 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SCK5B = 2;\
                                    }while(0)

#define Gpio_SetFunc_SCS50_0(x)     do{\
                                        M0P_GPIO->FN0_f.P07 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SCS50B = 1;\
                                    }while(0)

#define Gpio_SetFunc_SCS50_1(x)     do{\
                                        M0P_GPIO->FN6_f.P62 = 1;\
                                        M0P_GPIO->FN_SEL07_f.SCS50B = 2;\
                                    }while(0)


/* FN_SEL08 */
#define Gpio_SetFunc_RTS6_0(x)      do{\
                                        bM0P_GPIO_ANALOG_ADE09 = 0;\
                                        M0P_GPIO->FN6_f.P61 = 1;\
                                        M0P_GPIO->FN_SEL08_f.RTS6E = 1;\
                                    }while(0)

#define Gpio_SetFunc_RTS6_1(x)      do{\
                                        M0P_GPIO->FN0_f.P00 = 1;\
                                        M0P_GPIO->FN_SEL08_f.RTS6E = 2;\
                                    }while(0)

#define Gpio_SetFunc_CTS6_0(x)      do{\
                                        bM0P_GPIO_ANALOG_ADE08 = 0;\
                                        M0P_GPIO->FN6_f.P60 = 1;\
                                        M0P_GPIO->FN_SEL08_f.CTS6S = 1;\
                                    }while(0)

#define Gpio_SetFunc_CTS6_1(x)      do{\
                                        M0P_GPIO->FN7_f.P76 = 1;\
                                        M0P_GPIO->FN_SEL08_f.CTS6S = 2;\
                                    }while(0)

#define Gpio_SetFunc_SIN6_0(x)      do{\
                                        bM0P_GPIO_ANALOG_ADE12 = 0;\
                                        M0P_GPIO->FN7_f.P72 = 1;\
                                        M0P_GPIO->FN_SEL08_f.SIN6S = 1;\
                                    }while(0)

#define Gpio_SetFunc_SIN6_1(x)      do{\
                                        M0P_GPIO->FN0_f.P06 = 1;\
                                        M0P_GPIO->FN_SEL08_f.SIN6S = 2;\
                                    }while(0)

#define Gpio_SetFunc_SOT6_0(x)      do{\
                                        bM0P_GPIO_ANALOG_ADE11 = 0;\
                                        M0P_GPIO->FN7_f.P71 = 1;\
                                        M0P_GPIO->FN_SEL08_f.SOT6B = 1;\
                                    }while(0)

#define Gpio_SetFunc_SOT6_1(x)      do{\
                                        M0P_GPIO->FN0_f.P05 = 1;\
                                        M0P_GPIO->FN_SEL08_f.SOT6B = 2;\
                                    }while(0)

#define Gpio_SetFunc_SCK6_0(x)      do{\
                                        bM0P_GPIO_ANALOG_ADE10 = 0;\
                                        M0P_GPIO->FN7_f.P70 = 1;\
                                        M0P_GPIO->FN_SEL08_f.SCK6B = 1;\
                                    }while(0)

#define Gpio_SetFunc_SCK6_1(x)      do{\
                                        M0P_GPIO->FN0_f.P04 = 1;\
                                        M0P_GPIO->FN_SEL08_f.SCK6B = 2;\
                                    }while(0)

#define Gpio_SetFunc_SCS60_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE15 = 0;\
                                        M0P_GPIO->FN7_f.P75 = 1;\
                                        M0P_GPIO->FN_SEL08_f.SCS60B = 1;\
                                    }while(0)

#define Gpio_SetFunc_SCS60_1(x)     do{\
                                        M0P_GPIO->FN0_f.P03 = 1;\
                                        M0P_GPIO->FN_SEL08_f.SCS60B = 2;\
                                    }while(0)

#define Gpio_SetFunc_SCS61_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE14 = 0;\
                                        M0P_GPIO->FN7_f.P74 = 1;\
                                        M0P_GPIO->FN_SEL08_f.SCS61E = 1;\
                                    }while(0)

#define Gpio_SetFunc_SCS61_1(x)     do{\
                                        M0P_GPIO->FN0_f.P02 = 1;\
                                        M0P_GPIO->FN_SEL08_f.SCS61E = 2;\
                                    }while(0)

#define Gpio_SetFunc_SCS62_0(x)     do{\
                                        bM0P_GPIO_ANALOG_ADE13 = 0;\
                                        M0P_GPIO->FN7_f.P73 = 1;\
                                        M0P_GPIO->FN_SEL08_f.SCS62E = 1;\
                                    }while(0)

#define Gpio_SetFunc_SCS62_1(x)     do{\
                                        M0P_GPIO->FN0_f.P01 = 1;\
                                        M0P_GPIO->FN_SEL08_f.SCS62E = 2;\
                                    }while(0)

/* FN_SEL09 */
#define Gpio_SetFunc_CAOUT(x)       do{\
                                        M0P_GPIO->FN1_f.P10 = 1;\
                                        M0P_GPIO->FN_SEL09_f.VCOE = x;\
                                    }while(0)

#define Gpio_SetFunc_ADTG_0(x)      do{\
                                        bM0P_GPIO_ANALOG_ADE16 = 0;\
                                        M0P_GPIO->FN4_f.P40 = 1;\
                                        M0P_GPIO->FN_SEL09_f.ADTGS = 0;\
                                    }while(0)

#define Gpio_SetFunc_ADTG_1(x)      do{\
                                        M0P_GPIO->FN0_f.P07 = 1;\
                                        M0P_GPIO->FN_SEL09_f.ADTGS = 1;\
                                    }while(0)


/* SPS */
#define Gpio_SetFunc_XHIO(x)        do{\
                                        M0P_GPIO->SPS_f.MAINCS = 1;\
                                    }while(0)

#define Gpio_SetFunc_XLIO(x)        do{\
                                        M0P_GPIO->SPS_f.SUBCS = 1;\
                                    }while(0)



/* Fast GPIO BitMap (FGET, FSET) */
#define MOP_FGPIO_FGET0             (0xF8010000)
#define MOP_FGPIO_FGET1             (0xF8010400)
#define MOP_FGPIO_FGET2             (0xF8010800)
#define MOP_FGPIO_FGET3             (0xF8010C00)
#define MOP_FGPIO_FGET4             (0xF8011000)
#define MOP_FGPIO_FGET5             (0xF8011400)
#define MOP_FGPIO_FGET6             (0xF8011800)
#define MOP_FGPIO_FGET7             (0xF8011C00)

#define MOP_FGPIO_FSET0             (0xF8014000)
#define MOP_FGPIO_FSET1             (0xF8014400)
#define MOP_FGPIO_FSET2             (0xF8014800)
#define MOP_FGPIO_FSET3             (0xF8014C00)
#define MOP_FGPIO_FSET4             (0xF8015000)
#define MOP_FGPIO_FSET5             (0xF8015400)
#define MOP_FGPIO_FSET6             (0xF8015800)
#define MOP_FGPIO_FSET7             (0xF8015C00)


#define bM0P_FASTIO_FGET0_P00       (*(volatile uint8_t *)(MOP_FGPIO_FGET0 + 0x4))
#define bM0P_FASTIO_FGET0_P01       (*(volatile uint8_t *)(MOP_FGPIO_FGET0 + 0x8))
#define bM0P_FASTIO_FGET0_P02       (*(volatile uint8_t *)(MOP_FGPIO_FGET0 + 0x10))
#define bM0P_FASTIO_FGET0_P03       (*(volatile uint8_t *)(MOP_FGPIO_FGET0 + 0x20))
#define bM0P_FASTIO_FGET0_P04       (*(volatile uint8_t *)(MOP_FGPIO_FGET0 + 0x40))
#define bM0P_FASTIO_FGET0_P05       (*(volatile uint8_t *)(MOP_FGPIO_FGET0 + 0x80))
#define bM0P_FASTIO_FGET0_P06       (*(volatile uint8_t *)(MOP_FGPIO_FGET0 + 0x100))
#define bM0P_FASTIO_FGET0_P07       (*(volatile uint8_t *)(MOP_FGPIO_FGET0 + 0x200))

#define bM0P_FASTIO_FGET1_P10       (*(volatile uint8_t *)(MOP_FGPIO_FGET1 + 0x4))
#define bM0P_FASTIO_FGET1_P11       (*(volatile uint8_t *)(MOP_FGPIO_FGET1 + 0x8))
#define bM0P_FASTIO_FGET1_P12       (*(volatile uint8_t *)(MOP_FGPIO_FGET1 + 0x10))
#define bM0P_FASTIO_FGET1_P13       (*(volatile uint8_t *)(MOP_FGPIO_FGET1 + 0x20))
#define bM0P_FASTIO_FGET1_P14       (*(volatile uint8_t *)(MOP_FGPIO_FGET1 + 0x40))
#define bM0P_FASTIO_FGET1_P15       (*(volatile uint8_t *)(MOP_FGPIO_FGET1 + 0x80))
#define bM0P_FASTIO_FGET1_P16       (*(volatile uint8_t *)(MOP_FGPIO_FGET1 + 0x100))

#define bM0P_FASTIO_FGET2_P20       (*(volatile uint8_t *)(MOP_FGPIO_FGET2 + 0x4))
#define bM0P_FASTIO_FGET2_P21       (*(volatile uint8_t *)(MOP_FGPIO_FGET2 + 0x8))
#define bM0P_FASTIO_FGET2_P22       (*(volatile uint8_t *)(MOP_FGPIO_FGET2 + 0x10))
#define bM0P_FASTIO_FGET2_P23       (*(volatile uint8_t *)(MOP_FGPIO_FGET2 + 0x20))
#define bM0P_FASTIO_FGET2_P24       (*(volatile uint8_t *)(MOP_FGPIO_FGET2 + 0x40))
#define bM0P_FASTIO_FGET2_P25       (*(volatile uint8_t *)(MOP_FGPIO_FGET2 + 0x80))
#define bM0P_FASTIO_FGET2_P26       (*(volatile uint8_t *)(MOP_FGPIO_FGET2 + 0x100))
#define bM0P_FASTIO_FGET2_P27       (*(volatile uint8_t *)(MOP_FGPIO_FGET2 + 0x200))

#define bM0P_FASTIO_FGET3_P30       (*(volatile uint8_t *)(MOP_FGPIO_FGET3 + 0x4))
#define bM0P_FASTIO_FGET3_P31       (*(volatile uint8_t *)(MOP_FGPIO_FGET3 + 0x8))
#define bM0P_FASTIO_FGET3_P32       (*(volatile uint8_t *)(MOP_FGPIO_FGET3 + 0x10))

#define bM0P_FASTIO_FGET4_P40       (*(volatile uint8_t *)(MOP_FGPIO_FGET4 + 0x4))
#define bM0P_FASTIO_FGET4_P41       (*(volatile uint8_t *)(MOP_FGPIO_FGET4 + 0x8))
#define bM0P_FASTIO_FGET4_P42       (*(volatile uint8_t *)(MOP_FGPIO_FGET4 + 0x10))
#define bM0P_FASTIO_FGET4_P43       (*(volatile uint8_t *)(MOP_FGPIO_FGET4 + 0x20))
#define bM0P_FASTIO_FGET4_P44       (*(volatile uint8_t *)(MOP_FGPIO_FGET4 + 0x40))
#define bM0P_FASTIO_FGET4_P45       (*(volatile uint8_t *)(MOP_FGPIO_FGET4 + 0x80))
#define bM0P_FASTIO_FGET4_P46       (*(volatile uint8_t *)(MOP_FGPIO_FGET4 + 0x100))
#define bM0P_FASTIO_FGET4_P47       (*(volatile uint8_t *)(MOP_FGPIO_FGET4 + 0x200))

#define bM0P_FASTIO_FGET5_P50       (*(volatile uint8_t *)(MOP_FGPIO_FGET5 + 0x4))
#define bM0P_FASTIO_FGET5_P51       (*(volatile uint8_t *)(MOP_FGPIO_FGET5 + 0x8))
#define bM0P_FASTIO_FGET5_P52       (*(volatile uint8_t *)(MOP_FGPIO_FGET5 + 0x10))
#define bM0P_FASTIO_FGET5_P53       (*(volatile uint8_t *)(MOP_FGPIO_FGET5 + 0x20))
#define bM0P_FASTIO_FGET5_P54       (*(volatile uint8_t *)(MOP_FGPIO_FGET5 + 0x40))
#define bM0P_FASTIO_FGET5_P55       (*(volatile uint8_t *)(MOP_FGPIO_FGET5 + 0x80))

#define bM0P_FASTIO_FGET6_P60       (*(volatile uint8_t *)(MOP_FGPIO_FGET6 + 0x4))
#define bM0P_FASTIO_FGET6_P61       (*(volatile uint8_t *)(MOP_FGPIO_FGET6 + 0x8))
#define bM0P_FASTIO_FGET6_P62       (*(volatile uint8_t *)(MOP_FGPIO_FGET6 + 0x10))
#define bM0P_FASTIO_FGET6_P63       (*(volatile uint8_t *)(MOP_FGPIO_FGET6 + 0x20))
#define bM0P_FASTIO_FGET6_P64       (*(volatile uint8_t *)(MOP_FGPIO_FGET6 + 0x40))
#define bM0P_FASTIO_FGET6_P65       (*(volatile uint8_t *)(MOP_FGPIO_FGET6 + 0x80))
#define bM0P_FASTIO_FGET6_P66       (*(volatile uint8_t *)(MOP_FGPIO_FGET6 + 0x100))
#define bM0P_FASTIO_FGET6_P67       (*(volatile uint8_t *)(MOP_FGPIO_FGET6 + 0x200))

#define bM0P_FASTIO_FGET7_P70       (*(volatile uint8_t *)(MOP_FGPIO_FGET7 + 0x4))
#define bM0P_FASTIO_FGET7_P71       (*(volatile uint8_t *)(MOP_FGPIO_FGET7 + 0x8))
#define bM0P_FASTIO_FGET7_P72       (*(volatile uint8_t *)(MOP_FGPIO_FGET7 + 0x10))
#define bM0P_FASTIO_FGET7_P73       (*(volatile uint8_t *)(MOP_FGPIO_FGET7 + 0x20))
#define bM0P_FASTIO_FGET7_P74       (*(volatile uint8_t *)(MOP_FGPIO_FGET7 + 0x40))
#define bM0P_FASTIO_FGET7_P75       (*(volatile uint8_t *)(MOP_FGPIO_FGET7 + 0x80))
#define bM0P_FASTIO_FGET7_P76       (*(volatile uint8_t *)(MOP_FGPIO_FGET7 + 0x100))

#define bM0P_FASTIO_FSET0_P00       (*(volatile uint8_t *)(MOP_FGPIO_FSET0 + 0x4))
#define bM0P_FASTIO_FSET0_P01       (*(volatile uint8_t *)(MOP_FGPIO_FSET0 + 0x8))
#define bM0P_FASTIO_FSET0_P02       (*(volatile uint8_t *)(MOP_FGPIO_FSET0 + 0x10))
#define bM0P_FASTIO_FSET0_P03       (*(volatile uint8_t *)(MOP_FGPIO_FSET0 + 0x20))
#define bM0P_FASTIO_FSET0_P04       (*(volatile uint8_t *)(MOP_FGPIO_FSET0 + 0x40))
#define bM0P_FASTIO_FSET0_P05       (*(volatile uint8_t *)(MOP_FGPIO_FSET0 + 0x80))
#define bM0P_FASTIO_FSET0_P06       (*(volatile uint8_t *)(MOP_FGPIO_FSET0 + 0x100))
#define bM0P_FASTIO_FSET0_P07       (*(volatile uint8_t *)(MOP_FGPIO_FSET0 + 0x200))

#define bM0P_FASTIO_FSET1_P10       (*(volatile uint8_t *)(MOP_FGPIO_FSET1 + 0x4))
#define bM0P_FASTIO_FSET1_P11       (*(volatile uint8_t *)(MOP_FGPIO_FSET1 + 0x8))
#define bM0P_FASTIO_FSET1_P12       (*(volatile uint8_t *)(MOP_FGPIO_FSET1 + 0x10))
#define bM0P_FASTIO_FSET1_P13       (*(volatile uint8_t *)(MOP_FGPIO_FSET1 + 0x20))
#define bM0P_FASTIO_FSET1_P14       (*(volatile uint8_t *)(MOP_FGPIO_FSET1 + 0x40))
#define bM0P_FASTIO_FSET1_P15       (*(volatile uint8_t *)(MOP_FGPIO_FSET1 + 0x80))
#define bM0P_FASTIO_FSET1_P16       (*(volatile uint8_t *)(MOP_FGPIO_FSET1 + 0x100))

#define bM0P_FASTIO_FSET2_P20       (*(volatile uint8_t *)(MOP_FGPIO_FSET2 + 0x4))
#define bM0P_FASTIO_FSET2_P21       (*(volatile uint8_t *)(MOP_FGPIO_FSET2 + 0x8))
#define bM0P_FASTIO_FSET2_P22       (*(volatile uint8_t *)(MOP_FGPIO_FSET2 + 0x10))
#define bM0P_FASTIO_FSET2_P23       (*(volatile uint8_t *)(MOP_FGPIO_FSET2 + 0x20))
#define bM0P_FASTIO_FSET2_P24       (*(volatile uint8_t *)(MOP_FGPIO_FSET2 + 0x40))
#define bM0P_FASTIO_FSET2_P25       (*(volatile uint8_t *)(MOP_FGPIO_FSET2 + 0x80))
#define bM0P_FASTIO_FSET2_P26       (*(volatile uint8_t *)(MOP_FGPIO_FSET2 + 0x100))
#define bM0P_FASTIO_FSET2_P27       (*(volatile uint8_t *)(MOP_FGPIO_FSET2 + 0x200))

#define bM0P_FASTIO_FSET3_P30       (*(volatile uint8_t *)(MOP_FGPIO_FSET3 + 0x4))
#define bM0P_FASTIO_FSET3_P31       (*(volatile uint8_t *)(MOP_FGPIO_FSET3 + 0x8))
#define bM0P_FASTIO_FSET3_P32       (*(volatile uint8_t *)(MOP_FGPIO_FSET3 + 0x10))

#define bM0P_FASTIO_FSET4_P40       (*(volatile uint8_t *)(MOP_FGPIO_FSET4 + 0x4))
#define bM0P_FASTIO_FSET4_P41       (*(volatile uint8_t *)(MOP_FGPIO_FSET4 + 0x8))
#define bM0P_FASTIO_FSET4_P42       (*(volatile uint8_t *)(MOP_FGPIO_FSET4 + 0x10))
#define bM0P_FASTIO_FSET4_P43       (*(volatile uint8_t *)(MOP_FGPIO_FSET4 + 0x20))
#define bM0P_FASTIO_FSET4_P44       (*(volatile uint8_t *)(MOP_FGPIO_FSET4 + 0x40))
#define bM0P_FASTIO_FSET4_P45       (*(volatile uint8_t *)(MOP_FGPIO_FSET4 + 0x80))
#define bM0P_FASTIO_FSET4_P46       (*(volatile uint8_t *)(MOP_FGPIO_FSET4 + 0x100))
#define bM0P_FASTIO_FSET4_P47       (*(volatile uint8_t *)(MOP_FGPIO_FSET4 + 0x200))

#define bM0P_FASTIO_FSET5_P50       (*(volatile uint8_t *)(MOP_FGPIO_FSET5 + 0x4))
#define bM0P_FASTIO_FSET5_P51       (*(volatile uint8_t *)(MOP_FGPIO_FSET5 + 0x8))
#define bM0P_FASTIO_FSET5_P52       (*(volatile uint8_t *)(MOP_FGPIO_FSET5 + 0x10))
#define bM0P_FASTIO_FSET5_P53       (*(volatile uint8_t *)(MOP_FGPIO_FSET5 + 0x20))
#define bM0P_FASTIO_FSET5_P54       (*(volatile uint8_t *)(MOP_FGPIO_FSET5 + 0x40))
#define bM0P_FASTIO_FSET5_P55       (*(volatile uint8_t *)(MOP_FGPIO_FSET5 + 0x80))

#define bM0P_FASTIO_FSET6_P60       (*(volatile uint8_t *)(MOP_FGPIO_FSET6 + 0x4))
#define bM0P_FASTIO_FSET6_P61       (*(volatile uint8_t *)(MOP_FGPIO_FSET6 + 0x8))
#define bM0P_FASTIO_FSET6_P62       (*(volatile uint8_t *)(MOP_FGPIO_FSET6 + 0x10))
#define bM0P_FASTIO_FSET6_P63       (*(volatile uint8_t *)(MOP_FGPIO_FSET6 + 0x20))
#define bM0P_FASTIO_FSET6_P64       (*(volatile uint8_t *)(MOP_FGPIO_FSET6 + 0x40))
#define bM0P_FASTIO_FSET6_P65       (*(volatile uint8_t *)(MOP_FGPIO_FSET6 + 0x80))
#define bM0P_FASTIO_FSET6_P66       (*(volatile uint8_t *)(MOP_FGPIO_FSET6 + 0x100))
#define bM0P_FASTIO_FSET6_P67       (*(volatile uint8_t *)(MOP_FGPIO_FSET6 + 0x200))

#define bM0P_FASTIO_FSET7_P70       (*(volatile uint8_t *)(MOP_FGPIO_FSET7 + 0x4))
#define bM0P_FASTIO_FSET7_P71       (*(volatile uint8_t *)(MOP_FGPIO_FSET7 + 0x8))
#define bM0P_FASTIO_FSET7_P72       (*(volatile uint8_t *)(MOP_FGPIO_FSET7 + 0x10))
#define bM0P_FASTIO_FSET7_P73       (*(volatile uint8_t *)(MOP_FGPIO_FSET7 + 0x20))
#define bM0P_FASTIO_FSET7_P74       (*(volatile uint8_t *)(MOP_FGPIO_FSET7 + 0x40))
#define bM0P_FASTIO_FSET7_P75       (*(volatile uint8_t *)(MOP_FGPIO_FSET7 + 0x80))
#define bM0P_FASTIO_FSET7_P76       (*(volatile uint8_t *)(MOP_FGPIO_FSET7 + 0x100))


/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
extern en_result_t Gpio_SetAnalog(uint8_t u8Port, uint8_t u8Pin, boolean_t bFlag);

extern en_result_t Gpio_InitIO(uint8_t u8Port, uint8_t u8Pin,
                            en_Gpio_dir_t enDir, boolean_t bPullup,
                            boolean_t bOdr);
extern void Gpio_SetIO(uint8_t u8Port, uint8_t u8Pin,boolean_t bVal);
extern boolean_t Gpio_GetIO(uint8_t u8Port, uint8_t u8Pin);

extern en_result_t FGpio_InitIO(uint8_t u8Port, uint8_t u8Pin, en_Gpio_dir_t enDir);
extern boolean_t FGpio_GetIO(uint8_t u8Port, uint8_t u8Pin);
extern void FGpio_SetIO(uint8_t u8Port, uint8_t u8Pin, boolean_t bVal);


//@} // GpioGroup

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H_ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
