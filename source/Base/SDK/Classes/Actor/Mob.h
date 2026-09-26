#pragma once

class Mob : public Actor {
public:
	void setSprinting(bool a1)
	{
		Memory::CallVFunc<140, void, bool>(this, a1);
	}
};
