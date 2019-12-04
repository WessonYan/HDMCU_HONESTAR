;/******************************************************************************
;* Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
;*
;* This software is owned and published by:
;* Huada Semiconductor Co.,Ltd ("HDSC").
;*
;* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
;* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
;*
;* This software contains source code for use with HDSC
;* components. This software is licensed by HDSC to be adapted only
;* for use in systems utilizing HDSC components. HDSC shall not be
;* responsible for misuse or illegal use of this software for devices not
;* supported herein. HDSC is providing this software "AS IS" and will
;* not be responsible for issues arising from incorrect user implementation
;* of the software.
;*
;* Disclaimer:
;* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
;* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
;* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
;* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
;* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
;* WARRANTY OF NONINFRINGEMENT.
;* HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
;* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
;* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
;* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
;* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
;* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
;* SAVINGS OR PROFITS,
;* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
;* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
;* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
;* FROM, THE SOFTWARE.
;*
;* This software may be replicated in part or whole for the licensed use,
;* with the restriction that this Disclaimer and Copyright notice must be
;* included with each copy of this software, whether used in part or whole,
;* at all times.
;*/
;/*****************************************************************************/

;/************************************************************************/
;/*  Startup for ARM                                                     */
;/*  Version     V1.2                                                    */
;/*  Date        2016-06-02                                              */
;/*  Target-mcu  HC32F_M14x                                                 */
;/************************************************************************/

; Stack Configuration
;  Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; Heap Configuration
;  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset        
                DCD     NMI_Handler               ; NMI
                DCD     HardFault_Handler         ; Hard Fault
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV
                DCD     SysTick_Handler           ; SysTick

                DCD     IRQ000_Handler            ; 0: Clock Super Visor
                DCD     IRQ001_Handler            ; 1: Software Watchdog Timer
                DCD     IRQ002_Handler            ; 2: Low Voltage Detector
                DCD     IRQ003_Handler            ; 3: Reserved
                DCD     IRQ004_Handler            ; 4: External Interrupt Request ch.0 to ch.7
                DCD     IRQ005_Handler            ; 5: External Interrupt Request ch.8 to ch.15
                DCD     IRQ006_Handler            ; 6: Dual Timer
                DCD     IRQ007_Handler            ; 7: MultiFunction Serial ch.0
                DCD     IRQ008_Handler            ; 8: MultiFunction Serial ch.0
                DCD     IRQ009_Handler            ; 9: MultiFunction Serial ch.1
                DCD     IRQ010_Handler            ; 10: MultiFunction Serial ch.1
                DCD     IRQ011_Handler            ; 11: MultiFunction Serial ch.2
                DCD     IRQ012_Handler            ; 12: MultiFunction Serial ch.2
                DCD     IRQ013_Handler            ; 13: Reserved
                DCD     IRQ014_Handler            ; 14: Reserved
                DCD     IRQ015_Handler            ; 15: MultiFunction Serial ch.4
                DCD     IRQ016_Handler            ; 16: MultiFunction Serial ch.4
                DCD     IRQ017_Handler            ; 17: MultiFunction Serial ch.5
                DCD     IRQ018_Handler            ; 18: MultiFunction Serial ch.5
                DCD     IRQ019_Handler            ; 19: MultiFunction Serial ch.6
                DCD     IRQ020_Handler            ; 20: MultiFunction Serial ch.6
                DCD     IRQ021_Handler            ; 21: Reserved
                DCD     IRQ022_Handler            ; 22: Reserved
                DCD     IRQ023_Handler            ; 23: Reserved
                DCD     IRQ024_Handler            ; 24: OSC / MFS Wake Up / RTC
                DCD     IRQ025_Handler            ; 25: ADC0
                DCD     IRQ026_Handler            ; 26: Voltage comparator
                DCD     IRQ027_Handler            ; 27: SCI7816
                DCD     IRQ028_Handler            ; 28: Pulse counter
                DCD     IRQ029_Handler            ; 29: AES
                DCD     IRQ030_Handler            ; 30: Reserved
                DCD     IRQ031_Handler            ; 31: Base Timer ch.0 to ch.7 / Flash

__Vectors_End

__Vectors_Size 	EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  IRQ000_Handler               [WEAK]
                EXPORT  IRQ001_Handler               [WEAK]
                EXPORT  IRQ002_Handler               [WEAK]
                EXPORT  IRQ003_Handler               [WEAK]
                EXPORT  IRQ004_Handler               [WEAK]
                EXPORT  IRQ005_Handler               [WEAK]
                EXPORT  IRQ006_Handler               [WEAK]
                EXPORT  IRQ007_Handler               [WEAK]
                EXPORT  IRQ008_Handler               [WEAK]
                EXPORT  IRQ009_Handler               [WEAK]
                EXPORT  IRQ010_Handler               [WEAK]
                EXPORT  IRQ011_Handler               [WEAK]
                EXPORT  IRQ012_Handler               [WEAK]
                EXPORT  IRQ013_Handler               [WEAK]
                EXPORT  IRQ014_Handler               [WEAK]
                EXPORT  IRQ015_Handler               [WEAK]
                EXPORT  IRQ016_Handler               [WEAK]
                EXPORT  IRQ017_Handler               [WEAK]
                EXPORT  IRQ018_Handler               [WEAK]
                EXPORT  IRQ019_Handler               [WEAK]
                EXPORT  IRQ020_Handler               [WEAK]
                EXPORT  IRQ021_Handler               [WEAK]
                EXPORT  IRQ022_Handler               [WEAK]
                EXPORT  IRQ023_Handler               [WEAK]
                EXPORT  IRQ024_Handler               [WEAK]
                EXPORT  IRQ025_Handler               [WEAK]
                EXPORT  IRQ026_Handler               [WEAK]
                EXPORT  IRQ027_Handler               [WEAK]
                EXPORT  IRQ028_Handler               [WEAK]
                EXPORT  IRQ029_Handler               [WEAK]
                EXPORT  IRQ030_Handler               [WEAK]
                EXPORT  IRQ031_Handler               [WEAK]


IRQ000_Handler
IRQ001_Handler
IRQ002_Handler
IRQ003_Handler
IRQ004_Handler
IRQ005_Handler
IRQ006_Handler
IRQ007_Handler
IRQ008_Handler
IRQ009_Handler
IRQ010_Handler
IRQ011_Handler
IRQ012_Handler
IRQ013_Handler
IRQ014_Handler
IRQ015_Handler
IRQ016_Handler
IRQ017_Handler
IRQ018_Handler
IRQ019_Handler
IRQ020_Handler
IRQ021_Handler
IRQ022_Handler
IRQ023_Handler
IRQ024_Handler
IRQ025_Handler
IRQ026_Handler
IRQ027_Handler
IRQ028_Handler
IRQ029_Handler
IRQ030_Handler
IRQ031_Handler
               B .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
