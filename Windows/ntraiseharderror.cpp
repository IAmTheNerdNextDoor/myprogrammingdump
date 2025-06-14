#include <windows.h>
#include <iostream>
#include <string>

typedef NTSTATUS(NTAPI* RtlAdjustPrivilegePtr) (
    ULONG Privilege,
    BOOLEAN Enable,
    BOOLEAN CurrentThread,
    PBOOLEAN Enabled
);

typedef NTSTATUS(NTAPI* NtRaiseHardErrorPtr) (
    NTSTATUS ErrorStatus,
    ULONG NumberOfParameters,
    ULONG UnicodeStringParameterMask,
    PVOID* Parameters,
    ULONG ValidResponseOptions,
    PULONG Response
);

int main() {
    // Load the functions dynamically from ntdll.dll
    HMODULE ntdll = LoadLibraryA("ntdll.dll");
    if (ntdll == NULL) {
        std::cout << "Failed to load ntdll.dll" << std::endl;
        return 1;
    }

    RtlAdjustPrivilegePtr RtlAdjustPrivilege = reinterpret_cast<RtlAdjustPrivilegePtr>(GetProcAddress(ntdll, "RtlAdjustPrivilege"));
    if (RtlAdjustPrivilege == NULL) {
        std::cout << "Failed to retrieve RtlAdjustPrivilege function." << std::endl;
        FreeLibrary(ntdll);
        return 1;
    }

    NtRaiseHardErrorPtr NtRaiseHardError = reinterpret_cast<NtRaiseHardErrorPtr>(GetProcAddress(ntdll, "NtRaiseHardError"));
    if (NtRaiseHardError == NULL) {
        std::cout << "Failed to retrieve NtRaiseHardError function." << std::endl;
        FreeLibrary(ntdll);
        return 1;
    }

    // Prompt the user to enter the NTSTATUS code
    std::cout << "Enter the NTSTATUS code in hexadecimal (e.g., C000007B): ";
    std::string statusCode;
    std::cin >> statusCode;

    // Convert the entered code to NTSTATUS
    NTSTATUS errorStatus;
    try {
        errorStatus = std::stoul(statusCode, nullptr, 16);
    } catch (const std::exception&) {
        std::cout << "Invalid NTSTATUS code. Exiting..." << std::endl;
        FreeLibrary(ntdll);
        return 1;
    }

    // Prompt the user to trigger the bugcheck
    std::cout << "Press a key to trigger a bugcheck." << std::endl;
    std::cin.get();
    std::cin.get();

    // Adjust privilege
    BOOLEAN previousValue;
    NTSTATUS privilegeResult = RtlAdjustPrivilege(19, TRUE, FALSE, &previousValue);
    if (privilegeResult != 0) {
        std::cout << "Failed to adjust privilege. Error: 0x" << std::hex << privilegeResult << std::endl;
        FreeLibrary(ntdll);
        return 1;
    }

    // Trigger bugcheck with custom error status and parameters
    NTSTATUS raiseResult;
    ULONG response;
    raiseResult = NtRaiseHardError(errorStatus, 0, 0, NULL, 6, &response);
    if (raiseResult != 0) {
        std::cout << "Failed to trigger bugcheck. Error 0x" << std::hex << raiseResult << std::endl;
        FreeLibrary(ntdll);
        return 1;
    }

    // Free the loaded library
    FreeLibrary(ntdll);

    return 0;
}

