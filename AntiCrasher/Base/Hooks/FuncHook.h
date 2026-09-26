#pragma once

// This is an abstract base class for function hooks to inherit.
class FuncHook {
public:
	// This function initializes the hooks.
	virtual bool Initialize();
};

// Hooks Include
#pragma region Hooks Include

#include "Base/DirectX/DirectXHook.h"
// Actor
#include "Actor/ActorHooks.h"

#include "Game/LoopbackPacketSenderHook.h"

#pragma endregion

// This function initializes all registered function hooks
void InitializeHooks() {
	static FuncHook* Hooks[] = {
		&ActorHooks::Instance(),
		&LoopbackPacketSenderHook::Instance(),
		//&DirectXHook::Instance()
	};
	for (std::size_t i = 0; i < std::size(Hooks); ++i) {
		if (!Hooks[i]->Initialize()) {
			std::cout << "Initializing hook of " + std::to_string(i) + " was failed.\n";
		}
	}
}