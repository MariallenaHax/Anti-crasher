#pragma once

// This is an abstract base class for function hooks to inherit.
class FuncHook {
public:
	// This function initializes the hooks.
	virtual bool Initialize();
};

// Hooks Include
#pragma region Hooks Include
//Game

#include "JetBrainsMonoNL-Regular.h"

#include "Game/LoopbackPacketSenderHook.h"

#include "../Vulkan/VulkanHook.h"
// Actor
#include "Actor/ActorHooks.h"

#pragma endregion