#include "greedy.h"

greedySolver::greedySolver(int nof_vars, char* dirname){
		child_parent = new varset_t[nof_vars];
		
		tranclosure =  new int*[nof_vars];
		for (int i = 0; i < nof_vars; i++)
		{
			tranclosure[i] = new int[nof_vars];
		}

		unparented_vs = 1U<<(nof_vars)-1;

		max_nof_ps = 1U<<(nof_vars-1);

		i_vs_scores = new score_t*[nof_vars];
		for (int i = 0; i < nof_vars ; i++)
		{
			i_vs_scores[i] = new score_t[max_nof_ps];
		}
}

void greedySolver::read_resfile(int nof_vars, char* dirname){
	for (int i = 0; i < nof_vars; i++)
	{
		FILE* fin = open_file(dirname, i, "", "rb");
		FREAD(i_vs_scores[i], sizeof(score_t), max_nof_ps, fin);
		fclose(fin);
	}
}


varset_t greedySolver::get_maxP_for_single_var(var_t v){
	varset_t best_index = 0;
	for (int i = 1; i < max_nof_ps; i++)
	{
		if(i_vs_scores[v][i] > i_vs_scores[v][best_index]){
			best_index = i;
		}
	}
	
	i_vs_scores[v][best_index] = -DBL_MAX;
}


bool greedySolver::acyclicity_check(int nof_vars, var_t v, varset_t vs){
	varset_t descendants_v = 0;
	for (int out = 0; out < nof_vars; ++out){
		if (tranclosure[v][out])
		{
			descendants_v |= (1U)<<out;
		}
	}

	for (var_t vsi = 0; vsi < nof_vars && vs&SINGLETON(vsi); ++vsi){
		for (var_t pvsi = 0; pvsi < nof_vars && tranclosure[pvsi][vsi] == 1; ++pvsi)
		{
			for (var_t dvi = 0; dvi < nof_vars && descendants_v&SINGLETON(dvi); ++dvi)
			{
				
			}
			
			tranclosure[pvsi][]
		}
		
	}
}

int main(int argc, char const *argv[])
{
	if (argc!=3) {
		fprintf(stderr, "Usage: split_local_scores nof_vars dirname\n");
		return 1;
	}

	int nof_vars = atoi(argv[1]);
	(nof_vars, argv[2]);

	return 0;
}
