#pragma once

uintptr_t FindModuleByName(wchar_t* ModuleName)
{
    PPEB Peb = reinterpret_cast<PPEB>(GetPEB());
    if (!Peb) return 0;

    if (PPEB_LDR_DATA pLdr = Peb->Ldr)
    {
        for (PLIST_ENTRY ListEntry = (PLIST_ENTRY)pLdr->InMemoryOrderModuleList.Flink;
            ListEntry != &pLdr->InMemoryOrderModuleList;
            ListEntry = (PLIST_ENTRY)ListEntry->Flink) {

            LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(ListEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

            if (__wcsicmp_insensitive(pEntry->FullDllName.Buffer, ModuleName))
            {
                return uintptr_t(pEntry->DllBase);
            }
        }
    }

    return 0;
}