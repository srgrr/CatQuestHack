#pragma once
#include "asm_injector.h"
// multiply the coins you collect by X
// the GUI will still show the actual amount of coins you collect
class coin_multiplier {
private:
  const static byte asm_prefix[];
  const static byte asm_code[];
  const static int multiplier_offset;
  const static int prefix_length;
  LPCVOID code_page;
  LPCVOID instruction_address;
  HANDLE proc_handle;
public:
  coin_multiplier(HANDLE proc_handle);
  void inject();
  void change_multiplier(int);
};

