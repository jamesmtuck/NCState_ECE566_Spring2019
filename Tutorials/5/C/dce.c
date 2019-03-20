#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"
#include "llvm-c/Analysis.h"
#include "worklist.h"

void NoOptimization(LLVMModuleRef Module) 
{
  printf("NoOptimization is running!\n");

  int i = 0;
  for (LLVMValueRef F = LLVMGetFirstFunction(Module);
       F!=NULL;
       F=LLVMGetNextFunction(F))
    {
      // Use each function, F
      
      //LLVMDumpValue(F);

      for(LLVMBasicBlockRef BB=LLVMGetFirstBasicBlock(F);
	  BB!=NULL;
	  BB = LLVMGetNextBasicBlock(BB))
	{
	  // get each basic block in F

	  for (LLVMValueRef I = LLVMGetFirstInstruction(BB);
	       I != NULL;
	       I = LLVMGetNextInstruction(I))
	    {
	      // loop over all instructions

	      printf("%d: \n", i++); LLVMDumpValue(I);
	      printf("\n");
	    }

	}


    }

}

int isDead(LLVMValueRef I)
{
  // All conditions passed

  // Not dead, because there are uses
  if(LLVMGetFirstUse(I)!=NULL)
    return 0;

#if 0
  if(LLVMGetFirstUse(I)==NULL) {
    printf("No uses here: \n");
    LLVMDumpValue(I);
   return 0;
  } else {
    LLVMUseRef use;
    LLVMDumpValue(I);
    printf(" ---- Used by: \n");
    for(use = LLVMGetFirstUse(I);
	use != NULL;
	use = LLVMGetNextUse(use))
      {
	LLVMValueRef j = LLVMGetUser(use);
	LLVMDumpValue(j);
      }
    printf("\n");
  }
#endif
  
  LLVMOpcode opcode = LLVMGetInstructionOpcode(I);
  switch(opcode) {
  // when in doubt, keep it! add opcode here to remove:
  case LLVMAdd:
  case LLVMFAdd: 	
  case LLVMSub:
  case LLVMFSub: 	
  case LLVMMul:
  case LLVMFMul: 	
  case LLVMUDiv:	
  case LLVMSDiv:	
  case LLVMFDiv:	
  case LLVMURem: 	
  case LLVMSRem: 	
  case LLVMFRem: 	
  case LLVMShl: 	
  case LLVMLShr: 	
  case LLVMAShr: 	
  case LLVMAnd: 	
  case LLVMOr: 	
  case LLVMXor: 	
  case LLVMAlloca:
  case LLVMGetElementPtr: 	
  case LLVMTrunc: 	
  case LLVMZExt: 	
  case LLVMSExt: 	
  case LLVMFPToUI: 	
  case LLVMFPToSI: 	
  case LLVMUIToFP: 	
  case LLVMSIToFP: 	
  case LLVMFPTrunc: 	
  case LLVMFPExt: 	
  case LLVMPtrToInt: 	
  case LLVMIntToPtr: 	
  case LLVMBitCast: 	
  case LLVMAddrSpaceCast: 	
  case LLVMICmp: 	
  case LLVMFCmp: 	
  case LLVMPHI: 
  case LLVMSelect: 
  case LLVMExtractElement: 	
  case LLVMInsertElement: 	
  case LLVMShuffleVector: 	
  case LLVMExtractValue: 	
  case LLVMInsertValue: 
    // Success!
    return 1;

  case LLVMLoad: if(!LLVMGetVolatile(I)) return 1; // Success

  // all others must be kept
  default:
    break;
  }

  return 0;
}

int DCE_count=0;

void RunDeadCodeElimination(LLVMModuleRef Module) 
{

  worklist_t worklist = worklist_create();

  for (LLVMValueRef F = LLVMGetFirstFunction(Module);
       F!=NULL;
       F=LLVMGetNextFunction(F))
    {
      // Use each function, F
      
      //LLVMDumpValue(F);

      for(LLVMBasicBlockRef BB=LLVMGetFirstBasicBlock(F);
	  BB!=NULL;
	  BB = LLVMGetNextBasicBlock(BB))
	{
	  // get each basic block in F

	  for (LLVMValueRef I = LLVMGetFirstInstruction(BB);
	       I != NULL;
	       I = LLVMGetNextInstruction(I))
	    {
	      // loop over all instructions
	      if (isDead(I)) {
		// take some action
		worklist_insert(worklist,I);
	      }
	    }

	}


    }

  // While there’s an instruction in the worklist
  while(!worklist_empty(worklist)) {
    // take one instruction out of the list
    LLVMValueRef I = worklist_pop(worklist);

    // Erase it from its parent basic block
    
    // Check if instruction is dead because it may have been
    // added in if-statement below and we did not check to see
    // if it was dead first
    if (isDead(I))
      {
	// Loop over operands of I
	for(unsigned i=0; i<LLVMGetNumOperands(I); i++)
	  {
	    LLVMValueRef J = LLVMGetOperand(I,i);
	    
	    // Add to worklist only if J is an instruction
	    // Note, J still has one use (in I) so the isDead routine
	    // would return false, so we’d better not check that yet.
	    // This forces us to check in the if statement above.
	    if (LLVMIsAInstruction(J))
	      worklist_insert(worklist,J);
	  }
	DCE_count++;
	LLVMInstructionEraseFromParent(I);
      }

  }


}


int main (int argc, char ** argv)
{
/* some basic error handling */
  if (argc < 3) {
    fprintf(stderr,"Not enough files specified.\n");
    return 0;
  }

  LLVMMemoryBufferRef Buff=NULL;
  char *outMessage=NULL;

  /* 1. Read contents of object file from command line argv[1] */  
  LLVMCreateMemoryBufferWithContentsOfFile(argv[1],&Buff,&outMessage);

  LLVMModuleRef Module=NULL;
  /* 2. Try to parse buffer into a legal Module */
  if (!Buff || LLVMParseBitcode(Buff,&Module,&outMessage))
    {
      /* handle error */
      printf("Error opening file: %s\n",outMessage);
      return 1;
    }
  
  /* 3. Do optimization on Module */
  //  NoOptimization(Module);
  RunDeadCodeElimination(Module);


  /* 4. Save result to a new file (argv[2]) to preserve original */
  char *msg;
  LLVMBool res = LLVMVerifyModule(Module, LLVMPrintMessageAction, &msg);
  if (!res) 
    LLVMWriteBitcodeToFile(Module,argv[2]);
  else
    fprintf(stderr,"Error: %s not created.\n",argv[2]); 

  fprintf(stderr,"Removed %d instructions.",DCE_count);

  return 0;
}
