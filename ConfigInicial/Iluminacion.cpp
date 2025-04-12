//Practica 8 (Materiales e iluminación) 	Mendoza Espinosa Ricardo
//Fecha de entrega : 28 - 03 - 2025    	 	319018370
 

// Std. Includes
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
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes

glm::vec3 lightPos(0.5f, 0.5f, 2.5f); // Primera luz

float movelightPos = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;

bool TiempoDIA = true; // Controla si es de día o noche
float lightAngle = 0.0f;
float lightRadius = 5.0f; // Radio de movimiento de la luz 

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8 (Materiales e iluminacion)-Ricardo Mendoza", nullptr, nullptr);

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
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

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

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");



    // Carga de  modelos

    //Modelo de luna
	Model luna((char*)"Models/moon/moon.obj");

    //Modelo de sol
	Model sol1((char*)"Models/sol1/sol.obj");

    //Perro rojo
    Model red_dog((char*)"Models/RedDog/RedDog.obj");

    //Modelo 2 de sonic
    Model sonic((char*)"Models/Sonic/SonicTH06Model.obj");
  
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
    
    //Modelo de Helicoptero
    Model Helicop((char*)"Models/Helicoptero/helicoptero.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

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

    // Load textures

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("Models/RedDog/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        lightPos.x = lightRadius * cos(glm::radians(lightAngle));
        lightPos.y = lightRadius * sin(glm::radians(lightAngle));
        lightPos.z = 0.0f; // Fijo


        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Using the lighting shader
        lightingShader.Use();
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"), lightPos.x, lightPos.y, lightPos.z);


       // Propiedades de luz según día o noche
       
        if (TiempoDIA) {
            // Sol más brillante
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.5f, 0.5f, 0.3f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.0f, 1.0f, 0.6f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 0.8f);
        }
        else {
            // Luna más brillante
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.3f, 0.3f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.6f, 0.6f, 1.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.6f, 0.6f, 1.0f);
        }

        glm::mat4 modelLight(1.0f);
        modelLight = glm::translate(modelLight, lightPos);
        modelLight = glm::scale(modelLight, glm::vec3(1.3f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLight));

        if (TiempoDIA) {
            sol1.Draw(lightingShader); //  carga el modelo `sol`
        }
        else {
            luna.Draw(lightingShader); // carga el modelo `luna`
        }


        // Set camera position
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.2f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.0f);



        // Draw the loaded model (red_dog)
        glm::mat4 model1(1);
        model1 = glm::scale(model1, glm::vec3(1.0f, 1.0f, 1.0f));
		model1 = glm::translate(model1, glm::vec3(1.5f, -0.8f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model1));
        red_dog.Draw(lightingShader);

        // Draw the second model (sonic)
        glm::mat4 modelsonic(1);
        modelsonic = glm::translate(modelsonic, glm::vec3(1.0f, -1.15f, 1.0f));
        modelsonic = glm::scale(modelsonic, glm::vec3(0.01f, 0.01f, 0.01f)); // Aquí se define la escala del modelo de Sonic
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelsonic));
        sonic.Draw(lightingShader);
  

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



        // Para el modelo del Helicoptero
        glm::mat4 modelHelicop(1); // Nueva matriz de modelo para Helicoptero
        modelHelicop = glm::translate(modelHelicop, glm::vec3(1.0f, 2.0f, 0.0f));  // Posicionar en el fondo
        modelHelicop = glm::scale(modelHelicop, glm::vec3(3.0f, 3.0f, 3.0f)); // Escala adecuada solo para Helicoptero
        modelHelicop = glm::rotate(modelHelicop, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de Helicoptero
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelHelicop));
        Helicop.Draw(shader);



        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36); // Dibujar cubo
        glBindVertexArray(0);


        glBindVertexArray(0);

 
        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{   //movimeinto de 0 a 138 grados
    if (keys[GLFW_KEY_O]) {
        if (lightAngle < 180.0f)
            lightAngle += 30.0f * deltaTime; // aumenta hasta 180°
    }
    if (keys[GLFW_KEY_L]) {
        if (lightAngle > 0.0f)
            lightAngle -= 30.0f * deltaTime; // disminuye hasta 0°
    }


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

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }

}

// Is called whenever a key is pressed/released via GLFW
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

    if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        TiempoDIA = !TiempoDIA;
    }


    // Movimiento para la primera fuente de luz (cubos 1) con las teclas "O" y "L"
    if (keys[GLFW_KEY_O]) // Mueve el primer cubo de luz hacia la derecha
    {
        lightPos.z += 0.1f;
    }

    if (keys[GLFW_KEY_L]) // Mueve el primer cubo de luz hacia la izquierda
    {
        lightPos.z -= 0.1f;
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


