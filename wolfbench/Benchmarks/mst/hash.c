#include "mst.h"
#include <stdlib.h>
#define localassert(num,a) if (!(a)) {chatting("Assertion failure:%d in hash\n",num); exit(-1);}

static int remaining = 0;
static char *temp;

static char *localmalloc(int size) 
{
  char *blah;
  
  if (size>remaining) 
    {
      temp = (char *) malloc(32768);
      if (!temp) chatting("Error! malloc returns null\n");
      remaining = 32768;
    }
  blah = temp;
  temp += size;
  remaining -= size;
  return blah;
}

#define localfree(sz)

Hash MakeHash(int size, int (*map)(unsigned int)) 
{
  Hash retval;
  int i;

  retval = (Hash) localmalloc(sizeof(*retval));
  retval->array = (HashEntry *) localmalloc(size*sizeof(HashEntry));
  for (i=0; i<size; i++)
    retval->array[i]=NULL;
  retval->mapfunc = map;
  retval->size = size;
  return retval;
}

void *HashLookup(unsigned int key, Hash hash)
{
  int j;
  HashEntry ent;

  j = (hash->mapfunc)(key);
  localassert(1,j>=0);
  localassert(2,j<hash->size);
  for (ent = hash->array[j]; ent && ent->key!=key; ent=ent->next);
  if (ent) return ent->entry;
  return NULL;
}

void HashInsert(void *entry,unsigned int key,Hash hash) 
{
  HashEntry ent;
  int j;
  
  localassert(3,!HashLookup(key,hash));
  
  j = (hash->mapfunc)(key);
  ent = (HashEntry) localmalloc(sizeof(*ent));
  ent->next = hash->array[j];
  hash->array[j]=ent;
  ent->key = key;
  ent->entry = entry;
}

void HashDelete(unsigned int key,Hash hash)
{
  HashEntry *ent;
  HashEntry tmp;
  int j;

  j = (hash->mapfunc)(key);
  for (ent=&(hash->array[j]); (*ent) && (*ent)->key!=key; ent=&((*ent)->next));
  localassert(4,*ent);
  tmp = *ent;
  *ent = (*ent)->next;
  localfree(tmp);
}

  
  


