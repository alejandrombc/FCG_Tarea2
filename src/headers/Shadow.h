#pragma once
#include "Main.h"
#include "Modelos.h"

class Shadow {
public:
	CGLSLProgram programDepth;
	CGLSLProgram programShadowBox;
	GLuint depthMap;
	GLuint depthMapFBO;
	const GLuint SHADOW_WIDTH = 1024;
	const GLuint SHADOW_HEIGHT = 1024;
	GLuint quadVAO = 0;
	GLuint quadVBO;
	//Metodos
	Shadow();
	~Shadow();
	void initShadow();
	void drawShadow();
	void drawDepth();
	void loadShadowShader();
	void loadDepthShader();
	glm::mat4 createShadowMatrix(glm::vec4 light);
};