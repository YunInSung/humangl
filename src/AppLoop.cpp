#include "App.hpp"

void App::loop() {

    std::vector<glm::vec3> quadVertices = {
        {-1.0f,  1.0f, 0},
        {-1.0f, -1.0f, 0},
        { 1.0f, -1.0f, 0},
        {-1.0f,  1.0f, 0},
        { 1.0f, -1.0f, 0},
        { 1.0f,  1.0f, 0}
    };
    std::vector<glm::vec2> quadTexture = {
        {0.0f, 1.0f},
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f}
    };

	std::vector<glm::vec3> corAxisArray;
	for (int i = -1000; i <= 1000; i++) {
		corAxisArray.push_back(glm::vec3(i, 0, 0));
		corAxisArray.push_back(glm::vec3(0, i, 0));
		corAxisArray.push_back(glm::vec3(0, 0, i));
	}

	float px = 1 / (float)this->framebufferWidth;
	float py = 1 / (float)this->framebufferHeight;
	float wid = px * 70 / 2.f;
	float hei = py * 70 / 2.f;
	std::vector<glm::vec3> crossHeadArray;
	crossHeadArray.push_back({wid * -1, 0, 0});
	crossHeadArray.push_back({wid, 0, 0});
	crossHeadArray.push_back({0, hei * -1, 0});
	crossHeadArray.push_back({0, hei, 0});

	Object corAxis;
	corAxis.setVertex(corAxisArray);
	corAxis.setting();

	Object crossHead;
	crossHead.setVertex(crossHeadArray);
	crossHead.setting();

	Skeleton sk;
	sk.setASF("./resource/skeleton/143.asf");
	sk.setAMC("./resource/motion/143.amc");

	Object frame;
	frame.setVertex(quadVertices);
	frame.setTexture(quadTexture);
	frame.setting();

	Framebuffer quad(this->framebufferWidth, this->framebufferHeight, false, 0);

	UniformBlockManager uboM("Matrices", this->programs);

	while(!glfwWindowShouldClose(this->window)) {

		double time = glfwGetTime();
		double delta = time - this->lastTime;
		this->lastTime = time;

		this->setFrameTitle();

		this->renderCamera(delta);
		glm::mat4 view(this->camera.getView());
		glm::mat4 projection(this->camera.getProjection());

		sk.setLocal(this->lastTime);
		sk.setGlobalTransform();

		Person per(sk);
		glm::mat4 model(1);

		uboM.use();
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboM.getUniformBlockObject(), 0, 2 * sizeof(glm::mat4));
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		uboM.unuse();

		quad.use();
		glViewport(0, 0, this->framebufferWidth, this->framebufferHeight);

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.3, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		this->useProgram("cor");
		this->setMat4("cor", "MODEL", corAxis.getModel());
		this->setVec4("cor", "COLOR", 1, 0.2, 0.2, 1);
		glPointSize(5);
		corAxis.drawPoints();

		this->useProgram("normal");
		this->setMat4("normal", "MODEL", model);
		this->setVec4("normal", "COLOR", 0, 1, 1, 1);
		per.drawLine();

		this->useProgram("normal");
		glPointSize(10);
		this->setMat4("normal", "MODEL", model);
		this->setVec4("normal", "COLOR", 0, 1, 0, 1);
		per.drawPoint();

		this->useProgram("skeleton");
		this->setMat4("skeleton", "MODEL", model);
		this->setVec4("skeleton", "COLOR", 0, 0, 1, 0.5);
		per.drawBody();

		// print quad
		quad.unuse();
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		this->useProgram("default");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, quad.getTexture());
		frame.drawTriangles();

		this->useProgram("crossHead");
		this->setVec4("crossHead", "COLOR", 0, 1, 0, 0.6);
		crossHead.drawLines();
		// crossHead.drawTriangles();

		// print quad

		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}
}