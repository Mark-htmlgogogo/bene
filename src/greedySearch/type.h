#ifndef TYPE_HEAD
#define TYPE_HEAD

#include <bits/stdc++.h>

#define SINGLETON(i) (((varset_t)1) << i) /* = 2^i */

typedef     double          score_t;
typedef     unsigned int    varset_t;
typedef     unsigned int    var_t;

typedef     std::pair<var_t, varset_t> cp_pair;
typedef     std::pair<score_t, cp_pair> score_cp_pair;

#endif