#ifndef _INIT_H
#define _INIT_H

#define __init		__attribute__ ((__section__ (".init.text")))
#define __initdata	__attribute__ ((__section__ (".init.data")))

#endif // _INIT_H
