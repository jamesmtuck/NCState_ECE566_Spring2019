// review 

#include <stdio.h>

enum Opcode {
  ADD = 0,
  SUB = 1
};

enum Register {
  R0 = 0,
  R1 = 1,
  R2 = 2
};

typedef struct {
  Opcode opcode;
  Register dest; // int means the register number
  Register src1;
  Register src2;
} Instruction;

int main()
{
  Instruction add = { ADD, R0, R1, R2 };
  Instruction sub = { SUB, R1, R2, R1 };


  return 0;
}
