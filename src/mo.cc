#include <cstddef>
#include <filesystem>
#include <iostream>
#include <string_view>
#include <vector>
#include <array>

#include "mach-o/loader.h"
#include "memory_mapped_file.hpp"

using std::cout, std::endl;


void DumpHeader(memory_mapped_file::read_only_mmf file) {
    // auto header = file.data();
    mach_header_64* mh = reinterpret_cast<mach_header_64*>((void*)file.data());
    cout << "Mach-O Header:\n";
    cout << std::hex << std::showbase;
    cout << "\tmagic:       " << mh->magic << endl;
    cout << "\tcputtype:    " << mh->cputype << ":" << mh->cpusubtype << endl;
    cout << "\tfiletype:    " << mh->filetype << endl;
    cout << std::dec << "\tncmds:       " << mh->ncmds << endl;
    cout << "\tsizeofcmds:  " << mh->sizeofcmds << endl;
    cout << std::hex << "\tflags:       " << mh->flags << endl;
}


int main(int argc, char*argv[]) {
    if (argc < 2) {
        auto exe_name = std::filesystem::path(*argv).stem().string();
        std::cerr << "# usage: " << exe_name << " <mach-o file>..." << std::endl;
        exit(EXIT_SUCCESS);
    }
    
    auto args = std::vector<std::string>(argv+1, argv+argc); // +1 skips argv[0]
    for (auto arg: args) {
        memory_mapped_file::read_only_mmf file(arg.c_str());
        if ((! file.is_open()) || (! file.data())) {
            std::cerr << "Error:  something ain't right with " << arg<< endl;
            continue;
        }
        cout << "File: " << arg << '\n';
        DumpHeader(file);
        file.close();
    } 
}
