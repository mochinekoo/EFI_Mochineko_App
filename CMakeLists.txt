cmake_minimum_required(VERSION 3.20)

project(MochinekoEFIApp C)

set(CMAKE_C_STANDARD 11)
set(EFI_INCLUDE /usr/include/efi)
set(EFI_ARCH_INCLUDE /usr/include/efi/x86_64)

set(EFI_LIB_DIR /usr/lib)
set(EFI_CRT ${EFI_LIB_DIR}/crt0-efi-x86_64.o)
set(EFI_LDS ${EFI_LIB_DIR}/elf_x86_64_efi.lds)

add_library(uefi_obj OBJECT main.c)
target_include_directories(uefi_obj PRIVATE ${EFI_INCLUDE} ${EFI_ARCH_INCLUDE})

target_compile_options(uefi_obj PRIVATE
        -fpic
        -ffreestanding
        -fno-stack-protector
        -fshort-wchar
        -mno-red-zone
        -DEFI_FUNCTION_WRAPPER
)

add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/main.so

        COMMAND ld
        -nostdlib
        -znocombreloc
        -T ${EFI_LDS}
        -shared
        -Bsymbolic
        -L ${EFI_LIB_DIR}
        ${EFI_CRT}
        $<TARGET_OBJECTS:uefi_obj>
        -o ${CMAKE_BINARY_DIR}/main.so
        -lefi
        -lgnuefi

        DEPENDS $<TARGET_OBJECTS:uefi_obj>

        COMMAND_EXPAND_LISTS
)

add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/BOOTX64.EFI

        COMMAND objcopy
        -j .text
        -j .sdata
        -j .data
        -j .rodata
        -j .dynamic
        -j .dynsym
        -j .rel
        -j .rela
        -j .reloc
        --target=efi-app-x86_64
        ${CMAKE_BINARY_DIR}/main.so
        ${CMAKE_BINARY_DIR}/BOOTX64.EFI

        DEPENDS ${CMAKE_BINARY_DIR}/main.so
)

add_custom_target(UEFI ALL DEPENDS ${CMAKE_BINARY_DIR}/BOOTX64.EFI)