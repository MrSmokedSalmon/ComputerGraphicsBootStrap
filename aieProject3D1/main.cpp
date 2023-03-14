#include "aieProject3D1App.h"

int main() {
	
	// allocation
	auto app = new aieProject3D1App();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}