#pragma once
HOOK_DEFINE_TRAMPOLINE(Insta) {
	static int64_t Callback(ClientInstance* _this, bool a) {
	if (Address::getClientInstance() && Address::getClientInstance()->getLoopbackPacketSender())
		{
			if(!aaab)
			{
				aaab = LoopbackPacketSenderHook::Instance().Initialize();
			}
		}
        Address::Core::ClientInstance2 = _this;
        return Orig(_this, a);
    }
};
class ActorHooks : public FuncHook {
public:
	bool Initialize() override
	{
		uintptr_t address4 = Memory::findSig("FD 7B BA A9 FC 0B 00 F9 FD 03 00 91 FA 67 02 A9 F8 5F 03 A9 F6 57 04 A9 F4 4F 05 A9 FF C3 0E D1 ? ? ? ? F4 03 01 2A");
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
