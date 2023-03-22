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