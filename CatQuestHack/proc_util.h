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
  // read from a process
  static byte* read_from_proc(HANDLE proc_handle, LPCVOID base_address, int byte_amount);
  // write to process memory, must have a handle with proper permissions
  static bool write_to_proc_mem(HANDLE proc_handle, LPCVOID mem_pos, LPCVOID content, std::uint32_t bytes);
  // get a map < module_name, module_base_address > for a given process
  static std::map< std::string, LPCVOID > get_module_base_addresses(HANDLE proc_handle);
  // get the module base address of a given module for a given process, uses the thing above this one
  static LPCVOID get_module_base_address(std::map< std::string, LPCVOID >& process_modules, std::string mod_name);
  // same, this one calls the one above 
  static LPCVOID get_module_base_address(HANDLE proc_handle, std::string mod_name);
  // alloc memory in a foreign process, get the pointer to the new region
  static LPCVOID alloc(HANDLE proc_handle, int byte_amount);
  // find a memory pattern in a memory region of a process
  static std::vector< LPCVOID > find_pattern(HANDLE proc_handle, LPCVOID region_base, int region_bytes, byte* pattern, int pattern_bytes);
  // return all the memory region of a process
  static std::vector< MEMORY_BASIC_INFORMATION > get_memory_regions(HANDLE proc_handle);
};

