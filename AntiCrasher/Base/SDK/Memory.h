#pragma once

namespace Memory {
    template <typename R, typename... Args>
    R CallFunc(void* func, Args... args)
    {
        return ((R(*)(Args...))func)(args...);
         }

    template <unsigned int IIdx, typename TRet, typename... TArgs>
    static inline auto CallVFunc(void* thisptr, TArgs... argList) -> TRet 
    {
        using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
        return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
    }

    template <typename T>
    bool HookFunction(void* pTarget, T pDetour, void* pOriginal, const char* SignatureIdName) {
        static bool initialized = false;
        if (!initialized)
        {
            initialized = true;
            if (MH_Initialize() != MH_OK)
            {
                return false;
            }
        }

        if (MH_CreateHook(pTarget, pDetour, (LPVOID*)pOriginal) != MH_OK)
        {
            return false;
        }

        if (MH_EnableHook(pTarget) != MH_OK)
        {
            return false;
        }

        return true;
    }

    static uintptr_t getXref(uintptr_t ptr, int offset = 1) 
    {
        if (!ptr)
            return 0;

        uintptr_t newAddr = ptr + *reinterpret_cast<int*>(ptr + offset) + (offset + 4);
        return newAddr;
    }

    static void* findSig(std::string_view sig) 
    {
        auto address = hat::parse_signature(sig);
        assert(address.has_value());
        auto result = hat::find_pattern(address.value(), ".text");
        if (!result.has_result()) {
            return NULL;
        }
        else return reinterpret_cast<void*>((void*)result.get());
    }

    static uintptr_t scanSig(const char* sig, int offset)
    {
        auto parsed_sig = hat::parse_signature(sig);
        const auto result = hat::find_pattern(parsed_sig.value(), ".text");
        if (result.has_result())
            return reinterpret_cast<uintptr_t>(offset > 0 ? result.rel(offset) : result.get());
        else
            return 0;
    }


    static void writeBytes(uintptr_t addr, const std::vector<unsigned char>& bytes, int length) {
        DWORD oldProtect;
        VirtualProtect((LPVOID)addr, length, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy((LPVOID)addr, bytes.data(), length);
        VirtualProtect((LPVOID)addr, length, oldProtect, &oldProtect);
    }

    static void writeBytes(uintptr_t address, std::string bytes, int length) {
        DWORD oldProtect;
        VirtualProtect((LPVOID)address, length, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy((LPVOID)address, bytes.c_str(), length);
        VirtualProtect((LPVOID)address, length, oldProtect, &oldProtect);
    }

    static void readBytes(void* address, void* buffer, size_t size) {
        DWORD oldProtect;
        VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(buffer, address, size);
        VirtualProtect(address, size, oldProtect, &oldProtect);
    }

    void patchBytes(void* address, void* src, unsigned int size)
    {
        DWORD OldProtect;
        VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &OldProtect);
        memcpy(address, src, size);
        VirtualProtect(address, size, OldProtect, &OldProtect);
    }

    static void setProtection(uintptr_t addr, size_t size, DWORD protect) {
        DWORD oldProtect;
        VirtualProtect((LPVOID)addr, size, protect, &oldProtect);
    }
}