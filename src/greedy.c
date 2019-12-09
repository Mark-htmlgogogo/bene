#include "greedy.h"

varset_t*   child_parent; /* [i]: parents for var i, represented in binary */
int**       tranclosure; /* transitive closure for solution graph */
int         unparented; /* set of unparented vars  */
score_t**   scores_buffer; /* [i][vs]: score(i|vs), vs in binary */
score_t**   candidate_queue;



void init_globals(){

}

/* read res file into scores_buffer */
void read_resfiles(int nof_vars, char* dirname){

}

/* find the best parent set for var i in scores_buffer[i] then store in */
void find_maxparent_for_single_var(int i){
    
}

void 

void reverse_files(int nof_vars, char* dirname) {
  varset_t nof_parsets = 1U<<(nof_vars-1);
  score_t* buffer = malloc(nof_parsets*sizeof(score_t));

  int v;
  for(v=0; v<nof_vars; ++v){

    {
      FILE* fin  = open_file(dirname, v, ".slt", "rb");
      FREAD(buffer, sizeof(score_t), nof_parsets, fin);
      fclose(fin);
    }

    {
      FILE* fout = open_file(dirname, v, "", "wb");
      varset_t ps = nof_parsets;
      do {
	fwrite(buffer + (--ps), sizeof(score_t), 1, fout);
      } while(ps);
      
      fclose(fout);
    }
  }
  
  free(buffer);

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
