//Practica 9  Fuentes de luz					Mendoza Espinosa Ricardo
//Fecha de entrega : 04 - 04 - 2025    	 	319018370



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
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

//Arreglo de Localizacion de  las   4   fuentes   de   luces   de   tipo   puntual   (pointLights),
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.3f,-2.8f, -0.73f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};


//nueva implementacion de luces
glm::vec3 dirLightPos(-2.5f, 3.5f, -3.0f);
glm::vec3 lamparaPos(0.5f, 1.7f, 1.5f); // posición base de la lámpara

//Declaracion de  variables globales para los colores de dirLight:
glm::vec3 dirLightAmbient(0.08f, 0.08f, 0.08f);
glm::vec3 dirLightDiffuse(0.2f, 0.2f, 0.1f);
glm::vec3 dirLightSpecular(0.3f, 0.3f, 0.3f);


//Declaracion de  variables globales para los colores de pointLight:
// Intensidad de la PointLight
glm::vec3 pointAmbient(0.02f, 0.02f, 0.02f);
glm::vec3 pointDiffuse(0.05f, 0.05f, 0.05f);
glm::vec3 pointSpecular(1.0f, 1.0f, 0.0f);

// Atenuación
float pointConstant = 1.0f;
float pointLinear = 0.045f;
float pointQuadratic = 0.075f;

//Declaracion de variables globales spotllight

float spotCutOff = glm::cos(glm::radians(12.0f));
float spotOuterCutOff = glm::cos(glm::radians(18.0f));
glm::vec3 spotSpecular = glm::vec3(1.3f, 1.3f, 1.3f);  // brillo inicial




float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


//Se define para cada una de las fuentes la configuración que vamos a tener con el vector3
glm::vec3 Light1 = glm::vec3(1.0f, 1.0f, 0.0f); // luz verde-amarilla


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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 9 Fuentes de luz-Ricardo Mendoza", nullptr, nullptr);

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

	//Modelos a cargar

	//modelo pelota
	Model ball1((char*)"Models/ball.obj");// Modelo para iluminación ambiental
	//Modelo de shadow
	Model shadow((char*)"Models/Shadow.obj");// Modelo para iluminación difusa
	//Modelo de parque
	Model Parque((char*)"Models/parque2/Parque.obj");// Modelo para iluminación especular

	//Modelo de Camioneta
	Model Camioneta((char*)"Models/Camioneta/tripo_convert_ff643e8d-b55f-46d1-bdd7-a83812ff66e4.obj");
	//Modelo de Planta
	Model Planta((char*)"Models/Planta/eb_house_plant_03.obj");
	//Modelo de Tortuga
	Model Tortuga((char*)"Models/Tortuga/13103_pearlturtle_v1_l2.obj");
	//Modelo de Sonic
	Model Sonic((char*)"Models/STF_Sonic/sonic.obj");
	//Modelo de Helicoptero
	Model Helicop((char*)"Models/Helicoptero/helicoptero.obj");
	//Modelo de lampara
	Model Lampara((char*)"Models/Lampara/objLamp.obj");
	//Modelo de sol
	Model Sol((char*)"Models/sol1/sol.obj");
	//Modelo de foco
	Model Foco((char*)"Models/Foco/Foco2.obj");


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

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

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"), 1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Configuración de luces

		//DirLight

		glm::vec3 dirLightDirection = glm::normalize(glm::vec3(0.0f) - dirLightPos); // dirección desde el cubo hacia el centro
		glUniform3fv(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 1, glm::value_ptr(dirLightDirection));


		// Configurar la luz direccional- punto y spot (igual para todos los modelos)
		glUniform3fv(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1, glm::value_ptr(dirLightAmbient));
		glUniform3fv(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 1, glm::value_ptr(dirLightDiffuse));
		glUniform3fv(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1, glm::value_ptr(dirLightSpecular));


		//Point light 1- Configura las luces puntuales  
		glm::vec3 lightColor;//Genera un color dinámico y animado para la luz, cambiando con el tiempo usando sin(tiempo).
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));//asegura que los valores sean positivos (evitando luz negativa).
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		//ambient, diffuse, specular: definen cómo la luz interactúa con los materiales.
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);//posicion:esta en  0  la ubicación de la luz en el espacio 3D.

		//Pasamos las variaables globales 
		glUniform3fv(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 1, glm::value_ptr(pointAmbient));
		glUniform3fv(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 1, glm::value_ptr(pointDiffuse));
		glUniform3fv(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1, glm::value_ptr(pointSpecular));

		//Parámetros de atenuación (qué tan rápido se debilita la luz con la distancia).
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);



		 //Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.0f);

		// Point light 3		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.0f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);




		// SpotLight
		glUniform3fv(glGetUniformLocation(lightingShader.Program, "spotLight.position"), 1, glm::value_ptr(lamparaPos));//se cambio la ubicacion por la de la lampara
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 0.0f, -1.0f, 0.0f); // o puedes usar una dirección calculada
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3fv(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1, glm::value_ptr(spotSpecular));

		//Controla cómo se atenúa la luz con la distancia. Aquí no se atenúa (porque los coeficientes no se usan realmente si la luz está en cero).
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		//El ángulo de apertura del cono de luz,este  también está en 0 grados, lo cual hace que no se vea absolutamente nada, incluso si se encendiera.
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), spotCutOff);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), spotOuterCutOff);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 36.0f);//Propiedad de brillo





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

		//Carga de modelos 

		// Modelo de pelota
		view = camera.GetViewMatrix();      // Obtener la matriz de vista de la cámara
		//glEnable(GL_BLEND);  // Habilitar el blending para la transparencia
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Habilitar el blending para la transparencia
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));  // Establecer la matriz de transformación para el modelo
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);  // Enviar al shader el valor de transparencia (desactivado en este caso)
		ball1.Draw(lightingShader);  // Dibujar el modelo de la pelota con el shader actual
		// glDisable(GL_BLEND);  // Desactivar el blending (opcional)
		glBindVertexArray(0);  // Limpiar el estado de OpenGL desactivando el VAO



		//Modelo de Shadow
		glm::mat4 modelShadow(1); // Nueva matriz de modelo para Shadow
		modelShadow = glm::translate(modelShadow, glm::vec3(0.0f, -0.75f, 0.0f));  // Posicionar en el fondo
		modelShadow = glm::scale(modelShadow, glm::vec3(0.05f, 0.05f, 0.05f)); // Escala adecuada solo para Shadow
		modelShadow = glm::rotate(modelShadow, glm::radians(102.07f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotac0.0 Shadow
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelShadow));
		shadow.Draw(lightingShader);


		// Iluminación Especular - Modelo 3: Solo especular
		glm::mat4 modelParque = glm::mat4(1);
		modelParque = glm::translate(modelParque, glm::vec3(0.0f, 0.0f, 0.0f));  // Posicionar en el fondo
		modelParque = glm::scale(modelParque, glm::vec3(7.0f, 7.0f, 7.0f)); // Escala adecuada solo para Parque
		modelParque = glm::rotate(modelParque, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de Parque
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelParque));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "specularOnly"), 1); // Solo iluminación especular
		Parque.Draw(lightingShader);  // Dibuja el modelo con solo iluminación especular




		// Para el modelo de camioneta
		glm::mat4 modelCamioneta(1); // Nueva matriz de modelo para camioneta
		modelCamioneta = glm::translate(modelCamioneta, glm::vec3(-1.7f, -0.35f, 2.2f));  // Posicionar en el fondo
		modelCamioneta = glm::scale(modelCamioneta, glm::vec3(3.0f, 3.0f, 3.0f)); // Escala adecuada solo para camioneta
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCamioneta));
		Camioneta.Draw(lightingShader);


		// Para el modelo de Planta
		glm::mat4 modelPlanta(1); // Nueva matriz de modelo para Planta
		modelPlanta = glm::translate(modelPlanta, glm::vec3(2.0f, -1.5f, 0.5f));  // Posicionar en el fondo
		modelPlanta = glm::scale(modelPlanta, glm::vec3(0.02f, 0.02f, 0.02f)); // Escala adecuada solo para Planta
		modelPlanta = glm::rotate(modelPlanta, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de Planta
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPlanta));
		Planta.Draw(lightingShader);

		// Para el modelo de Tortuga
		glm::mat4 modelTortuga(1); // Nueva matriz de modelo para Tortuga
		modelTortuga = glm::translate(modelTortuga, glm::vec3(-2.0f, -1.5f, 0.0f));  // Posicionar en el fondo
		modelTortuga = glm::scale(modelTortuga, glm::vec3(0.05f, 0.05f, 0.05f)); // Escala adecuada solo para Tortuga
		modelTortuga = glm::rotate(modelTortuga, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación de Tortuga
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTortuga));
		Tortuga.Draw(lightingShader);

		// Para el modelo de Sonic
		glm::mat4 modelSonic(1); // Nueva matriz de modelo para Sonic
		modelSonic = glm::translate(modelSonic, glm::vec3(1.3f, -1.2f, 0.2f));  // Posicionar en el fondo
		modelSonic = glm::scale(modelSonic, glm::vec3(0.2f, 0.2f, 0.2f)); // Escala adecuada solo para Sonic
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSonic));
		Sonic.Draw(lightingShader);

		// Para el modelo del Helicoptero
		glm::mat4 modelHelicop(1); // Nueva matriz de modelo para Helicoptero
		modelHelicop = glm::translate(modelHelicop, glm::vec3(1.3f, 4.0f, 0.0f));  // Posicionar en el fondo
		modelHelicop = glm::scale(modelHelicop, glm::vec3(3.0f, 3.0f, 3.0f)); // Escala adecuada solo para Helicoptero
		modelHelicop = glm::rotate(modelHelicop, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de Helicoptero
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelHelicop));
		Helicop.Draw(lightingShader);




		// FUENTE DE LUZ SPOTLIGHT
		// Para el modelo del lampara
		// MATRIZ DE TRANSFORMACIÓN DE LA LÁMPARA
		glm::vec3 lamparaPosSec = glm::vec3(0.7f, -1.3f, 1.5f); // posición base de la lámpara

		glm::mat4 modelLampara(1);
		modelLampara = glm::translate(modelLampara, lamparaPosSec);
		modelLampara = glm::scale(modelLampara, glm::vec3(0.2f));
		modelLampara = glm::rotate(modelLampara, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLampara));
		Lampara.Draw(lightingShader);




		// FUENTE DE LUZ DIRLIGHT
		// Para el modelo del SOL
		// MATRIZ DE TRANSFORMACIÓN DE LA SOL
		glm::vec3 solPosSec = glm::vec3(-2.5f, 3.5f, -3.0f); // posición base de la SOL
		glm::mat4 modelSol(1);
		modelSol = glm::translate(modelSol, solPosSec);
		modelSol = glm::scale(modelSol, glm::vec3(2.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSol));
		Sol.Draw(lightingShader);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");


		//PARTE DE CODIGO LAS FUENTES DE LUZ (CAJAS BLANCAS)

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Draw the light object (using light's vertex attributes)
		
		//Aqui se usa el tipo de luz pointLigh

		for (GLuint i = 0; i < 4; i++) {
			glm::mat4 modelFoco(1.0f);
			modelFoco = glm::translate(modelFoco, pointLightPositions[0]); // usa la posición de la fuente de luz
			modelFoco = glm::scale(modelFoco, glm::vec3(0.2f)); // ajusta escala si es necesario
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelFoco));
			Foco.Draw(lightingShader); // ¡Aquí usas tu modelo de foco!

		}	
		//CUBO  SPOTLIGHT 
		model = glm::mat4(1);
		model = glm::translate(model, lamparaPos);
		model = glm::scale(model, glm::vec3(0.2f)); // mismo tamaño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// DIRLIGHT CUBE
		model = glm::mat4(1);
		model = glm::translate(model, dirLightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // mismo tamaño
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

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

	// Aumentar intensidad (tecla K)
	if (keys[GLFW_KEY_K]) {
		dirLightAmbient += glm::vec3(0.02f);
		dirLightDiffuse += glm::vec3(0.02f);
		dirLightSpecular += glm::vec3(0.02f);

		// Clamp: evitar que pasen de 1.0
		dirLightAmbient = glm::min(dirLightAmbient, glm::vec3(1.0f));
		dirLightDiffuse = glm::min(dirLightDiffuse, glm::vec3(1.0f));
		dirLightSpecular = glm::min(dirLightSpecular, glm::vec3(1.0f));
	}

	// Disminuir intensidad (tecla I)
	if (keys[GLFW_KEY_I]) {
		dirLightAmbient -= glm::vec3(0.02f);
		dirLightDiffuse -= glm::vec3(0.02f);
		dirLightSpecular -= glm::vec3(0.02f);

		// Clamp: evitar valores negativos
		dirLightAmbient = glm::max(dirLightAmbient, glm::vec3(0.0f));
		dirLightDiffuse = glm::max(dirLightDiffuse, glm::vec3(0.0f));
		dirLightSpecular = glm::max(dirLightSpecular, glm::vec3(0.0f));
	}

	// Subir intensidad y atenuación (tecla O)
	if (keys[GLFW_KEY_O]) {
		pointAmbient += glm::vec3(0.02f);
		pointDiffuse += glm::vec3(0.02f);
		pointSpecular += glm::vec3(0.02f);

		pointLinear += 0.01f;
		pointQuadratic += 0.01f;

		// Limita valores para que no exploten
		pointAmbient = glm::min(pointAmbient, glm::vec3(1.0f));
		pointDiffuse = glm::min(pointDiffuse, glm::vec3(1.0f));
		pointSpecular = glm::min(pointSpecular, glm::vec3(1.0f));
		pointLinear = glm::min(pointLinear, 1.0f);
		pointQuadratic = glm::min(pointQuadratic, 1.0f);
	}

	// Bajar intensidad y atenuación (tecla L)
	if (keys[GLFW_KEY_L]) {
		pointAmbient -= glm::vec3(0.02f);
		pointDiffuse -= glm::vec3(0.02f);
		pointSpecular -= glm::vec3(0.02f);

		pointLinear -= 0.01f;
		pointQuadratic -= 0.01f;

		// Limita para que no sean negativos
		pointAmbient = glm::max(pointAmbient, glm::vec3(0.0f));
		pointDiffuse = glm::max(pointDiffuse, glm::vec3(0.0f));
		pointSpecular = glm::max(pointSpecular, glm::vec3(0.0f));
		pointLinear = glm::max(pointLinear, 0.0f);
		pointQuadratic = glm::max(pointQuadratic, 0.0f);
	}
	// Aumentar brillo y ángulo del cono de la luz Spotlight (U)
	if (keys[GLFW_KEY_U]) {
		spotSpecular += glm::vec3(0.05f);
		spotSpecular = glm::min(spotSpecular, glm::vec3(1.0f));  // límite superior

		spotCutOff = glm::cos(glm::radians(glm::degrees(acos(spotCutOff)) + 1.0f));  // más abierto
		spotOuterCutOff = glm::cos(glm::radians(glm::degrees(acos(spotOuterCutOff)) + 1.0f));
	}

	// Disminuir brillo y cerrar el cono de luz Spotlight (J)
	if (keys[GLFW_KEY_J]) {
		spotSpecular -= glm::vec3(0.05f);
		spotSpecular = glm::max(spotSpecular, glm::vec3(0.0f));  // límite inferior

		spotCutOff = glm::cos(glm::radians(glm::degrees(acos(spotCutOff)) - 1.0f));  // más estrecho
		spotOuterCutOff = glm::cos(glm::radians(glm::degrees(acos(spotOuterCutOff)) - 1.0f));
	}


	


}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
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
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
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