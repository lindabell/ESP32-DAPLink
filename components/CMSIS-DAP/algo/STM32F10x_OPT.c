/* Flash OS Routines (Automagically Generated)
 * Copyright (c) 2009-2015 ARM Limited
 */
#include "flash_blob.h"

static const uint32_t flash_code_F1[] =
{
	0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
	0x4603B510, 0x4C5E2000, 0x485E6020, 0x485E6060, 0x485C6060, 0x485C60A0, 0x462060A0, 0xF01069C0,
	0xD1080F04, 0x5055F245, 0x60204C58, 0x60602006, 0x70FFF640, 0x200060A0, 0x4601BD10, 0x69004850,
	0x7080F420, 0x61104A4E, 0x69004610, 0x0080F040, 0x20006110, 0x484A4770, 0xF0406900, 0x49480020,
	0x46086108, 0xF0406900, 0x61080040, 0xF64AE003, 0x494620AA, 0x48426008, 0xF01068C0, 0xD1F60F01,
	0x6900483F, 0x0020F020, 0x6108493D, 0x69004608, 0x0010F040, 0xF6456108, 0x493D20A5, 0xE0038008,
	0x20AAF64A, 0x60084939, 0x68C04835, 0x0F01F010, 0x4833D1F6, 0xF0206900, 0x49310010, 0x46086108,
	0xF01068C0, 0xD0060F14, 0x68C04608, 0x0014F040, 0x200160C8, 0x20004770, 0x4601E7FC, 0x69004828,
	0x0020F040, 0x61104A26, 0x69004610, 0x0040F040, 0xE0036110, 0x20AAF64A, 0x60104A24, 0x68C04820,
	0x0F01F010, 0x481ED1F6, 0xF0206900, 0x4A1C0020, 0x20006110, 0x46034770, 0x47702001, 0x4603B510,
	0xF0201C48, 0xE0260101, 0x69004815, 0x0010F040, 0x61204C13, 0x80188810, 0xF64AE003, 0x4C1320AA,
	0x480F6020, 0xF01068C0, 0xD1F60F01, 0x6900480C, 0x0010F020, 0x61204C0A, 0x68C04620, 0x0F14F010,
	0x4620D006, 0xF04068C0, 0x60E00014, 0xBD102001, 0x1C921C9B, 0x29001E89, 0x2000D1D6, 0x0000E7F7,
	0x40022000, 0x45670123, 0xCDEF89AB, 0x40003000, 0x1FFFF800, 0x00000000
};

const program_target_t flash_algo_F1 =
{
	0x20000021,  // Init
	0x2000005B,  // UnInit
	0x20000077,  // EraseChip
	0x200000FB,  // EraseSector
	0x2000013D,  // ProgramPage
	0,

	// BKPT : start of blob + 1
	// RSB  : address to access global/static data
	// RSP  : stack pointer
	{
		0x20000001,
		0x20000C00,
		0x20001000
	},

	0x20000400,  // mem buffer location
	0x20000000,  // location to write prog_blob in target RAM
	sizeof(flash_code_F1),  // prog_blob size
	flash_code_F1,  // address of prog_blob
	0x00000400,  // ram_to_flash_bytes_to_be_written
};
