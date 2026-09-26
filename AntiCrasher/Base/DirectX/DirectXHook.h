using PresentFn = HRESULT(STDMETHODCALLTYPE*)(IDXGISwapChain3*,int,int);
PresentFn oPresent = nullptr;

using CommandQueueFn = void(__stdcall*)(ID3D12CommandQueue*, UINT,ID3D12CommandList* const*);
CommandQueueFn oCmd = nullptr;

using ResizeBuffers1Fn = HRESULT(__stdcall*)(
	IDXGISwapChain3*,
	UINT,
	UINT,
	UINT,
	DXGI_FORMAT,
	UINT,
	const UINT*,
	IUnknown* const*
	);
ResizeBuffers1Fn oResize = nullptr;

#include "../../../../Libs/imgui/JetBrainsMonoNL-Regular.h"
typedef HRESULT(__thiscall* drawIndexed)(struct ID3D11DeviceContext*, unsigned int, unsigned int, int);
drawIndexed oDrawIndexed;

bool imguiInit = false;

ID3D11Device* d3d11Device = nullptr;
ID3D12Device* d3d12Device = nullptr;

#include <d3d11on12.h>

struct WrappedBuffer
{
	winrt::com_ptr<ID3D12Resource> native;
	winrt::com_ptr<ID3D11Resource> wrapped;
};
winrt::com_ptr<ID3D11Device> nativeD3D11;
winrt::com_ptr<ID3D11DeviceContext> context;
static winrt::com_ptr<ID3D11Device>        g_d3d11Device;
static winrt::com_ptr<ID3D11DeviceContext> g_d3d11Context;
static winrt::com_ptr<ID3D11On12Device>    g_d3d11on12Device;
winrt::com_ptr<ID3D12CommandQueue> g_d3d12CommandQueue{};

static std::vector<WrappedBuffer> g_wrappedBuffers;

static bool g_imguiInit = false;

struct FrameTransform {
	glm::vec3 mOrigin{};
	glm::vec3 mPlayerPos{};
};

static inline std::queue<FrameTransform> FrameTransforms = {};
static inline int transformDelay = 3;

class ScopedVirtualProtect {
public:
	ScopedVirtualProtect(void* addr, size_t size, DWORD newProtect,
		bool instruction = true)
		: addr(addr), size(size), instruction(instruction) {
		restore = VirtualProtect(addr, size, newProtect, &oldProtect);
	}
	~ScopedVirtualProtect() {
		if (restore) {
			VirtualProtect(addr, size, oldProtect, &oldProtect);
		}
		if (instruction) {
			FlushInstructionCache(GetCurrentProcess(), addr, size);
		}
	}

private:
	void* addr;
	size_t size;
	DWORD oldProtect;
	bool instruction;
	bool restore;
};

#define GLUE1(a, b) a##b
#define GLUE(a, b) GLUE1(a, b)
#define ScopedVP(ptr, ...)                                                     \
  ScopedVirtualProtect GLUE(svp, __LINE__)((void *)(ptr), __VA_ARGS__);

void SetupImGuiStyle() {
	// Comfortable Light Orange style by SouthCraftX from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();
	
	style.Alpha = 1.0f;
	style.DisabledAlpha = 1.0f;
	style.WindowPadding = ImVec2(20.0f, 20.0f);
	style.WindowRounding = 11.5f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(20.0f, 20.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.ChildRounding = 20.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 17.4f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(20.0f, 3.4f);
	style.FrameRounding = 11.9f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.9f, 13.4f);
	style.ItemInnerSpacing = ImVec2(7.1f, 1.8f);
	style.CellPadding = ImVec2(12.1f, 9.2f);
	style.IndentSpacing = 0.0f;
	style.ColumnsMinSpacing = 8.7f;
	style.ScrollbarSize = 11.6f;
	style.ScrollbarRounding = 15.9f;
	style.GrabMinSize = 3.7f;
	style.GrabRounding = 20.0f;
	style.TabRounding = 9.8f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
	
	style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.7254902f, 0.68235296f, 0.54901963f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.92156863f, 0.9137255f, 0.8980392f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.90588236f, 0.8980392f, 0.88235295f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.92156863f, 0.9137255f, 0.8980392f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.84313726f, 0.83137256f, 0.80784315f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92156863f, 0.9137255f, 0.8980392f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.8862745f, 0.8745098f, 0.84705883f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.84313726f, 0.83137256f, 0.80784315f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.84313726f, 0.83137256f, 0.80784315f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.9529412f, 0.94509804f, 0.92941177f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.9529412f, 0.94509804f, 0.92941177f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.92156863f, 0.9137255f, 0.8980392f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.9019608f, 0.89411765f, 0.8784314f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.9529412f, 0.94509804f, 0.92941177f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.88235295f, 0.8666667f, 0.8509804f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.84313726f, 0.83137256f, 0.80784315f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.88235295f, 0.8666667f, 0.8509804f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.96862745f, 0.050980393f, 0.15686275f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.9647059f, 0.8f, 0.02745098f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.96862745f, 0.5882353f, 0.03529412f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.88235295f, 0.8666667f, 0.8509804f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.81960785f, 0.8117647f, 0.8039216f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.84705883f, 0.84705883f, 0.84705883f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.85882354f, 0.8352941f, 0.7921569f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.89411765f, 0.89411765f, 0.89411765f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92156863f, 0.9137255f, 0.8980392f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.87058824f, 0.8509804f, 0.80784315f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.84313726f, 0.8156863f, 0.7490196f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.84313726f, 0.8156863f, 0.7490196f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.85490197f, 0.85490197f, 0.85490197f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.96862745f, 0.050980393f, 0.15686275f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.92156863f, 0.9137255f, 0.8980392f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.88235295f, 0.8666667f, 0.8509804f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.88235295f, 0.8666667f, 0.8509804f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.92156863f, 0.9137255f, 0.8980392f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.8745098f, 0.7254902f, 0.42745098f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.47843137f, 0.4f, 0.29803923f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.9607843f, 0.019607844f, 0.11764706f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90588236f, 0.6627451f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.6392157f, 0.39607844f, 0.043137256f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.9529412f, 0.94509804f, 0.92941177f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.9529412f, 0.94509804f, 0.92941177f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.88235295f, 0.8666667f, 0.8509804f, 1.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.9019608f, 0.89411765f, 0.8784314f, 1.0f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0627451f, 0.0627451f, 0.0627451f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.5019608f, 0.4862745f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.73333335f, 0.70980394f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.5019608f, 0.4862745f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8039216f, 0.8235294f, 0.45490196f, 0.502f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8039216f, 0.8235294f, 0.45490196f, 0.502f);
	style.Alpha = 0.75f;
}
bool aaa2 = true;
ImDrawData* ImguiCalc() {
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = 1 / 60.0f;
	ImGui::NewFrame();
	static bool aaa = false;
	if (Address::getClientInstance() && (uintptr_t)Address::getLocalPlayer())
	{
		if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Tab))
		{
			auto jds = ImVec2(420, 10);
			auto sda = ImVec2(400 + ImGui::GetContentRegionAvail().x, 700);
			ImGui::SetNextWindowPos(jds, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(sda, ImGuiCond_FirstUseEver);
			ImGui::Begin("Tablist");
			if (!aaa)
			{
				ImVec2 winSize = ImGui::GetWindowSize();
				float baseWidth = 300.0f;
				float scale = winSize.x / baseWidth;
				if (scale < 0.1f) scale = 0.1f;
				if (scale > 1.35f) scale = 1.35f;
				ImGui::SetWindowFontScale(scale);
				aaa = true;
			}
			auto* level = Address::getLocalPlayer()->getLevel();
			auto players = level->getPlayerList();
			for (auto& [uuid, player] : *players)
			{
				std::string platform;
				switch ((uint8_t)player.buildPlatform)
				{
				case 1:
					platform = "Android";
					break;
				case 2:
					platform = "iOS";
					break;
				case 7:
				case 8:
					platform = "Windows";
					break;
				case 11:
					platform = "PlayStation";
					break;
				case 12:
					platform = "Nintendo Switch";
					break;
				case 13:
					platform = "Xbox";
					break;
				case 14:
					platform = "ChromeOS";
					break;
				}
				ImGui::Text("%s", std::string("Name: " + player.name + "\nPlatform: " + platform + "\n").c_str());
			}
			ImGui::End();

		}
	}
	else
		aaa = false;
	ImGui::Render();
	return ImGui::GetDrawData();
}

static bool InitD3D11On12(IDXGISwapChain3* swapChain)
{
	winrt::com_ptr<ID3D12Device> device12;
	if (FAILED(swapChain->GetDevice(IID_PPV_ARGS(device12.put()))))
	{
		return false;
	}

	ID3D12CommandQueue* commandQueue =g_d3d12CommandQueue.get();

	if (!commandQueue)
		return false;

	IUnknown* queues[] = {commandQueue};

	const UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED;

	HRESULT hr = D3D11On12CreateDevice(device12.get(),flags,nullptr,0,queues,1,0,g_d3d11Device.put(),g_d3d11Context.put(),nullptr);

	if (FAILED(hr))
		return false;

	g_d3d11on12Device = g_d3d11Device.as<ID3D11On12Device>();

	return true;
}

static bool CreateWrappedBuffers(
	IDXGISwapChain3* swapChain)
{
	DXGI_SWAP_CHAIN_DESC desc{};

	if (FAILED(swapChain->GetDesc(&desc)))
		return false;

	g_wrappedBuffers.clear();
	g_wrappedBuffers.resize(desc.BufferCount);

	D3D11_RESOURCE_FLAGS flags{};
	flags.BindFlags = D3D11_BIND_RENDER_TARGET;

	for (UINT i = 0; i < desc.BufferCount; ++i)
	{
		auto& buffer = g_wrappedBuffers[i];
		HRESULT hr = swapChain->GetBuffer(i,IID_PPV_ARGS(buffer.native.put()));

		if (FAILED(hr))
			return false;

		hr = g_d3d11on12Device->CreateWrappedResource(buffer.native.get(),&flags,D3D12_RESOURCE_STATE_RENDER_TARGET,D3D12_RESOURCE_STATE_PRESENT,IID_PPV_ARGS(buffer.wrapped.put()));

		if (FAILED(hr))
			return false;
	}

	return true;
}

static void RenderD3D12(IDXGISwapChain3* swapChain)
{
	if (g_wrappedBuffers.empty())
	{
		if (!CreateWrappedBuffers(swapChain))
			return;
	}

	const UINT index = swapChain->GetCurrentBackBufferIndex();

	if (index >= g_wrappedBuffers.size())
		return;

	auto& buffer = g_wrappedBuffers[index];

	ID3D11Resource* wrapped = buffer.wrapped.get();

	g_d3d11on12Device->AcquireWrappedResources(&wrapped,1);

	winrt::com_ptr<ID3D11RenderTargetView> rtv;

	HRESULT hr = g_d3d11Device->CreateRenderTargetView(wrapped,nullptr,rtv.put());

	if (SUCCEEDED(hr))
	{
		ID3D11RenderTargetView* view = rtv.get();

		g_d3d11Context->OMSetRenderTargets(1,&view,nullptr);

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

		ImguiCalc();

		ImGui::EndFrame();
		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(
			ImGui::GetDrawData()
		);
	}

	g_d3d11on12Device->ReleaseWrappedResources(&wrapped,1);

	g_d3d11Context->Flush();
}

void ImGUIInit(bool isNativeD3D11)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	SetupImGuiStyle();

	ImFontConfig fontConfig{};

	io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
	io.MouseDrawCursor = false;
	io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
	io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

	io.FontDefault = io.Fonts->AddFontFromMemoryCompressedTTF(JetBrainsMonoNL_compressed_data, JetBrainsMonoNL_compressed_size,18.0f);

	io.IniFilename = nullptr;
	if (isNativeD3D11)
	{
		ImGui_ImplDX11_Init(
			nativeD3D11.get(),
			context.get()
		);
	}
	else
	{
		ImGui_ImplDX11_Init(
			g_d3d11Device.get(),
			g_d3d11Context.get()
		);
	}

	ImGui_ImplWin32_Init(window);

	g_imguiInit = true;
}

__forceinline HRESULT D3D12_PresentDetour(IDXGISwapChain3* pSwapChain, int syncInterval, int flags) {

	if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(nativeD3D11.put()))))
	{
		nativeD3D11->GetImmediateContext(context.put());

		winrt::com_ptr<ID3D11Texture2D> backBuffer;

		if (SUCCEEDED(pSwapChain->GetBuffer(0,IID_PPV_ARGS(backBuffer.put()))))
		{
			winrt::com_ptr<ID3D11RenderTargetView> rtv;

			nativeD3D11->CreateRenderTargetView(backBuffer.get(),nullptr,rtv.put());

			if (!g_imguiInit)
			{
				ImGUIInit(true);
			}

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();

			ImGui::NewFrame();

			ImguiCalc();

			ImGui::EndFrame();
			ImGui::Render();

			ID3D11RenderTargetView* view = rtv.get();

			context->OMSetRenderTargets(1,&view,nullptr);

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			context->Flush();
		}
	}
	else
	{
		winrt::com_ptr<ID3D12Device> device12;

		if (SUCCEEDED(
			pSwapChain->GetDevice(
				IID_PPV_ARGS(device12.put())
			)))
		{
			if (!g_d3d11on12Device)
			{
				if (InitD3D11On12(pSwapChain))
				{
					ImGUIInit(false);
				}
			}

			if (g_d3d11on12Device)
				RenderD3D12(pSwapChain);
		}
	}

	return Memory::CallFunc<HRESULT,IDXGISwapChain3*,UINT,UINT>(reinterpret_cast<void*>(oPresent),pSwapChain,syncInterval,flags);
}

HRESULT __stdcall resizeBuffersCallback(IDXGISwapChain3* swapChain,UINT BufferCount,UINT Width,UINT Height,DXGI_FORMAT Format,UINT SwapChainFlags,const UINT* pCreationNodeMask,IUnknown* const* ppPresentQueue)
{
	ImFX::CleanupFX();

	if (g_d3d11Context) {
		if (g_d3d11on12Device) {
			g_wrappedBuffers.resize(0);
		}

		g_d3d11Context->Flush();
	}

	return Memory::CallFunc<HRESULT,IDXGISwapChain3*,UINT,UINT,UINT,DXGI_FORMAT,UINT,const UINT*,IUnknown* const*>(oResize,swapChain,BufferCount,Width,Height,Format,SwapChainFlags,pCreationNodeMask,ppPresentQueue);
}

LONG_PTR wndProcO;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT wndProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(reinterpret_cast<WNDPROC>(wndProcO), hWnd, uMsg, wParam, lParam);
};
void executeCommandListsHook(ID3D12CommandQueue* queue, UINT NumCommandLists,
	ID3D12CommandList* const* ppCommandLists) {

	if (!g_d3d12CommandQueue) {
		g_d3d12CommandQueue.copy_from(queue);
	}

	return Memory::CallFunc<void>(oCmd, queue, NumCommandLists, ppCommandLists);
};
class DirectXHook : public FuncHook {
public:
	bool Initialize() override {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		window = FindWindowA("Bedrock", "Minecraft");
		wndProcO = SetWindowLongPtrA(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&wndProcHook));

		if (kiero::init(kiero::RenderType::Auto) == kiero::Status::Success) {
			auto renderType = kiero::getRenderType();

			if (renderType == kiero::RenderType::D3D12) {

				if (kiero::bind<&ID3D12CommandQueue::ExecuteCommandLists, CommandQueueFn>(&oCmd, executeCommandListsHook) != kiero::Status::Success)
				{
					std::cout << "o1\n";
					return false;
				}

				if (kiero::bind<&IDXGISwapChain3::ResizeBuffers1, ResizeBuffers1Fn>(&oResize, resizeBuffersCallback)!= kiero::Status::Success)
				{
					std::cout << "o2\n";
					return false;
				}
			}
			else {

			}

			if (kiero::bind<&IDXGISwapChain::Present, PresentFn>(&oPresent, D3D12_PresentDetour) != kiero::Status::Success)
			{
				std::cout << "o3\n";
				return false;
			}
		}
		else
		{
			std::cout << "llllll";
			return false;
		}
		return true;
	}

	static DirectXHook& Instance() {
		static DirectXHook instance;
		return instance;
	}
};