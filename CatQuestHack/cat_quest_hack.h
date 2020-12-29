#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <map>
#include "multilevel_pointer.h"
#include "coin_multiplier.h"
#include "xp_multiplier.h"

// Backend class
class cat_quest_hack {
private:
  // Constants
  const std::string proc_name = "Cat Quest.exe";
  // Class attributes
  HANDLE proc_handle;
  std::map< std::string, LPCVOID > proc_modules;
  // must be resolved in runtime due to the mono.dll base address
  LPCVOID coins_address;
  // cheat features
  coin_multiplier* coin_mul;
  xp_multiplier* xp_mul;

  // private methods
  void _set_proc_handle();
  void _set_proc_modules();
  void _set_coins_address();
public:
  cat_quest_hack();
  void set_coin_multiplier(int amount);
  void set_xp_multiplier(int amount);
  void set_coins(int amount);
  int get_coin_multiplier();
  int get_xp_multiplier();
  int get_coins();
};

