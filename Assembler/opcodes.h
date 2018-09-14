#pragma once

#ifndef OPCODE_H__
#define OPCODE_H__

#include "../Shared/stdafx.h"

struct opcode {
    //http://www.emulator101.com/6502-addressing-modes.html
    struct modes {
        enum mode {
            ABS_ADDR = 1,
            ACCUMULATOR = 2,
            IMMEDIATE = 4,
            IMPLIED = 8,
            ZERO_PAGE = 16,

            LABEL = 32768
        };
    };
    std::string name;
    modes::mode mode;
    short numcode;
    unsigned int time;

    struct error {
        class invalid_mode :std::exception {};
    };
};
#endif