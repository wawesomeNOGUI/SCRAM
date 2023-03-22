#include <stdio.h>
#include <stdlib.h>
#include "util.c"

#define MAXINPUT 100000
#define PROGLENGTH 16
#define HASHSIZE 256
#define TMPSIZE 20  // no command should be over 20 characters

// OPCODES bitwise OR with operands
#define LDA 0b00010000
#define LDI 0b00100000
#define STA 0b00110000
#define STI 0b01000000
#define ADD 0b01010000
#define SUB 0b01100000
#define JMP 0b01110000
#define JMZ 0b10000000

char inputBufOne[MAXINPUT];
char inputBufTwo[MAXINPUT];
char tempInput[TMPSIZE];

// variables & labels indexed by "hashmap" which will be 
// their string summed, modulo HASHSIZE
// this means that names can't be too long or else will rollover
// and overwrite entries (the c programming book has a better implementation)
char varLocations[HASHSIZE];
char labelLocations[HASHSIZE];

char machineCode[PROGLENGTH];

// comments are written in format: ;comment
removeComments(s, t)
char s[];
char t[];
{
    int inlineComment = 0;
    while (*s != EOF) {
        if (*s != ';') {
            *t++ = *s;

            // remove sequential newlines
            if (*s == '\n') {
                while (*++s == '\n') {};
                inlineComment = 0;
            } else {
                s++;
                inlineComment = 1;
            }
        } else {
            while (*s != '\n') {
                s++;
            }
            if (!inlineComment) {
                s++;    // remove newline from comment
            }
        }
    }

    *--t = EOF;  // decremented to remove trailing newline
}

// labels are written in format: .LABEL  
storeAssemLables(s, t)  /* returns length of program */
char s[];
char t[];
{
    int line = 0;
    while (*s != EOF) {
        if (*s == '\n') {
            line++;
        } else if (*s == '.') {
            s++;

            int hash = 0;
            while (*s != '\n') {
                hash += *s++;
            }
            s++;    // remove newline from line label
            labelLocations[hash%HASHSIZE] = line;
        }

        *t++ = *s++;
    }
    *t = EOF;
    return line;
}

// vars are written in format: x = 0000 
// vars can only be named by a single non numeric character  
// we'll store varaibles at the end of the code
storeVariables(s, t)    /* returns number of variables */
char s[];
char t[];
{
    int numOfVars = 0;
    while (*s != EOF) {
        if ( (*s > '9' || s < '0') && *(s+1) == '=' ) { // var names can't be numbers
            numOfVars++;
            int hash = *s;
            s++;
            s++; // ignore equals sign

            // check number format
            char *end;
            long ret;
            if (*s == '%') {
                s++;
                // convert binary string to int
                ret = strtol(s, &end, 2);
            } else if (*s == '$') {
                s++;
                // convert hex string to int
                ret = strtol(s, &end, 16);
            } else {
                // assume decimal (base 10)
                ret = strtol(s, &end, 10);
            }
            s = end;
            if (*s != '\n')
                printf("\nERROR: NO NEWLINE AFTER VAR\n");
            s++;

            machineCode[PROGLENGTH-numOfVars] = ret;
            varLocations[hash] = PROGLENGTH-numOfVars;
        } else {
            *t++ = *s++;
        }
    }
    *t = EOF;
    return numOfVars;
}

main() {
  /* assemble programs writen for the SCRAM computer */
  getInputAndRemoveWhitespace(inputBufOne);

  removeComments(inputBufOne, inputBufTwo);

  int progLength = storeAssemLables(inputBufTwo, inputBufOne);
  int numOfVars = storeVariables(inputBufOne, inputBufTwo);

  printf("PROGRAM LENGTH: %d OF 15 AVAILABLE LINES USED.\nTHERE ARE %d VARS DECLARED.\n\n", progLength, numOfVars);


  char *start = inputBufTwo;
  char *look = start;

  // parse assembly program
  while (*look != EOF) {
    printf("%c", *look++);
    // if (*look == 'f') {
      
    // } else if (*look == 'b') {
    //   if (*++look == 'k') {
    //     // //backward
    //     // //get distance parameter
    //     // int i = 0;
    //     // for (int j = 0; j < MAXDIGITS; j++)
    //     //   logoArg[j] = '\0';
    //     // while (*++look >= '0' && *look <= '9')
    //     //   logoArg[i++] = *look;

    //     // printf("backward(");
    //     // for (int j = 0; j < MAXDIGITS; j++) {
    //     //   if (logoArg[j] == '\0')
    //     //     break;
    //     //   printf("%c", logoArg[j]);
    //     // }
    //     // printf("); \n");
    //   }
    // } 
    //start = look;
  }
}