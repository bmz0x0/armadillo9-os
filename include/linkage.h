
#ifndef _LINKAGE_H
#define _LINKAGE_H

#ifndef __ALIGN
#define __ALIGN		.align 4,0x90
#define __ALIGN_STR	".align 4,0x90"
#endif

#ifdef __ASSEMBLY__

#define ALIGN __ALIGN
#define ALIGN_STR __ALIGN_STR

#define ENTRY(name) \
  .globl name; \
  ALIGN; \
  name:

#endif


#endif // _LINKAGE_H
