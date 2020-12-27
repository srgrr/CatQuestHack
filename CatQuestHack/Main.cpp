#include "cat_quest.h"
#include "proc_util.h"
#include "coin_multiplier.h"
#include <Windows.h>
#include <fstream>

LPCVOID COIN_ADDR = (LPCVOID)0x1C82C2F1;

int main(int argc, char** argv) {
  HANDLE cat_quest_handle = proc_util::get_proc_handle(cat_quest::proc_name);
  if (cat_quest_handle == nullptr) {
    MessageBoxA(
      NULL,
      "Couldn't get Cat Quest handle, is the game open? do you have permissions to access the process?",
      "ERROR",
      MB_OK | MB_ICONQUESTION);
  }

  auto mono_dll_base_address = proc_util::get_module_base_address(cat_quest_handle, "mono.dll");
  cat_quest::money_value.set_base_address((DWORD)mono_dll_base_address);
  LPCVOID money_addr = cat_quest::money_value.get_final_address(cat_quest_handle);
  coin_multiplier multiplier(cat_quest_handle);
  multiplier.run(COIN_ADDR);
  while (true) {
    int x;
    std::cin >> x;
    multiplier.change_multiplier(x);
  }
}