#ifndef GREEDY_HEAD
#define GREEDY_HEAD

#include "type.h"
#include "read_files.h"

class greedySolver
{
private:
    int         nof_vars;
    int         unparented_vs; /* set of unparented vars, represented in binary  */
    varset_t*   solution_cp; /* solution, [i]: parents for var i, represented in binary */
    bool**      tranclosure; /* transitive closure for solution graph */
    bool**      tranclosure_copy; 
    int         max_nof_ps;
    score_t**   scores_buffer; /* [i][vs]: score(i|vs), vs in binary */
    std::priority_queue<score_cp_pair>   candidate_queue;

    /* get the best parent set in i_vs_scores[i] */
    score_cp_pair get_max_scp_scorebuffer(var_t v);

    varset_t get_descendants_v(var_t v, bool** t);

    void insert_cp_tranclosure(var_t v, varset_t vs, bool** t);

    /* check the acyclicity of tranclosure t */
    bool acyclicity_check(bool** t);

public:

    greedySolver(int _nof_vars, const char* dirname);

    /* Accessors */
    varset_t* get_solution() {return solution_cp;}
    

    /* read res files into scores_buffer, like 0, 1, 2 */
    void read_resfile(const char* dirname);

    /* main part of our greedy algorithm */
    void greedy_search();
};

void print_result(varset_t* s, int nof_vars);

#endif