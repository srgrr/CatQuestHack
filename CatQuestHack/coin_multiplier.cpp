#include "coin_multiplier.h"

// the code template multiplies the amount to be added by 2
// but it can be changed to any 32bit value
const byte coin_multiplier::asm_code[] = {
  0x29, 0xd1, // sub ecx, edx (undo the honest addition)
  0x50, // push eax
  0x53, // push ebx
  0xbb, 0x02, 0x00, 0x00, 0x00, // mov ebx, 2
  0x89, 0xd0, // mov eax, edx
  0xf7, 0xe3, // mul ebx
  0x89, 0xc2, // mov edx, eax
  0x01, 0xd1, // add ecx, edx
  0x5b, // pop ebx
  0x58 // pop eax
};

coin_multiplier::coin_multiplier(HANDLE proc_handle) {
  this->proc_handle = proc_handle;
}


bool coin_multiplier::run(LPCVOID base_address) {
  return asm_injector::builder()
    .with_proc_handle(this->proc_handle)
    .with_injection_address(base_address)
    .with_bytes_to_inject(sizeof(asm_code))
    .with_prefix_size(5)
    .with_code_to_inject((byte*)asm_code)
    .build()
    .inject();
}
