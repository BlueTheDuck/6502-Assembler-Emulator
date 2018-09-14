
#include "stdafx.h"
#include "primitives.h"


primitives::bytes::bytes(size_t size) {
	this->data = new byte[size];
	this->size = size;
	PRINT("Created new <bytes> with size of " << HEX(size));
}
void primitives::bytes::operator=(bytes otherBytes) {
	this->data = otherBytes.data;
	this->size = otherBytes.size;
}
/*void primitives::bytes::operator=(int value) {
	this->data = new byte[value];
	this->size = value;
	for (size_t i = 0; i < value; i++) {
		this->data[i] = 0;
	}
}*/
byte& primitives::bytes::operator[](int i) {
	return this->data[i];
}
std::istream& primitives::operator>>(std::istream& input, primitives::bytes& o) {
	char* buf = new char[o.size];
	input.read(buf, o.size);

	for (size_t i = 0; i < o.size; i++) {
		o.data[i] = (buf[i] & 0xFF);
	}

	return input;
}
 std::ostream& primitives::operator<<(std::ostream& output, const primitives::bytes& o) {
	std::ios_base::fmtflags f(output.flags());

	output << std::hex;
	for (int i = 0; i < o.size; i++)
		output << (o.data[i] & 0xFF);

	output.flags(f);
	return output;
}
