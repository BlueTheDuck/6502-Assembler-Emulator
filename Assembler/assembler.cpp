#include "../Shared/stdafx.h"
#include "assembler.h"

assembler* assembler::instance = nullptr;

assembler::assembler() {
	this->ROM[ROMSIZE] = {};
	this->nextAddress = ROMSTART;
	this->labels = {};
	for(size_t addr = 0; addr <= ROMSIZE; addr++) {
		ROM[addr] = 0x00;
	}
	assembler::instance = this;
}
void assembler::storeInROM(primitives::bytes data) {
	for(size_t i = 0; i < data.size; i++) {
		if(nextAddress == ROMSIZE) {
			ERROR("Tried to store " << (data.size) << " B and run out of space", new assembler::error::out_of_memory);
		}
		ROM[nextAddress++] = data.data[i];
	}
}
void assembler::storeInROM(primitives::byte byte) {
	if(nextAddress == ROMSIZE) {
		ERROR("Tried to store 1 B and run out of space", new assembler::error::out_of_memory);
	}
	ROM[nextAddress++] = byte;
}
primitives::bytes assembler::compileLine(opcode op, std::string arg) {
	primitives::bytes dataarg;

	if(std::regex_match(arg, std::regex(R"(_?[A-Za-z][A-Za-z0-9_]+)"))) {
		dataarg.size = 2;
		dataarg.data = new uint8_t[dataarg.size];

		short lblAddr;
		if(assembler::labels.count(arg.c_str()) > 0) {
			lblAddr = labels.at(arg);
		} else {
			PRINT("Tried to used yet undefined label " << arg);
			if(missingLabels.count(arg) == 0) {
				PRINT("Registering undefined label");
				std::vector<short> newListOfAddresses;
				missingLabels[arg] = newListOfAddresses;
			}
			missingLabels.at(arg).push_back(nextAddress);
			lblAddr = 0xFFFF;
		}
		dataarg.data[0] = lblAddr & 0x00FF;
		dataarg.data[1] = (lblAddr & 0xFF00) >> 8;
	} else {
		std::regex e(R"([^\dA-Fa-z]*([\dA-Fa-z]+)[^\dA-Fa-z]*?)");
		std::string parsedarg = std::regex_replace(arg, e, "$1");
		dataarg = stringToHex(parsedarg);
	}
	return assembler::compileLine(op, dataarg);
}
primitives::bytes assembler::compileLine(opcode op, primitives::bytes arg) {
	using modes = opcode::modes;

	primitives::bytes output;
	output.size = 1 + arg.size;
	output.data = new uint8_t[output.size];

	output.data[0] = op.numcode & 0xFF;

	for(int i = 0; i < arg.size; i++)
		output.data[i + 1] = arg.data[i];

	return output;
}
void assembler::processDirective(std::string directive, std::string arg) {
	if(directive == ".org") {
		if(std::regex_match(arg, std::regex(R"(\$\d{4})"))) {
			assembler::nextAddress = std::stoi(arg.substr(1, 4), nullptr, 16);
		} else {
			ERROR(".org argument should be in format $\d{4}", new assembler::error::invalid_argument);
		}
	} else if(directive == ".bytes") {
		auto args = std::explode(arg, ',');
		for(size_t i = 0; i < args.size(); i++) {
			if(!isHexVal(args[i]))
				ERROR(".bytes arguments should only be bytes (e.g. [0-9A-F]+)", new assembler::error::invalid_argument);
			uint8_t byte = std::stoi(args[i], nullptr, 16) & 0xFF;
			assembler::storeInROM(byte);
		}
	} else if(directive == ".skip") {
		auto args = std::explode(arg, ',');

		primitives::bytes bytesToSkip, replaceValue;

		bytesToSkip = assembler::stringToHex(args[0]);
		if(bytesToSkip.size != 1)
			ERROR("First parameter for .skip should be one byte", new assembler::error::invalid_argument);

		if(args.size() == 2) {
			replaceValue = stringToHex(args[1]);
			if(replaceValue.size != 1)
				ERROR("Second parameter for .skip should be one byte", new assembler::error::invalid_argument);
			for(size_t i = 0; i < bytesToSkip.data[0]; i++) {
				this->storeInROM(replaceValue.data[0]);
			}
		} else {
			this->nextAddress += bytesToSkip.data[0];
			if(this->nextAddress >= ROMSIZE)
				ERROR("Tryed to skip " << bytesToSkip.data[0] << " B and ended outside ROM (Started address: " << (this->nextAddress - bytesToSkip.data[0]) << ")", new assembler::error::out_of_memory);
		}
	}
}
void assembler::registerLabel(std::string name, short address) {
	name = name.substr(0, name.length() - 1);
	labels[name] = address;
	if(missingLabels.count(name) > 0) {
		auto references = missingLabels.at(name);
		for(const short& addr : references) {
			ROM[addr + 1] = address & 0x00FF;
			ROM[addr + 2] = (address & 0xFF00) >> 8;
		}
		missingLabels.erase(name);
	}
}

assembler::lineTypes::lineType assembler::getLineType(std::string line) {
	if(line[0] == ';')
		return assembler::lineTypes::COMMENT;
	else if(std::regex_match(line, std::regex(R"(_?[A-Za-z][A-Za-z0-9_]+:)")))
		return assembler::lineTypes::LABEL;
	else if(std::regex_match(line, std::regex(R"(^\w{3}.*)")))
		return assembler::lineTypes::OPCODE;
	else if(line[0] == '.')
		return assembler::lineTypes::DIRECTIVE;
	else
		throw std::exception("Invalid data found");

}
primitives::bytes assembler::stringToHex(std::string hex) {
	if(hex == "")
		return primitives::bytes(0);
	if(!isHexVal(hex))
		ERROR("assembler::stringToHex expects a hex number as first parameter", new assembler::error::invalid_argument);

	hex = std::regex_replace(hex, std::regex(R"((0x|#?\$)?)"), "");

	primitives::bytes bytes;

	bytes.size = hex.size() / 2;
	bytes.data = new uint8_t[bytes.size];

	for(size_t i = 0; i < bytes.size; i++) {
		bytes.data[i] = std::stoi(hex.substr(i * 2, 2), nullptr, 16);
	}

	return bytes;
}
bool assembler::isHexVal(std::string stringToTest) {
	return std::regex_match(stringToTest, std::regex(R"((0x|#?\$)?[0-9A-Fa-f]+)"));
}
assembler* assembler::getInstance() {
	if(instance == nullptr) {
		instance = new assembler();
	}
	return instance;
}