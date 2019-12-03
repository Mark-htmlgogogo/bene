#ifndef _XTAB_H_
#define _XTAB_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct xentry xentry;
typedef unsigned char uchar;
typedef unsigned int uint;

struct xentry {
  uchar*  key;   /* points to a distinct row of data in .iris file, such as "1 1 2 2 2" */
  int*    val;   /* counts the frequency of the key */ 
  xentry* next;  /* points to the next distinct xentry */
  uint    h;     /* h denotes hash key */
};

typedef struct xtab xtab;

struct xtab {
  int range;        /* range of hash keys, for example 1024 */
  xentry** ix;      /* hashkey to xentry pointer mapping    */
  xentry* free;     /* pointer to unused xentries           */
  xentry* xentries;
};


#define xcount(t) ((t)->free - (t)->xentries)

# define xreset(t) \
{\
  memset((t)->ix, 0, (t)->range * sizeof(xentry*));\
  memset((t)->xentries, 0, xcount(t) * sizeof(xentry));\
  (t)->free = (t)->xentries;\
}

extern xentry* xadd(xtab* t, uint h, uchar* key, int klen, int* new) ;
extern xtab* xcreate(int range, int nof_xentries);
extern void  xdestroy(xtab* t);

#endif
