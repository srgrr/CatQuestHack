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
  std::cout << std::hex << "Final address from data " << (DWORD)cat_quest::money_value.get_final_address(cat_quest_handle) << std::endl;
}