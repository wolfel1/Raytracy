#include "raytracypch.h"
#include "Mesh.h"

namespace raytracy {
	Mesh::Mesh() {
		vertex_array = VertexArray::Create();

		vertex_buffer = VertexBuffer::Create(mesh_data.vertices);
		vertex_buffer->SetLayout({
			{ "position", VertexDataType::Float3 },
			{ "color", VertexDataType::Float4 }
		});

		index_buffer = IndexBuffer::Create(mesh_data.indices.data(), mesh_data.indices.size());

		shader = ShaderLibrary::Get().Load("basic");
		RTY_ASSERT(shader, "Could not create a shader program!");
		uniform_buffer.push_back(UniformBuffer::Create({
			{ "color", VertexDataType::Float4 }
		}));

		shader->Bind();
		uniform_buffer[0]->Link();
		uniform_buffer[0]->SetVec4("color", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		shader->Unbind();

		vertex_array->SetVertexBuffer(vertex_buffer);
		vertex_array->SetIndexBuffer(index_buffer);

		RTY_RENDERER_TRACE("Mesh created with type plane.");
	}

	Mesh::~Mesh() {
	}
}