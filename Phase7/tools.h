// tools.h, 159

#ifndef _TOOLS_H
#define _TOOLS_H

#include "typedef.h" // q_t needs be defined in code below

void MyBzero(char *, int);
int DeQ(q_t *);
void EnQ(int, q_t *);
void MyStrcpy(char *, char *);
int MyStrcmp(char *p, char *q, int size);
int MyStrlen(char *s);
void MyMemcpy(char *dest, char *src, int size);
#endif

