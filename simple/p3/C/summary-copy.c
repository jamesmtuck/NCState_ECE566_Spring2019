/*
 * File: summary.c
 *
 * Description:
 *   This is where you implement your project 3 support.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* LLVM Header Files */
#include "llvm-c/Core.h"
#include "dominance.h"
#include "valmap.h"

/* Header file global to this project */
#include "summary.h"

typedef struct Stats_def {
  int functions;
  int globals;
  int bbs;

  int insns;
  int insns_nearby_deps;
  
  int conditional_branches;
  int calls;

  int loads;
  int loads_alloca;
  int loads_globals;

  int stores;
  int stores_alloca;
  int stores_globals;

  int gep;
  int gep_load;
  int gep_alloca;
  int gep_globals;
  int gep_gep;

  int loops; //approximated by backedges
} Stats;

void pretty_print_stats(FILE *f, Stats s, int spaces)
{
  char spc[128];
  int i;

  // insert spaces before each line
  for(i=0; i<spaces; i++)
    spc[i] = ' ';
  spc[i] = '\0';
    
  fprintf(f,"%sFunctions.......................%d\n",spc,s.functions);
  fprintf(f,"%sGlobal Vars.....................%d\n",spc,s.globals);
  fprintf(f,"%sBasic Blocks....................%d\n",spc,s.bbs);
  fprintf(f,"%sInstructions....................%d\n",spc,s.insns);
  fprintf(f,"%sInstructions (w/ nearby deps)...%d\n",spc,s.insns_nearby_deps);

  fprintf(f,"%sInstructions - Cond. Branches...%d\n",spc,s.conditional_branches);
  fprintf(f,"%sInstructions - Calls............%d\n",spc,s.calls);

  fprintf(f,"%sInstructions - Loads............%d\n",spc,s.loads);
  fprintf(f,"%sInstructions - Loads (alloca)...%d\n",spc,s.loads_alloca);
  fprintf(f,"%sInstructions - Loads (globals)..%d\n",spc,s.loads_globals);


  fprintf(f,"%sInstructions - Stores...........%d\n",spc,s.stores);
  fprintf(f,"%sInstructions - Stores (alloca)..%d\n",spc,s.stores_alloca);
  fprintf(f,"%sInstructions - Stores (globals).%d\n",spc,s.stores_globals);


  fprintf(f,"%sInstructions - gep..............%d\n",spc,s.gep);
  fprintf(f,"%sInstructions - gep (load).......%d\n",spc,s.gep_load);
  fprintf(f,"%sInstructions - gep (alloca).....%d\n",spc,s.gep_alloca);
  fprintf(f,"%sInstructions - gep (globals)....%d\n",spc,s.gep_globals);
  fprintf(f,"%sInstructions - gep (gep)........%d\n",spc,s.gep_gep);

  fprintf(f,"%sInstructions - Other............%d\n",spc,
	  s.insns-s.conditional_branches-s.loads-s.stores-s.gep-s.calls);
  fprintf(f,"%sLoops...........................%d\n",spc,s.loops);
}

void print_csv_file(const char *filename, Stats s, const char *id)
{
  FILE *f = fopen(filename,"w");
  fprintf(f,"id,%s\n",id);
  fprintf(f,"functions,%d\n",s.functions);
  fprintf(f,"globals,%d\n",s.globals);
  fprintf(f,"bbs,%d\n",s.bbs);
  fprintf(f,"insns,%d\n",s.insns);
  fprintf(f,"insns_nearby_deps,%d\n",s.insns_nearby_deps);
  fprintf(f,"branches,%d\n",s.conditional_branches);
  fprintf(f,"calls,%d\n",s.calls);
  fprintf(f,"loads,%d\n",s.loads);
  fprintf(f,"loads_alloca,%d\n",s.loads_alloca);
  fprintf(f,"loads_globals,%d\n",s.loads_globals);
  fprintf(f,"stores,%d\n",s.stores);
  fprintf(f,"stores_alloca,%d\n",s.stores_alloca);
  fprintf(f,"stores_global,%d\n",s.stores_globals);
  fprintf(f,"gep,%d\n",s.gep);
  fprintf(f,"gep_alloca,%d\n",s.gep_load);
  fprintf(f,"gep_alloca,%d\n",s.gep_alloca);
  fprintf(f,"gep_globals,%d\n",s.gep_globals);
  fprintf(f,"gep_gep,%d\n",s.gep_gep);
  fprintf(f,"loops,%d\n",s.loops);
  fclose(f);
}

Stats MyStats;

void
Summarize(LLVMModuleRef Module, const char *id, const char* filename)
{
  LLVMValueRef F;
  LLVMValueRef I;
  LLVMValueRef G;

  valmap_t headers = valmap_create();

  for(G=LLVMGetFirstGlobal(Module); G != NULL;
      G = LLVMGetNextGlobal(G))
    {
      if (LLVMGetInitializer(G)!=NULL)
	MyStats.globals++;
    }


  for(F=LLVMGetFirstFunction(Module); F != NULL; F = LLVMGetNextFunction(F))
    {
      if (LLVMGetFirstBasicBlock(F))
	MyStats.functions++;

      LLVMBasicBlockRef BB;
      for(BB=LLVMGetFirstBasicBlock(F); BB!=NULL; BB = LLVMGetNextBasicBlock(BB))
	{
	  MyStats.bbs++;

	  for(I=LLVMGetFirstInstruction(BB); I!=NULL; I = LLVMGetNextInstruction(I))
	    {
	      MyStats.insns++;

	      for(int op=0; op < LLVMGetNumOperands(I); op++)
		{
		  LLVMValueRef o = LLVMGetOperand(I,op);
		  if (LLVMIsAInstruction(o) && 
		      LLVMGetInstructionParent(o)!=BB) {
		    MyStats.insns_nearby_deps++;
		    break;
		  }		  		  
		}
	      
	      if (LLVMIsABranchInst(I)) 
		{ 
		  if (LLVMGetNumOperands(I)==3)
		    {		      
		      MyStats.conditional_branches++;
		 
		      LLVMBasicBlockRef bb1 = LLVMValueAsBasicBlock(LLVMGetOperand(I,1));
		      LLVMBasicBlockRef bb2 = LLVMValueAsBasicBlock(LLVMGetOperand(I,1));

		      if (LLVMDominates(F,bb1,BB)) 
			{
			  if (!valmap_check(headers,(LLVMValueRef)bb1)) 
			    {
			      valmap_insert(headers,(LLVMValueRef)bb1,0);
			      MyStats.loops++;
			    }
			}

		      if (LLVMDominates(F,bb2,BB)) 
			{
			  if (!valmap_check(headers,(LLVMValueRef)bb2)) 
			    {
			      valmap_insert(headers,(LLVMValueRef)bb2,0);
			      MyStats.loops++;
			    }
			}

		    }
		  else if (LLVMGetNumOperands(I)==1)
		    {
		      LLVMBasicBlockRef bb0 = LLVMValueAsBasicBlock(LLVMGetOperand(I,0)); 
		      if (LLVMDominates(F,bb0,BB)) 
			{
			  if (!valmap_check(headers,(LLVMValueRef)bb0)) 
			    {
			      valmap_insert(headers,(LLVMValueRef)bb0,0);
			      MyStats.loops++;
			    }
			}


		    }
		}

	      if (LLVMIsACallInst(I))
		MyStats.calls++;

	      if (LLVMIsALoadInst(I)) 
		{
		  MyStats.loads++;

		  LLVMValueRef op = LLVMGetOperand(I,0);
		  if (LLVMIsAAllocaInst(op))
		    MyStats.loads_alloca++;
		  else if (LLVMIsAGlobalVariable(op))
		    MyStats.loads_globals++;
		}

	      if (LLVMIsAStoreInst(I)) 
		{
		  MyStats.stores++;

		  LLVMValueRef op = LLVMGetOperand(I,1);
		  if (LLVMIsAAllocaInst(op))
		    MyStats.stores_alloca++;
		  else if (LLVMIsAGlobalVariable(op))
		    MyStats.stores_globals++;
		}

	      if (LLVMIsAGetElementPtrInst(I)) 
		{
		  MyStats.gep++;

		  LLVMValueRef op = LLVMGetOperand(I,0);
		  if (LLVMIsAAllocaInst(op))
		    MyStats.gep_alloca++;
		  else if (LLVMIsAGlobalVariable(op))
		    MyStats.gep_globals++;
		  else if (LLVMIsALoadInst(op))
		    MyStats.gep_load++;
		  else if (LLVMIsAGetElementPtrInst(op))
		    MyStats.gep_gep++;
		}
	    }
	}
    }
  

  pretty_print_stats(stdout,MyStats,0);
  print_csv_file(filename,MyStats,id);
}

