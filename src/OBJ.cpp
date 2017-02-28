#include "OBJ.h"
#include "Modelos.h"

extern Modelos mod;
extern vector<Modelos> models; //Todos los modelos iran en este vector

//Para separar un string
vector<string> split(const string &s, char delim) {
	stringstream ss(s);
	string item;
	vector<string> tokens;
	while (getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;

}

							   
//Lector de OBJ (con normales + posicion de modelos en el mapa + textura)
void read_obj(char *filename) {
	vector<vertice> auxVertices_1, auxVertices_2;
	vector<vertice> coord_texturasAux;
	char line[4096] = ""; //Leo 4096 char en una linea
	ifstream file;
	bool centrado = true;
	file.open(filename);
	string comprobacion;
	double x, y, z, minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX, maxX = INT_MIN, maxY = INT_MIN, maxZ = INT_MIN;
	vertice v_secundario, normal;
	Modelos m;
	vector<face> faces;
	while (!file.eof()) { //Hasta terminar el archivo
		file >> comprobacion; // Leo el primer string (para ver si es vn, vt, v o f)
		if (comprobacion == "v") {
			//Como es v leo los x y z pertinentes, agrego al vector y sumo un vertice
			file >> x >> y >> z;
			if (x>maxX) maxX = x; if (y>maxY) maxY = y; if (z>maxZ) maxZ = z;
			if (x<minX) minX = x; if (y<minY) minY = y; if (z<minZ) minZ = z;
			v_secundario.x = x;
			v_secundario.y = y;
			v_secundario.z = z;
			auxVertices_1.push_back(v_secundario);
		}
		else if (comprobacion == "vt") {
			//Obtengo las coordenadas de texturas
			file >> x >> y;
			vertice a;
			a.x = x;
			a.y = y;
			coord_texturasAux.push_back(a);
		}
		else if (comprobacion == "f") { //Si es f leo la cara                     
										//Obtengo la cara
			face cara;
			int nFacesAux = 0;
			file.getline(line, 4096); //Obtengo toda la linea (sin el "f")
			vector<string> indices_vertices;
			string f_auxiliar = "";
			string auxiliar = line;
			auxiliar.erase(auxiliar.find_last_not_of(" ") + 1); //Borro espacios al final de la linea
			stringstream ss(auxiliar);
			while (!ss.eof()) { //Recorro la linea que agarre hasta el final
				ss >> f_auxiliar; //Leo strings
				if (f_auxiliar != "\0") {
					indices_vertices = split(f_auxiliar, '/'); //separo el string por "/"
					nFacesAux++;
					const char * c = indices_vertices[0].c_str(); //Siempre el [0] sera el vertice, lo transformo a int
					int real_index = atoi(c) - 1;
					cara.f.push_back(real_index);
					c = indices_vertices[1].c_str(); //Siempre el [1] sera la textura, lo transformo a int
					real_index = atoi(c) - 1;
					cara.t.push_back(real_index);
					auxiliar = f_auxiliar;
				}
			}
			//Termine esa cara, pusheo al vector de caras
			if (nFacesAux != 0) {
				cara.n_vertex = nFacesAux;
				faces.push_back(cara);
			}

		}
		else if ((comprobacion[0] != 'v' && comprobacion[0] != 'f')) {
			//Si es vn salto la linea
			file.getline(line, 4096);
		}
	}
	//YA LEI EL MODELO, TOCA TRIANGULAR Y ESO
	//Saco los vertices centrales del modelo
	double cx = (maxX + minX) / 2.0; double cy = (maxY + minY) / 2.0; double cz = (maxZ + minZ) / 2.0;
	vertice v, nor;
	double maxXYZ = max(maxX, max(maxY, maxZ));
	minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX, maxX = INT_MIN, maxY = INT_MIN, maxZ = INT_MIN;

	vector<int> totales;
	vector<vertice> normales_pavo;

	for (int i = 0; i < auxVertices_1.size(); i++) {

		v.x = ((auxVertices_1[i].x - cx) / maxXYZ);
		v.y = (auxVertices_1[i].y - cy) / maxXYZ;
		v.z = (auxVertices_1[i].z - cz) / maxXYZ;
		if (v.x>maxX) maxX = v.x; if (v.y>maxY) maxY = v.y; if (v.z>maxZ) maxZ = v.z;
		if (v.x<minX) minX = v.x; if (v.y<minY) minY = v.y; if (v.z<minZ) minZ = v.z;

		auxVertices_2.push_back(v);

		nor.x = 0;
		nor.y = 0;
		nor.z = 0;
		normales_pavo.push_back(nor);

		totales.push_back(0);
	}

	for (int i = 0; i < faces.size(); i++) {
		int v1, v2, v3, t1, t2, t3, aux = 1, k = 0, aux2 = 1;
		vertice u, v;

		while (faces[i].n_vertex - 1 > aux) {
			v1 = faces[i].f[k];
			v2 = faces[i].f[aux];
			v3 = faces[i].f[++aux];

			m.vertices.push_back(auxVertices_2[v1].x);
			m.vertices.push_back(auxVertices_2[v1].y);
			m.vertices.push_back(auxVertices_2[v1].z);

			m.vertices.push_back(auxVertices_2[v2].x);
			m.vertices.push_back(auxVertices_2[v2].y);
			m.vertices.push_back(auxVertices_2[v2].z);

			m.vertices.push_back(auxVertices_2[v3].x);
			m.vertices.push_back(auxVertices_2[v3].y);
			m.vertices.push_back(auxVertices_2[v3].z);

			t1 = faces[i].t[k];
			t2 = faces[i].t[aux2];
			t3 = faces[i].t[++aux2];

			m.coord_texturas.push_back(coord_texturasAux[t1].x);
			m.coord_texturas.push_back(coord_texturasAux[t1].y);
			m.coord_texturas.push_back(coord_texturasAux[t2].x);
			m.coord_texturas.push_back(coord_texturasAux[t2].y);
			m.coord_texturas.push_back(coord_texturasAux[t3].x);
			m.coord_texturas.push_back(coord_texturasAux[t3].y);

			//Saco normales
			u.x = auxVertices_2[v2].x - auxVertices_2[v1].x;
			u.y = auxVertices_2[v2].y - auxVertices_2[v1].y;
			u.z = auxVertices_2[v2].z - auxVertices_2[v1].z;
			v.x = auxVertices_2[v3].x - auxVertices_2[v1].x;
			v.y = auxVertices_2[v3].y - auxVertices_2[v1].y;
			v.z = auxVertices_2[v3].z - auxVertices_2[v1].z;
			normal.x = u.y*v.z - u.z*v.y;
			normal.y = u.z*v.x - u.x*v.z;
			normal.z = u.x*v.y - u.y*v.x;


			//Calculo la tangente y bitangente
			glm::vec3 tangent, bitangent;

			glm::vec3 edge1 = glm::vec3(auxVertices_2[v2].x - auxVertices_2[v1].x, auxVertices_2[v2].y - auxVertices_2[v1].y, auxVertices_2[v2].z - auxVertices_2[v1].z);
			glm::vec3 edge2 = glm::vec3(auxVertices_2[v3].x - auxVertices_2[v1].x, auxVertices_2[v3].y - auxVertices_2[v1].y, auxVertices_2[v3].z - auxVertices_2[v1].z);
			glm::vec2 deltaUV1 = glm::vec2(coord_texturasAux[t2].x - coord_texturasAux[t1].x, coord_texturasAux[t2].y - coord_texturasAux[t1].y);
			glm::vec2 deltaUV2 = glm::vec2(coord_texturasAux[t3].x - coord_texturasAux[t1].x, coord_texturasAux[t3].y - coord_texturasAux[t1].y);

			GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			tangent = glm::normalize(tangent);

			//Pusheo 3 veces por cada vertice
			m.tangents.push_back(tangent); m.tangents.push_back(tangent); m.tangents.push_back(tangent);

			//Guardo suma de normales por vertice
			normales_pavo[v1].x += normal.x;
			normales_pavo[v1].y += normal.y;
			normales_pavo[v1].z += normal.z;

			normales_pavo[v2].x += normal.x;
			normales_pavo[v2].y += normal.y;
			normales_pavo[v2].z += normal.z;

			normales_pavo[v3].x += normal.x;
			normales_pavo[v3].y += normal.y;
			normales_pavo[v3].z += normal.z;

			//Guardo total de la suma de normales para dividir
			totales[v1] += 1;
			totales[v2] += 1;
			totales[v3] += 1;

		}
	}

	for (int i = 0; i < faces.size(); i++) {
		int v1, v2, v3, aux = 1, k = 0;
		vertice u, v, final;
		glm::vec3 real_final;

		while (faces[i].n_vertex - 1 > aux) {
			v1 = faces[i].f[k];
			v2 = faces[i].f[aux];
			v3 = faces[i].f[++aux];
			final.x = (normales_pavo[v1].x / (float)totales[v1]);
			final.y = (normales_pavo[v1].y / (float)totales[v1]);
			final.z = (normales_pavo[v1].z / (float)totales[v1]);

			m.normales_vertice_fin.push_back(final.x); m.normales_vertice_fin.push_back(final.y); m.normales_vertice_fin.push_back(final.z);

			final.x = (normales_pavo[v2].x / (float)totales[v2]);
			final.y = (normales_pavo[v2].y / (float)totales[v2]);
			final.z = (normales_pavo[v2].z / (float)totales[v2]);

			m.normales_vertice_fin.push_back(final.x); m.normales_vertice_fin.push_back(final.y); m.normales_vertice_fin.push_back(final.z);

			final.x = (normales_pavo[v3].x / (float)totales[v3]);
			final.y = (normales_pavo[v3].y / (float)totales[v3]);
			final.z = (normales_pavo[v3].z / (float)totales[v3]);

			m.normales_vertice_fin.push_back(final.x); m.normales_vertice_fin.push_back(final.y); m.normales_vertice_fin.push_back(final.z);
		}
	}
	m.minX = minX; m.maxX = maxX; m.minY = minY; m.maxY = maxY; m.minZ = minZ; m.maxZ = maxZ;
	models.push_back(m); //Pusheo el modelo a vector de modelos
	
	if (models.size() - 1 == 0) {
		models[models.size() - 1].ejeX = 0.0;
		models[models.size() - 1].ejeY = -20.0 + 23.64;
		models[models.size() - 1].ejeZ = -32.0;
		models[models.size() - 1].scaleT = 20.0;
		models[models.size() - 1].normalMap = false;

	}
	//Plano
	else if (models.size() - 1 == 1) {
		models[models.size() - 1].ejeX = -1.92;
		models[models.size() - 1].ejeY = 0.0; // -23.64;
		models[models.size() - 1].ejeZ = 0.0;
		models[models.size() - 1].scaleT = 32.43;
		models[models.size() - 1].normalMap = true;
	}
	else if (models.size() - 1 == 2) {

		models[models.size() - 1].ejeX = -1.11;
		models[models.size() - 1].ejeY = 3.0;
		models[models.size() - 1].ejeZ = 26.33 - 15.0;
		models[models.size() - 1].scaleT = 2.00;
		models[models.size() - 1].normalMap = false;
	}
	else if (models.size() - 1 == 3) {

		models[models.size() - 1].scaleT = 3.0;
		models[models.size() - 1].ejeX = 0.0;
		models[models.size() - 1].ejeY = -6.23 + 23.64;
		models[models.size() - 1].ejeZ = 8.66 - 15.0;
		models[models.size() - 1].normalMap = false;
	}
	else if (models.size() - 1 == 4) {

		models[models.size() - 1].ejeX = 28.29;
		models[models.size() - 1].ejeY = -20.40 + 23.64;
		models[models.size() - 1].ejeZ = 11.97 - 15.0;
		models[models.size() - 1].normalMap = false;

	}
	else if (models.size() - 1 == 5) {
		models[models.size() - 1].scaleT = 4.0;
		models[models.size() - 1].ejeX = -27.61;
		models[models.size() - 1].ejeY = -18.84 + 23.64;
		models[models.size() - 1].ejeZ = 20.30 - 15.0;
		models[models.size() - 1].normalMap = false;
	}

	else if (models.size() - 1 == 6) {
		models[models.size() - 1].scaleT = 8.0;
		models[models.size() - 1].ejeX = 21.74;
		models[models.size() - 1].ejeY = -20.52 + 23.64;
		models[models.size() - 1].ejeZ = 27.63 - 15.0;
		models[models.size() - 1].normalMap = true;
	}
	//Sol
	else if (models.size() - 1 == 7) {
		models[models.size() - 1].ejeX = -0.14;
		models[models.size() - 1].ejeY = 0.69 + 23.64;
		models[models.size() - 1].ejeZ = 50.22 - 15.0;
		models[models.size() - 1].scaleT = 4.0;
		models[models.size() - 1].normalMap = false;
	}
	file.close();
	mod.initVBO();
}
