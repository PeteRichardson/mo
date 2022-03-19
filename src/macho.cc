#include "macho.h"

#include <iomanip>
#include <iostream>

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

std::string const mach_header_64::describe_magic(decltype(mach_header_64::magic) magic) {
    switch (magic) {
        case MH_MAGIC: return "MH_MAGIC: the mach magic number"; break;
        case MH_CIGAM: return "MH_CIGAM: NXSwapInt(MH_MAGIC)"; break;
        case MH_MAGIC_64: return "MH_MAGIC_64: the 64-bit mach magic number"; break;
        case MH_CIGAM_64: return "MH_CIGAM_64: NXSwapInt(MH_MAGIC_64)"; break;
    }
    return "MH_?????: Unrecognized mach header magic number";
}

std::string const mach_header_64::describe_filetype(decltype(mach_header_64::filetype) filetype) {

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
    _put_hex(out, "\tcputype:", mh.cputype);
    _put_hex(out, "\tcpusubtype:", mh.cpusubtype);
    _put_hex(out, "\tfiletype:", mh.filetype, mach_header_64::describe_filetype(mh.filetype));
    _put_dec(out, "\tncmds:", mh.ncmds);
    _put_dec(out, "\tsizeofcmds:", mh.sizeofcmds);
    _put_hex(out, "\tflags:", mh.flags);
    _put_hex(out, "\treserved:", mh.reserved);
    return out;
}

load_command::load_command(const char *data) {
    uint32_t o = 0; // offset into incoming data
    std::memcpy(&(this->cmd),      &data[o],  sizeof(decltype(this->cmd)));      o += sizeof(decltype(this->cmd));
    std::memcpy(&(this->cmdsize),  &data[o],  sizeof(decltype(this->cmdsize)));
};

std::ostream& operator<<(std::ostream& out, const load_command& lc) {
    _put_hex(out, "\tcmd:", lc.cmd);
    _put_dec(out, "\tcmdsize:", lc.cmdsize);
    return out;
}

mach_file::mach_file(const char *data) {
    uint32_t offset = 0;

    // parse header
    header = mach_header_64(data);
    offset += sizeof(mach_header_64);

    for (auto i=0; i<header.ncmds; i++){
        auto cmd = load_command(data + offset);
        commands.push_back(cmd);
        offset += cmd.cmdsize; 
    }
};

std::ostream& operator<<(std::ostream& out, const mach_file& mf) {
    out << "Header:\n" << mf.header << std::endl;
    for (auto cmd: mf.commands) {
        out << "Load Command:\n"  << cmd << std::endl;
    }
    return out;
}



