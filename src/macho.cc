#include "macho.h"

#include <iomanip>
#include <iostream>

template <typename T>
void _put_dec(std::ostream& out, std::string label, T value) {
    out << std::setw(14) << std::setfill(' ') << std::left << std::nouppercase
    << label
    << std::setw(10) << std::setfill(' ') << std::right << std::dec << std::noshowbase
    << value
    << '\n';}

template <typename T>
void _put_hex(std::ostream& out, std::string label, T value) {
    out << std::setw(14) << std::setfill(' ') << std::left << std::nouppercase
    << label
    << "0x"
    << std::setw(8) << std::setfill('0') << std::right 
    << std::hex << std::noshowbase << std::uppercase
    << value
    << '\n';
}

mach_header_64::mach_header_64(const char *data) {
    uint32_t o = 0; // offset into incoming data
    std::memcpy(&(this->magic),      &data[o],  sizeof(decltype(this->magic)));      o += sizeof(decltype(this->magic));
    std::memcpy(&(this->cputype),    &data[o],  sizeof(decltype(this->cputype)));    o += sizeof(decltype(this->cputype));
    std::memcpy(&(this->cpusubtype), &data[o],  sizeof(decltype(this->cpusubtype))); o += sizeof(decltype(this->cpusubtype));
    std::memcpy(&(this->filetype),   &data[o], sizeof(decltype(this->filetype)));    o += sizeof(decltype(this->filetype));
    std::memcpy(&(this->ncmds),      &data[o], sizeof(decltype(this->ncmds)));       o += sizeof(decltype(this->ncmds));
    std::memcpy(&(this->sizeofcmds), &data[o], sizeof(decltype(this->sizeofcmds)));  o += sizeof(decltype(this->sizeofcmds));
    std::memcpy(&(this->flags),      &data[o], sizeof(decltype(this->flags)));       o += sizeof(decltype(this->flags));
    std::memcpy(&(this->reserved),   &data[o], sizeof(decltype(this->reserved)));
}

std::ostream& operator<<(std::ostream& out, const mach_header_64& mh) {
    out << std::hex << std::showbase;
    _put_hex(out, "\tmagic:", mh.magic);
    _put_hex(out, "\tcputype:", mh.cputype);
    _put_hex(out, "\tcpusubtype:", mh.cpusubtype);
    _put_hex(out, "\tfiletype:", mh.filetype);
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
    out << "Header:\n" << mf.header << '\n';
    for (auto cmd: mf.commands) {
        out << "Load Command:\n"  << cmd << '\n';
    }
    return out;
}



