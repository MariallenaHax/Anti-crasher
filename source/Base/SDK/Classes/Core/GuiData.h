#pragma once

class GuiData {
public:
	void displayClientMessage(const std::string& message) {
		static uintptr_t Address;

		if (!Address) {
			Address = Memory::findSig("FF C3 07 D1 FD 7B 1C A9 FC EB 00 F9 F4 4F 1E A9 FD 03 07 91 ? ? 08 ? E9");
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
