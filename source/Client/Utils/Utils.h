#pragma once

template <typename Ret, typename Type>
Ret& direct_access(Type* type, size_t offset) {
    return *reinterpret_cast<Ret*>(reinterpret_cast<uintptr_t>(type) + offset);
}

#define BUILD_ACCESS(type, name, offset) \
    type get##name() const { \
        return direct_access<type>(this, offset); \
    } \
    void set##name(type v) { \
        direct_access<type>(this, offset) = v; \
    }
