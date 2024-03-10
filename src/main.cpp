#include "main.hpp"

int main(int ac, char **av) {


	App app("test", 1024, 720);
	app.setting();

	app.setProgram("skeleton", "./src/shader/skeleton.vs", "./src/shader/skeleton.fs");
	app.setProgram("cor", "./src/shader/cor.vs", "./src/shader/cor.fs");
	app.setProgram("default", "./src/shader/default.vs", "./src/shader/default.fs");
	app.setProgram("normal", "./src/shader/normal.vs", "./src/shader/normal.fs");
	app.setProgram("crossHead", "./src/shader/crossHead.vs", "./src/shader/normal.fs");

	if (app.isError())
		return 1;

	app.loop();
}

