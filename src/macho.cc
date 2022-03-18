#include "macho.h"

#include <iostream>

std::ostream& DumpHeader(std::ostream& out, void* filebuf) {
    // auto header = file.data();
    mach_header_64* mh = reinterpret_cast<mach_header_64*>(filebuf);
    out << "Mach-O Header:\n";
    out << std::hex << std::showbase;
    out << "\tmagic:       " << mh->magic << '\n';
    out << "\tcputype:    " << mh->cputype << ":" << mh->cpusubtype << '\n';
    out << "\tfiletype:    " << mh->filetype << '\n';
    out << std::dec << "\tncmds:       " << mh->ncmds << '\n';
    out << "\tsizeofcmds:  " << mh->sizeofcmds << '\n';
    out << std::hex << "\tflags:       " << mh->flags << '\n';
    return out;
}

