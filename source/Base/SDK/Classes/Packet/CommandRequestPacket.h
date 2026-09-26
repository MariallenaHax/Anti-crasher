#pragma once

enum class CommandOriginType : int8_t {
    Player = 0x0,
    CommandBlock = 0x1,
    MinecartCommandBlock = 0x2,
    DevConsole = 0x3,
    Test = 0x4,
    AutomationPlayer = 0x5,
    ClientAutomation = 0x6,
    DedicatedServer = 0x7,
    Entity = 0x8,
    Virtual = 0x9,
    GameArgument = 0xA,
    EntityServer = 0xB,
    Precompiled = 0xC,
    GameDirectorEntityServer = 0xD,
    Scripting = 0xE,
    ExecuteContext = 0xF,
};


class mcUUID {
public:
    uint64_t mostSig, leastSig;
    bool operator==(const mcUUID& other) const {
        return mostSig == other.mostSig &&
               leastSig == other.leastSig;
    }
};
namespace std {
    template<>
    struct hash<mcUUID> {
        size_t operator()(const mcUUID& uuid) const noexcept {
            size_t h1 = std::hash<uint64_t>{}(uuid.mostSig);
            size_t h2 = std::hash<uint64_t>{}(uuid.leastSig);

            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };
};
class CommandOriginData {
public:
    CommandOriginType mType;
    mcUUID            mUuid;
    char              mRequestId[0x18];
    int64_t           mPlayerId;
};

class CommandRequestPacket : public Packet {

public:
    BUILD_ACCESS(std::string,Command,0x30);
    BUILD_ACCESS(CommandOriginData,Origin,0x48);
    BUILD_ACCESS(bool,InternalSource,0x84);
};