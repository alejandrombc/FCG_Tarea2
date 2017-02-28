//Alejandro Barone - CI 24206267

#include "Modelos.h"
extern vector<Modelos> models; //Todos los modelos iran en este vector

//Para crear la matriz de escalamiento en base a un modelo
glm::mat4 scale_en_matriz(float scale_tx) {
	
	glm::mat4 scaleMatrix;
	scaleMatrix = glm::mat4(glm::vec4(scale_tx, 0.0, 0.0, 0.0), glm::vec4(0.0, scale_tx, 0.0, 0.0), glm::vec4(0.0, 0.0, scale_tx, 0.0), glm::vec4(0.0, 0.0, 0.0, 1)); //Creo matriz de escalamiento

	return scaleMatrix;
}

//Para crear la matriz de translate en base a un modelo
glm::mat4 translate_en_matriz(float translate_tx, float translate_ty, float translate_tz) {
	glm::mat4 translateMatrix = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 1.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 1.0, 0.0), glm::vec4(translate_tx, translate_ty, translate_tz, 1)); //Creo matriz de translate
	return translateMatrix;
}



//Para crear la matriz de rotacion en base a un modelo
glm::mat4 rotacion_en_matriz(float rotacion_tx, float rotacion_ty, float rotacion_tz, float rotacion_ta) {
	glm::mat4 rotateMatrix;
	//Creo matriz de rotacion usando los quat
	rotateMatrix[0][0] = 1.0 - 2.0 * (rotacion_ty * rotacion_ty + rotacion_tz * rotacion_tz);
	rotateMatrix[0][1] = 2.0 * (rotacion_tx * rotacion_ty + rotacion_tz * rotacion_ta);
	rotateMatrix[0][2] = 2.0 * (rotacion_tx * rotacion_tz - rotacion_ty * rotacion_ta);
	rotateMatrix[0][3] = 0.0;
	rotateMatrix[1][0] = 2.0 * (rotacion_tx * rotacion_ty - rotacion_tz * rotacion_ta);
	rotateMatrix[1][1] = 1.0 - 2.0 * (rotacion_tx * rotacion_tx + rotacion_tz * rotacion_tz);
	rotateMatrix[1][2] = 2.0 * (rotacion_ty * rotacion_tz + rotacion_tx * rotacion_ta);
	rotateMatrix[1][3] = 0.0;
	rotateMatrix[2][0] = 2.0 * (rotacion_tx * rotacion_tz + rotacion_ty * rotacion_ta);
	rotateMatrix[2][1] = 2.0 * (rotacion_ty * rotacion_tz - rotacion_tx * rotacion_ta);
	rotateMatrix[2][2] = 1.0 - 2.0 * (rotacion_tx * rotacion_tx + rotacion_ty * rotacion_ty);
	rotateMatrix[2][3] = 0.0;
	rotateMatrix[3][0] = 0.0;
	rotateMatrix[3][1] = 0.0;
	rotateMatrix[3][2] = 0.0;
	rotateMatrix[3][3] = 1.0;
	return rotateMatrix;
}


//Funciones de la clase Modelos

Modelos::Modelos() {}

Modelos::~Modelos() {}

//Cargar textura
unsigned int Modelos::LoadTexture(const char* filename, bool text) {
	ilInit();
	ILuint imageID;
	GLuint textureID;
	ILboolean success;
	ILenum error;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	success = ilLoadImage(filename);
	if (success)
	{
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		if (!success) {
			error = ilGetError();
			cout << "Image conversion fails" << endl;
		}
		glGenTextures(1, &textureID);
		if (text)
			glActiveTexture(GL_TEXTURE0);
		else
			glActiveTexture(GL_TEXTURE1);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexImage2D(GL_TEXTURE_2D,
			0,
			ilGetInteger(IL_IMAGE_FORMAT),
			ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT),
			0,
			ilGetInteger(IL_IMAGE_FORMAT),
			GL_UNSIGNED_BYTE,
			ilGetData()
		);
	}

	ilDeleteImages(1, &imageID);
	return textureID;
}

//Crea los VBOS con vertice, normal y textura
void Modelos::initVBO() {
	glGenBuffers(1, &(models[models.size() - 1].vbo));
	glBindBuffer(GL_ARRAY_BUFFER, models[models.size() - 1].vbo);
	glBufferData(GL_ARRAY_BUFFER, models[models.size() - 1].vertices.size() * sizeof(float) + models[models.size() - 1].coord_texturas.size() * sizeof(float) + models[models.size() - 1].normales_vertice_fin.size() * sizeof(float) + models[models.size() - 1].tangents.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	//Guardo Vertices en el VBO
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		models[models.size() - 1].vertices.size() * sizeof(float),
		models[models.size() - 1].vertices.data());
	glBufferSubData(GL_ARRAY_BUFFER,
		models[models.size() - 1].vertices.size() * sizeof(float),
		models[models.size() - 1].coord_texturas.size() * sizeof(float),
		models[models.size() - 1].coord_texturas.data());

	glBufferSubData(GL_ARRAY_BUFFER,
		models[models.size() - 1].vertices.size() * sizeof(float) + models[models.size() - 1].coord_texturas.size() * sizeof(float),
		models[models.size() - 1].normales_vertice_fin.size() * sizeof(float),
		models[models.size() - 1].normales_vertice_fin.data());
	
	glBufferSubData(GL_ARRAY_BUFFER,
		models[models.size() - 1].vertices.size() * sizeof(float) + models[models.size() - 1].coord_texturas.size() * sizeof(float) + models[models.size() - 1].normales_vertice_fin.size() * sizeof(float),
		models[models.size() - 1].tangents.size() * sizeof(glm::vec3),
		models[models.size() - 1].tangents.data());
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	const char* file;
	if (models.size() == 1) {
		file = "Texturas/texture_sun.jpg";
		texture1 = LoadTexture(file, true);
	}
	else if (models.size() == 2) {
		file = "Texturas/brickwall.JPG";
		texture2 = LoadTexture(file, true);
		file = "Texturas/brickwall_normal.JPG";
		texture8 = LoadTexture(file, true);
	}
	else if (models.size() == 3) {
		file = "Texturas/cubeText.gif";
		texture3 = LoadTexture(file, true);

		file = "Texturas/LightmapText.gif";
		texture10 = LoadTexture(file, true);
	}
	else if (models.size() == 4) {
		file = "Texturas/cube.png";
		texture4 = LoadTexture(file, true);
	}
	else if (models.size() == 5) {
		file = "Texturas/cy.png";
		texture5 = LoadTexture(file, true);
	}
	else if (models.size() == 6) {
		file = "Texturas/skull.jpg";
		texture6 = LoadTexture(file, true);
	}
	else if (models.size() == 7) {

		file = "Texturas/arrow.tga";
		texture7 = LoadTexture(file, true);
		//file = "Texturas/cube_N.png";
		file = "Texturas/arrow_normal.jpg";
		texture9 = LoadTexture(file, true);
	}

}

//Iniciar shader principal
void Modelos::loadShaderPrincipal() {
	programPrincipal.loadShader("Shaders/vertex_shader.ver", CGLSLProgram::VERTEX);
	programPrincipal.loadShader("Shaders/fragment_shader.frag", CGLSLProgram::FRAGMENT);

	//system("pause");
	programPrincipal.create_link();

	programPrincipal.enable();

	programPrincipal.addAttribute("position");
	programPrincipal.addAttribute("coord_texturas");
	programPrincipal.addAttribute("normal");
	programPrincipal.addAttribute("tangentes");


	programPrincipal.addUniform("projection_matrix");
	programPrincipal.addUniform("view_matrix");
	programPrincipal.addUniform("model_matrix");
	programPrincipal.addUniform("reflect_matrix");
	programPrincipal.addUniform("lightSpaceMatrix");
	programPrincipal.addUniform("shadowMat");
	programPrincipal.addUniform("view");

	programPrincipal.addUniform("dirLight.direction");
	programPrincipal.addUniform("dirLight.position");
	programPrincipal.addUniform("dirLight.ambient");
	programPrincipal.addUniform("dirLight.diffuse");
	programPrincipal.addUniform("dirLight.specular");


	programPrincipal.addUniform("model.ambient");
	programPrincipal.addUniform("model.diffuse");
	programPrincipal.addUniform("model.specular");
	programPrincipal.addUniform("model.biasM");

	programPrincipal.addUniform("tex");
	programPrincipal.addUniform("normalMap");
	programPrincipal.addUniform("skyboxText");
	programPrincipal.addUniform("depthText");

	programPrincipal.addUniform("position_i");
	programPrincipal.addUniform("bb");
	programPrincipal.addUniform("hayTex");
	programPrincipal.addUniform("planar");

	programPrincipal.addUniform("fRefract");

	programPrincipal.addUniform("currentLight");
	programPrincipal.addUniform("pointConstant");
	programPrincipal.addUniform("pointLinear");
	programPrincipal.addUniform("pointQuadratic");

	programPrincipal.addUniform("innerCut");
	programPrincipal.addUniform("outerCut");

	programPrincipal.addUniform("hayLuz");
	programPrincipal.addUniform("hayNormalMapping");

	programPrincipal.disable();
}
