#pragma once

class GuiData {
public:
	void displayClientMessage(const std::string& message) {
		static uintptr_t Address;

		if (!Address) {
			Address = Memory::findSig("ff c3 07 d1 fd 7b 1c a9 fc eb 00 f9 f4 4f 1e a9 fd 03 07 91 48 61");
		}
		using func_t = void (*)(GuiData*, const char*, std::optional<std::string>, bool);
		auto Function = reinterpret_cast<func_t>(Address);
		return Function(this, message.c_str(), {}, false);
	}
	BUILD_ACCESS(Vector2<float>, mcResolution, 0x0040); //ScreenSize()
	BUILD_ACCESS(Vector2<float>, Resolution, 0x0040); //ScreenSizeScaled()
	BUILD_ACCESS(float, Scale, 0x004C); // getScale()


	BUILD_ACCESS(float, screenResRounded, 0x0038);
	BUILD_ACCESS(float, sliderAmount, 0x004C);
	BUILD_ACCESS(float, scalingMultiplier, 0x0050);

	BUILD_ACCESS(Vector2<short>, MousePos, 0x006A); //MousePos
};