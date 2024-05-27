#pragma once

#include <gtest/gtest.h>

#include <raytracy/Application.h>

using namespace raytracy;

namespace ApplicationTest {

	TEST(HelloTest, BasicAssertions) {
		EXPECT_STRNE("ello", "world");
		EXPECT_EQ(7*6, 42);
	}
	
}