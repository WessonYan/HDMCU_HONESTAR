/******************************************************************************
* Copyright (C) 2017, Huada Semiconductor Co.,Ltd All rights reserved.
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
#include "lcd.h"
#include "lpm.h"
#include "gpio.h"
#include "flash.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')                                         
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                             
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
static void _UserKeyWait(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    ///< 端口方向配置->输出
    stcGpioCfg.enDir = GpioDirIn;
    ///< 端口驱动能力配置->低驱动能力
    stcGpioCfg.enDrv = GpioDrvL;
    ///< 端口上下拉配置->上拉
    stcGpioCfg.enPu = GpioPuEnable;
    stcGpioCfg.enPd = GpioPdDisable;
    ///< 端口开漏输出配置->开漏输出关闭
    stcGpioCfg.enOD = GpioOdDisable;
    ///< GPIO IO 初始化(在STK上外接KEY(USER))
    Gpio_Init(STK_USER_PORT, STK_USER_PIN, &stcGpioCfg);
    
    while(1)
    {
        ///< 检测电平(USER按键是否按下(低电平))
        if(FALSE == Gpio_GetInputIO(STK_USER_PORT, STK_USER_PIN))
        {
            break;
        }
        else
        {
            continue;
        }
    }
}

void Led_Cfg(void)
{
    stc_gpio_cfg_t stcGpioCfg;

    ///< LED关闭
    Gpio_ClrIO(STK_LED_PORT, STK_LED_PIN);
    
    ///< 端口方向配置->输出
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口驱动能力配置->高驱动能力
    stcGpioCfg.enDrv = GpioDrvH;
    ///< 端口上下拉配置->无上下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
    ///< 端口开漏输出配置->开漏输出关闭
    stcGpioCfg.enOD = GpioOdDisable;
    ///< 端口输入/输出值寄存器总线控制模式配置->AHB
    stcGpioCfg.enCtrlMode = GpioAHB;

    ///< GPIO IO LED端口初始化
    Gpio_Init(STK_LED_PORT, STK_LED_PIN, &stcGpioCfg);
}


/**
 ******************************************************************************
 ** \brief  初始化外部GPIO引脚
 **
 ** \return 无
 ******************************************************************************/
void Lcd_CfgPin(void)
{
    Gpio_SetAnalogMode(GpioPortA, GpioPin9);//COM0
    Gpio_SetAnalogMode(GpioPortA, GpioPin10);//COM1
    Gpio_SetAnalogMode(GpioPortA, GpioPin11);//COM2
    Gpio_SetAnalogMode(GpioPortA, GpioPin12);//COM3   

    Gpio_SetAnalogMode(GpioPortA, GpioPin8);//SEG0
    Gpio_SetAnalogMode(GpioPortC, GpioPin9);//SEG1
    Gpio_SetAnalogMode(GpioPortC, GpioPin8);//SEG2
    Gpio_SetAnalogMode(GpioPortC, GpioPin7);//SEG3
    Gpio_SetAnalogMode(GpioPortC, GpioPin6);//SEG4
    Gpio_SetAnalogMode(GpioPortB, GpioPin15);//SEG5
    Gpio_SetAnalogMode(GpioPortB, GpioPin14);//SEG6
    Gpio_SetAnalogMode(GpioPortB, GpioPin13);//SEG7
    Gpio_SetAnalogMode(GpioPortB, GpioPin3);//VLCDH
    Gpio_SetAnalogMode(GpioPortB, GpioPin4);//VLCD3
    Gpio_SetAnalogMode(GpioPortB, GpioPin5);//VLCD2
    Gpio_SetAnalogMode(GpioPortB, GpioPin6);//VLCD1
    
}

/**
 ******************************************************************************
 ** \brief  配置LCD
 **
 ** \return 无
 ******************************************************************************/
void Lcd_Cfg(void)
{
    stc_lcd_cfg_t LcdInitStruct;
    stc_lcd_segcom_t LcdSegCom;

    LcdSegCom.u32Seg0_31 = 0xffffffff;
    LcdSegCom.u32Seg0_31 = 0xffffff00;
    LcdSegCom.stc_seg32_51_com0_8_t.seg32_51_com0_8 = 0xffffffff;
    LcdSegCom.stc_seg32_51_com0_8_t.segcom_bit.Com0_3 = 0;
    LcdSegCom.stc_seg32_51_com0_8_t.segcom_bit.Mux = 0;
    LcdSegCom.stc_seg32_51_com0_8_t.segcom_bit.Seg32_35 = 0;
    Lcd_SetSegCom(&LcdSegCom);

    LcdInitStruct.LcdBiasSrc = LcdExtCap;
    LcdInitStruct.LcdDuty    = LcdDuty4;
    LcdInitStruct.LcdBias    = LcdBias3;
    LcdInitStruct.LcdCpClk   = LcdClk2k;
    LcdInitStruct.LcdScanClk = LcdClk128hz;
    LcdInitStruct.LcdMode    = LcdMode0;
    LcdInitStruct.LcdClkSrc  = LcdXTL;
    LcdInitStruct.LcdEn      = LcdEnable;
    Lcd_Init(&LcdInitStruct);
}

/**
******************************************************************************
    ** \brief  主函数
    ** 
    ** @param  无
    ** \retval 无
    **
******************************************************************************/ 
int32_t main(void)
{    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralLcd,TRUE);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralFlash,TRUE);

///<====================== INIT =============================
    ///<外部低速初始化时钟配置(for LCD)
    Sysctrl_XTLDriverCfg(SysctrlXtlAmp2, SysctrlXtalDriver2);
    Sysctrl_SetXTLStableTime(SysctrlXtlStableCycle16384);
    Sysctrl_ClkSourceEnable(SysctrlClkXTL,TRUE);
    
    ///< 外部高速初始化时钟配置
    ///< 切换时钟前（根据外部高速晶振）设置XTH频率范围,配置晶振参数，使能目标时钟，此处为32MHz
    Sysctrl_SetXTHFreq(SysctrlXthFreq20_32MHz);
    Sysctrl_XTHDriverCfg(SysctrlXtalDriver3);
    Sysctrl_SetXTHStableTime(SysctrlXthStableCycle16384);
    Sysctrl_ClkSourceEnable(SysctrlClkXTH, TRUE);
    
    ///< LCD 初始化配置
    Lcd_CfgPin();
    Lcd_Cfg();
    Lcd_ClearDisp();
    
    ///< LED 初始化配置
    Led_Cfg();
   
   ///<====================== TEST 1 (静态显示) =============================    

    ///< LCD显示“HC:32”
    Lcd_WriteRam(0,0x09050607);
    Lcd_WriteRam(1,0x0b060f02);

    ///< LED点亮
    Gpio_SetIO(STK_LED_PORT, STK_LED_PIN);
        
    ///<===================== TEST 2 (动态显示) =============================
    ///< User KEY 按下后程序继续执行
    _UserKeyWait();
    
    ///< 时钟切换
    ///< 因要使用的时钟源HCLK将大于24M：此处设置FLASH 读等待周期为1 cycle(默认值也为1 cycle)
    Flash_WaitCycle(FlashWaitCycle1);
    Sysctrl_SysClkSwitch(SysctrlClkXTH);
    
    while(1)
    {
        ///< LED点亮
        Gpio_SetIO(STK_LED_PORT, STK_LED_PIN);
        ///< LCD显示“HC:32”
        Lcd_WriteRam(0,0x09050607);
        Lcd_WriteRam(1,0x0b060f02);
    
        delay1ms(500);
        
        ///< LED熄灭
        Gpio_ClrIO(STK_LED_PORT, STK_LED_PIN);
        ///< LCD熄灭
        Lcd_WriteRam(0,0x0000000);
        Lcd_WriteRam(1,0x0000000);
        
        delay1ms(500);
    }        

}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


