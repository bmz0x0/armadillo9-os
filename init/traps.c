
#include <sci.h>

void __div0(void)
{
	sci_write("Division by zero in kernel.\n");
	while(1);
}
