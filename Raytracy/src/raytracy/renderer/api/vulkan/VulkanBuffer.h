#pragma once

namespace raytracy {
	enum class VertexDataType {
		None = 0, Bool, Int, Int2, Int3, Int4, Float, Float2, Float3, Float4
	};

	static uint32_t SizeOfVertexDataType(VertexDataType type) {
		switch (type) {
		case VertexDataType::Bool:
			return 1;
		case VertexDataType::Float:
		case VertexDataType::Int:
			return 4;
		case VertexDataType::Int2:
		case VertexDataType::Float2:
			return 4 * 2;
		case VertexDataType::Int3:
		case VertexDataType::Float3:
			return 4 * 3;
		case VertexDataType::Int4:
		case VertexDataType::Float4:
			return 4 * 4;
		}

		RTY_RENDERER_ERROR("Unknown VertexDataType!");
		return 0;
	}

	struct BufferElement {
		std::string name;
		VertexDataType type;
		bool normalized;
		uint32_t offset{};

		BufferElement(const std::string& name, VertexDataType type, bool normalized = false) :
			name(name), type(type), normalized(normalized) {
		}

		uint32_t GetComponentCount() const {
			switch (type)
			{
			case raytracy::VertexDataType::Bool:
			case raytracy::VertexDataType::Int:
			case raytracy::VertexDataType::Float:
				return 1;
			case raytracy::VertexDataType::Int2:
			case raytracy::VertexDataType::Float2:
				return 2;
			case raytracy::VertexDataType::Int3:
			case raytracy::VertexDataType::Float3:
				return 3;
			case raytracy::VertexDataType::Int4:
			case raytracy::VertexDataType::Float4:
				return 4;
			default:
				break;
			}

			RTY_RENDERER_ERROR("Unknown VertexDataType!");
			return 0;
		}
	};

	class BufferLayout {
	private:
		std::vector<BufferElement> elements;
		uint32_t stride = 0;

	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements) : elements(elements) {
			CalculateOffsetAndStride();
		}

		uint32_t GetStride() const {
			return stride;
		}

		const std::vector<BufferElement>& GetElements() const {
			return elements;
		}

		void AddElement(const BufferElement& element) {
			elements.push_back(element);
			CalculateOffsetAndStride();
		}

	private:
		void CalculateOffsetAndStride() {
			size_t offset = 0;
			stride = 0;
			for (auto& element : elements) {
				element.offset = offset;
				offset += SizeOfVertexDataType(element.type);
				stride += SizeOfVertexDataType(element.type);
			}
		}
	};
	class VulkanVertexBuffer {
	private:
		BufferLayout layout;
	public:
		VulkanVertexBuffer(size_t size);
		VulkanVertexBuffer(float* vertices, size_t size);
		~VulkanVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		const BufferLayout& GetLayout() const {
			return layout;
		}
		void SetLayout(const BufferLayout& layout) {
			this->layout = layout;
		}
	};

	class VulkanIndexBuffer {

	public:
		VulkanIndexBuffer(uint32_t* indices, uint32_t count);
		~VulkanIndexBuffer();

		 void Bind() const ;
		 void Unbind() const ;
	};
}