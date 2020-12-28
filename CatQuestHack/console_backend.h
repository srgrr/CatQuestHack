#pragma once
#include <iostream>
#include "cat_quest_hack.h"

class console_backend {
private:
  cat_quest_hack hack;
  static console_backend* instance;
  console_backend();
public:
  static console_backend* get_instance();
  void execute_command(std::string command, int amount);
};

