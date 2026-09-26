#pragma once
#include "../../../../entt/entt/entt.hpp"

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


public:
	// Level Actor::getLevel(void)
	BUILD_ACCESS(class Level*, Level, 0x1D0);
	BUILD_ACCESS(StateVectorComponent*, StateVector, 0x210);
	// AABBShapeComponent Actor::getAABB(void) or StateVector + 8
	BUILD_ACCESS(AABBShapeComponent*, AABBShape, 0x218);
	// MovementInterpolatorComponent Actor::getRotation(void) or StateVector + 16
	BUILD_ACCESS(MovementInterpolatorComponent*, MovementInterpolator, 0x220);
};