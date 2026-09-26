bool isRunning = true;
bool Tablist = false;
#pragma region Includes & Macros

// ImGui & Kiero
#include "Libs/kiero/kiero.h"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/imgui_internal.h"
#include "Libs/imgui/impl/imgui_impl_win32.h"
#include "Libs/imgui/impl/imgui_impl_dx11.h"
#include "Libs/imgui/impl/imgui_impl_dx12.h"
#include "Libs/imgui/imfx.h"

HWND window;
HMODULE hModule;

using namespace winrt::Windows::UI::Notifications;
using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Windows::ApplicationModel::Core;

// Module List
class Module;
class ConfigurationManager;
class CommandManager;
std::vector<Module*> modules = std::vector<Module*>();
std::vector<std::string> GUICategories;
std::vector<std::string> categories;

// Maths
#include "Client/Utils/Math/Keys.h"
#include "Client/Utils/Math/Math.h"
#include "Client/Utils/Math/TextFormat.h"
#include "Client/Utils/Math/TextHolder.h"
#include "Client/Utils/Math/Vector2.h"
#include "Client/Utils/Math/Vector3.h"
#include "Client/Utils/Math/Vector4.h"

// Memory
#include "Libs/xorstr.h"
#include "Client/Utils/Utils.h"
#include "Base/SDK/Memory.h"

#include "Client/Utils/Math/String.h"

// Utils
#include "Client/Utils/TimeUtils.h"

// SDK
//#include "Base/SDK/Classes/Core/ClientInstance.h"
#include "Base/SDK/Classes/Packet/LoopbackPacketSender.h"
#include "Base/SDK/Classes/Packet/Packet.h"
//#include "Base/SDK/Classes/Block/Block.h"
//#include "Base/SDK/Classes/Block/BlockSource.h"
//#include "Base/SDK/Classes/Block/BlockActor.h"
//#include "Base/SDK/Classes/Actor/ActorCollision.h"
#include "Base/SDK/Classes/Actor/Actor.h"
#include "Base/SDK/Classes/Actor/Level.h"
#include "Base/SDK/Classes/GameMode/GameMode.h"
#include "Base/SDK/Classes/Actor/Mob.h"
#include "Base/SDK/Classes/Actor/Player.h"
#include "Base/SDK/Classes/Core/ClientInstance.h"

/*
// SDK
#include "Base/SDK/Classes/Render/HashedString.h"
#include "Base/SDK/Classes/Packet/LoopbackPacketSender.h"
#include "Base/SDK/Classes/Packet/Packet.h"
#include "Base/SDK/Classes/Container/Item.h"
#include "Base/SDK/Classes/Container/ItemStack.h"
#include "Base/SDK/Classes/Container/Inventory.h"
#include "Base/SDK/Classes/Container/SimpleContainer.h"
#include "Base/SDK/Classes/Container/PlayerInventory.h"
#include "Base/SDK/Classes/Container/ContainerScreenController.h"
#include "Base/SDK/Classes/Container/ContainerManagerModel.h"
#include "Base/SDK/Classes/Actor/Actor.h"
#include "Base/SDK/Classes/Actor/Level.h"
#include "Base/SDK/Classes/GameMode/GameMode.h"
#include "Base/SDK/Classes/Actor/Mob.h"
#include "Base/SDK/Classes/Actor/Player.h"
#include "Base/SDK/Classes/Core/ClientInstance.h"
#include "Base/SDK/Classes/Render/ScreenView.h"
#include "Base/SDK/Classes/Render/MatrixStack.h"
#include "Base/SDK/Classes/Render/Camera.h"
*/

#include "Data/ClientData.h"
#include "Base/SDK/Address.h"

#include "Client/Utils/AudioUtils.h"
#include "Client/Utils/RenderUtils.h"

// Event
#include "Client/Events/Event.h"

#include "Client/Utils/ChatUtils.h"


template <typename TRet>
TRet* getModule() {
    for (auto pMod : modules) {
        if (auto pRet = dynamic_cast<typename std::remove_pointer<TRet>::type*>(pMod->getModule())) {

            return pRet;
        }
    }
    return nullptr;
};

// Hooks
#include "Base/Hooks/FuncHook.h"

#pragma endregion

void InitializeClient(LPVOID lpParam) {
    {
        hModule = reinterpret_cast<HMODULE>(lpParam);
    }
    if (auto ptr = (uint8_t*)Memory::findSig("81 BE ? ? ? ? ? ? ? ? 75 ? 48 C7 45"); ptr) {
        // 1.19.40
        ScopedVP(ptr, 10, PAGE_READWRITE);
        ptr[6] = 0;
        ptr[7] = 0;
    }
    else if (auto ptr = (uint8_t*)Memory::findSig("81 BE ?? ?? 00 00 86 80 00 00"); ptr) {
        // 1.20.0.23 preview
        ScopedVP(ptr, 10, PAGE_READWRITE);
        ptr[6] = 0;
        ptr[7] = 0;
    }
    InitializeHooks();
}

/*
DWORD APIENTRY ejectThread(HMODULE lpParam)
{
    while (isRunning) {
        if ((ClientOld::Keymap[VK_CONTROL] && ClientOld::Keymap['L']) || (ClientOld::Keymap[VK_END]) || (Socials::shouldeject)) {
            ChatUtils::sendMessage("Uninjected!");
            AudioUtils::PlayFromMC("beacon.deactivate", 1.f, 1.f);
            isRunning = false;
        }
        Sleep(0);
    }

    Sleep(50);
    kiero::shutdown();
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    UninitializeMods();
#ifdef IDEBUG
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
    FreeLibraryAndExitThread(lpParam, 1);
}
*/

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);

        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)InitializeClient, hModule, 0, nullptr);
        //CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ejectThread, hModule, 0, 0);
    }

    return TRUE;
}

