
#ifndef _IO_H
#define _IO_H

#include "type.h"


#define __raw_writel(v,a)	(*(volatile unsigned int *)(a) = (v))

#define __io(a)			((unsigned long)(a))

#define outl(v,p)		__raw_writel((u32)v,__io(p))



#endif // _IO_H

