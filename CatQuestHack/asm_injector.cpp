#include "asm_injector.h"

asm_injector::asm_injector(HANDLE proc_handle, LPCVOID injection_address, int prefix_size, byte* code_to_inject, int bytes_to_inject) {
  this->proc_handle = proc_handle;
  this->injection_address = injection_address;
  // -1 means we must compute it, not available atm (assume sequences of 3 bytes)
  this->prefix_size = prefix_size;
  this->code_to_inject = code_to_inject;
  this->bytes_to_inject = bytes_to_inject;
}

bool asm_injector::inject() {
  return false;
}

// getters

HANDLE asm_injector::get_proc_handle() {
  return this->proc_handle;
}

LPCVOID asm_injector::get_injection_address() {
  return this->injection_address;
}

int asm_injector::get_prefix_size() {
  return this->prefix_size;
}

byte* asm_injector::get_code_to_inject() {
  return this->code_to_inject;
}

int asm_injector::get_bytes_to_inject() {
  return this->bytes_to_inject;
}

// builder
asm_injector::builder::builder() {

}

asm_injector::builder asm_injector::builder::with_proc_handle(HANDLE proc_handle) {
  this->proc_handle = proc_handle;
  return *this;
}

asm_injector::builder asm_injector::builder::with_injection_address(LPCVOID injection_address) {
  this->injection_address = injection_address;
  return *this;
}

asm_injector::builder asm_injector::builder::with_prefix_size(int prefix_size) {
  this->prefix_size = prefix_size;
  return *this;
}

asm_injector::builder asm_injector::builder::with_code_to_inject(byte* code_to_inject) {
  this->code_to_inject = code_to_inject;
  return *this;
}

asm_injector::builder asm_injector::builder::with_bytes_to_inject(int bytes_to_inject) {
  this->bytes_to_inject = bytes_to_inject;
  return *this;
}

asm_injector asm_injector::builder::build() {
  return asm_injector(
    this->proc_handle,
    this->injection_address,
    this->prefix_size,
    this->code_to_inject,
    this->bytes_to_inject
  );
}
