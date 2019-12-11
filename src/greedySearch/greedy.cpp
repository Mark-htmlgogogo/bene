#include "greedy.h"

using namespace std;

greedySolver::greedySolver(int _nof_vars, const char* dirname){
	nof_vars = _nof_vars;

	unparented_vs = 1U<<(nof_vars)-1;

	solution_cp = new varset_t[nof_vars];
	
	tranclosure =  new bool*[nof_vars];
	for (int i = 0; i < nof_vars; i++)
	{
		tranclosure[i] = new bool[nof_vars];
		tranclosure[i][i] = true;
	}

	tranclosure_copy =  new bool*[nof_vars];
	for (int i = 0; i < nof_vars; i++)
	{
		tranclosure_copy[i] = new bool[nof_vars];
		tranclosure_copy[i][i] = true;
	}

	max_nof_ps = 1U<<(nof_vars-1);

	scores_buffer = new score_t*[nof_vars];
	for (int i = 0; i < nof_vars ; i++)
	{
		scores_buffer[i] = new score_t[max_nof_ps];
	}
}

void greedySolver::read_resfile(const char* dirname){
	for (int i = 0; i < nof_vars; i++)
	{
		FILE* fin = open_file(dirname, i, "", "rb");
		FREAD(scores_buffer[i], sizeof(score_t), max_nof_ps, fin);
		fclose(fin);
	}
}

score_cp_pair greedySolver::get_max_scp_scorebuffer(var_t v){
	varset_t best_vs = 0;
	for (varset_t i = 1; i < max_nof_ps; i++)
	{
		if(scores_buffer[v][i] > scores_buffer[v][best_vs]){
			best_vs = i;
		}
	}
	/* make score_cp_pair: (s, (c,p)) */
	cp_pair cp = make_pair(v, best_vs);
	score_cp_pair scp = make_pair(scores_buffer[v][best_vs], cp);
	
	/* set selected one to -∞, such that it wouldn't be considered next time */
	scores_buffer[v][best_vs] = -DBL_MAX;

	return scp;
}

varset_t greedySolver::get_descendants_v(var_t v, bool** t){
	varset_t vs = 0;
	for (int out = 0; out < nof_vars; ++out){
		if (t[v][out])
		{
			vs |= (1U)<<out;
		}
	}

	return vs;
}

void greedySolver::insert_cp_tranclosure(var_t v, varset_t vs, bool** t){
	varset_t descendants_v = get_descendants_v(v, t);

	for (var_t vsi = 0; vsi < nof_vars && vs&SINGLETON(vsi); ++vsi){
	/* for each var in vs: vsi */
		for (var_t pvsi = 0; pvsi < nof_vars && t[pvsi][vsi]; ++pvsi)
		/* for each var in vsi's parent: pvsi */
		{
			for (var_t dvi = 0; dvi < nof_vars && descendants_v&SINGLETON(dvi); ++dvi)
			{/* for each var in v's descendants: dvi */
				t[pvsi][dvi] = true;
			}
		}
	}
}

bool greedySolver::acyclicity_check(bool** t){
	return true;
}

void greedySolver::greedy_search(){
	score_cp_pair scp;
	var_t topv = 0;
	varset_t topvs = -1;

	/* init the queue: get the largest vs in each scores_buffer[] and push it to the priority queue*/
	for (var_t i = 0; i < nof_vars && unparented_vs&SINGLETON(i); i++)
	{
		scp = get_max_scp_scorebuffer(i);
		candidate_queue.push(scp);
	}

	while (unparented_vs)
	{
		/* dump the top one in queue */
		score_cp_pair top =  candidate_queue.top();
		topv = top.second.first;
		topvs = top.second.second;
		candidate_queue.pop();

		/* insert top to tranclosure_copy */
		insert_cp_tranclosure(topv, topvs, tranclosure_copy);

		/* acyclicity check */
		if (acyclicity_check(tranclosure_copy))
		{	
			/* tranclosure <- tranclosure_copy */
			for (int i = 0; i < nof_vars; i++)
			{
				memcpy(tranclosure_copy+i, tranclosure+i, nof_vars*sizeof(bool));
			}

			/* update unparented_vs: delete added var from it */
			unparented_vs ^= SINGLETON(topv);

			/* update solution */
			solution_cp[topv] = topvs;
		}
		else
		{
			/* tranclosure_copy <- tranclosure */
			for (int i = 0; i < nof_vars; i++)
			{
				memcpy(tranclosure+i, tranclosure_copy+i, nof_vars*sizeof(bool));
			}

			/* dump the new largest one from scores_queque[topv] and add it to the queue*/
			scp = get_max_scp_scorebuffer(topv);
			candidate_queue.push(scp);
		}
	}
}		

void print_result(varset_t* s, int nof_vars){
	for (var_t v = 0; v < nof_vars; ++v)
	{
		cout << v << " :\t";
		for (var_t u = 0; u < nof_vars; ++u)
		{
			if(s[v]&SINGLETON(v)){
				cout << v << "\t";
			}
		}
		cout << endl;
	}
}

int main(int argc, char const *argv[])
{
	if (argc!=3) {
		fprintf(stderr, "Usage: greedy nof_vars dirname\n");
		return 1;
	}

	int nof_vars = atoi(argv[1]);
	const char * dirname = argv[2];

	greedySolver bn_solver = greedySolver(nof_vars, dirname);

	bn_solver.read_resfile(dirname);

	bn_solver.greedy_search();

	print_result(bn_solver.get_solution(), nof_vars);

	return 0;
}