#define GUEEPO2D_MAIN
#include <gueepo2d.h>

class GoblinApplication : public gueepo::Application {
public:
	GoblinApplication() : Application("Goblin Game Engine", 640, 360) {}
	~GoblinApplication() {}
};

gueepo::Application* gueepo::CreateApplication() {
	return new GoblinApplication();
}
