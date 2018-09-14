#pragma once

#include "definitions.h"
#include "../Shared/stdafx.h"
#include "../Shared/primitives.h"

using byte = primitives::byte;
using word = primitives::word;
using bytes = primitives::bytes;

struct emulator::state {
	state();
	struct flags {
		byte byte_flags;
		enum {
			F_CARRY,
			F_ZERO,
			F_IRQDISABLED,
			F_DECIMAL,
			F_BREAKPOINT,
			F_UNUSED,
			F_OVERFLOW,
			F_NEGATIVE,
		};
		bool get(int) const;
		void set(int, bool);
	};

	flags status;
	byte A{0};
	byte X{0};
	byte Y{0};
	word PC{0};
	byte S{0};

	bool ok = true;

	//Methods to handle the registers
	void setA(byte);
	void addToA(byte);
	void setX(byte);
	void setY(byte);

	//Dump processor
	std::stringstream dump();
};