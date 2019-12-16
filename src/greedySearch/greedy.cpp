#include "greedy.h"

using namespace std;

greedySolver::greedySolver(int _nof_vars, const char* dirname){
	nof_vars = _nof_vars;

	unparented_vs = (1U<<(nof_vars))-1;

	solution_score = 0;

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

/* the best ps for var v, not the best vs! */
score_cp_pair greedySolver::get_max_scp_scorebuffer(var_t v){
	varset_t best_ps = 0;
	for (varset_t ps = 1; (int)ps < max_nof_ps; ps++)
	{
		if(scores_buffer[v][ps] > scores_buffer[v][best_ps]){
			best_ps = ps;
		}
	}
	/* make score_cp_pair: (s, (c,p)) */
	cp_pair cp = make_pair(v, best_ps);
	score_cp_pair scp = make_pair(scores_buffer[v][best_ps], cp);
	
	/* set selected one to -∞, such that it wouldn't be considered next time */
	scores_buffer[v][best_ps] = -DBL_MAX;

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

// void greedySolver::insert_cp_tranclosure(var_t v, varset_t vs, bool** t){
// 	varset_t dv = get_descendants_v(v, t);

// 	for (var_t vsi = 0; ((int)vsi < nof_vars) && (vs&SINGLETON(vsi)); ++vsi){/* for each var in vs: vsi */
// 		for (var_t pvsi = 0; ((int)pvsi < nof_vars) && t[pvsi][vsi]; ++pvsi){/* for each var preceding vsi: pvsi */
// 			for (var_t dvi = 0; (int)dvi < nof_vars && (dv&SINGLETON(dvi)); ++dvi){/* for each var in v's descendants: dvi */
// 				t[pvsi][dvi] = true;
// 			}
// 		}
// 	}
// 	print_tranclosure(t, nof_vars);
// }

void greedySolver::insert_cp_tranclosure(var_t v, varset_t vs, bool** t){
	varset_t dv = get_descendants_v(v, t);

	//print_tranclosure(t, nof_vars);

	for (var_t vsi = 0; (int)vsi < nof_vars; ++vsi)
	{
		if (vs&SINGLETON(vsi))
		{/* for each var in vs: vsi */
			for (var_t pvsi = 0; (int)pvsi < nof_vars; ++pvsi)
			{
				if (t[pvsi][vsi])
				{/* for each var preceding vsi: pvsi */
					for (var_t dvi = 0; (int)dvi < nof_vars; ++dvi)
					{
						if (dv&SINGLETON(dvi))
						{/* for each var in v's descendants: dvi */
							t[pvsi][dvi] = true;
						}
					}
				}
			}
		}
	}

	//print_tranclosure(t, nof_vars);
}

bool greedySolver::is_acyclic(bool** t){
	
	//print_tranclosure(t, nof_vars);
	for (var_t v = 0; (int)v < nof_vars; ++v)
	{
		for (var_t u = 0; u < v; ++u)
		{
			if (t[v][u] == true && t[u][v] == true)
			{
				return false;
			}
		}
	}
	return true;
}

void greedySolver::greedy_search(){
	score_cp_pair scp;
	var_t topv = 0;
	varset_t topvs = -1;

	print_socresbuffer(scores_buffer, nof_vars, max_nof_ps);
	/* init the queue: get the largest vs in each scores_buffer[] and push it to the priority queue*/
	for (var_t v = 0; (int)v < nof_vars && unparented_vs&SINGLETON(v); v++)
	{
		scp = get_max_scp_scorebuffer(v);
		candidate_queue.push(scp);
	}

	while (unparented_vs)
	{
		/* dump the top one in queue */
		score_cp_pair top =  candidate_queue.top();
		topv = top.second.first;
		topvs = parset2varset(topv, top.second.second);
		candidate_queue.pop();

		/* insert top to tranclosure_copy */
		insert_cp_tranclosure(topv, topvs, tranclosure_copy);

		/* acyclicity check */
		if (is_acyclic(tranclosure_copy))
		{
			/* tranclosure <- tranclosure_copy */
			//print_tranclosure(tranclosure_copy, nof_vars);
			for (int i = 0; i < nof_vars; i++)
			{
				memcpy(*(tranclosure+i), *(tranclosure_copy+i), nof_vars*sizeof(bool));
			}
			//print_tranclosure(tranclosure, nof_vars);

			/* update unparented_vs: delete added var from it */
			unparented_vs ^= SINGLETON(topv);

			/* update solution */
			solution_cp[topv] = topvs;
			solution_score += top.first;
			//printf("scores_buffer[%d][%d]: %lf\n", topv, top.second.second, top.first);
		}
		else
		{
			/* tranclosure_copy <- tranclosure */
			//print_tranclosure(tranclosure, nof_vars);
			for (int i = 0; i < nof_vars; i++)
			{
				memcpy(*(tranclosure_copy+i), *(tranclosure+i), nof_vars*sizeof(bool));
			}
			//print_tranclosure(tranclosure_copy, nof_vars);

			/* dump the new largest one from scores_queque[topv] and add it to the queue*/
			scp = get_max_scp_scorebuffer(topv);
			candidate_queue.push(scp);
		}
	}
}

varset_t parset2varset(var_t v, varset_t set){
	varset_t singleton = 1U<<v;
	varset_t mask      = singleton - 1;
	varset_t low       = set &  mask;
	varset_t high      = set & ~mask;
	return (high<<1) | low;	
}

void print_solution_graph(varset_t* _solution_cp, int nof_vars){
	for (var_t v = 0; (int)v < nof_vars; ++v)
	{
		cout << v << " :\t";
		for (var_t u = 0; (int)u < nof_vars; ++u)
		{
			if(_solution_cp[v]&SINGLETON(u)){
				cout << u << "\t";
			}
		}
		cout << endl;
	}
}

void print_tranclosure(bool** t, int n){
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cout << t[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void print_socresbuffer(score_t** s, int nof_vars, int max_nof_ps){
	varset_t vs;
	for (int v = 0; v < nof_vars; ++v)
	{
		for (int ps = 0; ps < max_nof_ps; ++ps)
		{
			vs = parset2varset(v, ps);
			//printf("s[%d][%d](%d): %.10lf\n", v, ps, vs, s[v][ps]);
		}
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

	auto start = chrono::system_clock::now();

	greedySolver bn_solver = greedySolver(nof_vars, dirname);

	bn_solver.read_resfile(dirname);

	bn_solver.greedy_search();

	auto end = chrono::system_clock::now();

	chrono::duration<double> elapsed_seconds = end-start;
    time_t end_time = chrono::system_clock::to_time_t(end);

    cout << "finished computation at " << ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";

	printf("%.3f\n", bn_solver.get_solution_score());

	//print_solution_graph(bn_solver.get_solution_cp(), nof_vars);

	return 0;
}