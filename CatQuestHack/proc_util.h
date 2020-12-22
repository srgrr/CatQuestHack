#pragma once
#include <Windows.h>
#include <psapi.h>
#include <iostream>
#include <vector>
#include <map>

class proc_util {
public:
  static std::vector<DWORD> get_all_processes();
  static std::string get_process_name(DWORD process_id);
  static HANDLE get_proc_handle(std::string process_name);
  static bool write_to_proc_mem(HANDLE proc_handle, void *mem_pos, void *content, std::uint32_t bytes);
  static std::map< std::string, LPCVOID > get_module_base_addresses(HANDLE proc_handle);
  static LPCVOID get_module_base_address(std::map< std::string, LPCVOID>& process_modules, std::string mod_name);
  static LPCVOID get_module_base_address(HANDLE proc_handle, std::string mod_name);
};

