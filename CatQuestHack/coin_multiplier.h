#pragma once
#include "asm_injector.h"
#include <set>
// multiply the coins you collect by X
// the GUI will still show the actual amount of coins you collect
class coin_multiplier {
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
  coin_multiplier(HANDLE proc_handle);
  // delete porque jodete
  coin_multiplier(const coin_multiplier&) = delete;
  // Getters
  int get_multiplier();
  // Setters
  void set_multiplier(int);
};

