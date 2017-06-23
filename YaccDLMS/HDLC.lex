%{


#include <stdio.h>
#include "y.tab.h"



%}

%%
7[eE]           return PD;
[0-9a-zA-Z]{2}  return BYTE;
%%



