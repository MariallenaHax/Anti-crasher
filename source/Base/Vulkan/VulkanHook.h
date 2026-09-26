#include <unistd.h>
#include <string>
#include <vector>
#include "helper/InputHelper.h"
#include <chrono>
int Touched = 0;
inline void catch_all() {}

static void *(*Malloc)(size_t) = reinterpret_cast<void* (*)(size_t)>(catch_all);
static void (*Free)(void*) = reinterpret_cast<void (*)(void*)>(catch_all);

void* Allocate(size_t size) {
  return (Malloc)(size);
}

void Deallocate(void *ptr) {
  (Free)(ptr);
}

#include "imgui/backends/imgui_impl_vulkan.h"

#define MAX_FRAMES_IN_FLIGHT 2

VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
VkInstance g_Instance = VK_NULL_HANDLE;
VkDevice g_Device = VK_NULL_HANDLE;
VkQueue g_GraphicsQueue = VK_NULL_HANDLE;
uint32_t g_QueueFamilyIndex = UINT32_MAX;
VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;
VkCommandPool g_CommandPool = VK_NULL_HANDLE;
VkRenderPass g_RenderPass = VK_NULL_HANDLE;
VkSurfaceKHR g_Surface = VK_NULL_HANDLE;
VkSwapchainKHR g_Swapchain = VK_NULL_HANDLE;
VkExtent2D g_SwapChainExtent = {0, 0};
VkFormat g_SwapChainFormat = VK_FORMAT_UNDEFINED;
std::vector<VkImage> g_SwapChainImages;
std::vector<VkImageView> g_SwapChainImageViews;
std::vector<VkFramebuffer> g_Framebuffers;

VkQueue g_QueuePresent = VK_NULL_HANDLE;

struct FrameData
{
    VkCommandBuffer commandBuffer;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
    bool used;
};
std::vector<FrameData> g_FrameData;
uint32_t g_CurrentFrame = 0;

bool g_ImGuiInitialized = false;
bool g_InitInProgress = false;
bool g_InSubmit = false;

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
void ImGuiRenderCallback() {
    static bool aaa = false;
if (Address::getClientInstance() && (uintptr_t)Address::getLocalPlayer() && InputHelper::isHoldPadUp())
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
        if(level)
{
        auto players = level->getPlayerList();
        if(players)
        {
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
			ImGui::Text("Name: %s\nPlatform: %s\n",charToName(player.name).c_str(),platform.c_str());
		    }
        }
    }
    ImGui::End();
    }
    else
    aaa = false;
}


uint32_t findGraphicsQueueFamily()
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

    for (uint32_t i = 0; i < queueFamilyCount; i++)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            return i;
        }
    }
    return UINT32_MAX;
}

bool createCommandPool()
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = g_QueueFamilyIndex;

    if (vkCreateCommandPool(g_Device, &poolInfo, nullptr, &g_CommandPool) != VK_SUCCESS)
    {
        return false;
    }


    return true;
}

VkCommandBuffer createCommandBuffer() {
    if (g_Device == VK_NULL_HANDLE || g_CommandPool == VK_NULL_HANDLE) {
        return VK_NULL_HANDLE;
    }

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = g_CommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    VkResult result = vkAllocateCommandBuffers(g_Device, &allocInfo, &commandBuffer);
    if (result == VK_SUCCESS) {
    } else {
        return VK_NULL_HANDLE;
    }
    return commandBuffer;
}

bool createSyncObjects()
{
    g_FrameData.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = g_CommandPool;
    allocInfo.commandBufferCount = 1;

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkAllocateCommandBuffers(g_Device, &allocInfo, &g_FrameData[i].commandBuffer) != VK_SUCCESS ||
            vkCreateSemaphore(g_Device, &semaphoreInfo, nullptr, &g_FrameData[i].imageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(g_Device, &semaphoreInfo, nullptr, &g_FrameData[i].renderFinishedSemaphore) != VK_SUCCESS ||
            vkCreateFence(g_Device, &fenceInfo, nullptr, &g_FrameData[i].inFlightFence) != VK_SUCCESS)
        {
            return false;
        }
        g_FrameData[i].used = false;
    }

    return true;
}

bool createDescriptorPool()
{
    VkDescriptorPoolSize pool_sizes[] = {
        {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
        {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
    poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    poolInfo.pPoolSizes = pool_sizes;

    if (vkCreateDescriptorPool(g_Device, &poolInfo, nullptr, &g_DescriptorPool) != VK_SUCCESS)
    {
        return false;
    }

    return true;
}

VkRenderPass createRenderPass()
{

    VkAttachmentDescription attachment{};
    attachment.format = g_SwapChainFormat;
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;    // Important: LOAD to preserve Unity rendering
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; //  STORE to preserve Unity rendering
    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // present layout
    attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;   // to present layout

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &attachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    VkRenderPass renderPass;
    VkResult result = vkCreateRenderPass(g_Device, &renderPassInfo, nullptr, &renderPass);
    if (result != VK_SUCCESS)
    {
        return VK_NULL_HANDLE;
    }

    return renderPass;
}

bool createFramebuffers()
{
    g_SwapChainImageViews.resize(g_SwapChainImages.size());
    g_Framebuffers.resize(g_SwapChainImages.size());

    for (size_t i = 0; i < g_SwapChainImages.size(); i++)
    {

        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = g_SwapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = g_SwapChainFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(g_Device, &createInfo, nullptr, &g_SwapChainImageViews[i]) != VK_SUCCESS)
        {
            return false;
        }

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = g_RenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = &g_SwapChainImageViews[i];
        framebufferInfo.width = g_SwapChainExtent.width;
        framebufferInfo.height = g_SwapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(g_Device, &framebufferInfo, nullptr, &g_Framebuffers[i]) != VK_SUCCESS)
        {
            return false;
        }
    }

    return true;
}

void imageLayoutTransition(VkCommandBuffer &commandBuffer,
                           VkImage &image, VkImageLayout oldLayout, VkImageLayout newLayout,
                           bool bATTACHMENT)
{
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    VkPipelineStageFlags srcStage, dstStage;

    if (oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR &&
        newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        srcStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        dstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
    {
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        srcStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dstStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    }
    else
    {

        barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        srcStage = bATTACHMENT ? VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT : VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        dstStage = bATTACHMENT ? VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT : VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        srcStage, dstStage,
        0, 0, nullptr, 0, nullptr, 1, &barrier);
}

bool initializeImGui() {

        if (!createDescriptorPool())
            return false;

        if (!createSyncObjects())
            return false;

        g_RenderPass = createRenderPass();
        if (g_RenderPass == VK_NULL_HANDLE)
            return false;

        IMGUI_CHECKVERSION();

        uintptr_t mallocFn;
        uintptr_t freeFn;

        R_ABORT_UNLESS(nn::ro::LookupSymbol(&mallocFn, "malloc"));
        R_ABORT_UNLESS(nn::ro::LookupSymbol(&freeFn, "free"));

        Malloc = reinterpret_cast<void *(*)(size_t)>(mallocFn);
        Free = reinterpret_cast<void (*)(void*)>(freeFn);

        ImGuiMemAllocFunc allocFunc = [](size_t size, void *user_data) {
            return Allocate(size);
        };

        ImGuiMemFreeFunc freeFunc = [](void *ptr, void *user_data) {
            Deallocate(ptr);
        };

        ImGui::SetAllocatorFunctions(allocFunc, freeFunc, nullptr);

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        SetupImGuiStyle();

        io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
        io.MouseDrawCursor = false;
        io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
        io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
        io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
        io.DeltaTime = 1.0f / 60.0f;
        io.DisplaySize = ImVec2(1280.0f, 720.0f);
        io.IniFilename = nullptr;

        InputHelper::initKBM();

        io.FontDefault = io.Fonts->AddFontFromMemoryCompressedTTF(JetBrainsMonoNL_compressed_data, JetBrainsMonoNL_compressed_size,18.0f);

        unsigned char *pixels;
        int width, height, pixelByteSize;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &pixelByteSize);

        ImGui_ImplVulkan_InitInfo init_info{};
        init_info.Instance = g_Instance;
        init_info.PhysicalDevice = g_PhysicalDevice;
        init_info.Device = g_Device;
        init_info.QueueFamily = g_QueueFamilyIndex;
        init_info.Queue = g_QueuePresent;
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = g_DescriptorPool;
        init_info.MinImageCount = g_SwapChainImages.size();
        init_info.ImageCount = g_SwapChainImages.size();
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = nullptr;


        if (!ImGui_ImplVulkan_Init(&init_info, g_RenderPass))
        {
            return false;
        }

        VkCommandBuffer fontCommandBuffer = createCommandBuffer();
        if (fontCommandBuffer == VK_NULL_HANDLE)
        {
            return false;
        }

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VkResult result = vkBeginCommandBuffer(fontCommandBuffer, &beginInfo);
        if (result != VK_SUCCESS)
        {
            return false;
        }

        if (!ImGui_ImplVulkan_CreateFontsTexture(fontCommandBuffer))
        {
            return false;
        }

        result = vkEndCommandBuffer(fontCommandBuffer);
        if (result != VK_SUCCESS)
        {
            return false;
        }

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

        VkFence fontFence = VK_NULL_HANDLE;

        result = vkCreateFence(g_Device,&fenceInfo,nullptr,&fontFence);

        if (result != VK_SUCCESS)
        {
            return false;
        }

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &fontCommandBuffer;

        result = vkQueueSubmit(g_QueuePresent,1,&submitInfo,fontFence);

        if (result != VK_SUCCESS)
        {
            vkDestroyFence(g_Device, fontFence, nullptr);
            return false;
        }

        result = vkWaitForFences(g_Device,1,&fontFence,VK_TRUE,UINT64_MAX);

        if (result != VK_SUCCESS)
        {
            vkDestroyFence(g_Device, fontFence, nullptr);
            return false;
        }

        vkDestroyFence(g_Device, fontFence, nullptr);

        ImGui_ImplVulkan_DestroyFontUploadObjects();

        vkFreeCommandBuffers(g_Device,g_CommandPool,1,&fontCommandBuffer);

        return true;
}

bool renderImGui(uint32_t imageIndex)
{
        FrameData &currentFrame = g_FrameData[g_CurrentFrame];

        VkResult fenceResult = vkWaitForFences(g_Device,1,&currentFrame.inFlightFence,VK_TRUE,0);

        if (fenceResult == VK_TIMEOUT)
        {
            return false;
        }

        if (fenceResult != VK_SUCCESS)
        {
            return false;
        }

        vkResetCommandBuffer(currentFrame.commandBuffer, 0);
        vkResetFences(g_Device, 1, &currentFrame.inFlightFence);

        // begin command buffer recording
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        if (vkBeginCommandBuffer(currentFrame.commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            return false;
        }

        // imageLayoutTransition : PRESENT_SRC -> COLOR_ATTACHMENT
        imageLayoutTransition(
            currentFrame.commandBuffer,
            g_SwapChainImages[imageIndex],
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            false);

        // BeginRenderPass
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = g_RenderPass;
        renderPassInfo.framebuffer = g_Framebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = g_SwapChainExtent;

        vkCmdBeginRenderPass(currentFrame.commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // RenderDrawData ImGui
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), currentFrame.commandBuffer);

        vkCmdEndRenderPass(currentFrame.commandBuffer);

        // imageLayoutTransition : COLOR_ATTACHMENT -> PRESENT_SRC
        imageLayoutTransition(
            currentFrame.commandBuffer,
            g_SwapChainImages[imageIndex],
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            true);

        // end command buffer recording
        if (vkEndCommandBuffer(currentFrame.commandBuffer) != VK_SUCCESS)
        {
            return false;
        }

        // submit command buffer to queue
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &currentFrame.commandBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &currentFrame.renderFinishedSemaphore;

        if (vkQueueSubmit(g_QueuePresent, 1, &submitInfo, currentFrame.inFlightFence) != VK_SUCCESS)
        {
            return false;
        }

        // update current frame
        currentFrame.used = true;
        g_CurrentFrame = (g_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        return true;
}
  void updateTouch(ImGuiIO &io) {
    ImVec2 pos(0, 0);
    if (!InputHelper::getTouchCoords(&pos.x, &pos.y)) {
      if (InputHelper::isTouchRelease()) {
        io.AddMouseButtonEvent(ImGuiMouseButton_Left, false);
      }
      return;
    }

    io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);

    pos.x = (pos.x / (float)1280) * io.DisplaySize.x;
    pos.y = (pos.y / (float)720) * io.DisplaySize.y;
    io.AddMousePosEvent(pos.x, pos.y);
    io.MouseDrawCursor = true;
    Touched = 80;
    if (InputHelper::isTouchPress()) {
      io.AddMouseButtonEvent(ImGuiMouseButton_Left, true);
    }
    return;
  }

  void updateInput() {

    ImGuiIO &io = ImGui::GetIO();
    if(InputHelper::isHoldL())
    {
        io.AddMouseWheelEvent(0.0f, 0.125f);
    }
    else if (InputHelper::isHoldR())
    {
        io.AddMouseWheelEvent(0.0f, -0.125f);
    }
    if (Touched < 1)
    {
        io.MouseDrawCursor = false;
    }
    else
        Touched--;
    updateTouch(io);
  }
  
  float getDeltaTime()
{
    static bool initialized = false;

    static struct timespec prev;
    struct timespec now;

    if (!initialized) {
        clock_gettime(CLOCK_MONOTONIC, &prev);
        initialized = true;
        return 0.0f;
    }

    clock_gettime(CLOCK_MONOTONIC, &now);
    double delta = (now.tv_sec - prev.tv_sec) + (now.tv_nsec - prev.tv_nsec) / 1e9;
    prev = now;
    return (float)delta;
}
void destroyFramebuffers()
{
    for (auto framebuffer : g_Framebuffers)
    {
        if (framebuffer != VK_NULL_HANDLE)
        {
            vkDestroyFramebuffer(g_Device, framebuffer, nullptr);
        }
    }
    g_Framebuffers.clear();
    for (auto imageView : g_SwapChainImageViews)
    {
        if (imageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(g_Device, imageView, nullptr);
        }
    }
    g_SwapChainImageViews.clear();
}
// ----------------------------- HOOKS -----------------------------
HOOK_DEFINE_TRAMPOLINE(hooked_vkQueuePresentKHR) {
static VkResult Callback(VkQueue queue, const VkPresentInfoKHR *pPresentInfo)
    {
        if (g_Device == VK_NULL_HANDLE)
        {
            return Orig(queue, pPresentInfo);
        }
        if (!g_ImGuiInitialized)
        {
            if (!initializeImGui())
                return Orig(queue, pPresentInfo);
            else
                g_ImGuiInitialized = true;
        }
        if (pPresentInfo && pPresentInfo->swapchainCount > 0)
        {
            uint32_t imageIndex = pPresentInfo->pImageIndices[0];

            ImGui_ImplVulkan_NewFrame();
            ImGuiIO &io = ImGui::GetIO();
            io.DisplaySize = ImVec2((float)g_SwapChainExtent.width,(float)g_SwapChainExtent.height);

            float dt = getDeltaTime();

            io.DeltaTime = (dt > 0.0f) ? dt : (1.0f / 60.0f);

            updateInput();

            InputHelper::updatePadState();

            ImGui::NewFrame();
            ImGuiRenderCallback();
            ImGui::Render();
            renderImGui(imageIndex);
        }

        return Orig(queue, pPresentInfo);
    }
};
HOOK_DEFINE_TRAMPOLINE(hooked_vkQueueSubmit) {
static VkResult Callback(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) {
    g_QueuePresent = queue;
    return Orig(queue, submitCount, pSubmits, fence);
}
};
HOOK_DEFINE_TRAMPOLINE(vkCreateDeviceReplace) {
static VkResult Callback(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {
    VkResult result = Orig(physicalDevice, pCreateInfo, pAllocator, pDevice);

    if (result == VK_SUCCESS)
    {
        g_PhysicalDevice = physicalDevice;
        g_Device = *pDevice;

        g_QueueFamilyIndex = findGraphicsQueueFamily();
        if (g_QueueFamilyIndex != UINT32_MAX)
        {
            vkGetDeviceQueue(g_Device, g_QueueFamilyIndex, 0, &g_GraphicsQueue);
            createCommandPool();
        }
    }

    return result;
}
};

HOOK_DEFINE_TRAMPOLINE(vkCreateDisplayPlaneSurfaceKHRReplace) {
static VkResult Callback(VkInstance instance, const vkCreateDisplayPlaneSurfaceKHRReplace* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
    return Orig(instance, pCreateInfo, pAllocator, pSurface);
}
};

HOOK_DEFINE_TRAMPOLINE(vkCreateSwapchainKHRReplace) {
static VkResult Callback(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) {
    VkResult result = Orig(device, pCreateInfo, pAllocator, pSwapchain);
    if (result == VK_SUCCESS)
    {

        g_Swapchain = *pSwapchain;
        g_SwapChainExtent = pCreateInfo->imageExtent;
        g_Surface = pCreateInfo->surface;
        g_SwapChainFormat = pCreateInfo->imageFormat;

        uint32_t imageCount;
        vkGetSwapchainImagesKHR(device, g_Swapchain, &imageCount, nullptr);
        g_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device, g_Swapchain, &imageCount, g_SwapChainImages.data());
        createFramebuffers();
    }
    return result;
}
};
HOOK_DEFINE_TRAMPOLINE(hooked_vkGetInstanceProcAddr) {
static PFN_vkVoidFunction Callback(VkInstance instance, const char *pName)
{
    if (instance != VK_NULL_HANDLE && g_Instance == VK_NULL_HANDLE)
    {
        g_Instance = instance;
    }
    PFN_vkVoidFunction result = Orig(instance, pName);
    if (strcmp(pName, "vkCreateDevice") == 0)
        {
            static bool installed = false;
            if (!installed)
            {
                vkCreateDeviceReplace::InstallAtPtr(reinterpret_cast<uintptr_t>(result));
                installed = true;
            }
        }
        return result;
    }
};
HOOK_DEFINE_TRAMPOLINE(vkDestroySwapchainKHRReplace) 
{
    static void Callback(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
    {
        if (swapchain == g_Swapchain && g_Device != VK_NULL_HANDLE)
        {
            vkDeviceWaitIdle(device);
            destroyFramebuffers();
            g_SwapChainImages.clear();
            g_Swapchain = VK_NULL_HANDLE;
        }
        Orig(device, swapchain, pAllocator);
    }
};

HOOK_DEFINE_TRAMPOLINE(hooked_vkGetDeviceProcAddr)
{
    static PFN_vkVoidFunction Callback(VkDevice device,const char* pName)
    {
        PFN_vkVoidFunction result = Orig(device, pName);

        if (!pName || !result)
            return result;

        if (strcmp(pName, "vkCreateDisplayPlaneSurfaceKHR") == 0)
        {
            static bool installed = false;
            if (!installed)
            {
                vkCreateDisplayPlaneSurfaceKHRReplace::InstallAtPtr(reinterpret_cast<uintptr_t>(result));
                installed = true;
            }
        }
        else if (strcmp(pName, "vkCreateSwapchainKHR") == 0)
        {
            static bool installed = false;
            if (!installed)
            {
                vkCreateSwapchainKHRReplace::InstallAtPtr(reinterpret_cast<uintptr_t>(result));
                installed = true;
            }
        }
        else if (strcmp(pName, "vkDestroySwapchainKHR") == 0)
        {
            static bool installed = false;
            if (!installed)
            {
                vkDestroySwapchainKHRReplace::InstallAtPtr(reinterpret_cast<uintptr_t>(result));
                installed = true;
            }
        }
        else if (strcmp(pName, "vkQueueSubmit") == 0)
        {
            static bool installed = false;
            if (!installed)
            {
                hooked_vkQueueSubmit::InstallAtPtr(reinterpret_cast<uintptr_t>(result));
                installed = true;
            }
        }
        else if (strcmp(pName, "vkQueuePresentKHR") == 0)
        {
            static bool installed = false;
            if (!installed)
            {
                hooked_vkQueuePresentKHR::InstallAtPtr(reinterpret_cast<uintptr_t>(result));
                installed = true;
            }
        }

        return result;
    }
};
class VulkanHook : public FuncHook {
public:
	bool Initialize() override {
    hooked_vkGetInstanceProcAddr::InstallAtSymbol("vkGetInstanceProcAddr");
    hooked_vkGetDeviceProcAddr::InstallAtSymbol("vkGetDeviceProcAddr");
		return true;
	}

	static VulkanHook& Instance() {
		static VulkanHook instance;
		return instance;
	}
};