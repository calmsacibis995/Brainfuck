//
// Brainfuck interpreter for Microsoft Windows.
// Copyright (c) 2025 Stefanos Stefanidis. All rights reserved.
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program.
// If not, see <http://www.gnu.org/licenses/>.
//
// File:
// Brainfuck.cpp
//
// Description:
// Main Brainfuck interpreter logic.
//
// Created on:
// 2025.06.14
//

#include "Brainfuck.h"

const std::string copyright_msg =
    "Copyright (c) 2025 Stefanos Stefanidis. All rights reserved.\n\n"
    "This is free software, and you are welcome to redistribute it under the terms of the\n"
    "GNU General Public License.\n"
    "This program comes with ABSOLUTELY NO WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License\n"
    "for more details.\n\n";

int
main(int argc, char **argv)
{
    Brainfuck bf;

    std::cout << "Brainfuck interpreter for Windows (built " << __DATE__ << " " << __TIME__ << ")" << std::endl;
    std::cout << copyright_msg;

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " program" << std::endl;
        return 1;
    }

    bf.bf_file.open(argv[1]);

    if (!bf.bf_file.is_open()) {
        std::cout << "Could not open file " << argv[1] << std::endl;
        return 1;
    }

    bf.BrainfuckInit();

    if (!bf.BrainfuckRead()) {
        bf.bf_file.close();
        return 1;
    }

    bf.BrainfuckInterpret();

    bf.bf_file.close();
    return 0;
}

/*
 * Initialize Brainfuck array and data pointer.
 */
void
Brainfuck::BrainfuckInit(void)
{
    int i;

    // Initialize Brainfuck's array.
    for (i = 0; i < BF_ARRAY_SIZE; i++)
        bf_mem[i] = 0;

    // Initialize Brainfuck's data pointer.
    bf_data_ptr = bf_mem;
}

bool
Brainfuck::BrainfuckRead(void)
{
    char c;

    // Process all valid Brainfuck characters as instructions, and write them to the program buffer.
    // Ignore all invalid characters to allow comments to be included in source file and allow Brainfuck code
    // to be split into multiple lines.
    while (bf_file.get(c)) {
        if (c == '>' || c == '<' || c == '+' || c == '-' || c == '.' || c == ',' || c == '[' || c == ']')
            bf_program += c;
    }

    if (bf_program.empty()) {
        std::cout << "Error: Input file is empty or contains no valid Brainfuck instructions." << std::endl;
        return false;
    }

    return true;
}

void
Brainfuck::BrainfuckInterpret(void)
{
    for (bf_pc = 0; bf_pc < bf_program.size(); bf_pc++) {
        switch (bf_program[bf_pc]) {
            case '>':   // Move one cell to the right.
                if (!Brainfuck::BrainfuckMoveCellRight())
                    return;
                break;
            case '<':   // Move one cell to the left.
                if (!Brainfuck::BrainfuckMoveCellLeft())
                    return;
                break;
            case '+':   // Increment cell by one.
                Brainfuck::BrainfuckIncrementCell();
                break;
            case '-':   // Decrement cell by one.
                Brainfuck::BrainfuckDecrementCell();
                break;
            case '.':   // Output the byte stored in current cell as a character.
                Brainfuck::BrainfuckOutputCharacter();
                break;
            case ',':   // Accept input from user and store it in the current cell.
                Brainfuck::BrainfuckInputCharacter();
                break;
            case '[':   // If the byte at the data pointer is zero, jump the program counter forward to the command after the matching ']'.
                if (!Brainfuck::BrainfuckLoopOpeningBracket())
                    return;
                break;
            case ']':   // If the byte at the data pointer is nonzero, jump the program counter back to the command after the matching '['.
                if (!Brainfuck::BrainfuckLoopClosingBracket())
                    return;
                break;
        }
    }
}
