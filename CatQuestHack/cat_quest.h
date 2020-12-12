#pragma once
#include "multilevel_pointer.h"
#include <iostream>
#include <cstring>

namespace cat_quest {

	std::string proc_name = "Cat Quest.exe";

	multilevel_pointer player_data =
		multilevel_pointer::builder()
		.with_base_address(0x00001A78)
		.add_offset(0)
		.build();

	multilevel_pointer money_value =
		multilevel_pointer::builder()
		.from_multilevel_pointer(player_data)
		.add_offset(0x00)
		.add_offset(0x14)
		.add_offset(0x0C)
		.add_offset(0x08)
		.add_offset(0x18)
		.build();
};