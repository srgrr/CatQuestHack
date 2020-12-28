#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <map>
#include "multilevel_pointer.h"
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
  // must be resolved in runtime due to the mono.dll base address
  LPCVOID coins_address;
  LPCVOID xp_address;
  // cheat features
  coin_multiplier* coin_mul;

  // private methods
  void _set_proc_handle();
  void _set_proc_modules();
  void _set_memory_regions();
  void _set_coins_address();
  void _set_xp_address();
public:
  cat_quest_hack();
  void set_coin_multiplier(int amount);
  void set_xp_multiplier(int amount);
  void set_coins(int amount);
  void set_xp(int amount);
  int get_coin_multiplier();
  int get_xp_multiplier();
  int get_coins();
  int get_xp();
};

