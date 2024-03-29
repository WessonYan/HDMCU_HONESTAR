================================================================================
                                样例使用说明
================================================================================
版本历史 
日期        版本    负责人         IAR     MDK   描述
2017-08-15  1.0     Jason          7.70    5.16  first version
================================================================================
功能描述
================================================================================
本样例主要展示基于ADC扫描转换的转换结果区间比较配置，ADC通用初始化配置，ADC 单次/重复采样示例。
说明：
    1、本样列选用外部A10(P70口)作为ADC扫描采样口
    2、本样列使用内部avcc 5.0V，12位采样数据左对齐，采样频率选择PCLK0 8分频，采样周期6个ADC时钟，chn 10 采样时间寄存器为reg1，FIFO级数1，连续采样
    3、本样例使能了转换结果区间比较，区间比较参考值为0x300~0x800，比较条件区间外
    4、本样列使能了转换结果区间比较完成中断，并在中断响应后除能转换结果区间比较及转换结果区间比较完成中断

================================================================================
测试环境
================================================================================
测试用板:
---------------------
SK-HC32F156-64L v10

辅助工具:
---------------------
Tektronix PWS2721

辅助软件:
---------------------
无

PIN脚连接:
---------------------
各通道P70脚连接外部输入V+，pin GND 接V-。
================================================================================
使用步骤
================================================================================
1）正确接线；
2）打开工程并重新编译；
3）启动IDE的下载和调试功能；
4）在全速模式下，于转换结果比较完成中断服务程序中打上断点，缓慢调节A10输入的电压；
5）停在对应断点时，查看当前输入电压是否符合小于0x300或大于0x800的转换值，结束；


================================================================================
注意
================================================================================
无 

================================================================================
