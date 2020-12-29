#pragma once
#include "proc_util.h"
#include "asm_injector.h"

class xp_multiplier {
private:
  const static byte asm_prefix[];
  const static byte asm_code[];
  const static int multiplier_offset;
  const static int prefix_length;
  int multiplier;
  LPCVOID code_page;
  LPCVOID instruction_address;
  HANDLE proc_handle;
  void _solve_injection_point();
  void _update_multiplier();
  void _inject();
public:
  xp_multiplier(HANDLE proc_handle);
  // delete porque jodete
  xp_multiplier(const xp_multiplier&) = delete;
  // Getters
  int get_multiplier();
  // Setters
  void set_multiplier(int);
};

