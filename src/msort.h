#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <locale.h>

#define STRSIZE 1024
#define BUFSIZE 512

typedef char stringType[STRSIZE];

typedef stringType bufType[BUFSIZE];

typedef struct mergeLine{
    bufType line;
    int nlines;
    int morelines;
} mergeLine;

typedef struct key {
    char line[8];
    int colstart;
    int startfrac;
    int colend;
    int endfrac;
} key;

extern int optind;

extern char *optarg;

int bflag, dflag, fflag, iflag, kflag, mflag, nflag, oflag, rflag, tflag, uflag, err;

key key1;
char *outFilename = "output", *sep = " ";

void getkey(key *k);
int getpos(char *haystack, char *needle, int count);
double createNumberfromString(char *a, int i, int j);

int fillBuffer(FILE *datafile, bufType buffer);
int makeSortedRuns(FILE *datafile);

void merge(FILE *f1, FILE *f2, FILE *f3);
void handleMerges(int numFiles, char x);
void handleOnlyMerges(int numFiles, char *mergefiles[16]);

int strcompare(const void * a, const void * b);
int strcomparelesskey(const void * a, const void * b );
int numcompare(const void * a, const void * b);