#pragma once
#include <Windows.h>
#include <psapi.h>
#include <iostream>
#include <vector>

class proc_util {
public:
  static std::vector<DWORD> get_all_processes();
  static std::string get_process_name(DWORD process_id);
  static HANDLE get_proc_handle(std::string process_name);
  static void write_to_proc_mem(DWORD proc_handle, void *mem_pos, void *content, std::uint32_t bytes);
};

