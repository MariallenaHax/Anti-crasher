#pragma once
void* aaa;
bool hooked = false;
void bbb(ClientInstance* _this, bool a)
{
	Address::Core::ClientInstance = _this;
	Memory::CallFunc<int64_t*, ClientInstance*>(aaa, _this, a);
}
class ActorHooks : public FuncHook {
public:
	bool Initialize() override
	{
		void* OptionGetViewPerspective = Memory::findSig("55 41 57 41 56 41 55 41 54 56 57 53 48 81 ec f8 04 00 00 48 8d ac 24 80 00 00 00 48 c7 85 70 04 00 00 fe ff ff ff 89");
		if (!Memory::HookFunction(OptionGetViewPerspective, (void*)&bbb, &aaa, "ClientIntance::update")) {};
		return true;
	}

	static ActorHooks& Instance() {
		static ActorHooks instance;
		return instance;
	}
};