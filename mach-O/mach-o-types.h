#ifndef __mach_o_types_h__
#define __mach_o_types_h__

#include <sys/types.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/fat.h>
#include <ar.h>

typedef struct mach_header_64 MACHO_header_64_t;

typedef struct load_command MACHO_load_command_t;

typedef struct segment_command_64 MACHO_segment_command_64_t;
typedef struct section_64 MACHO_section_64_t;
/*
#define	MH_MAGIC	0xfeedface
#define MH_CIGAM	NXSwapInt(MH_MAGIC)
#define MH_MAGIC_64	0xfeedfacf
#define MH_CIGAM_64	NXSwapInt(MH_MAGIC_64)
*/
#endif
