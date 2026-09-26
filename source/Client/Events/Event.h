#pragma once

enum EventType
{
    ActorBaseTick,
    ContainerTick,
    ImGuiRender,
    IntersectsTick,
    Keyboard,
    Layer,
    Mouse,
    MouseScroll,
    PacketSend,
    RenderContext,
    ViewBobbingTick,
    ItemInHandRenderItem
};

class Event
{
public:
    virtual EventType getType() const = 0; // get the Event type.
    bool* cancelled; // Canceled bool to stop from sending.
};

// Include Events
#pragma region Events

#include "Events/ActorEvent.h"
#include "Events/PacketEvent.h"

#pragma endregion

// CallBack Event
template<typename EventT>
void CallBackEvent(EventT* event) // CallBack the event for modules and others etc.
{

}