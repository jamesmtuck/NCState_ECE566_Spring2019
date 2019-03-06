#include <stdlib.h>
#include <stdio.h>
#include "llvm-c/Core.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"
#include "llvm-c/Analysis.h"

int
main (int argc, char ** argv)
{
  // Build for-loop here!
  LLVMModuleRef Module;

  /* Build global Module, this will be what we output */
  Module = LLVMModuleCreateWithName("main");

  // Make a function that's the instructions
  LLVMTypeRef VoidTy = LLVMVoidType();
  LLVMTypeRef VoidFnTy = LLVMFunctionType(VoidTy, NULL, 0, 0);

  LLVMValueRef Fn = LLVMAddFunction(Module,"main",VoidFnTy);

  // Fn is a function with no basic blocks
  LLVMBasicBlockRef b0 = LLVMAppendBasicBlock(Fn /* main function */,"b0");
  // Create Builder object that make instructions for us
  LLVMBuilderRef Builder = LLVMCreateBuilder();
  LLVMPositionBuilderAtEnd(Builder,b0);

  // Build for-loop here!

  LLVMBasicBlockRef b1 = LLVMAppendBasicBlock(Fn,"b1");
  LLVMBasicBlockRef b2 = LLVMAppendBasicBlock(Fn,"b2");
  LLVMValueRef i = LLVMBuildAlloca(Builder,LLVMInt64Type(),"");
  LLVMBuildStore(Builder,LLVMConstInt(LLVMInt64Type(),0,0),i);
  LLVMBuildBr(Builder,b2);

  LLVMPositionBuilderAtEnd(Builder,b2);
  
  LLVMValueRef add = LLVMBuildAdd(Builder,  
				  LLVMBuildLoad(Builder,i,"i"),
				  LLVMConstInt(LLVMInt64Type(),1,0),
				  "iplus1");
  LLVMBuildStore(Builder,add,i);
  LLVMBuildCondBr(
     Builder, 
     LLVMBuildICmp(Builder, 
                   LLVMIntSLT, 
                   add,       
                   LLVMConstInt(LLVMInt64Type(),1000,0),
                   ""),
     b2,
     b1);

  LLVMPositionBuilderAtEnd(Builder,b1);
  LLVMBuildRetVoid(Builder);


  LLVMDumpModule(Module);


#if 0
  LLVMUseRef use;
  fprintf(stderr,"All users of the instruction: ");     
  LLVMDumpValue(phi_i);
  for(use = LLVMGetFirstUse(phi_i); 
      use!=NULL; 
      use = LLVMGetNextUse(use)) 
    {
      LLVMValueRef user = LLVMGetUser(use);
      LLVMDumpValue(user);            
    }

  printf("Print all operands of the add instruction: \n");
  
  //LLVMValueRef slice = add;
  //while (LLVMGetNumOperands(slice)>0){

    for(unsigned op=0; op< LLVMGetNumOperands(add); op++) {
      LLVMValueRef definition = LLVMGetOperand(add,op);
      fprintf(stderr,"Definition of op=%d is:",op); 
      LLVMDumpValue(definition);
    } 

    //}
#endif

  // Verify Module is correct
  char *msg;
  LLVMBool res = LLVMVerifyModule(Module,LLVMPrintMessageAction,&msg);
  
  // Dump module if verified
  if (!res) 
    LLVMWriteBitcodeToFile(Module,"main.bc");
  else
    printf("main.bc not created\n");

  /* Return an error status if it failed */
  return 0;
}
