#pragma once
#include "multilevel_pointer.h"
#include <iostream>
#include <cstring>

namespace cat_quest {

	std::string proc_name = "Cat Quest.exe";

	// the base address is the one from mono.dll
	// must be extracted as module base address from
	// the mono.dll submodule in Cat Quest.exe
	multilevel_pointer money_value =
		multilevel_pointer::builder()
		.add_offset(0x001F50AC)
		.add_offset(0x5FC)
		.add_offset(0x90)
		.add_offset(0x10)
		.add_offset(0x70)
		.add_offset(0x0C)
		.add_offset(0xD4)
		.add_offset(0x158)
		.build();

};