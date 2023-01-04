#include <raytracy.h>
#include <raytracy/EntryPoint.h>

using namespace raytracy;

class Sandbox : public Application {
public:
	Sandbox() {
		const auto aspect_ratio = 16.0f / 9.0f;
		ImageData image_data{};
		image_data.width = 480;
		image_data.height = static_cast<int>(image_data.width / aspect_ratio);
		image_data.samples_per_pixel = 100;
		image_data.max_depth = 50;

		HitableCollection scene;
		auto material_ground =
			make_shared<LambertianDiffuse>(Color3(0.8f, 0.8f, 0.0f));
		auto material_center =
			make_shared<LambertianDiffuse>(Color3(0.7f, 0.3f, 0.3f));
		auto material_left = make_shared<Dielectric>(1.5f);
		auto material_right = make_shared<Metal>(Color3(0.8f, 0.6f, 0.2f), 0.0f);

		scene.Add(make_shared<Sphere>(Point3(0.0f, -100.5f, -1.0f), 100.0f,
									  material_ground));
		scene.Add(
			make_shared<Sphere>(Point3(0.0f, 0.0f, -1.0f), 0.5f, material_center));
		scene.Add(
			make_shared<Sphere>(Point3(-1.0f, 0.0f, -1.0f), 0.5f, material_left));
		scene.Add(
			make_shared<Sphere>(Point3(-1.0f, 0.0f, -1.0f), -0.4f, material_left));
		scene.Add(
			make_shared<Sphere>(Point3(1.0f, 0.0f, -1.0f), 0.5f, material_right));

		Point3 look_from(-2, 2, 1);
		Point3 look_at(0, 0, -1);
		glm::vec3 up(0, 1, 0);

		Camera camera(look_from, look_at, up, 20.0f, aspect_ratio);

		Raytracer::Submit(scene, camera, image_data);
	}
	~Sandbox() {}
};

raytracy::Application* raytracy::CreateApplication() {
	return new Sandbox();
}