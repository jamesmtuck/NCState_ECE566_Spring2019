
%{
#include <stdio.h>

FILE *yyin;
%}

%union {
  int val;
}

%token NUMBER PLUS MINUS LPAREN RPAREN NEWLINE MULTIPLY DIVIDE DECREMENT

%type <val> NUMBER expr

%left DECREMENT
%left PLUS MINUS
%left MULTIPLY DIVIDE

%start expr_list

%%

expr_list:  NEWLINE                    {}
            |  expr NEWLINE            { printf("=%d\n",$1); }
            | expr_list expr NEWLINE  { printf("=%d\n",$2); }
            | expr_list NEWLINE {}
;

expr :   expr PLUS expr                  { $$ = $1 + $3; }
       | expr MINUS expr                 { $$ = $1 - $3; } 
       | expr MULTIPLY expr                 { $$ = $1 * $3; } 
| expr DIVIDE expr                 { if ($3 != 0) $$ = $1 / $3;  else { printf("Don't divide by zero please!\n"); $$=0; } } 
       | LPAREN expr RPAREN              { $$ = $2; }
        | NUMBER                          { $$ = $1 ; }
       | MINUS expr                      { $$ = - $2 ; }
       | DECREMENT expr                  { $$ = $2 - 1; }
;

%%

int yyerror(char *msg) {
  printf("%s",msg);
  return 0;
}

int main()
{
  yyin = stdin;

  yyparse();

  return 0;
}
