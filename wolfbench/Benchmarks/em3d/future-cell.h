/* $Log: future-cell.h,v $
/* Revision 1.1  2006/01/10 20:52:20  liuwei
/* Original Olden v1.01
/*
 * Revision 1.1  1995/09/22  17:34:36  mcc
 * Initial revision
 *
 * Revision 1.1  1995/09/13  18:59:52  mcc
 * Initial revision
 *
 *
 */
/* sparc implementation of future-cell.h */

#ifndef _FUTURE_CELL_
#define _FUTURE_CELL_

#include "constants.h"

#ifndef _ASM_
extern void __Steal(/* sp, fc, fp, fnptr */);
extern void __Overflow();
extern void __MultiSteal();
extern void __BarrierResume();

typedef struct fc_impl {
    /* lazy scheme IV -- was state */
    unsigned int       modmask[2];
    struct fc_int *next;
         /* The saved state: this is either the state to */
         /* steal, or the state of the waiting thread. */
    word_t  returnPC;
    word_t  fp;
    word_t      waiting;   /* 8 byte align */
} future_cell_impl;

typedef struct fc_int {
    future_cell_impl impl;
    word_t	value;		/* The cell's value (int or pointer) */
    int dummy;
  } future_cell_int;

typedef struct fc_pointer {
    future_cell_impl impl;
    word_t	*value;		/* The cell's value (int or pointer) */
    int dummy;
  } future_cell_pointer;

typedef struct fc_float {
    future_cell_impl impl;
    float       value;	
    int dummy;
  } future_cell_float;

typedef struct fc_double {
    future_cell_impl impl;
    double      value;
  } future_cell_double;

extern future_cell_int *__TopFutureStk();
extern future_cell_int *__PopFutureStk();

#endif


/* offsets into a future cell */
#define FCELL_MODMASK0	0
#define FCELL_MODMASK1  4
#define FCELL_NEXT	8
#define FBUF_PC		12
#define FBUF_FP		16
#define FCELL_VALUE	24

/* future cell states */
#define FC_EMPTY	0
#define FC_FULL		0
#define FC_STOLEN	1
#define FC_WAITING	2
#define FC_FULLRET  3
#define FC_REMWAITING 5
#define FC_MASK 7

/* MULTIFUTURE stuff */
#ifndef _ASM_
typedef struct mc_sync {
  word_t count;
  word_t fp;
  word_t pc;
  word_t stealaddr;
} multi_cell_sync;

typedef struct mc_int {
    multi_cell_sync *sync;
    word_t	value;		/* The cell's value (int or pointer) */
  } multi_cell_int;

typedef struct mc_pointer {
    multi_cell_sync *sync;
    word_t	*value;		/* The cell's value (int or pointer) */
  } multi_cell_pointer;

typedef struct mc_float {
    multi_cell_sync *sync;
    float       value;	
  } multi_cell_float;

typedef struct mc_double {
    multi_cell_sync *sync;
    int dummy; /* eight byte align double */
    double      value;
  } multi_cell_double;

/* all structures must begin at 8 and be <= 8 bytes */
typedef struct mc_struct {
    multi_cell_sync *sync;
    int dummy; /* eight byte align double */
    word_t      value[2];
  } multi_cell_struct;

#endif

#define MCELL_COUNT 0
#define MCELL_FP 4
#define MCELL_PC 8
#define MCELL_STEALADDR 12

#endif















