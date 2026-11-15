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

    EFI_STATUS Status;
    UINTN EventIndex;
    EFI_INPUT_KEY Key;

    do {
        Status = SystemTable->BootServices->WaitForEvent(
            1, &SystemTable->ConIn->WaitForKey, &EventIndex);
        if (EFI_ERROR(Status)) {
            Print(L"Waiting for keyboard input failed: 0x%lx\n", Status);
            return Status;
        }

        Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
    } while (Status == EFI_NOT_READY);

    if (EFI_ERROR(Status)) {
        Print(L"Reading keyboard input failed: 0x%lx\n", Status);
        return Status;
    }

    Print(L"\nKey received (scan code: 0x%x, character: 0x%x).\n",
          Key.ScanCode, Key.UnicodeChar);
    Print(L"Returning to the firmware...\n");

    return EFI_SUCCESS;
}