#include <stdlib.h>
#include <string.h>
#include "xtab.h"


xentry* xadd(xtab* t, uint h, uchar* key, int klen, int* new) {

  /* starting from ix[h], go through x:s and try to find key */

  xentry* x = t->ix[h]; /* header of the linked list for hashkey h */
  xentry** p = t->ix + h; /* prev-pointer to chain the possibly new xentry */
  
  for(; x && memcmp(x->key, key, klen); x = x->next){ 
  /* if x is not NULL and the key of x 
  is different with the data in the new line */
    p = &(x->next); /* if x == NULL, *p is the last node in the linked list */
  }

  /* either x == NULL or
   x != NULL && x->key == key */
  *new = !x; 
  if(*new){ /* new = 1 iff x == NULL and we're at the last  of linked list */
    x = (t->free)++; /* allocate memory to x, which is NULL before allocated*/
    x->h = h; /* set the hashkey of x as h */
    *p = x; /* link the new x */
  }

  return x;
}

/* Returns an xtab* with parameters */
xtab* xcreate(int range, int nof_xentries){
  printf("xcreate(%d, %d)\n", range, nof_xentries);
  xtab* t     = calloc(1,sizeof(xtab));
  t->range    = range;
  t->ix       = calloc(range, sizeof(xentry*));
  t->xentries = calloc(nof_xentries, sizeof(xentry));
  t->free     = t->xentries;
  return t;
}

void  xdestroy(xtab* t) {
  free(t->ix);
  free(t->xentries);
  free(t);
}

