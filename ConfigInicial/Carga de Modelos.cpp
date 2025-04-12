//Práctica 6 (Carga de modelos)                      	Mendoza Espinosa Ricardo
//Fecha de entrega : 07 - 03 - 2025    	 	                    319018370 



// // Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 1000, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );


// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 10.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Practica 6 (Carga de modelos)-Ricardo Mendoza", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shader/modelLoading.vs", "Shader/modelLoading.frag" );
    
    // Load models
    //Modelo de shadow
    Model shadow((char*)"Models/Shadow.obj");
	//Modelo de parque
    Model Parque((char*)"Models/parque2/Parque.obj");
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


    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    //Modelo de parque
    

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
       // Para el modelo de Shadow
        glm::mat4 modelShadow(1); // Nueva matriz de modelo para Shadow
        modelShadow = glm::translate(modelShadow, glm::vec3(0.0f, -0.75f, 0.0f));  // Posicionar en el fondo
        modelShadow = glm::scale(modelShadow, glm::vec3(0.05f, 0.05f, 0.05f)); // Escala adecuada solo para Shadow
        modelShadow = glm::rotate(modelShadow, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de Shadow
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelShadow));
        shadow.Draw(shader);

        // Para el modelo de Parque
        glm::mat4 modelParque(1); // Nueva matriz de modelo para Parque
        modelParque = glm::translate(modelParque, glm::vec3(0.0f, 0.0f, 0.0f));  // Posicionar en el fondo
        modelParque = glm::scale(modelParque, glm::vec3(7.0f, 7.0f, 7.0f)); // Escala adecuada solo para Parque
        modelParque = glm::rotate(modelParque, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de Parque
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelParque));
        Parque.Draw(shader);

        // Para el modelo de camioneta
        glm::mat4 modelCamioneta(1); // Nueva matriz de modelo para camioneta
        modelCamioneta = glm::translate(modelCamioneta, glm::vec3(-1.7f, -0.65f, 2.2f));  // Posicionar en el fondo
        modelCamioneta = glm::scale(modelCamioneta, glm::vec3(2.0f, 2.0f, 2.0f)); // Escala adecuada solo para camioneta
        modelCamioneta = glm::rotate(modelCamioneta, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de camioneta
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCamioneta));
        Camioneta.Draw(shader);


        // Para el modelo de Planta
        glm::mat4 modelPlanta(1); // Nueva matriz de modelo para Planta
        modelPlanta = glm::translate(modelPlanta, glm::vec3(2.0f, -1.5f, 0.5f));  // Posicionar en el fondo
        modelPlanta = glm::scale(modelPlanta, glm::vec3(0.02f, 0.02f, 0.02f)); // Escala adecuada solo para Planta
        modelPlanta = glm::rotate(modelPlanta, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de Planta
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPlanta));
        Planta.Draw(shader);

        // Para el modelo de Tortuga
        glm::mat4 modelTortuga(1); // Nueva matriz de modelo para Tortuga
        modelTortuga = glm::translate(modelTortuga, glm::vec3(-2.0f, -1.5f, 0.0f));  // Posicionar en el fondo
        modelTortuga = glm::scale(modelTortuga, glm::vec3(0.05f, 0.05f, 0.05f)); // Escala adecuada solo para Tortuga
        modelTortuga = glm::rotate(modelTortuga, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación de Tortuga
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTortuga));
        Tortuga.Draw(shader);

        // Para el modelo de Sonic
        glm::mat4 modelSonic(1); // Nueva matriz de modelo para Sonic
        modelSonic = glm::translate(modelSonic, glm::vec3(1.3f, -1.2f, 0.2f));  // Posicionar en el fondo
        modelSonic = glm::scale(modelSonic, glm::vec3(0.2f, 0.2f, 0.2f)); // Escala adecuada solo para Sonic
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSonic));
        Sonic.Draw(shader);

        // Para el modelo del Helicoptero
        glm::mat4 modelHelicop(1); // Nueva matriz de modelo para Helicoptero
        modelHelicop = glm::translate(modelHelicop, glm::vec3(1.3f, 4.0f, 0.0f));  // Posicionar en el fondo
        modelHelicop = glm::scale(modelHelicop, glm::vec3(3.0f, 3.0f, 3.0f)); // Escala adecuada solo para Helicoptero
        modelHelicop = glm::rotate(modelHelicop, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de Helicoptero
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelHelicop));
        Helicop.Draw(shader);



        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }

   
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }

 

 
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

