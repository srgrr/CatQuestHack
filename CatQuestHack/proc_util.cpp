#include "proc_util.h"

std::vector<DWORD> proc_util::get_all_processes() {
  DWORD proc_array[1024];
  DWORD needed;
  std::vector<DWORD> ret;
  
  if (EnumProcesses(proc_array, sizeof(proc_array), &needed)) {
    for (std::uint32_t i = 0; i < 1024; ++i) {
      ret.push_back(proc_array[i]);
    }
  }

  return ret;
}


std::string proc_util::get_process_name(DWORD process_id) {
  HANDLE process_handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, process_id);
  TCHAR tchar_process_name[MAX_PATH];

  if (process_handle) {
    if (!GetModuleFileNameEx(process_handle, 0, tchar_process_name, MAX_PATH)) {
    }
    CloseHandle(process_handle);
  }

  std::wstring wret(&tchar_process_name[0]);
  return std::string(wret.begin(), wret.end());
}

HANDLE proc_util::get_proc_handle(std::string process_name) {
  for (DWORD process_id : proc_util::get_all_processes()) {
    std::string current_process_name = proc_util::get_process_name(process_id);
    if (
      process_name.size() < current_process_name.size() &&
      std::equal(process_name.rbegin(), process_name.rend(), current_process_name.rbegin())
      ) {
        HANDLE ret = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, process_id);
        return ret;
    }
  }
  return nullptr;
}

void proc_util::write_to_proc_mem(DWORD proc_handle, void* mem_pos, void* content, std::uint32_t bytes) {
}
