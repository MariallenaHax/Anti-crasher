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
        using Fn = TRet(*)(void*, decltype(argList)...);
        return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
    }
uint8_t hex_to_byte(const char* hex) {
    int ret = 0;

    while (*hex == ' ')
        hex++;

    for (int i = 0; i < 2 && hex[i] != '\0'; i++) {
        char c = hex[i];

        if (c == ' ')
            break;

        uint8_t n = 0;
        if (c >= '0' && c <= '9')
            n = c - '0';
        else if (c >= 'a' && c <= 'f')
            n = 10 + c - 'a';
        else if (c >= 'A' && c <= 'F')
            n = 10 + c - 'A';

        ret <<= 4;
        ret |= n & 0xF;
    }

    return ret & 0xFF;
}

uint8_t* find_pattern(uint8_t* start, uint8_t* end, const char* pattern) {
    if (!start || !end)
        return nullptr;

    while (*pattern == ' ')
        pattern++;

    uint8_t* mem_ptr    = start;
    const char* pat_ptr = pattern;

    while ((uint8_t*)mem_ptr < end && *pat_ptr != '\0') {
        if (*pat_ptr == '?') {
            mem_ptr++;

            while (*pat_ptr == '?')
                pat_ptr++;

            while (*pat_ptr == ' ')
                pat_ptr++;

            continue;
        }
        uint8_t cur_byte = hex_to_byte(pat_ptr);

        if (*mem_ptr == cur_byte) {
            mem_ptr++;
            while (*pat_ptr != ' ' && *pat_ptr != '\0')
                pat_ptr++;
        } else {
            start++;
            mem_ptr = start;
            pat_ptr = pattern;
        }
        while (*pat_ptr == ' ')
            pat_ptr++;
    }
    return (*pat_ptr == '\0') ? start : nullptr;
}
std::optional<uintptr_t> SigScanSafe(std::string_view signature) {
    auto& mod = exl::util::GetMainModuleInfo();
    auto begin = mod.m_Total.m_Start;
    auto end = mod.m_Rodata.GetEnd();
    uint8_t* result = find_pattern((uint8_t*)begin, (uint8_t*)end, signature.data());
    return reinterpret_cast<uintptr_t>(result);
}
uintptr_t findSig(std::string_view signature) {
    auto result = SigScanSafe(signature);
    return result.value();
}
}