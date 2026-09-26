#pragma once

class MinecraftGame
{
public:
	// 0x1A8 1.20.61
	// 0x190 1.20.51
	// 0x108 1.20.0.1
	BUILD_ACCESS(bool, CanUseKeys, 0x1A8); // 1.21.2

	void playUI(std::string a1, float a2, float a3)
	{
		Memory::CallVFunc<151, void, std::string, float, float>(this, a1, a2, a3);
	}
};