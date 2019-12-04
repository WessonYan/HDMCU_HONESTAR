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
/** \file interrupts_hc32f_m14x.h
 **
 ** A detailed description is available at
 ** @link InterruptGroup Interrupt description @endlink
 **
 **   - 2017-05-15  1.0  Zhangxl First version for Device Driver Library of
 **     interrupt.
 **
 ******************************************************************************/
#ifndef __INTERRUPTS_HC32F_M14X_H___
#define __INTERRUPTS_HC32F_M14X_H___

/*******************************************************************************
 * Include files
 ******************************************************************************/

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup InterruptGroup Interrupt
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

#define DDL_IRQ_LEVEL_CSV           3u  ///< Default intterupt level of Csv

#define DDL_IRQ_LEVEL_SWDT          3u  ///< Default intterupt level of Swwdt

#define DDL_IRQ_LEVEL_LVD           3u  ///< Default intterupt level of Lvd

#define DDL_IRQ_LEVEL_TIM4PWMEMI    3u  ///< Default intterupt level of Timer4_PWM_EMI

#define DDL_IRQ_LEVEL_EXT07         3u  ///< Default intterupt level of External interrupt Ch.0~7

#define DDL_IRQ_LEVEL_EXT815        3u  ///< Default intterupt level of External interrupt Ch.8~15

#define DDL_IRQ_LEVEL_DTIM          3u  ///< Default intterupt level of Dual timer

#define DDL_IRQ_LEVEL_MSC4RX        3u  ///< Default intterupt level of Msc Ch.4 Rx

#define DDL_IRQ_LEVEL_MSC4TX        3u  ///< Default intterupt level of Msc Ch.4 Tx

#define DDL_IRQ_LEVEL_MSC5RX        3u  ///< Default intterupt level of Msc Ch.5 Rx

#define DDL_IRQ_LEVEL_MSC5TX        3u  ///< Default intterupt level of Msc Ch.5 Tx

#define DDL_IRQ_LEVEL_MSC6RX        3u  ///< Default intterupt level of Msc Ch.6 Rx

#define DDL_IRQ_LEVEL_MSC6TX        3u  ///< Default intterupt level of Msc Ch.6 Tx

#define DDL_IRQ_LEVEL_OSCCCRP     3u  ///< Default intterupt level of Osc, Cc, Parity

#define DDL_IRQ_LEVEL_ADC           3u  ///< Default intterupt level of Adc

#define DDL_IRQ_LEVEL_VC            3u  ///< Default intterupt level of Vc

#define DDL_IRQ_LEVEL_CT03          3u  ///< Default intterupt level of Ct Ch.0~3

#define DDL_IRQ_LEVEL_DMAC0         3u  ///< Default intterupt level of Dmac Ch.0

#define DDL_IRQ_LEVEL_DMAC1         3u  ///< Default intterupt level of Dmac Ch.1

#define DDL_IRQ_LEVEL_CNT           3u  ///< Default intterupt level of Timer4_Cnt

#define DDL_IRQ_LEVEL_OCO05         3u  ///< Default intterupt level of Timer4_Oco

#define DDL_IRQ_LEVEL_DEFAULT       3u  ///< Default intterupt level

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
__WEAKDEF void Nmi_IrqHandler(uint8_t u8Param);
__WEAKDEF void Hwwdt_IrqHandler(uint8_t u8Param);

__WEAKDEF void Dt_IrqHandler(uint8_t u8Param);
__WEAKDEF void Clk_CsvIrqHandler(uint8_t u8Param);
__WEAKDEF void Swwdt_IrqHandler(uint8_t u8Param);
__WEAKDEF void Lvd_IrqHandler(uint8_t u8Param);
__WEAKDEF void Exti_07IrqHandler(uint8_t u8Param);
__WEAKDEF void Exti_815IrqHandler(uint8_t u8Param);
__WEAKDEF void Uart_Rx_IrqHandler(uint8_t u8Param);
__WEAKDEF void Spi_Rx_IrqHandler(uint8_t u8Param);
__WEAKDEF void I2c_Rx_IrqHander(uint8_t u8Param);
__WEAKDEF void Uart_Tx_IrqHandler(uint8_t u8Param);
__WEAKDEF void Spi_Tx_IrqHandler(uint8_t u8Param);
__WEAKDEF void I2c_Tx_IrqHander(uint8_t u8Param);
__WEAKDEF void Spi_Stat_IrqHandler(uint8_t u8Param);
__WEAKDEF void I2c_Stat_IrqHander(uint8_t u8Param);

__WEAKDEF void Clk_HSXTRdy_IrqHandler(uint8_t u8Param);
__WEAKDEF void Clk_LSXTRdy_IrqHandler(uint8_t u8Param);
__WEAKDEF void Clk_PLLRdy_IrqHandler(uint8_t u8Param);
__WEAKDEF void Cc_IrqHandler(uint8_t u8Param);
__WEAKDEF void Rami_IrqHandler(uint8_t u8Param);
__WEAKDEF void Adc_PriorityConvIrqHandler(uint8_t u8Param);
__WEAKDEF void Adc_ScanConvIrqHandler(uint8_t u8Param);
__WEAKDEF void Adc_FifoOvfIrqHandler(uint8_t u8Param);
__WEAKDEF void Adc_ResultCmpIrqHandler(uint8_t u8Param);
__WEAKDEF void Adc_RangeCmpIrqHandler(uint8_t u8Param);
__WEAKDEF void Vc_IrqHandler(uint8_t u8Param);
__WEAKDEF void Flash_IrqHandler(uint8_t u8Param);
__WEAKDEF void Ct_Irq0Handler(uint8_t u8Param);
__WEAKDEF void Ct_Irq1Handler(uint8_t u8Param);
__WEAKDEF void Dmac_IrqHandler(uint8_t u8Param);
__WEAKDEF void Timer4_Cnt_PeakIrqHandler(uint8_t u8Param);
__WEAKDEF void Timer4_Cnt_ZeroIrqHandler(uint8_t u8Param);
__WEAKDEF void Timer4_Oco_IrqHandler(uint8_t u8Param);
__WEAKDEF void Timer4_Emi_IrqHandler(uint8_t u8Param);
__WEAKDEF void Timer4_Pwm_IrqHandler(uint8_t u8Param);

//@} // InterruptGroup

#ifdef __cplusplus
}
#endif

#endif /* __INTERRUPTS_HC32F_M14X_H___ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
