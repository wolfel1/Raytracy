#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <raytracy/core/Input.h>
#include <raytracy/Application.h>

using namespace raytracy;

class ApplicationMock : public IApplication {
public:
	virtual ~ApplicationMock() {}
	MOCK_METHOD(shared_ptr<IWindow>, GetWindow, (), (const, override));
	MOCK_METHOD(ApplicationSpecification const&, GetSpecification, (), (const, override));
};

class WindowMock : public IWindow {
public:
	MOCK_METHOD(void*, GetNativeWindow, (), (const override));
	MOCK_METHOD(uint32_t, GetWidth, (), (const override));
	MOCK_METHOD(uint32_t, GetHeight, (), (const override));
};

class GLFWProxyMock {
public:
	static int GetKey(GLFWwindow* window, int key) {
		int state = -1;
		if (key == static_cast<int>(KeyCode::E)) {
			state = GLFW_PRESS;
		} else if (key == static_cast<int>(KeyCode::A)) {
			state = GLFW_REPEAT;
		}
		return state;
	}

	static int GetMouseButton(GLFWwindow* window, int button) {
		int state = -1;
		if (button == static_cast<int>(MouseCode::Button0)) {
			state = GLFW_PRESS;
		} else if (button == static_cast<int>(MouseCode::Button1)) {
			state = GLFW_REPEAT;
		}
		return state;
	}

	static void GetCursorPos(GLFWwindow* window, double* x, double* y) {
		*x = 1.0; *y = 1.0;
	}

};

class InputTest : public testing::Test {
public:
	ApplicationMock application_mock;
	shared_ptr<WindowMock> window_mock;
	Input& instance = Input::Get();

protected:

	void SetUp() override {
		window_mock = make_shared<WindowMock>();
		ON_CALL(application_mock, GetWindow).WillByDefault(testing::Return(window_mock));
	}

	void TearDown() override {
	}
};

TEST_F(InputTest, IsKeyPressed) {

	EXPECT_CALL(application_mock, GetWindow).Times(3);
	EXPECT_CALL(*window_mock.get(), GetNativeWindow).Times(3);

	EXPECT_FALSE(instance.IsKeyPressed<GLFWProxyMock>(KeyCode::D));

	EXPECT_TRUE(instance.IsKeyPressed<GLFWProxyMock>(KeyCode::A));
	EXPECT_TRUE(instance.IsKeyPressed<GLFWProxyMock>(KeyCode::E));
}

TEST_F(InputTest, IsMouseButtonPressed) {

	EXPECT_CALL(application_mock, GetWindow).Times(3);
	EXPECT_CALL(*window_mock.get(), GetNativeWindow).Times(3);

	EXPECT_FALSE(instance.IsMouseButtonPressed<GLFWProxyMock>(MouseCode::Button2));

	EXPECT_TRUE(instance.IsMouseButtonPressed<GLFWProxyMock>(MouseCode::Button0));
	EXPECT_TRUE(instance.IsMouseButtonPressed<GLFWProxyMock>(MouseCode::Button1));
}

TEST_F(InputTest, GetMousePosition) {

	EXPECT_CALL(application_mock, GetWindow).Times(1);
	EXPECT_CALL(*window_mock.get(), GetNativeWindow).Times(1);

	glm::vec2 result = instance.GetMousePosition<GLFWProxyMock>();
	EXPECT_FLOAT_EQ(result.x, 1.0f) << "Actual: " << result.x;
	EXPECT_FLOAT_EQ(result.y, 1.0f) << "Actual: " << result.y;
}