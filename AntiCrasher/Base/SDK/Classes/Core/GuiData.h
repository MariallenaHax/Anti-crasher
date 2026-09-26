#pragma once

class GuiData {
public:
	void displayClientMessage(const std::string& message) {
		static void* Address;

		if (Address == nullptr) {
			Address = Memory::findSig("55 56 53 48 81 EC ? ? ? ? 48 8D AC 24 ? ? ? ? 48 C7 85 ? ? ? ? ? ? ? ? 44 88 CB");
		}
		using func_t = void (*)(GuiData*, const std::string&, std::optional<std::string>, bool);
		auto Function = reinterpret_cast<func_t>(Address);
		return Function(this, message, {}, false);
	}
	BUILD_ACCESS(Vector2<float>, mcResolution, 0x0040); //ScreenSize()
	BUILD_ACCESS(Vector2<float>, Resolution, 0x0040); //ScreenSizeScaled()
	BUILD_ACCESS(float, Scale, 0x004C); // getScale()


	BUILD_ACCESS(float, screenResRounded, 0x0038);
	BUILD_ACCESS(float, sliderAmount, 0x004C);
	BUILD_ACCESS(float, scalingMultiplier, 0x0050);

	BUILD_ACCESS(Vector2<short>, MousePos, 0x006A); //MousePos
};