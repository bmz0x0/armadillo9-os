
#ifndef _REGMAP_H
#define _REGMAP_H


#define IO_BASE_VIRT        0xFF000000     // Virtual address of IO 
#define IO_BASE_PHYS        0x80000000     // Physical address of IO

#if 1
#define EP93XX_AHB_BASE (IO_BASE_PHYS)
#else
#define EP93XX_AHB_BASE (IO_BASE_VIRT)
#endif

#if 1
#define EP93XX_APB_BASE (IO_BASE_PHYS | 0x00800000)
#else
#define EP93XX_APB_BASE (IO_BASE_VIRT | 0x00800000)
#endif

// AHB_BASE
/* 800B_0000 - 800B_FFFF: VIC 0 */
#define VIC0_OFFSET              0x0B0000
#define VIC0_BASE                (EP93XX_AHB_BASE|VIC0_OFFSET)
#define VIC0                     (VIC0_BASE+0x000) 
#define VIC0IRQSTATUS            (VIC0_BASE+0x000) /* R   IRQ status register               */
#define VIC0FIQSTATUS            (VIC0_BASE+0x004) /* R   FIQ status register               */
#define VIC0RAWINTR              (VIC0_BASE+0x008) /* R   Raw interrupt status register     */
#define VIC0INTSELECT            (VIC0_BASE+0x00C) /* R/W Interrupt select register         */
#define VIC0INTENABLE            (VIC0_BASE+0x010) /* R/W Interrupt enable register         */
#define VIC0INTENCLEAR           (VIC0_BASE+0x014) /* W   Interrupt enable clear register   */
#define VIC0SOFTINT              (VIC0_BASE+0x018) /* R/W Software interrupt register       */
#define VIC0SOFTINTCLEAR         (VIC0_BASE+0x01C) /* R/W Software interrupt clear register */
#define VIC0PROTECTION           (VIC0_BASE+0x020) /* R/W Protection enable register        */
#define VIC0VECTADDR             (VIC0_BASE+0x030) /* R/W Vector address register           */
#define VIC0DEFVECTADDR          (VIC0_BASE+0x034) /* R/W Default vector address register   */
#define VIC0VECTADDR00           (VIC0_BASE+0x100) /* R/W Vector address 00 register        */
#define VIC0VECTADDR01           (VIC0_BASE+0x104) /* R/W Vector address 01 register        */
#define VIC0VECTADDR02           (VIC0_BASE+0x108) /* R/W Vector address 02 register        */
#define VIC0VECTADDR03           (VIC0_BASE+0x10C) /* R/W Vector address 03 register        */
#define VIC0VECTADDR04           (VIC0_BASE+0x110) /* R/W Vector address 04 register        */
#define VIC0VECTADDR05           (VIC0_BASE+0x114) /* R/W Vector address 05 register        */
#define VIC0VECTADDR06           (VIC0_BASE+0x118) /* R/W Vector address 06 register        */
#define VIC0VECTADDR07           (VIC0_BASE+0x11C) /* R/W Vector address 07 register        */
#define VIC0VECTADDR08           (VIC0_BASE+0x120) /* R/W Vector address 08 register        */
#define VIC0VECTADDR09           (VIC0_BASE+0x124) /* R/W Vector address 09 register        */
#define VIC0VECTADDR10           (VIC0_BASE+0x128) /* R/W Vector address 10 register        */
#define VIC0VECTADDR11           (VIC0_BASE+0x12C) /* R/W Vector address 11 register        */
#define VIC0VECTADDR12           (VIC0_BASE+0x130) /* R/W Vector address 12 register        */
#define VIC0VECTADDR13           (VIC0_BASE+0x134) /* R/W Vector address 13 register        */
#define VIC0VECTADDR14           (VIC0_BASE+0x138) /* R/W Vector address 14 register        */
#define VIC0VECTADDR15           (VIC0_BASE+0x13C) /* R/W Vector address 15 register        */
#define VIC0VECTCNTL00           (VIC0_BASE+0x200) /* R/W Vector control 00 register        */
#define VIC0VECTCNTL01           (VIC0_BASE+0x204) /* R/W Vector control 01 register        */
#define VIC0VECTCNTL02           (VIC0_BASE+0x208) /* R/W Vector control 02 register        */
#define VIC0VECTCNTL03           (VIC0_BASE+0x20C) /* R/W Vector control 03 register        */
#define VIC0VECTCNTL04           (VIC0_BASE+0x210) /* R/W Vector control 04 register        */
#define VIC0VECTCNTL05           (VIC0_BASE+0x214) /* R/W Vector control 05 register        */
#define VIC0VECTCNTL06           (VIC0_BASE+0x218) /* R/W Vector control 06 register        */
#define VIC0VECTCNTL07           (VIC0_BASE+0x21C) /* R/W Vector control 07 register        */
#define VIC0VECTCNTL08           (VIC0_BASE+0x220) /* R/W Vector control 08 register        */
#define VIC0VECTCNTL09           (VIC0_BASE+0x224) /* R/W Vector control 09 register        */
#define VIC0VECTCNTL10           (VIC0_BASE+0x228) /* R/W Vector control 10 register        */
#define VIC0VECTCNTL11           (VIC0_BASE+0x22C) /* R/W Vector control 11 register        */
#define VIC0VECTCNTL12           (VIC0_BASE+0x230) /* R/W Vector control 12 register        */
#define VIC0VECTCNTL13           (VIC0_BASE+0x234) /* R/W Vector control 13 register        */
#define VIC0VECTCNTL14           (VIC0_BASE+0x238) /* R/W Vector control 14 register        */
#define VIC0VECTCNTL15           (VIC0_BASE+0x23C) /* R/W Vector control 15 register        */
#define VIC0ITCR                 (VIC0_BASE+0x300) /* R/W Test control register             */
#define VIC0ITIP1                (VIC0_BASE+0x304) /* R   Test input register (nVICIRQIN/nVICFIQIN)*/
#define VIC0ITIP2                (VIC0_BASE+0x308) /* R   Test input register (VICVECTADDRIN)      */
#define VIC0ITOP1                (VIC0_BASE+0x30C) /* R   Test output register (nVICIRQ/nVICFIQ)   */
#define VIC0ITOP2                (VIC0_BASE+0x310) /* R   Test output register (VICVECTADDROUT)    */
#define VIC0PERIPHID0            (VIC0_BASE+0xFE0) /* R   Peripheral ID register bits 7:0   */
#define VIC0PERIPHID1            (VIC0_BASE+0xFE4) /* R   Peripheral ID register bits 15:8  */
#define VIC0PERIPHID2            (VIC0_BASE+0xFE8) /* R   Peripheral ID register bits 23:16 */
#define VIC0PERIPHID3            (VIC0_BASE+0xFEC) /* R   Peripheral ID register bits 31:24 */


/* 800C_0000 - 800C_FFFF: VIC 0 */
#define VIC1_OFFSET              0x0C0000
#define VIC1_BASE                (EP93XX_AHB_BASE|VIC1_OFFSET)
#define VIC1                     (VIC1_BASE+0x000) 
#define VIC1IRQSTATUS            (VIC1_BASE+0x000) /* R   IRQ status register               */
#define VIC1FIQSTATUS            (VIC1_BASE+0x004) /* R   FIQ status register               */
#define VIC1RAWINTR              (VIC1_BASE+0x008) /* R   Raw interrupt status register     */
#define VIC1INTSELECT            (VIC1_BASE+0x00C) /* R/W Interrupt select register         */
#define VIC1INTENABLE            (VIC1_BASE+0x010) /* R/W Interrupt enable register         */
#define VIC1INTENCLEAR           (VIC1_BASE+0x014) /* W   Interrupt enable clear register   */
#define VIC1SOFTINT              (VIC1_BASE+0x018) /* R/W Software interrupt register       */
#define VIC1SOFTINTCLEAR         (VIC1_BASE+0x01C) /* R/W Software interrupt clear register */
#define VIC1PROTECTION           (VIC1_BASE+0x020) /* R/W Protection enable register        */
#define VIC1VECTADDR             (VIC1_BASE+0x030) /* R/W Vector address register           */
#define VIC1DEFVECTADDR          (VIC1_BASE+0x034) /* R/W Default vector address register   */
#define VIC1VECTADDR00           (VIC1_BASE+0x100) /* R/W Vector address 00 register        */
#define VIC1VECTADDR01           (VIC1_BASE+0x104) /* R/W Vector address 01 register        */
#define VIC1VECTADDR02           (VIC1_BASE+0x108) /* R/W Vector address 02 register        */
#define VIC1VECTADDR03           (VIC1_BASE+0x10C) /* R/W Vector address 03 register        */
#define VIC1VECTADDR04           (VIC1_BASE+0x110) /* R/W Vector address 04 register        */
#define VIC1VECTADDR05           (VIC1_BASE+0x114) /* R/W Vector address 05 register        */
#define VIC1VECTADDR06           (VIC1_BASE+0x118) /* R/W Vector address 06 register        */
#define VIC1VECTADDR07           (VIC1_BASE+0x11C) /* R/W Vector address 07 register        */
#define VIC1VECTADDR08           (VIC1_BASE+0x120) /* R/W Vector address 08 register        */
#define VIC1VECTADDR09           (VIC1_BASE+0x124) /* R/W Vector address 09 register        */
#define VIC1VECTADDR10           (VIC1_BASE+0x128) /* R/W Vector address 10 register        */
#define VIC1VECTADDR11           (VIC1_BASE+0x12C) /* R/W Vector address 11 register        */
#define VIC1VECTADDR12           (VIC1_BASE+0x130) /* R/W Vector address 12 register        */
#define VIC1VECTADDR13           (VIC1_BASE+0x134) /* R/W Vector address 13 register        */
#define VIC1VECTADDR14           (VIC1_BASE+0x138) /* R/W Vector address 14 register        */
#define VIC1VECTADDR15           (VIC1_BASE+0x13C) /* R/W Vector address 15 register        */
#define VIC1VECTCNTL00           (VIC1_BASE+0x200) /* R/W Vector control 00 register        */
#define VIC1VECTCNTL01           (VIC1_BASE+0x204) /* R/W Vector control 01 register        */
#define VIC1VECTCNTL02           (VIC1_BASE+0x208) /* R/W Vector control 02 register        */
#define VIC1VECTCNTL03           (VIC1_BASE+0x20C) /* R/W Vector control 03 register        */
#define VIC1VECTCNTL04           (VIC1_BASE+0x210) /* R/W Vector control 04 register        */
#define VIC1VECTCNTL05           (VIC1_BASE+0x214) /* R/W Vector control 05 register        */
#define VIC1VECTCNTL06           (VIC1_BASE+0x218) /* R/W Vector control 06 register        */
#define VIC1VECTCNTL07           (VIC1_BASE+0x21C) /* R/W Vector control 07 register        */
#define VIC1VECTCNTL08           (VIC1_BASE+0x220) /* R/W Vector control 08 register        */
#define VIC1VECTCNTL09           (VIC1_BASE+0x224) /* R/W Vector control 09 register        */
#define VIC1VECTCNTL10           (VIC1_BASE+0x228) /* R/W Vector control 10 register        */
#define VIC1VECTCNTL11           (VIC1_BASE+0x22C) /* R/W Vector control 11 register        */
#define VIC1VECTCNTL12           (VIC1_BASE+0x230) /* R/W Vector control 12 register        */
#define VIC1VECTCNTL13           (VIC1_BASE+0x234) /* R/W Vector control 13 register        */
#define VIC1VECTCNTL14           (VIC1_BASE+0x238) /* R/W Vector control 14 register        */
#define VIC1VECTCNTL15           (VIC1_BASE+0x23C) /* R/W Vector control 15 register        */
#define VIC1ITCR                 (VIC1_BASE+0x300) /* R/W Test control register             */
#define VIC1ITIP1                (VIC1_BASE+0x304) /* R   Test input register (nVICIRQIN/nVICFIQIN)*/
#define VIC1ITIP2                (VIC1_BASE+0x308) /* R   Test input register (VICVECTADDRIN)      */
#define VIC1ITOP1                (VIC1_BASE+0x30C) /* R   Test output register (nVICIRQ/nVICFIQ)   */
#define VIC1ITOP2                (VIC1_BASE+0x310) /* R   Test output register (VICVECTADDROUT)    */
#define VIC1PERIPHID0            (VIC1_BASE+0xFE0) /* R   Peripheral ID register bits 7:0   */
#define VIC1PERIPHID1            (VIC1_BASE+0xFE4) /* R   Peripheral ID register bits 15:8  */
#define VIC1PERIPHID2            (VIC1_BASE+0xFE8) /* R   Peripheral ID register bits 23:16 */
#define VIC1PERIPHID3            (VIC1_BASE+0xFEC) /* R   Peripheral ID register bits 31:24 */


// APB_BASE
#define TIMERS_OFFSET           0x010000
#define TIMERS_BASE             (EP93XX_APB_BASE|TIMERS_OFFSET)

#define TIMER1LOAD              (TIMERS_BASE+0x00)
#define TIMER1VALUE             (TIMERS_BASE+0x04)
#define TIMER1CONTROL           (TIMERS_BASE+0x08)
#define TIMER1CLEAR             (TIMERS_BASE+0x0C)
#define TIMER1TEST              (TIMERS_BASE+0x10)

#define TIMER2LOAD              (TIMERS_BASE+0x20)
#define TIMER2VALUE             (TIMERS_BASE+0x24)
#define TIMER2CONTROL           (TIMERS_BASE+0x28)
#define TIMER2CLEAR             (TIMERS_BASE+0x2C)
#define TIMER2TEST              (TIMERS_BASE+0x30)

#define TIMER3LOAD              (TIMERS_BASE+0x80)
#define TIMER3VALUE             (TIMERS_BASE+0x84)
#define TIMER3CONTROL           (TIMERS_BASE+0x88)
#define TIMER3CLEAR             (TIMERS_BASE+0x8C)
#define TIMER3TEST              (TIMERS_BASE+0x90)

#define TTIMERBZCONT            (TIMERS_BASE+0x40)

#define TIMER4VALUELOW          (TIMERS_BASE+0x60)
#define TIMER4VALUEHIGH         (TIMERS_BASE+0x64)


#define IO_DWORD(offset)	(*(volatile unsigned long *)(offset))

//-----------------------------------------------------------------------------
// UART1 Register
//-----------------------------------------------------------------------------
#define UART1_OFFSET            0x0C0000
#define UART1_BASE              (EP93XX_APB_BASE|UART1_OFFSET)
#define UART1DR                 (UART1_BASE+0x000)
#define UART1RSR                (UART1_BASE+0x004)
#define UART1ECR                (UART1_BASE+0x004)
#define UART1CR_H               (UART1_BASE+0x008)
#define UART1CR_M               (UART1_BASE+0x00C)
#define UART1CR_L               (UART1_BASE+0x010)
#define UART1CR                 (UART1_BASE+0x014)
#define UART1FR                 (UART1_BASE+0x018)
#define UART1IIR                (UART1_BASE+0x01C)
#define UART1ICR                (UART1_BASE+0x01C)
#define UART1ILPR               (UART1_BASE+0x020)
#define UART1DMACR              (UART1_BASE+0x028)
#define UART1TMR                (UART1_BASE+0x084)
#define UART1MCR                (UART1_BASE+0x100)
#define UART1MSR                (UART1_BASE+0x104)
#define UART1TCR                (UART1_BASE+0x108)
#define UART1TISR               (UART1_BASE+0x10C)
#define UART1TOCR               (UART1_BASE+0x110)
#define HDLC1CR                 (UART1_BASE+0x20c)
#define HDLC1AMV                (UART1_BASE+0x210)
#define HDLC1AMSK               (UART1_BASE+0x214)
#define HDLC1RIB                (UART1_BASE+0x218)
#define HDLC1SR                 (UART1_BASE+0x21c)

//-----------------------------------------------------------------------------
// Bits in UARTFR - UART1FR, UART2FR, or UART3FR
//-----------------------------------------------------------------------------
#define UARTFR_RSR_ERRORS       0x0000000F
#define UARTFR_CTS              0x00000001
#define UARTFR_DSR              0x00000002
#define UARTFR_DCD              0x00000004
#define UARTFR_BUSY             0x00000008
#define UARTFR_RXFE             0x00000010
#define UARTFR_TXFF             0x00000020
#define UARTFR_RXFF             0x00000040
#define UARTFR_TXFE             0x00000080



#endif // _REGMAP_H



