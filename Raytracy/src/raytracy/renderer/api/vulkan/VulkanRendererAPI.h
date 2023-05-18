#pragma once

#include "../RendererAPI.h"
#include "../Buffer.h"

namespace raytracy {

	class VulkanRendererAPI : public RendererAPI {
		

	public:

		virtual void Init(shared_ptr<GraphicsContext> context) override;

		virtual void ClearViewport() override;

		virtual void SetClearColor(const glm::vec4& clear_color) override;

		virtual void DrawIndexed(const shared_ptr<IndexBuffer>& index_buffer) override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void Shutdown() override;
	};
}