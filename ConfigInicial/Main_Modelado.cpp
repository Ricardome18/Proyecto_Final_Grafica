//Práctica 4	 	Mendoza Espinosa Ricardo
//Fecha de entrega : 27 - 02 - 2025    	 	319018370




#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 800, HEIGHT = 600;
float movX=0.0f;
float movY=0.0f;
float movZ=-5.0f;
float rot = 0.0f;
int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4-Ricardo Mendoza", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers

	

	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front X,Y,X ROJO VERDE Y AZUL
		0.5f, -0.5f, 0.5f,  1.0f, 0.0f,0.0f,//CADA CONJUNTO DE VERTICES COMPONE UN TRIANGULO QUE COMPONE UNA CARA DEL CUBO
		0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,//6 VERTICES SON NECARIO PARA CADA CARA 
		0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,//TENEMOS 2 TRIANGULOS POR CADA CARA
		-0.5f,  0.5f, 0.5f, 1.0f, 0.0f,0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,
		
	    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,//Back
		 0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	    -0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  -0.5f, 0.5f, 0.0f, 0.0f,1.0f,
      
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
		
		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.2f,0.5f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.2f,0.5f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
	};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fondo blanco
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
	//MATRIZ DE MODEL Y DE VISTA

		view = glm::translate(view, glm::vec3(movX,movY, movZ));//SE ENVIAN LOS NOMBRES DE LAS VARIABLES QUE VAN A TENER LOS VALORES DE CADA VISTA 
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint colorLoc = glGetUniformLocation(ourShader.Program, "color");

		//SE ENVIAN LOS SHADER POR LOS Uniforms
		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	

		glBindVertexArray(VAO);
		
		glBindVertexArray(VAO);
		//Se emepieza en esta seccion a dibujar el cuerpo 
		glUniform3f(colorLoc, 0.855f, 0.647f, 0.125f); // Goldenrod
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(6.0f, 10.0f, 3.0f));  // Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.0f, 0.6f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Detalles del cuerpo del lado izquierdo
		glUniform3f(colorLoc, 0.973f, 0.973f, 1.0f); // GhostWhite
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 4.0f, 3.0f));  // Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-1.5f, 1.3f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.627f, 0.322f, 0.176f); // Sienna
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));  // Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-1.5f, 5.3f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.973f, 0.973f, 1.0f); // GhostWhite
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));  // Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-1.5f, 6.0f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


				//Detalles del cuerpo del lado izquierdo
		glUniform3f(colorLoc, 0.973f, 0.973f, 1.0f); // GhostWhite
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 4.0f, 3.0f));  // Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-1.5f, 1.3f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.627f, 0.322f, 0.176f); // Sienna
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));  // Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-1.5f, 5.3f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.973f, 0.973f, 1.0f); // GhostWhite
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));  // Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-1.5f, 6.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Detalles del cuerpo del lado derecho
		glUniform3f(colorLoc, 0.973f, 0.973f, 1.0f); // GhostWhite
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 4.0f, 3.0f));  // Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(1.5f, 1.9f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.627f, 0.322f, 0.176f); // Sienna
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));  // Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(1.5f, 3.2f, 0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.973f, 0.973f, 1.0f); // GhostWhite
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));  // Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(1.5f, 2.5f, 0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);




		//Pierna 1 DERECHA
		glUniform3f(colorLoc, 0.227f, 0.255f, 0.776f); // Azul violaceo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.4f, 13.7f, 0.75f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.8f, -0.42f, -0.5f));  // Movimiento en X,Alruta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Pierna 2 Izquierda
		glUniform3f(colorLoc, 0.227f, 0.255f, 0.776f); // Azul violaceo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 13.6f, 3.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-1.5f, -0.42f, 0.0f));  // Posiciona la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//Brazo DERECHA
		glUniform3f(colorLoc, 0.855f, 0.647f, 0.125f); // Goldenrod
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 2.6f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(3.5f, 3.6f, -0.5f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.855f, 0.647f, 0.125f); // Goldenrod
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 2.6f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(4.5f, 3.0f, -0.5f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.855f, 0.647f, 0.125f); // Goldenrod
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 2.6f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(5.5f, 2.6f, -0.5f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

				
		glUniform3f(colorLoc, 0.855f, 0.647f, 0.125f); // Goldenrod
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 2.6f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(6.0f, 2.6f, -0.5f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.855f, 0.647f, 0.125f); // Goldenrod
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 5.6f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(6.0f, 1.6f, -0.5f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Mano de woody
		glUniform3f(colorLoc, 0.961f, 0.871f, 0.702f); // Wheat
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.5f, 1.0f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(2.1f, 12.3f, -0.4f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.6f, 1.0f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(3.3f, 12.3f, 0.5f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.6f, 1.0f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(3.3f, 13.3f, -0.4f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Brazo derecho

		glUniform3f(colorLoc, 0.855f, 0.647f, 0.125f); // Goldenrod
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 2.6f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-3.5f, 3.6f, -0.5f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.855f, 0.647f, 0.125f); // Goldenrod
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 2.6f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-4.5f, 3.0f, -0.5f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.855f, 0.647f, 0.125f); // Goldenrod
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.6f, 1.0f, 2.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-2.2f, 7.0f, 0.0f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.855f, 0.647f, 0.125f); // Goldenrod
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-5.5f, 3.2f,0.0f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glUniform3f(colorLoc, 0.855f, 0.647f, 0.125f); // Goldenrod
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-4.5f, 2.8f, 0.0f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Mano derecha
		glUniform3f(colorLoc, 0.961f, 0.871f, 0.702f); // Wheat
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-2.0f, 1.8f, 0.28f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//Corbata de woody
		glUniform3f(colorLoc, 0.502f, 0.0f, 0.0f); // Maroon
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(4.0f, 1.5f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.0f, 7.8f, -0.1f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//CABEZA de woody
		glUniform3f(colorLoc, 0.961f, 0.871f, 0.702f); // Wheat
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.0f, 3.63f, -0.1f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.961f, 0.871f, 0.702f); // Wheat
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.0f, 7.2f, 0.7f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.961f, 0.871f, 0.702f); // Wheat
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 5.0f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.0f, 3.2f, 1.9f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//CABELLO WOODY
		glUniform3f(colorLoc, 0.647f, 0.165f, 0.165f); // Brown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 3.2f, 0.9f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.0f, 5.3f, 1.0f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glUniform3f(colorLoc, 0.647f, 0.165f, 0.165f); // Brown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 2.2f, 0.9f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.0f, 8.0f, 0.1f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.647f, 0.165f, 0.165f); // Brown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 2.2f, 2.8f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.0f, 8.9f, 0.37f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Sombrero woody


		//costado izquierdo
		glUniform3f(colorLoc, 0.647f, 0.165f, 0.165f); // Brown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 2.2f, 2.8f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-1.0f, 8.9f, 0.37f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//costado derecho
		glUniform3f(colorLoc, 0.647f, 0.165f, 0.165f); // Brown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 2.2f, 2.8f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(1.0f, 8.9f, 0.37f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Cubo en medio
		glUniform3f(colorLoc, 0.647f, 0.165f, 0.165f); // Brown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(3.0f, 4.2f, 2.8f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.37f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Recuadro grande
		glUniform3f(colorLoc, 0.647f, 0.165f, 0.165f); // Brown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(8.0f, 2.0f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.0f, 10.5f, 0.37f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);




		//Recuadro mediano
		glUniform3f(colorLoc, 0.647f, 0.165f, 0.165f); // Brown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(6.0f, 2.0f, 1.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.0f, 10.5f, 1.0f));  // Movimiento en X,Altuta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//Funda pistola  Izquierdo
		glUniform3f(colorLoc, 0.545f, 0.271f, 0.075f); // SaddleBrown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 4.6f, 1.5f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-3.5f, -0.2f, 0.0f));  // Posiciona la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Pie 1 DERECHA
		glUniform3f(colorLoc, 0.545f, 0.271f, 0.075f); // SaddleBrown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.4f, 4.0f, 0.75f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.8f, -3.6f, -0.5f));  // Movimiento en X,Alruta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.722f, 0.525f, 0.043f); // DarkGoldenRod
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.4f, 1.0f, 0.75f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.8f, -16.9f, -0.5f));  // Movimiento en X,Alruta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.545f, 0.271f, 0.075f); // SaddleBrown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(1.5f, -23.7f, -0.5f));  // Movimiento en X,Alruta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.545f, 0.271f, 0.075f); // SaddleBrown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.75f, 0.75f, 0.75f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(1.5f, -23.7f, -0.5f));  // Movimiento en X,Alruta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Pie 2 Izquierda
		glUniform3f(colorLoc, 0.545f, 0.271f, 0.075f); // SaddleBrown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 4.0f, 3.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-1.5f, -3.6f, 0.0f));  // Posiciona la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.722f, 0.525f, 0.043f); // DarkGoldenRod
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 3.0f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-1.5f, -16.9f, 0.0f));  // Posiciona la pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glUniform3f(colorLoc, 0.545f, 0.271f, 0.075f); // SaddleBrown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.75f));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-1.5f, -17.8f, -1.5f));  // Movimiento en X,Alruta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glUniform3f(colorLoc, 0.545f, 0.271f, 0.075f); // SaddleBrown
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 3.0));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(-1.5f, -17.8f, 0.8f));  // Movimiento en X,Alruta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//Superficie de suelo
		glUniform3f(colorLoc, 0.871f, 0.722f, 0.529f); // BurlyWood
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(33.0f, 1.0f, 33.0));  // Tamaño de la pata/ Ancho, Alto, Profundidad
		model = glm::translate(model, glm::vec3(0.0f, -18.7f, 0.0f));  // Movimiento en X,Alruta posicion y Movimiento en Profundidad
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);

	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 //Se agrego esta fuincion para la entrada del teclado
 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);//se cierra la ventana con SCAPE
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)//TECLA D,SE LE SUMA A LA VARIAB
		 movX += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		 movY += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		 movY -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.1f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.1f;
 }


