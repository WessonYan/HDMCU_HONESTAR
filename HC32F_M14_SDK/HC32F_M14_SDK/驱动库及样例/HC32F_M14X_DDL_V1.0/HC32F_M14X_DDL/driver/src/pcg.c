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
/** \file pcg.c
 **
 ** A detailed description is available at
 ** @link PcgGroup PCG description @endlink
 **
 **   - 2017-08-09  1.0  chengy First version for Device Driver Library of PCG.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "pcg.h"

/**
 *******************************************************************************
 ** \addtogroup PcgGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*!<Parameter validity check for peripheral*/
#define IS_VALID_PERIPHERAL(x)         \
(   (MSC4 <= (x))   &&                 \
    (SWWDT >= (x))  )

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
 ** \brief Enable Peripheral's CLK
 **
 ** \param [in]  enPeri     Enumeration of peripheral name
 ** \arg MSC4   Multi-functional Serial Channel 4
 ** \arg MSC5   Multi-functional Serial Channel 5
 ** \arg MSC6   Multi-functional Serial Channel 6
 ** \arg GPIO   General Purpose Input/Output
 ** \arg DMA    Direct Memory Access
 ** \arg ADC    Analog to Digital Converter
 ** \arg OPA_0  Operational Amplifier Channel 0
 ** \arg OPA_1  Operational Amplifier Channel 1
 ** \arg TIM4   Timer4
 ** \arg VC     Voltage Comparator
 ** \arg CT     Composite Timer(channels 0-3)
 ** \arg LCDC   LCD Controller
 ** \arg DT     Dual timer
 ** \arg HWWDT  Hardware watchdog timer
 ** \arg SWWDT  Software watchdog timer
 **
 ** \retval Ok                    Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                                 - enPeri is invalid
 **
 ******************************************************************************/
en_result_t Pcg_EnableCLK(en_peripheral_t enPeri)
{
    en_result_t     enResult = Ok;

    switch (enPeri)
    {
        case MSC4:
            /* Enable CLK of MSC4 */
            M0P_PCG->CLKEN0_f.MSC4CK = TRUE;
            while(TRUE != M0P_PCG->CLKEN0_f.MSC4CK){}
            break;
        case MSC5:
            /* Enable CLK of MSC5 */
            M0P_PCG->CLKEN0_f.MSC5CK = TRUE;
            while(TRUE != M0P_PCG->CLKEN0_f.MSC5CK){}
            break;
        case MSC6:
            /* Enable CLK of MSC6 */
            M0P_PCG->CLKEN0_f.MSC6CK = TRUE;
            while(TRUE != M0P_PCG->CLKEN0_f.MSC6CK){}
            break;
        case GPIO:
            /* Enable CLK of GPIO */
            M0P_PCG->CLKEN0_f.GIOCK = TRUE;
            while(TRUE != M0P_PCG->CLKEN0_f.GIOCK){}
            break;
        case DMA:
            /* Enable clk of DMA */
            M0P_PCG->CLKEN0_f.DMACK = TRUE;
            while(TRUE != M0P_PCG->CLKEN0_f.DMACK){}
            break;
        case ADC:
            /* Enable CLK of ADC */
            M0P_PCG->CLKEN0_f.ADCCK = TRUE;
            while(TRUE != M0P_PCG->CLKEN0_f.ADCCK){}
            break;
        case OPA_0:
            /* Enable clk of OPA_0 */
            M0P_PCG->CLKEN1_f.OPA0CK = TRUE;
            while(TRUE != M0P_PCG->CLKEN1_f.OPA0CK){}
            break;
        case OPA_1:
            /* Enable CLK of OPA_1 */
            M0P_PCG->CLKEN1_f.OPA1CK = TRUE;
            while(TRUE != M0P_PCG->CLKEN1_f.OPA1CK){}
            break;
        case TIM4:
            /* Enable CLK of TIM4 */
            M0P_PCG->CLKEN1_f.TM4CK = TRUE;
            while(TRUE != M0P_PCG->CLKEN1_f.TM4CK){}
            break;
        case VC:
            /* Enable CLK of VC */
            M0P_PCG->CLKEN1_f.VCCK = TRUE;
            while(TRUE != M0P_PCG->CLKEN1_f.VCCK){}
            break;
        case CT:
            /* Enable CLK of CT */
            M0P_PCG->CLKEN1_f.CTCK = TRUE;
            while(TRUE != M0P_PCG->CLKEN1_f.CTCK){}
            break;
        case LCDC:
            /* Enable CLK of LCDC */
            M0P_PCG->CLKEN2_f.LCDCCK = TRUE;
            while(TRUE != M0P_PCG->CLKEN2_f.LCDCCK){}
            break;
        case DT:
            /* Enable CLK of DT */
            M0P_CLOCK->SWDG_CTRL_f.DTCKE = TRUE;
            while(TRUE !=M0P_CLOCK->SWDG_CTRL_f.DTCKE){}
            break;
        case HWWDT:
            /* Enable CLK of HWWDT */
            M0P_CLOCK->SWDG_CTRL_f.HWCKE = TRUE;
            while(TRUE !=M0P_CLOCK->SWDG_CTRL_f.HWCKE){}
            break;
        case SWWDT:
            /* Enable CLK of SWWDT */
            M0P_CLOCK->SWDG_CTRL_f.SWCKE = TRUE;
            while(TRUE !=M0P_CLOCK->SWDG_CTRL_f.SWCKE){}
            break;
        default:
            enResult = ErrorInvalidParameter;
            break;
    }

    return (enResult);
}

/**
 *******************************************************************************
 ** \brief Disable Peripheral's CLK
 **
 ** \param [in]  enPeri     Enumeration of peripheral name
 ** \arg MSC4   Multi-functional Serial Channel 4
 ** \arg MSC5   Multi-functional Serial Channel 5
 ** \arg MSC6   Multi-functional Serial Channel 6
 ** \arg GPIO   General Purpose Input/Output
 ** \arg DMA    Direct Memory Access
 ** \arg ADC    Analog to Digital Converter
 ** \arg OPA_0  Operational Amplifier Channel 0
 ** \arg OPA_1  Operational Amplifier Channel 1
 ** \arg TIM4   Timer4
 ** \arg VC     Voltage Comparator
 ** \arg CT     Composite Timer(channels 0-3)
 ** \arg LCDC   LCD Controller
 ** \arg DT     Dual timer
 ** \arg HWWDT  Hardware watchdog timer
 ** \arg SWWDT  Software watchdog timer
 **
 ** \retval Ok                    Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                                 - enPeri is invalid
 **
 ******************************************************************************/
en_result_t Pcg_DisableCLK(en_peripheral_t enPeri)
{
    en_result_t     enResult = Ok;

    switch (enPeri)
    {
        case MSC4:
            /* Disable CLK of MSC4 */
            M0P_PCG->CLKEN0_f.MSC4CK = FALSE;
            break;
        case MSC5:
            /* Disable CLK of MSC5 */
            M0P_PCG->CLKEN0_f.MSC5CK = FALSE;
            break;
        case MSC6:
            /* Disable CLK of MSC6 */
            M0P_PCG->CLKEN0_f.MSC6CK = FALSE;
            break;
        case GPIO:
            /* Disable CLK of GPIO */
            M0P_PCG->CLKEN0_f.GIOCK = FALSE;
            break;
        case DMA:
            /* Disable CLK of DMA */
            M0P_PCG->CLKEN0_f.DMACK = FALSE;
            break;
        case ADC:
            /* Disable CLK of ADC */
            M0P_PCG->CLKEN0_f.ADCCK = FALSE;
            break;
        case OPA_0:
            /* Disable CLK of OPA_0 */
            M0P_PCG->CLKEN1_f.OPA0CK = FALSE;
            break;
        case OPA_1:
            /* Disable CLK of OPA_1 */
            M0P_PCG->CLKEN1_f.OPA1CK = FALSE;
            break;
        case TIM4:
            /* Disable CLK of TIM4 */
            M0P_PCG->CLKEN1_f.TM4CK = FALSE;
            break;
        case VC:
            /* Disable CLK of VC */
            M0P_PCG->CLKEN1_f.VCCK = FALSE;
            break;
        case CT:
            /* Disable CLK of CT */
            M0P_PCG->CLKEN1_f.CTCK = FALSE;
            break;
        case LCDC:
            /* Disable CLK of LCD */
            M0P_PCG->CLKEN2_f.LCDCCK = FALSE;
            break;
        case DT:
            /* Disable CLK of DT */
            M0P_CLOCK->SWDG_CTRL_f.DTCKE = FALSE;
            break;
        case HWWDT:
            /* Disable CLK of HWWDT */
            M0P_CLOCK->SWDG_CTRL_f.HWCKE = FALSE;
            break;
        case SWWDT:
            /* Disable CLK of SWWDT */
            M0P_CLOCK->SWDG_CTRL_f.SWCKE = FALSE;
            break;
        default:
            enResult = ErrorInvalidParameter;
            break;
    }

    return (enResult);
}

/**
 *******************************************************************************
 ** \brief Enable Peripheral's RST
 **
 ** \param [in]  enPeri     Enumeration of peripheral name
 ** \arg MSC4   Multi-functional Serial Channel 4
 ** \arg MSC5   Multi-functional Serial Channel 5
 ** \arg MSC6   Multi-functional Serial Channel 6
 ** \arg GPIO   General Purpose Input/Output
 ** \arg DMA    Direct Memory Access
 ** \arg ADC    Analog to Digital Converter
 ** \arg OPA_0  Operational Amplifier Channel 0
 ** \arg OPA_1  Operational Amplifier Channel 1
 ** \arg TIM4   Timer4
 ** \arg VC     Voltage Comparator
 ** \arg CT     Composite Timer(channels 0-3)
 ** \arg LCDC   LCD Controller
 **
 ** \retval Ok                    Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                                 - enPeri is invalid
 **
 ******************************************************************************/
en_result_t Pcg_EnableRST(en_peripheral_t enPeri)
{
    en_result_t     enResult = Ok;

    switch (enPeri)
    {
        case MSC4:
            /* Enable RST of MSC4 */
            M0P_PCG->RSTCTL0_f.MSC4RST = TRUE;
            break;
        case MSC5:
            /* Enable RST of MSC5 */
            M0P_PCG->RSTCTL0_f.MSC5RST = TRUE;
            break;
        case MSC6:
            /* Enable RST of MSC6 */
            M0P_PCG->RSTCTL0_f.MSC6RST = TRUE;
            break;
        case DMA:
            /* Enable RST of DMA */
            M0P_PCG->RSTCTL0_f.DMARST = TRUE;
            break;
        case ADC:
            /* Enable RST of ADC */
            M0P_PCG->RSTCTL0_f.ADCRST = TRUE;
            break;
        case OPA_0:
            /* Enable RST of OPA_0 */
            M0P_PCG->RSTCTL1_f.OPA0RST = TRUE;
            break;
        case OPA_1:
            /* Enable RST of OPA_1 */
            M0P_PCG->RSTCTL1_f.OPA1RST = TRUE;
            break;
        case TIM4:
            /* Enable RST of TIM4 */
            M0P_PCG->RSTCTL1_f.TM4RST = TRUE;
            break;
        case VC:
            /* Enable RST of VC */
            M0P_PCG->RSTCTL1_f.VCRST = TRUE;
            break;
        case CT:
            /* Enable RST of CT */
            M0P_PCG->RSTCTL1_f.CTMRST = TRUE;
            break;
        case LCDC:
            /* Enable RST of LCDC */
            M0P_PCG->RSTCTL2_f.LCDCRST = TRUE;
            break;
        default:
            enResult = ErrorInvalidParameter;
            break;
    }

    return (enResult);
}

/**
 *******************************************************************************
 ** \brief Disable Peripheral's RST
 **
 ** This function Disables the peripheral's RST.
 **
 ** \param [in]  enPeri     Enumeration of peripheral name
 ** \arg MSC4   Multi-functional Serial Channel 4
 ** \arg MSC5   Multi-functional Serial Channel 5
 ** \arg MSC6   Multi-functional Serial Channel 6
 ** \arg GPIO   General Purpose Input/Output
 ** \arg DMA    Direct Memory Access
 ** \arg ADC    Analog to Digital Converter
 ** \arg OPA_0  Operational Amplifier Channel 0
 ** \arg OPA_1  Operational Amplifier Channel 1
 ** \arg TIM4   Timer4
 ** \arg VC     Voltage Comparator
 ** \arg CT     Composite Timer(channels 0-3)
 ** \arg LCDC   LCD Controller
 **
 ** \retval Ok                    Process successfully done.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                                 - enPeri is invalid
 **
 ******************************************************************************/
en_result_t Pcg_DisableRST(en_peripheral_t enPeri)
{
    en_result_t     enResult = Ok;

    switch (enPeri)
    {
        case MSC4:
            /* Disable RST of MSC4 */
            M0P_PCG->RSTCTL0_f.MSC4RST = FALSE;
            break;
        case MSC5:
            /* Disable RST of MSC5 */
            M0P_PCG->RSTCTL0_f.MSC5RST = FALSE;
            break;
        case MSC6:
            /* Disable RST of MSC6 */
            M0P_PCG->RSTCTL0_f.MSC6RST = FALSE;
            break;
        case DMA:
            /* Disable RST of DMA */
            M0P_PCG->RSTCTL0_f.DMARST = FALSE;
            break;
        case ADC:
            /* Disable RST of ADC */
            M0P_PCG->RSTCTL0_f.ADCRST = FALSE;
            break;
        case OPA_0:
            /* Disable RST of OPA_0 */
            M0P_PCG->RSTCTL1_f.OPA0RST = FALSE;
            break;
        case OPA_1:
            /* Disable RST of OPA_1 */
            M0P_PCG->RSTCTL1_f.OPA1RST = FALSE;
            break;
        case TIM4:
            /* Disable RST of TIM4 */
            M0P_PCG->RSTCTL1_f.TM4RST = FALSE;
            break;
        case VC:
            /* Disable RST of VC */
            M0P_PCG->RSTCTL1_f.VCRST = FALSE;
            break;
        case CT:
            /* Disable RST of CT */
            M0P_PCG->RSTCTL1_f.CTMRST = FALSE;
            break;
        case LCDC:
            /* Disable RST of LCDC */
            M0P_PCG->RSTCTL2_f.LCDCRST = FALSE;
            break;
        default:
            enResult = ErrorInvalidParameter;
            break;
    }

    return (enResult);
}

/**
 *******************************************************************************
 ** \brief Stop peripheral CLK
 **
 ** This function Stop peripheral CLK.
 **
 ** \param [in]  enPeri     Enumeration of peripheral name
 ** \arg MSC4   Multi-functional Serial Channel 4
 ** \arg MSC5   Multi-functional Serial Channel 5
 ** \arg MSC6   Multi-functional Serial Channel 6
 ** \arg GPIO   General Purpose Input/Output
 ** \arg DMA    Direct Memory Access
 ** \arg ADC    Analog to Digital Converter
 ** \arg OPA_0  Operational Amplifier Channel 0
 ** \arg OPA_1  Operational Amplifier Channel 1
 ** \arg TIM4   Timer4
 ** \arg VC     Voltage Comparator
 ** \arg CT     Composite Timer(channels 0-3)
 ** \arg LCDC   LCD Controller
 **
 ** \retval Ok          Process successfully done.
 ** \retval Error       If one of following conditions are met:
 **                       - enPeri is invalid
 **
 ******************************************************************************/
en_result_t Pcg_Stop(en_peripheral_t enPeri)
{
    ASSERT(IS_VALID_PERIPHERAL(enPeri));

    if (Ok != Pcg_DisableCLK(enPeri))
    {
        return Error;
    }

    if (Ok != Pcg_EnableRST(enPeri))
    {
        return Error;
    }

    if (Ok != Pcg_DisableRST(enPeri))
    {
        return Error;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Recovery peripheral CLK
 **
 ** This function Recovery peripheral CLK.
 **
 ** \param [in]  enPeri     Enumeration of peripheral name
 ** \arg MSC4   Multi-functional Serial Channel 4
 ** \arg MSC5   Multi-functional Serial Channel 5
 ** \arg MSC6   Multi-functional Serial Channel 6
 ** \arg GPIO   General Purpose Input/Output
 ** \arg DMA    Direct Memory Access
 ** \arg ADC    Analog to Digital Converter
 ** \arg OPA_0  Operational Amplifier Channel 0
 ** \arg OPA_1  Operational Amplifier Channel 1
 ** \arg TIM4   Timer4
 ** \arg VC     Voltage Comparator
 ** \arg CT     Composite Timer(channels 0-3)
 ** \arg LCDC   LCD Controller
 **
 ** \retval Ok          Process successfully done.
 ** \retval Error       If one of following conditions are met:
 **                       - enPeri is invalid
 **
 ******************************************************************************/
en_result_t Pcg_Recovery(en_peripheral_t enPeri)
{
    ASSERT(IS_VALID_PERIPHERAL(enPeri));

    if (Ok != Pcg_EnableRST(enPeri))
    {
        return Error;
    }

    if (Ok != Pcg_DisableRST(enPeri))
    {
        return Error;
    }

    if (Ok != Pcg_EnableCLK(enPeri))
    {
        return Error;
    }

    return Ok;
}

//@} // PcgGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
