#include "Main.h"
#include "Modelos.h"
#include "Skybox.h"
#include "OBJ.h"
#include "Interfaz.h"
#include "Shadow.h"

using namespace std;
Modelos mod; Skybox sky; Interfaz inter; Shadow sha;
GLint GLFW_WIDTH = 800, GLFW_HEIGHT = 600;

lightT currentLight = Direccional;

int selectedModel = 0, tiempoRotacion, apagarLuz = 1;
float scaleT = 1.00, ejeX = 0.0, ejeY = 0.0, ejeZ = 0.0, ejeXL = 0.0, ejeYL = 30.64 , ejeZL = 35.50,
bias = 0.05, puntualC = 1.0, puntualL = 0.007, puntualQ = 0.0002, innerCut = 0.91, 
outerCut = 0.815, refraccion=0.65;
float rotacion[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float color_luz_ambiental[] = { 0.0f, 0.0f, 0.0f }; float color_mat_ambiental[3] = { 0.0f, 0.0f, 0.0f };
float color_luz_difusa[] = { 1.0f, 1.0f, 1.0f }; float color_mat_difusa[3] = { 1.0f, 1.0f, 1.0f };
float color_luz_especular[] = { 1.0f, 1.0f, 1.0f }; float color_mat_especular[3] = { 1.0f, 1.0f, 1.0f };
float rotacion_principal[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float direccionLuz[] = { 0.92, 0.39, -0.10 };
float rotacionInicial[] = { 0.0f, 0.0f, 0.0f, 1.0f };
bool selecting = false, MENU_TRY = false, boolTexturas = true, boolMapping = true, hayDepthMap = true;
bool keys[1024]; //Todas las teclas
int tiempo_traslacion[2] = { 0,0 }; int tiempo_escalar[2] = {0,0};

float incrementalValue = 1.0f, beforeOuter = outerCut;

GLfloat lastX = 400, lastY = 300;
GLfloat deltaTime = 0.0f;
bool firstMouse = true;
GLfloat lastFrame = 0.0f;


//<-----------------VARIABLES Y ESTRUCTURAS--------------------->
// Camera
Camera camera(glm::vec3(18.0f, 7.0f, 35.8f));
glm::mat4 project_mat; //Matriz de Proyeccion
glm::mat4 view_mat; //Matriz de View
glm::vec3 eye(0.0f, 0.0f, 2.0f); // Ojo

vector<Modelos> models; //Todos los modelos iran en este ve

//Impresion de pantalla
void initGlew() {
	glEnable(GL_DEPTH_TEST);
	printf("OpenGL Version: %s \n", glGetString(GL_VERSION));
	printf("Vendor: %s \n", glGetString(GL_VENDOR));
	printf("Renderer: %s \n", glGetString(GL_RENDERER));
	printf("GLSL Version: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}


//<-----------------FUNCIONES DE DESPLIEGUE--------------------->

//Funcion que dibuja el BB del modelo seleccionado
void draw_bb(GLuint bool_loc) {
	mod.programPrincipal.enable();
	bool_loc = mod.programPrincipal.getLocation("bb");
	glUniform1i(bool_loc, 1);

	glBegin(GL_LINE_STRIP);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
	glEnd();
	mod.programPrincipal.disable();
}

//Funcion que dibuja el modelo VBO con texturas
void draw(bool flip, bool planar) {
	if (innerCut < outerCut) innerCut = outerCut;
	//Saco matriz de luz
	glm::mat4 projectionLightMat, viewLightMat;
	glm::mat4 spaceLightMatrix, shadow_mat = glm::mat4();
	GLfloat near_plane, far_plane;
	near_plane = 1.0f, far_plane = 68.5f;
	if (currentLight == 3) {
		float fov = outerCut >= 1.0 ? 0.0 : 2.0 - (outerCut*incrementalValue);
		projectionLightMat = glm::perspective(fov, (GLfloat)sha.SHADOW_WIDTH / (GLfloat)sha.SHADOW_HEIGHT, 25.0f, 1000.0f);
		if (!flip) {
			glm::vec3 dirLSpot = glm::vec3(direccionLuz[0], direccionLuz[1], direccionLuz[2]);
			viewLightMat = glm::lookAt(glm::vec3(ejeXL, ejeYL, ejeZL), glm::vec3(ejeXL, ejeYL, ejeZL) + dirLSpot, glm::vec3(0.0, 1.0, 0.0));
		}
		else {
			glm::vec3 dirLSpot = glm::vec3(direccionLuz[0], direccionLuz[1], -direccionLuz[2]);
			viewLightMat = glm::lookAt(glm::vec3(ejeXL, ejeYL, -ejeZL - 63.5), glm::vec3(ejeXL, ejeYL, -ejeZL - 63.5) + dirLSpot, glm::vec3(0.0, 1.0, 0.0));
		}
	} else if(currentLight == 1){

		projectionLightMat = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, near_plane, far_plane);

		if (!flip) viewLightMat = glm::lookAt(glm::vec3(ejeXL, ejeYL, ejeZL), glm::vec3(0.0, -22.0 + 23.64 , -19.5), glm::vec3(0.0, 1.0, 0.0));
		else viewLightMat = glm::lookAt(glm::vec3(ejeXL, ejeYL, -ejeZL - 63.5), glm::vec3(0.0, -22.0 + 23.64, -43.35), glm::vec3(0.0, 1.0, 0.0));

	} else {
		//Es puntual hago sombras planares
		if (planar) {
			if (!flip) shadow_mat = sha.createShadowMatrix(glm::vec4(ejeXL, ejeYL, ejeZL, 1.0));
			else shadow_mat = sha.createShadowMatrix(glm::vec4(ejeXL, ejeYL, -ejeZL - 63.5, 1.0));
		}
	}

	spaceLightMatrix = projectionLightMat * viewLightMat;
	for (int i = 1+planar; i < models.size()-planar; i++) { //Para los modelos quitando el espejo
		if(!flip && !planar) glStencilFunc(GL_ALWAYS, i, -1); //Picking

		mod.programPrincipal.enable();
		GLuint view_matr_loc = mod.programPrincipal.getLocation("view_matrix");
		GLuint model_matr_loc = mod.programPrincipal.getLocation("model_matrix");
		GLuint reflect_matr_loc = mod.programPrincipal.getLocation("reflect_matrix");
		GLuint proj_matr_loc = mod.programPrincipal.getLocation("projection_matrix");
		GLuint view_loc = mod.programPrincipal.getLocation("view");

		GLuint bool_loc = mod.programPrincipal.getLocation("bb");
		GLuint tex_loc = mod.programPrincipal.getLocation("tex");
		GLuint normalMap_loc = mod.programPrincipal.getLocation("normalMap");
		GLuint shadowDepth_loc = mod.programPrincipal.getLocation("depthText");

		glUniform1i(tex_loc, 0);
		glUniform1i(normalMap_loc, 1);
		glUniform1i(bool_loc, 0);
		glUniform3f(view_loc, camera.Position[0], camera.Position[1], camera.Position[2]);
		glUniform1i(mod.programPrincipal.getLocation("position_i"), i);
		glUniform1i(mod.programPrincipal.getLocation("hayNormalMapping"), models[i].normalMap);

		glUniform1f(mod.programPrincipal.getLocation("currentLight"), currentLight);
		glUniform1f(mod.programPrincipal.getLocation("fRefract"), refraccion);

		glUniform1f(mod.programPrincipal.getLocation("pointConstant"), puntualC);
		glUniform1f(mod.programPrincipal.getLocation("pointLinear"), puntualL);
		glUniform1f(mod.programPrincipal.getLocation("pointQuadratic"), puntualQ);

		glUniform1f(mod.programPrincipal.getLocation("innerCut"), innerCut);
		glUniform1f(mod.programPrincipal.getLocation("outerCut"), outerCut);

		//Si estoy dibujando reflejados debo cambiar la posicion de la luz
		if (!flip) {
			glUniform3f(mod.programPrincipal.getLocation("dirLight.direction"), ejeXL, ejeYL, ejeZL);
			glUniform3f(mod.programPrincipal.getLocation("dirLight.position"), direccionLuz[0], direccionLuz[1], direccionLuz[2]);
		}
		else {
			glUniform3f(mod.programPrincipal.getLocation("dirLight.direction"), ejeXL, ejeYL, -ejeZL-63.5);
			glUniform3f(mod.programPrincipal.getLocation("dirLight.position"), direccionLuz[0], direccionLuz[1], -direccionLuz[2]);
		}

		glUniform3f(mod.programPrincipal.getLocation("dirLight.ambient"), color_luz_ambiental[0], color_luz_ambiental[1], color_luz_ambiental[2]);
		glUniform3f(mod.programPrincipal.getLocation("dirLight.diffuse"), color_luz_difusa[0], color_luz_difusa[1], color_luz_difusa[2]);
		glUniform3f(mod.programPrincipal.getLocation("dirLight.specular"), color_luz_especular[0], color_luz_especular[1], color_luz_especular[2]);

		glUniform3f(mod.programPrincipal.getLocation("model.ambient"), models[i].color_ambiental[0], models[i].color_ambiental[1], models[i].color_ambiental[2]);
		glUniform3f(mod.programPrincipal.getLocation("model.diffuse"), models[i].color_difuso[0], models[i].color_difuso[1], models[i].color_difuso[2]);
		glUniform3f(mod.programPrincipal.getLocation("model.specular"), models[i].color_especular[0], models[i].color_especular[1], models[i].color_especular[2]);
		glUniform1f(mod.programPrincipal.getLocation("model.biasM"), models[i].biasM);


		glUniform1i(mod.programPrincipal.getLocation("hayLuz"), apagarLuz);
		glUniform1i(mod.programPrincipal.getLocation("planar"), planar);
		glUniform1i(mod.programPrincipal.getLocation("skyboxText"), 2);
		glUniform1i(shadowDepth_loc, 3);

		//Matrices de view y proyeccion
		glm::mat4 model_mat = glm::mat4();
		glm::mat4 reflect_mat = glm::mat4();

		view_mat = camera.GetViewMatrix();	
		if(flip){
			reflect_mat = translate_en_matriz(models[i].ejeX, models[i].ejeY, -models[i].ejeZ-63.5);

			reflect_mat = reflect_mat * rotacion_en_matriz(-models[i].rotacion[0], -models[i].rotacion[1], models[i].rotacion[2], models[i].rotacion[3]);

			reflect_mat = reflect_mat * scale_en_matriz(models[i].scaleT);
			reflect_mat = glm::scale(reflect_mat, glm::vec3(1, 1, -1));

		}
		else {
			model_mat = translate_en_matriz(models[i].ejeX, models[i].ejeY, models[i].ejeZ);
			model_mat = model_mat * rotacion_en_matriz(models[i].rotacion[0], models[i].rotacion[1], models[i].rotacion[2], models[i].rotacion[3]);

			model_mat = model_mat * scale_en_matriz(models[i].scaleT);
		}

		glUniformMatrix4fv(reflect_matr_loc, 1, GL_FALSE, glm::value_ptr(reflect_mat));
		glUniformMatrix4fv(mod.programPrincipal.getLocation("shadowMat"), 1, GL_FALSE, glm::value_ptr(shadow_mat));
		glUniformMatrix4fv(model_matr_loc, 1, GL_FALSE, glm::value_ptr(model_mat));
		glUniformMatrix4fv(view_matr_loc, 1, GL_FALSE, glm::value_ptr(view_mat));
		glUniformMatrix4fv(mod.programPrincipal.getLocation("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(spaceLightMatrix));


		project_mat = glm::perspective(camera.Zoom, (float)GLFW_WIDTH / (float)GLFW_HEIGHT, 0.1f, 1000.0f);

		glUniformMatrix4fv(proj_matr_loc, 1, GL_FALSE, glm::value_ptr(project_mat));

			glEnable(GL_TEXTURE_2D);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);


			if (i == 0 && models[i].texturizado) {
				//Espejo
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_CUBE_MAP, sky.cubemapTexture);
			}
			else if (i == 1) {
				//Piso con normal mapping
				if (models[i].texturizado) {
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mod.texture2);
				}

				if (models[i].normalMap) {
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, mod.texture8);
				}

				glActiveTexture(GL_TEXTURE0);
			}
			else if (i == 2 && models[i].texturizado) {
				//Modelo con LightMap (Cubo)
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mod.texture3);

					//Trato el lightmap como si fuese una textura "normal" para no activar otra
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, mod.texture10);

				glActiveTexture(GL_TEXTURE0);

			}
			else if (i == 3 && models[i].texturizado) {
				//Modelo Refractivo
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_CUBE_MAP, sky.cubemapTexture);

				glActiveTexture(GL_TEXTURE0);

			}
			else if (i == 4 && models[i].texturizado) glBindTexture(GL_TEXTURE_2D, mod.texture5); //Modelo sin nada
			else if (i == 5 && models[i].texturizado) glBindTexture(GL_TEXTURE_2D, mod.texture6); //Modelo sin nada
			else if (i == 6) {
				//Arrow con normal mapping
				if (models[i].texturizado) {
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mod.texture7);
				}

				if (models[i].normalMap) {
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, mod.texture9);
				}

				glActiveTexture(GL_TEXTURE0);
			}

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, sha.depthMap);
			glActiveTexture(GL_TEXTURE0);

			glBindBuffer(GL_ARRAY_BUFFER, models[i].vbo);

			//VERTICE Y NORMALES
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[i].vertices.size() * sizeof(float)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[i].vertices.size() * sizeof(float) + (models[i].coord_texturas.size() * sizeof(float))));
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[i].vertices.size() * sizeof(float) + (models[i].coord_texturas.size() * sizeof(float) + models[i].normales_vertice_fin.size() * sizeof(float))));
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			if (!models[i].texturizado) glUniform1i(mod.programPrincipal.getLocation("hayTex"), selectedModel);
			else  glUniform1i(mod.programPrincipal.getLocation("hayTex"), -1);

			if (!planar || i!= 3) glDrawArrays(GL_TRIANGLES, 0, models[i].vertices.size() / 3);


			glDisableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glDisableVertexAttribArray(3);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			mod.programPrincipal.disable();
			glPushMatrix();
			if (!planar) {
			if (selecting && i == selectedModel && i != 7) {
				draw_bb(bool_loc);

				models[selectedModel].biasM = bias;

				models[selectedModel].color_ambiental[0] = color_mat_ambiental[0];
				models[selectedModel].color_ambiental[1] = color_mat_ambiental[1];
				models[selectedModel].color_ambiental[2] = color_mat_ambiental[2];

				models[selectedModel].color_difuso[0] = color_mat_difusa[0];
				models[selectedModel].color_difuso[1] = color_mat_difusa[1];
				models[selectedModel].color_difuso[2] = color_mat_difusa[2];

				models[selectedModel].color_especular[0] = color_mat_especular[0];
				models[selectedModel].color_especular[1] = color_mat_especular[1];
				models[selectedModel].color_especular[2] = color_mat_especular[2];
				models[selectedModel].scaleT = scaleT;

				models[selectedModel].ejeX = ejeX;
				models[selectedModel].ejeY = ejeY;

				if (ejeZ > -27.0) models[selectedModel].ejeZ = ejeZ;
				else ejeZ = models[selectedModel].ejeZ;

				models[selectedModel].rotacion[0] = rotacion_principal[0];
				models[selectedModel].rotacion[1] = rotacion_principal[1];
				models[selectedModel].rotacion[2] = rotacion_principal[2];
				models[selectedModel].rotacion[3] = rotacion_principal[3];
				models[selectedModel].normalMap = boolMapping;

			}

			if (i == 7) {
				if (selecting && selectedModel == 7) {
					draw_bb(bool_loc);
				}
				models[i].ejeX = ejeXL;
				models[i].ejeY = ejeYL;

				if (ejeZL > -27.0) models[i].ejeZ = ejeZL;
				else ejeZL = models[i].ejeZ;

				scaleT = models[selectedModel].scaleT;
			}

			glPopMatrix();
		}
	}
}

//Dibujo solo el piso (para sombras planares)
void drawGround(bool flip, bool planar) {
	if (innerCut < outerCut) innerCut = outerCut;
	//Saco matriz de luz
	glm::mat4 projectionLightMat, viewLightMat;
	glm::mat4 spaceLightMatrix, shadow_mat = glm::mat4();
	GLfloat near_plane, far_plane;
	near_plane = 1.0f, far_plane = 68.5f;
	if (currentLight == 3) {
		float fov = outerCut >= 1.0 ? 0.0 : 2.0 - (outerCut*incrementalValue);
		projectionLightMat = glm::perspective(fov, (GLfloat)sha.SHADOW_WIDTH / (GLfloat)sha.SHADOW_HEIGHT, 25.0f, 1000.0f);
		if (!flip) {
			glm::vec3 dirLSpot = glm::vec3(direccionLuz[0], direccionLuz[1], direccionLuz[2]);
			viewLightMat = glm::lookAt(glm::vec3(ejeXL, ejeYL, ejeZL), glm::vec3(ejeXL, ejeYL, ejeZL) + dirLSpot, glm::vec3(0.0, 1.0, 0.0));
		}
		else {
			glm::vec3 dirLSpot = glm::vec3(direccionLuz[0], direccionLuz[1], -direccionLuz[2]);
			viewLightMat = glm::lookAt(glm::vec3(ejeXL, ejeYL, -ejeZL - 63.5), glm::vec3(ejeXL, ejeYL, -ejeZL - 63.5) + dirLSpot, glm::vec3(0.0, 1.0, 0.0));
		}
	}
	else if (currentLight == 1) {

		projectionLightMat = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, near_plane, far_plane);

		if (!flip) viewLightMat = glm::lookAt(glm::vec3(ejeXL, ejeYL, ejeZL), glm::vec3(0.0, -22.0 + 23.64, -19.5), glm::vec3(0.0, 1.0, 0.0));
		else viewLightMat = glm::lookAt(glm::vec3(ejeXL, ejeYL, -ejeZL - 63.5), glm::vec3(0.0, -22.0 + 23.64, -43.35), glm::vec3(0.0, 1.0, 0.0));

	}
	else {
		//Es puntual hago sombras planares
		if (planar) {
			if (!flip) shadow_mat = sha.createShadowMatrix(glm::vec4(ejeXL, ejeYL, ejeZL, 1.0));
			else shadow_mat = sha.createShadowMatrix(glm::vec4(ejeXL, ejeYL, -ejeZL - 63.5, 1.0));
		}
	}

	spaceLightMatrix = projectionLightMat * viewLightMat;

		mod.programPrincipal.enable();
		GLuint view_matr_loc = mod.programPrincipal.getLocation("view_matrix");
		GLuint model_matr_loc = mod.programPrincipal.getLocation("model_matrix");
		GLuint reflect_matr_loc = mod.programPrincipal.getLocation("reflect_matrix");
		GLuint proj_matr_loc = mod.programPrincipal.getLocation("projection_matrix");
		GLuint view_loc = mod.programPrincipal.getLocation("view");

		GLuint bool_loc = mod.programPrincipal.getLocation("bb");
		GLuint tex_loc = mod.programPrincipal.getLocation("tex");
		GLuint normalMap_loc = mod.programPrincipal.getLocation("normalMap");
		GLuint shadowDepth_loc = mod.programPrincipal.getLocation("depthText");

		glUniform1i(tex_loc, 0);
		glUniform1i(normalMap_loc, 1);
		glUniform1i(bool_loc, 0);
		glUniform3f(view_loc, camera.Position[0], camera.Position[1], camera.Position[2]);
		glUniform1i(mod.programPrincipal.getLocation("position_i"), 1);
		glUniform1i(mod.programPrincipal.getLocation("hayNormalMapping"), models[1].normalMap);

		glUniform1f(mod.programPrincipal.getLocation("currentLight"), currentLight);
		glUniform1f(mod.programPrincipal.getLocation("fRefract"), refraccion);

		glUniform1f(mod.programPrincipal.getLocation("pointConstant"), puntualC);
		glUniform1f(mod.programPrincipal.getLocation("pointLinear"), puntualL);
		glUniform1f(mod.programPrincipal.getLocation("pointQuadratic"), puntualQ);

		glUniform1f(mod.programPrincipal.getLocation("innerCut"), innerCut);
		glUniform1f(mod.programPrincipal.getLocation("outerCut"), outerCut);

		//Si estoy dibujando reflejados debo cambiar la posicion de la luz
		if (!flip) {
			glUniform3f(mod.programPrincipal.getLocation("dirLight.direction"), ejeXL, ejeYL, ejeZL);
			glUniform3f(mod.programPrincipal.getLocation("dirLight.position"), direccionLuz[0], direccionLuz[1], direccionLuz[2]);
		}
		else {
			glUniform3f(mod.programPrincipal.getLocation("dirLight.direction"), ejeXL, ejeYL, -ejeZL - 63.5);
			glUniform3f(mod.programPrincipal.getLocation("dirLight.position"), direccionLuz[0], direccionLuz[1], -direccionLuz[2]);
		}

		glUniform3f(mod.programPrincipal.getLocation("dirLight.ambient"), color_luz_ambiental[0], color_luz_ambiental[1], color_luz_ambiental[2]);
		glUniform3f(mod.programPrincipal.getLocation("dirLight.diffuse"), color_luz_difusa[0], color_luz_difusa[1], color_luz_difusa[2]);
		glUniform3f(mod.programPrincipal.getLocation("dirLight.specular"), color_luz_especular[0], color_luz_especular[1], color_luz_especular[2]);

		glUniform3f(mod.programPrincipal.getLocation("model.ambient"), models[1].color_ambiental[0], models[1].color_ambiental[1], models[1].color_ambiental[2]);
		glUniform3f(mod.programPrincipal.getLocation("model.diffuse"), models[1].color_difuso[0], models[1].color_difuso[1], models[1].color_difuso[2]);
		glUniform3f(mod.programPrincipal.getLocation("model.specular"), models[1].color_especular[0], models[1].color_especular[1], models[1].color_especular[2]);
		glUniform1f(mod.programPrincipal.getLocation("model.biasM"), models[1].biasM);


		glUniform1i(mod.programPrincipal.getLocation("hayLuz"), apagarLuz);
		glUniform1i(mod.programPrincipal.getLocation("skyboxText"), 2);
		glUniform1i(shadowDepth_loc, 3);

		//Matrices de view y proyeccion
		glm::mat4 model_mat = glm::mat4();
		glm::mat4 reflect_mat = glm::mat4();

		view_mat = camera.GetViewMatrix();
		if (flip) {
			reflect_mat = translate_en_matriz(models[1].ejeX, models[1].ejeY, -models[1].ejeZ - 63.5);

			reflect_mat = reflect_mat * rotacion_en_matriz(-models[1].rotacion[0], -models[1].rotacion[1], models[1].rotacion[2], models[1].rotacion[3]);

			reflect_mat = reflect_mat * scale_en_matriz(models[1].scaleT);
			reflect_mat = glm::scale(reflect_mat, glm::vec3(1, 1, -1));

		}
		else {
			model_mat = translate_en_matriz(models[1].ejeX, models[1].ejeY, models[1].ejeZ);
			model_mat = model_mat * rotacion_en_matriz(models[1].rotacion[0], models[1].rotacion[1], models[1].rotacion[2], models[1].rotacion[3]);

			model_mat = model_mat * scale_en_matriz(models[1].scaleT);
		}

		glUniformMatrix4fv(reflect_matr_loc, 1, GL_FALSE, glm::value_ptr(reflect_mat));
		glUniformMatrix4fv(mod.programPrincipal.getLocation("shadowMat"), 1, GL_FALSE, glm::value_ptr(shadow_mat));
		glUniformMatrix4fv(model_matr_loc, 1, GL_FALSE, glm::value_ptr(model_mat));
		glUniformMatrix4fv(view_matr_loc, 1, GL_FALSE, glm::value_ptr(view_mat));
		glUniformMatrix4fv(mod.programPrincipal.getLocation("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(spaceLightMatrix));


		project_mat = glm::perspective(camera.Zoom, (float)GLFW_WIDTH / (float)GLFW_HEIGHT, 0.1f, 1000.0f);

		glUniformMatrix4fv(proj_matr_loc, 1, GL_FALSE, glm::value_ptr(project_mat));

		glEnable(GL_TEXTURE_2D);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);


		//Piso con normal mapping
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mod.texture2);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mod.texture8);

		glActiveTexture(GL_TEXTURE0);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, sha.depthMap);
		glActiveTexture(GL_TEXTURE0);


		glBindBuffer(GL_ARRAY_BUFFER, models[1].vbo);

		//VERTICE Y NORMALES
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[1].vertices.size() * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[1].vertices.size() * sizeof(float) + (models[1].coord_texturas.size() * sizeof(float))));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[1].vertices.size() * sizeof(float) + (models[1].coord_texturas.size() * sizeof(float) + models[1].normales_vertice_fin.size() * sizeof(float))));
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (!models[1].texturizado) glUniform1i(mod.programPrincipal.getLocation("hayTex"), selectedModel);
		else  glUniform1i(mod.programPrincipal.getLocation("hayTex"), -1);

		glDrawArrays(GL_TRIANGLES, 0, models[1].vertices.size() / 3);


		glDisableClientState(GL_VERTEX_ARRAY); glBindBuffer(GL_ARRAY_BUFFER, 0); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(0); glDisableVertexAttribArray(1); glDisableVertexAttribArray(2); glDisableVertexAttribArray(3);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		mod.programPrincipal.disable();
}

//Funcion para dibujar mirror y objetos reflejados
void draw_mirror(bool ground) {
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	glDepthMask(GL_FALSE);
	glClear(GL_STENCIL_BUFFER_BIT);

	//Se dibujan los espejos
	mod.programPrincipal.enable();

	GLuint view_matr_loc = mod.programPrincipal.getLocation("view_matrix");
	GLuint model_matr_loc = mod.programPrincipal.getLocation("model_matrix");
	GLuint reflect_matr_loc = mod.programPrincipal.getLocation("reflect_matrix");
	GLuint proj_matr_loc = mod.programPrincipal.getLocation("projection_matrix");
	GLuint view_loc = mod.programPrincipal.getLocation("view");

	GLuint bool_loc = mod.programPrincipal.getLocation("bb");
	GLuint tex_loc = mod.programPrincipal.getLocation("tex");
	GLuint normalMap_loc = mod.programPrincipal.getLocation("normalMap");

	glUniform1i(tex_loc, 0);
	glUniform1i(normalMap_loc, 1);
	glUniform1i(bool_loc, 0);
	glUniform3f(view_loc, camera.Position[0], camera.Position[1], camera.Position[2]);
	glUniform1i(mod.programPrincipal.getLocation("position_i"), 0);
	glUniform1i(mod.programPrincipal.getLocation("hayNormalMapping"), models[0].normalMap);

	glUniform1f(mod.programPrincipal.getLocation("currentLight"), currentLight);
	glUniform1f(mod.programPrincipal.getLocation("fRefract"), refraccion);

	glUniform1f(mod.programPrincipal.getLocation("pointConstant"), puntualC);
	glUniform1f(mod.programPrincipal.getLocation("pointLinear"), puntualL);
	glUniform1f(mod.programPrincipal.getLocation("pointQuadratic"), puntualQ);

	glUniform1f(mod.programPrincipal.getLocation("innerCut"), innerCut);
	glUniform1f(mod.programPrincipal.getLocation("outerCut"), outerCut);

	glUniform3f(mod.programPrincipal.getLocation("dirLight.direction"), ejeXL, ejeYL, ejeZL);
	glUniform3f(mod.programPrincipal.getLocation("dirLight.position"), direccionLuz[0], direccionLuz[1], direccionLuz[2]);
	glUniform3f(mod.programPrincipal.getLocation("dirLight.ambient"), color_luz_ambiental[0], color_luz_ambiental[1], color_luz_ambiental[2]);
	glUniform3f(mod.programPrincipal.getLocation("dirLight.diffuse"), color_luz_difusa[0], color_luz_difusa[1], color_luz_difusa[2]);
	glUniform3f(mod.programPrincipal.getLocation("dirLight.specular"), color_luz_especular[0], color_luz_especular[1], color_luz_especular[2]);

	glUniform3f(mod.programPrincipal.getLocation("model.ambient"), models[0].color_ambiental[0], models[0].color_ambiental[1], models[0].color_ambiental[2]);
	glUniform3f(mod.programPrincipal.getLocation("model.diffuse"), models[0].color_difuso[0], models[0].color_difuso[1], models[0].color_difuso[2]);
	glUniform3f(mod.programPrincipal.getLocation("model.specular"), models[0].color_especular[0], models[0].color_especular[1], models[0].color_especular[2]);
	glUniform1f(mod.programPrincipal.getLocation("model.biasM"), models[0].biasM);


	glUniform1i(mod.programPrincipal.getLocation("hayLuz"), apagarLuz);
	glUniform1i(mod.programPrincipal.getLocation("skyboxText"), 2);

	//Matrices de view y proyeccion
	glm::mat4 model_mat = glm::mat4();
	glm::mat4 reflect_mat = glm::mat4();
	glm::mat4 shadow_mat = glm::mat4();
	view_mat = camera.GetViewMatrix();
	model_mat = translate_en_matriz(models[0].ejeX, models[0].ejeY, models[0].ejeZ);
	model_mat = model_mat * rotacion_en_matriz(models[0].rotacion[0], models[0].rotacion[1], models[0].rotacion[2], models[0].rotacion[3]);

	model_mat = model_mat * scale_en_matriz(models[0].scaleT);


	glUniformMatrix4fv(reflect_matr_loc, 1, GL_FALSE, glm::value_ptr(reflect_mat));
	glUniformMatrix4fv(mod.programPrincipal.getLocation("shadowMat"), 1, GL_FALSE, glm::value_ptr(shadow_mat));
	glUniformMatrix4fv(model_matr_loc, 1, GL_FALSE, glm::value_ptr(model_mat));
	glUniformMatrix4fv(view_matr_loc, 1, GL_FALSE, glm::value_ptr(view_mat));

	project_mat = glm::perspective(camera.Zoom, (float)GLFW_WIDTH / (float)GLFW_HEIGHT, 0.1f, 1000.0f);

	glUniformMatrix4fv(proj_matr_loc, 1, GL_FALSE, glm::value_ptr(project_mat));

	glEnable(GL_TEXTURE_2D);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBindBuffer(GL_ARRAY_BUFFER, models[0].vbo);

	//VERTICE Y NORMALES
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[0].vertices.size() * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[0].vertices.size() * sizeof(float) + (models[0].coord_texturas.size() * sizeof(float))));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[0].vertices.size() * sizeof(float) + (models[0].coord_texturas.size() * sizeof(float) + models[0].normales_vertice_fin.size() * sizeof(float))));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[0].vertices.size() * sizeof(float) + (models[0].coord_texturas.size() * sizeof(float) + models[0].normales_vertice_fin.size() * sizeof(float) + models[0].tangents.size() * sizeof(glm::vec3))));
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (!models[0].texturizado) glUniform1i(mod.programPrincipal.getLocation("hayTex"), selectedModel);
	else  glUniform1i(mod.programPrincipal.getLocation("hayTex"), -1);

	glDrawArrays(GL_TRIANGLES, 0, models[0].vertices.size() / 3);


	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	mod.programPrincipal.disable();
	glPushMatrix();
	glPopMatrix();

	//Modelos reflejados
	glStencilFunc(GL_EQUAL, 1, 0xFF);	// Only draw model if stencil value has equal
	glStencilMask(0x00);
	glDepthMask(GL_TRUE);

	if (ground) {
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			drawGround(true, false); //Ground para sombras normales

		// Shadow planar con stencil buffer;
		glStencilFunc(GL_EQUAL, 1, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-1.0, -2.0);
			draw(true, true); 	// Dibujo toda la sombra de los modelos reflejados
		glDisable(GL_POLYGON_OFFSET_FILL);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
	}

	draw(true, false);
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0xFF);
}

//Funcion que actualiza el depthMap (desde el punto de vista de la luz)
void draw_shadow(bool flip) {
	glm::mat4 projectionLightMat, viewLightMat;
	glm::mat4 spaceLightMatrix;
	GLfloat near_plane, far_plane;
	near_plane = 1.0f, far_plane = 68.5f;


	if (currentLight == 3) {
		//Modifico el valor incremental si se modifico el outerCos (para evitar cambio en depthmap brusco)
		if (beforeOuter != outerCut) {
			incrementalValue = incrementalValue + ((outerCut - beforeOuter) * 3);
			beforeOuter = outerCut;
		}
		float fov = outerCut >= 1.0 ? 0.0 : 2.0 - (outerCut*incrementalValue);
		projectionLightMat = glm::perspective(fov, (GLfloat)sha.SHADOW_WIDTH / (GLfloat)sha.SHADOW_HEIGHT, 25.0f, 1000.0f);
		if (!flip) {
			glm::vec3 dirLSpot = glm::vec3(direccionLuz[0], direccionLuz[1], direccionLuz[2]);
			viewLightMat = glm::lookAt(glm::vec3(ejeXL, ejeYL, ejeZL), glm::vec3(ejeXL, ejeYL, ejeZL) + dirLSpot, glm::vec3(0.0, 1.0, 0.0));
		}
		else {
			glm::vec3 dirLSpot = glm::vec3(direccionLuz[0], direccionLuz[1], -direccionLuz[2]);
			viewLightMat = glm::lookAt(glm::vec3(ejeXL, ejeYL, -ejeZL - 63.5), glm::vec3(ejeXL, ejeYL, -ejeZL - 63.5) + dirLSpot, glm::vec3(0.0, 1.0, 0.0));
		}
	} else if(currentLight == 1){
		projectionLightMat = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, near_plane, far_plane);

		if (!flip) viewLightMat = glm::lookAt(glm::vec3(ejeXL, ejeYL, ejeZL), glm::vec3(0.0, -22.0 + 23.64 , -19.5), glm::vec3(0.0, 1.0, 0.0));
		else viewLightMat = glm::lookAt(glm::vec3(ejeXL, ejeYL, -ejeZL - 63.5), glm::vec3(0.0, -22.0 + 23.64 , -43.35), glm::vec3(0.0, 1.0, 0.0));

	}
	spaceLightMatrix = projectionLightMat * viewLightMat;
	
	sha.programShadowBox.enable();
	glUniformMatrix4fv(sha.programShadowBox.getLocation("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(spaceLightMatrix));
	glViewport(0, 0, sha.SHADOW_WIDTH, sha.SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, sha.depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	for (int i = 1; i < models.size()-1; i++) { //Para los modelos quitando el espejo
		if (i != 3) {
			GLuint model_matr_loc = sha.programShadowBox.getLocation("model");
			GLuint reflect_matr_loc = sha.programShadowBox.getLocation("reflect_matrix");
			//Matrices de model, view y projection de modelos
			glm::mat4 model_mat = glm::mat4();
			glm::mat4 reflect_mat = glm::mat4();
			if (flip) {
				reflect_mat = translate_en_matriz(models[i].ejeX, models[i].ejeY, -models[i].ejeZ - 63.5);

				reflect_mat = reflect_mat * rotacion_en_matriz(-models[i].rotacion[0], -models[i].rotacion[1], models[i].rotacion[2], models[i].rotacion[3]);

				reflect_mat = reflect_mat * scale_en_matriz(models[i].scaleT);
				reflect_mat = glm::scale(reflect_mat, glm::vec3(1, 1, -1));

			}
			else {
				model_mat = translate_en_matriz(models[i].ejeX, models[i].ejeY, models[i].ejeZ);
				model_mat = model_mat * rotacion_en_matriz(models[i].rotacion[0], models[i].rotacion[1], models[i].rotacion[2], models[i].rotacion[3]);

				model_mat = model_mat * scale_en_matriz(models[i].scaleT);
			}

			glUniformMatrix4fv(reflect_matr_loc, 1, GL_FALSE, glm::value_ptr(reflect_mat));
			glUniformMatrix4fv(model_matr_loc, 1, GL_FALSE, glm::value_ptr(model_mat));
			glBindBuffer(GL_ARRAY_BUFFER, models[i].vbo);

			//VERTICE Y NORMALES
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[i].vertices.size() * sizeof(float)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[i].vertices.size() * sizeof(float) + (models[i].coord_texturas.size() * sizeof(float))));
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[i].vertices.size() * sizeof(float) + (models[i].coord_texturas.size() * sizeof(float) + models[i].normales_vertice_fin.size() * sizeof(float))));
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0, models[i].vertices.size() / 3);


			glDisableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glDisableVertexAttribArray(3);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

	}
	sha.programShadowBox.disable();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//Funcion que dibuja el skybox
void draw_skybox() {
	glDepthMask(GL_FALSE);
	sky.programSkyBox.enable();

	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	glm::mat4 projection = glm::perspective(45.0f, (float)GLFW_WIDTH / (float)GLFW_HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(sky.programSkyBox.getLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv((sky.programSkyBox.getLocation("projection")), 1, GL_FALSE, glm::value_ptr(project_mat));
	// Cubo del Skybox
	glBindVertexArray(sky.skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(sky.programSkyBox.getLocation("skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sky.cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	sky.programSkyBox.disable();
}

//Funcion que mueve la camara con WASD
void moverme() {
	bool una_vez = true;
	if ((keys['w'] || keys['W']) && una_vez) {
		//Muevo hacia delante
		camera.ProcessKeyboard(FORWARD, deltaTime);
		una_vez = false;
	}
	if ((keys['s'] || keys['S']) && una_vez) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		una_vez = false;
	}
	if ((keys['a'] || keys['A']) && una_vez) {
		camera.ProcessKeyboard(LEFT, deltaTime);
		una_vez = false;
	}
	if ((keys['d'] || keys['D']) && una_vez) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
		una_vez = false;
	}
}

//Funcion de display (llama a los draws)
void display() {
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame; 
	lastFrame = currentFrame;
	glfwPollEvents();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	draw_skybox(); 	// Dibujo skybox de primero
	
	if (currentLight == 3 || currentLight == 1) {
		//Activo y desactivo el culling para el peter panning
		glCullFace(GL_FRONT);
			draw_shadow(true); //Sombra de modelos reflejados
		glCullFace(GL_BACK);

		glViewport(0, 0, GLFW_WIDTH, GLFW_HEIGHT); //Cambio el viewport al original


		draw_mirror(false); //Dibujo el espejo y los objetos reflejados
		glClear(GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_FRONT);
			draw_shadow(false); //Dibujo sombra de modelos normales
		glCullFace(GL_BACK);

		glViewport(0, 0, GLFW_WIDTH, GLFW_HEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glClear(GL_STENCIL_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glClear(GL_STENCIL_BUFFER_BIT);
			draw(false, false); 	// Dibujo todos los modelos normales
		glDisable(GL_STENCIL_TEST);

		if (hayDepthMap && currentLight != 2) sha.drawShadow(); //Dibujo el quad con la textura dephMap
	}
	else if (currentLight == 2) {
		draw_mirror(true); //Dibujo los espejos y objetos reflejados
		glClear(GL_DEPTH_BUFFER_BIT);
	
		glClear(GL_STENCIL_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glClear(GL_STENCIL_BUFFER_BIT);
			draw(false, false); 	// Dibujo todos los modelos normales
		glDisable(GL_STENCIL_TEST);

		// Ground plane
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			drawGround(false, false); //Ground para sombras normales
		glDisable(GL_STENCIL_TEST);

		// Shadow con stencil buffer
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_EQUAL, 1, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-1.0, -2.0);
			draw(false, true); 	// Dibujo toda la sombra de los modelos
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_STENCIL_TEST);
	}

	moverme();
	TwDraw();

}


//<-----------------FUNCION DE MAIN--------------------->
int main(int argc, char* argv[]) {
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);


	GLFWwindow *window = glfwCreateWindow(GLFW_WIDTH, GLFW_HEIGHT, "Tarea 2 FCG - Alejandro Barone", nullptr, nullptr);
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
		
	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, inter.reshape);
	glfwSetKeyCallback(window, inter.keyFunc);
	glfwSetMouseButtonCallback(window, inter.motionFunc);
	glfwSetCursorPosCallback(window, inter.passiveMotionFunc);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCharCallback(window, (GLFWcharfun)inter.obtenerTecla);
	//glfwSetScrollCallback(window, scroll); SERIA EXTRA (ESCALAR CON SCROLL?)

	inter.initAntTweakParam();

	initGlew();
	glewExperimental = GL_TRUE;
	
	if (GLEW_OK != glewInit()) {
		cout << "ERROR AL INICIALIZAR GLEW" << endl;
		return EXIT_FAILURE;
	}
	mod.loadShaderPrincipal();
	sky.loadShaderSkyBox(); //cargo shader skybox
	sky.initSkybox(); //inicializado el skybox
	sha.loadShadowShader(); //cargo shader de sombra
	sha.loadDepthShader(); //Cargo shader de profundidad

	//Todos los objetos
	read_obj("Modelos/obj/wall.obj"); //Espejo

	read_obj("Modelos/obj/floor.obj"); //Modelo Normal Mapping 1
	read_obj("Modelos/obj/cube.obj"); //LightMaps Model
	read_obj("Modelos/obj/bad_skull.obj"); //Modelo Refractivo

	read_obj("Modelos/obj/cy.obj"); //Extra 2
	

	read_obj("Modelos/obj/sphere.obj"); //Sombras

	read_obj("Modelos/obj/arrow.obj"); //Modelo Normal Mapping 2

	read_obj("Modelos/obj/MarsPlanet.obj");  //Modelo de luz

	//Habilitamos el alfa de los colores
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	inter.reshape(window, GLFW_WIDTH, GLFW_HEIGHT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	sha.initShadow(); // inicializo shader de sombras
	while (!glfwWindowShouldClose(window)) {
		display();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}