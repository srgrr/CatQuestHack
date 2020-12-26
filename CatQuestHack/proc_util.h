#pragma once
#include <Windows.h>
#include <psapi.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>

class proc_util {
public:
  // get the PIDs of all the running processes
  static std::vector<DWORD> get_all_processes();
  // get the process name given a PID, must have proper permissions to work properly
  static std::string get_process_name(DWORD process_id);
  // get a HANDLE with WR and VM query operation permissions, may fail if the targeted process
  // is somewhow protected
  static HANDLE get_proc_handle(std::string process_name);
  // write to process memory, must have a handle with proper permissions
  static bool write_to_proc_mem(HANDLE proc_handle, void *mem_pos, void *content, std::uint32_t bytes);
  // get a map < module_name, module_base_address > for a given process
  static std::map< std::string, LPCVOID > get_module_base_addresses(HANDLE proc_handle);
  // get the module base address of a given module for a given process, uses the thing above this one
  static LPCVOID get_module_base_address(std::map< std::string, LPCVOID >& process_modules, std::string mod_name);
  // same, this one calls the one above 
  static LPCVOID get_module_base_address(HANDLE proc_handle, std::string mod_name);
};

