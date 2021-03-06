#pragma once

#include "../gui.hpp"
#include "../window/window.hpp"
#include "../util/transform.hpp"

#include <algorithm> // Necessary for std::min/std::max
#include <cstdint>   // Necessary for UINT32_MAX
#include <cstring>
#include <fstream>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>
#include <chrono>
#include <string.h>
#include <assert.h>
#include <cmath>

#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

#ifdef DEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = false;
#endif

#define VK_CHECK_RESULT(f)                                                                \
    {                                                                                     \
        VkResult res = (f);                                                               \
        if (res != VK_SUCCESS)                                                            \
        {                                                                                 \
            printf("Fatal : VkResult is %d in %s at line %d\n", res, __FILE__, __LINE__); \
            assert(res == VK_SUCCESS);                                                    \
        }                                                                                 \
    }

namespace game
{
    Window *window;
    float deltaTime;

    VkInstance instance;

    // Validation
    std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    VkDebugUtilsMessengerEXT debugMessenger;
    bool checkValidationLayerSupport();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    // Surface
    VkSurfaceKHR surface;

    // Device
    VkDevice device;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    struct QueueFamilyIndices
    {
        uint32_t graphicsFamily;
        uint32_t presentFamily;
        uint32_t computeFamily;
        uint32_t transferFamily;
    };

    QueueFamilyIndices queueFamilyIndices;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue computeQueue;
    VkQueue transferQueue;

    // SwapChain
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    // Renderpass
    VkRenderPass renderPass;

    void createRenderPass();

    // FrameBuffers
    std::vector<VkFramebuffer> swapChainFramebuffers;

    void createFramebuffers();

    // Descriptor
    VkDescriptorPool graphicsDescriptorPool;
    VkDescriptorSetLayout graphicsDescriptorSetLayout;
    std::vector<VkDescriptorSet> graphicsDescriptorSets;

    VkDescriptorPool computeDescriptorPool;
    VkDescriptorSet computeDescriptorSet;
    VkDescriptorSetLayout computeDescriptorSetLayout;

    void createGraphicsDescriptorPool();
    void createGraphicsDescriptorSetLayout();
    void createGraphicsDescriptorSets();

    struct UniformBufferObject
    {
        glm::mat4 view;
        glm::mat4 proj;
    };

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    void createUniformBuffers();

    // Buffer
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    // Graphics Pipeline
    VkPipeline graphicsPipeline;
    VkPipelineLayout graphicsPipelineLayout;

    // Compute Pipeline
    VkPipeline computePipeline;
    VkPipelineLayout computePipelineLayout;

    void createGraphicsPipeline();
    static std::vector<char> readFile(const std::string &filename);

    // Mesh
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;
        glm::vec2 texCoord;
    };

    struct Mesh
    {
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;
    };

    Mesh mesh;

    VkDeviceSize vertexBufferSize;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer vertexBuffer;
    void *vertexBufferdata;

    VkDeviceSize indexBufferSize;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    void *indexBufferdata;

    // Comand
    VkCommandPool graphicsCommandPool;
    std::vector<VkCommandBuffer> graphicsCommandBuffers;

    VkCommandPool computeCommandPool;
    VkCommandBuffer computeCommandBuffer;

    VkCommandPool transferCommandPool;

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void createGraphicsCommandBuffers();

    // Sync
    const int MAX_FRAMES_IN_FLIGHT = 2;
    size_t currentFrame = 0;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;

    VkFence computeFence;

    // Camera
    Transform cameraTransform;
    const float cameraSpeed = 10.0f;
    const float zoomSpeed = 5.0f;
    float cameraBounds = 2.0f;

    // Depth
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    void createDepthResources();
    VkFormat findDepthFormat();

    // Image
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    // Compute Buffer
    const int WIDTH = 3200;        // Size of rendered mandelbrot set.
    const int HEIGHT = 2400;       // Size of renderered mandelbrot set.
    const int WORKGROUP_SIZE = 32; // Workgroup size in compute shader.
    struct Pixel
    {
        float r, g, b, a;
    };

    VkBuffer computeBuffer;
    VkDeviceMemory computeBufferMemory;

    uint32_t computeBufferSize; // size of `buffer` in bytes.

} // namespace game