#include "macho.h"

#include <iostream>

mach_header_64::mach_header_64(const char *data) {
    std::memcpy(&(this->magic),      &data[0],  sizeof(decltype(this->magic)));
    std::memcpy(&(this->cputype),    &data[4],  sizeof(decltype(this->cputype)));
    std::memcpy(&(this->cpusubtype), &data[8],  sizeof(decltype(this->cpusubtype)));
    std::memcpy(&(this->filetype),   &data[12], sizeof(decltype(this->filetype)));
    std::memcpy(&(this->ncmds),      &data[16], sizeof(decltype(this->ncmds)));
    std::memcpy(&(this->sizeofcmds), &data[20], sizeof(decltype(this->sizeofcmds)));
    std::memcpy(&(this->flags),      &data[24], sizeof(decltype(this->flags)));
    std::memcpy(&(this->reserved),   &data[28], sizeof(decltype(this->reserved)));
}


std::ostream& operator<<(std::ostream& out, const mach_header_64& mh) {
    out << "Mach-O Header:\n";
    out << std::hex << std::showbase;
    out << "\tmagic:       " << mh.magic << '\n';
    out << "\tcputype:     " << mh.cputype << ":" << mh.cpusubtype << '\n';
    out << "\tfiletype:    " << mh.filetype << '\n';
    out << std::dec << "\tncmds:       " << mh.ncmds << '\n';
    out << "\tsizeofcmds:  " << mh.sizeofcmds << '\n';
    out << std::hex << "\tflags:       " << mh.flags << '\n';
    return out;
}

