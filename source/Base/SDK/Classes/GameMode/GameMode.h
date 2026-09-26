#pragma once

#include <functional>

class GameMode
{
public:
    BUILD_ACCESS(Player*, player, 0x8);
    BUILD_ACCESS(float, lastBreakProgress, 0x20);
    BUILD_ACCESS(float, BreakProgress, 0x24);
private:
    virtual void Destructor(); // 0 // GameMode Destructor
public:
    virtual int64_t startDestroyBlock(Vector3<int> const& pos, unsigned char blockSide, bool& isDestroyedOut); // 1
    virtual int64_t destroyBlock(const Vector3<int>&, unsigned char blockSide); // 2
    virtual int64_t continueDestroyBlock(Vector3<int> const& a1, unsigned char a2, Vector3<float> const& a3, bool& a4); // 3
    virtual int64_t stopDestroyBlock(Vector3<int> const&); // 4
    virtual int64_t startBuildBlock(Vector3<int> const& a1, unsigned char a2, bool auth); // 5
    virtual int64_t buildBlock(Vector3<int> const& a1, unsigned char a2, bool auth); // 6
    virtual int64_t continueBuildBlock(Vector3<int> const& a1, unsigned char a2); // 7
    virtual int64_t stopBuildBlock(); // 8
    virtual int64_t tick(); // 9
    virtual int64_t getPickRange(InputMode const& a1, bool a2); // 10
    virtual int64_t useItem(class ItemStack& a1); // 11
    virtual int64_t useItemAsAtack(class ItemStack& a1); // 12
    virtual int64_t useItemOn(ItemStack& a1, Vector3<int> const& a2, unsigned char a3, Vector3<float> const& a4, class Block const* a5); // 13
    virtual int64_t interact(Actor* a1, Vector3<float> const& a2); // 14
    virtual int64_t attack(Actor&); // 15
    virtual int64_t releaseUsingItem(); // 16
    virtual void setTrialMode(bool a1); // 17
    virtual void isInTrialMode(); // 18
    virtual int64_t registerUpsellScreenCallback(std::function<void> a3); // 19
};