#pragma once
#include "../../../../Libs/entt/entt/entt.hpp"

#include "../GameMode/GameType.h"
#include "ActorDefinitionIdentifier.h"

#include "../../Components/ActorDataFlagComponent.h"
#include "../../Components/ActorDefinitionIdentifierComponent.h"
#include "../../Components/ActorTypeComponent.h"
#include "../../Components/ActorGameTypeComponent.h"
#include "../../Components/ActorUniqueIDComponent.h"
#include "../../Components/JumpComponent.h"
#include "../../Components/RuntimeIDComponent.h"
#include "../../Components/DepenetrationComponent.h"
#include "../../Components/StateVectorComponent.h"
#include "../../Components/AABBShapeComponent.h"
#include "../../Components/ActorEquipmentComponent.h"
#include "../../Components/ActorHeadRotationComponent.h"
#include "../../Components/ActorRotationComponent.h"
#include "../../Components/BlockMovementSlowdownMultiplierComponent.h"
#include "../../Components/FallDistanceComponent.h"
#include "../../Components/MaxAutoStepComponent.h"
#include "../../Components/MobHurtTimeComponent.h"
#include "../../Components/MobBodyRotationComponent.h"
#include "../../Components/RenderPositionComponent.h"
#include "../../Components/MoveInputComponent.h"
#include "../../Components/MovementInterpolatorComponent.h"
#include "../../Components/FlagComponent.h"

#include "ActorFlags.h"

#include "EntityIdTraits.h"
#include "EntityID.h"
#include "EntityContext.h"

#pragma region Classes & Structs

//class EntityRegistry;
class ActorLocation;
class ActorDamageSource;
class NewInteractionModel;
class UIProfanityContext;
class ActorUniqueID;
class ActorDamageCause;
class ItemStack;
class ActorEvent;
class EquipmentSlot;
class IContainerManager;
class DataLoadHelper;
class ActorLink;
class LevelSoundEvent;
class AnimationComponent;
class RenderParams;
class HandSlot;
class EquipmentTableDefinition;
class Options;
class ActorInteraction;
class FrameUpdateContextBase;
class ItemStackBase;
class MobEffectInstance;
class Attribute;
class AnimationComponentGroupType;
class ItemUseMethod;
class ResolvedTextObject;
class INpcDialogueData;
class IConstBlockSource;
class ChalkboardBlockPlayer;
class BlockPlayer;
class Block;
class Tick;
class ChunkSource;
class LayeredAbilities;
class ChunkPos;
class MovementEventType;
class PlayerMovementSettings;
class Item;
class Container;
class EventPacket;

class ActorInitializationMethod;
class InitializationMethod;
class VariantParameterList;

class ChalkboardBlockActor;
class ResolvedTextObject;
struct TextObjectRoot;
class SubChunkPos;
class ChunkSource;
class LayeredAbilities;

class ContainerContentChangeListener;

// packets
class ChangeDimensionPacket;

#pragma endregion

enum ArmorSlot {
	Helmet = 0,
	Chestplate = 1,
	Leggings = 2,
	Boots = 3
};

// Actor VTable
class Actor { // 1.21.2
public:
	template <typename T>
	T* getComponent() {
		return const_cast<T*>(getEntityContext()->getRegistry().try_get<T>(getEntityContext()->mEntity));
	}

	template <typename T>
	bool hasComponent() {
		return getEntityContext()->getRegistry().all_of<T>(getEntityContext()->mEntity);
	}

	template <typename T>
	void getOrEmplaceComponent() {
		return getEntityContext()->getRegistry().get_or_emplace<T>(getEntityContext()->mEntity);
	}

	MoveInputComponent* getMoveInputHandler() {
		return getComponent<MoveInputComponent>();
	}

	bool isPlayer() {
		return hasComponent<FlagComponent<PlayerComponentFlag>>();
	}

	bool isLocalPlayer() {
		return hasComponent<FlagComponent<LocalPlayerComponentFlag>>();
	}

	bool isAlive()
	{
		return !hasComponent<IsDeadFlagComponent>();
	}

	bool isOnGround() {
		return hasComponent<OnGroundFlagComponent>();
	}

	EntityContext* getEntityContext()
	{
		uintptr_t address = reinterpret_cast<uintptr_t>(this);
		return reinterpret_cast<EntityContext*>((uintptr_t)this + 0x8);
	}


public:
	// Level Actor::getLevel(void)
	BUILD_ACCESS(class Level*, Level, 0x1D8);
	BUILD_ACCESS(StateVectorComponent*, StateVector, 0x218);
	// AABBShapeComponent Actor::getAABB(void) or StateVector + 8
	BUILD_ACCESS(AABBShapeComponent*, AABBShape, 0x220);
	// MovementInterpolatorComponent Actor::getRotation(void) or StateVector + 16
	BUILD_ACCESS(MovementInterpolatorComponent*, MovementInterpolator, 0x228);
};