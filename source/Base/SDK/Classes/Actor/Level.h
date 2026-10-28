#pragma once

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
	char platformOnlineId[0x18];
	char what[0x17];
    BuildPlatform buildPlatform;
    char pad2[0x22];
    bool isTeacher;
	bool isHost;
	bool isSubClient;
};
class Level {
public:
	std::unordered_map<mcUUID, PlayerListEntry>* getPlayerList() {
    	return *reinterpret_cast<std::unordered_map<mcUUID, PlayerListEntry>**>(reinterpret_cast<uintptr_t>(this) + 0x4A8);
	}
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
