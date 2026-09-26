#pragma once

class HitResult
{
	//private:
		//char pad_0x0000[0x1C]; //0x0000
public:
	Vector3<float> Origin;
	Vector3<float> RelativeRayEnd;
	int HitType;
	int BlockFace;
	Vector3<int> IBlockPos;
	Vector3<float> AbsoluteHitPos;

	void* entity; //0x0038 // WeakEntityRef
	bool isLiquid; //0x004C
	char pad_004D[3]; //0x004D
	int32_t liquidFace; //0x0050
	Vector3<int> liquidBlockPos; //0x0054
	Vector3<float> liquidPos; //0x0060
	bool indirectHit; //0x006C
	char pad_006D[3]; //0x006D
}; //Size: 0x0070

enum class BuildPlatform : int {
    Google = 1,
    IOS = 2,
    Osx = 3,
    Amazon = 4,
    GearVRDeprecated = 5,
    Uwp = 7,
    Win32 = 8,
    Dedicated = 9,
    TvOSDeprecated = 10,
    Sony = 11,
    Nx = 12,
    Xbox = 13,
    WindowsPhoneDeprecated = 14,
    Linux = 15,
    Unknown = -1,
};

class PlayerListEntry {
public:
    uint64_t id;
    mcUUID UUID;
	char pad1[0x09];
    char name[0x18]; 
	char XUID[0x18];
	char platformOnlineId[0x17];
    BuildPlatform buildPlatform;
    char pad2[0x22];
    bool isTeacher;
	bool isHost;
	bool isSubClient;
};
class LevelData
{
public:
	BUILD_ACCESS(std::string, Name, 0x2A8);
};
class Level {
public:
	std::vector<Actor*> getRuntimeActorList() {
		std::vector<Actor*> listOut;
		Memory::CallVFunc<316, decltype(&listOut)>(this, &listOut);
		return listOut;
	}

	std::unordered_map<mcUUID, PlayerListEntry>* getPlayerList() {
    	return *reinterpret_cast<std::unordered_map<mcUUID, PlayerListEntry>**>(reinterpret_cast<uintptr_t>(this) + 0x4A0);
	}
	BUILD_ACCESS(LevelData*, Level, 0x80);
private:
	char pad_0x0000[0xB30]; //0x0000
public:
	Vector3<float> Origin; //0x0BD8 
	Vector3<float> RelativeRayEnd;
	int HitType;
	int BlockFace;
	Vector3<int> IBlockPos; //0x0BF8 
	Vector3<float> AbsoluteHitPos;
};