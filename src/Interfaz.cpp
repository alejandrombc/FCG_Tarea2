#include "Interfaz.h"
#include "Modelos.h"

extern Camera camera;
extern vector<Modelos> models;
extern GLint GLFW_WIDTH, GLFW_HEIGHT;

extern glm::mat4 project_mat; //Matriz de Proyeccion
extern glm::mat4 view_mat; //Matriz de View
extern glm::vec3 eye; // Ojo

extern lightT currentLight;

extern int selectedModel; extern int tiempoRotacion; extern int apagarLuz;
extern float scaleT, ejeX, ejeY, ejeZ, ejeXL, ejeYL, ejeZL,
puntualC, puntualL, puntualQ, innerCut, outerCut, refraccion, bias;
extern float rotacion[];
extern float color_luz_ambiental[]; extern float color_mat_ambiental[3];
extern float color_luz_difusa[]; extern float color_mat_difusa[3];
extern float color_luz_especular[]; extern float color_mat_especular[3];
extern float rotacion_principal[];
extern float direccionLuz[];
extern float rotacionInicial[];
extern bool selecting, MENU_TRY, boolTexturas, boolMapping, hayDepthMap;
extern bool keys[1024]; //Todas las teclas

extern GLfloat lastX, lastY;
extern GLfloat deltaTime;
extern bool firstMouse;
extern GLfloat lastFrame;

//<-----------------FUNCIONES DE ANT TWEAK BAR--------------------->

										//Funcion de salir
void TW_CALL exit(void *clientData) {
	exit(1);
}


//Funcion para las texturas (aparecer y desaparecer)
void TW_CALL activateTex(void *clientData) {
	boolTexturas = !boolTexturas;
	models[selectedModel].texturizado = boolTexturas;
}

void TW_CALL activateNormalMap(void *clientData) {
	boolMapping = !boolMapping;
	models[selectedModel].normalMap = boolMapping;
}


//Funcion para editar luz
void TW_CALL editarL(void *clientData) {

	if (currentLight == 2) {
		TwDefine("Sol visible=false");
		TwDefine("EditLPuntual visible=true");
	}
	else if (currentLight == 3) {
		TwDefine("Sol visible=false");
		TwDefine("EditLReflector visible=true");
	}
}

//Funcion para volver menu
void TW_CALL volver(void *clientData) {
	TwDefine("Figura visible=false");
	TwDefine("Sol visible=false");
	selecting = false;
	selectedModel = 0;

	TwDefine("Menu visible=true");
}

//Funcion para volver a Figura
void TW_CALL volverF(void *clientData) {
	TwDefine("Figura visible=false");
	TwDefine("Sol visible=false");

	TwDefine("Figura visible=true");
}



//Funcion para volver menu SOL
void TW_CALL volverL(void *clientData) {
	TwDefine("Figura visible=false");
	TwDefine("EditLReflector visible=false");
	TwDefine("EditLPuntual visible=false");

	TwDefine("Sol visible=true");
}


//Activar o desactivar el depth map
void TW_CALL setDepth(const void *value, void *clientData) {
	hayDepthMap = *(const int *)value;
}

void TW_CALL getDepth(void *value, void *clientData) {
	(void)clientData;
	*(int *)value = hayDepthMap;
}


//Sets y gets de apagar luz
void TW_CALL SetAutoApagar(const void *value, void *clientData) {
	apagarLuz = *(const int *)value;
}

void TW_CALL GetAutoApagar(void *value, void *clientData) {
	(void)clientData; // unused
	*(int *)value = apagarLuz;
}


//Chequear si el click le dio a un objeto
void Interfaz::checkSelectedObject(GLint i) {
	if (MENU_TRY) {
		TwDefine("EditLPuntual visible=false");
		TwDefine("EditLReflector visible=false");

		if (i != selectedModel && selecting) {
			selecting = true;
			TwDefine("Menu visible=false");
			TwDefine("Figura visible=false");
			TwDefine("Sol visible=false");
		}
		else selecting = !selecting;
		selectedModel = i;

		if (selecting && i != 7) {
			TwDefine("Menu visible=false");
			TwDefine("Figura visible=true");
			TwDefine("Sol visible=false");


			selecting = true;
			rotacion_principal[0] = models[selectedModel].rotacion[0];
			rotacion_principal[1] = models[selectedModel].rotacion[1];
			rotacion_principal[2] = models[selectedModel].rotacion[2];
			rotacion_principal[3] = models[selectedModel].rotacion[3];
			scaleT = models[selectedModel].scaleT;
			ejeX = models[selectedModel].ejeX;
			ejeY = models[selectedModel].ejeY;
			ejeZ = models[selectedModel].ejeZ;
			bias = models[selectedModel].biasM;
			boolTexturas = models[selectedModel].texturizado;

			color_mat_ambiental[0] = models[selectedModel].color_ambiental[0];
			color_mat_ambiental[1] = models[selectedModel].color_ambiental[1];
			color_mat_ambiental[2] = models[selectedModel].color_ambiental[2];

			color_mat_difusa[0] = models[selectedModel].color_difuso[0];
			color_mat_difusa[1] = models[selectedModel].color_difuso[1];
			color_mat_difusa[2] = models[selectedModel].color_difuso[2];

			color_mat_especular[0] = models[selectedModel].color_especular[0];
			color_mat_especular[1] = models[selectedModel].color_especular[1];
			color_mat_especular[2] = models[selectedModel].color_especular[2];

			if (selectedModel != 6) TwDefine("Figura/NormalMap visible=false");
			else TwDefine("Figura/NormalMap visible=true");
		}
		else if (selecting && i == 7) {
			TwDefine("Menu visible=false");
			TwDefine("Figura visible=false");
			TwDefine("Sol visible=true");
			selecting = true;

			ejeXL = models[7].ejeX;
			ejeYL = models[7].ejeY;
			ejeZL = models[7].ejeZ;


		}
		else {
			TwDefine("Menu visible=true");
			TwDefine("Figura visible=false");
			TwDefine("Sol visible=false");
			if (selectedModel != 7) {
				models[selectedModel].scaleT = scaleT;
				boolTexturas = models[selectedModel].texturizado;

				models[selectedModel].ejeX = ejeX;
				models[selectedModel].ejeY = ejeY;
				models[selectedModel].ejeZ = ejeZ;
			}
			else {
				models[selectedModel].ejeX = ejeXL;
				models[selectedModel].ejeY = ejeYL;
				models[selectedModel].ejeZ = ejeZL;
			}
			selecting = false;
		}
	}
}

//Funcion de reshape
void Interfaz::reshape(GLFWwindow *window, int width, int height) {
	width = max(width, 1); height = max(height, 1);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLFW_HEIGHT = height;
	GLFW_WIDTH = width;

	project_mat = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	gluPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);

	glm::mat4 model_mat;
	glm::vec3 norm(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	gluLookAt(eye[0], eye[1], eye[2], norm[0], norm[1], norm[2], up[0], up[1], up[2]);

	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.5, 0.5, 0.5, 0);
	glColor3f(1.0, 1.0, 1.0);
	TwWindowSize(width, height);
}

//Funcion de motionfunc (mueve la camara con el mouse)
void Interfaz::motionFunc(GLFWwindow* window, int button, int action, int mods) {
	if (MENU_TRY) {
		if (TwEventMouseButtonGLFW(button, action)) {
			return;
		}
		else {
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				double mouse_x, mouse_y;
				glfwGetCursorPos(window, &mouse_x, &mouse_y);
				
				GLint indice_modelo;
				glReadPixels(mouse_x, GLFW_HEIGHT - mouse_y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &indice_modelo);

				if (indice_modelo > 1) checkSelectedObject(indice_modelo);
			}
		}

	}
	else {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		if (firstMouse)
		{
			lastX = x;
			lastY = y;
			firstMouse = false;
		}

		GLfloat xoffset = x - lastX;
		GLfloat yoffset = lastY - y;  // Reversed since y-coordinates go from bottom to left

		lastX = x;
		lastY = y;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}

}

//Funcion de MotionPassive del mouse
void Interfaz::passiveMotionFunc(GLFWwindow* window, double x, double y) {
	if (MENU_TRY) {
		if (TwEventMousePosGLFW(x, y)) return;
	}
	else {
		if (firstMouse) {
			lastX = x;
			lastY = y;
			firstMouse = false;
		}

		if (x < 100) {
			glfwSetCursorPos(window, GLFW_WIDTH - 100, y);
			lastX = GLFW_WIDTH - 100;
		}
		else if (x > GLFW_WIDTH - 100) {
			glfwSetCursorPos(window, 100, y);
			lastX = 100;
		}

		if (y < 100) {
			glfwSetCursorPos(window, x, GLFW_HEIGHT - 100);
			lastY = GLFW_HEIGHT - 100;
		}
		else if (y > GLFW_HEIGHT - 100) {
			glfwSetCursorPos(window, x, 100);
			lastY = 100;
		}

		GLfloat xoffset = x - lastX;
		GLfloat yoffset = lastY - y;  // Reversed since y-coordinates go from bottom to left

		yoffset = (yoffset < 100 && yoffset > -100) ? yoffset : 0;
		xoffset = (xoffset < 100 && xoffset > -100) ? xoffset : 0;

		lastX = x;
		lastY = y;
		camera.ProcessMouseMovement(xoffset, yoffset, true);
	}
}

//Teclado (escape para cerrar el juego)
void Interfaz::keyFunc(GLFWwindow* window, int Key, int iScancode, int iAction, int iMods) {
	if ((iAction == GLFW_PRESS && Key == GLFW_KEY_M)) {
		MENU_TRY = !MENU_TRY;
		if (selecting) {
			if(selectedModel != 7) TwDefine("Figura visible=true");
			else TwDefine("Sol visible=true");
		}
		else TwDefine("Menu visible=true");

		keys['W'] = false; keys['w'] = false;
		keys['S'] = false; keys['a'] = false;
		keys['A'] = false; keys['s'] = false;
		keys['D'] = false; keys['d'] = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (TwEventKeyGLFW(Key, iAction)) return;
	}
	else if (!MENU_TRY) {
		TwDefine("Figura visible=false");
		TwDefine("Menu visible=false");
		TwDefine("EditLReflector visible=false");
		TwDefine("EditLPuntual visible=false");
		TwDefine("Sol visible=false");
		MENU_TRY = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	if (iAction == GLFW_PRESS) keys[Key] = true;
	if (iAction == GLFW_RELEASE) keys[Key] = false;
}

//Para poner numeros en AntTweakBar
void Interfaz::obtenerTecla(GLFWwindow* window, int codepoint) {
	TwEventCharGLFW(codepoint, GLFW_PRESS);
}

//Iniciar el menu de Anttweakbar
void Interfaz::initAntTweakParam() {
	//Ant Tweak Bar
	TwBar *menuTW, *modelTW;

	TwInit(TW_OPENGL, NULL);

	menuTW = TwNewBar("Menu");
	TwWindowSize(800, 600);
	//Botones del menu inicial en AntTweakBar
	TwDefine(" GLOBAL help='Tarea 1 FCG - Alejandro Barone' "); // Mensaje de ventana
	TwDefine(" Menu size='200 100' color='0 0 0' "); // Color y tamano de tweak bar
	TwAddVarRW(menuTW, "refraccion", TW_TYPE_FLOAT, &refraccion, "min=0.001 step=0.001 max=1.000 label='Factor Refraccion'");
	TwAddVarCB(menuTW, "DepthMap", TW_TYPE_BOOL32, setDepth, getDepth, NULL, " label='Depth Map'");
	TwAddButton(menuTW, "exit", exit, NULL, " label='Salir' key=Esc");


	//Figura
	modelTW = TwNewBar("Figura");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwDefine("Figura visible=false size='270 500' position='20 20' color='128 0 0' label='Modelo principal'");

	TwAddVarRW(modelTW, "scale", TW_TYPE_FLOAT, &scaleT, "min=0.01 step=0.01 label='Escalar'");
	TwAddVarRW(modelTW, "ejeX", TW_TYPE_FLOAT, &ejeX, "step=0.01 label='x' group='Trasladar'");
	TwAddVarRW(modelTW, "ejeY", TW_TYPE_FLOAT, &ejeY, "step=0.01 label='y' group='Trasladar'");
	TwAddVarRW(modelTW, "ejeZ", TW_TYPE_FLOAT, &ejeZ, "step=0.01 label='z' group='Trasladar'");
	TwAddVarRW(modelTW, "rotation", TW_TYPE_QUAT4F, &rotacion_principal, " label='Rotación' opened=true group='Rotar'");
	TwAddButton(modelTW, "activateTex", activateTex, NULL, " label='Activar' group='Texturas'");
	TwAddVarRW(modelTW, "texturizado", TW_TYPE_BOOL8, &boolTexturas, "label='Texturas' readonly=true group='Texturas'");
	TwAddButton(modelTW, "activateNormalMap", activateNormalMap, NULL, " label='Activar' group='NormalMap'");
	TwAddVarRW(modelTW, "biasMR", TW_TYPE_FLOAT, &bias, "min=0.001 step=0.001 max=1.000 label='Bias'");
	TwAddVarRW(modelTW, "normalMap", TW_TYPE_BOOL8, &boolMapping, "label='Normal Mapping' readonly=true group='NormalMap'");
	TwAddVarRW(modelTW, "Color Material Ambiental", TW_TYPE_COLOR3F, &color_mat_ambiental, "label='Ambiental' group='Material'");
	TwAddVarRW(modelTW, "Color Material Difuso", TW_TYPE_COLOR3F, &color_mat_difusa, "label='Difuso' group='Material'");
	TwAddVarRW(modelTW, "Color Material Especular", TW_TYPE_COLOR3F, &color_mat_especular, "label='Especular' group='Material'");

	TwAddButton(modelTW, "Volver", volver, NULL, " label='Volver' key=Esc");


	//Menu de Luz
	modelTW = TwNewBar("Sol");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwDefine("Sol visible=false size='270 300' position='20 20' color='128 0 0' label='Iluminacion'");
	TwAddVarRW(modelTW, "ejeXL", TW_TYPE_FLOAT, &ejeXL, "step=0.01 label='x' group='Trasladar luz' group='Luz'");
	TwAddVarRW(modelTW, "ejeYL", TW_TYPE_FLOAT, &ejeYL, "step=0.01 label='y' group='Trasladar luz' group='Luz'");
	TwAddVarRW(modelTW, "ejeZL", TW_TYPE_FLOAT, &ejeZL, "step=0.01 label='z' group='Trasladar luz' group='Luz'");

	//Tipo de luz (Direccional, Puntual, Reflector)
	{
		TwEnumVal styleLight[3] = { { Direccional, "Direccional" },{ Puntual, "Puntual" },{ Reflector, "Reflector" } };
		TwType lighttype = TwDefineEnum("LightType", styleLight, 3);
		TwAddVarRW(modelTW, "Tipo de luz", lighttype, &currentLight, "group='Luz'");
		TwAddButton(modelTW, "EditarR", editarL, NULL, " label='Editar' key=Esc group='Luz'");
	}
	TwAddVarCB(modelTW, "autoApagar", TW_TYPE_BOOL32, SetAutoApagar, GetAutoApagar, NULL, "label='Prender/Apagar luz'");
	//Color del material, eambiental, difuso y especular
	TwAddVarRW(modelTW, "Color Luz Ambiental", TW_TYPE_COLOR3F, &color_luz_ambiental, "label='Ambiental'");
	TwAddVarRW(modelTW, "Color Luz Difuso", TW_TYPE_COLOR3F, &color_luz_difusa, "label='Difuso'");
	TwAddVarRW(modelTW, "Color Luz Especular", TW_TYPE_COLOR3F, &color_luz_especular, "label='Especular'");
	TwAddButton(modelTW, "Volver", volver, NULL, " label='Volver' key=Esc");

	//Editar Luz Puntual
	modelTW = TwNewBar("EditLPuntual");
	TwDefine("EditLPuntual visible=false size='200 150' position='20 20' color='128 0 0' label='Editar parametros Luz Puntual'");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwAddVarRW(modelTW, "puntualC", TW_TYPE_FLOAT, &puntualC, "min=0.01 step=0.01 max=1.0 label='Constante'");
	TwAddVarRW(modelTW, "puntualL", TW_TYPE_FLOAT, &puntualL, "min=0.001 step=0.001 max=1.0 label='Lineal'");
	TwAddVarRW(modelTW, "puntualQ", TW_TYPE_FLOAT, &puntualQ, "min=0.0001 step=0.0001 max=2.0 label='Cuadratica'");
	TwAddButton(modelTW, "Volver", volverL, NULL, " label='Volver' key=Esc");

	//Editar Luz Reflector
	modelTW = TwNewBar("EditLReflector");
	TwDefine("EditLReflector visible=false size='250 300' position='20 20' color='128 0 0' label='Editar parametros Luz Reflector'");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwAddVarRW(modelTW, "puntualC", TW_TYPE_FLOAT, &puntualC, "min=0.01 step=0.01 max=1.0 label='Constante'");
	TwAddVarRW(modelTW, "puntualL", TW_TYPE_FLOAT, &puntualL, "min=0.001 step=0.001 max=1.0 label='Lineal'");
	TwAddVarRW(modelTW, "puntualQ", TW_TYPE_FLOAT, &puntualQ, "min=0.0001 step=0.0001 max=2.0 label='Cuadratica'");
	TwAddVarRW(modelTW, "innerCut", TW_TYPE_FLOAT, &innerCut, "min=0.550 step=0.001 max=1.0 label='In coseno'");
	TwAddVarRW(modelTW, "outerCut", TW_TYPE_FLOAT, &outerCut, "min=0.550 step=0.001 max=1.0 label='Out coseno'");
	TwAddVarRW(modelTW, "LightDir", TW_TYPE_DIR3F, &direccionLuz,
		" label='Direccion spotlight' opened=true help='Cambiar direccion luz' ");
	TwAddButton(modelTW, "Volver", volverL, NULL, " label='Volver' key=Esc");

	//Como empiezo en modo libre dejo toda vision en falso
	TwDefine("Figura visible=false");
	TwDefine("Menu visible=false");
	TwDefine("EditLReflector visible=false");
	TwDefine("EditLPuntual visible=false");
	TwDefine("Sol visible=false");
}



