#ifndef MACHO_H
#define MACHO_H

#include <iostream>
#include <cstring>
#include <string_view>
#include <vector>

#include "modified_loader.h"

struct mach_header_64 {
    uint32_t magic;        /* mach magic number identifier */
    cpu_type_t cputype;    /* cpu specifier */
    cpu_subtype_t cpusubtype;    /* machine specifier */
    uint32_t filetype;    /* type of file */
    uint32_t ncmds;        /* number of load commands */
    uint32_t sizeofcmds;    /* the size of all the load commands */
    uint32_t flags;        /* flags */
    uint32_t reserved;    /* reserved */

    mach_header_64() = default;

    explicit mach_header_64(const char *);

    static std::string describe_magic(decltype(mach_header_64::magic));

    static std::string describe_filetype(decltype(mach_header_64::filetype));

    static std::string describe_flags(decltype(mach_header_64::flags));
};

std::ostream& operator<<(std::ostream& out, const mach_header_64& mh);

/*
 * The load commands directly follow the mach_header.  The total size of all
 * of the commands is given by the sizeofcmds field in the mach_header.  All
 * load commands must have as their first two fields cmd and cmdsize.  The cmd
 * field is filled in with a constant for that command type.  Each command type
 * has a structure specifically for it.  The cmdsize field is the size in bytes
 * of the particular load command structure plus anything that follows it that
 * is a part of the load command (i.e. section structures, strings, etc.).  To
 * advance to the next load command the cmdsize can be added to the offset or
 * pointer of the current load command.  The cmdsize for 32-bit architectures
 * MUST be a multiple of 4 bytes and for 64-bit architectures MUST be a multiple
 * of 8 bytes (these are forever the maximum alignment of any load commands).
 * The padded bytes must be zero.  All tables in the object file must also
 * follow these rules so the file can be memory mapped.  Otherwise the pointers
 * to these tables will not work well or at all on some machines.  With all
 * padding zeroed like objects will compare byte for byte.
 */
struct load_command {
	uint32_t cmd;		/* type of load command */
	uint32_t cmdsize;	/* total size of command in bytes */
	char* data;
	
	explicit load_command(const char *);
    static std::string const describe_cmd(decltype(load_command::cmd));
};
std::ostream& operator<<(std::ostream&, const load_command&);
/*
 * The 64-bit segment load command indicates that a part of this file is to be
 * mapped into a 64-bit task's address space.  If the 64-bit segment has
 * sections then section_64 structures directly follow the 64-bit segment
 * command and their size is reflected in cmdsize.
 */
struct segment_command_64 : load_command { /* for 64-bit architectures */
	uint32_t	cmd;		/* LC_SEGMENT_64 */
	uint32_t	cmdsize;	/* includes sizeof section_64 structs */
	char		segname[16];	/* segment name */
	uint64_t	vmaddr;		/* memory address of this segment */
	uint64_t	vmsize;		/* memory size of this segment */
	uint64_t	fileoff;	/* file offset of this segment */
	uint64_t	filesize;	/* amount to map from the file */
	vm_prot_t	maxprot;	/* maximum VM protection */
	vm_prot_t	initprot;	/* initial VM protection */
	uint32_t	nsects;		/* number of sections in segment */
	uint32_t	flags;		/* flags */

	explicit segment_command_64(const char *);
};
std::ostream& operator<<(std::ostream&, const segment_command_64&);


struct mach_file {
	mach_header_64 header;
	std::vector<load_command> commands;

	explicit mach_file(const char *);
};

std::ostream& operator<<(std::ostream&, const mach_file&);
#endif
