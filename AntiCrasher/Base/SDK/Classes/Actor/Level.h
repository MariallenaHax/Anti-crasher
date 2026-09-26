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
	std::string name;
	std::string XUID;
	std::string platformOnlineId;
	BuildPlatform buildPlatform;
	char pad[0x22];
	bool isTeacher;
	bool isHost;
	bool isSubClient;
};
class LevelData
{
public:
	BUILD_ACCESS(std::string, Name, 0x2A8);
};
class Level { // Level VTable
public:
	// Level::getRuntimeActorList
	// Use CallVFunc to call the VTables by there index.
	std::vector<Actor*> getRuntimeActorList() { // Using a CallVFunc for getRuntimeActorList because There is no need for the entire VTable class
		std::vector<Actor*> listOut;
		Memory::CallVFunc<315, decltype(&listOut)>(this, &listOut); // 313
		return listOut;
	}

	// Level::getHitResult
	// Use CallVFunc to call the VTables by there index.
	// <hiroki> but tozic is too idiot, i will use offset :nerd:
	HitResult* getHitResult() {
		return &*hat::member_at<std::shared_ptr<HitResult>>(this, 0x1E8);
		//return (HitResult*)Memory::CallVFunc<321, HitResult*>(this); // Tozic Moment
	}

	std::unordered_map<mcUUID, PlayerListEntry>* getPlayerList() {
		return *reinterpret_cast<std::unordered_map<mcUUID, PlayerListEntry>**>(reinterpret_cast<uintptr_t>(this) + 0x4E0);
	}

	//BUILD_ACCESS(class HitResult, HitResult, 0xB30);
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