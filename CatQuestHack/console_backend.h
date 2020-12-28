#pragma once
#include <iostream>
#include <map>
#include <string>
#include "cat_quest_hack.h"

class console_backend {
private:
  cat_quest_hack hack;
  static console_backend* instance;
  console_backend();
public:
  static console_backend* get_instance();
  std::map< std::string, std::string >  execute_command(std::string command, int amount);
};

