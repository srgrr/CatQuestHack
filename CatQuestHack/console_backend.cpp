#include "console_backend.h"

console_backend* console_backend::instance = 0;

console_backend* console_backend::get_instance() {
  if (console_backend::instance == nullptr) {
    console_backend::instance = new console_backend;
  }
  return console_backend::instance;
}

console_backend::console_backend() {

}

void console_backend::execute_command(std::string command, int amount) {
  if (command == "set_coin_multiplier") {
    this->hack.set_coin_multiplier(amount);
  }
  else if (command == "set_xp_multiplier") {
    this->hack.set_xp_multiplier(amount);
  }
  else if (command == "set_coins") {
    this->hack.set_coins(amount);
  }
  else if (command == "set_xp") {
    this->hack.set_xp(amount);
  }
  else if (command == "show_current_parameters") {
    std::cout << "Coin multiplier -> " << this->hack.get_coin_multiplier() << std::endl;
    std::cout << "XP multiplier -> " << this->hack.get_xp_multiplier() << std::endl;
    std::cout << "Coins -> " << this->hack.get_coins() << std::endl;
    std::cout << "XP -> " << this->hack.get_xp() << std::endl;
  }
  else {
    std::cerr << "Invalid command" << std::endl;
  }
}
