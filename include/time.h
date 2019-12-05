
#ifndef _TIME_H
#define _TIME_H

#define CLOCK_SOURCE_RATE 14745600
#define CLOCK_TICK_DIV 29

#define CLOCK_TICK_RATE \
        (((CLOCK_SOURCE_RATE+(CLOCK_TICK_DIV*HZ-1))/(CLOCK_TICK_DIV*HZ))*HZ)
#define CLOCK_RATE_DIFF (CLOCK_TICK_RATE*CLOCK_TICK_DIV-CLOCK_SOURCE_RATE)
#define CLOCK_CORRECT \
        ((CLOCK_TICK_RATE*CLOCK_TICK_DIV+CLOCK_RATE_DIFF/2)/CLOCK_RATE_DIFF)

#define HZ		100		/* Internal kernel timer frequency */

#define LATCH  ((CLOCK_TICK_RATE + HZ/2) / HZ)	/* For divider */

void timer_init(void);
void timer_interrupt(void);

#endif // _TIME_H


