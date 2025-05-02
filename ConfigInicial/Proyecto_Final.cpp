


#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 2110, HEIGHT = 1280;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(-90.0f, 10.0f, -60.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);//Vista modificada de la camara predeterminadamente
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Aqui hay 9 posiciones de las luces de tipo puntual  de las luces puntuales 
glm::vec3 pointLightPositions[] = {
	glm::vec3(-33.5f,12.3f,-3.5f),//C1-F2
	glm::vec3(-33.5f,12.3f,21.0f),//C1-F3
	glm::vec3(-33.5f,12.3f,45.0f),//C1-F4
	glm::vec3(-2.5f,12.3f,-3.5f),//C2-F2
	glm::vec3(-2.5f,12.3f,21.0f),//C2-F3
	glm::vec3(-2.5f,12.3f,45.0f),//C2-F4
	glm::vec3(31.5f,12.3f,45.0f),//C3-F4
	glm::vec3(31.5f,12.3f,21.0f),//C3-F3
	glm::vec3(31.5f,12.3f,-3.5f)//C3-F2
};

glm::vec3 spotLightDir(0.0f, -0.1f, 0.0f); // Dirección inicial
float spotPitch = 13.0f; // ángulo inicial en grados para la luz tipo spotlight del proyector

//Variable global para intercambio entre escenarios
bool mostrarEscenarioNuevo = false;


glm::vec3 Light1 = glm::vec3(0);//Luces

//Variables para Animacion
float rotBall = 1.0472;
bool AnimBall = false;
float salto = 0.0f;
float rot = 0.0f;

float animationSpeed = 0.06f;  // Controla la velocidad global de la animación. Un valor más bajo significa más lento.


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final Grafica", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);


	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	


	// ------------------- MODELOS LAB VIEJO -----------------------------------------

	Model cuartoOld((char*)"Models/Escenario_Viejo/CuartoViejo/salonViejo.obj");		// LAB VIEJO
	Model ventanasOld((char*)"Models/Escenario_Viejo/ventanasOLD/ventanasOLD.obj");		// VENTANAS OLD
	Model mesaOld((char*)"Models/Escenario_Viejo/mesaOLD/mesaOLD.obj");					// MESA VIEJA
	Model sillaOld((char*)"Models/Escenario_Viejo/sillaOLD/sillaOLD.obj");				// SILLA VIEJA
	Model oldCPU((char*)"Models/Escenario_Viejo/oldCPU/oldCPU.obj");					// CPU VIEJO
	Model monitorOld((char*)"Models/Escenario_Viejo/monitorViejo/monitorViejo.obj");	// MONITOR VIEJO
	Model tecladoOld((char*)"Models/Escenario_Viejo/tecladoViejo/teclado.obj");			// TECLADO VIEJO
	Model mouseOld((char*)"Models/Escenario_Viejo/mouseViejo/mouseViejo.obj");			// MOUSE VIEJO
	Model puertaOld((char*)"Models/Escenario_Viejo/puertaOLD/puertaOLD.obj");			// PUERTA VIEJA


	// ------------------- MODELOS LAB NUEVO -----------------------------------------
	Model Cuarto((char*)"Models/Escenario_Nuevo/Cuarto/Cuartov2.obj");
	Model Ventana((char*)"Models/Escenario_Nuevo/Ventanas_Vidrio/Ventanas.obj");
	Model Profesor((char*)"Models/Escenario_Nuevo/Profesor/Profesor.obj");
	Model Mesa((char*)"Models/Escenario_Nuevo/Mesa/Mesa.obj");
	Model Silla((char*)"Models/Escenario_Nuevo/Silla/Silla.obj");
	Model CPU_1((char*)"Models/Escenario_Nuevo/CPU/CPU1.obj");
	Model Mon1((char*)"Models/Escenario_Nuevo/Monitor/Mon1.obj");
	Model Teclado((char*)"Models/Escenario_Nuevo/Teclado/Teclado.obj");
	Model Mouse((char*)"Models/Escenario_Nuevo/Mouse/Mouse.obj");
	Model MonitorProf((char*)"Models/Escenario_Nuevo/Mon_Prof/Mon_Prof.obj");
	Model MesaProf((char*)"Models/Escenario_Nuevo/Mesa_Profe/Mesa_Prof.obj");

	


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 300.0f);//Modificacion a 300 para alcanze más lejano de la camara

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);//Se encarga de poner el fondo de color negro en formato RGB
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		//Nuevas funciones agregadas
		/*glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);*/

		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp1 = glm::mat4(1.0f); //Temp
		  
		

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light-Luz general al escenario
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 10.2f, 83.0f, 10.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.3f,0.3f,0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.7f, 0.7f, 0.7f);


		
		//Luz puntual-Luz de lamparas

		for (int i = 0; i < 9; ++i) {
			std::string number = std::to_string(i);
			glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].position").c_str()), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].ambient").c_str()), 0.05f, 0.05f, 0.05f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].diffuse").c_str()), 1.00f, 1.00f, 1.00f);// Blanco intenso
			glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].specular").c_str()), 0.2f, 0.2f, 0.2f);// Reflejo blanco
			//calculo de la atenuacion
			glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].constant").c_str()), 1.0f); // no  tocar mucho
			glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].linear").c_str()), 0.05f);// más = menos alcance
			glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].quadratic").c_str()), 0.009f);// más = menos alcance
		}

		

		// SpotLight - intensificada
		// Dirección calculada rotando en eje X
		spotLightDir.x = 0.0f;
		spotLightDir.y = sin(glm::radians(spotPitch));
		spotLightDir.z = -cos(glm::radians(spotPitch));  // apunta hacia "adelante"

		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), 3.0f, 12.3f, -40.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), spotLightDir.x, spotLightDir.y, spotLightDir.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), spotLightDir.x, spotLightDir.y, spotLightDir.z);
		// Intensidades más brillantes
		// Luz azul brillante
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.05f, 0.05f, 0.4f);   // luz azul tenue
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.1f, 0.1f, 12.5f);     // azul intenso visible
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.3f, 0.3f, 1.0f);    // reflejo azulado
		// Atenuación más suave para mayor alcance
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		// Ángulos más amplios (más área iluminada)
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(30.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(40.0f)));



		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

		//Condicion para intercambiar entre los dos escenarios

		if (mostrarEscenarioNuevo) {
			
		// =========================================================================================
		// ================================ INICIO DE  MODELADO LAB NUEVO =================================
		// =========================================================================================


		//Modelo cuarto
			glm::mat4 modelCuarto(1.0f);
			modelCuarto = glm::scale(modelCuarto, glm::vec3(1.0f, 1.0f, 0.9f));
			modelCuarto = glm::translate(modelCuarto, glm::vec3(0.351f, 17.922f, 1.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCuarto));
			Cuarto.Draw(lightingShader);

			//Modelo Profesor
			glm::mat4 modelProfesor(1.0f);
			modelProfesor = glm::translate(modelProfesor, glm::vec3(3.868f, 11.692f, -54.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelProfesor));
			Profesor.Draw(lightingShader);


			//Modelos de accesorios profesor
			glm::mat4 modelMonitorProf(1.0f);
			modelMonitorProf = glm::translate(modelMonitorProf, glm::vec3(45.0f, 10.1f, -49.0f));
			modelMonitorProf = glm::rotate(modelMonitorProf, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMonitorProf));
			MonitorProf.Draw(lightingShader);


			glm::mat4 modelMesaProf(1.0f);
			modelMesaProf = glm::translate(modelMesaProf, glm::vec3(45.0f, 5.31f, -48.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesaProf));
			MesaProf.Draw(lightingShader);


			glm::mat4 modelSillaProf(1.0f);
			modelSillaProf = glm::translate(modelSillaProf, glm::vec3(47.0f, 0.12f, -57.0));
			modelSillaProf = glm::rotate(modelSillaProf, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSillaProf));
			Silla.Draw(lightingShader);


			glm::mat4 modelCPU1_Prof(1.0f);
			modelCPU1_Prof = glm::translate(modelCPU1_Prof, glm::vec3(47.0f, 12.8f, -46.5f));
			modelCPU1_Prof = glm::rotate(modelCPU1_Prof, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCPU1_Prof));
			CPU_1.Draw(lightingShader);


			glm::mat4 modelTecladoProfe(1.0f);
			modelTecladoProfe = glm::scale(modelTecladoProfe, glm::vec3(1.1f, 1.1f, 1.1f));
			modelTecladoProfe = glm::translate(modelTecladoProfe, glm::vec3(42.0f, 9.0f, -46.5f));
			modelTecladoProfe = glm::rotate(modelTecladoProfe, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTecladoProfe));
			Teclado.Draw(lightingShader); // Mismo modelo "Teclado"


			glm::mat4 modelMouseProfe(1.0f);
			modelMouseProfe = glm::translate(modelMouseProfe, glm::vec3(41.0f, 9.9f, -51.0f));
			modelMouseProfe = glm::rotate(modelMouseProfe, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMouseProfe));
			Mouse.Draw(lightingShader);




			// --- Modelos de mesas ---
			// ---Columna 1 de mesas ---
			glm::mat4 modelMesa1(1.0f);
			modelMesa1 = glm::translate(modelMesa1, glm::vec3(-31.0f, 5.31f, -33.176f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa1));
			Mesa.Draw(lightingShader);

			glm::mat4 modelMesa2(1.0f);
			modelMesa2 = glm::translate(modelMesa2, glm::vec3(-31.0f, 5.31f, -8.044f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa2));
			Mesa.Draw(lightingShader);



			glm::mat4 modelMesa3(1.0f);
			modelMesa3 = glm::translate(modelMesa3, glm::vec3(-31.0f, 5.31f, 17.088f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa3));
			Mesa.Draw(lightingShader);


			glm::mat4 modelMesa4(1.0f);
			modelMesa4 = glm::translate(modelMesa4, glm::vec3(-31.0f, 5.31f, 42.22f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa4));
			Mesa.Draw(lightingShader);



			/// --- Columna 2 de mesas ---
			glm::mat4 modelMesa5(1.0f);
			modelMesa5 = glm::scale(model, glm::vec3(1.12f, 1.0f, 1.0f)); // Aumenta el tamaño en X, deja Y y Z igual
			modelMesa5 = glm::translate(modelMesa5, glm::vec3(27.8f, 5.31f, -33.176f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa5));
			Mesa.Draw(lightingShader);


			glm::mat4 modelMesa6(1.0f);
			modelMesa6 = glm::scale(model, glm::vec3(1.12f, 1.0f, 1.0f)); // ajusta según tamaño real del modelo
			modelMesa6 = glm::translate(modelMesa6, glm::vec3(27.8f, 5.31f, -8.044f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa6));
			Mesa.Draw(lightingShader);


			glm::mat4 modelMesa7(1.0f);
			modelMesa7 = glm::scale(model, glm::vec3(1.12f, 1.0f, 1.0f)); // ajusta según tamaño real del modelo
			modelMesa7 = glm::translate(modelMesa7, glm::vec3(27.8f, 5.31f, 17.088f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa7));
			Mesa.Draw(lightingShader);


			glm::mat4 modelMesa8(1.0f);
			modelMesa8 = glm::scale(model, glm::vec3(1.12f, 1.0f, 1.0f));  // ajusta según tamaño real del modelo
			modelMesa8 = glm::translate(modelMesa8, glm::vec3(27.8f, 5.31f, 42.22f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa8));
			Mesa.Draw(lightingShader);


			// --- Modelos de sillas --- 
			//COLUMNA 1
			int numFilas = 4;         // Número de filas de sillas
			int sillasPorFila = 4;    // Número de sillas en cada fila

			float xBase = -47.529f;         // X de la primera silla de cada fila
			float yBase = 0.12f;          // Y (altura fija)
			float zBase = -23.176f;         // Z de la primera fila
			float separacionX = 10.0f;      // Separación entre sillas (en X)
			float separacionZ = 25.273f;       // Separación entre filas (en Z)

			for (int fila = 0; fila < numFilas; ++fila) {
				float zActual = zBase + fila * separacionZ;
				for (int i = 0; i < sillasPorFila; ++i) {
					glm::mat4 modelSilla(1.0f);
					modelSilla = glm::translate(modelSilla, glm::vec3(xBase + i * separacionX, yBase, zActual));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSilla));
					Silla.Draw(lightingShader);
				}
			}



			//Modelos de sillas 
			//COLUMNA  2
			int numFilas_2 = 4;         // Número de filas de sillas
			int sillasPorFila_2 = 5;    // Número de sillas en cada fila

			float xBase_2 = 12.0f;         // X de la primera silla de cada fila
			float yBase_2 = 0.12f;          // Y (altura fija)
			float zBase_2 = -22.176f;         // Z de la primera fila
			float separacionX_2 = 9.028f;      // Separación entre sillas (en X)
			float separacionZ_2 = 25.0f;       // Separación entre filas (en Z)

			for (int fila_2 = 0; fila_2 < numFilas_2; ++fila_2) {
				float zActual_2 = zBase_2 + fila_2 * separacionZ_2;
				for (int i = 0; i < sillasPorFila_2; ++i) {
					glm::mat4 modelSilla(1.0f);
					modelSilla = glm::translate(modelSilla, glm::vec3(xBase_2 + i * separacionX_2, yBase_2, zActual_2));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSilla));
					Silla.Draw(lightingShader);
				}
			}



			// --- Modelos de CPU ---
			//Columna 1
			int numFilas_CPU1 = 4;         // Número de filas de CPU's
			int CPU_PorFila = 4;    // Número de cpu's en cada fila

			float xBase_CPU1 = -47.25f;         // X del primer CPU  de cada fila
			float yBase_CPU1 = 12.8f;          // Y (altura fija)
			float zBase_CPU1 = -35.6f;         // Z de la primera fila
			float separacionX_CPU1 = 10.0F;      // Separación entre cpus (en X)
			float separacionZ_CPU1 = 25.112f;       // Separación entre cpus (en Z)

			for (int fila = 0; fila < numFilas_CPU1; ++fila) {
				float zActual = zBase_CPU1 + fila * separacionZ_CPU1;
				for (int i = 0; i < CPU_PorFila; ++i) {
					glm::mat4 modelCPU1(1.0f);
					modelCPU1 = glm::translate(modelCPU1, glm::vec3(xBase_CPU1 + i * separacionX_CPU1, yBase_CPU1, zActual));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCPU1));
					CPU_1.Draw(lightingShader);
				}
			}

			// ---Columna 2 de CPUs ---
			int numFilas_CPU2 = 4;         // Número de filas de CPU's
			int CPU2_PorFila = 5;    // Número de cpu's en cada fila

			float xBase_CPU2 = 13.25f;         // X del primer CPU  de cada fila
			float yBase_CPU2 = 12.8f;          // Y (altura fija)
			float zBase_CPU2 = -35.6f;         // Z de la primera fila
			float separacionX_CPU2 = 9.017f;      // Separación entre cpus (en X)
			float separacionZ_CPU2 = 25.112f;       // Separación entre cpus (en Z)

			for (int fila = 0; fila < numFilas_CPU2; ++fila) {
				float zActual = zBase_CPU2 + fila * separacionZ_CPU2;
				for (int i = 0; i < CPU2_PorFila; ++i) {
					glm::mat4 modelCPU2(1.0f);
					modelCPU2 = glm::translate(modelCPU2, glm::vec3(xBase_CPU2 + i * separacionX_CPU2, yBase_CPU2, zActual));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCPU2));
					CPU_1.Draw(lightingShader);
				}
			}




			// --- Modelos de Monitores ---

			// Columna 1 (4 monitores por fila, 4 filas)
			int numFilas_Mon1 = 4;
			int Mon1_PorFila = 4;

			float xBase_Mon1 = -46.529f;         // X del primer monitor de la primera columna
			float yBase_Mon1 = 10.1f;           // Y (altura fija)
			float zBase_Mon1 = -33.676f;         // Z inicial de la primera fila
			float separacionX_Mon1 = 10.0f;      // Separación entre monitores (X)
			float separacionZ_Mon1 = 25.0f;      // Separación entre filas (Z)

			for (int fila = 0; fila < numFilas_Mon1; ++fila) {
				float zActual = zBase_Mon1 + fila * separacionZ_Mon1;
				for (int i = 0; i < Mon1_PorFila; ++i) {
					glm::mat4 modelMon1(1.0f);
					modelMon1 = glm::translate(modelMon1, glm::vec3(xBase_Mon1 + i * separacionX_Mon1, yBase_Mon1, zActual));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMon1));
					Mon1.Draw(lightingShader);
				}
			}

			// Columna 2 (5 monitores por fila, 4 filas)
			int numFilas_Mon2 = 4;
			int Mon2_PorFila = 5;

			float xBase_Mon2 = xBase_Mon1 + Mon1_PorFila * separacionX_Mon1 + 19.0f; // separación entre columna 1 y columna 2
			float yBase_Mon2 = 10.1f;           // Misma altura
			float zBase_Mon2 = -33.676f;         // Misma Z inicial
			float separacionX_Mon2 = 9.2f;      // Separación entre monitores (X)
			float separacionZ_Mon2 = 25.0f;      // Separación entre filas (Z)

			for (int fila = 0; fila < numFilas_Mon2; ++fila) {
				float zActual = zBase_Mon2 + fila * separacionZ_Mon2;
				for (int i = 0; i < Mon2_PorFila; ++i) {
					glm::mat4 modelMon2(1.0f);
					modelMon2 = glm::translate(modelMon2, glm::vec3(xBase_Mon2 + i * separacionX_Mon2, yBase_Mon2, zActual));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMon2));
					Mon1.Draw(lightingShader);
				}
			}



			// --- Modelos de Teclados ---

			// Columna 1 (4 teclados por fila)
			int numFilas_Teclado1 = 4;
			int Teclados1_PorFila = 4;

			float xBase_Teclado1 = -46.529f;         // X inicial de la primera columna
			float yBase_Teclado1 = 10.0f;             // Altura (ajústala si quieres que esté sobre la mesa)
			float zBase_Teclado1 = -31.0f;         // Z inicial
			float separacionX_Teclado1 = 10.0f;      // Separación entre teclados (X)
			float separacionZ_Teclado1 = 25.0f;      // Separación entre filas (Z)

			for (int fila = 0; fila < numFilas_Teclado1; ++fila) {
				float zActual = zBase_Teclado1 + fila * separacionZ_Teclado1;
				for (int i = 0; i < Teclados1_PorFila; ++i) {
					glm::mat4 modelTeclado1(1.0f);
					modelTeclado1 = glm::translate(modelTeclado1, glm::vec3(xBase_Teclado1 + i * separacionX_Teclado1, yBase_Teclado1, zActual));
					modelTeclado1 = glm::scale(modelTeclado1, glm::vec3(1.1f, 1.1f, 1.1f));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTeclado1));
					Teclado.Draw(lightingShader); // Asegúrate que tu modelo de teclado se llame "Teclado"
				}
			}

			// --- Columna 2 (5 teclados por fila) ---
			int numFilas_Teclado2 = 4;
			int Teclados2_PorFila = 5;

			float xBase_Teclado2 = xBase_Teclado1 + Teclados1_PorFila * separacionX_Teclado1 + 19.0f; // separación extra entre columnas
			float yBase_Teclado2 = 10.0f;           // Misma altura
			float zBase_Teclado2 = -31.0f;       // Misma Z inicial
			float separacionX_Teclado2 = 9.0f;    // Separación entre teclados (X)
			float separacionZ_Teclado2 = 25.0f;    // Separación entre filas (Z)

			for (int fila = 0; fila < numFilas_Teclado2; ++fila) {
				float zActual = zBase_Teclado2 + fila * separacionZ_Teclado2;
				for (int i = 0; i < Teclados2_PorFila; ++i) {
					glm::mat4 modelTeclado2(1.0f);
					modelTeclado2 = glm::translate(modelTeclado2, glm::vec3(xBase_Teclado2 + i * separacionX_Teclado2, yBase_Teclado2, zActual));
					modelTeclado2 = glm::scale(modelTeclado2, glm::vec3(1.1f, 1.1f, 1.1f));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTeclado2));
					Teclado.Draw(lightingShader); // Mismo modelo "Teclado"
				}
			}

			// --- Modelos de Mouse ---

			// Columna 1 (4 mouses por fila)
			int numFilas_Mouse1 = 4;
			int Mouses1_PorFila = 4;

			float xBase_Mouse1 = -42.0f;
			float yBase_Mouse1 = 10.0f;
			float zBase_Mouse1 = -31.0f;
			float separacionX_Mouse1 = 10.0f;
			float separacionZ_Mouse1 = 25.0f;

			for (int fila = 0; fila < numFilas_Mouse1; ++fila) {
				float zActual = zBase_Mouse1 + fila * separacionZ_Mouse1;
				for (int i = 0; i < Mouses1_PorFila; ++i) {
					glm::mat4 modelMouse1(1.0f);
					modelMouse1 = glm::translate(modelMouse1, glm::vec3(xBase_Mouse1 + i * separacionX_Mouse1, yBase_Mouse1, zActual));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMouse1));
					Mouse.Draw(lightingShader); // Modelo de Mouse
				}
			}

			// Columna 2 (5 mouses por fila)
			int numFilas_Mouse2 = 4;
			int Mouses2_PorFila = 5;

			float xBase_Mouse2 = xBase_Mouse1 + Mouses1_PorFila * separacionX_Mouse1 + 19.0f;
			float yBase_Mouse2 = 10.0f;
			float zBase_Mouse2 = -31.0f;
			float separacionX_Mouse2 = 8.9f;
			float separacionZ_Mouse2 = 25.0f;

			for (int fila = 0; fila < numFilas_Mouse2; ++fila) {
				float zActual = zBase_Mouse2 + fila * separacionZ_Mouse2;
				for (int i = 0; i < Mouses2_PorFila; ++i) {
					glm::mat4 modelMouse2(1.0f);
					modelMouse2 = glm::translate(modelMouse2, glm::vec3(xBase_Mouse2 + i * separacionX_Mouse2, yBase_Mouse2, zActual));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMouse2));
					Mouse.Draw(lightingShader);
				}
			}


			//--- Modelo ventanas con trasnparencia---

			glm::mat4 modelVentanas(1.0f);
			glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelVentanas));
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			modelVentanas = glm::scale(modelVentanas, glm::vec3(1.0f, 1.0f, 0.9f));
			modelVentanas = glm::translate(modelVentanas, glm::vec3(0.15f, 17.922f, 1.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelVentanas));
			Ventana.Draw(lightingShader);
			glDisable(GL_BLEND);  //Desactiva el canal alfa 




			// =========================================================================================
			// ================================ FIN MODELADO LAB NUEVO =================================
			// =========================================================================================

		}else{

		// =========================================================================================
			// ===================== MODELADO LABORATORIO VIEJO ========================================
			// =========================================================================================

			// ------------- Modelado salón viejo ----------------------------------------------
		glm::mat4 modelCuartoOld(1.0f);
		modelCuartoOld = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		modelCuartoOld = glm::translate(modelCuartoOld, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCuartoOld));
		cuartoOld.Draw(lightingShader);
		 // ventanas y puerta se dibujan al final para activar la transparencia
		
		// ---------------------------------------------------------------------------------

		// ------------- Modelado escritorio Profesor ----------------------------------------------
		// Escritorio 1 (profesor)
		glm::mat4 modelOldMesa1(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa1));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa1));
		modelOldMesa1 = glm::translate(modelOldMesa1, glm::vec3(38.224f, 6.758f, -59.289f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa1));
		mesaOld.Draw(lightingShader);

		// CPU profesor
		glm::mat4 modeloldCPU1(1.0f);
		modeloldCPU1 = glm::translate(modeloldCPU1, glm::vec3(42.13f, 14.443f, -59.904f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU1));
		oldCPU.Draw(lightingShader);

		// monitor Profesor
		glm::mat4 modeloldMonitor1(1.0f);
		modeloldMonitor1 = glm::translate(modeloldMonitor1, glm::vec3(37.701f, 14.434f, -60.837f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor1));
		monitorOld.Draw(lightingShader);

		// teclado Profesor
		glm::mat4 modeloldTeclado1(1.0f);
		modeloldTeclado1 = glm::translate(modeloldTeclado1, glm::vec3(37.659f, 12.327f, -57.878f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado1));
		tecladoOld.Draw(lightingShader);

		// mouse Profesor
		glm::mat4 modeloldMouse1(1.0f);
		modeloldMouse1 = glm::translate(modeloldMouse1, glm::vec3(38.811f, 12.405f, -59.408f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse1));
		mouseOld.Draw(lightingShader);

		// silla Profesor
		glm::mat4 modeloldSilla1(1.0f);
		modeloldSilla1 = glm::translate(modeloldSilla1, glm::vec3(37.575f, 6.377f, -49.771f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla1));
		sillaOld.Draw(lightingShader);


		// ---------------------------------------------------------------------------------
		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ------------- Modelado fila 1 columna 1 ----------------------------------------------
		
		
		// Escritorio 2
		glm::mat4 modelOldMesa2(1.0f);
		modelOldMesa2 = glm::translate(modelOldMesa2, glm::vec3(-40.424f, 6.758f, -28.937f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa2));
		mesaOld.Draw(lightingShader);

		// Escritorio 3
		glm::mat4 modelOldMesa3(1.0f);
		modelOldMesa3 = glm::translate(modelOldMesa3, glm::vec3(-20.105f, 6.758f, -28.937f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa3));
		mesaOld.Draw(lightingShader);


		// ----------- CPUs --------------------
		// CPU 2
		glm::mat4 modeloldCPU2(1.0f);
		modeloldCPU2 = glm::translate(modeloldCPU2, glm::vec3(-49.072f, 14.443f, -30.699f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU2));
		oldCPU.Draw(lightingShader);
		// CPU 3
		glm::mat4 modeloldCPU3(1.0f);
		modeloldCPU3 = glm::translate(modeloldCPU3, glm::vec3(-39.384f, 14.443f, -30.699f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU3));
		oldCPU.Draw(lightingShader);
		// CPU 4
		glm::mat4 modeloldCPU4(1.0f);
		modeloldCPU4 = glm::translate(modeloldCPU4, glm::vec3(-28.76f, 14.443f, -30.699f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU4));
		oldCPU.Draw(lightingShader);
		// CPU 5
		glm::mat4 modeloldCPU5(1.0f);
		modeloldCPU5 = glm::translate(modeloldCPU5, glm::vec3(-19.072f, 14.443f, -30.699f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU5));
		oldCPU.Draw(lightingShader);


		// ------------ monitores ------------
		// monitor 2
		glm::mat4 modeloldMonitor2(1.0f);
		modeloldMonitor2 = glm::translate(modeloldMonitor2, glm::vec3(-44.764f, 14.434f, -31.492f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor2));
		monitorOld.Draw(lightingShader);
		// monitor 3
		glm::mat4 modeloldMonitor3(1.0f);
		modeloldMonitor3 = glm::translate(modeloldMonitor3, glm::vec3(-34.541f, 14.434f, -31.492f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor3));
		monitorOld.Draw(lightingShader);
		// monitor 4
		glm::mat4 modeloldMonitor4(1.0f);
		modeloldMonitor4 = glm::translate(modeloldMonitor4, glm::vec3(-24.452f, 14.434f, -31.492f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor4));
		monitorOld.Draw(lightingShader);
		// monitor 5
		glm::mat4 modeloldMonitor5(1.0f);
		modeloldMonitor5 = glm::translate(modeloldMonitor5, glm::vec3(-14.616f, 14.434f, -31.492f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor5));
		monitorOld.Draw(lightingShader);


		// -------- teclados -----------------
		// teclado 2
		glm::mat4 modeloldTeclado2(1.0f);
		modeloldTeclado2 = glm::translate(modeloldTeclado2, glm::vec3(-45.233f, 12.299f, -28.673f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado2));
		tecladoOld.Draw(lightingShader);
		// teclado 3
		glm::mat4 modeloldTeclado3(1.0f);
		modeloldTeclado3 = glm::translate(modeloldTeclado3, glm::vec3(-35.143f, 12.299f, -28.896f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado3));
		tecladoOld.Draw(lightingShader);
		// teclado 4
		glm::mat4 modeloldTeclado4(1.0f);
		modeloldTeclado4 = glm::translate(modeloldTeclado4, glm::vec3(-24.92f, 12.299f, -28.673f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado4));
		tecladoOld.Draw(lightingShader);
		// teclado 5
		glm::mat4 modeloldTeclado5(1.0f);
		modeloldTeclado5 = glm::translate(modeloldTeclado5, glm::vec3(-14.831f, 12.299f, -28.896f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado5));
		tecladoOld.Draw(lightingShader);


		// ---------- mouses -------------------
		// mouse 2
		glm::mat4 modeloldMouse2(1.0f);
		modeloldMouse2 = glm::translate(modeloldMouse2, glm::vec3(-43.789f, 12.405f, -30.203f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse2));
		mouseOld.Draw(lightingShader);
		// mouse 3
		glm::mat4 modeloldMouse3(1.0f);
		modeloldMouse3 = glm::translate(modeloldMouse3, glm::vec3(-33.594f, 12.405f, -30.203f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse3));
		mouseOld.Draw(lightingShader);
		// mouse 4
		glm::mat4 modeloldMouse4(1.0f);
		modeloldMouse4 = glm::translate(modeloldMouse4, glm::vec3(-23.476f, 12.405f, -30.203f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse4));
		mouseOld.Draw(lightingShader);
		// mouse 5
		glm::mat4 modeloldMouse5(1.0f);
		modeloldMouse5 = glm::translate(modeloldMouse5, glm::vec3(-13.281f, 12.405f, -30.203f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse5));
		mouseOld.Draw(lightingShader);


		// -------------- sillas ------------------------------
		// silla 2
		glm::mat4 modeloldSilla2(1.0f);
		modeloldSilla2 = glm::translate(modeloldSilla2, glm::vec3(-45.472f, 6.377f, -23.339f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla2));
		sillaOld.Draw(lightingShader);
		// silla 3
		glm::mat4 modeloldSilla3(1.0f);
		modeloldSilla3 = glm::translate(modeloldSilla3, glm::vec3(-35.524f, 6.377f, -23.339f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla3));
		sillaOld.Draw(lightingShader);
		// silla 4
		glm::mat4 modeloldSilla4(1.0f);
		modeloldSilla4 = glm::translate(modeloldSilla4, glm::vec3(-25.261f, 6.377f, -23.339f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla4));
		sillaOld.Draw(lightingShader);
		// silla 5
		glm::mat4 modeloldSilla5(1.0f);
		modeloldSilla5 = glm::translate(modeloldSilla5, glm::vec3(-15.112f, 6.377f, -23.339f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla5));
		sillaOld.Draw(lightingShader);


		// ---------------------------------------------------------------------------------
		// ---------------------------------------------------------------------------------
		// 
		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ------------- Modelado fila 2 columna 1 ----------------------------------------------
		
		// Escritorio 4
		glm::mat4 modelOldMesa4(1.0f);
		modelOldMesa4 = glm::translate(modelOldMesa4, glm::vec3(-40.424f, 6.758f, -2.779f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa4));
		mesaOld.Draw(lightingShader);

		// Escritorio 5
		glm::mat4 modelOldMesa5(1.0f);
		modelOldMesa5 = glm::translate(modelOldMesa5, glm::vec3(-20.105f, 6.758f, -2.779f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa5));
		mesaOld.Draw(lightingShader);

		// ----------- CPUs --------------------
		// CPU 6
		glm::mat4 modeloldCPU6(1.0f);
		modeloldCPU6 = glm::translate(modeloldCPU6, glm::vec3(-49.072f, 14.443f, -3.594f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU6));
		oldCPU.Draw(lightingShader);
		// CPU 7
		glm::mat4 modeloldCPU7(1.0f);
		modeloldCPU7 = glm::translate(modeloldCPU7, glm::vec3(-39.384f, 14.443f, -3.594f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU7));
		oldCPU.Draw(lightingShader);
		// CPU 8
		glm::mat4 modeloldCPU8(1.0f);
		modeloldCPU8 = glm::translate(modeloldCPU8, glm::vec3(-28.76f, 14.443f, -3.594f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU8));
		oldCPU.Draw(lightingShader);
		// CPU 9
		glm::mat4 modeloldCPU9(1.0f);
		modeloldCPU9 = glm::translate(modeloldCPU9, glm::vec3(-19.072f, 14.443f, -3.594f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU9));
		oldCPU.Draw(lightingShader);
		// ------------ monitores ------------
		// monitor 6
		glm::mat4 modeloldMonitor6(1.0f);
		modeloldMonitor6 = glm::translate(modeloldMonitor6, glm::vec3(-44.764f, 14.434f, -4.386f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor6));
		monitorOld.Draw(lightingShader);
		// monitor 7
		glm::mat4 modeloldMonitor7(1.0f);
		modeloldMonitor7 = glm::translate(modeloldMonitor7, glm::vec3(-34.541f, 14.434f, -4.386f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor7));
		monitorOld.Draw(lightingShader);
		// monitor 8
		glm::mat4 modeloldMonitor8(1.0f);
		modeloldMonitor8 = glm::translate(modeloldMonitor8, glm::vec3(-24.452f, 14.434f, -4.386f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor8));
		monitorOld.Draw(lightingShader);
		// monitor 9
		glm::mat4 modeloldMonitor9(1.0f);
		modeloldMonitor9 = glm::translate(modeloldMonitor9, glm::vec3(-14.616f, 14.434f, -4.386f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor9));
		monitorOld.Draw(lightingShader);
		// -------- teclados -----------------
		// teclado 6
		glm::mat4 modeloldTeclado6(1.0f);
		modeloldTeclado6 = glm::translate(modeloldTeclado6, glm::vec3(-45.233f, 12.299f, -1.568f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado6));
		tecladoOld.Draw(lightingShader);
		// teclado 7
		glm::mat4 modeloldTeclado7(1.0f);
		modeloldTeclado7 = glm::translate(modeloldTeclado7, glm::vec3(-35.143f, 12.299f, -1.568f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado7));
		tecladoOld.Draw(lightingShader);
		// teclado 8
		glm::mat4 modeloldTeclado8(1.0f);
		modeloldTeclado8 = glm::translate(modeloldTeclado8, glm::vec3(-24.92f, 12.299f, -1.568f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado8));
		tecladoOld.Draw(lightingShader);
		// teclado 9
		glm::mat4 modeloldTeclado9(1.0f);
		modeloldTeclado9 = glm::translate(modeloldTeclado9, glm::vec3(-14.831f, 12.299f, -1.568f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado9));
		tecladoOld.Draw(lightingShader);
		// ---------- mouses -------------------
		// mouse 6
		glm::mat4 modeloldMouse6(1.0f);
		modeloldMouse6 = glm::translate(modeloldMouse6, glm::vec3(-43.789f, 12.405f, -3.098f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse6));
		mouseOld.Draw(lightingShader);
		// mouse 7
		glm::mat4 modeloldMouse7(1.0f);
		modeloldMouse7 = glm::translate(modeloldMouse7, glm::vec3(-33.594f, 12.405f, -3.098f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse7));
		mouseOld.Draw(lightingShader);
		// mouse 4
		glm::mat4 modeloldMouse8(1.0f);
		modeloldMouse8 = glm::translate(modeloldMouse8, glm::vec3(-23.476f, 12.405f, -3.098f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse8));
		mouseOld.Draw(lightingShader);
		// mouse 9
		glm::mat4 modeloldMouse9(1.0f);
		modeloldMouse9 = glm::translate(modeloldMouse9, glm::vec3(-13.281f, 12.405f, -3.098f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse9));
		mouseOld.Draw(lightingShader);
		// ----------- sillas ----------------------------
		// silla 6
		glm::mat4 modeloldSilla6(1.0f);
		modeloldSilla6 = glm::translate(modeloldSilla6, glm::vec3(-45.472f, 6.377f, 5.122f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla6));
		sillaOld.Draw(lightingShader);
		// silla 7
		glm::mat4 modeloldSilla7(1.0f);
		modeloldSilla7 = glm::translate(modeloldSilla7, glm::vec3(-35.524f, 6.377f, 5.122f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla7));
		sillaOld.Draw(lightingShader);
		// silla 8
		glm::mat4 modeloldSilla8(1.0f);
		modeloldSilla8 = glm::translate(modeloldSilla8, glm::vec3(-25.261f, 6.377f, 5.122f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla8));
		sillaOld.Draw(lightingShader);
		// silla 9
		glm::mat4 modeloldSilla9(1.0f);
		modeloldSilla9 = glm::translate(modeloldSilla9, glm::vec3(-15.112f, 6.377f, 5.122f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla9));
		sillaOld.Draw(lightingShader);


		// ---------------------------------------------------------------------------------

		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ------------- Modelado fila 3 columna 1 ----------------------------------------------
		// 
		
		// Escritorio 6
		glm::mat4 modelOldMesa6(1.0f);
		modelOldMesa6 = glm::translate(modelOldMesa6, glm::vec3(-40.424f, 6.758f, 22.489f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa6));
		mesaOld.Draw(lightingShader);
		// Escritorio 7
		glm::mat4 modelOldMesa7(1.0f);
		modelOldMesa7 = glm::translate(modelOldMesa7, glm::vec3(-20.105f, 6.758f, 22.489f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa7));
		mesaOld.Draw(lightingShader);
		// ----------- CPUs --------------------
		// CPU 10
		glm::mat4 modeloldCPU10(1.0f);
		modeloldCPU10 = glm::translate(modeloldCPU10, glm::vec3(-49.072f, 14.443f, 21.984f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU10));
		oldCPU.Draw(lightingShader);
		// CPU 11
		glm::mat4 modeloldCPU11(1.0f);
		modeloldCPU11 = glm::translate(modeloldCPU11, glm::vec3(-39.384f, 14.443f, 21.984f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU11));
		oldCPU.Draw(lightingShader);
		// CPU 12
		glm::mat4 modeloldCPU12(1.0f);
		modeloldCPU12 = glm::translate(modeloldCPU12, glm::vec3(-28.76f, 14.443f, 21.984f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU12));
		oldCPU.Draw(lightingShader);
		// CPU 13
		glm::mat4 modeloldCPU13(1.0f);
		modeloldCPU13 = glm::translate(modeloldCPU13, glm::vec3(-19.072f, 14.443f, 21.984f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU13));
		oldCPU.Draw(lightingShader);
		// ------------ monitores ------------
		// monitor 10
		glm::mat4 modeloldMonitor10(1.0f);
		modeloldMonitor10 = glm::translate(modeloldMonitor10, glm::vec3(-44.764f, 14.434f, 21.191f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor10));
		monitorOld.Draw(lightingShader);
		// monitor 11
		glm::mat4 modeloldMonitor11(1.0f);
		modeloldMonitor11 = glm::translate(modeloldMonitor11, glm::vec3(-34.541f, 14.434f, 21.191f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor11));
		monitorOld.Draw(lightingShader);
		// monitor 12
		glm::mat4 modeloldMonitor12(1.0f);
		modeloldMonitor12 = glm::translate(modeloldMonitor12, glm::vec3(-24.452f, 14.434f, 21.191f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor12));
		monitorOld.Draw(lightingShader);
		// monitor 13
		glm::mat4 modeloldMonitor13(1.0f);
		modeloldMonitor13 = glm::translate(modeloldMonitor13, glm::vec3(-14.616f, 14.434f, 21.191f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor13));
		monitorOld.Draw(lightingShader);
		// -------- teclados -----------------
		// teclado 10
		glm::mat4 modeloldTeclado10(1.0f);
		modeloldTeclado10 = glm::translate(modeloldTeclado10, glm::vec3(-45.233f, 12.299f, 24.009f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado10));
		tecladoOld.Draw(lightingShader);
		// teclado 11
		glm::mat4 modeloldTeclado11(1.0f);
		modeloldTeclado11 = glm::translate(modeloldTeclado11, glm::vec3(-35.143f, 12.299f, 23.786f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado11));
		tecladoOld.Draw(lightingShader);
		// teclado 12
		glm::mat4 modeloldTeclado12(1.0f);
		modeloldTeclado12 = glm::translate(modeloldTeclado12, glm::vec3(-24.92f, 12.299f, 23.786f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado12));
		tecladoOld.Draw(lightingShader);
		// teclado 13
		glm::mat4 modeloldTeclado13(1.0f);
		modeloldTeclado13 = glm::translate(modeloldTeclado13, glm::vec3(-14.831f, 12.299f, 24.009f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado13));
		tecladoOld.Draw(lightingShader);
		// ---------- mouses -------------------
		// mouse 10
		glm::mat4 modeloldMouse10(1.0f);
		modeloldMouse10 = glm::translate(modeloldMouse10, glm::vec3(-43.789f, 12.405f, 22.479f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse10));
		mouseOld.Draw(lightingShader);
		// mouse 11
		glm::mat4 modeloldMouse11(1.0f);
		modeloldMouse11 = glm::translate(modeloldMouse11, glm::vec3(-33.594f, 12.405f, 22.254f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse11));
		mouseOld.Draw(lightingShader);
		// mouse 12
		glm::mat4 modeloldMouse12(1.0f);
		modeloldMouse12 = glm::translate(modeloldMouse12, glm::vec3(-23.476f, 12.405f, 22.479f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse12));
		mouseOld.Draw(lightingShader);
		// mouse 13
		glm::mat4 modeloldMouse13(1.0f);
		modeloldMouse13 = glm::translate(modeloldMouse13, glm::vec3(-13.281f, 12.405f, 22.254f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse13));
		mouseOld.Draw(lightingShader);
		// ----------- sillas ----------------------------
		// silla 10
		glm::mat4 modeloldSilla10(1.0f);
		modeloldSilla10 = glm::translate(modeloldSilla10, glm::vec3(-45.472f, 6.377f, 29.252f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla10));
		sillaOld.Draw(lightingShader);
		// silla 11
		glm::mat4 modeloldSilla11(1.0f);
		modeloldSilla11 = glm::translate(modeloldSilla11, glm::vec3(-35.524f, 6.377f, 29.252f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla11));
		sillaOld.Draw(lightingShader);
		// silla 12
		glm::mat4 modeloldSilla12(1.0f);
		modeloldSilla12 = glm::translate(modeloldSilla12, glm::vec3(-25.261f, 6.377f, 29.252f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla12));
		sillaOld.Draw(lightingShader);
		// silla 13
		glm::mat4 modeloldSilla13(1.0f);
		modeloldSilla13 = glm::translate(modeloldSilla13, glm::vec3(-15.112f, 6.377f, 29.252f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla13));
		sillaOld.Draw(lightingShader);


		// ---------------------------------------------------------------------------------

		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ------------- Modelado fila 4 columna 1 ----------------------------------------------
		
		
		// Escritorio 8
		glm::mat4 modelOldMesa8(1.0f);
		modelOldMesa8 = glm::translate(modelOldMesa8, glm::vec3(-40.424f, 6.758f, 48.24f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa8));
		mesaOld.Draw(lightingShader);
		// Escritorio 9
		glm::mat4 modelOldMesa9(1.0f);
		modelOldMesa9 = glm::translate(modelOldMesa9, glm::vec3(-20.105f, 6.758f, 48.24f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa9));
		mesaOld.Draw(lightingShader);


		// ----------- CPUs --------------------
		// CPU 14
		glm::mat4 modeloldCPU14(1.0f);
		modeloldCPU14 = glm::translate(modeloldCPU14, glm::vec3(-49.072f, 14.443f, 47.706f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU14));
		oldCPU.Draw(lightingShader);
		// CPU 15
		glm::mat4 modeloldCPU15(1.0f);
		modeloldCPU15 = glm::translate(modeloldCPU15, glm::vec3(-39.384f, 14.443f, 47.706f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU15));
		oldCPU.Draw(lightingShader);
		// CPU 16
		glm::mat4 modeloldCPU16(1.0f);
		modeloldCPU16 = glm::translate(modeloldCPU16, glm::vec3(-28.76f, 14.443f, 47.706f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU16));
		oldCPU.Draw(lightingShader);
		// CPU 17
		glm::mat4 modeloldCPU17(1.0f);
		modeloldCPU17 = glm::translate(modeloldCPU17, glm::vec3(-19.072f, 14.443f, 47.706f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU17));
		oldCPU.Draw(lightingShader);
		// ------------ monitores ------------
		// monitor 14
		glm::mat4 modeloldMonitor14(1.0f);
		modeloldMonitor14 = glm::translate(modeloldMonitor14, glm::vec3(-44.764f, 14.434f, 46.913f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor14));
		monitorOld.Draw(lightingShader);
		// monitor 15
		glm::mat4 modeloldMonitor15(1.0f);
		modeloldMonitor15 = glm::translate(modeloldMonitor15, glm::vec3(-34.541f, 14.434f, 46.913f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor15));
		monitorOld.Draw(lightingShader);
		// monitor 16
		glm::mat4 modeloldMonitor16(1.0f);
		modeloldMonitor16 = glm::translate(modeloldMonitor16, glm::vec3(-24.452f, 14.434f, 46.913f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor16));
		monitorOld.Draw(lightingShader);
		// monitor 17
		glm::mat4 modeloldMonitor17(1.0f);
		modeloldMonitor17 = glm::translate(modeloldMonitor17, glm::vec3(-14.616f, 14.434f, 46.913f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor17));
		monitorOld.Draw(lightingShader);
		// -------- teclados -----------------
		// teclado 14
		glm::mat4 modeloldTeclado14(1.0f);
		modeloldTeclado14 = glm::translate(modeloldTeclado14, glm::vec3(-45.233f, 12.299f, 49.709f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado14));
		tecladoOld.Draw(lightingShader);
		// teclado 15
		glm::mat4 modeloldTeclado15(1.0f);
		modeloldTeclado15 = glm::translate(modeloldTeclado15, glm::vec3(-35.143f, 12.299f, 49.532f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado15));
		tecladoOld.Draw(lightingShader);
		// teclado 16
		glm::mat4 modeloldTeclado16(1.0f);
		modeloldTeclado16 = glm::translate(modeloldTeclado16, glm::vec3(-24.92f, 12.299f, 49.532f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado16));
		tecladoOld.Draw(lightingShader);
		// teclado 17
		glm::mat4 modeloldTeclado17(1.0f);
		modeloldTeclado17 = glm::translate(modeloldTeclado17, glm::vec3(-14.831f, 12.299f, 49.709f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado17));
		tecladoOld.Draw(lightingShader);
		// ---------- mouses -------------------
		// mouse 14
		glm::mat4 modeloldMouse14(1.0f);
		modeloldMouse14 = glm::translate(modeloldMouse14, glm::vec3(-43.789f, 12.405f, 48.202f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse14));
		mouseOld.Draw(lightingShader);
		// mouse 15
		glm::mat4 modeloldMouse15(1.0f);
		modeloldMouse15 = glm::translate(modeloldMouse15, glm::vec3(-33.594f, 12.405f, 48.202f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse15));
		mouseOld.Draw(lightingShader);
		// mouse 16
		glm::mat4 modeloldMouse16(1.0f);
		modeloldMouse16 = glm::translate(modeloldMouse16, glm::vec3(-23.476f, 12.405f, 48.202f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse16));
		mouseOld.Draw(lightingShader);
		// mouse 17
		glm::mat4 modeloldMouse17(1.0f);
		modeloldMouse17 = glm::translate(modeloldMouse17, glm::vec3(-13.281f, 12.405f, 48.202f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse17));
		mouseOld.Draw(lightingShader);
		// ----------- sillas ----------------------------
		// silla 14
		glm::mat4 modeloldSilla14(1.0f);
		modeloldSilla14 = glm::translate(modeloldSilla14, glm::vec3(-45.472f, 6.377f, 56.301f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla14));
		sillaOld.Draw(lightingShader);
		// silla 15
		glm::mat4 modeloldSilla15(1.0f);
		modeloldSilla15 = glm::translate(modeloldSilla15, glm::vec3(-35.524f, 6.377f, 56.301f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla15));
		sillaOld.Draw(lightingShader);
		// silla 16
		glm::mat4 modeloldSilla16(1.0f);
		modeloldSilla16 = glm::translate(modeloldSilla16, glm::vec3(-25.261f, 6.377f, 56.301f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla16));
		sillaOld.Draw(lightingShader);
		// silla 17
		glm::mat4 modeloldSilla17(1.0f);
		modeloldSilla17 = glm::translate(modeloldSilla17, glm::vec3(-15.112f, 6.377f, 56.301f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla17));
		sillaOld.Draw(lightingShader);


		// ---------------------------------------------------------------------------------

		// ---------------------------------------------------------------------------------
		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ------------- Modelado fila 1 columna 2 ----------------------------------------------
		
		
		// Escritorio 10
		glm::mat4 modelOldMesa10(1.0f);
		modelOldMesa10 = glm::translate(modelOldMesa10, glm::vec3(18.113f, 6.758f, -28.937f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa10));
		mesaOld.Draw(lightingShader);
		// Escritorio 11
		glm::mat4 modelOldMesa11(1.0f);
		modelOldMesa11 = glm::translate(modelOldMesa11, glm::vec3(38.431f, 6.758f, -28.937f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa11));
		mesaOld.Draw(lightingShader);

		// ----------- CPUs --------------------
		// CPU 18
		glm::mat4 modeloldCPU18(1.0f);
		modeloldCPU18 = glm::translate(modeloldCPU18, glm::vec3(15.345f, 14.443f, -29.801f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU18));
		oldCPU.Draw(lightingShader);
		// CPU 19
		glm::mat4 modeloldCPU19(1.0f);
		modeloldCPU19 = glm::translate(modeloldCPU19, glm::vec3(24.0f, 14.443f, -29.801f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU19));
		oldCPU.Draw(lightingShader);
		// CPU 20
		glm::mat4 modeloldCPU20(1.0f);
		modeloldCPU20 = glm::translate(modeloldCPU20, glm::vec3(32.721f, 14.443f, -29.801f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU20));
		oldCPU.Draw(lightingShader);
		// CPU 21
		glm::mat4 modeloldCPU21(1.0f);
		modeloldCPU21 = glm::translate(modeloldCPU21, glm::vec3(41.355f, 14.443f, -29.801f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU21));
		oldCPU.Draw(lightingShader);
		// CPU 22
		glm::mat4 modeloldCPU22(1.0f);
		modeloldCPU22 = glm::translate(modeloldCPU22, glm::vec3(49.108f, 3.487f, -29.801f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU22));
		oldCPU.Draw(lightingShader);
		// ------------ monitores ------------
		// monitor 18
		glm::mat4 modeloldMonitor18(1.0f);
		modeloldMonitor18 = glm::translate(modeloldMonitor18, glm::vec3(10.916f, 14.434f, -30.734f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor18));
		monitorOld.Draw(lightingShader);
		// monitor 19
		glm::mat4 modeloldMonitor19(1.0f);
		modeloldMonitor19 = glm::translate(modeloldMonitor19, glm::vec3(19.539f, 14.434f, -30.734f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor19));
		monitorOld.Draw(lightingShader);
		// monitor 20
		glm::mat4 modeloldMonitor20(1.0f);
		modeloldMonitor20 = glm::translate(modeloldMonitor20, glm::vec3(28.307f, 14.434f, -30.734f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor20));
		monitorOld.Draw(lightingShader);
		// monitor 21
		glm::mat4 modeloldMonitor21(1.0f);
		modeloldMonitor21 = glm::translate(modeloldMonitor21, glm::vec3(36.955f, 14.434f, -30.734f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor21));
		monitorOld.Draw(lightingShader);
		// monitor 22
		glm::mat4 modeloldMonitor22(1.0f);
		modeloldMonitor22 = glm::translate(modeloldMonitor22, glm::vec3(45.651f, 14.434f, -31.018f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor22));
		monitorOld.Draw(lightingShader);
		// -------- teclados -----------------
		// teclado 18
		glm::mat4 modeloldTeclado18(1.0f);
		modeloldTeclado18 = glm::translate(modeloldTeclado18, glm::vec3(10.874f, 12.299f, -27.775f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado18));
		tecladoOld.Draw(lightingShader);
		// teclado 19
		glm::mat4 modeloldTeclado19(1.0f);
		modeloldTeclado19 = glm::translate(modeloldTeclado19, glm::vec3(19.207f, 12.299f, -27.775f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado19));
		tecladoOld.Draw(lightingShader);
		// teclado 20
		glm::mat4 modeloldTeclado20(1.0f);
		modeloldTeclado20 = glm::translate(modeloldTeclado20, glm::vec3(27.908f, 12.299f, -27.775f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado20));
		tecladoOld.Draw(lightingShader);
		// teclado 21
		glm::mat4 modeloldTeclado21(1.0f);
		modeloldTeclado21 = glm::translate(modeloldTeclado21, glm::vec3(36.87f, 12.299f, -27.775f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado21));
		tecladoOld.Draw(lightingShader);
		// teclado 22
		glm::mat4 modeloldTeclado22(1.0f);
		modeloldTeclado22 = glm::translate(modeloldTeclado22, glm::vec3(45.076f, 12.299f, -28.491f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado22));
		tecladoOld.Draw(lightingShader);
		// ---------- mouses -------------------
		// mouse 18
		glm::mat4 modeloldMouse18(1.0f);
		modeloldMouse18 = glm::translate(modeloldMouse18, glm::vec3(12.026f, 12.405f, -29.305f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse18));
		mouseOld.Draw(lightingShader);
		// mouse 19
		glm::mat4 modeloldMouse19(1.0f);
		modeloldMouse19 = glm::translate(modeloldMouse19, glm::vec3(20.356f, 12.405f, -29.305f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse19));
		mouseOld.Draw(lightingShader);
		// mouse 20
		glm::mat4 modeloldMouse20(1.0f);
		modeloldMouse20 = glm::translate(modeloldMouse20, glm::vec3(29.09f, 12.405f, -29.619f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse20));
		mouseOld.Draw(lightingShader);
		// mouse 21
		glm::mat4 modeloldMouse21(1.0f);
		modeloldMouse21 = glm::translate(modeloldMouse21, glm::vec3(38.015f, 12.405f, -29.619f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse21));
		mouseOld.Draw(lightingShader);
		// mouse 22
		glm::mat4 modeloldMouse22(1.0f);
		modeloldMouse22 = glm::translate(modeloldMouse22, glm::vec3(46.045f, 12.405f, -29.305f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse22));
		mouseOld.Draw(lightingShader);
		// -------------- sillas ------------------------------
		// silla 18
		glm::mat4 modeloldSilla18(1.0f);
		modeloldSilla18 = glm::translate(modeloldSilla18, glm::vec3(11.455, 6.377f, -21.457f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla18));
		sillaOld.Draw(lightingShader);
		// silla 19
		glm::mat4 modeloldSilla19(1.0f);
		modeloldSilla19 = glm::translate(modeloldSilla19, glm::vec3(19.541f, 6.377f, -21.457f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla19));
		sillaOld.Draw(lightingShader);
		// silla 20
		glm::mat4 modeloldSilla20(1.0f);
		modeloldSilla20 = glm::translate(modeloldSilla20, glm::vec3(27.603f, 6.377f, -21.457f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla20));
		sillaOld.Draw(lightingShader);
		// silla 21
		glm::mat4 modeloldSilla21(1.0f);
		modeloldSilla21 = glm::translate(modeloldSilla21, glm::vec3(36.099f, 6.377f, -21.457f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla21));
		sillaOld.Draw(lightingShader);
		// silla 22
		glm::mat4 modeloldSilla22(1.0f);
		modeloldSilla22 = glm::translate(modeloldSilla22, glm::vec3(44.156f, 6.377f, -21.457f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla22));
		sillaOld.Draw(lightingShader);


		// ---------------------------------------------------------------------------------

		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ------------- Modelado fila 2 columna 2 ----------------------------------------------
		
		// Escritorio 12
		glm::mat4 modelOldMesa12(1.0f);
		modelOldMesa12 = glm::translate(modelOldMesa12, glm::vec3(18.113f, 6.758f, -2.779f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa12));
		mesaOld.Draw(lightingShader);
		// Escritorio 13
		glm::mat4 modelOldMesa13(1.0f);
		modelOldMesa13 = glm::translate(modelOldMesa13, glm::vec3(38.431f, 6.758f, -2.779f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa13));
		mesaOld.Draw(lightingShader);
		// ----------- CPUs --------------------
		// CPU 23
		glm::mat4 modeloldCPU23(1.0f);
		modeloldCPU23 = glm::translate(modeloldCPU23, glm::vec3(15.345f, 14.443f, -3.545f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU23));
		oldCPU.Draw(lightingShader);
		// CPU 24
		glm::mat4 modeloldCPU24(1.0f);
		modeloldCPU24 = glm::translate(modeloldCPU24, glm::vec3(24.0f, 14.443f, -3.545f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU24));
		oldCPU.Draw(lightingShader);
		// CPU 25
		glm::mat4 modeloldCPU25(1.0f);
		modeloldCPU25 = glm::translate(modeloldCPU25, glm::vec3(32.721f, 14.443f, -3.545f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU25));
		oldCPU.Draw(lightingShader);
		// CPU 26
		glm::mat4 modeloldCPU26(1.0f);
		modeloldCPU26 = glm::translate(modeloldCPU26, glm::vec3(41.355f, 14.443f, -3.545f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU26));
		oldCPU.Draw(lightingShader);
		// CPU 27
		glm::mat4 modeloldCPU27(1.0f);
		modeloldCPU27 = glm::translate(modeloldCPU27, glm::vec3(49.108f, 3.487f, -3.545f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU27));
		oldCPU.Draw(lightingShader);
		// ------------ monitores ------------
		// monitor 23
		glm::mat4 modeloldMonitor23(1.0f);
		modeloldMonitor23 = glm::translate(modeloldMonitor23, glm::vec3(10.916f, 14.434f, -4.479f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor23));
		monitorOld.Draw(lightingShader);
		// monitor 24
		glm::mat4 modeloldMonitor24(1.0f);
		modeloldMonitor24 = glm::translate(modeloldMonitor24, glm::vec3(19.539f, 14.434f, -4.479f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor24));
		monitorOld.Draw(lightingShader);
		// monitor 25
		glm::mat4 modeloldMonitor25(1.0f);
		modeloldMonitor25 = glm::translate(modeloldMonitor25, glm::vec3(28.307f, 14.434f, -4.479f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor25));
		monitorOld.Draw(lightingShader);
		// monitor 26
		glm::mat4 modeloldMonitor26(1.0f);
		modeloldMonitor26 = glm::translate(modeloldMonitor26, glm::vec3(36.955f, 14.434f, -4.479f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor26));
		monitorOld.Draw(lightingShader);
		// monitor 27
		glm::mat4 modeloldMonitor27(1.0f);
		modeloldMonitor27 = glm::translate(modeloldMonitor27, glm::vec3(45.651f, 14.434f, -4.763f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor27));
		monitorOld.Draw(lightingShader);
		// -------- teclados -----------------
		// teclado 23
		glm::mat4 modeloldTeclado23(1.0f);
		modeloldTeclado23 = glm::translate(modeloldTeclado23, glm::vec3(10.874f, 12.299f, -1.519f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado23));
		tecladoOld.Draw(lightingShader);
		// teclado 24
		glm::mat4 modeloldTeclado24(1.0f);
		modeloldTeclado24 = glm::translate(modeloldTeclado24, glm::vec3(19.207f, 12.299f, -1.519f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado24));
		tecladoOld.Draw(lightingShader);
		// teclado 25
		glm::mat4 modeloldTeclado25(1.0f);
		modeloldTeclado25 = glm::translate(modeloldTeclado25, glm::vec3(27.908f, 12.299f, -1.519f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado25));
		tecladoOld.Draw(lightingShader);
		// teclado 26
		glm::mat4 modeloldTeclado26(1.0f);
		modeloldTeclado26 = glm::translate(modeloldTeclado26, glm::vec3(36.87f, 12.299f, -1.519f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado26));
		tecladoOld.Draw(lightingShader);
		// teclado 27
		glm::mat4 modeloldTeclado27(1.0f);
		modeloldTeclado27 = glm::translate(modeloldTeclado27, glm::vec3(45.076f, 12.299f, -2.226f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado27));
		tecladoOld.Draw(lightingShader);
		// ---------- mouses -------------------
		// mouse 23
		glm::mat4 modeloldMouse23(1.0f);
		modeloldMouse23 = glm::translate(modeloldMouse23, glm::vec3(12.026f, 12.405f, -3.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse23));
		mouseOld.Draw(lightingShader);
		// mouse 24
		glm::mat4 modeloldMouse24(1.0f);
		modeloldMouse24 = glm::translate(modeloldMouse24, glm::vec3(20.356f, 12.405f, -3.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse24));
		mouseOld.Draw(lightingShader);
		// mouse 25
		glm::mat4 modeloldMouse25(1.0f);
		modeloldMouse25 = glm::translate(modeloldMouse25, glm::vec3(29.09f, 12.405f, -3.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse25));
		mouseOld.Draw(lightingShader);
		// mouse 26
		glm::mat4 modeloldMouse26(1.0f);
		modeloldMouse26 = glm::translate(modeloldMouse26, glm::vec3(38.015f, 12.405f, -3.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse26));
		mouseOld.Draw(lightingShader);
		// mouse 27
		glm::mat4 modeloldMouse27(1.0f);
		modeloldMouse27 = glm::translate(modeloldMouse27, glm::vec3(46.045f, 12.405f, -3.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse27));
		mouseOld.Draw(lightingShader);
		// -------------- sillas ------------------------------
		// silla 23
		glm::mat4 modeloldSilla23(1.0f);
		modeloldSilla23 = glm::translate(modeloldSilla23, glm::vec3(11.455, 6.377f, 5.179f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla23));
		sillaOld.Draw(lightingShader);
		// silla 24
		glm::mat4 modeloldSilla24(1.0f);
		modeloldSilla24 = glm::translate(modeloldSilla24, glm::vec3(19.541f, 6.377f, 5.179f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla24));
		sillaOld.Draw(lightingShader);
		// silla 25
		glm::mat4 modeloldSilla25(1.0f);
		modeloldSilla25 = glm::translate(modeloldSilla25, glm::vec3(27.603f, 6.377f, 5.179f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla25));
		sillaOld.Draw(lightingShader);
		// silla 26
		glm::mat4 modeloldSilla26(1.0f);
		modeloldSilla26 = glm::translate(modeloldSilla26, glm::vec3(36.099f, 6.377f, 5.179f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla26));
		sillaOld.Draw(lightingShader);
		// silla 27
		glm::mat4 modeloldSilla27(1.0f);
		modeloldSilla27 = glm::translate(modeloldSilla27, glm::vec3(44.156f, 6.377f, 5.179f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla27));
		sillaOld.Draw(lightingShader);
		// ---------------------------------------------------------------------------------

		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ------------- Modelado fila 3 columna 2 ----------------------------------------------
		// Escritorio 14
		glm::mat4 modelOldMesa14(1.0f);
		modelOldMesa14 = glm::translate(modelOldMesa14, glm::vec3(18.113f, 6.758f, 22.489f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa14));
		mesaOld.Draw(lightingShader);
		// Escritorio 15
		glm::mat4 modelOldMesa15(1.0f);
		modelOldMesa15 = glm::translate(modelOldMesa15, glm::vec3(38.431f, 6.758f, 22.489f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa15));
		mesaOld.Draw(lightingShader);
		// ----------- CPUs --------------------
		// CPU 28
		glm::mat4 modeloldCPU28(1.0f);
		modeloldCPU28 = glm::translate(modeloldCPU28, glm::vec3(15.345f, 14.443f, 21.519f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU28));
		oldCPU.Draw(lightingShader);
		// CPU 29
		glm::mat4 modeloldCPU29(1.0f);
		modeloldCPU29 = glm::translate(modeloldCPU29, glm::vec3(24.0f, 14.443f, 21.519f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU29));
		oldCPU.Draw(lightingShader);
		// CPU 30
		glm::mat4 modeloldCPU30(1.0f);
		modeloldCPU30 = glm::translate(modeloldCPU30, glm::vec3(32.721f, 14.443f, 21.519f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU30));
		oldCPU.Draw(lightingShader);
		// CPU 31
		glm::mat4 modeloldCPU31(1.0f);
		modeloldCPU31 = glm::translate(modeloldCPU31, glm::vec3(41.355f, 14.443f, 21.519f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU31));
		oldCPU.Draw(lightingShader);
		// CPU 32
		glm::mat4 modeloldCPU32(1.0f);
		modeloldCPU32 = glm::translate(modeloldCPU32, glm::vec3(49.108f, 3.487f, 21.519f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU32));
		oldCPU.Draw(lightingShader);
		// ------------ monitores ------------
		// monitor 28
		glm::mat4 modeloldMonitor28(1.0f);
		modeloldMonitor28 = glm::translate(modeloldMonitor28, glm::vec3(10.916f, 14.434f, 20.585f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor28));
		monitorOld.Draw(lightingShader);
		// monitor 29
		glm::mat4 modeloldMonitor29(1.0f);
		modeloldMonitor29 = glm::translate(modeloldMonitor29, glm::vec3(19.539f, 14.434f, 20.585f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor29));
		monitorOld.Draw(lightingShader);
		// monitor 30
		glm::mat4 modeloldMonitor30(1.0f);
		modeloldMonitor30 = glm::translate(modeloldMonitor30, glm::vec3(28.307f, 14.434f, 20.585f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor30));
		monitorOld.Draw(lightingShader);
		// monitor 31
		glm::mat4 modeloldMonitor31(1.0f);
		modeloldMonitor31 = glm::translate(modeloldMonitor31, glm::vec3(36.955f, 14.434f, 20.585f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor31));
		monitorOld.Draw(lightingShader);
		// monitor 32
		glm::mat4 modeloldMonitor32(1.0f);
		modeloldMonitor32 = glm::translate(modeloldMonitor32, glm::vec3(45.651f, 14.434f, 20.301f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor32));
		monitorOld.Draw(lightingShader);
		// -------- teclados -----------------
		// teclado 28
		glm::mat4 modeloldTeclado28(1.0f);
		modeloldTeclado28 = glm::translate(modeloldTeclado28, glm::vec3(10.874f, 12.299f, 23.545f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado28));
		tecladoOld.Draw(lightingShader);
		// teclado 24
		glm::mat4 modeloldTeclado29(1.0f);
		modeloldTeclado29 = glm::translate(modeloldTeclado29, glm::vec3(19.207f, 12.299f, 23.545f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado29));
		tecladoOld.Draw(lightingShader);
		// teclado 30
		glm::mat4 modeloldTeclado30(1.0f);
		modeloldTeclado30 = glm::translate(modeloldTeclado30, glm::vec3(27.908f, 12.299f, 23.545f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado30));
		tecladoOld.Draw(lightingShader);
		// teclado 31
		glm::mat4 modeloldTeclado31(1.0f);
		modeloldTeclado31 = glm::translate(modeloldTeclado31, glm::vec3(36.87f, 12.299f, 23.545f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado31));
		tecladoOld.Draw(lightingShader);
		// teclado 32
		glm::mat4 modeloldTeclado32(1.0f);
		modeloldTeclado32 = glm::translate(modeloldTeclado32, glm::vec3(45.076f, 12.299f, 22.838f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado32));
		tecladoOld.Draw(lightingShader);
		// ---------- mouses -------------------
		// mouse 28
		glm::mat4 modeloldMouse28(1.0f);
		modeloldMouse28 = glm::translate(modeloldMouse28, glm::vec3(12.026f, 12.405f, 22.014f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse28));
		mouseOld.Draw(lightingShader);
		// mouse 29
		glm::mat4 modeloldMouse29(1.0f);
		modeloldMouse29 = glm::translate(modeloldMouse29, glm::vec3(20.356f, 12.405f, 21.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse29));
		mouseOld.Draw(lightingShader);
		// mouse 30
		glm::mat4 modeloldMouse30(1.0f);
		modeloldMouse30 = glm::translate(modeloldMouse30, glm::vec3(29.09f, 12.405f, 22.014f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse30));
		mouseOld.Draw(lightingShader);
		// mouse 31
		glm::mat4 modeloldMouse31(1.0f);
		modeloldMouse31 = glm::translate(modeloldMouse31, glm::vec3(38.015f, 12.405f, 21.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse31));
		mouseOld.Draw(lightingShader);
		// mouse 32
		glm::mat4 modeloldMouse32(1.0f);
		modeloldMouse32 = glm::translate(modeloldMouse32, glm::vec3(46.045f, 12.405f, 22.014f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse32));
		mouseOld.Draw(lightingShader);
		// -------------- sillas ------------------------------
		// silla 28
		glm::mat4 modeloldSilla28(1.0f);
		modeloldSilla28 = glm::translate(modeloldSilla28, glm::vec3(11.455, 6.377f, 30.964f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla28));
		sillaOld.Draw(lightingShader);
		// silla 29
		glm::mat4 modeloldSilla29(1.0f);
		modeloldSilla29 = glm::translate(modeloldSilla29, glm::vec3(19.541f, 6.377f, 30.964f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla29));
		sillaOld.Draw(lightingShader);
		// silla 30
		glm::mat4 modeloldSilla30(1.0f);
		modeloldSilla30 = glm::translate(modeloldSilla30, glm::vec3(27.603f, 6.377f, 30.964f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla30));
		sillaOld.Draw(lightingShader);
		// silla 31
		glm::mat4 modeloldSilla31(1.0f);
		modeloldSilla31 = glm::translate(modeloldSilla31, glm::vec3(36.099f, 6.377f, 30.964f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla31));
		sillaOld.Draw(lightingShader);
		// silla 32
		glm::mat4 modeloldSilla32(1.0f);
		modeloldSilla32 = glm::translate(modeloldSilla32, glm::vec3(44.156f, 6.377f, 30.964f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla32));
		sillaOld.Draw(lightingShader);
		// ---------------------------------------------------------------------------------

		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ------------- Modelado fila 3 columna 2 ----------------------------------------------
		// Escritorio 16
		glm::mat4 modelOldMesa16(1.0f);
		modelOldMesa16 = glm::translate(modelOldMesa16, glm::vec3(18.113f, 6.758f, 48.24f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa16));
		mesaOld.Draw(lightingShader);
		// Escritorio 17
		glm::mat4 modelOldMesa17(1.0f);
		modelOldMesa17 = glm::translate(modelOldMesa17, glm::vec3(38.431f, 6.758f, 48.24f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa17));
		mesaOld.Draw(lightingShader);
		// ----------- CPUs --------------------
		// CPU 33
		glm::mat4 modeloldCPU33(1.0f);
		modeloldCPU33 = glm::translate(modeloldCPU33, glm::vec3(15.345f, 14.443f, 47.706f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU33));
		oldCPU.Draw(lightingShader);
		// CPU 34
		glm::mat4 modeloldCPU34(1.0f);
		modeloldCPU34 = glm::translate(modeloldCPU34, glm::vec3(24.0f, 14.443f, 47.706f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU34));
		oldCPU.Draw(lightingShader);
		// CPU 35
		glm::mat4 modeloldCPU35(1.0f);
		modeloldCPU35 = glm::translate(modeloldCPU35, glm::vec3(32.721f, 14.443f, 47.706f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU35));
		oldCPU.Draw(lightingShader);
		// CPU 36
		glm::mat4 modeloldCPU36(1.0f);
		modeloldCPU36 = glm::translate(modeloldCPU36, glm::vec3(41.355f, 14.443f, 47.006f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU36));
		oldCPU.Draw(lightingShader);
		// CPU 37
		glm::mat4 modeloldCPU37(1.0f);
		modeloldCPU37 = glm::translate(modeloldCPU37, glm::vec3(49.108f, 3.487f, 47.706f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU37));
		oldCPU.Draw(lightingShader);
		// ------------ monitores ------------
		// monitor 33
		glm::mat4 modeloldMonitor33(1.0f);
		modeloldMonitor33 = glm::translate(modeloldMonitor33, glm::vec3(10.916f, 14.434f, 46.773f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor33));
		monitorOld.Draw(lightingShader);
		// monitor 34
		glm::mat4 modeloldMonitor34(1.0f);
		modeloldMonitor34 = glm::translate(modeloldMonitor34, glm::vec3(19.539f, 14.434f, 46.773f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor34));
		monitorOld.Draw(lightingShader);
		// monitor 35
		glm::mat4 modeloldMonitor35(1.0f);
		modeloldMonitor35 = glm::translate(modeloldMonitor35, glm::vec3(28.307f, 14.434f, 46.773f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor35));
		monitorOld.Draw(lightingShader);
		// monitor 36
		glm::mat4 modeloldMonitor36(1.0f);
		modeloldMonitor36 = glm::translate(modeloldMonitor36, glm::vec3(36.955f, 14.434f, 46.773f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor36));
		monitorOld.Draw(lightingShader);
		// monitor 37
		glm::mat4 modeloldMonitor37(1.0f);
		modeloldMonitor37 = glm::translate(modeloldMonitor37, glm::vec3(45.651f, 14.434f, 46.773f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor37));
		monitorOld.Draw(lightingShader);
		// -------- teclados -----------------
		// teclado 33
		glm::mat4 modeloldTeclado33(1.0f);
		modeloldTeclado33 = glm::translate(modeloldTeclado33, glm::vec3(10.874f, 12.299f, 49.732f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado33));
		tecladoOld.Draw(lightingShader);
		// teclado 34
		glm::mat4 modeloldTeclado34(1.0f);
		modeloldTeclado34 = glm::translate(modeloldTeclado34, glm::vec3(19.207f, 12.299f, 49.732f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado34));
		tecladoOld.Draw(lightingShader);
		// teclado 35
		glm::mat4 modeloldTeclado35(1.0f);
		modeloldTeclado35 = glm::translate(modeloldTeclado35, glm::vec3(27.908f, 12.299f, 49.732f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado35));
		tecladoOld.Draw(lightingShader);
		// teclado 36
		glm::mat4 modeloldTeclado36(1.0f);
		modeloldTeclado36 = glm::translate(modeloldTeclado36, glm::vec3(36.87f, 12.299f, 49.732f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado36));
		tecladoOld.Draw(lightingShader);
		// teclado 37
		glm::mat4 modeloldTeclado37(1.0f);
		modeloldTeclado37 = glm::translate(modeloldTeclado37, glm::vec3(45.076f, 12.299f, 49.025f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado37));
		tecladoOld.Draw(lightingShader);
		// ---------- mouses -------------------
		// mouse 33
		glm::mat4 modeloldMouse33(1.0f);
		modeloldMouse33 = glm::translate(modeloldMouse33, glm::vec3(12.026f, 12.405f, 48.202f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse33));
		mouseOld.Draw(lightingShader);
		// mouse 34
		glm::mat4 modeloldMouse34(1.0f);
		modeloldMouse34 = glm::translate(modeloldMouse34, glm::vec3(20.356f, 12.405f, 47.888f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse34));
		mouseOld.Draw(lightingShader);
		// mouse 35
		glm::mat4 modeloldMouse35(1.0f);
		modeloldMouse35 = glm::translate(modeloldMouse35, glm::vec3(29.09f, 12.405f, 47.888f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse35));
		mouseOld.Draw(lightingShader);
		// mouse 36
		glm::mat4 modeloldMouse36(1.0f);
		modeloldMouse36 = glm::translate(modeloldMouse36, glm::vec3(38.015f, 12.405f, 47.888f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse36));
		mouseOld.Draw(lightingShader);
		// mouse 37
		glm::mat4 modeloldMouse37(1.0f);
		modeloldMouse37 = glm::translate(modeloldMouse37, glm::vec3(46.045f, 12.405f, 48.202f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse37));
		mouseOld.Draw(lightingShader);
		// -------------- sillas ------------------------------
		// silla 33
		glm::mat4 modeloldSilla33(1.0f);
		modeloldSilla33 = glm::translate(modeloldSilla33, glm::vec3(11.455, 6.377f, 56.301f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla33));
		sillaOld.Draw(lightingShader);
		// silla 34
		glm::mat4 modeloldSilla34(1.0f);
		modeloldSilla34 = glm::translate(modeloldSilla34, glm::vec3(19.541f, 6.377f, 56.301f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla34));
		sillaOld.Draw(lightingShader);
		// silla 35
		glm::mat4 modeloldSilla35(1.0f);
		modeloldSilla35 = glm::translate(modeloldSilla35, glm::vec3(27.603f, 6.377f, 56.301f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla35));
		sillaOld.Draw(lightingShader);
		// silla 36
		glm::mat4 modeloldSilla36(1.0f);
		modeloldSilla36 = glm::translate(modeloldSilla36, glm::vec3(36.099f, 6.377f, 56.301f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla36));
		sillaOld.Draw(lightingShader);
		// silla 37
		glm::mat4 modeloldSilla37(1.0f);
		modeloldSilla37 = glm::translate(modeloldSilla37, glm::vec3(44.156f, 6.377f, 56.301f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla37));
		sillaOld.Draw(lightingShader);

		// -----------------------------------------------------------------
		// ventanas con transparencia
		glm::mat4 modelVentanasOld(1.0f);
		glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelVentanasOld));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		modelCuartoOld = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		modelCuartoOld = glm::translate(modelCuartoOld, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelVentanasOld));
		ventanasOld.Draw(lightingShader);
		glDisable(GL_BLEND);
		glBindVertexArray(0);
		// Puerta del salón
		glm::mat4 modelPuertaOld(1.0f);
		glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPuertaOld));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		modelPuertaOld = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		modelPuertaOld = glm::translate(modelCuartoOld, glm::vec3(-51.728f, 12.277f, -67.497f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPuertaOld));
		puertaOld.Draw(lightingShader);
		glDisable(GL_BLEND);
		glBindVertexArray(0);
		// =========================================================================================
		// ================================ FIN MODELADO LAB VIEJO =================================
		// =========================================================================================

	
		}
		

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
			
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
	if (keys[GLFW_KEY_N])
	{
		AnimBall = !AnimBall;
		
	}
	//Nueva tecla para intercambiar entre escenarios
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
			mostrarEscenarioNuevo = !mostrarEscenarioNuevo;
			std::cout << "Escenario actual: " << (mostrarEscenarioNuevo ? "Nuevo" : "Viejo") << std::endl;

		}


}


void Animation() {
	if (AnimBall)
	{
		rotBall += 0.5f * animationSpeed;  // Ralentiza la rotación de la pelota
		printf("Angulo de rotacion:%f\n", rotBall);

		//Resetear las rotaciónes de la pelota y el perro
		if (rotBall >= 360) {
			rotBall = 0;
		}

		// Control de rotación de la pelota
		if (rotBall >= 120 && rotBall < 150) {
			rot -= 0.1 * animationSpeed;  // Ralentiza el movimiento de rotación del perro
		}
		else if (rotBall >= 150 && rotBall < 165) {
			rot += 0.2 * animationSpeed;
		}
		else if (rotBall >= 300 && rotBall < 330) {//cambio
			rot -= 0.1 * animationSpeed;
		}
		else if (rotBall >= 330 && rotBall < 345) {
			rot += 0.2 * animationSpeed;
		}
		else {
			rot = 0;
		}

		// Control de salto (más lento o rápido dependiendo de animationSpeed)
		if (rotBall >= 160 && rotBall <= 180) {
			salto += 0.03 * animationSpeed;  // Ajusta la altura del salto
		}
		else if (rotBall > 180 && rotBall <= 200) {
			salto -= 0.03 * animationSpeed;  // Ajusta la altura del descenso del salto
		}
		else if (rotBall > 340) {
			salto += 0.03 * animationSpeed;  // Ajusta la altura del salto
		}
		else if (rotBall >= 0 && rotBall <= 20) {
			salto -= 0.03 * animationSpeed;  // Ajusta la altura del descenso
		}
		else {
			salto = 0;
		}
	}
	else
	{
		// Si la animación está desactivada, restablecer las variables
		rotBall = 0.0f;
	}
}


void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}