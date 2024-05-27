#pragma once

#include <raytracy/Application.h>

using namespace raytracy;

namespace ApplicationTest {

	TEST_CASE("[Application]") {
		unique_ptr<Application> application = make_unique<Application>();
	}
}