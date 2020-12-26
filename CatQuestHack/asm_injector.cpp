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
  int bytes_to_alloc = prefix_size + bytes_to_inject + 5;
  // Get memory in the process to allocate the malicious code
  LPCVOID malicious_address = proc_util::alloc(this->proc_handle, bytes_to_alloc);
  std::cerr << std::hex << "Honest code base address is " << this->injection_address << std::endl;
  std::cerr << "Prefix size is " << this->prefix_size << std::endl;
  std::cerr << std::hex << "Malicious code base address is " << malicious_address << std::endl;
  // Get the first k bytes from base injection address (these are the prefix instructions
  // that will be injected before the malicious code itself)
  byte* prefix_instructions = proc_util::read_from_proc(this->proc_handle, this->injection_address, this->prefix_size);
  // Write them in the destination page
  proc_util::write_to_proc_mem(
    this->proc_handle,
    malicious_address,
    prefix_instructions,
    this->prefix_size
  );
  // write the injection code itself
  proc_util::write_to_proc_mem(
    this->proc_handle,
    (LPCVOID)((DWORD)malicious_address + this->prefix_size),
    this->code_to_inject,
    this->bytes_to_inject
  );
  // write the return jmp, note that the jmp addr is relative, so we need to compute the difference
  // orig - dest and feed it as the parameter
  DWORD return_address = (DWORD)this->injection_address + this->prefix_size;
  std::cerr << std::hex << "Return address is " << return_address << std::endl;
  DWORD caller_address = (DWORD)malicious_address + bytes_to_alloc - 5;
  std::cerr << std::hex << "Caller address is " << caller_address << std::endl;
  // write the final jmp to the malicious code segment
  // the jmp opcode is E9 XXXXXX
  int jump_diff = int(return_address) - int(caller_address) - 5;
  std::cerr << std::hex << "Jump diff between injected and actual is " << jump_diff << std::endl;
  proc_util::write_to_proc_mem(
    this->proc_handle,
    (LPCVOID)((DWORD)malicious_address + bytes_to_alloc - 4),
    &jump_diff,
    4
  );
  byte jmp[] = {0xe9};
  proc_util::write_to_proc_mem(
    this->proc_handle,
    (LPCVOID)((DWORD)malicious_address + bytes_to_alloc - 5),
    &jmp,
    1
  );
  // Replace the first instruction with the malicious jmp
  jump_diff = int(malicious_address) - int(this->injection_address) - 5;
  proc_util::write_to_proc_mem(
    this->proc_handle,
    (LPCVOID)((DWORD)this->injection_address + 1),
    &jump_diff,
    4
  );
  proc_util::write_to_proc_mem(
    this->proc_handle,
    (LPCVOID)this->injection_address,
    &jmp,
    1
  );
  // Replace the gaps with NOPS to avoid malformed opcodes
  // or replicated code
  for (int i = 0; i < this->prefix_size - 5; ++i) {
    byte nop = 0x90;
    proc_util::write_to_proc_mem(
      this->proc_handle,
      (LPCVOID)((DWORD)this->injection_address + 5 + i),
      &nop,
      1
    );
  }
  // ... and just pray this shit works
  return true;
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
