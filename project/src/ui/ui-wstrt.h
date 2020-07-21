
/***************************************************************************
 *                       _______ _______ _______                           *
 *                      |  ___  |____   |  ___  |                          *
 *                      | |   |_|    / /| |   |_|                          *
 *                      | |_____    / / | |_____                           *
 *                      |_____  |  / /  |_____  |                          *
 *                       _    | | / /    _    | |                          *
 *                      | |___| |/ /____| |___| |                          *
 *                      |_______|_______|_______|                          *
 *                                                                         *
 *                           Wiimms SZS Tools                              *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 *   This file is part of the SZS project.                                 *
 *   Visit https://szs.wiimm.de/ for project details and sources.          *
 *                                                                         *
 *   Copyright (c) 2011-2020 by Dirk Clemens <wiimm@wiimm.de>              *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   See file gpl-2.0.txt or http://www.gnu.org/licenses/gpl-2.0.txt       *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 *   >>>  This file is automatically generated by './src/gen-ui.c'.  <<<   *
 *   >>>                   Do not edit this file!                    <<<   *
 *                                                                         *
 ***************************************************************************/


#ifndef SZS_UI_WSTRT_H
#define SZS_UI_WSTRT_H
#include "dclib-basics.h"
#include "dclib-ui.h"

//
///////////////////////////////////////////////////////////////////////////////
///////////////                enum enumOptions                 ///////////////
///////////////////////////////////////////////////////////////////////////////

typedef enum enumOptions
{
	OPT_NONE,

	//----- command specific options -----

	OPT_LONG,
	OPT_BRIEF,
	OPT_NO_HEADER,
	OPT_CHEAT,
	OPT_ADD_WCODE,
	OPT_DEST,
	OPT_DEST2,
	OPT_ESC,
	OPT_OVERWRITE,
	OPT_NUMBER,
	OPT_REMOVE_DEST,
	OPT_UPDATE,
	OPT_PRESERVE,
	OPT_IGNORE,
	OPT_ADD_LECODE,
	OPT_ADD_CTCODE,
	OPT_MOVE_D8,
	OPT_ADD_SECTION,
	OPT_FULL_GCH,
	OPT_GCT_NO_SEP,
	OPT_GCT_SEP,
	OPT_GCT_ASM_SEP,
	OPT_GCT_LIST,
	OPT_GCT_MOVE,
	OPT_GCT_ADDR,
	OPT_GCT_SPACE,
	OPT_ALLOW_USER_GCH,
	OPT_CREATE_SECT,
	OPT_WPF,
	OPT_SECTIONS,
	OPT_VADDR,
	OPT_FADDR,
	OPT_SNAME,
	OPT_INT1,
	OPT_INT2,
	OPT_INT3,
	OPT_INT4,
	OPT_INT5,
	OPT_INT6,
	OPT_INT7,
	OPT_INT8,
	OPT_FLOAT,
	OPT_DOUBLE,
	OPT_LE,
	OPT_BE,
	OPT_ZEROS,
	OPT_HEX,
	OPT_DEC,
	OPT_C_SYNTAX,
	OPT_ADDR,
	OPT_ALIGN,
	OPT_TRIGGER,
	OPT_NO_ADDR,
	OPT_NO_NUMBERS,
	OPT_NO_TEXT,
	OPT_FORMAT,
	OPT_NO_NULL,
	OPT_NARROW,
	OPT_SMALL,
	OPT_WIDE,
	OPT_BYTES,

	OPT__N_SPECIFIC, // == 62

	//----- global options -----

	OPT_VERSION,
	OPT_HELP,
	OPT_XHELP,
	OPT_AT_DUMMY,
	OPT_ALLOW_ALL,
	OPT_COMPATIBLE,
	OPT_WIDTH,
	OPT_QUIET,
	OPT_VERBOSE,
	OPT_LOGGING,
	OPT_DE,
	OPT_COLORS,
	OPT_NO_COLORS,
	OPT_CT_CODE,
	OPT_LE_CODE,
	OPT_CHDIR,
	OPT_CONST,
	OPT_TRACKS,
	OPT_ARENAS,
	OPT_REGION,
	OPT_VS_REGION,
	OPT_BT_REGION,
	OPT_ALL_RANKS,
	OPT_POINTS,
	OPT_HTTPS,
	OPT_DOMAIN,
	OPT_WIIMMFI,
	OPT_TWIIMMFI,
	OPT_WC24,
	OPT_WCODE,
	OPT_PB_MODE,
	OPT_PATCHED_BY,
	OPT_VS,
	OPT_VS2,
	OPT_BT,
	OPT_BT2,
	OPT_CANNON,
	OPT_MENO,
	OPT_UTF_8,
	OPT_NO_UTF_8,
	OPT_TEST,
	OPT_FORCE,
	OPT_REPAIR_MAGICS,
	OPT_OLD,
	OPT_STD,
	OPT_NEW,
	OPT_EXTRACT,
	OPT_CLEAN_DOL,
	OPT_CT_DIR,

	OPT__N_TOTAL // == 111

} enumOptions;

//
///////////////////////////////////////////////////////////////////////////////
///////////////               enum enumOptionsBit               ///////////////
///////////////////////////////////////////////////////////////////////////////

//	*****  only for verification  *****

//typedef enum enumOptionsBit
//{
//	//----- command specific options -----
//
//	OB_LONG			= 1llu << OPT_LONG,
//	OB_BRIEF		= 1llu << OPT_BRIEF,
//	OB_NO_HEADER		= 1llu << OPT_NO_HEADER,
//	OB_CHEAT		= 1llu << OPT_CHEAT,
//	OB_ADD_WCODE		= 1llu << OPT_ADD_WCODE,
//	OB_DEST			= 1llu << OPT_DEST,
//	OB_DEST2		= 1llu << OPT_DEST2,
//	OB_ESC			= 1llu << OPT_ESC,
//	OB_OVERWRITE		= 1llu << OPT_OVERWRITE,
//	OB_NUMBER		= 1llu << OPT_NUMBER,
//	OB_REMOVE_DEST		= 1llu << OPT_REMOVE_DEST,
//	OB_UPDATE		= 1llu << OPT_UPDATE,
//	OB_PRESERVE		= 1llu << OPT_PRESERVE,
//	OB_IGNORE		= 1llu << OPT_IGNORE,
//	OB_ADD_LECODE		= 1llu << OPT_ADD_LECODE,
//	OB_ADD_CTCODE		= 1llu << OPT_ADD_CTCODE,
//	OB_MOVE_D8		= 1llu << OPT_MOVE_D8,
//	OB_ADD_SECTION		= 1llu << OPT_ADD_SECTION,
//	OB_FULL_GCH		= 1llu << OPT_FULL_GCH,
//	OB_GCT_NO_SEP		= 1llu << OPT_GCT_NO_SEP,
//	OB_GCT_SEP		= 1llu << OPT_GCT_SEP,
//	OB_GCT_ASM_SEP		= 1llu << OPT_GCT_ASM_SEP,
//	OB_GCT_LIST		= 1llu << OPT_GCT_LIST,
//	OB_GCT_MOVE		= 1llu << OPT_GCT_MOVE,
//	OB_GCT_ADDR		= 1llu << OPT_GCT_ADDR,
//	OB_GCT_SPACE		= 1llu << OPT_GCT_SPACE,
//	OB_ALLOW_USER_GCH	= 1llu << OPT_ALLOW_USER_GCH,
//	OB_CREATE_SECT		= 1llu << OPT_CREATE_SECT,
//	OB_WPF			= 1llu << OPT_WPF,
//	OB_SECTIONS		= 1llu << OPT_SECTIONS,
//	OB_VADDR		= 1llu << OPT_VADDR,
//	OB_FADDR		= 1llu << OPT_FADDR,
//	OB_SNAME		= 1llu << OPT_SNAME,
//	OB_INT1			= 1llu << OPT_INT1,
//	OB_INT2			= 1llu << OPT_INT2,
//	OB_INT3			= 1llu << OPT_INT3,
//	OB_INT4			= 1llu << OPT_INT4,
//	OB_INT5			= 1llu << OPT_INT5,
//	OB_INT6			= 1llu << OPT_INT6,
//	OB_INT7			= 1llu << OPT_INT7,
//	OB_INT8			= 1llu << OPT_INT8,
//	OB_FLOAT		= 1llu << OPT_FLOAT,
//	OB_DOUBLE		= 1llu << OPT_DOUBLE,
//	OB_LE			= 1llu << OPT_LE,
//	OB_BE			= 1llu << OPT_BE,
//	OB_ZEROS		= 1llu << OPT_ZEROS,
//	OB_HEX			= 1llu << OPT_HEX,
//	OB_DEC			= 1llu << OPT_DEC,
//	OB_C_SYNTAX		= 1llu << OPT_C_SYNTAX,
//	OB_ADDR			= 1llu << OPT_ADDR,
//	OB_ALIGN		= 1llu << OPT_ALIGN,
//	OB_TRIGGER		= 1llu << OPT_TRIGGER,
//	OB_NO_ADDR		= 1llu << OPT_NO_ADDR,
//	OB_NO_NUMBERS		= 1llu << OPT_NO_NUMBERS,
//	OB_NO_TEXT		= 1llu << OPT_NO_TEXT,
//	OB_FORMAT		= 1llu << OPT_FORMAT,
//	OB_NO_NULL		= 1llu << OPT_NO_NULL,
//	OB_NARROW		= 1llu << OPT_NARROW,
//	OB_SMALL		= 1llu << OPT_SMALL,
//	OB_WIDE			= 1llu << OPT_WIDE,
//	OB_BYTES		= 1llu << OPT_BYTES,
//
//	//----- group & command options -----
//
//	OB_GRP_DEST		= OB_DEST
//				| OB_DEST2
//				| OB_ESC
//				| OB_OVERWRITE
//				| OB_NUMBER
//				| OB_REMOVE_DEST
//				| OB_UPDATE
//				| OB_PRESERVE,
//
//	OB_GRP_PATCH		= 0,
//
//	OB_GRP_HEXDUMP		= OB_INT1
//				| OB_INT2
//				| OB_INT3
//				| OB_INT4
//				| OB_INT5
//				| OB_INT6
//				| OB_INT7
//				| OB_INT8
//				| OB_FLOAT
//				| OB_DOUBLE
//				| OB_LE
//				| OB_BE
//				| OB_ZEROS
//				| OB_HEX
//				| OB_DEC
//				| OB_C_SYNTAX
//				| OB_ADDR
//				| OB_ALIGN
//				| OB_TRIGGER
//				| OB_NO_ADDR
//				| OB_NO_NUMBERS
//				| OB_NO_TEXT
//				| OB_FORMAT
//				| OB_NO_NULL
//				| OB_NARROW
//				| OB_SMALL
//				| OB_WIDE
//				| OB_BYTES,
//
//	OB_CMD_VERSION		= OB_BRIEF
//				| OB_SECTIONS
//				| OB_LONG,
//
//	OB_CMD_HELP		= ~(u64)0,
//
//	OB_CMD_ARGTEST		= ~(u64)0,
//
//	OB_CMD_TEST		= ~(u64)0,
//
//	OB_CMD_COLORS		= OB_LONG
//				| OB_BRIEF,
//
//	OB_CMD_ERROR		= OB_SECTIONS
//				| OB_NO_HEADER
//				| OB_LONG
//				| OB_BRIEF,
//
//	OB_CMD_FILETYPE		= OB_LONG
//				| OB_IGNORE,
//
//	OB_CMD_FILEATTRIB	= OB_NO_HEADER,
//
//	OB_CMD_RAWDUMP		= OB_DEST
//				| OB_DEST2,
//
//	OB_CMD_DUMP		= OB_LONG,
//
//	OB_CMD_HEXDUMP		= OB_VADDR
//				| OB_FADDR
//				| OB_SNAME
//				| OB_GRP_HEXDUMP,
//
//	OB_CMD_CDUMP		= 0,
//
//	OB_CMD_HTTPS		= OB_LONG,
//
//	OB_CMD_EXTRACT		= OB_DEST
//				| OB_DEST2
//				| OB_OVERWRITE,
//
//	OB_CMD_TRACKS		= OB_NO_HEADER
//				| OB_BRIEF
//				| OB_LONG
//				| OB_GRP_PATCH,
//
//	OB_CMD_ARENAS		= OB_CMD_TRACKS,
//
//	OB_CMD_FILES		= OB_GRP_PATCH,
//
//	OB_CMD_FIND		= OB_GRP_PATCH
//				| OB_LONG,
//
//	OB_CMD_POINTS		= OB_BRIEF
//				| OB_LONG
//				| OB_CHEAT,
//
//	OB_CMD_ANALYZE		= OB_GRP_PATCH
//				| OB_LONG,
//
//	OB_CMD_PATCH		= OB_GRP_DEST
//				| OB_IGNORE
//				| OB_GRP_PATCH
//				| OB_ADD_LECODE
//				| OB_ADD_CTCODE
//				| OB_MOVE_D8
//				| OB_ADD_SECTION
//				| OB_ADD_WCODE
//				| OB_FULL_GCH
//				| OB_GCT_NO_SEP
//				| OB_GCT_SEP
//				| OB_GCT_ASM_SEP
//				| OB_GCT_LIST
//				| OB_GCT_MOVE
//				| OB_GCT_ADDR
//				| OB_GCT_SPACE
//				| OB_ALLOW_USER_GCH
//				| OB_CREATE_SECT
//				| OB_WPF,
//
//} enumOptionsBit;

//
///////////////////////////////////////////////////////////////////////////////
///////////////                enum enumCommands                ///////////////
///////////////////////////////////////////////////////////////////////////////

typedef enum enumCommands
{
	CMD__NONE,

	CMD_VERSION,
	CMD_HELP,
	CMD_ARGTEST,
	CMD_TEST,
	CMD_COLORS,
	CMD_ERROR,
	CMD_FILETYPE,
	CMD_FILEATTRIB,
	CMD_RAWDUMP,

	CMD_DUMP,
	CMD_CDUMP,
	CMD_HEXDUMP,
	CMD_HTTPS,
	CMD_EXTRACT,

	CMD_TRACKS,
	CMD_ARENAS,
	CMD_FILES,
	CMD_FIND,
	CMD_POINTS,

	CMD_ANALYZE,
	CMD_PATCH,

	CMD__N // == 22

} enumCommands;

//
///////////////////////////////////////////////////////////////////////////////
///////////////                   enumGetOpt                    ///////////////
///////////////////////////////////////////////////////////////////////////////

typedef enum enumGetOpt
{
	GO__ERR			= '?',

	GO_AT_DUMMY		= '@',

	GO_ARENAS		= 'A',
	GO_BRIEF		= 'B',
	GO_DEST2		= 'D',
	GO_ESC			= 'E',
	GO_NO_HEADER		= 'H',
	GO_LOGGING		= 'L',
	GO_REGION		= 'R',
	GO_TRACKS		= 'T',
	GO_VERSION		= 'V',

	GO_CONST		= 'c',
	GO_DEST			= 'd',
	GO_HELP			= 'h',
	GO_IGNORE		= 'i',
	GO_LONG			= 'l',
	GO_OVERWRITE		= 'o',
	GO_PRESERVE		= 'p',
	GO_QUIET		= 'q',
	GO_REMOVE_DEST		= 'r',
	GO_TEST			= 't',
	GO_UPDATE		= 'u',
	GO_VERBOSE		= 'v',

	GO_XHELP		= 0x80,
	GO_ALLOW_ALL,
	GO_COMPATIBLE,
	GO_WIDTH,
	GO_DE,
	GO_COLORS,
	GO_NO_COLORS,
	GO_CT_CODE,
	GO_LE_CODE,
	GO_CHDIR,
	GO_VS_REGION,
	GO_BT_REGION,
	GO_ALL_RANKS,
	GO_POINTS,
	GO_CHEAT,
	GO_HTTPS,
	GO_DOMAIN,
	GO_WIIMMFI,
	GO_TWIIMMFI,
	GO_WC24,
	GO_WCODE,
	GO_ADD_WCODE,
	GO_PB_MODE,
	GO_PATCHED_BY,
	GO_VS,
	GO_VS2,
	GO_BT,
	GO_BT2,
	GO_CANNON,
	GO_MENO,
	GO_UTF_8,
	GO_NO_UTF_8,
	GO_FORCE,
	GO_REPAIR_MAGICS,
	GO_OLD,
	GO_STD,
	GO_NEW,
	GO_EXTRACT,
	GO_NUMBER,
	GO_CLEAN_DOL,
	GO_ADD_LECODE,
	GO_ADD_CTCODE,
	GO_CT_DIR,
	GO_MOVE_D8,
	GO_ADD_SECTION,
	GO_FULL_GCH,
	GO_GCT_NO_SEP,
	GO_GCT_SEP,
	GO_GCT_ASM_SEP,
	GO_GCT_LIST,
	GO_GCT_MOVE,
	GO_GCT_ADDR,
	GO_GCT_SPACE,
	GO_ALLOW_USER_GCH,
	GO_CREATE_SECT,
	GO_WPF,
	GO_SECTIONS,
	GO_VADDR,
	GO_FADDR,
	GO_SNAME,
	GO_INT1,
	GO_INT2,
	GO_INT3,
	GO_INT4,
	GO_INT5,
	GO_INT6,
	GO_INT7,
	GO_INT8,
	GO_FLOAT,
	GO_DOUBLE,
	GO_LE,
	GO_BE,
	GO_ZEROS,
	GO_HEX,
	GO_DEC,
	GO_C_SYNTAX,
	GO_ADDR,
	GO_ALIGN,
	GO_TRIGGER,
	GO_NO_ADDR,
	GO_NO_NUMBERS,
	GO_NO_TEXT,
	GO_FORMAT,
	GO_NO_NULL,
	GO_NARROW,
	GO_SMALL,
	GO_WIDE,
	GO_BYTES,

} enumGetOpt;

//
///////////////////////////////////////////////////////////////////////////////
///////////////                  external vars                  ///////////////
///////////////////////////////////////////////////////////////////////////////

//extern const InfoOption_t OptionInfo[OPT__N_TOTAL+1];
//extern const KeywordTab_t CommandTab[];
//extern const char OptionShort[];
//extern const struct option OptionLong[];
//extern u8 OptionUsed[OPT__N_TOTAL+1];
//extern const OptionIndex_t OptionIndex[UIOPT_INDEX_SIZE];
//UIOPT_INDEX_SIZE := 0x12c = 300
//extern const InfoCommand_t CommandInfo[CMD__N+1];
extern const InfoUI_t InfoUI_wstrt;

//
///////////////////////////////////////////////////////////////////////////////
///////////////                       END                       ///////////////
///////////////////////////////////////////////////////////////////////////////

#endif // SZS_UI_WSTRT_H
