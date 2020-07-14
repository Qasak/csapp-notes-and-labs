/*
将C语言的声明转换为文字描述
char **argv
    argv:  pointer to char
int (*daytab)[13]
    daytab:  pointer to array[13] of int
int *daytab[13]
    daytab:  array[13] of pointer to int
void *comp()
    comp: function returning pointer to void
void (*comp)()
    comp: pointer to function returning void
char (*(*x())[])()
    x: function returning pointer to array[] of
    pointer to function returning char
char (*(*x[3])())[5]
    x: array[3] of pointer to function returning
    pointer to array[5] of char 
*/



/*
dcl:       optional *'s direct-dcl
direct-dcl name
                 (dcl)
                 direct-dcl()
                 direct-dcl[optional size]
*/


/* dcl:  parse a declarator */
void dcl(void)
{
   int ns;
   for (ns = 0; gettoken() == '*'; ) /* count *'s */
	   ns++;
   dirdcl();
   while (ns-- > 0)
	   strcat(out, " pointer to");
}
/* dirdcl:  parse a direct declarator */
void dirdcl(void)
{
   int type;
   if (tokentype == '(') {         /* ( dcl ) */
	   dcl();
	   if (tokentype != ')')
		   printf("error: missing )\n");
   } else if (tokentype == NAME)  /* variable name */
	   strcpy(name, token);
   else
	   printf("error: expected name or (dcl)\n");
   while ((type=gettoken()) == PARENS || type == BRACKETS)
	   if (type == PARENS)
		   strcat(out, " function returning");           else {
		   strcat(out, " array");
		   strcat(out, token);
		   strcat(out, " of");
	   }
}

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAXTOKEN  100
enum { NAME, PARENS, BRACKETS };
void dcl(void);
void dirdcl(void);
int gettoken(void);
int tokentype;           /* type of last token */
char token[MAXTOKEN];    /* last token string */
char name[MAXTOKEN];     /* identifier name */
char datatype[MAXTOKEN]; /* data type = char, int, etc. */
char out[1000];
main()  /* convert declaration to words */
{
   while (gettoken() != EOF) {   /* 1st token on line */
	   strcpy(datatype, token);  /* is the datatype */
	   out[0] = '\0';
	   dcl();       /* parse rest of line */
	   if (tokentype != '\n')
		   printf("syntax error\n");
	   printf("%s: %s %s\n", name, out, datatype);
   }
   return 0;
}





