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

std::map< std::string, std::string > console_backend::execute_command(std::string command, int amount) {
  std::map< std::string, std::string > ret;
  ret["result"] = "OK";
  if (command == "set_coin_multiplier") {
    this->hack.set_coin_multiplier(amount);
  }
  else if (command == "set_xp_multiplier") {
    this->hack.set_xp_multiplier(amount);
  }
  else if (command == "set_coins") {
    this->hack.set_coins(amount);
  }
  else {
    ret["result"] = "INVALID_CMD";
  }
  ret["coin_multiplier"] = std::to_string(this->hack.get_coin_multiplier());
  ret["xp_multiplier"] = std::to_string(this->hack.get_xp_multiplier());
  ret["coins"] = std::to_string(this->hack.get_coins());
  std::cerr << "-------------" << std::endl;
  for (auto& kv : ret) {
    std::cerr << kv.first << " -> " << kv.second << std::endl;
  }
  std::cerr << "-------------" << std::endl;
  return ret;
}
