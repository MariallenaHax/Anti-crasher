#pragma once

void* onLoopbackPacketSender;
void* onTextPacketDispatcher;
void* onCommandRequestPacketDispatcher;

bool isboriontryingtocrash = false;
bool isluminespamming = false;
bool isnaturespamming = false;
bool istoolongsize = false;

bool ContainsIgnoreCase(const std::string& str, const std::string& find) {
    if (find.empty() || str.empty()) return false;

    std::string pattern;
    pattern.reserve(find.size());
    for (char c : find) {
        pattern.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }

    const size_t n = str.size();
    const size_t m = pattern.size();
    if (m > n) return false;

    for (size_t i = 0; i + m <= n; ++i) {
        size_t j = 0;
        for (; j < m; ++j) {
            char c1 = static_cast<char>(
                std::tolower(static_cast<unsigned char>(str[i + j]))
                );
            if (c1 != pattern[j]) {
                break;
            }
        }
        if (j == m) {
            return true;
        }
    }

    return false;
}

void sendCommand(const std::string command) {
    std::shared_ptr<Packet> packet = MinecraftPackets::createPacket(77);
    auto* command_packet = reinterpret_cast<CommandRequestPacket*>(packet.get());
    command_packet->Command = command;

    command_packet->Origin.mType = CommandOriginType::Player;

    command_packet->InternalSource = true;
    Address::getClientInstance()->getLoopbackPacketSender()->sendToServer(command_packet);
}
    
/*void LoopbackPacketSenderDetour(LoopbackPacketSender* _this, Packet* packet) {
    bool cancelled = false;
    PacketEvent event{ _this, packet }; // PacketEvents
        event.cancelled = &cancelled;
        CallBackEvent(&event);

    if (!cancelled) {
        // Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
        Memory::CallFunc<void*, LoopbackPacketSender*, Packet*>( // CallFunc to call the original.
            onLoopbackPacketSender, _this, packet
        );
    }
}*/

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
}

void TextPacketDispatcherDetour(const float* a1, const float* networkIdentifier, const float* netEventCallback, const std::shared_ptr<Packet>& packet) {
    bool aaa = true;
        auto* pkt = reinterpret_cast<TextPacket*>(packet.get());
        uint32_t type = static_cast<uint32_t>(pkt->type);   
        const size_t maxAllowed = 1ull << 20; //1mb
        if (type != static_cast<uint32_t>(TextPacketType::CHAT)) {

        }
        else if (pkt->message.size() > maxAllowed) {
            aaa = false;
        }
        else {
            if (ContainsIgnoreCase(pkt->message, "boron is here for you") || ContainsIgnoreCase(pkt->message, "§l§k§o§u+§r")) {
                kickPlayer(pkt->author, "borion");

                if (isboriontryingtocrash) {
                    isboriontryingtocrash = false;
                }
                else {
                    sendCommand("/tellraw @a { \"rawtext\": [{ \"text\": \"Detected crasher. " + pkt->author + " (type : borion)\"}]");
                    //AudioUtils::PlayFromMC("random.orb", 1.f, 2.f);
                    isboriontryingtocrash = true;
                }
                return;
            }

            else if (ContainsIgnoreCase(pkt->message, "Ens76385:必須侵入QQ群:908840510") || ContainsIgnoreCase(pkt->message, "LUMINE UTILITY PROXY ON TOP!")) {
                kickPlayer(pkt->author, "external");

                if (isluminespamming) {
                    isluminespamming = false;
                }
                else {
                    sendCommand("/tellraw @a { \"rawtext\": [{ \"text\": \"Detected crasher. " + pkt->author + " (type : external)\"}]");
                    //AudioUtils::PlayFromMC("random.orb", 1.f, 2.f);
                    isluminespamming = true;
                }
                return;
            }

            else if (ContainsIgnoreCase(pkt->message, "Dont use skid ugly client!") ||
                ContainsIgnoreCase(pkt->message, "How about your ugly shit client?") ||
                ContainsIgnoreCase(pkt->message, "Please dont use solstice dumbs shit skid")) {
                kickPlayer(pkt->author, "nature");

                if (isnaturespamming) {
                    isnaturespamming = false;
                }
                else {
                    sendCommand("/tellraw @a { \"rawtext\": [{ \"text\": \"Detected spammer. " + pkt->author + " (type : nature)\"}]");
                    //AudioUtils::PlayFromMC("random.orb", 1.f, 2.f);
                    isnaturespamming = true;
                }
                return;
            }

            else if (pkt->message.size() > 500.f) {
                if (istoolongsize) {
                    istoolongsize = true;
                }
                else {
                    sendCommand("/tellraw @a { \"rawtext\": [{ \"text\": \"Detected spammer. " + pkt->author + " (type : size)\"}]");
                    istoolongsize = false;
                }
                return;
            }
            
        }
    if(aaa)
    Memory::CallFunc<void*, const float*, const float*, const float*, const std::shared_ptr<Packet>&>(onTextPacketDispatcher, a1, networkIdentifier, netEventCallback, packet);
}
void CommandRequestPacketDispatcherDetour(const float* a1, const float* networkIdentifier, const float* netEventCallback, const std::shared_ptr<Packet>& packet) {
        auto* pkt = reinterpret_cast<CommandRequestPacket*>(packet.get());
        if (pkt->Command.find("/me ") != std::string::npos || pkt->Command.find("/mE ") != std::string::npos || pkt->Command.find("/Me ") != std::string::npos || pkt->Command.find("/ME ") != std::string::npos)
        {
            return;
        }
    Memory::CallFunc<void*, const float*, const float*, const float*, const std::shared_ptr<Packet>&>(onCommandRequestPacketDispatcher, a1, networkIdentifier, netEventCallback, packet);
}
class LoopbackPacketSenderHook : public FuncHook {
public:
    bool Initialize() override
    {
        std::thread([] {
            while (isRunning)
            {
                if (Address::getClientInstance() &&
                    Address::getClientInstance()->getLoopbackPacketSender())
                {
                    break;
                }

                Sleep(100);
            }

            if (!isRunning)
            {
                return;
            }

           // auto LoopbackVTable = *(uintptr_t**)Address::getClientInstance()->getLoopbackPacketSender();

            std::shared_ptr<Packet> textPacket = MinecraftPackets::createPacket((int)PacketID::Text);

            std::shared_ptr<Packet> commandRequestPacket = MinecraftPackets::createPacket((int)PacketID::CommandRequest);

            //if (!Memory::HookFunction((void*)LoopbackVTable[4], (void*)&LoopbackPacketSenderDetour, &onLoopbackPacketSender, "LoopbackPacketSender")) {};

            if (!Memory::HookFunction((void*)textPacket->packetHandlerDispatcher->vTable[1], (void*)&TextPacketDispatcherDetour, &onTextPacketDispatcher, "TextPacketDispatcher")) {};

            if (!Memory::HookFunction((void*)commandRequestPacket->packetHandlerDispatcher->vTable[1], (void*)&CommandRequestPacketDispatcherDetour, &onCommandRequestPacketDispatcher, "CommandRequestPacketDispatcher")) {};

        }).detach();

        return true;
    }

    static LoopbackPacketSenderHook& Instance() { // a class setup function called Instance.
        static LoopbackPacketSenderHook instance;
        return instance;
    }
};