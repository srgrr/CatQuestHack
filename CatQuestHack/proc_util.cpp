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

byte* proc_util::read_from_proc(HANDLE proc_handle, LPCVOID base_address, int byte_amount) {
  byte *buffer = new byte[byte_amount];
  ReadProcessMemory(proc_handle, base_address, (LPVOID)buffer, byte_amount, NULL);
  return buffer;
} 

bool proc_util::write_to_proc_mem(HANDLE proc_handle, LPCVOID mem_pos, LPCVOID content, std::uint32_t bytes) {
  return WriteProcessMemory(proc_handle, (LPVOID)mem_pos, content, bytes, NULL);
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

LPCVOID proc_util::alloc(HANDLE proc_handle, int byte_amount) {
  return VirtualAllocEx(proc_handle, NULL, byte_amount, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
}

std::vector<LPCVOID> proc_util::find_pattern(HANDLE proc_handle, LPCVOID region_base, int region_bytes, byte* pattern, int pattern_bytes) {
  std::vector< int > fail_function(pattern_bytes + 1);
  fail_function[0] = -1;
  int l = -1, r = 0;
  while (r < pattern_bytes) {
    while (l >= 0 && pattern[l] != pattern[r]) l = fail_function[l];
    ++l, ++r;
    fail_function[r] = l;
  }
  std::vector< LPCVOID > ret;
  l = 0, r = 0;
  while (r < pattern_bytes) {
    LPCVOID current_pointer = (LPCVOID)((DWORD)region_base + r);
    byte current_byte = proc_util::read_from_proc(proc_handle, current_pointer, 1)[0];
    while (l >= 0 && current_byte != pattern[l]) {
      l = fail_function[l];
      ++l, ++r;
      if (l == pattern_bytes) {
        l = fail_function[l];
        ret.push_back(current_pointer);
      }
    }
  }
  return ret;
}

std::vector< MEMORY_BASIC_INFORMATION > proc_util::get_memory_regions(HANDLE proc_handle) {
  std::vector< MEMORY_BASIC_INFORMATION > ret;
  DWORD addr = 0;
  SIZE_T last_amount = 1;
  while (last_amount != 0) {
    MEMORY_BASIC_INFORMATION buffer;
    last_amount = VirtualQueryEx(proc_handle, (LPCVOID)addr, &buffer, sizeof(buffer));
    ret.push_back(buffer);
    addr = (DWORD)buffer.BaseAddress + buffer.RegionSize;
  }
  return ret;
}
