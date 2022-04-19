# mo - a simple mach-o dumper

At the moment it only dumps the header and Load Command types.

usage:   mo \<path-to-mach-o-file\>
  
Sample Output (running mo on itself)
```
$ ./mo mo
File: mo
Header:
	magic:       0xFEEDFACF   # MH_MAGIC_64: the 64-bit mach magic number
	cputype:     0x01000007   # cpu specifier
	cpusubtype:  0x00000003   # machine specifier
	filetype:    0x00000002   # MH_EXECUTE: demand paged executable file
	ncmds:               17   # number of load commands
	sizeofcmds:        1496   # the size of all the load commands
	flags:       0x00218085
	             0x00000001   # MH_NOUNDEFS: the object file has no undefined references
	             0x00000004   # MH_DYLDLINK: the object file is input for the dynamic linker and can't be staticly link edited again
	             0x00000080   # MH_TWOLEVEL: the image is using two-level name space bindings
	             0x00008000   # MH_WEAK_DEFINES: the final linked image contains external weak symbols
	             0x00010000   # MH_BINDS_TO_WEAK: the final linked image uses weak symbols
	             0x00200000   # MH_PIE: When this bit is set, the OS will load the main executable at a random address.  Only used in MH_EXECUTE filetypes.
	reserved:    0x00000000   # reserved

Load Command:
	cmd:         0x00000019   # LC_SEGMENT_64: 64-bit segment of this file to be mapped
	cmdsize:             72

Load Command:
	cmd:         0x00000019   # LC_SEGMENT_64: 64-bit segment of this file to be mapped
	cmdsize:            552

Load Command:
	cmd:         0x00000019   # LC_SEGMENT_64: 64-bit segment of this file to be mapped
	cmdsize:            152

Load Command:
	cmd:         0x00000019   # LC_SEGMENT_64: 64-bit segment of this file to be mapped
	cmdsize:            232

Load Command:
	cmd:         0x00000019   # LC_SEGMENT_64: 64-bit segment of this file to be mapped
	cmdsize:             72

Load Command:
	cmd:         0x80000022   # LC_DYLD_INFO_ONLY: compressed dyld information only
	cmdsize:             48

Load Command:
	cmd:         0x00000002   # LC_SYMTAB: link-edit stab symbol table info
	cmdsize:             24

Load Command:
	cmd:         0x0000000B   # LC_DYSYMTAB: dynamic link-edit symbol table info
	cmdsize:             80

Load Command:
	cmd:         0x0000000E   # LC_LOAD_DYLINKER: load a dynamic linker
	cmdsize:             32

Load Command:
	cmd:         0x0000001B   # LC_UUID: the uuid
	cmdsize:             24

Load Command:
	cmd:         0x00000032   # LC_BUILD_VERSION: build for platform min OS version
	cmdsize:             32

Load Command:
	cmd:         0x0000002A   # LC_SOURCE_VERSION: source version used to build binary
	cmdsize:             16

Load Command:
	cmd:         0x80000028   # LC_MAIN: replacement for LC_UNIXTHREAD
	cmdsize:             24

Load Command:
	cmd:         0x0000000C   # LC_LOAD_DYLIB: load a dynamically linked shared library
	cmdsize:             48

Load Command:
	cmd:         0x0000000C   # LC_LOAD_DYLIB: load a dynamically linked shared library
	cmdsize:             56

Load Command:
	cmd:         0x00000026   # LC_FUNCTION_STARTS: compressed table of function start addresses
	cmdsize:             16

Load Command:
	cmd:         0x00000029   # LC_DATA_IN_CODE: table of non-instructions in __text
	cmdsize:             16
```

