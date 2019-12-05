
#include <time.h>
#include <regmap.h>
#include <io.h>
#include <sci.h>

volatile u64 gtime;

void timer_init()
{
	u32	cpsr;

	outl(0, TIMER1CONTROL);
	outl(LATCH - 1, TIMER1LOAD);
	outl(0xc8, TIMER1CONTROL);
	outl(1, TIMER1CLEAR);

	gtime = 0;

	asm( "mrs	%0, cpsr" : "=r"(cpsr) : );
	cpsr &= ~(0x80);

	asm( "msr	cpsr_c, %0" :: "r"(cpsr) );

	outl( (1 << 4), VIC0INTENABLE );
}

void timer_interrupt()
{
	outl(1, TIMER1CLEAR);

	// time tick
	gtime++;
}




