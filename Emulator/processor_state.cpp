
#include "../Shared/stdafx.h"
#include "processor_state.h"

//Constructors
emulator::state::state() {}
//Flags
bool emulator::state::flags::get(int f) const {
	return byte_flags & (1 << f);
}
void emulator::state::flags::set(int f, bool val) {
	byte_flags = byte_flags & !(1 << f) + (val << f);
}
//State
void emulator::state::setA(byte newA) {
	status.set(flags::F_ZERO, newA == 0);
	status.set(flags::F_NEGATIVE, newA & 0x80 != 0);
	A = newA;
}
void emulator::state::addToA(byte add) {
	int newA = (int)A + (int)add;
	status.set(flags::F_CARRY, newA & 0x100 != 0);
	setA(newA & 0xFF);
}
void emulator::state::setX(byte newX) {
	status.set(flags::F_ZERO, newX == 0);
	status.set(flags::F_NEGATIVE, newX & 0x80 != 0);
	X = newX;
}
void emulator::state::setY(byte newY) {
	status.set(flags::F_ZERO, newY == 0);
	status.set(flags::F_NEGATIVE, newY & 0x80 != 0);
	Y = newY;
}

std::stringstream emulator::state::dump() {

	std::stringstream ss;

	using flags = emulator::state::flags;

	ss << std::endl << "Flags: " << std::endl;
	ss << "\t F_CARRY = " << this->status.get(flags::F_CARRY) << std::endl;
	ss << "\t F_ZERO = " << this->status.get(flags::F_ZERO) << std::endl;
	ss << "\t F_IRQDISABLED = " << this->status.get(flags::F_IRQDISABLED) << std::endl;
	ss << "\t F_DECIMAL = " << this->status.get(flags::F_DECIMAL) << std::endl;
	ss << "\t F_BREAKPOINT = " << this->status.get(flags::F_BREAKPOINT) << std::endl;
	ss << "\t F_UNUSED = " << this->status.get(flags::F_UNUSED) << std::endl;
	ss << "\t F_OVERFLOW = " << this->status.get(flags::F_OVERFLOW) << std::endl;
	ss << "\t F_NEGATIVE = " << this->status.get(flags::F_NEGATIVE) << std::endl;

	ss << "Registers: " << std::hex << std::endl;
	ss << "\t A = " << std::setw(2) << std::setfill('0') << (int)this->A << std::endl;
	ss << "\t X = " << std::setw(2) << std::setfill('0') << (int)this->X << std::endl;
	ss << "\t Y = " << std::setw(2) << std::setfill('0') << (int)this->Y << std::endl;
	ss << "\t PC = " << std::setw(4) << std::setfill('0') << (int)this->PC << std::endl;
	ss << "\t S = " << std::setw(2) << std::setfill('0') << (int)this->S << std::endl;


	return ss;
}
