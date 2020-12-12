#pragma once
#include "multilevel_pointer.h"
#include <iostream>
#include <cstring>

namespace cat_quest {

	std::string proc_name = "Cat Quest.exe";

	multilevel_pointer player_data =
		multilevel_pointer::builder()
		.with_base_address(0x176A0000)
		.add_offset(0x0)
		.add_offset(0x8)
		.add_offset(0x54)
		.add_offset(0x61C)
		.add_offset(0x0)
		.add_offset(0x520)
		.build();

	multilevel_pointer money_value =
		multilevel_pointer::builder()
		.from_multilevel_pointer(player_data)
		.add_offset(0x14)
		.add_offset(0x0C)
		.add_offset(0x08)
		.add_offset(0x18)
		.build();

};