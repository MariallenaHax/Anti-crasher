#pragma once

namespace Address {
	class Core {
	public:
		static inline ClientInstance* ClientInstance2 = nullptr;
	};

	ClientInstance* getClientInstance() {
		return Core::ClientInstance2;
	}

	LoopbackPacketSender* getLoopback() {
		return getClientInstance()->getLoopbackPacketSender();
	}

	Player* getLocalPlayer() {
		ClientInstance* client = getClientInstance();
		return client ? client->getLocalPlayer() : nullptr;
	}

	MinecraftGame* getMinecraftGame() {
		return getClientInstance()->getMinecraftGame();
	}
}