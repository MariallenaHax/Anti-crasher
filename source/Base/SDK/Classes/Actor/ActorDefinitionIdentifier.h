#pragma once

struct NamespacedKey {
	/* Fields */
	std::string name;
	std::string key;
	/* Virtuals */
	/* Functions */
	std::string asString()
	{
		return name + ":" + key;
	}
};

class ActorDefinitionIdentifier
{
public:
	NamespacedKey namespacedKey; //0x0020
	std::string identifier; //0x0040
	std::string initEvent; //0x0060
}; 