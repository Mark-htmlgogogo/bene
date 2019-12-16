#include "cfg.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "files.h"
#include "varpar.h"

score_t score_net(char* netfile, char* dirprefix){
	score_t score = 0.0;

	int v;
	varset_t varset;
	FILE* netf = strcmp(netfile,"-") ? fopen(netfile, "rt") : stdin;
	for(v=0; 1 == fscanf(netf, "%"VARSET_SCNFMT, &varset); ++v){
		score_t vscore;
		FILE* fin = open_file(dirprefix, v, "", "rb");
		fseek(fin, varset2parset(v, varset)*sizeof(score_t), SEEK_SET);
		FREAD(&vscore, sizeof(score_t), 1, fin);
		fclose(fin);
		score += vscore;
	}
	fclose(netf);
	return score;
}

int main(int argc, char* argv[])
{

	clock_t t = clock();	
	if (argc!=3) {
		fprintf(stderr, "Usage: score_net netfile dirname\n");
		return 1;
	}

	score_t score;
	score = score_net(argv[1], argv[2]);

	t = clock() - t;

	double elasped_seconds = ((double)t)/CLOCKS_PER_SEC;
	printf("score_net elapsed time: %f s\n", elasped_seconds);

		printf("%.3f\n", score);

	return 0;
}
