#ifndef MACHO_H
#define MACHO_H

#include <iostream>
#include <cstring>

#include "modified_loader.h"

struct mach_header_64 {
	uint32_t	magic;		/* mach magic number identifier */
	cpu_type_t	cputype;	/* cpu specifier */
	cpu_subtype_t	cpusubtype;	/* machine specifier */
	uint32_t	filetype;	/* type of file */
	uint32_t	ncmds;		/* number of load commands */
	uint32_t	sizeofcmds;	/* the size of all the load commands */
	uint32_t	flags;		/* flags */
	uint32_t	reserved;	/* reserved */

    explicit mach_header_64(const char *);
};

std::ostream& operator<<(std::ostream& out, const mach_header_64& mh);

#endif