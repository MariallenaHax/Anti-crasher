#pragma once

class ChatUtils
{
public:
	static void SendClientMessage(std::string message) {
		Address::getClientInstance()->getGuiData()->displayClientMessage(Utils::combine(RED, ClientData::Clientname, reinterpret_cast<const char*>(u8" » "), RESET, message));
	}

	static void SendWarnMessage(std::string message) {
		Address::getClientInstance()->getGuiData()->displayClientMessage(Utils::combine(RED, "[WARNING] ", RESET, message));
	}

	static void SendDefaultMessage(std::string message) {
		Address::getClientInstance()->getGuiData()->displayClientMessage(message);
	}
};
