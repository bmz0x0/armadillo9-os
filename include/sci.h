
#ifndef _SCI_H
#define _SCI_H

int sci_setup(void);
void sci_write_char(char c);
void sci_write_char_nchk(char c);
void sci_write_hex_long(unsigned long l);
int sci_write(char *s);

#endif // _SCI_H


