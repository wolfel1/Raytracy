#include <raytracy.h>
#include <raytracy/EntryPoint.h>

using namespace raytracy;

class Sandbox : public Application {
public:
	Sandbox() {

	}
	~Sandbox() {}
};

raytracy::Application* raytracy::CreateApplication() {
	return new Sandbox();
}