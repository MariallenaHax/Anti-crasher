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

static std::string getCommandMessage(void* packet) {
    uintptr_t payload = reinterpret_cast<uintptr_t>(packet) + 0x30;
    return std::string(reinterpret_cast<const char*>(payload));
}
static std::string getTextPacketMessage(void* packet) {
    uintptr_t payload = reinterpret_cast<uintptr_t>(packet) + 0x30;
    uint32_t variantIndex = *reinterpret_cast<uint32_t*>(payload + 0x90);
    if (variantIndex == 1)
        return std::string(reinterpret_cast<const char*>(payload + 0x78));
    if (variantIndex == 0 || variantIndex == 2) 
        return std::string(reinterpret_cast<const char*>(payload + 0x60));
    return {};
}
static std::string getAuthor(void* packet) {
    uintptr_t payload = reinterpret_cast<uintptr_t>(packet) + 0x30;
    uint32_t variantIndex = *reinterpret_cast<uint32_t*>(payload + 0x90);
    if (variantIndex == 1)
        return std::string(reinterpret_cast<const char*>(payload + 0x60));
    return {};
}
void sendCommand(const std::string command) {
    /*std::shared_ptr<Packet> packet = MinecraftPackets::createPacket(77);
    if (!packet) return;
    auto* payload = reinterpret_cast<std::byte*>(packet.get()) + 0x30;
    auto* dst = reinterpret_cast<uint8_t*>(payload);
    *reinterpret_cast<std::uint8_t*>(payload - 0x30) = (uint32_t)(command.size() + 0x30);
    dst[0] = (int32_t)command.size();
    for (size_t i = 0; i < command.size(); i++)
        dst[i + 2] = (uint8_t)command[i];
    *reinterpret_cast<std::uint8_t*>(payload + 0x18 + command.size() + 2) = 0;
    *reinterpret_cast<bool*>(payload + 0x54 + command.size() + 2) = true;
    Address::getLoopback()->sendToServer(packet.get());*/
};

void kickPlayer(std::string author, std::string type) {
	sendCommand("/kick " + author);

    static bool isSent = false;
    if (!isSent) {
        //AudioUtils::PlayFromMC("random.orb", 0.25f, 1.f);
        //AudioUtils::PlayFromMC("firework.blast", 0.25f, 1.f);
        sendCommand("/tellraw @a { \"rawtext\": [{ \"text\": \"Kicked player " + author + " (type : " + type + ")\"}]");
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
        if (command.find("/me") != std::string::npos || command.find("/mE") != std::string::npos || command.find("/Me") != std::string::npos || command.find("/ME") != std::string::npos)
        {
            return;
        }
        if (command.find("/.playerlist") != std::string::npos)
            {
                auto* level = Address::getLocalPlayer()->getLevel();
                auto players = level->getPlayerList();
                //AudioUtils::PlayFromMC("random.orb", 0.5f, 1.f);
                for (auto& [uuid, player] : *players)
                {
                    std::string platform;
                    switch ((uint8_t)player.buildPlatform)
                    {
                    case 1:
                        platform = "Android";
                        break;
                    case 2:
                        platform = "iOS";
                        break;
                    case 7:
                    case 8:
                        platform = "Windows";
                        break;
                    case 11:
                        platform = "PlayStation";
                        break;
                    case 12:
                        platform = "Switch 1/2";
                        break;
                    case 13:
                        platform = "Xbox";
                        break;
                    case 14:
                        platform = "ChromeOS";
                        break;
                    }
                    ChatUtils::SendMessage("\n\nName: " + charToName(player.name));
                    ChatUtils::SendMessage("\nPlatform: " + platform + "\n");
                }
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
                    sendCommand("/tellraw @a { \"rawtext\": [{ \"text\": \"Detected crasher. " + author + " (type : borion)\"}]");
                    //AudioUtils::PlayFromMC("random.orb", 1.f, 2.f);
                    isboriontryingtocrash = true;
                }
                return;
            }

            else if (message.find("Ens76385:必須侵入QQ群:908840510") != std::string::npos || message.find("LUMINE UTILITY PROXY ON TOP!") != std::string::npos) {
                kickPlayer(author, "external");

                if (isluminespamming) {
                    isluminespamming = false;
                }
                else {
                    sendCommand("/tellraw @a { \"rawtext\": [{ \"text\": \"Detected crasher. " + author + " (type : external)\"}]");
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
                    sendCommand("/tellraw @a { \"rawtext\": [{ \"text\": \"Detected spammer. " + author + " (type : nature)\"}]");
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
                    sendCommand("/tellraw @a { \"rawtext\": [{ \"text\": \"Detected spammer. " + author + " (type : size)\"}]");
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