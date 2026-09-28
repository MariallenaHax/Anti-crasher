#pragma once
#include "PacketID.h"


enum class PacketPriority {
	High,
	Immediate,
	Low,
	Medium,
	Count
};

enum class Reliability {
	Reliable,
	ReliableOrdered,
	Unreliable,
	UnreliableSequenced
};

enum class Compressibility {
	Compressible,
	Incompressible
};

class PacketHandlerDispatcherInstance {
public:
	uintptr_t** vTable;
};

class Packet {
public:
	/* Fields */
	PacketPriority packetPriority = PacketPriority::Low; //0x0008
	Reliability peerReliability = Reliability::Reliable; //0x000C
	uint64_t clientSubID; //0x0010
	char pad[8];
	PacketHandlerDispatcherInstance* packetHandlerDispatcher; //0x0020
	Compressibility compressType = Compressibility::Compressible; //0x0028
	char pad_002C[4]; //0x002C

	virtual void packetConstructor(void) {};
	virtual PacketID getId(void) { return (PacketID)0x0; };
	virtual class TextHolder getTypeName(void) { return TextHolder(); };
	virtual void write(class BinaryStream&) {};
	virtual void read(class ReadOnlyBinaryStream&) {};
	virtual void readExtended(class ReadOnlyBinaryStream&) {};
	virtual void disallowBatching(void) {};
};

class MinecraftPackets {
public:
	static std::shared_ptr<Packet> createPacket(int packetId) {
		static uintptr_t Address;

		if (!Address) {
			Address = Memory::findSig("? ? ? ? ? ? ? ? E9 03 00 2A ? ? ? ? ? ? ? ? ? ? ? ? 4C 79 69 78 6B 09 0C 8B 60 01 1F D6");
		}

		auto pFunction = reinterpret_cast<std::shared_ptr<Packet>(__attribute__((fastcall))*)(int)>(Address);
		return pFunction(packetId);
	}
};

#include "NetworkBlockPosition.h"

#include "CommandRequestPacket.h"
#include "DisconnectPacket.h"
#include "InventoryTransactionPacket.h"
#include "LevelEventPacket.h"
#include "MobEquipmentPacket.h"
#include "MovePlayerPacket.h"
#include "NetworkStackLatencyPacket.h"
#include "PlayerActionPacket.h"
#include "PlayerAuthInputPacket.h"
#include "PlaySoundPacket.h"
#include "TextPacket.h"
#include "SetActorMotionPacket.h"
