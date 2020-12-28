#include "multilevel_pointer.h"
#include "asm_injector.h"

multilevel_pointer::builder::builder() {

}

multilevel_pointer::builder::~builder() {
}

multilevel_pointer::builder multilevel_pointer::builder::from_multilevel_pointer(multilevel_pointer& o_pointer) {
  this->pointer.set_previous_pointer(o_pointer);
  return *this;
}

multilevel_pointer::builder multilevel_pointer::builder::with_base_address(DWORD base_address) {
  this->pointer.set_base_address(base_address);
  return *this;
}

multilevel_pointer::builder multilevel_pointer::builder::add_offset(std::uint32_t offset) {
  this->pointer.add_offset(offset);
  return *this;
}

multilevel_pointer multilevel_pointer::builder::build() {
  multilevel_pointer ret = multilevel_pointer(this->pointer);
  return ret;
}

multilevel_pointer::multilevel_pointer() {
  this->base_address = (DWORD)0xDEADBEEF;
  this->previous_pointer = nullptr;
}

multilevel_pointer::multilevel_pointer(const multilevel_pointer& other) {
  this->base_address = other.get_base_address();
  this->previous_pointer = other.get_previous_pointer();
  this->offsets = other.get_offsets();
}

multilevel_pointer::~multilevel_pointer() {
}

LPCVOID multilevel_pointer::get_final_address(HANDLE process_handle) {
  LPVOID ret;
  if (this->get_previous_pointer() != nullptr) {
    ret = (LPVOID)this->get_previous_pointer()->get_final_address(process_handle);
  }
  else {
    ret = (LPVOID)this->get_base_address();
  }
  std::cerr << std::hex << (std::uint32_t)ret << std::endl;
  for (int i = 0; i < int(this->offsets.size()); ++i) {
    std::uint32_t offset = this->offsets[i];
    DWORD address_with_offset = (DWORD)ret + offset;
    std::cerr << std::hex << (std::uint32_t)ret << " " << offset << " " << address_with_offset << std::endl;
    // avoid derefence to the last offset
    // we want the final address, not the value inside it
    if (i < int(this->offsets.size() - 1)) {
      if (process_handle == nullptr) {
        // read from the current memory
        DWORD new_address = *(DWORD*)address_with_offset;
        ret = (LPVOID)new_address;
      }
      else {
        // read from other process memory
        std::uint32_t new_address;
        if (
          !ReadProcessMemory(
            process_handle,
            (LPVOID)address_with_offset,
            (LPVOID)&new_address,
            sizeof(LPVOID),
            nullptr
          )) {
          std::cerr << "Couldnt properly read process memory, aborting..." << std::endl;
          std::exit(0);
        }
        ret = (LPVOID)new_address;
      }
    }
    else {
      ret = (LPVOID)address_with_offset;
    }
  }
  return (LPCVOID)ret;
}

DWORD multilevel_pointer::get_base_address() const {
  return base_address;
}

std::vector<std::uint32_t> multilevel_pointer::get_offsets() const {
  return offsets;
}

multilevel_pointer* multilevel_pointer::get_previous_pointer() const {
  return this->previous_pointer;
}

void multilevel_pointer::set_base_address(DWORD base_address) {
  this->base_address = base_address;
}

void multilevel_pointer::add_offset(std::uint32_t offset) {
  this->offsets.push_back(offset);
}

void multilevel_pointer::set_offsets(std::vector<std::uint32_t> offsets) {
  this->offsets = offsets;
}

void multilevel_pointer::set_previous_pointer(multilevel_pointer& other) {
  this->previous_pointer = &other;
}
