#include "Shadow.h"


Shadow::Shadow() {}

Shadow::~Shadow() {}

extern lightT currentLight; 

void Shadow::loadShadowShader() {
	programShadowBox.loadShader("Shaders/vertShadow_shader.ver", CGLSLProgram::VERTEX);
	programShadowBox.loadShader("Shaders/fragShadow_shader.frag", CGLSLProgram::FRAGMENT);

	programShadowBox.create_link();

	programShadowBox.enable();

	programShadowBox.addAttribute("position");
	programShadowBox.addAttribute("coord_texturas");
	programShadowBox.addAttribute("normal");
	programShadowBox.addAttribute("tangentes");

	programShadowBox.addUniform("lightSpaceMatrix");
	programShadowBox.addUniform("reflect_matrix");
	programShadowBox.addUniform("model");


	programShadowBox.disable();
}


void Shadow::loadDepthShader() {
	programDepth.loadShader("Shaders/vertDepth_shader.ver", CGLSLProgram::VERTEX);
	programDepth.loadShader("Shaders/fragDepth_shader.frag", CGLSLProgram::FRAGMENT);

	programDepth.create_link();

	programDepth.enable();

	programDepth.addAttribute("position");
	programDepth.addAttribute("texCoords");

	programDepth.addUniform("depthMap_T");
	programDepth.addUniform("near_plane");
	programDepth.addUniform("model");
	programDepth.addUniform("far_plane");
	programDepth.addUniform("currentL");


	programDepth.disable();
}


void Shadow::initShadow() {

	glGenFramebuffers(1, &depthMapFBO);
	// - Create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


void Shadow::drawDepth() {
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
			1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
			1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


void Shadow::drawShadow() {
	GLfloat near_plane = -10.0f, far_plane = 20.0f;
	programDepth.enable();
	glUniform1f(programDepth.getLocation("near_plane"), near_plane);
	glUniform1f(programDepth.getLocation("far_plane"), far_plane);
	glUniform1f(programDepth.getLocation("currentL"), currentLight);

	//Saco la matriz model para colocar el quad en la pantalla
	glm::mat4 model;
	model = translate_en_matriz(0.7, -0.7, 0.0);
	model = model * scale_en_matriz(0.3f);

	glUniformMatrix4fv(programDepth.getLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glUniform1i(programDepth.getLocation("depthMap_T"), 0);
	drawDepth();
	programDepth.disable();
}

glm::mat4 Shadow::createShadowMatrix(glm::vec4 light) {
	glm::mat4 m;
	m[0][0] = light[1];
	m[1][0] = -light[0];
	m[2][0] = 0.0;
	m[3][0] = 0.0;
	m[0][1] = 0.0;
	m[1][1] = 0.0;
	m[2][1] = 0.0;
	m[3][1] = 0.0;
	m[0][2] = 0.0;
	m[1][2] = -light[2];
	m[2][2] = light[1];
	m[3][2] = 0.0;
	m[0][3] = 0.0;
	m[1][3] = -1.0;
	m[2][3] = 0.0;
	m[3][3] = light[1];

	return m;
}