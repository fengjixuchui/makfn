#pragma once
//#define DLL_PROCESS_ATTACH 1
//#define DLL_THREAD_ATTACH 2
//#define DLL_THREAD_DETACH 3
//#define DLL_PROCESS_DETACH 0
//
//typedef unsigned long       DWORD;
//typedef int                 BOOL;
//typedef unsigned char       BYTE;
//typedef unsigned short      WORD;
//typedef float               FLOAT;
//typedef int                 INT;
//typedef unsigned int        UINT;
//typedef unsigned int* PUINT;
//
//typedef unsigned __int64   uintptr_t;
//typedef signed char        int8_t;
//typedef short              int16_t;
//typedef int                int32_t;
//typedef long long          int64_t;
//typedef unsigned char      uint8_t;
//typedef unsigned short     uint16_t;
//typedef unsigned int       uint32_t;
//typedef unsigned long long uint64_t;
//typedef unsigned __int64   size_t;
//
//typedef signed char        int_least8_t;
//typedef short              int_least16_t;
//typedef int                int_least32_t;
//typedef long long          int_least64_t;
//typedef unsigned char      uint_least8_t;
//typedef unsigned short     uint_least16_t;
//typedef unsigned int       uint_least32_t;
//typedef unsigned long long uint_least64_t;
//
//typedef signed char        int_fast8_t;
//typedef int                int_fast16_t;
//typedef int                int_fast32_t;
//typedef long long          int_fast64_t;
//typedef unsigned char      uint_fast8_t;
//typedef unsigned int       uint_fast16_t;
//typedef unsigned int       uint_fast32_t;
//typedef unsigned long long uint_fast64_t;
//
//typedef long long          intmax_t;
//typedef unsigned long long uintmax_t;
//typedef void* PVOID;
//typedef unsigned long ULONG;
//
//typedef char* PCHAR;
//
//#if defined(_WIN64)
//typedef uint64_t ULONG_PTR;
//#elif defined(_WIN32)
//typedef uint32_t ULONG_PTR;
//#endif

//typedef struct _LIST_ENTRY {
//    PVOID Flink;
//    PVOID Blink;
//} LIST_ENTRY, * PLIST_ENTRY;

typedef struct _PEB_LDR_DATA {
    BYTE Reserved1[8];
    PVOID Reserved2[3];
    LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    PVOID Reserved3[2];
    PPEB_LDR_DATA Ldr;
    PVOID ProcessParameters;
    PVOID Reserved4[3];
    PVOID AtlThunkSListPtr;
    PVOID Reserved5;
    ULONG Reserved6;
    PVOID Reserved7;
    ULONG Reserved8;
    ULONG AtlThunkSListPtr32;
    PVOID Reserved9[45];
    BYTE Reserved10[96];
    PVOID PostProcessInitRoutine;
    BYTE Reserved11[128];
    PVOID Reserved12[1];
    ULONG SessionId;
} PEB, * PPEB;

struct UnicodeStringT {
    unsigned short Length;
    unsigned short MaximumLength;
    wchar_t* Buffer;
};

struct LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY inLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY inInitializationOrderLinks;
    const char* DllBase;
    const char* entryPoint;
    union {
        unsigned long sizeOfImage;
        const char* _dummy;
    } misc;
    UnicodeStringT FullDllName;
    UnicodeStringT baseDllName;

    inline const LDR_DATA_TABLE_ENTRY* LoadOrderNext() const noexcept {
        return reinterpret_cast<const LDR_DATA_TABLE_ENTRY*>(inLoadOrderLinks.Flink);
    }
};

//typedef struct _MEMORY_BASIC_INFORMATION {
//    PVOID BaseAddress;
//    PVOID AllocationBase;
//    DWORD AllocationProtect;
//    size_t RegionSize;
//    DWORD State;
//    DWORD Protect;
//    DWORD Type;
//} MEMORY_BASIC_INFORMATION, * PMEMORY_BASIC_INFORMATION;

struct ImageDataDirectory {
    unsigned long virtualAddress;
    unsigned long size;
};

typedef struct _ImageSectionHeader {
    BYTE Name[0x8];
    union {
        DWORD PhysicalAddress;
        DWORD VirtualSize;
    } Misc;
    DWORD VirtualAddress;
    DWORD SizeOfRawData;
    DWORD PointerToRawData;
    DWORD PointerToRelocations;
    DWORD PointerToLinenumbers;
    WORD NumberOfRelocations;
    WORD NumberOfLinenumbers;
    DWORD Characteristics;
} ImageSectionHeader, * PImageSectionHeader;

typedef struct _ImageFileHeader {
    WORD Machine;
    WORD NumberOfSections;
    DWORD TimeDateStamp;
    DWORD PointerToSymbolTable;
    DWORD NumberOfSymbols;
    WORD SizeOfOptionalHeader;
    WORD Characteristics;
} ImageFileHeader, * PImageFileHeader;

typedef struct _ImageOptionalHeader64 {
    WORD                 Magic;
    BYTE                 MajorLinkerVersion;
    BYTE                 MinorLinkerVersion;
    DWORD                SizeOfCode;
    DWORD                SizeOfInitializedData;
    DWORD                SizeOfUninitializedData;
    DWORD                AddressOfEntryPoint;
    DWORD                BaseOfCode;
    uintptr_t            ImageBase;
    DWORD                SectionAlignment;
    DWORD                FileAlignment;
    WORD                 MajorOperatingSystemVersion;
    WORD                 MinorOperatingSystemVersion;
    WORD                 MajorImageVersion;
    WORD                 MinorImageVersion;
    WORD                 MajorSubsystemVersion;
    WORD                 MinorSubsystemVersion;
    DWORD                Win32VersionValue;
    DWORD                SizeOfImage;
    DWORD                SizeOfHeaders;
    DWORD                CheckSum;
    WORD                 Subsystem;
    WORD                 DllCharacteristics;
    uintptr_t            SizeOfStackReserve;
    uintptr_t            SizeOfStackCommit;
    uintptr_t            SizeOfHeapReserve;
    uintptr_t            SizeOfHeapCommit;
    DWORD                LoaderFlags;
    DWORD                NumberOfRvaAndSizes;
    ImageDataDirectory datadirectory[16];
} ImageOptionalHeader64, * PImageOptionalHeader64;

//typedef struct _IMAGE_DOS_HEADER {
//    WORD e_magic;
//    WORD e_cblp;
//    WORD e_cp;
//    WORD e_crlc;
//    WORD e_cparhdr;
//    WORD e_minalloc;
//    WORD e_maxalloc;
//    WORD e_ss;
//    WORD e_sp;
//    WORD e_csum;
//    WORD e_ip;
//    WORD e_cs;
//    WORD e_lfarlc;
//    WORD e_ovno;
//    WORD e_res[4];
//    WORD e_oemid;
//    WORD e_oeminfo;
//    WORD e_res2[10];
//    long e_lfanew;
//} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;

//typedef struct _IMAGE_NT_HEADERS {
//    DWORD Signature;
//    ImageFileHeader FileHeader;
//    ImageOptionalHeader64 OptionalHeader;
//} IMAGE_NT_HEADERS, * PIMAGE_NT_HEADERS;

typedef struct _ImageExportDirectory {
    DWORD characteristics;
    DWORD timeDateStamp;
    WORD majorVersion;
    WORD minorVersion;
    DWORD name;
    DWORD base;
    DWORD numberOfFunctions;
    DWORD numberOfNames;
    DWORD addressOfFunctions;
    DWORD addressOfNames;
    DWORD addressOfNameOrdinals;
} ImageExportDirectory, * PImageExportDirectory;

#define CONTAINING_RECORD(address, type, field) ((type *)( \
                                                  (PCHAR)(address) - \
                                                  (ULONG_PTR)(&((type *)0)->field)))