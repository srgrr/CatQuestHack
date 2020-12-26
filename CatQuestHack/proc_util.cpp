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

  std::basic_string<TCHAR> wret(tchar_process_name);
  return std::string(wret.begin(), wret.end());
}

HANDLE proc_util::get_proc_handle(std::string process_name) {
  for (DWORD process_id : proc_util::get_all_processes()) {
    std::string current_process_name = proc_util::get_process_name(process_id);
    if (
      process_name.size() <= current_process_name.size() &&
      std::equal(process_name.rbegin(), process_name.rend(), current_process_name.rbegin())
      ) {
      HANDLE ret = OpenProcess(PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, process_id);
        return ret;
    }
  }
  return nullptr;
}

bool proc_util::write_to_proc_mem(HANDLE proc_handle, void* mem_pos, void* content, std::uint32_t bytes) {
  return WriteProcessMemory(proc_handle, (LPVOID)mem_pos, (LPCVOID)content, bytes, NULL);
}

std::map< std::string, LPCVOID > proc_util::get_module_base_addresses(HANDLE proc_handle) {
  std::map< std::string, LPCVOID > ret;
  HMODULE proc_modules[1024];
  DWORD arr_byte_len;

  if (EnumProcessModules(proc_handle, proc_modules, sizeof(proc_modules), &arr_byte_len)) {
    for (int i = 0; i < (arr_byte_len / sizeof(HMODULE)); ++i) {
      TCHAR mod_name[MAX_PATH];
      if (GetModuleFileNameEx(proc_handle, proc_modules[i], mod_name, sizeof(mod_name) / sizeof(TCHAR))) {
        std::basic_string<TCHAR> wmod_name(mod_name);
        ret[std::string(wmod_name.begin(), wmod_name.end())] = (LPCVOID)proc_modules[i];
      }
    }
  }
  return ret;
}

LPCVOID proc_util::get_module_base_address(std::map<std::string, LPCVOID>& process_modules, std::string mod_name) {
  for (auto& kv : process_modules) {
    std::string proc_mod_name = kv.first;
    if (mod_name.size() <= proc_mod_name.size() && std::equal(mod_name.rbegin(), mod_name.rend(), proc_mod_name.rbegin())) {
      return kv.second;
    }
  }
  return nullptr;
}

LPCVOID proc_util::get_module_base_address(HANDLE proc_handle, std::string mod_name) {
  std::map< std::string, LPCVOID > module_base_addresses = proc_util::get_module_base_addresses(proc_handle);
  return proc_util::get_module_base_address(module_base_addresses, mod_name);
}
