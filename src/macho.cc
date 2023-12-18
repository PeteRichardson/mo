#include "macho.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <tuple>

template <typename T>
void _put_dec(std::ostream& out, std::string label, T value, std::string comment="") {
    out << std::setw(14) << std::setfill(' ') << std::left << std::nouppercase
    << label
    << std::setw(10) << std::setfill(' ') << std::right << std::dec << std::noshowbase
    << value;
    if (comment.length() > 0) {
        out << "   # "
        << std::nouppercase
        << comment;
    }
    out << std::endl;
}

template <typename T>
void _put_hex(std::ostream& out, std::string label, T value, std::string comment="") {
    out << std::setw(14) << std::setfill(' ') << std::left << std::nouppercase
    << label
    << "0x"
    << std::setw(8) << std::setfill('0') << std::right 
    << std::hex << std::noshowbase << std::uppercase
    << value;
    if (comment.length() > 0) {
        out << "   # "
        << std::nouppercase
        << comment;
    }
    out << std::endl;
}

std::string mach_header_64::describe_magic(decltype(mach_header_64::magic) magic) {
    switch (magic) {
        case MH_MAGIC: return "MH_MAGIC: the mach magic number"; break;
        case MH_CIGAM: return "MH_CIGAM: NXSwapInt(MH_MAGIC)"; break;
        case MH_MAGIC_64: return "MH_MAGIC_64: the 64-bit mach magic number"; break;
        case MH_CIGAM_64: return "MH_CIGAM_64: NXSwapInt(MH_MAGIC_64)"; break;
    }
    return "MH_?????: Unrecognized mach header magic number";
}

std::string mach_header_64::describe_filetype(decltype(mach_header_64::filetype) filetype) {

    switch (filetype) {
        case MH_OBJECT: return "relocatable object file";
            break;
        case	MH_EXECUTE:     return "MH_EXECUTE: demand paged executable file"; break;
        case	MH_FVMLIB:      return "MH_FVMLIB: fixed VM shared library file"; break;
        case	MH_CORE:        return "MH_CORE: core file"; break;
        case	MH_PRELOAD:     return "MH_PRELOAD: preloaded executable file"; break;
        case	MH_DYLIB:       return "MH_DYLIB: dynamically bound shared library"; break;
        case	MH_DYLINKER:    return "MH_DYLINKER: dynamic link editor"; break;
        case	MH_BUNDLE:      return "MH_BUNDLE: dynamically bound bundle file"; break;
        case	MH_DYLIB_STUB:  return "MH_DYLIB_STUB: shared library stub for static linking only, no section contents"; break;
        case	MH_DSYM:        return "MH_DSYM: companion file with only debug sections"; break;
        case	MH_KEXT_BUNDLE: return "MH_KEXT_BUNDLE: x86_64 kexts"; break;
        case    MH_FILESET:     return "MH_FILESET: a file composed of other Mach-Os to be run in the same userspace sharing  a single linkedit."; break;
    }
    return "MH_?????: Unrecognized mach header filetype";
}

std::string mach_header_64::describe_flags(decltype(mach_header_64::flags) flags) {
    std::stringstream result;
    std::vector<std::pair<uint32_t, std::string>> defined_flags = {
        std::make_pair(MH_NOUNDEFS, "MH_NOUNDEFS: the object file has no undefined references"),
        std::make_pair(MH_INCRLINK, "MH_INCRLINK: the object file is the output of an incremental link against a base file and can't be link edited again"),
        std::make_pair(MH_DYLDLINK, "MH_DYLDLINK: the object file is input for the dynamic linker and can't be staticly link edited again"),
        std::make_pair(MH_BINDATLOAD, "MH_BINDATLOAD: the object file's undefined references are bound by the dynamic linker when loaded."),
        std::make_pair(MH_PREBOUND, "MH_PREBOUNDthe file has its dynamic undefined references prebound."),
        std::make_pair(MH_SPLIT_SEGS, "MH_SPLIT_SEGS: the file has its read-only and read-write segments split"),
        std::make_pair(MH_LAZY_INIT, "MH_LAZY_INIT: the shared library init routine is to be run lazily via catching memory faults to its writeable segments (obsolete)"),
        std::make_pair(MH_TWOLEVEL, "MH_TWOLEVEL: the image is using two-level name space bindings"),
        std::make_pair(MH_FORCE_FLAT, "MH_FORCE_FLAT: the executable is forcing all images to use flat name space bindings"),
        std::make_pair(MH_NOMULTIDEFS, "MH_NOMULTIDEFS: this umbrella guarantees no multiple defintions of symbols in its sub-images so the two-level namespace hints can always be used."),
        std::make_pair(MH_NOFIXPREBINDING, "MH_NOFIXPREBINDING: do not have dyld notify the prebinding agent about this executable"),
        std::make_pair(MH_PREBINDABLE, "MH_PREBINDABLE: the binary is not prebound but can have its prebinding redone. only used when MH_PREBOUND is not set."),
        std::make_pair(MH_ALLMODSBOUND, "MH_ALLMODSBOUND: indicates that this binary binds to all two-level namespace modules of its dependent libraries. only used when MH_PREBINDABLE and MH_TWOLEVEL are both set."),
        std::make_pair(MH_SUBSECTIONS_VIA_SYMBOLS, "MH_SUBSECTIONS_VIA_SYMBOLS: safe to divide up the sections into sub-sections via symbols for dead code stripping"),
        std::make_pair(MH_CANONICAL, "MH_CANONICAL: the binary has been canonicalized via the unprebind operation"),
        std::make_pair(MH_WEAK_DEFINES, "MH_WEAK_DEFINES: the final linked image contains external weak symbols"),
        std::make_pair(MH_BINDS_TO_WEAK, "MH_BINDS_TO_WEAK: the final linked image uses weak symbols"),
        std::make_pair(MH_ALLOW_STACK_EXECUTION, "MH_ALLOW_STACK_EXECUTION: When this bit is set, all stacks in the task will be given stack execution privilege.  Only used in MH_EXECUTE filetypes."),
        std::make_pair(MH_ROOT_SAFE, "MH_ROOT_SAFE: When this bit is set, the binary declares it is safe for use in processes with uid zero"),
        std::make_pair(MH_SETUID_SAFE, "MH_SETUID_SAFE: When this bit is set, the binary declares it is safe for use in processes when issetugid() is true"),
        std::make_pair(MH_NO_REEXPORTED_DYLIBS, "MH_NO_REEXPORTED_DYLIBS: When this bit is set on a dylib, the static linker does not need to examine dependent dylibs to see if any are re-exported"),
        std::make_pair(MH_PIE, "MH_PIE: When this bit is set, the OS will load the main executable at a random address.  Only used in MH_EXECUTE filetypes."),
        std::make_pair(MH_DEAD_STRIPPABLE_DYLIB, "MH_DEAD_STRIPPABLE_DYLIB: Only for use on dylibs.  When linking against a dylib that has this bit set, the static linker will automatically not create a LC_LOAD_DYLIB load command to the dylib if no symbols are being referenced from the dylib."),
        std::make_pair(MH_HAS_TLV_DESCRIPTORS, "MH_HAS_TLV_DESCRIPTORS: Contains a section of type S_THREAD_LOCAL_VARIABLES"),
        std::make_pair(MH_NO_HEAP_EXECUTION, "MH_NO_HEAP_EXECUTION: When this bit is set, the OS will run the main executable with a non-executable heap even on platforms (e.g. i386) that don't require it. Only used in MH_EXECUTE filetypes."),
        std::make_pair(MH_APP_EXTENSION_SAFE, "MH_APP_EXTENSION_SAFE: The code was linked for use in an application extension."),
        std::make_pair(MH_NLIST_OUTOFSYNC_WITH_DYLDINFO, "MH_NLIST_OUTOFSYNC_WITH_DYLDINFO: The external symbols listed in the nlist symbol table do not include all the symbols listed in the dyld info."),
        std::make_pair(MH_SIM_SUPPORT, "MH_SIM_SUPPORT: Allow LC_MIN_VERSION_MACOS and LC_BUILD_VERSION load commands with the platforms macOS, macCatalyst, iOSSimulator, tvOSSimulator and watchOSSimulator."),
        std::make_pair(MH_DYLIB_IN_CACHE, "MH_DYLIB_IN_CACHE: Only for use on dylibs. When this bit is set, the dylib is part of the dyld shared cache, rather than loose in the filesystem."),
    };
    for (const auto& [fl, desc]: defined_flags) {
        if (flags & fl)
            _put_hex(result, "\t", fl, desc);
    }
    return result.str();
}

mach_header_64::mach_header_64(const char *data) {
    uint32_t o = 0; // offset into incoming data
    std::memcpy(&(this->magic),      &data[o], sizeof(decltype(this->magic)));      o += sizeof(decltype(this->magic));
    std::memcpy(&(this->cputype),    &data[o], sizeof(decltype(this->cputype)));    o += sizeof(decltype(this->cputype));
    std::memcpy(&(this->cpusubtype), &data[o], sizeof(decltype(this->cpusubtype))); o += sizeof(decltype(this->cpusubtype));
    std::memcpy(&(this->filetype),   &data[o], sizeof(decltype(this->filetype)));   o += sizeof(decltype(this->filetype));
    std::memcpy(&(this->ncmds),      &data[o], sizeof(decltype(this->ncmds)));      o += sizeof(decltype(this->ncmds));
    std::memcpy(&(this->sizeofcmds), &data[o], sizeof(decltype(this->sizeofcmds))); o += sizeof(decltype(this->sizeofcmds));
    std::memcpy(&(this->flags),      &data[o], sizeof(decltype(this->flags)));      o += sizeof(decltype(this->flags));
    std::memcpy(&(this->reserved),   &data[o], sizeof(decltype(this->reserved)));
}

std::ostream& operator<<(std::ostream& out, const mach_header_64& mh) {
    out << std::hex << std::showbase;
    _put_hex(out, "\tmagic:", mh.magic, mach_header_64::describe_magic(mh.magic));
    _put_hex(out, "\tcputype:", mh.cputype, "cpu specifier");
    _put_hex(out, "\tcpusubtype:", mh.cpusubtype, "machine specifier");
    _put_hex(out, "\tfiletype:", mh.filetype, mach_header_64::describe_filetype(mh.filetype));
    _put_dec(out, "\tncmds:", mh.ncmds, "number of load commands");
    _put_dec(out, "\tsizeofcmds:", mh.sizeofcmds, "the size of all the load commands");
    _put_hex(out, "\tflags:", mh.flags);
    out << mach_header_64::describe_flags(mh.flags);
    _put_hex(out, "\treserved:", mh.reserved, "reserved");
    return out;
}

load_command::load_command(const char *data) {
    uint32_t o = 0; // offset into incoming data
    std::memcpy(&(this->cmd),      &data[o],  sizeof(decltype(this->cmd)));      o += sizeof(decltype(this->cmd));
    std::memcpy(&(this->cmdsize),  &data[o],  sizeof(decltype(this->cmdsize)));
};

std::string const load_command::describe_cmd(decltype(load_command::cmd) cmd) {

    switch (cmd) {
        case LC_SEGMENT:                  return "LC_SEGMENT: segment of this file to be mapped"; break;
        case LC_SYMTAB:                   return "LC_SYMTAB: link-edit stab symbol table info"; break;
        case LC_SYMSEG:                   return "LC_SYMSEG: link-edit gdb symbol table info (obsolete)"; break;
        case LC_THREAD:                   return "LC_THREAD: thread"; break;
        case LC_UNIXTHREAD:               return "LC_UNIXTHREAD: unix thread (includes a stack)"; break;
        case LC_LOADFVMLIB:               return "LC_LOADFVMLIB: load a specified fixed VM shared library"; break;
        case LC_IDFVMLIB:                 return "LC_IDFVMLIB: fixed VM shared library identification"; break;
        case LC_IDENT:                    return "LC_IDENT: object identification info (obsolete)"; break;
        case LC_FVMFILE:                  return "LC_FVMFILE: fixed VM file inclusion (internal use)"; break;
        case LC_PREPAGE:                  return "LC_PREPAGE: prepage command (internal use)"; break;
        case LC_DYSYMTAB:                 return "LC_DYSYMTAB: dynamic link-edit symbol table info"; break;
        case LC_LOAD_DYLIB:               return "LC_LOAD_DYLIB: load a dynamically linked shared library"; break;
        case LC_ID_DYLIB:                 return "LC_ID_DYLIB: dynamically linked shared lib ident"; break;
        case LC_LOAD_DYLINKER:            return "LC_LOAD_DYLINKER: load a dynamic linker"; break;
        case LC_ID_DYLINKER:              return "LC_ID_DYLINKER: dynamic linker identification"; break;
        case LC_PREBOUND_DYLIB:           return "LC_PREBOUND_DYLIB: modules prebound for a dynamically linked shared library"; break;
        case LC_ROUTINES:                 return "LC_ROUTINES: image routines"; break;
        case LC_SUB_FRAMEWORK:            return "LC_SUB_FRAMEWORK: sub framework"; break;
        case LC_SUB_UMBRELLA:             return "LC_SUB_UMBRELLA: sub umbrella"; break;
        case LC_SUB_CLIENT:               return "LC_SUB_CLIENT: sub client"; break;
        case LC_SUB_LIBRARY:              return "LC_SUB_LIBRARY: sub library"; break;
        case LC_TWOLEVEL_HINTS:           return "LC_TWOLEVEL_HINTS: two-level namespace lookup hints"; break;
        case LC_PREBIND_CKSUM:            return "LC_PREBIND_CKSUM: prebind checksum"; break;
        case LC_LOAD_WEAK_DYLIB:          return "LC_LOAD_WEAK_DYLIB:"; break;
        case LC_SEGMENT_64:               return "LC_SEGMENT_64: 64-bit segment of this file to be mapped"; break;
        case LC_ROUTINES_64:              return "LC_ROUTINES_64: 64-bit image routines"; break;
        case LC_UUID:                     return "LC_UUID: the uuid"; break;
        case LC_RPATH:                    return "LC_RPATH: runpath additions"; break;
        case LC_CODE_SIGNATURE:           return "LC_CODE_SIGNATURE: local of code signature"; break;
        case LC_SEGMENT_SPLIT_INFO:       return "LC_SEGMENT_SPLIT_INFO: local of info to split segments"; break;
        case LC_REEXPORT_DYLIB:           return "LC_REEXPORT_DYLIB: load and re-export dylib"; break;
        case LC_LAZY_LOAD_DYLIB:          return "LC_LAZY_LOAD_DYLIB: delay load of dylib until first use"; break;
        case LC_ENCRYPTION_INFO:          return "LC_ENCRYPTION_INFO: encrypted segment information"; break;
        case LC_DYLD_INFO :               return "LC_DYLD_INFO: compressed dyld information"; break;
        case LC_DYLD_INFO_ONLY:	          return "LC_DYLD_INFO_ONLY: compressed dyld information only"; break;
        case LC_LOAD_UPWARD_DYLIB:        return "LC_LOAD_UPWARD_DYLIB: load upward dylib"; break;
        case LC_VERSION_MIN_MACOSX:       return "LC_VERSION_MIN_MACOSX: build for MacOSX min OS version"; break;
        case LC_VERSION_MIN_IPHONEOS:     return "LC_VERSION_MIN_IPHONEOS: build for iPhoneOS min OS version"; break;
        case LC_FUNCTION_STARTS:          return "LC_FUNCTION_STARTS: compressed table of function start addresses"; break;
        case LC_DYLD_ENVIRONMENT:         return "LC_DYLD_ENVIRONMENT: string for dyld to treat like environment variable"; break;
        case LC_MAIN:                     return "LC_MAIN: replacement for LC_UNIXTHREAD"; break;
        case LC_DATA_IN_CODE:             return "LC_DATA_IN_CODE: table of non-instructions in __text"; break;
        case LC_SOURCE_VERSION:           return "LC_SOURCE_VERSION: source version used to build binary"; break;
        case LC_DYLIB_CODE_SIGN_DRS:      return "LC_DYLIB_CODE_SIGN_DRS: Code signing DRs copied from linked dylibs"; break;
        case LC_ENCRYPTION_INFO_64:       return "LC_ENCRYPTION_INFO_64: 64-bit encrypted segment information"; break;
        case LC_LINKER_OPTION:            return "LC_LINKER_OPTION: linker options in MH_OBJECT files"; break;
        case LC_LINKER_OPTIMIZATION_HINT: return "LC_LINKER_OPTIMIZATION_HINT: optimization hints in MH_OBJECT files"; break;
        case LC_VERSION_MIN_TVOS:         return "LC_VERSION_MIN_TVOS: build for AppleTV min OS version"; break;
        case LC_VERSION_MIN_WATCHOS:      return "LC_VERSION_MIN_WATCHOS: build for Watch min OS version"; break;
        case LC_NOTE:                     return "LC_NOTE: arbitrary data included within a Mach-O file"; break;
        case LC_BUILD_VERSION:            return "LC_BUILD_VERSION: build for platform min OS version"; break;
        case LC_DYLD_EXPORTS_TRIE:        return "LC_DYLD_EXPORTS_TRIE used with linkedit_data_command, payload is trie"; break;
        case LC_DYLD_CHAINED_FIXUPS:      return "LC_DYLD_CHAINED_FIXUPS: used with linkedit_data_command"; break;
        case LC_FILESET_ENTRY:            return "LC_FILESET_ENTRY: used with fileset_entry_command"; break;
    }
    return "LC_?????: Unrecognized load command";
}

std::ostream& operator<<(std::ostream& out, const load_command& lc) {
    _put_hex(out, "\tcmd:", lc.cmd, load_command::describe_cmd(lc.cmd));
    _put_dec(out, "\tcmdsize:", lc.cmdsize);
    return out;
}

load_command make_load_command(const char *data) {
    uint32_t cmd = (uint32_t) *data;
    switch (cmd) {
        // case LC_SEGMENT_64:
        default:
            return segment_command_64(data);
        // default:
        //     return load_command(data);
    }
}

segment_command_64::segment_command_64(const char *data) : load_command(data) {
    uint32_t o = 8; // offset into incoming data
    std::memcpy(&(this->segname),  &data[o],  sizeof(decltype(this->segname)));  o += sizeof(decltype(this->segname));
    std::memcpy(&(this->vmaddr),   &data[o],  sizeof(decltype(this->vmaddr)));   o += sizeof(decltype(this->vmaddr));
    std::memcpy(&(this->vmsize),   &data[o],  sizeof(decltype(this->vmsize)));   o += sizeof(decltype(this->vmsize));
    std::memcpy(&(this->fileoff),  &data[o],  sizeof(decltype(this->fileoff)));  o += sizeof(decltype(this->fileoff));
    std::memcpy(&(this->filesize), &data[o],  sizeof(decltype(this->filesize))); o += sizeof(decltype(this->filesize));
    std::memcpy(&(this->maxprot),  &data[o],  sizeof(decltype(this->maxprot)));  o += sizeof(decltype(this->maxprot));
    std::memcpy(&(this->initprot), &data[o],  sizeof(decltype(this->initprot))); o += sizeof(decltype(this->initprot));
    std::memcpy(&(this->nsects),   &data[o],  sizeof(decltype(this->nsects)));   o += sizeof(decltype(this->nsects));
    std::memcpy(&(this->flags),    &data[o],  sizeof(decltype(this->flags)));    o += sizeof(decltype(this->flags));
}

std::ostream& operator<<(std::ostream& out, const segment_command_64& seg) {
    _put_hex(out, "\tcmd:", seg.cmd, load_command::describe_cmd(seg.cmd));
    _put_dec(out, "\tcmdsize:", seg.cmdsize);
    _put_hex(out, "\tsegname:", seg.segname, "segment name");
    _put_hex(out, "\tvmaddr:", seg.vmaddr, "memory address of this segment");
    _put_hex(out, "\tvmsize:", seg.vmsize, "memory size of this segment");
    _put_hex(out, "\tfileoff:", seg.fileoff, "file offset of this segment");
    _put_hex(out, "\tfilesize:", seg.filesize, "amount to map from the file");
    _put_hex(out, "\tmaxprot:", seg.maxprot, "maximum VM protection");
    _put_hex(out, "\tinitprot:", seg.initprot, "initial VM protection");
    _put_hex(out, "\tnsects:", seg.nsects, "number of sections in segment");
    _put_hex(out, "\tflags:", seg.flags, "flags");
    return out;
}


mach_file::mach_file(const char *data) {
    uint32_t offset = 0;

    uint32_t *magic = (uint32_t *)data;
    if (*magic != MH_MAGIC_64) {
        std::cerr << "# Sorry.  I can only parse 64bit Mach-O binaries." << std::endl;
        exit(EXIT_FAILURE);
    };

    // parse header
    header = mach_header_64(data);
    offset += sizeof(header);

    for (auto i=0; i<header.ncmds; i++){
        auto cmd = make_load_command(data + offset);
        //std::cout << typeid(cmd).name() << std::endl;
        commands.push_back(cmd);
        offset += cmd.cmdsize; 
    }
};

std::ostream& operator<<(std::ostream& out, const mach_file& mf) {
    out << "Header:\n" << mf.header << std::endl;
    for (auto cmd: mf.commands) {
        out << "Load Command:\n";
        out << cmd << std::endl;
    }
    return out;
}



