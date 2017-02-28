# Tarea 2 - [FCG]


## Contenido

* [Enunciado](#enunciado)
* [Mundo](#mundo)
* [Uso](#uso)
* [Herramienta](#herramienta)
* [LightMap](#lightmap)
* [Instalacion](#instalacion)
* [Autor](#autor)


#Enunciado

El enunciado de la tarea se encuentra en el pdf **_Enunciado.pdf_**

# Mundo

El mundo esta formado por 8 modelos 3D contando plano y esfera solar.
Estos modelos se dividen de la siguiente forma:
  - 1 plano que representa al piso de la escena (**_con normal mapping_**).
  - 1 Modelo extra con normal mapping (**_arrow_**).
  - 1 plano que representa a un espejo.
  - 2 Modelos extras (**_cyborg y cy_**).
  - 1 modelo refractor (**_calavera_**).
  - 1 Modelo con LightMaps (**_cubo_**).
  - 1 esfera que representa la luz.

Ademas se tiene un skybox de Canada para acompañar la escena.

A continuacion una imagen de dicho mundo:
![alt tag](https://i.gyazo.com/afc7bff93ac0c2a9892f038f17d74de9.png)

# Uso

Al cargar el mundo usted podra moverse libremente por la escena usando las teclas de su teclado W (arriba), S (abajo), A(izquierda) y D (derecha) y usando el cursor para (**_mirar_**). Si desea seleccionar un modelo debe entrar en el "modo seleccion", eso se logra tecleando la tecla "M" de su teclado, vera que el cursor aparecera en la pantalla, en este modo solo podra moverse con WASD. Todos los objetos del mundo se seleccionan con el picking del mouse (a excepcion de los dos planos, estos se hicieron no clickeables). Existen dos menus con los que podra interactuar segun el objeto que escoja, el menu de iluminacion y el de objetos. Para des-seleccionar un modelo solamente le debe **dar click nuevamente** a dicho objeto que ya esta seleccionado, o en cuyo caso seleccionar otro.

En el menu de inicio del programa podra editar el factor de refraccion del modelo refractante y si desea que aparezca el depthmap en la pantalla.

Todos los modelos cuentan con la edicion de parametros como colores ambientales, difuso y especular del material, poder activar/desactivar texturas, editar su BIAS de antialiasing, asi como transformaciones basicas como rotar/escalar/trasladar. 

Para la luz (esfera solar) podra editar que tipo de iluminacion sera (Puntual, Direccional o Reflector). Cabe destacar que la direccion del spot tambien se edita en esta nueva ventana. Tambien se tiene un boton para activar/desactivar la luz y la edicion de los colores de la luz difuso, especular y ambiental. Aqui podra prender y apagar la luz si asi lo desea.

En la tarea se manejan los modelos difuso y especular Lambert y Blinn-Phong, respectivamente. 

# LightMap

Como adicional a las actividades del enunciado se logro aplicar una textura de lightmap al modelo del cubo, las texturas usadas para crear este efecto de iluminacion son las siguientes:

![alt tag](https://i.gyazo.com/a35bed9506258c9a18508c0d81074a1c.gif) ![alt tag](https://i.gyazo.com/8ecb2c0296d73fe50ee4928a03854c72.gif)

Ambas se encuentran en la carpeta de "Texturas" con los nombres (**cubeText**) y (**LightmapText**) respectivamente.

# Herramienta 

La tarea previamente explicada fue desarrollada usando las siguientes herramientas:

| Herramienta                         	 | Versión   													   |                            
|----------------------------------------|-----------------------------------------------------------------|
| Microsoft Visual Studio        	 	 | 2015      													   |
| OpenGL				        	 	 | 4.0.0      													   |
| GLSL Version				             | 4.00      													   |

Y fue probada en la siguiente GPU:


| Fabricante                         	 | Nombre   													   |                            
|----------------------------------------|-----------------------------------------------------------------|
| Intel     	 					     | Intel HD Graphics 4000      									   |



# Instalacion

Para usar el proyecto se debe primero clonar el repositorio con el siguiente comando:

> git clone https://github.com/alejandrombc/FCG_Tarea2.git

Con esto puede ejecutar el ejecutable en la carpeta _/bin_ o abrir el proyecto de VS ubicado en _/Tarea 1 - 24206267_FCG_ con extension **.sln**.


# Autor

**Alejandro Barone**
**CI: 24206267**
