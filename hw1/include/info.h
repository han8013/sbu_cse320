#ifndef INFO_H
#define INFO_H
#include <stdlib.h>
#include <stdio.h>

#ifdef INFO
	#define info(fmt) fprintf(stderr,"INFO--: %s",fmt)
#else
	#define info(fmt)
#endif

#endif /* DINFO_H */
