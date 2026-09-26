#pragma once

#include "GLMatrix.h"
#include "MinecraftGame.h"
#include "GuiData.h"
#include "LevelRenderer.h"

class GameRenderer {
	char pad[0x388];

public:
	GLMatrix lastViewMatrix;

private:
	char pad2[0x40];

public:
	GLMatrix lastProjectionMatrix;
};

class ClientInstance {
public:
	void playUI(std::string a1, float a2, float a3) {
		Memory::CallVFunc<0xBB, void, std::string, float, float>(this, a1, a2, a3);
	}
	Player* getLocalPlayer() {
		return Memory::CallVFunc<32, Player*>(this); 
	}

	Vector2<float> getFov() {
		return getLevelRender()->getFov();
	}

	GLMatrix* getGLMatrix() {
		return &getGameRenderer2()->lastViewMatrix;
	}

	inline bool WorldToScreen(Vector3<float> pos, Vector2<float>& screen)
	{ // pos = pos 2 w2s, screen = output screen coords
		if (!getGuiData()) {
			return false;
		}
		Vector2<float> displaySize = getGuiData()->getmcResolution();
		class LevelRender* lr = getLevelRender();
		Vector3<float> origin = lr->getOrigin();
		Vector2<float> fov = getFov();

		pos.x -= origin.x;
		pos.y -= origin.y;
		pos.z -= origin.z;

		auto glmatrix = getGLMatrix();
		std::shared_ptr<GLMatrix> matrix = std::shared_ptr<GLMatrix>(glmatrix->correct());

		float x = matrix->transformx(pos);
		float y = matrix->transformy(pos);
		float z = matrix->transformz(pos);

		if (z > 0) return false;
		
		float mX = (float)displaySize.x / 2.0F;
		float mY = (float)displaySize.y / 2.0F;

		screen.x = mX + (mX * x / -z * fov.x);
		screen.y = mY - (mY * y / -z * fov.y);

		return true;
	}

	class LevelRender* getLevelRender()
	{
		return Memory::CallVFunc<191, LevelRender*>(this);
	}
public:
	BUILD_ACCESS(GameRenderer*, GameRenderer2, 0x1418);
	BUILD_ACCESS(class LoopbackPacketSender*, LoopbackPacketSender, 0x1A8); // 0xF8
	BUILD_ACCESS(class MinecraftGame*, MinecraftGame, 0x178); // 0xD0
	BUILD_ACCESS(class TimerClass*, TimerClass, 0x180); // 0xD8
	BUILD_ACCESS(class GuiData*, GuiData, 0x628)
};