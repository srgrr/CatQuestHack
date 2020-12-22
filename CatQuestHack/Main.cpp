#include "cat_quest.h"
#include "proc_util.h"
#include <Windows.h>

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
  while (true) {
    int content = 1000000000;
    proc_util::write_to_proc_mem(cat_quest_handle, (void*)money_addr, &content, sizeof(int));
  }
}