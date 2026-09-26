#pragma once
#include <glm/glm.hpp>

struct MoveInputState
{
    enum class Flag : int {
        SneakDown = 0,
        SneakToggleDown = 1,
        WantDownSlow = 2,
        WantUpSlow = 3,
        BlockSelectDown = 4,
        AscendBlock = 5,
        DescendBlock = 6,
        JumpDown = 7,
        SprintDown = 8,
        UpLeft = 9,
        UpRight = 10,
        DownLeft = 11,
        DownRight = 12,
        Up = 13,
        Down = 14,
        Left = 15,
        Right = 16,
        Ascend = 17,
        Descend = 18,
        ChangeHeight = 19,
        LookCenter = 20,
        SneakInputCurrentlyDown = 21,
        SneakInputWasReleased = 22,
        SneakInputWasPressed = 23,
        JumpInputWasReleased = 24,
        JumpInputWasPressed = 25,
        JumpInputCurrentlyDown = 26,
        Count = 27,
    };

    std::bitset<27> FlagValues;
    glm::vec2 AnalogMoveVector;
    unsigned char LookSlightDirField;
    unsigned char LookNormalDirField;
    unsigned char LookSmoothDirField;

    bool IsForward()
    {
        return FlagValues.test((size_t)Flag::Up);
    }
    bool IsDown()
    {
        return FlagValues.test((size_t)Flag::Down);
    }
    bool IsLeft()
    {
        return FlagValues.test((size_t)Flag::Left);
    }
    bool IsRight()
    {
        return FlagValues.test((size_t)Flag::Right);
    }
    bool IsJumping()
    {
        return FlagValues.test((size_t)Flag::JumpDown);
    }
};

struct MoveInputComponent {
    enum class Flag : int {
        Sneaking = 0,
        Sprinting = 1,
        WantUp = 2,
        WantDown = 3,
        Jumping = 4,
        AutoJumpingInWater = 5,
        MoveInputStateLocked = 6,
        PersistSneak = 7,
        AutoJumpEnabled = 8,
        IsCameraRelativeMovementEnabled = 9,
        IsRotControlledByMoveDirection = 10,
        Count = 11,
    };

    MoveInputState InputState;
    MoveInputState RawInputState;
    unsigned char HoldAutoJumpInWaterTicks;
    glm::vec2 Move;
    glm::vec2 LookDelta;
    glm::vec2 InteractDir;
    glm::vec3 Displacement;
    glm::vec3 DisplacementDelta;
    glm::vec3 CameraOrientation;
    std::bitset<8> FlagValues;
    std::array<bool, 2> IsPaddling;

    bool isPressed() {
        return InputState.IsForward() ||
            InputState.IsDown() ||
            InputState.IsLeft() ||
            InputState.IsRight();
	};
};