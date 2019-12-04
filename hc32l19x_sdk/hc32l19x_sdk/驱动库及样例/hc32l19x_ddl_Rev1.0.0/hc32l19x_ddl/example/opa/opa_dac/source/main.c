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
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2016-02-16  1.0  XYZ First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "gpio.h"
#include "opa.h"
#include "bgr.h"
#include "dac.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
 
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
static void App_DacInit(void);
static void App_GpioInit(void);
static void App_OpaInit(void);
/**
 ******************************************************************************
 ** \brief  主函数
 **
 ** \return 无
 ******************************************************************************/
int32_t main(void)
{   
    uint16_t tmp;
    
    // 配置OPA端口
    App_GpioInit();
    
    App_DacInit();                      // 配置DAC，DAC输出三角波
    App_OpaInit();                      // OPA 初始化
    while (1)
    {
        Dac_SoftwareTriggerCmd();    // DAC软件触发输出
        for(tmp=0; tmp<100; tmp++);  // 延时 
    }
}


static void App_GpioInit(void)
{
    stc_gpio_cfg_t GpioInitStruct;
    
    // 使能外设时钟模块，使用OPA，必须也要开启AdcBgr模块时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);    // 使能GPIO模块的时钟
    
    // 配置OPA端口
    GpioInitStruct.enDir = GpioDirIn;
    Gpio_Init(GpioPortB, GpioPin0, &GpioInitStruct);          // OPA_INP引脚：PB00
}

static void App_OpaInit(void)
{
    // 使能外设时钟模块，使用OPA，必须也要开启AdcBgr模块时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralOpa, TRUE);     // 使能OPA模块的时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE);  // 使能BGR模块的时钟
    Bgr_BgrEnable();
    
    Opa_Cmd(FALSE);                     // 禁止OPA(注：此时的OPA输入时通过DAC的输出提供信号)
    Opa_CmdBuf(TRUE);                   // 使能OPABUFEN   
}


/**
******************************************************************************
    ** \brief  配置DAC
    ** 
    ** @param  无
    ** \retval 无
    **
******************************************************************************/ 
static void App_DacInit(void)
{
    stc_dac_cfg_t  dac_initstruct;
    
    DDL_ZERO_STRUCT(dac_initstruct);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralDac, TRUE);     //使能DAC模块的时钟
    
    dac_initstruct.boff_t = DacBoffDisable;    //DAC0通道输出缓冲器使能
    dac_initstruct.ten_t  = DacTenEnable;     //DAC0通道触发使能
    dac_initstruct.sref_t = DacVoltageAvcc;
    dac_initstruct.mamp_t = DacMenp2047;
    dac_initstruct.wave_t = DacTrWaveEnable;
    dac_initstruct.tsel_t = DacSwTriger;      //软件触发方式
    dac_initstruct.align  = DacRightAlign;    //右对齐
    dac_initstruct.dhr12  = 50;           //三角波基值
    Dac_Init(&dac_initstruct);
    Dac_Cmd(TRUE);
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


