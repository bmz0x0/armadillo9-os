
#include "sci.h"
#include <regmap.h>

int sci_setup(void)
{
	while(IO_DWORD(UART1FR) & UARTFR_BUSY);

	IO_DWORD(UART1CR) = 0;
	IO_DWORD(UART1MCR) = 0;
	IO_DWORD(UART1RSR) = 0;
	IO_DWORD(UART1CR_M) = 0;
	IO_DWORD(UART1CR_L) = 7;		// 115200
	//IO_DWORD(UART1CR_L) = 15;		// 57600
	IO_DWORD(UART1CR_H) = 0x60;

	IO_DWORD(UART1CR) = 1;

	return 0;
}


void sci_write_char(char c)
{
	while (IO_DWORD(UART1FR) & UARTFR_TXFF);
	IO_DWORD(UART1DR) = c;
}

void sci_write_char_nchk(char c)
{
	IO_DWORD(UART1DR) = c;
}

void sci_write_hex_long(unsigned long l)
{
	int i;
	unsigned long tmp;

	for ( i = 28; i >= 0; i -= 4 ) {
		tmp = (l >> i) & 0xF;
		if ( tmp >= 0xA ) {
			sci_write_char( 'A' + tmp - 0xA );
		} else {
			sci_write_char( '0' + tmp);
		}
	}
}

int sci_write(char *s)
{
	int count;

	for (count=0; *s; ++count) {
		if (*s == '\n') {
			++count;
			sci_write_char('\r');
		}
		sci_write_char(*s++);
	}

	while (IO_DWORD(UART1FR) & UARTFR_TXFF);

	return count;
}

