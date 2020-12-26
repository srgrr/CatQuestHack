#pragma once
#include "asm_injector.h"
// multiply the coins you collect by X
// the GUI will still show the actual amount of coins you collect
class coin_multiplier {
private:
  const static byte asm_code[];
  HANDLE proc_handle;
public:
  coin_multiplier(HANDLE proc_handle);
  bool run(LPCVOID base_address);
};

