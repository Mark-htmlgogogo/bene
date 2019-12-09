#include <stdlib.h>

#include "cfg.h"

varset_t*   child_parent; /* [i]: parents for var i, represented in binary */
int**       tranclosure; /* transitive closure for solution graph */
int         unparented; /* set of unparented vars  */
