#pragma once

class SetActorMotionPacket : public Packet {
public:
	int64_t runtimeID;
	Vector3<float> motion;             // this+0x8
};
