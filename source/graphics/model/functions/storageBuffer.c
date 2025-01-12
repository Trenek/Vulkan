#include <vulkan/vulkan.h>

#include "bufferOperations.h"
#include "model.h"

#include "definitions.h"

void createStorageBuffer(VkDeviceSize bufferSize, VkBuffer uniformBuffers[], VkDeviceMemory uniformBuffersMemory[], void *uniformBuffersMapped[], VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    size_t i = 0;

    while (i < MAX_FRAMES_IN_FLIGHT) {
        uniformBuffers[i] = createBuffer(
            device,
            physicalDevice,
            surface,
            bufferSize,
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        );
        uniformBuffersMemory[i] = createBufferMemory(
            device,
            physicalDevice,
            uniformBuffers[i],
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);

        i += 1;
    }
}

void destroyStorageBuffer(VkDevice device, VkBuffer uniformBuffers[], VkDeviceMemory uniformBuffersMemory[]) {
    size_t i = 0;

    while (i < MAX_FRAMES_IN_FLIGHT) {
        vkDestroyBuffer(device, uniformBuffers[i], NULL);
        vkFreeMemory(device, uniformBuffersMemory[i], NULL);

        i += 1;
    }
}
