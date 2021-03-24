#include "msort.h"

struct Options options;

/**
 * @brief Compare two strings based on keys pointed by void pointers
 *
 * Sort them according to global keys and return difference on first non-equal
 * character according to their ASCII value
 *
 * @param a First string
 * @param b Second string
 * @return int
 * 		   < 0 if a < b
 * 		   = 0 if a = b
 * 		   > 0 if a > b
 * @note Original strings are not modified
 */
int strcompare(const void * a, const void * b) {
    /* Typecasting void pointers to char pointers */
    char *s = (char *)a;
    char *t = (char *)b;
    char x, y;
    register int i, j, endpos1, endpos2;
    int length1 = strlen(s), length2= strlen(t);
    int s_gpstart, t_gpstart, s_gpend, t_gpend;
    i = j = 0;

    if(options.bflag) {
        /* Ignore leading white spaces */
        while(isspace(s[i]))
            i++;
        s = s + i;

        while(isspace(t[j]))
            j++;
        t = t + j;
        /* s and t now point to first non white space character */
    }
    endpos1 = length1;
    endpos2 = length2;

    if(options.kflag) {
        /* Get start and end indices based on keys */
        s_gpstart = getpos(s, sep, key1.colstart);
        t_gpstart = getpos(t, sep, key1.colstart);
        s_gpend   = getpos(s, sep, key1.colend + 1);
        t_gpend   = getpos(t, sep, key1.colend + 1);

        if(key1.colstart == 0 || key1.colend == 0) {
            fprintf(stderr,"Invalid field specification : %s\n", key1.line);
            exit(EINVAL);
        }

        i = s_gpstart + key1.startfrac;
        j = t_gpstart + key1.startfrac;

        if(key1.colstart == key1.colend) {
            /* Given keyfield is a single column */

            if(key1.startfrac == 0 && key1.endfrac == 0) {
                /* Example : sort -k2,2 */
                endpos1 = s_gpend + key1.endfrac;
                endpos2 = t_gpend + key1.endfrac;
            }
            else {
                /* Example : sort -k2.3,2.9 */
                endpos1 = getpos(s, sep, key1.colend) + key1.endfrac + 1;
                endpos2 = getpos(t, sep, key1.colend) + key1.endfrac + 1;
            }
        }
        else if(s_gpend == length1 && t_gpend == length2) {
            /* Second part of key specified was last column */
            endpos1 = getpos(s, sep, key1.colend) + key1.endfrac + 1;
            endpos2 = getpos(t, sep, key1.colend) + key1.endfrac + 1;
        }
        else {
            /* For all other cases */
            endpos1 = s_gpend + key1.endfrac;
            endpos2 = t_gpend + key1.endfrac;
        }

        /* Error checks and corrections */
        if(endpos1 > length1 || endpos2 > length2) {
            endpos1 = length1;
            endpos2 = length2;
        }
        if(endpos1 <= i || endpos2 <= j || i > length1 || j > length2) {
            i = 0;
            j = 0;
            endpos1 = length1;
            endpos2 = length2;
        }
    }
    else {
        /* Set indices to zero since s, t now point to first non-space char */
        i = j = 0;

        /* Calculate new strlen after ignoring white spaces */
        endpos1 = strlen(s);
        endpos2 = strlen(t);
    }

    do {
        /* Ignore non-printing characters */
        if(options.iflag) {
            while(!isprint(s[i]))
                i++;
            while(!isprint(t[j]))
                j++;
        }

        /* Consider only alphanumeric and white spaces */
        if(options.dflag) {
            while(i < endpos1 && !isalnum(s[i])
                    && !isspace(s[i]) && s[i] != '\0')
                i++;
            while(j < endpos2 && !isalnum(t[j])
                    && !isspace(t[j]) && t[j] != '\0')
                j++;
        }
        if(i < endpos1 && j < endpos2) {
            /* Ignore case */
            if(options.fflag) {
                x = tolower(s[i++]);
                y = tolower(t[j++]);
            }
            else {
                x = s[i++];
                y = t[j++];
            }

            if((x == '\t') && (y == '\n')) {
                if(options.rflag) {
                    return -1;
                }
                else {
                    return 1;
                }
            }

            if((x == '\n') && (y == '\t')) {
                if(options.rflag) {
                    return 1;
                }
                else {
                    return -1;
                }
            }

            if(x == y) {
                /* Strings are exactly same */
                if(x == '\0') {
                    return 0;
                }

                /* First string smaller than second */
                if(i == endpos1 && j!= endpos2) {
                    if(options.rflag) {
                        return -1;
                    }
                    else {
                        return 1;
                    }
                }

                /* Second string smaller than first */
                if(i != endpos1 && j == endpos2) {
                    if(options.rflag) {
                        return 1;
                    }
                    else {
                        return -1;
                    }
                }

                /* Keys are exactly same */
                if(i == endpos1 && j == endpos2) {
                    if(options.uflag) {
                        return 0;
                    }
                    else {
                        if(options.rflag)
                            return strcmp(t, s);
                        else
                            return strcmp(s, t);
                    }
                }
            }
        }
    } while(x == y && i < endpos1 && j < endpos2);

    /* Reverse sense of comparisons if rflag is set*/
    if(options.rflag)
        return y - x;
    else
        return x - y;
}

/**
 * @brief Compare two numbers passed as strings pointed by void pointers
 *
 * Sort them numerically according to global keys by using
 * createNumberfromString() and return difference on first non-equal
 * character according to their ASCII value
 *
 * @param a First number as string
 * @param b Second number as string
 * @return int
 * 		   < 0 if a < b
 * 		   = 0 if a = b
 * 		   > 0 if a > b
 * @note Original strings are not modified
 */
int numcompare(const void * a, const void * b) {
    /* Typecasting void pointers to char pointers */
    char *s = (char *)a;
    char *t = (char *)b;
    register int i = 0, j = 0, endpos1, endpos2;
    int length1 = strlen(s), length2 = strlen(t);
    int s_gpstart, t_gpstart, s_gpend, t_gpend;

    if(options.bflag) {
        /* Ignore leading white spaces */
        while(isspace(s[i]))
            i++;
        s = s + i;

        while(isspace(t[j]))
            j++;
        t = t + j;
        /* s and t now point to first non white space character */
    }
    endpos1 = length1;
    endpos2 = length2;

    if(options.kflag) {
        /* Get start and end indices based on keys */
        s_gpstart = getpos(s, sep, key1.colstart);
        t_gpstart = getpos(t, sep, key1.colstart);
        s_gpend = getpos(s, sep, key1.colend + 1);
        t_gpend = getpos(t, sep, key1.colend + 1);

        if(key1.colstart == 0 || key1.colend == 0) {
            fprintf(stderr,"Invalid field specification : %s\n", key1.line);
            exit(EINVAL);
        }

        i = s_gpstart + key1.startfrac;
        j = t_gpstart + key1.startfrac;

        if(key1.colstart == key1.colend) {
            /* Given keyfield is a single column */

            if(key1.startfrac == 0 && key1.endfrac == 0) {
                /* Example : sort -k2,2 */
                endpos1 = s_gpend + key1.endfrac;
                endpos2 = t_gpend + key1.endfrac;
            }
            else {
                /* Example : sort -k2.3,2.9 */
                endpos1 = getpos(s, sep, key1.colend) + key1.endfrac + 1;
                endpos2 = getpos(t, sep, key1.colend) + key1.endfrac + 1;
            }
        }
        else if(s_gpend == length1 && t_gpend == length2) {
            /* Second part of key specified was last column */
            endpos1 = getpos(s, sep, key1.colend) + key1.endfrac + 1;
            endpos2 = getpos(t, sep, key1.colend) + key1.endfrac + 1;
        }
        else {
            /* For all other cases */
            endpos1 = s_gpend + key1.endfrac;
            endpos2 = t_gpend + key1.endfrac;
        }

        /* Error checks and corrections */
        if(endpos1 > length1 || endpos2 > length2) {
            endpos1 = length1;
            endpos2 = length2;
        }
        if(endpos1 <= i || endpos2 <= j || i > length1 || j > length2) {
            i = 0;
            j = 0;
            endpos1 = length1;
            endpos2 = length2;
        }
    }

    /* Convert string limited by indices into double */
    double x = createNumberfromString((char *)a, i, endpos1);
    double y = createNumberfromString((char *)b, j, endpos2);

    /*
     * If equal numerical value, check for remainder
     * part of string according to flags
     */
    if(essentiallyEqual(x, y)) {
        if(options.uflag) {
            return 0;
        }
        else {
            return strcomparelesskey(a, b);
        }
    }
    else {
        if(definitelyGreaterThan(x, y)) {
            return (options.rflag ? -1 : 1);
        }
        else if(definitelyLessThan(x, y)) {
            return (options.rflag ? 1 : -1);
        }
        else {
            return 0;
        }
    }
}

/**
 * @brief Compare two strings without keys pointed by void pointers
 *
 * Sort entire string and return difference on first non-equal
 * character according to their ASCII value
 *
 * @param a First string
 * @param b Second string
 * @return int
 *         < 0 if a < b
 * 		   = 0 if a = b
 * 		   > 0 if a > b
 *
 * @note Original strings are not modified
 * @note Global keys are ignored
 */
int strcomparelesskey(const void * a, const void * b) {
    /* Typecasting void pointers to char pointers */
    char *s = (char *)a;
    char *t = (char *)b;
    char x = '\0', y = '\0';
    register int i = 0, j = 0, endpos1, endpos2;
    int length1 = strlen(s), length2= strlen(t);
    endpos1 = length1;
    endpos2 = length2;

    if(options.bflag) {
        /* Ignore leading white spaces */
        while(isspace(s[i]))
            i++;
        while(isspace(t[j]))
            j++;
    }

    do {
        /* Ignore non-printing characters */
        if(options.iflag) {
            while(!isprint(s[i]))
                i++;
            while(!isprint(t[j]))
                j++;
        }
        /* Consider only alphanumeric and white spaces */
        if(options.dflag) {
            while(i < endpos1 && !isalnum(s[i])
                    && !isspace(s[i])  && s[i] != '\0')
                i++;
            while(j < endpos2 && !isalnum(t[j])
                    && !isspace(t[j]) && t[j] != '\0')
                j++;
        }
        if(i < endpos1 && j < endpos2) {
            /* Ignore case */
            if(options.fflag) {
                x = tolower(s[i++]);
                y = tolower(t[j++]);
            }
            else {
                x = s[i++];
                y = t[j++];
            }

            if((x == '\t') && (y == '\n')) {
                if(options.rflag) {
                    return -1;
                }
                else {
                    return 1;
                }
            }
            if((x == '\n') && (y == '\t')) {
                if(options.rflag) {
                    return 1;
                }
                else {
                    return -1;
                }
            }
            if(x == y) {
                /* Strings are exactly same */
                if(x == '\0') {
                    return 0;
                }

                /* First string smaller than second */
                if(i == endpos1 && j!= endpos2) {
                    if(options.rflag) {
                        return -1;
                    }
                    else {
                        return 1;
                    }
                }

                /* Second string smaller than first */
                if(i != endpos1 && j == endpos2) {
                    if(options.rflag) {
                        return 1;
                    }
                    else {
                        return -1;
                    }
                }

                /* Exactly same */
                if(i == endpos1 && j == endpos2) {
                    return 0;
                }
            }
        }
    } while(x == y && i < endpos1 && j < endpos2);

    /* Reverse sense of comparisons if rflag is set*/
    if(options.rflag)
        return y - x;
    else {
        return x - y;
    }
}

/**
 * @brief Create a number from string
 *
 * Converts string between i and j to a double numerical value which may
 * contain leading zeroes/spaces and/or radix point.
 *
 * @param a String
 * @param i Start indice
 * @param j End indice
 * @return double Number or 0 if first character is not a digit after ignoring
 *                leading spaces/zeroes.
 */
double createNumberfromString(char *a, int i, int j) {
    int n = 1, negative = 1;
    double sum = 0, fraction = 0;

    /* Ignore leading white spaces and zeroes */
    while(isspace(a[i]) || a[i] == '0')
        i++;

    /* Check for negative numbers */
    if(a[i] == '-') {
        negative = -1;
        i++;
    }

    /* Parse number from string */
    while(i < j && a[i] != '\0') {
        if(!isdigit(a[i])) {
            if(a[i] == '.') {
                i++;
                while(a[i] != '\0' && isdigit(a[i]) && i < j) {
                    fraction = fraction + (a[i] - '0')/ pow(10, n);
                    i++;
                    n++;
                }
            }
            return (sum + fraction) * negative;
        }
        else {
            sum = (sum * 10) + (a[i] - '0');
            i++;
        }
    }

    return (sum + fraction) * negative;
}

/**
 * @brief Checks if two numbers are equal
 *
 * Tells whether the difference between a and b is smaller than the acceptable
 * error (epsilon), determined by the smaller of a or b. This means that the values
 * differ less than the acceptable difference in any calculation, so that perhaps
 * they're not actually equal, but they're "essentially equal" (given the epsilon).
 *
 * @param a First number
 * @param b Second number
 * @return bool
 * 		   = 0 if a != b
 * 		   = 1 if a ~= b
 * @note Borrowed from https://stackoverflow.com/a/253874
 */
int essentiallyEqual(double a, double b) {
    return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * EPSILON);
}

/**
 * @brief Checks if a is greater than b .
 *
 * @param a First number
 * @param b Second number
 * @return bool
 * 		   = 1 if a > b
 * 		   = 0 if a <= b
 * @note Borrowed from https://stackoverflow.com/a/253874
 */
int definitelyGreaterThan(double a, double b) {
    return (a - b) > ((fabs (a) < fabs (b) ? fabs (b) : fabs (a)) * EPSILON);
}

/**
 * @brief Checks if a is lesser than b .
 *
 * @param a First number
 * @param b Second number
 * @return bool
 * 		   = 1 if a < b
 * 		   = 0 if a >= b
 * @note Borrowed from https://stackoverflow.com/a/253874
 */
int definitelyLessThan(double a, double b) {
    return (b - a) > ((fabs (a) < fabs (b) ? fabs (b) : fabs (a)) * EPSILON);
}

/**
 * @brief Returns position of character in string given a particular column number
 *
 * @param haystack String to be searched in
 * @param needle Character to be searched for (separator)
 * @param count Column to be searched
 * @return int
 *         = indice if valid
 * 		   = length if invalid
 */
int getpos(char *haystack, char *needle, int count) {
    register int i;
    int length = strlen(haystack), maxcount = 0;

    /* Check maximum number of columns in line */
    for(i = 0; haystack[i] != '\0'; i++)
        if(haystack[i] == needle[0])
            maxcount++;

    /* Return Invalid column as input */
    if(count > maxcount + 1 || count < 0)
        return length;

    i = 0;
    while(--count && i < length) {
        while(haystack[i] != needle[0]) {
            i++;
        }
        i++;
    }

    return i;
}

/**
 * @brief Set global keys using arguments passed on command line
 *
 * To set field positions according to argument passed via command line.
 *
 * @param k Pointer to key (global variable)
 * @return key containing converted command line argument
 */
void getkey(key *k) {
    char a[8], *b;
    int i = 0, intpart = 0, fracpart = 0;

    /* First string... */
    strcpy(a, k->line);
    b = strtok(a, ",");

    /* Ignore leading whitespace characters */
    while(isspace(b[i]))
        i++;

    /* Parse integer and fractional part from first half of string */
    while(b[i] != '\0') {
        if(!isdigit(b[i])) {
            if(b[i] == '.') {
                i++;
                while(b[i] != '\0' && isdigit(b[i])) {
                    fracpart = (fracpart * 10) + (b[i] - '0');
                    i++;
                }
            }
            break;
        }
        else {
            intpart = (intpart * 10) + (b[i] - '0');
            i++;
        }
    }
    k->colstart = intpart;
    if(fracpart)
        k->startfrac = fracpart - 1;

    /* Second string... */
    b = strtok(NULL, ",");
    if(!b) {
        k->colend = k->endfrac = -1;
        return ;
    }
    i = intpart = fracpart = 0;
    while(isspace(b[i]))
        i++;

    /* Parse integer and fractional part from second half of string */
    while(b[i] != '\0') {
        if(!isdigit(b[i])) {
            if(b[i] == '.') {
                i++;
                while(b[i] != '\0' && isdigit(b[i])) {
                    fracpart = (fracpart * 10) + (b[i] - '0');
                    i++;
                }
            }
            break;
        }
        else {
            intpart = (intpart * 10) + (b[i] - '0');
            i++;
        }
    }

    k->colend = intpart;

    if(fracpart)
        k->endfrac = fracpart - 1;

    return;
}

/**
 * @brief Make sorted runs of file
 *
 * Store sorted runs of the file made using in memory buffer and inbuilt qsort
 * on disk.
 *
 * @param datafile File poiner to a text file containing data
 * @return int Number of sorted runs created
 * @return Files named "temp1", "temp2" and so on.. in the current directory,
 *         each one corresponding to a run
 */
int makeSortedRuns(FILE *datafile) {
    FILE *outfile;
    stringType outFilename;
    bufType buffer;
    int lines = 0, files = 0, k = 0;

    while(1) {
        /* Fill buffer with the next set of lines */
        lines = fillBuffer(datafile, buffer);

        /* Break if no more lines */
        if(lines == 0)
            break;

        /* String or numeric sort... */
        if(options.nflag)
            qsort(buffer, lines, STRSIZE, numcompare);
        else
            qsort(buffer, lines, STRSIZE, strcompare);

        /* Make custom name for sorted runs */
        sprintf(outFilename, "temp%d", files);

        /* Open the file */
        outfile = fopen(outFilename, "w");
        if(outfile == NULL) {
            printf("Couldn't open %s", outFilename);
            return errno;
        }

        /* Write to the file */
        for(k = 0; k < lines; k++) {
            fprintf(outfile, "%s", buffer[k]);
        }

        /* Close the file */
        fclose(outfile);

        /* One less file remaining... */
        files++;
    }

    return files;
}

/**
 * @brief Fill the buffer with data from file while checking for duplicates
 *
 * Place as much data as possible into the buffer while also checking for
 * duplicates
 *
 * @param datafile Text file stream already opened for input
 * @param buffer 2D array of char to copy data into
 * @return int Number of lines read into the buffer
 * @return Buffer containing data
 */
int fillBuffer(FILE *datafile, bufType buffer) {
    int k = 0;
    for(k = 0; k < BUFSIZE; k++) {
        strcpy(buffer[k], "");
    }

    for(k = 0; k < BUFSIZE;) {
        if(fgets(buffer[k], STRSIZE, datafile)) {
            /* Check for duplicate lines */
            if(options.uflag && k) {
                if(options.nflag && (numcompare((void *)buffer[k], (void *)buffer[k - 1]) == 0))
                    continue;
                else if(strcompare((void *)buffer[k], (void *)buffer[k - 1]) == 0) {
                    continue;
                }
                else
                    k++;
            }
            else
                k++;
        }
        else
            break;
    }

    return k;
}

/**
 * @brief Merge contents of two files into one
 *
 * Merge contents of files f1 and f2 into f3 using the same comparison method
 * used earlier while sorting.
 * Three buffers are used. Two for input files and one for output file.
 * A 2 way merge is used.
 *
 * @param f1 FILE pointer to file having sorted run #1
 * @param f2 FILE pointer to file having sorted run #2
 * @param f3 FILE pointer to file that should contain merged runs
 * @note This also removes the initial runs which are named "temp#"
 */
void merge(FILE *f1, FILE *f2, FILE *f3) {
    mergeLine line1, line2, result;
    result.nlines = BUFSIZE;
    int i = 0, j = 0, k = 0, t = 0, cmp = 0;

    /* Fill input buffers */
    line1.nlines = fillBuffer(f1, line1.line);
    line2.nlines = fillBuffer(f2, line2.line);

    while(1) {
        while(i < line1.nlines && j < line2.nlines && k < result.nlines) {
            if(options.nflag)
                cmp = numcompare((void *)line1.line[i], (void *)line2.line[j]);
            else
                cmp = strcompare((void *)line1.line[i], (void *)line2.line[j]);

            if(cmp < 0) {
                strcpy(result.line[k++], line1.line[i++]);
            }
            else if(cmp > 0) {
                strcpy(result.line[k++], line2.line[j++]);
            }
            else {
                strcpy(result.line[k++], line1.line[i++]);
                if(options.uflag)
                    j++;
            }
        }

        if(i == line1.nlines) {
            /* Fill first buffer if more lines */
            line1.nlines = fillBuffer(f1, line1.line);
            i = 0;

            if(line1.nlines == 0) {
                /* No more lines in first file */
                break;
            }
        }

        if(j == line2.nlines) {
            /* Fill second buffer if more lines */
            line2.nlines = fillBuffer(f2, line2.line);
            j = 0;

            if(line2.nlines == 0) {
                /* No more lines in second file */
                break;
            }
        }

        if(k == result.nlines) {
            /* Output buffer is full, so store on disk */
            for(t = 0; t < k; t++) {
                fprintf(f3, "%s", result.line[t]);
                strcpy(result.line[t], "");
            }
            k = 0;
        }
    }

    if(line1.nlines == 0) {
        /* First file smaller than second */

        /* Put output buffer on disk */
        for(t = 0; t < k; t++) {
            fprintf(f3, "%s", result.line[t]);
            strcpy(result.line[t], "");
        }
        k = 0;

        /* Put second buffer on disk */
        for(t = j; t < line2.nlines; t++) {
            fprintf(f3, "%s", line2.line[t]);
            strcpy(line2.line[t], "");
        }

        /* Put remaining data from second file on disk */
        while(fgets(line2.line[0], STRSIZE, f2)) {
            fprintf(f3, "%s", line2.line[0]);
        }
    }

    if(line2.nlines == 0) {
        /* Second file smaller than first */

        /* Put output buffer on disk */
        for(t = 0; t < k; t++) {
            fprintf(f3, "%s", result.line[t]);
            strcpy(result.line[t], "");
        }
        k = 0;

        /* Put first buffer on disk */
        for(t = i; t < line1.nlines; t++) {
            fprintf(f3, "%s", line1.line[t]);
            strcpy(line1.line[t], "");
        }

        /* Put remaining data from first file on disk */
        while(fgets(line1.line[0], STRSIZE, f1)) {
            fprintf(f3, "%s", line1.line[0]);
        }
    }
}

/**
 * @brief Merge runs and merge again until one file remains
 *
 * Continously merge sorted runs until there is only one left.
 * This is achieved by using merge() passing two input files and a output file.
 *
 * @param numFiles Number of sorted runs
 * @param x Either 'r' or 'o'
 * @return Single file formed after merging all sorted runs/files.
 *         Named "temp0" if runs were merged.
 *         Named "output0" if output files were merged.
 */
void handleMerges(int numFiles, char x) {
    int filesLeft = numFiles, i = 0, k;
    char a[25], b[25], c[25];
    FILE *f1 = NULL, *f2 = NULL, *f3 = NULL;
    mergeLine line1;

    if(filesLeft == 1) {
        /* Open input file */
        if(x == 'r') {
            f1 = fopen("temp0", "r");
        }
        else if(x == 'o') {
            f1 = fopen("output0", "r");
        }
        if(f1 == NULL) {
            perror("Couldn't open file ");
            exit(ENOENT) ;
        }

        /* Open output file */
        f3 = fopen("output", "w+");
        if(f3 == NULL) {
            perror("Couldn't open file ");
            exit(ENOENT) ;
        }

        while(1) {
            /* Fill buffer */
            line1.nlines = fillBuffer(f1, line1.line);
            if(!line1.nlines)
                break;

            /* Write to output file */
            for(i = 0; i < line1.nlines; i++) {
                fprintf(f3, "%s", line1.line[i]);
            }

            /* Close the files */
            fclose(f1);
            fclose(f3);

            /* Delete temp files created in the process */
            if(x == 'r') {
                remove("temp0");
                rename("output", "temp0");
            }
            else if(x == 'o') {
                remove("output0");
                rename("output", "output0");
            }
        }
        return ;
    }

    while(filesLeft > 1) {
        for(i = 0, k = 0; i < numFiles - 1; i += 2, k++) {
            /* Make custom file names */
            if(x == 'r') {
                sprintf(a, "temp%d", i);
                sprintf(b, "temp%d", i + 1);
                sprintf(c, "temp%d", k);
            }
            else if(x == 'o') {
                sprintf(a, "output%d", i);
                sprintf(b, "output%d", i + 1);
                sprintf(c, "output%d", k);
            }

            /* Open input files for reading */
            f1 = fopen(a, "r");
            if(f1 == NULL) {
                perror("Couldn't open file ");
                exit(ENOENT) ;
            }
            f2 = fopen(b, "r");
            if(f2 == NULL) {
                perror("Couldn't open file ");
                exit(ENOENT) ;
            }

            /* Open output file for writing */
            f3 = fopen("output", "w+");
            if(f3 == NULL) {
                perror("Couldn't open file ");
                exit(ENOENT) ;
            }

            /* Merge! */
            merge(f1, f2, f3);

            /* Close the files */
            fclose(f1);
            fclose(f2);
            fclose(f3);

            /* Delete the temp files created in the process */
            remove(a);
            remove(b);

            /* Rename file to custom name */
            rename("output", c);

            /* One less file remaining... */
            filesLeft--;
        }
        if(numFiles & 1) {
            /* Handle the loner file */
            if(x == 'r') {
                sprintf(a, "temp%d", i);
                sprintf(c, "temp%d", k);
                rename(a, c);
            }
            else if(x == 'o') {
                sprintf(a, "output%d", i);
                sprintf(c, "output%d", k);
                rename(a, c);
            }
        }
        numFiles = filesLeft;
    }
}


/**
 * @brief Only merges files
 *
 * Continously merge sorted files until there is only one left.
 * This is achieved by using merge() by passing it two input files and a output
 * file.
 *
 * @param numFiles Number of files (assumed to be sorted) to be merged
 * @param mergefiles Array of pointers to filenames of files to be merged
 * @return File named "output0" after merging all input files
 * @note Input files are assumed to be sorted
 */
void handleOnlyMerges(int numFiles, char *mergefiles[16]) {
    char c[25];
    FILE *f1, *f2, *f3;
    int filesLeft = numFiles, i, k;

    for(i = 0, k = 0; i < numFiles - 1; i += 2, k++) {
        /* Try opening all files */
        f1 = fopen(mergefiles[i], "r");
        if(f1 == NULL) {
            perror("Couldn't open file ");
            exit(ENOENT) ;
        }
        f2 = fopen(mergefiles[i + 1], "r");
        if(f2 == NULL) {
            perror("Couldn't open file ");
            exit(ENOENT) ;
        }
        f3 = fopen("output", "w+");
        if(f3 == NULL) {
            perror("Couldn't open file ");
            exit(ENOENT) ;
        }

        /* Merge f1 and f2 and store output in f3 */
        merge(f1, f2, f3);

        /* Close all files */
        fclose(f1);
        fclose(f2);
        fclose(f3);

        /* Make custom file name for temporary file */
        sprintf(c, "output%d", k);

        /* Rename temp file to custom name */
        rename("output", c);

        /* One less file to sort now.. */
        filesLeft--;
    }
    if(numFiles & 1) {
        /* Odd number of files as input, so handle loner file */
        sprintf(c, "output%d", k);

        /* Rename it to convention adopted so far */
        rename(mergefiles[i], c);
    }

    numFiles = filesLeft;

    handleMerges(numFiles, 'o');
}

void show_help(void) {
    const char *help_text =
"Usage: msort [options]... [FILE]...\n\
     Ordering options:\n\
     -b, ignore leading blanks\n\
     -d, consider only blanks and alphanumeric characters\n\
     -f, fold lower case to upper case characters\n\
     -h, show help and exit\n\
     -i, consider only printable characters\n\
     -n, compare according to string numerical value\n\
     -r, reverse the result of comparisons\n\
     -k, sort via a key\n\
     -m, merge already sorted files; do not sort\n\
     -o, write result to FILE instead of standard output\n\
     -u, output only the first of an equal run\n\
";
    printf("%s", help_text);
}

/**
 * @brief Main function
 *
 * Parses command line arguments and sorts and / or merges based on arguments
 *
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return 0 on success, non zero on error
 */
int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "C");
    FILE *datafile = NULL;
    char c, *inputfiles[16] = {NULL}, *mergefiles[16] = {NULL}, output_name[16], line[STRSIZE];
    int i, numinput = 0, runs = 0;

    while ((c = getopt(argc, argv, "bdfhik:mno:rt:u")) != -1) {
        switch (c) {
            case 'b':
                options.bflag = 1;	/* Ignore leading blanks */
                break;
            case 'd':
                options.dflag = 1;	/* Dictionary order */
                break;
            case 'f':
                options.fflag = 1;	/* Ignore case */
                break;
            case 'h':
                options.hflag = 1; /* Show help and exit */
                break;
            case 'i':
                options.iflag = 1;	/* Ignore non-printing */
                break;
            case 'k':
                options.kflag = 1;	/* Key */
                strcpy(key1.line, optarg);
                getkey(&key1);
                break;
            case 'm':
                options.mflag = 1;	/* Merge already sorted files */
                break;
            case 'n':
                options.nflag = 1;	/* Numeric sort */
                break;
            case 'o':
                options.oflag = 1;	/* Output to a file */
                outFilename = optarg;
                break;
            case 'r':
                options.rflag = 1;	/* Reverse sort */
                break;
            case 't':
                options.tflag = 1;	/* Separator over non-blank to blank transition */
                sep = optarg;
                break;
            case 'u':
                options.uflag = 1;	/* Only unique lines based on keys */
                break;
            case '?':
                options.err = 1;	/* Option not in given list */
                fprintf(stderr, "Refer README.md for more information.\n");
                exit(EINVAL);
        }
    }

    if (options.hflag) {
        show_help();
        return 0;
    }

    if((options.dflag + options.iflag + options.nflag) > 1) {
        /* Undefined behaviour */
        fprintf(stderr, "The behaviour is undefined for combinations of [-dni] options.\n");
    }

    if(optind == argc) {
        /* No input files were specified, take input from stdin */
        datafile = stdin;
    }
    else if((strcmp("-", argv[optind]) == 0) && ((optind + 1) == argc)) {
        /* If a file operand is '-', take input from stdin */
        datafile = stdin;
    }
    else if(optind < argc) {
        /* Take arguments after the command line options */
        while(optind < argc) {
            if(options.mflag)
                mergefiles[numinput++] = argv[optind++];
            else
                inputfiles[numinput++] = argv[optind++];
        }
    }


    if((numinput == 0) && options.mflag) {
        /* No files specified to be merged */
        fprintf(stderr, "No files to be merged.\nRefer README.md for more information.\n");
        exit(EINVAL);
    }

    if(datafile == stdin && !options.mflag) {
        /* STDIN as input */
        if((c = fgetc(datafile)) == -1)
            exit(0);
        else
            ungetc(c, datafile);

        /* Make sorted runs out of file */
        runs = makeSortedRuns(datafile);

        /* Merge all the runs of the file */
        handleMerges(runs, 'r');

        /* Close the file */
        fclose(datafile);

        /* Rename file to desired name */
        rename("temp0", outFilename);
    }
    else if(datafile != stdin && !options.mflag) {
        /* Files as input */

        for(i = 0; i < numinput; i++) {
            /* Open the file */
            datafile = fopen(inputfiles[i], "r");
            if(datafile == NULL) {
                perror("Can't open file ");
                exit(ENOENT);
            }

            /* Make sorted runs out of file */
            runs = makeSortedRuns(datafile);

            /* Merge all the runs of the file */
            handleMerges(runs, 'r');

            /* Close the file */
            fclose(datafile);

            /* Make custom name for sorted file */
            sprintf(output_name, "output%d", i);

            /* Rename file to custom name to allow for merging */
            rename("temp0", output_name);
        }

        /* Merge all files*/
        if(numinput > 1)
            handleMerges(numinput, 'o');

        /* Rename file to desired file name */
        rename("output0", outFilename);
    }
    else if(options.mflag) {
        /* Only merge input files*/
        handleOnlyMerges(numinput, mergefiles);

        /* Rename file to desired file name */
        rename("output0", outFilename);
    }


    if(!options.oflag) {
        /* Print sorted output on terminal */
        datafile = fopen(outFilename, "r");
        while(fgets(line, STRSIZE, datafile)) {
            fprintf(stdout, "%s", line);
        }

        /* Close the file */
        fclose(datafile);

        /* Delete temporary file created in the process */
        remove(outFilename);
    }

    return 0;
}