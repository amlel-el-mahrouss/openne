<!-- Read Me of NeKernel -->

# OpenNE

![res/openne.png](res/openne.png)

OpenNE is a **modular microkernel and bootloader** written in **C++20**, designed for **performance, portability, and scalability**.  
Inspired by NT Kernel and microkernel architectures, it features **APIC-based multiprocessing, a custom filesystem (OpenNeFS), and EFI-based booting**.  

We welcome contributions and improvements—feel free to submit PRs or issues!

## Screenshots:

#### APIC:

![doc/apic.png](doc/apic.png)

#### Filesystem:

![doc/filesystem.png](doc/filesystem.png)

## Requirements:

- MinGW/GCC for the Kernel, Boot Loader and the LibSCI.
- Netwide Assembler for the x64 assembly files (in case you're compiling for AMD64).

## Installation:

Clone repository:

```
git clone git@github.com:amlel-el-mahrouss/openne.git
cd openne && ./setup_amd64.sh && ./debug_ata.sh
```

## License:

**Licensed under GPL v3** copyright Amlal El Mahrouss 2025, all rights reserved.
