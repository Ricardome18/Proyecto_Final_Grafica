


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
const GLuint WIDTH = 1300, HEIGHT = 900;
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
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

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

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	
	//Carga de modelos de Escenario Nuevo
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

	
		
		// --- Carga de modelos de escenario nuevo ---
		
		//Modelo cuarto
		glm::mat4 modelCuarto(1.0f);
		modelCuarto = glm::scale(modelCuarto, glm::vec3(1.0f, 1.0f, 0.9f));
		modelCuarto = glm::translate(modelCuarto, glm::vec3(0.351f, 17.922f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCuarto));
		Cuarto.Draw(lightingShader);

		//Modelo Profesor
		glm::mat4 modelProfesor(1.0f);
		modelProfesor = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		modelProfesor = glm::translate(modelProfesor, glm::vec3(3.868f, 11.692f,-54.0f));
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
		modelSillaProf = glm::scale(modelSillaProf, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSillaProf));
		Silla.Draw(lightingShader);


		glm::mat4 modelCPU1_Prof(1.0f);
		modelCPU1_Prof = glm::translate(modelCPU1_Prof, glm::vec3(47.0f,12.8f,-46.5f));
		modelCPU1_Prof = glm::scale(modelCPU1_Prof, glm::vec3(1.0f));
		modelCPU1_Prof = glm::rotate(modelCPU1_Prof, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCPU1_Prof));
		CPU_1.Draw(lightingShader);


		glm::mat4 modelTecladoProfe(1.0f);
		modelTecladoProfe = glm::translate(modelTecladoProfe, glm::vec3(46.0f, 10.0f, -51.0f));
		modelTecladoProfe = glm::rotate(modelTecladoProfe, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTecladoProfe = glm::scale(modelTecladoProfe, glm::vec3(1.1f, 1.1f, 1.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTecladoProfe));
		Teclado.Draw(lightingShader); // Mismo modelo "Teclado"
		
		
		glm::mat4 modelMouseProfe(1.0f);
		modelMouseProfe = glm::translate(modelMouseProfe, glm::vec3(41.0f, 9.9f, -51.0f));
		modelMouseProfe = glm::rotate(modelMouseProfe, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMouseProfe = glm::scale(modelMouseProfe, glm::vec3(1.0f));
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
				modelSilla = glm::scale(modelSilla, glm::vec3(1.0f));
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
				modelSilla = glm::scale(modelSilla, glm::vec3(1.0f));
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
				modelCPU1 = glm::scale(modelCPU1, glm::vec3(1.0f));
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
				modelCPU2 = glm::scale(modelCPU2, glm::vec3(1.0f));
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
				modelMon1 = glm::scale(modelMon1, glm::vec3(1.0f));
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
				modelMon2 = glm::scale(modelMon2, glm::vec3(1.0f));
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
				//modelMouse1 = glm::scale(modelMouse1, glm::vec3(5.0f));
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
				modelMouse2 = glm::scale(modelMouse2, glm::vec3(1.0f));
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