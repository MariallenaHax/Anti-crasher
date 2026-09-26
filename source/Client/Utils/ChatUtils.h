#pragma once

class ChatUtils
{
public:
	static void SendMessage(std::string message) {
		Address::getClientInstance()->getGuiData()->displayClientMessage(message);
	}
};
