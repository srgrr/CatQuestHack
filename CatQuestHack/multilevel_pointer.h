#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>

class multilevel_pointer {
private:
	DWORD base_address;
	std::vector<std::uint32_t> offsets;
public:
	multilevel_pointer();
	multilevel_pointer(const multilevel_pointer& other);
	~multilevel_pointer();
	LPCVOID get_final_address(HANDLE process_handle);
	
	// getters
	DWORD get_base_address() const;
	std::vector<std::uint32_t> get_offsets() const;
	// setters
	void set_base_address(DWORD base_address);
	void add_offset(std::uint32_t offset);
	void set_offsets(std::vector<std::uint32_t> offsets);

	// build mlps only through the builder
	class builder;
};


class multilevel_pointer::builder {
private:
	multilevel_pointer pointer;
public:
	builder();
	~builder();
	builder from_multilevel_pointer(multilevel_pointer&);
	builder with_base_address(DWORD base_address);
	builder add_offset(std::uint32_t offset);
	multilevel_pointer build();
};

