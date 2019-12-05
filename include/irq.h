
#ifndef _IRQ_H
#define _IRQ_H


#define IRQ_INT1_BASE   0

#define IRQ_RFU0	0
#define IRQ_RFU1	1
#define IRQ_COMMRX	2
#define IRQ_COMMTX	3
#define IRQ_TIMER1	4
#define IRQ_TIMER2	5
#define IRQ_AAC         6
#define IRQ_DMAM2P0	7
#define IRQ_DMAM2P1	8
#define IRQ_DMAM2P2     9
#define IRQ_DMAM2P3	10
#define IRQ_DMAM2P4	11
#define IRQ_DMAM2P5	12
#define IRQ_DMAM2P6	13
#define IRQ_DMAM2P7	14
#define IRQ_DMAM2P8	15
#define IRQ_DMAM2P9     16
#define IRQ_DMAM2M0	17
#define IRQ_DMAM2M1	18
#define IRQ_GPIO0	19
#define IRQ_GPIO1	20
#define IRQ_GPIO2	21
#define IRQ_GPIO3	22
#define IRQ_UARTRX1	23
#define IRQ_UARTTX1	24
#define IRQ_UARTRX2	25
#define IRQ_UARTTX2	26
#define IRQ_UARTRX3	27
#define IRQ_UARTTX3	28
#define IRQ_KEY	        29
#define IRQ_TOUCH	30
#define IRQ_GRAPHICS	31

#define INT1_IRQS       0xfffffffc

#define NR_IRQS_INT1    32

#define IRQ_INT2_BASE   NR_IRQS_INT1

#define IRQ_EXT0	32
#define IRQ_EXT1	33
#define IRQ_EXT2	34
#define IRQ_64HZ	35
#define IRQ_WEINT	36
#define IRQ_RTC 	37
#define IRQ_IRDA	38
#define IRQ_MAC 	39
#define IRQ_EXT3	40
#define IRQ_EIDE	IRQ_EXT3
#define IRQ_PROG	41
#define IRQ_1HZ 	42
#define IRQ_VSYNC	43
#define IRQ_VIDEOFIFO	44
#define IRQ_SSPRX	45
#define IRQ_SSPTX	46
#define IRQ_GPIO4	47
#define IRQ_GPIO5	48
#define IRQ_GPIO6	49
#define IRQ_GPIO7	50
#define IRQ_TIMER3	51
#define IRQ_UART1	52
#define IRQ_SSP 	53
#define IRQ_UART2	54
#define IRQ_UART3	55
#define IRQ_USH 	56
#define IRQ_PME 	57
#define IRQ_DSP 	58
#define IRQ_GPIO	59
#define IRQ_SAI		60
#define IRQ_RFU61	61
#define IRQ_RFU62	62
#define IRQ_RFU63	63

#define INT2_IRQS       0x1fffffff

#define NR_IRQS_INT2    64


#endif // _IRQ_H


