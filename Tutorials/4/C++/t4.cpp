#include <stdlib.h>
#include <stdio.h>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"

#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/FileSystem.h"

using namespace llvm;

static LLVMContext TheContext;

int main (int argc, char ** argv)
{  
  /* Build global Module, this will be what we output */
  Module *M = new Module("Tutorial4", TheContext);

  IRBuilder<> Builder(TheContext);

  // Create void function type with no arguments
  FunctionType *FunType = FunctionType::get(Builder.getVoidTy(),false);

  Function *F = Function::Create(FunType,GlobalValue::ExternalLinkage, "main", M);

  // Build for-loop here!

  // F is a function with no basic blocks
  BasicBlock *b0 = BasicBlock::Create(TheContext, "b0", F);

  BasicBlock *b1 = BasicBlock::Create(TheContext, "b1", F);
  BasicBlock *b2 = BasicBlock::Create(TheContext, "b2", F);

  // Set insertion point in b0
  Builder.SetInsertPoint(b0);

  AllocaInst *i = Builder.CreateAlloca(Builder.getInt64Ty());
  Builder.CreateStore(Builder.getInt64(0),i);
  Builder.CreateBr(b2);

  Builder.SetInsertPoint(b2);

  Value * add = Builder.CreateAdd(Builder.CreateLoad(i),Builder.getInt64(1),"iplus1");
  Builder.CreateStore(add, i);
  Builder.CreateCondBr(Builder.CreateICmpSLT(add,Builder.getInt64(1000)),b2,b1);

  Builder.SetInsertPoint(b1);
  Builder.CreateRet(Builder.getInt64(0));
/*
  
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

*/

M->print(errs(),nullptr);

//Write module to file
std::error_code EC;
raw_fd_ostream OS("main.bc",EC,sys::fs::F_None);  
WriteBitcodeToFile(*M,OS);


  /* Return an error status if it failed */
  return 0;
}
