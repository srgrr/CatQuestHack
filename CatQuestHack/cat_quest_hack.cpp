#include "cat_quest_hack.h"


void cat_quest_hack::_set_proc_handle() {
  this->proc_handle = proc_util::get_proc_handle(proc_name);
}

void cat_quest_hack::_set_proc_modules() {
  this->proc_modules = proc_util::get_module_base_addresses(this->proc_handle);
}

void cat_quest_hack::_set_memory_regions() {
  this->memory_regions = proc_util::get_memory_regions(this->proc_handle);

}

void cat_quest_hack::_set_coins_address() {
  multilevel_pointer coins_pointer =
    multilevel_pointer::builder()
    .with_base_address((DWORD)proc_util::get_module_base_address(this->proc_modules, "mono.dll"))
    .add_offset(0x001F50AC)
    .add_offset(0x5FC)
    .add_offset(0x90)
    .add_offset(0x10)
    .add_offset(0x70)
    .add_offset(0x0C)
    .add_offset(0xD4)
    .add_offset(0x158)
    .build();
  this->coins_address = coins_pointer.get_final_address(this->proc_handle);
}

void cat_quest_hack::_set_xp_address() {
  // TODO
}

cat_quest_hack::cat_quest_hack() {
  _set_proc_handle();
  _set_memory_regions();
  _set_proc_modules();
  _set_coins_address();
  this->coin_mul = new coin_multiplier(this->proc_handle, this->memory_regions);
}

void cat_quest_hack::set_coin_multiplier(int amount) {
  this->coin_mul->set_multiplier(amount);
}

void cat_quest_hack::set_xp_multiplier(int amount) {
  // TODO
}

void cat_quest_hack::set_coins(int amount) {
  proc_util::write_to_proc_mem(
    this->proc_handle,
    this->coins_address,
    (LPCVOID)&amount,
    4
  );
}

void cat_quest_hack::set_xp(int amount) {
}

int cat_quest_hack::get_coin_multiplier() {
  return this->coin_mul->get_multiplier();
}

int cat_quest_hack::get_xp_multiplier() {
  return 0;
}

int cat_quest_hack::get_coins() {
  byte* coins_buff =
    proc_util::read_from_proc(
      this->proc_handle,
      this->coins_address,
      4
    );
  int ret = *(int*)coins_buff;
  return ret;
}

int cat_quest_hack::get_xp() {
  return 0;
}
