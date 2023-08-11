#include "raytracypch.h"
#include "VulkanRendererAPI.h"


namespace raytracy {

	VulkanRendererAPI::VulkanRendererAPI() : clear_color({0.0f, 0.0f, 0.0f, 1.0f}) {
	}

	void VulkanRendererAPI::Init() {
		graphics_context = std::dynamic_pointer_cast<VulkanContext>(GraphicsContext::GetCurrentContext());
		RTY_ASSERT(graphics_context, "Context is not Vulkan!");

		auto& event_bus = EventBus::Get();
		event_bus.Register<ShaderChangedEvent>(RTY_BIND_EVENT_FN(VulkanRendererAPI::CreateGraphicsPipeline));

		CreateCommandPool();
		CreateCommandBuffers();
		CreateSyncObjects();
	}

	bool VulkanRendererAPI::CreateGraphicsPipeline(Event& e) {

		ShaderChangedEvent evt = static_cast<ShaderChangedEvent&>(e);
		auto shader = std::dynamic_pointer_cast<VulkanShader>(evt.GetShader());
		RTY_ASSERT(shader, "Shader is not a vulkan shader!");

		auto& shader_modules = shader->GetShaderModules();
		VkShaderModule vertex_shader_module = shader_modules.at(VK_SHADER_STAGE_VERTEX_BIT);
		VkShaderModule fragment_shader_module = shader_modules.at(VK_SHADER_STAGE_FRAGMENT_BIT);

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

		auto bindingDescription = shader->GetBindingDescription();;
		auto attributeDescriptions = shader->GetAttributeDescriptions();

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
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
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

		return true;
	}

	void VulkanRendererAPI::CreateCommandPool() {
		QueueFamilyIndices queue_family_indices = graphics_context->FindQueueFamilyIndices(graphics_context->GetPhysicalDevice());
		auto logical_device = graphics_context->GetLogicalDevice();

		VkCommandPoolCreateInfo pool_info{};
		pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

		if (vkCreateCommandPool(logical_device, &pool_info, nullptr, &command_pool) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create command pool!");
		}
	}

	void VulkanRendererAPI::CreateCommandBuffers() {
		auto logical_device = graphics_context->GetLogicalDevice();

		command_buffers.resize(MAX_FRAMES_IN_FLIGHT);
		VkCommandBufferAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.commandPool = command_pool;
		alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandBufferCount = (uint32_t)command_buffers.size();

		if (vkAllocateCommandBuffers(logical_device, &alloc_info, command_buffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("Failed to allocate command buffers!");
		}
	}

	void VulkanRendererAPI::CreateSyncObjects() {
		auto logical_device = graphics_context->GetLogicalDevice();
		image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
		render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
		in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphore_info{};
		semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fence_info{};
		fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(logical_device, &semaphore_info, nullptr, &image_available_semaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(logical_device, &semaphore_info, nullptr, &render_finished_semaphores[i]) != VK_SUCCESS ||
				vkCreateFence(logical_device, &fence_info, nullptr, &in_flight_fences[i]) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create sync objects");
			}
		}
	}

	void VulkanRendererAPI::RecordCommandBuffer(VkCommandBuffer command_buffer, uint32_t image_index, const shared_ptr<VertexArray>& vertex_array) {
		auto render_pass = graphics_context->GetRenderPass();
		auto& swap_chain_extent = graphics_context->GetSwapChainExtent();
		auto& swap_chain_framebuffers = graphics_context->GetSwapChainFrameBuffers();

		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = 0;
		begin_info.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo render_pass_info{};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass = render_pass;
		render_pass_info.framebuffer = swap_chain_framebuffers[image_index];
		render_pass_info.renderArea.offset = { 0, 0 };
		render_pass_info.renderArea.extent = swap_chain_extent;

		VkClearValue clear_color = { this->clear_color.r, this->clear_color.g, this->clear_color.b, this->clear_color.a };
		render_pass_info.clearValueCount = 1;
		render_pass_info.pClearValues = &clear_color;

		vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)swap_chain_extent.width;
		viewport.height = (float)swap_chain_extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(command_buffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = swap_chain_extent;
		vkCmdSetScissor(command_buffer, 0, 1, &scissor);

		auto vertex_buffer = std::dynamic_pointer_cast<VulkanVertexBuffer>(vertex_array->GetVertexBuffer());
		RTY_ASSERT(vertex_buffer, "Vertex buffer suits not to vulkan!");
		VkBuffer vertex_buffers[] = { vertex_buffer->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);

		auto index_buffer = std::dynamic_pointer_cast<VulkanIndexBuffer>(vertex_array->GetIndexBuffer());
		RTY_ASSERT(index_buffer, "Index buffer suits not to vulkan!");

		vkCmdBindIndexBuffer(command_buffer, index_buffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(command_buffer, index_buffer->GetCount(), 1, 0, 0, 0);

		vkCmdEndRenderPass(command_buffer);

		if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
			throw std::runtime_error("Failed to record command buffer!");
		}
	}


	void VulkanRendererAPI::ClearViewport() {
	}

	void VulkanRendererAPI::SetClearColor(const glm::vec4& clear_color) {
		this->clear_color = clear_color;
	}

	void VulkanRendererAPI::DrawIndexed(const shared_ptr<VertexArray>& vertex_array) {
		auto logical_device = graphics_context->GetLogicalDevice();
		auto swap_chain = graphics_context->GetSwapchain();

		vkWaitForFences(logical_device, 1, &in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);

		uint32_t image_index;
		VkResult result = vkAcquireNextImageKHR(logical_device, swap_chain, UINT64_MAX,
			image_available_semaphores[current_frame], VK_NULL_HANDLE, &image_index);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			graphics_context->RecreateSwapChain();
			return;
		} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to acquire swap chain images!");
		}
		vkResetFences(logical_device, 1, &in_flight_fences[current_frame]);

		vkResetCommandBuffer(command_buffers[current_frame], 0);
		RecordCommandBuffer(command_buffers[current_frame], image_index, vertex_array);

		VkSubmitInfo submit_info{};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore wait_semaphores[] = { image_available_semaphores[current_frame] };
		VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submit_info.waitSemaphoreCount = 1;
		submit_info.pWaitSemaphores = wait_semaphores;
		submit_info.pWaitDstStageMask = wait_stages;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffers[current_frame];

		VkSemaphore signal_semaphores[] = { render_finished_semaphores[current_frame] };
		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores = signal_semaphores;

		auto graphics_queue = graphics_context->GetGraphicsQueue();
		if (vkQueueSubmit(graphics_queue, 1, &submit_info, in_flight_fences[current_frame]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to submit draw command buffer!");
		}

		VkPresentInfoKHR present_info{};
		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount = 1;
		present_info.pWaitSemaphores = signal_semaphores;

		VkSwapchainKHR swap_chains[] = { swap_chain };
		present_info.swapchainCount = 1;
		present_info.pSwapchains = swap_chains;
		present_info.pImageIndices = &image_index;
		present_info.pResults = nullptr;

		auto presentation_queue = graphics_context->GetPresentationQueue();
		result = vkQueuePresentKHR(presentation_queue, &present_info);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebuffer_resized) {
			framebuffer_resized = false;
			graphics_context->RecreateSwapChain();
		} else if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to present swap chain image!");
		}

		current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		framebuffer_resized = true;
	}

	void VulkanRendererAPI::Shutdown() {
		auto logical_device = graphics_context->GetLogicalDevice();
		vkDeviceWaitIdle(logical_device);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(logical_device, image_available_semaphores[i], nullptr);
			vkDestroySemaphore(logical_device, render_finished_semaphores[i], nullptr);
			vkDestroyFence(logical_device, in_flight_fences[i], nullptr);
		}
		vkDestroyCommandPool(logical_device, command_pool, nullptr);
		vkDestroyPipeline(logical_device, graphics_pipeline, nullptr);
		vkDestroyPipelineLayout(logical_device, pipeline_layout, nullptr);

		graphics_context->Shutdown();
	}
}