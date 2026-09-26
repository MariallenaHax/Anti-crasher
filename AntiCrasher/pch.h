#pragma once

// C++ Includes
#include <Winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "crypt32.lib")
#include <sddl.h>

#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory> // Memory Region stuff
#include <map>
#include <vector>
#include <utility>
#include <chrono>
#include <unordered_map>
#include <filesystem>
#include <playsoundapi.h>
#include <format>
#include <stack>
#include <winrt/Windows.UI.notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include "winrt/windows.applicationmodel.core.h"
#include "winrt/Windows.UI.ViewManagement.h"
#include "winrt/Windows.Foundation.h"
#include <CoreWindow.h>
#include "winrt/Windows.UI.Core.h"
#include "winrt/windows.system.h"
#include "winrt/Windows.system.profile.h"
#include "winrt/Windows.Storage.Streams.h"
// 'winrt::impl:consume_Window5<d>::DispatcherQueue': a function that returns auto cannot be used before it is defined
//#include <winrt/Windows.Foundation.h>
//#include <cassert>
//#include <queue>
//#include <functional>
//#include <thread>
#include "Libs/json.hpp"
#include <urlmon.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "urlmon.lib")


// libs


// DirectX
#include <d2d1.h>
#include <d2d1_2.h>
#include <dwrite.h>
#include <d3d12.h>
#include <d3d11on12.h>
#include <dxgi1_4.h>
#include <d2d1_3.h>
#include <dxgi.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

// MinHook
#include "Libs/minhook/MinHook.h"
#include "Libs/glm/glm/glm.hpp"
#include "Libs/glm/glm/ext/matrix_transform.hpp"

// libhat
#include "Libs/libhat/libhat.hpp"
#include "Libs/libhat/libhat/Access.hpp"
#include "Libs/libhat/libhat/Callable.hpp"
#include "Libs/libhat/libhat/Concepts.hpp"
#include "Libs/libhat/libhat/CompileTime.hpp"
#include "Libs/libhat/libhat/Defines.hpp"
#include "Libs/libhat/libhat/FixedString.hpp"
#include "Libs/libhat/libhat/MemoryProtector.hpp"
#include "Libs/libhat/libhat/Process.hpp"
#include "Libs/libhat/libhat/Result.hpp"
#include "Libs/libhat/libhat/Scanner.hpp"
#include "Libs/libhat/libhat/Signature.hpp"
#include "Libs/libhat/libhat/StringLiteral.hpp"
#include "Libs/libhat/libhat/Traits.hpp"

// Curl
//#include <curl/curl.h>
