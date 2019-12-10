#include <float.h>

#define SINGLETON(i) (((varset_t)1) << i) /* = 2^i */

typedef     double          score_t;
typedef     unsigned int    varset_t;
typedef     unsigned int    var_t;

typedef     std::pair<var_t, varset_t> child_parent_pair;
typedef     std::pair<score_t, child_parent_pair> score_cp_pair;