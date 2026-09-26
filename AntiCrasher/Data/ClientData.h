#pragma once

namespace ClientData {
	static inline std::string Clientname = xorstr_("AntiCrasher");
	static inline std::string Clientversion = xorstr_("0.0.9");
	static inline bool DenyGrabMouseRequest = false;
	static inline std::map<uint64_t, bool> Keymap = {};
	static inline std::string CurrentLayer = "";
	static inline int Perspective = 0;
	static inline int GuardMode = 0;
}