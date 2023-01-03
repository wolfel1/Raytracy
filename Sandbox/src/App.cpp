#include <raytracy.h>
#include <raytracy/EntryPoint.h>


class Sandbox : public raytracy::Application {
public:
	Sandbox() {

	}
	~Sandbox() {}
};

raytracy::Application* raytracy::CreateApplication() {
	return new Sandbox();
}