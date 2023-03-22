#include <stdio.h>
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

storeAssemLables(s, t)
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
            tempInput[hash%HASHSIZE] = line;
        }

        *t++ = *s++;
    }
    *t = EOF;
    return line;
}

storeVariables(s, t)    /* we'll store varaibles at the end of the code*/
char s[];
char t[];
{
    
}

main() {
  /* assemble programs writen for the SCRAM computer */
  int newlines = getInputAndRemoveWhitespace(inputBufOne);
  removeComments(inputBufOne, inputBufTwo);
  int progLength = storeAssemLables(inputBufTwo, inputBufOne);
  printf("PROGRAM LENGTH: %d OF 15 AVAILABLE LINES USED.\n\n", progLength);

  char *start = inputBufOne;
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