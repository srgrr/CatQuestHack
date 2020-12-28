#include "coin_multiplier.h"

// this code just comes before the injection point
// should be unique in the 100k code page
const byte coin_multiplier::asm_prefix[] = {
  0xe8, 0xad, 0x01, 0x00, 0x00, // call XXXXXXXX
  0x83, 0xc4, 0x20, // add esp, 20
  0x8b, 0x47, 0x14, // mov eax,[edi+14]
  0x8b, 0x40, 0x0c, // mov eax,[eax+0c]
  0x8b, 0x48, 0x08, // mov ecx,[eax+08]
  0x8b, 0xc1, // mov eax, ecx
  0x39, 0x09, // cmp [ecx], ecx
  0x8b, 0x49, 0x18, // mov ecx,[ecx+18]
  0x8b, 0x56, 0x30 // mov edx,[esi+30]
};


// the code template multiplies the amount to be added by 1
// but it can be changed to any 32bit value
const byte coin_multiplier::asm_code[] = {
  0x29, 0xd1, // sub ecx, edx (undo the honest addition)
  0x50, // push eax
  0x53, // push ebx
  0xbb, 0x01, 0x00, 0x00, 0x00, // mov ebx, 1
  0x89, 0xd0, // mov eax, edx
  0xf7, 0xe3, // mul ebx
  0x89, 0xc2, // mov edx, eax
  0x01, 0xd1, // add ecx, edx
  0x5b, // pop ebx
  0x58 // pop eax
};

// where does the value of the multiplier start?
const int coin_multiplier::multiplier_offset = 5;

// length of the minimum movable prefix of the coin addition code
const int coin_multiplier::prefix_length = 5;

coin_multiplier::coin_multiplier(HANDLE proc_handle, std::vector< MEMORY_BASIC_INFORMATION >& memory_regions) :
  proc_handle(proc_handle),
  memory_regions(memory_regions),
  multiplier(1),
  code_page(nullptr),
  instruction_address(nullptr) {
  _solve_injection_point();
  _inject();
}

void coin_multiplier::_inject() {
  this->code_page = asm_injector::builder()
    .with_proc_handle(this->proc_handle)
    .with_injection_address(this->instruction_address)
    .with_bytes_to_inject(sizeof(asm_code))
    .with_prefix_size(5)
    .with_code_to_inject((byte*)asm_code)
    .build()
    .inject();
}

int coin_multiplier::get_multiplier() {
  return this->multiplier;
}

void coin_multiplier::_update_multiplier() {
  int offset_in_page = this->multiplier_offset + this->prefix_length;
  proc_util::write_to_proc_mem(
    this->proc_handle,
    (LPCVOID)((DWORD)this->code_page + offset_in_page),
    (LPCVOID)&this->multiplier,
    4
  );
}

void coin_multiplier::set_multiplier(int new_val) {
  this->multiplier = new_val;
  this->_update_multiplier();
}

void coin_multiplier::_solve_injection_point() {
  for (MEMORY_BASIC_INFORMATION& region : this->memory_regions) {
    if ((region.Protect | PAGE_EXECUTE_READ) && region.RegionSize == 0x100000) {
      std::cerr << "Scanning region [" << region.BaseAddress << ", " << (LPCVOID)((DWORD)region.BaseAddress + region.RegionSize - 1) << "]" << std::endl;
      auto matches = proc_util::find_pattern(
        this->proc_handle,
        region.BaseAddress,
        region.RegionSize,
        coin_multiplier::asm_prefix,
        sizeof(coin_multiplier::asm_prefix)
      );
      for (auto& match : matches) {
        std::cerr << std::hex << "Possible match -> " << match << std::endl;
        this->code_page = region.BaseAddress;
        this->instruction_address = (LPCVOID)((DWORD)match);
      }
    }
  }
}