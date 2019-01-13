
/*******************************************************************************
 *  The BYTE UNIX Benchmarks - Release 3
 *          Module: hanoi.c   SID: 3.3 5/15/91 19:30:20
 *          
 *******************************************************************************
 * Bug reports, patches, comments, suggestions should be sent to:
 *
 *	Ben Smith, Rick Grehan or Tom Yager
 *	ben@bytepb.byte.com   rick_g@bytepb.byte.com   tyager@bytepb.byte.com
 *
 *******************************************************************************
 *  Modification Log:
 *  $Header: /mounts/iacoma20/disks/0/cvsroot/benchmarks/kernels/hanoi/hanoi.c,v 1.1 2004/04/15 20:18:42 renau Exp $
 *  August 28, 1990 - Modified timing routines (ty)
 *
 ******************************************************************************/
char SCCSid[] = "@(#) @(#)hanoi.c:3.3 -- 5/15/91 19:30:20";

#define other(i,j) (6-(i+j))

#include <stdio.h>
#include <stdlib.h>

unsigned long iter = 0;
int num[4];
long cnt;

void mov(int n,int f,int t)
{
  if(n == 1) {
    num[f]--;
    num[t]++;
  }else{
    int o = other(f,t);
    mov(n-1,f,o);
    mov(1,f,t);
    mov(n-1,o,t);
  }
}

int main(int argc, char **argv)
{
  int disk=10; /* default number of disks */
  int iterations;
  int i;

  if (argc < 2) {
    printf("Usage: %s iterations [disks]\n", argv[0]);
    exit(1);
  }
  iterations = atoi(argv[1]);
  if(argc > 2) 
    disk = atoi(argv[2]);
  num[1] = disk;

  for(i=0;i<iterations;i++) {
    mov(disk,1,3);
    iter++;
  }
  
  exit(0);
}

