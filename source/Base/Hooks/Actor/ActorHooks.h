#pragma once
HOOK_DEFINE_TRAMPOLINE(Insta) {
	static int64_t Callback(ClientInstance* _this, bool a, bool b) {
	if (Address::getClientInstance() && Address::getClientInstance()->getLoopbackPacketSender())
		{
			if(!aaab)
			{
				aaab = LoopbackPacketSenderHook::Instance().Initialize();
			}
		}
        Address::Core::ClientInstance2 = _this;
        return Orig(_this, a, b);
    }
};
class ActorHooks : public FuncHook {
public:
	bool Initialize() override
	{
		uintptr_t address4 = Memory::findSig("E8 ? ? FC ? ? ? A9 ? ? 00 91 ? ? 02 A9 ? ? 03 A9 ? ? 04 A9 ? ? 05 A9 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 08 ? 40 F9 A8 83 1E F8");
			if(address4)
			{
				Insta::InstallAtPtr(address4);
				return true;
			}
			else
			{
				return false;
			}
	}

	static ActorHooks& Instance() {
		static ActorHooks instance;
		return instance;
	}
};
