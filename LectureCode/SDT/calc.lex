%{
#include <math.h>
#include "llvm-c/Core.h"
#include "calc.y.h"
%}


%%

[ \t]          ;

\n             { return NEWLINE; }
[0-9]+         { yylval.num = atoi(yytext);  return NUMBER; }

"+"            { return PLUS; }
"-"            { return MINUS; }
"("            { return LPAREN; }
")"            { return RPAREN; }
"x"            { return DONE; }

%%
