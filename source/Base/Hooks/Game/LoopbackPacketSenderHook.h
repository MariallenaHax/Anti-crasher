#pragma once

void* onLoopbackPacketSender;
void* onTextPacketDispatcher;
void* onChangeDimensionPacketDispatcher;
void* onDisconnectPacketDispatcher;
void* onLevelEventPacketDispatcher;
void* onSetActorMotionDispatcher;

bool isboriontryingtocrash = false;
bool isluminespamming = false;
bool isnaturespamming = false;
bool istoolongsize = false;

static std::string readString(uintptr_t addr)
{
    auto possiblePtr = *reinterpret_cast<uintptr_t*>(addr);

    if (possiblePtr != 0x000000031)
        return std::string(reinterpret_cast<const char*>(addr));

    return std::string(*reinterpret_cast<const char**>(addr + 0x10));
}

static std::string getCommandMessage(void* packet) {
    uintptr_t payload = reinterpret_cast<uintptr_t>(packet) + 0x30;
    return readString(payload);
}
static std::string getTextPacketMessage(void* packet) {
    uintptr_t payload = reinterpret_cast<uintptr_t>(packet) + 0x30;
    uint32_t variantIndex = *reinterpret_cast<uint32_t*>(payload + 0x90);
        if (variantIndex == 1)
            return readString(payload + 0x78);
        if (variantIndex == 0 || variantIndex == 2) 
            return readString(payload + 0x60);
    return {};
}
static std::string getAuthor(void* packet) {
    uintptr_t payload = reinterpret_cast<uintptr_t>(packet) + 0x30;
    uint32_t variantIndex = *reinterpret_cast<uint32_t*>(payload + 0x90);

        if (variantIndex == 1)
            return readString(payload + 0x60);
    return {};
}
void sendCommand(const std::string command) {
    /*std::shared_ptr<Packet> packet = MinecraftPackets::createPacket(77);
    if (!packet) return;
    auto* payload = reinterpret_cast<std::byte*>(packet.get()) + 0x30;
    if(command.size() >= 24)
    {
        *reinterpret_cast<uint8_t*>(payload + 0x0) = 0x31;
        *reinterpret_cast<uint8_t*>(payload + 0x8) = command.size();
        *reinterpret_cast<const char**>(payload + 0x10) = command.c_str();
    }
    else
    {
        std::memcpy(payload, command.c_str(), command.size() + 1);
    }
    *reinterpret_cast<uint8_t*>(payload + 0x48 + 0x0) = 0;
    *reinterpret_cast<bool*>(payload + 0x54) = true;
    Address::getLoopback()->sendToServer(packet.get());*/
};

void kickPlayer(std::string author, std::string type) {
	sendCommand("/kick " + author);

    static bool isSent = false;
    if (!isSent) {
        //AudioUtils::PlayFromMC("random.orb", 0.25f, 1.f);
        //AudioUtils::PlayFromMC("firework.blast", 0.25f, 1.f);
        sendCommand("/tellraw @a { \"rawtext\": [{ \"text\": \"Kicked player " + author + " (" + type + ")\"}]}");
        isSent = true;
    }
    else {
        isSent = false;
    }
};

/*HOOK_DEFINE_TRAMPOLINE(Loopback) {
    static void Callback(LoopbackPacketSender* _this, Packet* packet) {
        bool cancelled = false;
        PacketEvent event{ _this, packet }; // PacketEvent
        event.cancelled = &cancelled;
        CallBackEvent(&event);
        if (!cancelled) {
            Orig(_this, packet);
        }
}
};*/

HOOK_DEFINE_TRAMPOLINE(CommandP) {
static void Callback(const float* a1, const float* networkIdentifier, const float* netEventCallback, const std::shared_ptr<Packet>& packet) {
        auto command = getCommandMessage(packet.get());
        if (command.find("/me ") == 0 || command.find("/mE ") == 0 || command.find("/Me ") == 0 || command.find("/ME ") == 0 || command.find("me ") == 0 || command.find("mE ") == 0 || command.find("Me ") == 0 || command.find("ME ") == 0)
        {
            return;
        }
        Orig(a1, networkIdentifier, netEventCallback, packet);
    }
};
HOOK_DEFINE_TRAMPOLINE(TextPacket2) {
static void Callback(const float* a1, const float* networkIdentifier, const float* netEventCallback, const std::shared_ptr<Packet>& packet) {
    bool aaa = true;
        auto* pkt = reinterpret_cast<TextPacket*>(packet.get());
        auto message = getTextPacketMessage(packet.get());
        auto author = getAuthor(packet.get());
        const size_t maxAllowed = 1ull << 20; //1mb
        if (pkt->gettype() != TextPacketType::CHAT) {
            
        }
        else if (message.size() > maxAllowed) {
            aaa = false;
        }
        else {
            if (message.find("boron is here for you") != std::string::npos || message.find("§l§k§o§u+§r") != std::string::npos) {
                kickPlayer(author, "borion");

                if (isboriontryingtocrash) {
                    isboriontryingtocrash = false;
                }
                else {
                    sendCommand("/tellraw @a { \"rawtext\": [{ \"text\": \"Detected crasher. " + author + " (borion)\"}]}");
                    //AudioUtils::PlayFromMC("random.orb", 1.f, 2.f);
                    isboriontryingtocrash = true;
                }
                return;
            }

            else if (message.find("Ens76385:必須侵入QQ群:908840510") != std::string::npos || message.find("LUMINE PROXY ON TOP") != std::string::npos) {
                kickPlayer(author, "external");

                if (isluminespamming) {
                    isluminespamming = false;
                }
                else {
                    sendCommand("/tellraw @a { \"rawtext\": [{ \"text\": \"Detected crasher. " + author + " (external)\"}]}");
                    //AudioUtils::PlayFromMC("random.orb", 1.f, 2.f);
                    isluminespamming = true;
                }
                return;
            }

            else if (message.find("Dont use skid ugly client!") != std::string::npos || message.find("How about your ugly shit client?") != std::string::npos || message.find("Please dont use solstice dumbs shit skid") != std::string::npos) {
                kickPlayer(author, "nature");

                if (isnaturespamming) {
                    isnaturespamming = false;
                }
                else {
                    sendCommand("/tellraw @a { \"rawtext\": [{ \"text\": \"Detected spammer. " + author + " (nature)\"}]}");
                    //AudioUtils::PlayFromMC("random.orb", 1.f, 2.f);
                    isnaturespamming = true;
                }
                return;
            }
            else if (message.size() > 500.f) {
                if (istoolongsize) {
                    istoolongsize = true;
                }
                else {
                    sendCommand("/tellraw @a { \"rawtext\": [{ \"text\": \"Detected spammer. " + author + " (size)\"}]}");
                    istoolongsize = false;
                }
                return;
            }
        }
    if(aaa)
    Orig(a1, networkIdentifier, netEventCallback, packet);
}
};
class LoopbackPacketSenderHook : public FuncHook {
public:
bool Initialize() override
    {   
        
        //auto LoopbackVTable = *(uintptr_t**)Address::getClientInstance()->getLoopbackPacketSender();

        std::shared_ptr<Packet> textPacket = MinecraftPackets::createPacket((int)PacketID::Text);

        std::shared_ptr<Packet> commandRequestPacket = MinecraftPackets::createPacket((int)PacketID::CommandRequest);

        //Loopback::InstallAtPtr(LoopbackVTable[5]);

        TextPacket2::InstallAtPtr((uintptr_t)textPacket->packetHandlerDispatcher->vTable[2]);

        CommandP::InstallAtPtr((uintptr_t)commandRequestPacket->packetHandlerDispatcher->vTable[2]);

        return true;
    }

    static LoopbackPacketSenderHook& Instance() { // a class setup function called Instance.
        static LoopbackPacketSenderHook instance;
        return instance;
    }
};
