#pragma once

class ActorEvent : public Event
{
public:
    ActorEvent(Actor* _actor, Level* _level, GameMode* _gamemode)
    {
        actor = _actor;
        level = _level;
        gamemode = _gamemode;
    }

    EventType getType() const override { return EventType::ActorBaseTick; }
    Actor* actor;
    Level* level;
    GameMode* gamemode;
};