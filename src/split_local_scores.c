#include "cfg.h"

#define DO_NOT_DEFINE_LARGEFILE64_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "files.h"


void split_local_scores(int nof_vars, char* dirname) {

	//printf("split_local_scores(%d, %s)", nof_vars, dirname);

	FILE** files = open_files(nof_vars, dirname, ".slt", "wb");

	char* fn = malloc((strlen(dirname)+1+3+1)*sizeof(char));
	FILE* fin;

	fn = strcat(strcpy(fn, dirname), "/res");
	fin= fopen(fn, "rb");

	free(fn);

	{
		varset_t varset;
		
		for(varset=LARGEST_SET(nof_vars); varset; --varset){
		int v;
		for(v=0; v<nof_vars; ++v){
			if (varset & (1U<<v)){
			score_t d;
			FREAD(&d, sizeof(score_t), 1, fin);
			fwrite(&d, sizeof(score_t), 1, files[v]);
			}
		}
		}
	}
	free_files(nof_vars, files);
	fclose(fin);
}


int main(int argc, char* argv[])
{
	clock_t t = clock();

	if (argc!=3) {
		fprintf(stderr, "Usage: split_local_scores nof_vars dirname\n");
		return 1;
	}

	{
		int nof_vars = atoi(argv[1]);
		split_local_scores(nof_vars, argv[2]);
	}

	t = clock() - t;

	double elasped_seconds = ((double)t)/CLOCKS_PER_SEC;
	printf("split_local_scores elapsed time: %f s\n", elasped_seconds);

	return 0;
}
