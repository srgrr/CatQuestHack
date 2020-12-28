#include <Windows.h>
#include <iostream>
#include "console_backend.h"


int main(int argc, char** argv) {
  std::string command;
  int amount;
  while (std::cin >> command >> amount) {
    console_backend::get_instance()->execute_command(command, amount);
  }
}