#pragma once

#include <vulkan/vulkan.h>

#include "VulkanContext.h"
#include "VulkanBuffer.h"


namespace raytracy {

	static std::vector<char> ReadFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, position);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			return attributeDescriptions;
		}
	};

	class VulkanRendererAPI {
		
	private:
		shared_ptr<VulkanContext> graphics_context;

		VkPipelineLayout pipeline_layout{};
		VkPipeline graphics_pipeline{};

	public:

		void Init(shared_ptr<VulkanContext> context);

		void CreateGraphicsPipeline() {
			auto vertex_shader_code = ReadFile("basicspirv/basicvert.spv");
			auto fragment_shader_code = ReadFile("basicspirv/basicfrag.spv");

			VkShaderModule vertex_shader_module = CreateShaderModule(vertex_shader_code);
			VkShaderModule fragment_shader_module = CreateShaderModule(fragment_shader_code);

			VkPipelineShaderStageCreateInfo vertex_shader_stage_info{};
			vertex_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertex_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertex_shader_stage_info.module = vertex_shader_module;
			vertex_shader_stage_info.pName = "main";

			VkPipelineShaderStageCreateInfo fragment_shader_stage_info{};
			fragment_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragment_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragment_shader_stage_info.module = fragment_shader_module;
			fragment_shader_stage_info.pName = "main";


			VkPipelineShaderStageCreateInfo shader_stages[] = { vertex_shader_stage_info, fragment_shader_stage_info };

			auto bindingDescription = Vertex::getBindingDescription();
			auto attributeDescriptions = Vertex::getAttributeDescriptions();

			VkPipelineVertexInputStateCreateInfo vertex_input_info{};
			vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertex_input_info.vertexBindingDescriptionCount = 1;
			vertex_input_info.pVertexBindingDescriptions = &bindingDescription;
			vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
			vertex_input_info.pVertexAttributeDescriptions = attributeDescriptions.data();

			VkPipelineInputAssemblyStateCreateInfo input_assembly_info{};
			input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			input_assembly_info.primitiveRestartEnable = VK_FALSE;

			auto swap_chain_extent = graphics_context->GetSwapChainExtent();
			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float)swap_chain_extent.width;
			viewport.height = (float)swap_chain_extent.height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor{};
			scissor.offset = { 0, 0 };
			scissor.extent = swap_chain_extent;

			std::vector<VkDynamicState> dynamic_states = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			};
			VkPipelineDynamicStateCreateInfo dynamic_state_info{};
			dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
			dynamic_state_info.pDynamicStates = dynamic_states.data();

			VkPipelineViewportStateCreateInfo viewport_state_info{};
			viewport_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewport_state_info.viewportCount = 1;
			viewport_state_info.scissorCount = 1;

			VkPipelineRasterizationStateCreateInfo rasterizer{};
			rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizer.lineWidth = 1.0f;
			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
			rasterizer.depthBiasEnable = VK_FALSE;
			rasterizer.depthBiasConstantFactor = 0.0f;
			rasterizer.depthBiasClamp = 0.0f;
			rasterizer.depthBiasSlopeFactor = 0.0f;

			VkPipelineMultisampleStateCreateInfo multisample_info{};
			multisample_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisample_info.sampleShadingEnable = VK_FALSE;
			multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			multisample_info.minSampleShading = 1.0f;
			multisample_info.pSampleMask = nullptr;
			multisample_info.alphaToCoverageEnable = VK_FALSE;
			multisample_info.alphaToOneEnable = VK_FALSE;

			//VkPipelineDepthStencilStateCreateInfo used for depth and stencil testintg

			VkPipelineColorBlendAttachmentState color_blend_attachment_state{};
			color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			color_blend_attachment_state.blendEnable = VK_FALSE;
			color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
			color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
			color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
			color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;

			VkPipelineColorBlendStateCreateInfo color_blending_info{};
			color_blending_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			color_blending_info.logicOpEnable = VK_FALSE;
			color_blending_info.logicOp = VK_LOGIC_OP_COPY;
			color_blending_info.attachmentCount = 1;
			color_blending_info.pAttachments = &color_blend_attachment_state;
			color_blending_info.blendConstants[0] = 0.0f;
			color_blending_info.blendConstants[1] = 0.0f;
			color_blending_info.blendConstants[2] = 0.0f;
			color_blending_info.blendConstants[3] = 0.0f;

			VkPipelineLayoutCreateInfo pipeline_layout_info{};
			pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipeline_layout_info.setLayoutCount = 0;
			pipeline_layout_info.pSetLayouts = nullptr;
			pipeline_layout_info.pushConstantRangeCount = 0;
			pipeline_layout_info.pPushConstantRanges = nullptr;

			auto logical_device = graphics_context->GetLogicalDevice();
			if (vkCreatePipelineLayout(logical_device, &pipeline_layout_info, nullptr, &pipeline_layout) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create pipeline layout!");
			}

			VkGraphicsPipelineCreateInfo pipeline_info{};
			pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipeline_info.stageCount = 2;
			pipeline_info.pStages = shader_stages;
			pipeline_info.pVertexInputState = &vertex_input_info;
			pipeline_info.pInputAssemblyState = &input_assembly_info;
			pipeline_info.pViewportState = &viewport_state_info;
			pipeline_info.pRasterizationState = &rasterizer;
			pipeline_info.pMultisampleState = &multisample_info;
			pipeline_info.pDepthStencilState = nullptr;
			pipeline_info.pColorBlendState = &color_blending_info;
			pipeline_info.pDynamicState = &dynamic_state_info;
			pipeline_info.layout = pipeline_layout;
			pipeline_info.renderPass = graphics_context->GetRenderPass();
			pipeline_info.subpass = 0;
			pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
			pipeline_info.basePipelineIndex = -1;

			if (vkCreateGraphicsPipelines(logical_device, VK_NULL_HANDLE, 1,
										  &pipeline_info, nullptr, &graphics_pipeline) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create graphics pipeline!");
			}

			vkDestroyShaderModule(logical_device, fragment_shader_module, nullptr);
			vkDestroyShaderModule(logical_device, vertex_shader_module, nullptr);
		}

		VkShaderModule CreateShaderModule(const std::vector<char>& code) {
			VkShaderModuleCreateInfo create_info{};
			create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			create_info.codeSize = code.size();
			create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

			VkShaderModule shader_module;
			if (vkCreateShaderModule(graphics_context->GetLogicalDevice(), &create_info, nullptr, &shader_module) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create shader_module!");
			}

			return shader_module;
		}

		 void ClearViewport() ;

		 void SetClearColor(const glm::vec4& clear_color);

		 void DrawIndexed(const shared_ptr<VulkanIndexBuffer>& index_buffer) ;

		 void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) ;

		 void Shutdown() ;
	};
}