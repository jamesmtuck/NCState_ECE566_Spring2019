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
  int insns_nearby_dep;
  
  int allocas;

  int loads;
  int loads_alloca;
  int loads_globals;

  int stores;
  int stores_alloca;
  int stores_globals;
  
  int conditional_branches;
  int calls;

  int gep;
  int gep_load;
  int gep_alloca;
  int gep_globals;
  int gep_gep;

  int loops; //approximated by backedges
  int floats;
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
  fprintf(f,"%sInstructions - Nearby Dep.......%d\n",spc,s.insns_nearby_dep);

  fprintf(f,"%sInstructions - Cond. Branches...%d\n",spc,s.conditional_branches);
  fprintf(f,"%sInstructions - Calls............%d\n",spc,s.calls);

  fprintf(f,"%sInstructions - Allocas..........%d\n",spc,s.allocas);
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
  fprintf(f,"%sFloats..........................%d\n",spc,s.floats);
}

void print_csv_file(const char *filename, Stats s, const char *id)
{
  FILE *f = fopen(filename,"w");
  fprintf(f,"id,%s\n",id);
  fprintf(f,"functions,%d\n",s.functions);
  fprintf(f,"globals,%d\n",s.globals);
  fprintf(f,"bbs,%d\n",s.bbs);
  fprintf(f,"insns,%d\n",s.insns);
  fprintf(f,"insns_nearby_dep,%d\n",s.insns_nearby_dep);
  fprintf(f,"allocas,%d\n",s.allocas);
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
  fprintf(f,"floats,%d\n",s.floats);
  fclose(f);
}

Stats MyStats;

void
Summarize(LLVMModuleRef Module, const char *id, const char* filename)
{
  pretty_print_stats(stdout,MyStats,0);
  print_csv_file(filename,MyStats,id);
}

