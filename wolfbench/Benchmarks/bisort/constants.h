/* $Log: constants.h,v $
* Revision 1.1  2006/01/10 20:52:20  liuwei
* Original Olden v1.01
*
 * Revision 1.2  1995/09/29  13:59:36  mcc
 * Added #define PANICS (probably should enum someday).
 *
 *
 * global Olden constants
 *
 */

#ifndef CONSTANTS_DEFINED
#define CONSTANTS_DEFINED

#define IDMASK 127
#define MAXPROC 128
#define N_ADDR_BITS 25
#define PN_BITS 7

#ifndef NULL
#define NULL 0
#endif

#ifndef _ASM_
extern int __NumNodes,__NDim;
extern unsigned int __MyNodeId;
extern int __PanicFlag;

typedef unsigned int word_t;

#define ADDRONLY(ptr) ((void *) ((unsigned int) ptr >> 7))
#define PID(ptr) ((int) ptr & IDMASK)

#endif

#define PANIC_ENQ 4001
#define PANIC_WRITE_SZ 5001
#define PANIC_NO_MCB 1
#define PANIC_MSG_TYPE 2
#define PANIC_NO_RPCB 1001
#define PANIC_RPC_SRC 1002
#define PANIC_NO_RPC_REPB 1003
#define PANIC_RPC_TYPE 1004
#endif
