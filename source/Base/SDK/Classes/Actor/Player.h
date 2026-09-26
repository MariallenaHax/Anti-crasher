#pragma once

class Player : public Mob {
public:

	void displayClientMessage(std::string const& a1)
	{
		Memory::CallVFunc<198, void, std::string, std::optional<std::string>>(this, a1, std::optional<std::string>()); // 200 // 201
	}

	void addLevels(int a1)
	{
		Memory::CallVFunc<220, void, int>(this, a1); // 219
	}
	BUILD_ACCESS(std::string, PlatformId, 0x518);
	BUILD_ACCESS(std::string, Name, 0xBC0);
};