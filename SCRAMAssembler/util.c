#define HASHSIZE 256

getline(s, lim)   /* get line into s, return length */
char s[];
int lim;
{
  int c, i;

  i = 0;
  while (--lim > 0 && (c=getchar()) != EOF && c != '\n')
    s[i++] = c;
  if (c == '\n')
    s[i++] = c;
  s[i] = '\0';

  if (c == EOF)
    return(c);
  return(i);
}

getWord(s, lim) /* get word into s, return length */
char s[];
int lim;
{
  int c, i;

  i = 0;
  // first get rid of spaces/newlines/tabs
  while ((c=getchar()) == ' ' || c == '\n' || '\t')
    ;
  s[i++] = c;
  while (--lim > 0 && (c=getchar()) != EOF && c != '\n' && c != ' ')
    s[i++] = c;
  s[i] = '\0';

  if (c == EOF)
    return(c);
  return(i);
}

strCmp(s, t)  /* compare two strings */
char s[];
char t[];
{
  int b = 1;
  int i = 0;

  while (s[i] != '\0' && t[i] != '\0') {
    if (s[i] == t[i]) {
      i++;
    } else {
      b = 0;
      break;
    }
  }

  return b;
}

isStringNumber(s) /* return true if string contains a number */
char s[];
{
    int i = 0;
    while (s[i++] != '\0') {
      int number = s[i] <= '9' && s[i] >= '0';
      if (!number)
        return 0;
    }

  return 1;
}

intToBin8(n, s) /* returns 8-bit representation of int */
int n;
char s[];
{
  for (int i = 1 << 7; i > 0; i = i >> 1) {
    if ((i & n) != 0) {
      *s++ = '1';
    } else {
      *s++ = '0';
    }
    //printf("%c", *(s-1));    
  }
}

long getLongFromNumString(s, end)  /* Returns a long from bin/hex/dec number string */
char s[];
char **end;
{
  if (*s == '%') {
    s++;
    // convert binary string to int
    return strtol(s, end, 2);
  } else if (*s == '$') {
    s++;
    // convert hex string to int
    return strtol(s, end, 16);
  } else {
    // assume decimal (base 10)
    return strtol(s, end, 10);
  }
}

isLabel(s) /* returns 0 if not label, else returns hash for the label */
char s[];
{
  if (*s == '.') {
    s++;
    int hash = 0;
    while (*s != '\n') {
        hash += *s++;
    }
    return hash;
  } else {
    return 0;
  }
}

isVar(s)
char s[];
{
  if (isalpha(*s)) {
    return 1;
  } else {
    return 0;
  }
}

mergeOPCODEandOPERAND(c, o, varmap, labelmap) /* returns merged opcode and operand */
int c;
char o[];
char varmap[];
char labelmap[];
{
  int retINSTRUCT = 0;
  //check if operand variable, label, or literal number
  int labelHash = isLabel(o);
  if (isVar(o)) {
    retINSTRUCT = c | varmap[*o];
    // o++;
  } else if (labelHash) {
    retINSTRUCT = c | labelmap[labelHash%HASHSIZE];
  } else {
    // check number format (in util.c)
    char *end;
    long ret;
    ret = getLongFromNumString(o, &end);
    o = end;
    // o++;

    retINSTRUCT = c | ret;
  }

  return retINSTRUCT;
}

getInputAndRemoveWhitespace(s)  /* returns number of newlines */
char s[];
{
  int c, i, newlines = 0;

  // first ignore leading newlines and whitespace
  while ((c = getchar()) == '\n' || c == ' ' || c == '\t' || c == '\0') {}
  s[i++] = c; // store first char

  while ((c = getchar()) != EOF) {
    if (c != ' ' && c != '\t' && c != '\0') {
      //storechar
      s[i++] = c;
    } else if (c == '\n') {
        newlines++;
    }
  }

  s[i] = EOF;
  return newlines;
}