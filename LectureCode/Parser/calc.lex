%{
#include <math.h>
#include "calc.y.h"
%}


%%

[ \t]          ;

\n             { return NEWLINE; }
[0-9]+         { yylval.val = atoi(yytext);  return NUMBER; }

"--"           { return DECREMENT; }
"*"            { return MULTIPLY; }
"/"            { return DIVIDE; }
"+"            { return PLUS; }
"-"            { return MINUS; }
"("            { return LPAREN; }
")"            { return RPAREN; }

%%
