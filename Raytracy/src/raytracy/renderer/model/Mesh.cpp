#include "raytracypch.h"
#include "Mesh.h"

namespace raytracy {
	void Mesh::Init(shared_ptr<MeshData> const mesh_data)  {

		vertex_array = VertexArray::Create();

		vertex_buffer = VertexBuffer::Create(mesh_data->vertices);
		vertex_buffer->SetLayout({
			{ "position", VertexDataType::Float3 },
			{ "color", VertexDataType::Float4 }
		});

		index_buffer = IndexBuffer::Create(mesh_data->indices.data(), mesh_data->indices.size());

		shader = ShaderLibrary::Get().Load("basic");
		RTY_ASSERT(shader, "Could not create a shader program!"); 
		auto shading_uniform_buffer = UniformBuffer::Create({
			{ "color", VertexDataType::Float4 }
		});
		shading_uniform_buffer->SetVec4("color", mesh_data->display_color);
		shader->AddUniformBuffer("shading", shading_uniform_buffer);

		vertex_array->SetVertexBuffer(vertex_buffer);
		vertex_array->SetIndexBuffer(index_buffer);

		RTY_RENDERER_TRACE("Mesh created with type {0}.", mesh_data->name);
	}

	Plane::Plane() {
		auto data = make_shared<PlaneData>();
		data->Init();
		Init(data);
	}
}