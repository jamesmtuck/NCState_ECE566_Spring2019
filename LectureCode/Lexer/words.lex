%{
#include <stdio.h>
%}


%%

.            ;
\n+           printf("newline\n");
[a-z]+       printf("word (%s)\n",yytext);



%%
