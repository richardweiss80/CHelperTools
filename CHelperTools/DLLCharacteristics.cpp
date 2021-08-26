// For trainings purposes to understand PECOFF specification

#include "pch.h"

int Error(const char* description) {
    printf("[E] ERROR: %s - Code: %d\n", description, ::GetLastError());
    return 1;
}

int _tmain(int argc, const TCHAR* argv[])
{
    if (argc != 2) {
        _tprintf(_TEXT("Usage:\tDLLCharacteristics <filepath>\n"));
        return 0;
    }

    // IMAGE_LIBRARY_PROCESS_INIT            0x0001     // Reserved.
    // IMAGE_LIBRARY_PROCESS_TERM            0x0002     // Reserved.
    // IMAGE_LIBRARY_THREAD_INIT             0x0004     // Reserved.
    // IMAGE_LIBRARY_THREAD_TERM             0x0008     // Reserved.
    // #define IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA    0x0020  // Image can handle a high entropy 64-bit virtual address space.
    // #define IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE 0x0040     // DLL can move.
    // #define IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY    0x0080     // Code Integrity Image
    // #define IMAGE_DLLCHARACTERISTICS_NX_COMPAT    0x0100     // Image is NX compatible
    // #define IMAGE_DLLCHARACTERISTICS_NO_ISOLATION 0x0200     // Image understands isolation and doesn't want it
    // #define IMAGE_DLLCHARACTERISTICS_NO_SEH       0x0400     // Image does not use SEH.  No SE handler may reside in this image
    // #define IMAGE_DLLCHARACTERISTICS_NO_BIND      0x0800     // Do not bind this image.
    // #define IMAGE_DLLCHARACTERISTICS_APPCONTAINER 0x1000     // Image should execute in an AppContainer
    // #define IMAGE_DLLCHARACTERISTICS_WDM_DRIVER   0x2000     // Driver uses WDM model
    // #define IMAGE_DLLCHARACTERISTICS_GUARD_CF     0x4000     // Image supports Control Flow Guard.
    // #define IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE     0x8000

    // More Information on the different values: https://docs.microsoft.com/en-us/windows/win32/debug/pe-format#dll-characteristics
    // DLLCHARACTERISTICS CHANGED TO DLL_CHARACTERISTICS to avoid collision with defined values
    
    typedef struct _IMAGE_DLL_CHARACTERISTICS {
        USHORT IMAGE_LIBRARY_PROCESS_INIT : 1; // Reserved, must be zero
        USHORT IMAGE_LIBRARY_PROCESS_TERM : 1; // Reserved, must be zero
        USHORT IMAGE_LIBRARY_THREAD_INIT : 1; // Reserved, must be zero
        USHORT IMAGE_LIBRARY_THREAD_TERM : 1; // Reserved, must be zero
        USHORT IMAGE_DLL_CHARACTERISTICS_NOTDESCRIBED_OR_NOTUSED : 1; // not in documentation
        USHORT IMAGE_DLL_CHARACTERISTICS_HIGH_ENTROPY_VA : 1; // ASLR with 64 bit address space.
        USHORT IMAGE_DLL_CHARACTERISTICS_DYNAMIC_BASE : 1; // The DLL can be relocated at load time.
        USHORT IMAGE_DLL_CHARACTERISTICS_FORCE_INTEGRITY : 1;
        USHORT IMAGE_DLL_CHARACTERISTICS_NX_COMPAT : 1;
        USHORT IMAGE_DLL_CHARACTERISTICS_NO_ISOLATION : 1;
        USHORT IMAGE_DLL_CHARACTERISTICS_NO_SEH : 1;
        USHORT IMAGE_DLL_CHARACTERISTICS_NO_BIND : 1;
        USHORT IMAGE_DLL_CHARACTERISTICS_APPCONTAINER : 1;
        USHORT IMAGE_DLL_CHARACTERISTICS_WDM_DRIVER : 1;
        USHORT IMAGE_DLL_CHARACTERISTICS_GUARD_CF : 1;
        USHORT IMAGE_DLL_CHARACTERISTICS_TERMINAL_SERVER_AWARE : 1;
    } IMAGE_DLL_CHARACTERISTICS, *PIMAGE_DLL_CHARACTERISTICS;

    HANDLE hFile = ::CreateFile(argv[argc - 1], GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) {
        return Error("Cannot open file");
    }
    
    HANDLE hMapFile = ::CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (!hMapFile) {
        return Error("Cannot create MMF");
    }
    ::CloseHandle(hFile);

    LARGE_INTEGER offset = { 0 };
    LPVOID mappedImage = ::MapViewOfFile(hMapFile, FILE_MAP_READ, offset.HighPart, offset.LowPart, 0);
    if (!mappedImage) {
        return Error("Error in MapViewOfFile occured");
    }

    PIMAGE_DOS_HEADER ptrDosHeader = (PIMAGE_DOS_HEADER)mappedImage;
    PIMAGE_NT_HEADERS ptrNtHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)ptrDosHeader + ptrDosHeader->e_lfanew);
    WORD DllCharacteristics = ptrNtHeader->OptionalHeader.DllCharacteristics; // detailled information on OptionalHeader: https://docs.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-image_optional_header32
        
    // this is one possible way to check, using the predefined values:
    // if (value & IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE) {
    //      // what ever to do, whenn DYNAMIC_BASE flag is set
    // }

    PIMAGE_DLL_CHARACTERISTICS dllCharacteristicsStruct = (PIMAGE_DLL_CHARACTERISTICS) &DllCharacteristics;

    _tprintf(_TEXT("DLLCharacteristics->IMAGE_DLL_CHARACTERISTICS_HIGH_ENTROPY_VA:\t%d\n"), dllCharacteristicsStruct->IMAGE_DLL_CHARACTERISTICS_HIGH_ENTROPY_VA);
    _tprintf(_TEXT("DLLCharacteristics->IMAGE_DLL_CHARACTERISTICS_DYNAMIC_BASE:\t%d\n"), dllCharacteristicsStruct->IMAGE_DLL_CHARACTERISTICS_DYNAMIC_BASE);
    _tprintf(_TEXT("DLLCharacteristics->IMAGE_DLL_CHARACTERISTICS_FORCE_INTEGRITY:\t%d\n"), dllCharacteristicsStruct->IMAGE_DLL_CHARACTERISTICS_FORCE_INTEGRITY);
    _tprintf(_TEXT("DLLCharacteristics->IMAGE_DLL_CHARACTERISTICS_NX_COMPAT:\t%d\n"), dllCharacteristicsStruct->IMAGE_DLL_CHARACTERISTICS_NX_COMPAT);
    _tprintf(_TEXT("DLLCharacteristics->IMAGE_DLL_CHARACTERISTICS_NO_ISOLATION:\t%d\n"), dllCharacteristicsStruct->IMAGE_DLL_CHARACTERISTICS_NO_ISOLATION);

    return 0;

}
