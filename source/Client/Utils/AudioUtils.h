#pragma once


class AudioUtils
{
public:
    static void PlayFromMC(std::string Name, float volume, float pitch)
    {
        if (!Address::getMinecraftGame())
        {
            return;
        }

        Address::getClientInstance()->playUI(Name, volume, pitch);
    }
};
