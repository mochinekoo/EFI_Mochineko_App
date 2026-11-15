#include <efi.h>
#include <efilib.h>
#include <stdint.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    Print(L"Mochineko EFI App\n");
    Print(L"================\n\n");
    Print(L"Firmware vendor: %s\n", SystemTable->FirmwareVendor);
    Print(L"Firmware revision: 0x%x\n", SystemTable->FirmwareRevision);
    Print(L"UEFI specification: %u.%u\n",
          SystemTable->Hdr.Revision >> 16,
          SystemTable->Hdr.Revision & 0xffff);
    Print(L"\nPress any key to return to the firmware.\n");

    EFI_STATUS Status = 0;
    UINTN EventIndex = 0;
    EFI_INPUT_KEY Key = {};


    while (1) {
        Status = uefi_call_wrapper(SystemTable->BootServices->WaitForEvent, 3,
                           1, &SystemTable->ConIn->WaitForKey, &EventIndex);
        if (EFI_ERROR(Status)) {
            break;
        }
        Status = uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2,
                           SystemTable->ConIn, &Key);

        Print(L"\nKey received (scan code: 0x%x, character: 0x%x).\n", Key.ScanCode, Key.UnicodeChar);

        if (Key.ScanCode == SCAN_ESC) {
            break;
        }
    }

    return EFI_SUCCESS;
}