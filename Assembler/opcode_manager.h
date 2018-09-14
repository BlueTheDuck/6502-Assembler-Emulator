#pragma once

#ifndef OPCODES_MANAGER_H__
#define OPCODES_MANAGER_H__

#include "../Shared/stdafx.h"
#include "opcodes.h"
#include "definitions.h"

class opcodeManager {
public:
    static std::vector<opcode> opcodeList;

    struct error {
        class parsing_table : std::exception {};
        class unrecognized_mode : std::exception {};
    };

    opcodeManager();
    virtual ~opcodeManager();
    static std::vector<opcode> filterByName( std::string );
    static opcode opcodeParser( std::string, std::string );
    static void loadOpcodeList();

};

#endif // !OPCODES_MANAGER_H__