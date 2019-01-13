#include <assert.h>

#define chatting printf
#define __Olden_panic assert
#define ALLOC(nd,sz) (char *)malloc(sz)
