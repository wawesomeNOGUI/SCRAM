#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "util.c"

#define MAXINPUT 100000
#define MAXPROGLENGTH 16
#define HASHSIZE 256

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

// variables & labels indexed by "hashmap" which will be 
// their string summed, modulo HASHSIZE
// this means that names can't be too long or else will rollover
// and overwrite entries (the c programming book has a better implementation)
char varLocations[HASHSIZE];
char labelLocations[HASHSIZE];

char machineCode[MAXPROGLENGTH];
char tmpBin[8];

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
                s++;    // remove newline from comment and any sequential newlines
                if (*s == '\n')
                    while (*++s == '\n') {};
            }
        }
    }

    if (*(t-1) != '\n')
         *t++ = '\n';
    *t = EOF;
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
        } else if (*s == '.' && !isalpha(*(s-1))) { // isalpha to check if label being reference in instruction
            s++;

            int hash = 0;
            while (*s != '\n') {
                hash += *s++;
            }
            s++;    // remove newline from line label
            labelLocations[hash%HASHSIZE] = line;
            // printf("\n\n%d\n\n", line);
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
        if ( isalpha(*s) && *(s+1) == '=' ) { // var names can't be numbers
            numOfVars++;
            int hash = *s;
            s++;
            s++; // ignore equals sign

            // check number format (in util.c)
            char *end;
            long ret;
            ret = getLongFromNumString(s, &end);
            // printf("\n\n%d\n\n", ret);

            s = end;
            if (*s != '\n') {
                printf("\033[0;31m"); //Set the text to the color red
                printf("\n\nERROR: NO NEWLINE AFTER VAR\n\n");
                printf("\033[0m"); //Resets the text to default color
                return;
            }
            s++;

            machineCode[MAXPROGLENGTH-numOfVars] = ret;
            varLocations[hash] = MAXPROGLENGTH-numOfVars;
        } else {
            *t++ = *s++;
        }
    }
    *t = EOF;
    return numOfVars;
}

parseInstructions(s) 
char s[];
{
    int numOfOpcodes = 0;
    while (*s != EOF) {
        switch(*s) {
        case 'L':
            s++;
            s++;
            if (*s == 'A') {        // LDA
                s++;
                machineCode[numOfOpcodes] = mergeOPCODEandOPERAND(LDA, s, varLocations, labelLocations);
            } else if (*s == 'I') { // LDI
                s++;
                machineCode[numOfOpcodes] = mergeOPCODEandOPERAND(LDI, s, varLocations, labelLocations);
            }
            numOfOpcodes++;
            break;
        case 'S':
            s++;
            if (*s == 'U') {        // SUB
                s++;
                s++;
                machineCode[numOfOpcodes] = mergeOPCODEandOPERAND(SUB, s, varLocations, labelLocations);
                numOfOpcodes++;
                break;
            }

            s++;
            if (*s == 'A') {        // STA
                s++;
                machineCode[numOfOpcodes] = mergeOPCODEandOPERAND(STA, s, varLocations, labelLocations);
            } else if (*s == 'I') { // STI
                s++;
                machineCode[numOfOpcodes] = mergeOPCODEandOPERAND(STI, s, varLocations, labelLocations);
            }
            numOfOpcodes++;
            break;
        case 'A':
            s++;
            s++;
            s++;
            machineCode[numOfOpcodes] = mergeOPCODEandOPERAND(ADD, s, varLocations, labelLocations);
            numOfOpcodes++;
            break;
        case 'J':
            s++;
            s++;
            if (*s == 'P') {        // JMP
                s++;
                machineCode[numOfOpcodes] = mergeOPCODEandOPERAND(JMP, s, varLocations, labelLocations);
            } else if (*s == 'Z') { // JMZ
                s++;
                machineCode[numOfOpcodes] = mergeOPCODEandOPERAND(JMZ, s, varLocations, labelLocations);
            }
            numOfOpcodes++;
            break;
        default:
            printf("\033[0;31m"); //Set the text to the color red
            printf("\n\nERROR: DONT RECOGNIZE THIS INSTRUCTION.\n\n");
            printf("\033[0m"); //Resets the text to default color
            return;
        }

        // increment past operand and newline
        while (*s != '\n') {
            s++;
        }
        
        if (*s == '\n') {
            s++;
        } else {
            printf("%c", *s);
        }
    }
}

main() {
  /* assemble programs writen for the SCRAM computer */
  getInputAndRemoveWhitespace(inputBufOne);

  removeComments(inputBufOne, inputBufTwo);

  int numOfVars = storeVariables(inputBufTwo, inputBufOne);
  int progLength = storeAssemLables(inputBufOne, inputBufTwo);
  progLength += numOfVars;

  printf("PROGRAM LENGTH: %d OF 15 AVAILABLE LINES USED.\nTHERE ARE %d VARS DECLARED.\n\n", progLength, numOfVars);

  // print parsed assembly program
  printf("PARSED ASSEMBLY:\n");
  char *look = inputBufTwo;
  while (*look != EOF) {
    printf("%c", *look++);
  }
  printf("\n");

  parseInstructions(inputBufTwo);

  // print machine code
  printf("MACHINE CODE:\n");
  for (int j = 0; j < MAXPROGLENGTH; j++) {
    intToBin8(machineCode[j], tmpBin);
    for (int i = 0; i < 8; i++) {
        printf("%c", tmpBin[i]);
    }
    printf("\n");
  }

  // print machine code in VHDL format
  printf("\nFOR VHDL:\n");
  printf("(");
  for (int j = 0; j < MAXPROGLENGTH; j++) {
    printf("(\"");
    intToBin8(machineCode[j], tmpBin);
    for (int i = 0; i < 8; i++) {
        printf("%c", tmpBin[i]);
    }
    printf("\"),");
  }
  printf("\b);\n");
}