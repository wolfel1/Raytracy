#pragma once

#include <glm/glm.hpp>

struct BoundingBoxNode {
	glm::vec3 min_corner{};
	uint32_t left_child_index{};
	glm::vec3 max_corner{};
	uint32_t right_child_index{};
	std::vector<uint32_t> triangle_indices;
};