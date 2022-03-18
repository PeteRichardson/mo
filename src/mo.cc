#include <cstddef>
#include <filesystem>
#include <iostream>
#include <string_view>
#include <vector>
#include <array>

#include "memory_mapped_file.hpp"
#include "macho.h"

using std::cout, std::endl;

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
        
        auto filedata = (uint8_t *)file.data();

        auto header = mach_header_64( filedata );
        cout << &header << endl;
        file.close();
    } 
}
