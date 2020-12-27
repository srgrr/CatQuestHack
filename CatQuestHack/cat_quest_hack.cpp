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

cat_quest_hack::cat_quest_hack() {
  _set_proc_handle();
  _set_memory_regions();
  _get_code_page();
  this->coin_mul = new coin_multiplier(this->proc_handle);
}
