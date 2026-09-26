#pragma once
class MaterialPtr;
class LevelRendererPlayer
{
public:
	BUILD_ACCESS(Vector3<float>, cameraPos, 0x660); // 0x65C
	BUILD_ACCESS(float, fovX, 0xF58);
	BUILD_ACCESS(float, fovY, 0xF6C);
};

class LevelRender {
public:
	LevelRendererPlayer* getLevelRendererPlayer()
	{
		return *reinterpret_cast<LevelRendererPlayer**>((uintptr_t)(this) + 0x468);
	};

	Vector3<float> getOrigin() {
		return getLevelRendererPlayer()->getcameraPos();
	};

	Vector2<float> getFov() {
		return { getLevelRendererPlayer()->getfovX() , getLevelRendererPlayer()->getfovY() };
	};
};