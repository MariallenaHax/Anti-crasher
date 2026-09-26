#pragma once

#include <functional>

class GameMode
{
public:
    BUILD_ACCESS(float, lastBreakProgress, 0x20);
    BUILD_ACCESS(float, BreakProgress, 0x24);
private:
    virtual void Destructor(); // 0 // GameMode Destructor
public:
    virtual __int64 startDestroyBlock(Vector3<int> const& pos, unsigned char blockSide, bool& isDestroyedOut); // 1
    virtual __int64 destroyBlock(const Vector3<int>&, unsigned char blockSide); // 2
    virtual __int64 continueDestroyBlock(Vector3<int> const& a1, unsigned char a2, Vector3<float> const& a3, bool& a4); // 3
    virtual __int64 stopDestroyBlock(Vector3<int> const&); // 4
    virtual __int64 startBuildBlock(Vector3<int> const& a1, unsigned char a2, bool auth); // 5
    virtual __int64 buildBlock(Vector3<int> const& a1, unsigned char a2, bool auth); // 6
    virtual __int64 continueBuildBlock(Vector3<int> const& a1, unsigned char a2); // 7
    virtual __int64 stopBuildBlock(); // 8
    virtual __int64 tick(); // 9
    virtual __int64 getPickRange(InputMode const& a1, bool a2); // 10
    virtual __int64 useItem(class ItemStack& a1); // 11
    virtual __int64 useItemAsAtack(class ItemStack& a1); // 12
    virtual __int64 useItemOn(ItemStack& a1, Vector3<int> const& a2, unsigned char a3, Vector3<float> const& a4, class Block const* a5); // 13
    virtual __int64 interact(Actor* a1, Vector3<float> const& a2); // 14
    virtual __int64 attack(Actor&); // 15
    virtual __int64 releaseUsingItem(); // 16
    virtual void setTrialMode(bool a1); // 17
    virtual void isInTrialMode(); // 18
    virtual __int64 registerUpsellScreenCallback(std::function<void> a3); // 19
};