#pragma once

enum class TextPacketType {
	RAW = 0,
	CHAT = 1,
	TRANSLATION = 2,
	POPUP = 3,
	JUKEBOX_POPUP = 4,
	TIP = 5,
	SYSTEM = 6,
	WHISPER = 7,
	ANNOUNCEMENT = 8,
	JSON_WHISPER = 9,
	JSON = 10,
	JSON_ANNOUNCEMENT = 11
};

class TextPacket : public Packet
{
public:
	BUILD_ACCESS(TextPacketType,type,0x88);       // this+0x30
	BUILD_ACCESS(const char*,author,0x90);     // this+0x38
	BUILD_ACCESS(const char*,message,0xA8);    // this+0x58
};