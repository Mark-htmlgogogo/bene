#include <stdlib.h>
#include <stdio.h>
#include <queue>

#include "type.h"
#include "read_files.h"

class greedySolver
{
private:
    varset_t*   child_parent; /* solution, [i]: parents for var i, represented in binary */
    int**       tranclosure; /* transitive closure for solution graph */
    int         unparented_vs; /* set of unparented vars, represented in binary  */
    int         max_nof_ps;
    score_t**   i_vs_scores; /* [i][vs]: score(i|vs), vs in binary */
    std::priority_queue<score_cp_pair>   candidate_queue;

public:
    greedySolver(int nof_vars, char* dirname);

    /* read result from res files, like 0, 1, 2 */
    void read_resfile(int nof_vars, char* dirname);

    /* get the best parent set in i_vs_scores[i] */
    varset_t get_maxP_for_single_var(var_t v);

    /* check the acyclicity after adding v<-vs to the solution graph */
    bool acyclicity_check(int nof_vars, var_t v, varset_t vs);

    void delete_var_buffer(var_t i);

    void add_child_parent(var_t i, varset_t vs);
    
};
