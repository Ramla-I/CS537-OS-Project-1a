#include <stdlib.h> // atoi
#include<sys/types.h> // O_RDONLY...
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h> //getopt
#include<string.h> //strdup
#include<stdio.h> //stderr
#include "sort.h"


void
usage(char *prog) 
{
    fprintf(stderr, "usage: %s <-i input file> <-o output file>\n", prog);
    exit(1);
}

int compare(const void* p1, const void* p2)
{
	//return <0 if p1 before p2
	//return 0 if equal
	// return >0 if p1 after p2
	unsigned int key1 = *(unsigned int*)p1;
	unsigned int key2 = *(unsigned int*)p2;
	
	if(key1 < key2)
		return -1;
	else if(key1 > key2)
		return 1;
	else
		return 0; 
}

int main (int argc, char* argv[])
{
	char *outFile   = "/no/such/file";
	char *inFile   = "/no/such/file";
	int c;
	opterr = 0;
	
	//checking if arguments are the right amount
	/*if(argc != 5)
	{
		usage(argv[0]);
	}*/
	int num = argc/2;
	int j;

	for(j = 2; j < num+1; j++)
{
    	/*while ((c = getopt(argc, argv, "i:o:")) != -1) {
		switch (c) {
		case 'i':
	    	inFile = strdup(optarg);
	    	break;
		case 'o':
	    	outFile     = strdup(optarg);
	    	break;
		default:
	    	usage(argv[0]);
		}	
    	}*/
	
	

	
	inFile = strdup(argv[j]);
	outFile = strdup(argv[j+num]);

	
	
	

	
 	int inf = open(inFile, O_RDONLY);
    	if (inf < 0) {
		fprintf(stderr, "Error: Cannot open file %s\n", inFile);
		exit(1);
	}

 	int outf = open(outFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    	if (outf < 0) {
		fprintf(stderr, "Error: Cannot open file %s\n", outFile);
		exit(1);
	}
	//find size
	int f;
	struct stat buffer;
	int size;
	f = fstat(inf, &buffer);
	if(f<0)
	{
 		fprintf(stderr, "fstat error\n");
		exit(1);
	}
	size = buffer.st_size;
	//printf("%d \n", size);

	//create rec_t array according to size
	rec_t* data;
	data = (rec_t*) malloc(size);

	//store values in rec_t array
	int read_size;
	read_size = read(inf, data, size);
	if(read_size <0)
	{
		fprintf(stderr, "input file read problem\n");
		exit(1);
	}
	//printf("%d \n",read_size);


	//compare using keys
	int num_records = size/sizeof(rec_t);
	qsort(data, num_records, sizeof(rec_t), compare);

	//save to out put file
	rec_t r;
   	int i;
    	for (i = 0; i < num_records; i++) {
		
		r.key = data[i].key;
		
		int j;
		for (j = 0; j < NUMRECS; j++) {
	    	r.record[j] = data[i].record[j];
		}

		int rc = write(outf, &r, sizeof(rec_t));
		if (rc != sizeof(rec_t)) {
	    		fprintf(stderr, "output file write problem\n");
	    		exit(1);
	    	}
    	}

	//closing files
	int oc = close(outf);
	if(oc<0)
	{
		fprintf(stderr, "output file close problem\n");
	    	exit(1);
	}

	int ic = close(inf);
	if(ic<0)
	{
		fprintf(stderr, "input file close problem\n");
	    	exit(1);
	}
	
}
}

