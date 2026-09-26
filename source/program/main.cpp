bool isRunning = true;
bool aaab = false;
bool Tablist = false;
int arrrrr = 0;
#include "lib.hpp"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <system_error>
#include <unordered_map>
#include <vector>
#include <cmath>

std::string charToName(char* name)
{
    for (int i = 0; i < 0x18; i++) {
    if(name[i] == (char)0x00)
        return std::string(name);
    }
    return std::string(name);
}

#include "imgui/imgui.h"

#include "Client/Utils/Math/Vector2.h"
#include "Client/Utils/Math/Vector3.h"
#include "Client/Utils/Math/Vector4.h"

#include "Client/Utils/Utils.h"
#include "Base/SDK/Memory.h"

#include "Client/Utils/Math/String.h"
#include "Client/Utils/Math/TextHolder.h"

#include "Client/Utils/TimeUtils.h"
#include "Base/SDK/Classes/GameMode/GameType.h"

#include "Base/SDK/Classes/Packet/LoopbackPacketSender.h"
#include "Base/SDK/Classes/Packet/Packet.h"
#include "Base/SDK/Classes/Actor/Actor.h"
#include "Base/SDK/Classes/Actor/Level.h"
#include "Base/SDK/Classes/Actor/Mob.h"
#include "Base/SDK/Classes/Actor/Player.h"
#include "Base/SDK/Classes/Core/ClientInstance.h"
#include "Base/SDK/Classes/GameMode/GameMode.h"


#include "Base/SDK/Address.h"

#include "Client/Utils/AudioUtils.h"

#include "Client/Events/Event.h"

#include "Client/Utils/ChatUtils.h"

#include "Base/Hooks/FuncHook.h"

namespace fs = std::filesystem;

class OreUIConfig {
public:
  void *mUnknown1;
  void *mUnknown2;
  std::function<bool()> mUnknown3;
  std::function<bool()> mUnknown4;
};

class OreUi {
public:
  std::unordered_map<std::string, OreUIConfig> mConfigs;
};

void setOreUiConfigValue(OreUIConfig &config, bool value) {
  config.mUnknown3 = [value]() { return value; };
  config.mUnknown4 = [value]() { return value; };
}

HOOK_DEFINE_TRAMPOLINE(ForceCloseOreUI) {
	static void Callback(OreUi &a1, void *a2, void *a3, void *a4, void *a5, void *a6) {
    for (auto &[name, config] : a1.mConfigs) {
              setOreUiConfigValue(config, false);
        }
    }
};

HOOK_DEFINE_INLINE(ItemEnch) {
	static void Callback(exl::hook::InlineCtx* ctx) {
		ctx->W[2] = 1;
    }
};

HOOK_DEFINE_REPLACE(Command) {
    static bool Callback(int input, int low, int high, int64_t out) {
        return true;
    }
};
bool aaa = false;

extern "C" void exl_main(void* x0, void* x1) {
    exl::hook::Initialize();

    /*uintptr_t address1 = Memory::findSig("FF 43 06 D1 FD 7B 13 A9 FC 6F 14 A9 FA 67 15 A9 F8 5F 16 A9 F6 57 17 A9 F4 4F 18 A9 FD C3 04 91 E2 07");
        if(address1)
            ForceCloseOreUI::InstallAtPtr(address1);          
        
    uintptr_t address2 =  Memory::findSig("FD 7B BA A9 FB 0B 00 F9 FA 67 02 A9 F8 5F 03 A9 F6 57 04 A9 F4 4F 05 A9 FD 03 00 91 F4 03 01 AA F3 03 08 AA");
        if(address2)
            ItemEnch::InstallAtPtr(address2);  

    uintptr_t address3 = Memory::findSig("FF ? 03 D1 FD 7B ? A9 FB 43 00 F9 FA 67 ? A9 F8 5F ? A9 F6 57 ? A9 F4 4f ? A9 FD ? ? 91 68 E1");
		if(address3)
			Command::InstallAtPtr(address3);*/
    
    ActorHooks::Instance().Initialize();
    //VulkanHook::Instance().Initialize();
};

extern "C" NORETURN void exl_exception_entry() {
    EXL_ABORT("DAMN");
}