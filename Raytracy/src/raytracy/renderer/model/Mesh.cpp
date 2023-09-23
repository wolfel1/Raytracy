#include "raytracypch.h"
#include "Mesh.h"

namespace raytracy {
	Mesh::Mesh(Primitive const& base_type) {
		switch (base_type) {
		case Primitive::Custom:
			break;
			case Primitive::Plane:
				mesh_data = make_shared<PlaneData>();
				break;
			case Primitive::Cube:
				break;
			default:
				mesh_data = make_shared<MeshData>();
				break;
		}
		mesh_data->Init();

		vertex_array = VertexArray::Create();

		vertex_buffer = VertexBuffer::Create(mesh_data->vertices);
		vertex_buffer->SetLayout({
			{ "position", VertexDataType::Float3 },
			{ "color", VertexDataType::Float4 }
		});

		index_buffer = IndexBuffer::Create(mesh_data->indices.data(), mesh_data->indices.size());

		shader = ShaderLibrary::Get().Load("basic");
		RTY_ASSERT(shader, "Could not create a shader program!");
		shader->AddUniformBuffer("shading", UniformBuffer::Create({
			{ "color", VertexDataType::Float4 }
		}));

		vertex_array->SetVertexBuffer(vertex_buffer);
		vertex_array->SetIndexBuffer(index_buffer);

		RTY_RENDERER_TRACE("Mesh created with type {0}.", mesh_data->name);
	}
}