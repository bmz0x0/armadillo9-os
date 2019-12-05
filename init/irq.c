
#include <irq.h>
#include <cpu.h>
#include <time.h>
#include <regmap.h>
#include <sci.h>

void asm_do_IRQ(unsigned int irq, struct pt_regs *regs)
{
	if ( IRQ_TIMER1 == irq ) {
		timer_interrupt();
	}
}



