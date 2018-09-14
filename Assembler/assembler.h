#pragma once

#ifndef COMPILER_H__
#define COMPILER_H__

#include "../Shared/stdafx.h"
#include "../Shared/primitives.h"
#include "opcode_manager.h"

#define ROMSIZE 0x10000
#define ROMSTART 0x600

class assembler {
public:
    struct lineTypes {
        enum lineType {
            OPCODE,
            LABEL,
            DIRECTIVE,
            COMMENT
        };
    };
    struct error {
        class out_of_memory : std::exception {};
        class invalid_argument : std::exception {};
    };

    byte ROM[ROMSIZE];
    short nextAddress;
    std::map<std::string, std::vector<short>> missingLabels;
    std::map<std::string, short> labels;

    void storeInROM( primitives::bytes );
    void storeInROM( byte );
    primitives::bytes compileLine( opcode, std::string );
    primitives::bytes compileLine( opcode, primitives::bytes );
    void processDirective(std::string, std::string="");
    void registerLabel( std::string, short );

    static assembler::lineTypes::lineType getLineType( std::string );
    static primitives::bytes stringToHex( std::string hex );
    static bool isHexVal( std::string );
    static assembler* getInstance();

private:
    static assembler* instance;
    assembler();
};

#endif // !COMPILER_H__