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
/*****************************************************************************/
/** \file interrupts_hc32f_m14x.c
 **
 ** A detailed description is available at
 ** @link InterruptGroup Interrupt description @endlink
 **
 **   - 2017-05-15  1.0  Zhangxl First version for Device Driver Library of
 **     interrupt.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32f_m14x.h"

/**
 *******************************************************************************
 ** \addtogroup InterruptGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void MSCRX_IRQHander2(uint8_t u8ch,uint8_t u8mode);
static void MSCTX_IRQHander2(uint8_t u8ch,uint8_t u8mode);
static void MSCSTAT_IRQHander2(uint8_t u8ch,uint8_t u8mode);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief MSC reception interrupt implemention, called by MSCxTX_IRQHanlder
 **
 ******************************************************************************/
static void MSCRX_IRQHander2(uint8_t u8ch,uint8_t u8mode)
{
    switch (u8mode)
    {
        case 0:
            Uart_Rx_IrqHandler(u8ch);
            break;
        case 2:
            Spi_Rx_IrqHandler(u8ch);
            break;
        case 4:
            I2c_Rx_IrqHander(u8ch);
            break;
        default:
            break;
    }
}

/**
 *******************************************************************************
 ** \brief MSC transmission interrupt implemention, called by MSCxTX_IRQHanlder
 **
 ******************************************************************************/
static void MSCTX_IRQHander2(uint8_t u8ch,uint8_t u8mode)
{
    switch (u8mode)
    {
        case 0:
            Uart_Tx_IrqHandler(u8ch);
            break;
        case 2:
            Spi_Tx_IrqHandler(u8ch);
            break;
        case 4:
            I2c_Tx_IrqHander(u8ch);
            break;
        default:
            break;
    }
}

/**
 *******************************************************************************
 ** \brief MSC status interrupt implemention, called by MSCxTX_IRQHanlder
 **
 ******************************************************************************/
static void MSCSTAT_IRQHander2(uint8_t u8ch,uint8_t u8mode)
{
    switch (u8mode)
    {
        case 2:
            Spi_Stat_IrqHandler(u8ch);
            break;
        case 4:
            I2c_Stat_IrqHander(u8ch);
            break;
        default:
            break;
    }
}

/**
 *******************************************************************************
 ** \brief NMI & Hardware watchdog timer IRQ handler
 **
 ******************************************************************************/
void NMI_Handler(void)
{
    uint8_t u8val = 0;
    u8val = M0P_IRQC->IRQ_ST02;

    if ( (u8val&1) > 0)
    {
        Nmi_IrqHandler(0);
    }
    if ((u8val&2)> 0)
    {
        Hwwdt_IrqHandler(0);
    }
}

/**
 *******************************************************************************
 ** \brief Clock Supervisor IRQ handler
 **
 ******************************************************************************/
void CSV_IRQHandler(void)
{
    Clk_CsvIrqHandler(0);
}

/**
 *******************************************************************************
 ** \brief Software watchdog timer IRQ handler
 **
 ******************************************************************************/
void SWDT_IRQHandler(void)
{
     Swwdt_IrqHandler(0);
}

/**
 *******************************************************************************
 ** \brief Low voltage detect IRQ handler
 **
 ******************************************************************************/
void LVD_IRQHandler(void)
{
     Lvd_IrqHandler(0);
}

/**
 *******************************************************************************
 ** \brief External interrupt Ch.0~7 IRQ handler
 **
 ******************************************************************************/
void EXTI07_IRQHandler(void)
{
    uint8_t u8val = 0;
    uint8_t i = 0;

    u8val = M0P_IRQC->IRQ04PEND;

    for (i = 0; i < 8; i++)
    {
        if (u8val&(1u<<i))
        {
            Exti_07IrqHandler(i);
        }
    }
}

/**
 *******************************************************************************
 ** \brief External interrupt Ch.8~15 IRQ handler
 **
 ******************************************************************************/
void EXTI815_IRQHandler(void)
{
    uint8_t u8val = 0;
    uint8_t i = 0;

    u8val = M0P_IRQC->IRQ05PEND;

    for (i = 0; i < 8; i++)
    {
        if (u8val&(1u<<i))
        {
            Exti_815IrqHandler(8+i);
        }
    }
}

/**
 *******************************************************************************
 ** \brief Dual timer IRQ handler
 **
 ******************************************************************************/
void DTIM_IRQHandler(void)
{
    uint32_t u32IrqMon = M0P_IRQC->IRQ06PEND;

    if (0u != (u32IrqMon & 0x00000001ul))
    {
        Dt_IrqHandler(1);
    }

    if (0u != (u32IrqMon & 0x00000002ul))
    {
        Dt_IrqHandler(2);
    }
}

/**
 *******************************************************************************
 ** \brief MSC Ch.4 Reception IRQ handler
 **
 ******************************************************************************/
void MSC4RX_IRQHandler(void)
{
    uint8_t u8Mode = 0;
    u8Mode = M0P_MSC4_SPI->MR_f.MODE;
    MSCRX_IRQHander2(4,u8Mode);
}

/**
 *******************************************************************************
 ** \brief MSC Ch.4 Transmission & Status IRQ handler
 **
 ******************************************************************************/
void MSC4TX_IRQHandler(void)
{
    uint8_t u8Mode = 0;
    uint32_t u32Val = 0;

    u8Mode = M0P_MSC4_SPI->MR_f.MODE;
    u32Val = M0P_IRQC->IRQ16PEND;

    if (0u != (u32Val & 0x2u))
    {
        MSCSTAT_IRQHander2(4,u8Mode);
    }
    if (0u != (u32Val & 0x1u))
    {
        MSCTX_IRQHander2(4,u8Mode);
    }
}

/**
 *******************************************************************************
 ** \brief MSC Ch.5 Reception IRQ handler
 **
 ******************************************************************************/
void MSC5RX_IRQHandler(void)
{
    uint8_t u8Mode = 0;
    u8Mode = M0P_MSC5_SPI->MR_f.MODE;
    MSCRX_IRQHander2(5,u8Mode);
}

/**
 *******************************************************************************
 ** \brief MSC Ch.5 Transmission & Status IRQ handler
 **
 ******************************************************************************/
void MSC5TX_IRQHandler(void)
{
    uint8_t u8Mode = 0;
    uint32_t u32Val = 0;

    u8Mode = M0P_MSC5_SPI->MR_f.MODE;
    u32Val = M0P_IRQC->IRQ18PEND;

    if (0u != (u32Val & 0x2u))
    {
        MSCSTAT_IRQHander2(5u,u8Mode);
    }
    if (0u != (u32Val & 0x1u))
    {
        MSCTX_IRQHander2(5,u8Mode);
    }
}

/**
 *******************************************************************************
 ** \brief DMAC Ch.0 & MSC Ch.6 Reception IRQ handler
 **
 ******************************************************************************/
void MSC6RX_DMAC0_IRQHandler(void)
{
    uint8_t u8Mode = 0u;
    uint32_t u32Val = 0u;
    u32Val = M0P_IRQC->IRQ19PEND;
    if (0u != (u32Val & 0x1u))
    {
        u8Mode = M0P_MSC6_SPI->MR_f.MODE;

        MSCRX_IRQHander2(6,u8Mode);
    }
    if (0u != (u32Val & 0x10u))
    {
        Dmac_IrqHandler(0);
    }
}

/**
 *******************************************************************************
 ** \brief DMAC Ch.1 & MSC Ch.6 Transmission & Status IRQ handler
 **
 ******************************************************************************/
void MSC6TX_DMAC1_IRQHandler(void)
{
    uint8_t u8Mode = 0;
    uint32_t u32Val = 0;

    u8Mode = M0P_MSC6_SPI->MR_f.MODE;
    u32Val = M0P_IRQC->IRQ20PEND;

    if (0u != (u32Val & 0x2u))
    {
        MSCSTAT_IRQHander2(6,u8Mode);
    }
    if (0u != (u32Val & 0x1u))
    {
        MSCTX_IRQHander2(6,u8Mode);
    }
    if (0u != (u32Val & 0x10u))
    {
        Dmac_IrqHandler(1);
    }
}

/**
 *******************************************************************************
 ** \brief External High/Low Speed OSC; Clock Counter; Ram parity IRQ handler
 **
 ******************************************************************************/
void OSC_CC_RP_IRQHandler(void)
{
    uint32_t u32Val = M0P_IRQC->IRQ24PEND;

    if (0u != (u32Val & (1 << 0)))
    {
        Clk_HSXTRdy_IrqHandler(0);
    }

    if (0u != (u32Val & (1 << 1)))
    {
        Clk_LSXTRdy_IrqHandler(0);
    }

    if (0u != (u32Val & (1 << 2)))
    {
        Clk_PLLRdy_IrqHandler(0);
    }

    if (0u != (u32Val & (1 << 4)))
    {
        Cc_IrqHandler(1);
    }

    if (0u != (u32Val & (1 << 7)))
    {
        Rami_IrqHandler(0);
    }
}

/**
 *******************************************************************************
 ** \brief A/D Converter IRQ handler
 **
 ******************************************************************************/
void ADC_IRQHandler(void)
{
    uint32_t u32Val = M0P_IRQC->IRQ25PEND;

    if (0u != (u32Val & (1 << 0)))
    {
        Adc_PriorityConvIrqHandler(0);
    }

    if (0u != (u32Val & (1 << 1)))
    {
        Adc_ScanConvIrqHandler(0);
    }

    if (0u != (u32Val & (1 << 2)))
    {
        Adc_FifoOvfIrqHandler(0);
    }

    if (0u != (u32Val & (1 << 3)))
    {
        Adc_ResultCmpIrqHandler(0);
    }

    if (0u != (u32Val & (1 << 4)))
    {
        Adc_RangeCmpIrqHandler(0);
    }
}

/**
 *******************************************************************************
 ** \brief Voltage Comparator IRQ handler
 **
 ******************************************************************************/
void VC_IRQHandler(void)
{
    Vc_IrqHandler(0);
}

/**
 *******************************************************************************
 ** \brief Timer4 counter IRQ handler
 **
 ******************************************************************************/
void TIM4CNT_IRQHandler(void)
{
    uint32_t u32Val = 0;

    u32Val = M0P_IRQC->IRQ28PEND;

    if (u32Val & 0x01u)
    {
        Timer4_Cnt_PeakIrqHandler(0);
    }

    if (u32Val & 0x08u)
    {
        Timer4_Cnt_ZeroIrqHandler(0);
    }
}

/**
 *******************************************************************************
 ** \brief Composite timer Ch.0~3 IRQ handler
 **
 ******************************************************************************/
void CTIM03_FLASH_IRQHandler(void)
{
    uint8_t i = 0;
    uint32_t u32Val = M0P_IRQC->IRQ31PEND;

    for (i = 0u; i < 4u; i++)
    {
        if (u32Val & (1u<< (2*i)))
        {
            Ct_Irq0Handler(i);
        }

        if (u32Val & (2u << (2*i)))
        {
            Ct_Irq1Handler(i);
        }
    }
    if (0u != (u32Val & (1 << 27)))
    {
        Flash_IrqHandler(0);
    }
}

/**
 *******************************************************************************
 ** \brief Timer4 Output comparator IRQ handler
 **
 ******************************************************************************/
void TIM4OCO_IRQHandler(void)
{
    uint8_t i = 0;
    uint32_t u32Val = 0;

    u32Val = M0P_IRQC->IRQ30PEND;

    for (i = 0; i < 6u; i++)
    {
        if (u32Val&(1u<<i))
        {
            Timer4_Oco_IrqHandler(i);
        }
    }
}

/**
 *******************************************************************************
 ** \brief Timer4 PWM & Emergency malfunction IRQ handler
 **
 ******************************************************************************/
void TIMER4PWM_EMI_IRQHandler(void)
{
    uint8_t i = 1;
    uint32_t u32Val = 0;

    u32Val = M0P_IRQC->IRQ03PEND;

    if (u32Val&1u)
    {
        Timer4_Emi_IrqHandler(0);
    }

    for (i = 1; i < 4u; i++)
    {
        if (u32Val & (1u<<i))
        {
            Timer4_Pwm_IrqHandler(i-1);
        }
    }
}

//@} // InterruptGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
