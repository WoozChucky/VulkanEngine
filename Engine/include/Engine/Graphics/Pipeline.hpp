//
// Created by nunol on 12/30/2021.
//

#ifndef GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_PIPELINE_HPP_
#define GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_PIPELINE_HPP_

#include <Engine/Graphics/Device.hpp>
#include <string>

namespace nl {

struct PipelineConfigInfo {

	PipelineConfigInfo() = default;
	PipelineConfigInfo(const PipelineConfigInfo&) = delete;
	PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

	VkPipelineViewportStateCreateInfo viewportInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	std::vector<VkDynamicState> dynamicStateEnables;
	VkPipelineDynamicStateCreateInfo dynamicStateInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};

class Pipeline {

 public:
	Pipeline(Device& device, const std::string& vertexFilePath, const std::string& fragmentFilePath, const PipelineConfigInfo& config);
	~Pipeline();

	Pipeline(const Pipeline&) = delete;
	Pipeline& operator=(const Pipeline&) = delete;

	void bind(VkCommandBuffer commandBuffer);
	static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

 private:

	void createGraphicsPipeline(const std::string& vertexFilePath, const std::string& fragmentFilePath, const PipelineConfigInfo& config);

	void createShaderModule(const std::vector<U8>& code, VkShaderModule* shaderModule);

	Device& _device;
	VkPipeline _graphicsPipeline;
	VkShaderModule _vertShaderModule;
	VkShaderModule _fragShaderModule;
};

}



#endif //GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_PIPELINE_HPP_
