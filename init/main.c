
#include <main.h>
#include <type.h>
#include <sci.h>
#include <time.h>
#include <setup.h>
#include <printk.h>

extern volatile u64 gtime;

unsigned long kernel_stack[STACK_SIZE/sizeof(long)]
	__attribute__((__section__(".init.task")));

void __init start_kernel(void)
{
	u64	temp;

	sci_setup();

	sci_write( "\nKernel Start.\n" );

	setup_arch();

	printf( "timer_init start.\n" );

	timer_init();

	printf( "timer_init end.\n" );

	temp = 0;

	while (1) {
		if ( (temp+100) <  gtime ) {
			printf( "Hello World\n" );
			temp = gtime;
		}
	}

	while (1);
}

