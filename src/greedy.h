#include <stdlib.h>
#include <stdio.h>

#include "cfg.h"
#include "priority_queue.c"

/* ddddddd */
void init_globals();

/* find the max scored parent set for i */
varset_t find_max(int i);