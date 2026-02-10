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
// Brainfuck.h
//
// Description:
// Brainfuck interpreter support header file.
//
// Created on:
// 2025.06.14
//

#ifndef _BRAINFUCK_H_
#define _BRAINFUCK_H_

#include <iostream>
#include <fstream>
#include <string>

#include <cstdint>

/*
 * Brainfuck's one-dimensional array size. There is no standard regarding the size,
 * but most interpreters use an array with a size of 30000 bytes. If you need
 * more space, you can increase the size of the array by increasing this number.
 */
#define BF_ARRAY_SIZE	30000

class Brainfuck {
	public:
		uint8_t *bf_mem;	// Brainfuck's memory.
		uint8_t* bf_data_ptr;		// Data pointer.
		size_t bf_pc;		// Program counter.
		std::string bf_program;		// Program data as ASCII characters.
		std::ifstream bf_file;		// File stream.
		uint32_t bf_array_size;	// Size of Brainfuck's array.

		void BrainfuckInit(uint32_t ArraySize);	// Initialize Brainfuck array and data pointer.
		bool BrainfuckRead(void);	// Read an input text file.
		void BrainfuckInterpret(void);	// Interpret Brainfuck code.
	private:
		void BrainfuckIncrementCell(void) {
			if ((*bf_data_ptr) == 255)
				std::cout << "Warning: Integer overflow (PC: " << bf_pc << ")" << std::endl;
			(*bf_data_ptr)++;
		}
		void BrainfuckDecrementCell(void) {
			if ((*bf_data_ptr) == 0)
				std::cout << "Warning: Integer underflow (PC: " << bf_pc << ")" << std::endl;
			(*bf_data_ptr)--;
		}
		bool BrainfuckMoveCellRight(void) {
			if (bf_data_ptr + 1 >= bf_mem + bf_array_size) {
				std::cout << "Error: Array exceeded (PC: " << bf_pc << ")" << std::endl;
				return false;
			} else {
				bf_data_ptr++;
				return true;
			}
		}
		bool BrainfuckMoveCellLeft(void) {
			if (bf_data_ptr - 1 < bf_mem) {
				std::cout << "Error: Array underflow (PC: " << bf_pc << ")" << std::endl;
				return false;
			} else {
				bf_data_ptr--;
				return true;
			}
		}
		void BrainfuckOutputCharacter(void) {
			putchar(*bf_data_ptr);
		}
		void BrainfuckInputCharacter(void) {
			*bf_data_ptr = getchar();
		}
		bool BrainfuckLoopOpeningBracket(void) {
			if (*bf_data_ptr == 0) {
				int i = 1;
				while (i > 0) {
					bf_pc++;
					if (bf_pc >= bf_program.size()) {
						std::cout << "Error: Unmatched '[' (PC: " << bf_pc << ")" << std::endl;
						return false;
					}
					if (bf_program[bf_pc] == '[')
						i++;
					if (bf_program[bf_pc] == ']')
						i--;
				}
			}
			return true;
		}
		bool BrainfuckLoopClosingBracket(void) {
			if (*bf_data_ptr != 0) {
				int i = 1;
				while (i > 0) {
					if (bf_pc == 0) {
						std::cout << "Error: Unmatched ']' (PC: " << bf_pc << ")" << std::endl;
						return false;
					}
					bf_pc--;
					if (bf_program[bf_pc] == '[')
						i--;
					if (bf_program[bf_pc] == ']')
						i++;
				}
				bf_pc--;
			}
			return true;
		}
};

#endif /* _BRAINFUCK_H_ */
