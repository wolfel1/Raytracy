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
};

class GLFWProxyMock {
public:
	static int GetKey(GLFWwindow* window, int key) {
		int state = -1;
		if (key == static_cast<int>(KeyCode::E)) {
			key = GLFW_PRESS;
		} else if (key == static_cast<int>(KeyCode::A)) {
			key = GLFW_REPEAT;
		}
		return key;
	}

	static int GetMouseButton(GLFWwindow* window, int button) {

	}

	static void GetCursorPos(GLFWwindow* window, double* x, double* y) {

	}

};

class InputTest : public testing::Test {
public:
	ApplicationMock application_mock;
	shared_ptr<WindowMock> window_mock;

protected:

	void SetUp() override {
		window_mock = make_shared<WindowMock>();
		ON_CALL(application_mock, GetWindow).WillByDefault(testing::Return(window_mock));
	}

	void TearDown() override {
	}
};

TEST_F(InputTest, IsKeyPressed) {
	Input& instance = Input::Get();

	EXPECT_CALL(application_mock, GetWindow).Times(3);
	EXPECT_CALL(*window_mock.get(), GetNativeWindow).Times(3);

	EXPECT_FALSE(instance.IsKeyPressed<GLFWProxyMock>(KeyCode::D));

	EXPECT_TRUE(instance.IsKeyPressed<GLFWProxyMock>(KeyCode::A));
	EXPECT_TRUE(instance.IsKeyPressed<GLFWProxyMock>(KeyCode::E));
}
