#include "Chip8.h"
#include <iostream>

Chip8::Chip8()
{
	pc = 0x200;
	iReg = 0;
	for (int i = 0; i < 16; i++) V[i] = 0;
	for (int i = 0; i < 4096; i++) mem[i] = 0;
	for (int i = 0; i < 80; i++) mem[i] = fontset[i];
	for (int i = 0; i < 16; i++) keys[i] = 0;
	for (int i = 0; i < (64 * 32); i++) screen[i] = 0;
	soundTimer = 60;
	delayTimer = 60;
	drawFlag = false;
	paused = false;
}

void Chip8::emulateCycles(int cycles)
{
	for (int i = 0; i < cycles; i++)
	{
		executeOpcode();
	}
	updateTimers();
}

void Chip8::executeOpcode()
{
	u16 opcode = (mem[pc] << 8) | mem[pc + 1];
	pc += 2;

	u8 x = (opcode >> 8) & 0x000F;
	u8 y = (opcode >> 4) & 0x000F;

	switch (opcode & 0xF000)
	{
		case 0x0000:
			switch (opcode & 0x0FFF)
			{
				case 0x00E0: // CLS
					for (int i = 0; i < 2046; i++) screen[i] = 0;
					break;
				case 0xEE: // RET
					pc = SP->top();
					SP->pop();
					break;
			}
			break; // 0x0XXX instruction switch block
		case 0x1000: // SYS addr
			pc = opcode & 0x0FFF;
			break;
		case 0x2000: // CALL addr
			SP->push(pc);
			pc = opcode & 0x0FFF;
			break;
		case 0x3000: // SE Vx, byte
			if (V[x] == (opcode & 0x00FF)) pc += 2;
			break;
		case 0x4000: // SNE Vx, byte
			if (V[x] != (opcode & 0x00FF)) pc += 2;
			break;
		case 0x5000: // SE Vx, Vy
			if (V[x] == V[y]) pc += 2;
			break;
		case 0x6000: // LD Vx, byte
			V[x] = opcode & 0x00FF;
			break;
		case 0x7000: // ADD Vx, byte
			V[x] += opcode & 0x00FF;
			break;
		case 0x8000:
			switch (opcode & 0x000F) // 0x8XXX instruction switch block
			{

				case 0x0000: // LD Vx, Vy
					V[x] = V[y];
					break;
				case 0x0001: // OR Vx, Vy
					V[x] |= V[y];
					break;
				case 0x0002: // AND Vx, Vy
					V[x] &= V[y];
					break;
				case 0x0003: // XOR Vx, Vy
					V[x] ^= V[y];
					break;
				case 0x0004: // ADD Vx, Vy
					V[x] += V[y];
					if (V[x] < V[y]) V[0xF] = 1;
					else V[0xF] = 0;
					break;
				case 0x0005: // SUB Vx, Vy
					V[x] -= V[y];
					if (V[x] > V[y]) V[0xF] = 0;
					else V[0xF] = 1;
					break;
				case 0x0006: // SHR Vx {, Vy}
					if ((V[x] % 2) == 1) V[0xF] = 1;
					else V[0xF] = 0;
					V[x] /= 2;
					break;
				case 0x0007: // SUBN Vx, Vy
					V[x] = V[y] - V[x];
					if (V[y] > V[x]) V[0xF] = 1;
					else V[0xF] = 0;
					break;
				case 0x000E: // SHL Vx {, Vy}
					if (V[x] >= 0x80) V[0xF] = 1;
					else V[0xF] = 0;
					V[x] *= 2;
					break;
			}
			break; // 0x8XXX instruction switch block
		case 0x9000: // SNE Vx, Vy
			if (V[x] != V[y]) pc += 2;
			break;
		case 0xA000: // LD I, addr
			iReg = opcode & 0x0FFF;
			break;
		case 0xB000: // JP V0, addr
			pc = (opcode & 0x0FFF) + V[0];
			break;
		case 0xC000: // RND Vx, byte
			V[x] = (rand() % 256) & (opcode & 0x00FF);
			break;
		case 0xD000: // DRW Vx, Vy, nibble
		{
			u8 xCoord = V[x];
			u8 yCoord = V[y];
			u8 height = opcode & 0x000F;
			u8 pixel;
			V[0xF] = 0; // Reset flag
			for (int yLine = 0; yLine < height; yLine++)
			{
				pixel = mem[iReg + yLine];
				// Update screen bit by bit
				for (int xLine = 0; xLine < 8; xLine++)
				{
					if ((pixel & (0x0080 >> xLine)) != 0)
					{
						if (screen[(xCoord + xLine + ((yCoord + yLine)) * 64)] == 1)
							V[0xF] = 1;
						screen[(xCoord + xLine + ((yCoord + yLine)) * 64)] ^= 1;
					}
				}
			}
			drawFlag = true;
		}
			break;
		case 0xE000:
			switch (opcode & 0x00FF)
			{
				case 0x09E: // SKP Vx
					if (keys[V[x]] == 1) pc += 2;
					break;
				case 0x00A1: // SKNP Vx
					if (keys[V[x]] != 1) pc += 2;
					break;
			}
			break;
		case 0xF000:
			switch (opcode & 0x00FF) // 0xF000 instruction switch block
			{
				case 0x0007: // LD Vx, DT
					V[x] = delayTimer;
					break;
				case 0x000A: // LD Vx, K
				{
					bool press = false;
					for (int i = 0; i < 16; i++)
					{
						if (keys[i] == 1) press = true;
					}
					if (!press) pc -= 2;
				}
				break;
				case 0x0015: // LD DT, Vx
					delayTimer = V[x];
					break;
				case 0x0018: // LD ST, Vx
					soundTimer = V[x];
					break;
				case 0x001E: // ADD I, Vx
					iReg += V[x];
					break;
				case 0x0029: // LD F, Vx
					iReg = V[x] * 0x5;
					break;
				case 0x0033: // LD B, Vx
					// BCD at iReg
					mem[iReg] = V[x] / 100;
					mem[iReg + 1] = (V[x] / 10) % 10;
					mem[iReg + 2] = V[x] % 10;
					break;
				case 0x0055: // LD [I], Vx
					for (int i = 0; i <= x; i++)
					{
						mem[iReg + i] = V[i];
					}
					break;
				case 0x0065: // LD Vx, [I]
					for (int i = 0; i <= x; i++)
					{
						V[i] = mem[iReg + i];
					}
					break;
			}
			break; // 0xF000 instruction switch block
	}
}

void Chip8::updateTimers()
{
	if (delayTimer > 0) delayTimer--;
	if (soundTimer > 0) soundTimer--;
}

void Chip8::reset()
{
	pc = 0x200;
	iReg = 0;
	for (int i = 0; i < 16; i++) V[i] = 0;
	for (int i = 0; i < 16; i++) keys[i] = 0;
	for (int i = 0; i < (64 * 32); i++) screen[i] = 0;
	soundTimer = 60;
	delayTimer = 60;
	drawFlag = false;
}

bool Chip8::loadRom(const char* fileName)
{
	std::ifstream ifs(fileName, std::ios::binary | std::ios::ate);

	if (!ifs) 
		return false;

	std::streampos ifSize = 0;
	ifSize = ifs.tellg();
	ifs.seekg(0, std::ios::end);

	if (ifs.tellg() - ifSize > 0x800)
		return false;

	std::streampos pos = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	ifs.read(reinterpret_cast<char*>(&mem[0x200]), pos);

	return true;
}
