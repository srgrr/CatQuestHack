#pragma once
#include "multilevel_pointer.h"
#include "asm_injector.h"
#include <iostream>
#include <cstring>

namespace cat_quest {

	std::string proc_name = "Cat Quest.exe";

	// base = mono.dll (not fixed, retrieve it in runtime)
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