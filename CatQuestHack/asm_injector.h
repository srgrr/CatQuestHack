#pragma once
#include "proc_util.h"
class asm_injector {
private:
  // Handle to the victim process
  HANDLE proc_handle;
  // Address where the injection will occur
  LPCVOID injection_address;
  // The injector just replaces the instruction in the given address with a long jmp
  // and fills the gaps (if any) withs nops, the actual instruction is moved to the
  // injected page, the surplus value means the actual number of bytes we must move
  // from the original code to the injected page
  int prefix_size;
  byte* code_to_inject;
  int bytes_to_inject;
public:
  asm_injector(HANDLE, LPCVOID, int, byte*, int);
  // inject the code in a given address in the other process mem space
  LPCVOID inject();

  // getters
  HANDLE get_proc_handle();
  LPCVOID get_injection_address();
  int get_prefix_size();
  byte* get_code_to_inject();
  int get_bytes_to_inject();
  // builder, as the injector thing is complicated af
  class builder;
};

class asm_injector::builder {
private:
  HANDLE proc_handle;
  LPCVOID injection_address;
  int prefix_size;
  byte* code_to_inject;
  int bytes_to_inject;
public:
  builder();
  builder with_proc_handle(HANDLE);
  builder with_injection_address(LPCVOID);
  builder with_prefix_size(int);
  builder with_code_to_inject(byte*);
  builder with_bytes_to_inject(int);
  asm_injector build();
};