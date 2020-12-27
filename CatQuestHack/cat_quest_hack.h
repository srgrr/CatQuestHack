#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <map>
#include "coin_multiplier.h"

// Backend class
class cat_quest_hack {
private:
  // Constants
  const static SIZE_T code_page_size = 0x100000;
  const std::string proc_name = "Cat Quest.exe";
  // Class attributes
  HANDLE proc_handle;
  std::map< std::string, LPCVOID > proc_modules;
  std::vector< MEMORY_BASIC_INFORMATION > memory_regions;
  // cheat features
  coin_multiplier* coin_mul;

  // private methods
  void _set_proc_handle();
  void _set_proc_modules();
  void _set_memory_regions();
public:
  cat_quest_hack();
};

