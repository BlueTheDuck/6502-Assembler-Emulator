#include "../Shared/stdafx.h"
#include "opcode_manager.h"

//std::vector<opcode> opcodeManager::opcodeList = std::vector<opcode>( 0xFF );

std::vector<opcode> opcodeManager::opcodeList = std::vector<opcode>();

opcodeManager::opcodeManager() {
	opcodeManager::loadOpcodeList();
}
opcodeManager::~opcodeManager() {}
std::vector<opcode> opcodeManager::filterByName(std::string name) {
	std::vector<opcode> sublist;
	for(size_t i = 0; i < opcodeList.size(); i++) {
		if(opcodeManager::opcodeList[i].name == name)
			sublist.push_back(opcodeManager::opcodeList[i]);
	}
	return sublist;
}

opcode opcodeManager::opcodeParser(std::string name, std::string arg) {
	using modes = opcode::modes;

	std::vector<opcode> existingOpcodes = filterByName(name);
	modes::mode possibleMode;

	if(std::regex_match(arg, std::regex(R"(\$[A-Fa-f0-9]{4})")))//$xxxx
		possibleMode = modes::ABS_ADDR;
	else if(arg == "A")//A
		possibleMode = modes::ACCUMULATOR;
	else if(std::regex_match(arg, std::regex(R"(\#\$[A-Fa-f0-9]{2})")))//#$xx
		possibleMode = modes::IMMEDIATE;
	else if(arg == "")
		possibleMode = modes::IMPLIED;
	else if(std::regex_match(arg, std::regex(R"(\$[A-Fa-f0-9]{2})")))//$xxxx
		possibleMode = modes::ZERO_PAGE;
	else if(std::regex_match(arg, std::regex(R"(_?[A-Za-z][A-Za-z0-9_]+)")))
		if(name == "JMP") {
			possibleMode = modes::ABS_ADDR;
		} else {
			possibleMode = modes::LABEL;
		}

	else
		ERROR("Unrecognized addressing mode " << arg, opcodeManager::error::unrecognized_mode());

	for(size_t i = 0; i < existingOpcodes.size(); i++) {
		if(existingOpcodes[i].mode == possibleMode) {
			return existingOpcodes[i];
		}
	}
	ERROR("Invalid addressing mode for " << name, opcode::error::invalid_mode());
}
void opcodeManager::loadOpcodeList() {
	opcodeList.clear();
	std::ifstream opcodeReader;

	try {
		opcodeReader.open("opcodestable.tsv");
	} catch(const std::exception&) {
		ERROR("Couldn't open the opcode table ", new opcodeManager::error::parsing_table);
	}

	std::string line;
	while(getline(opcodeReader, line)) {
		if(line == "name\tmode\tcode")
			continue;
		std::vector<std::string> parsingLine{std::explode(line, '\t')};
		opcode newOpcode;
		if(parsingLine[1] == "abs")
			newOpcode.mode = opcode::modes::ABS_ADDR;
		else if(parsingLine[1] == "A")
			newOpcode.mode = opcode::modes::ACCUMULATOR;
		else if(parsingLine[1] == "#")
			newOpcode.mode = opcode::modes::IMMEDIATE;
		else if(parsingLine[1] == "impl")
			newOpcode.mode = opcode::modes::IMPLIED;
		else if(parsingLine[1] == "zpg")
			newOpcode.mode = opcode::modes::ZERO_PAGE;
		else
			ERROR("Unrecognized mode - Error parsing OPCODE table", new opcodeManager::error::unrecognized_mode);

		newOpcode.numcode = std::stoi(parsingLine[2], nullptr, 16);
		newOpcode.name = parsingLine[0];
		opcodeList.push_back(newOpcode);
	}
	PRINT("Loaded opcodes");
}
