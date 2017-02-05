#ifndef INFO_H
#define INFO_H
#include <stdlib.h>
#include <stdio.h>

#ifdef INFO
	#define info(alphabet,amount,input,output,operation) 	fprintf(stderr,"\nINFO: shifted aplhabet: %s\nINFO: shift amount: %s\nINFO: input file: %s\nINFO: output file: %s\nINFO: operation: %s\n",Alphabet,amount,input,output,operation)
#else
	#define info(alphabet,amount,input,output,operation)
#endif

#endif /* DINFO_H */
