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


struct BoneTransformationValues {
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
};

struct HumanoidModelInfo {
	BoneTransformationValues head;
	BoneTransformationValues torso;
	BoneTransformationValues rightArm;
	BoneTransformationValues subRightArm;
	BoneTransformationValues rightLeg;
	BoneTransformationValues subRightLeg;
	BoneTransformationValues leftArm;
	BoneTransformationValues subLeftArm;
	BoneTransformationValues leftLeg;
	BoneTransformationValues subLeftLeg;
};

struct HumanoidFrame {
	HumanoidModelInfo modelInfo;
	HumanoidModelInfo inc;
	int stepsToFrame = -1;
};

HumanoidModelInfo profesor;


// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

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


glm::vec3 Light1 = glm::vec3(0);
//Anim
float caminarTime = 0.0f;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;



//KeyFrames
float dogPosX , dogPosY , dogPosZ  ;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame {
	
	float rotDog;
	float rotDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int playAnimationIndex = -1;

int FrameIndex = 0;			//introducir datos
bool playCaminar = false;
bool play = false;
int playIndex = 0;


//Animacion 0 

const int caminarAnimationMaxKeyFrame = 9;
HumanoidFrame caminarAnimationKeyFrames[caminarAnimationMaxKeyFrame];
int caminarAnimationKeyIndex = -1;
int caminarAnimationMaxStep = -1;
int caminarAnimationCurrentStep = -1;



void startUpFrames() {

	int caminarDelay1 = 200;
	int caminarDelay2 = 90;

	caminarAnimationKeyFrames[0].modelInfo.torso.position.x = -100.0;
	caminarAnimationKeyFrames[0].modelInfo.torso.position.z = -60.0;
	caminarAnimationKeyFrames[0].modelInfo.torso.rotation.y = 90.0f;

	caminarAnimationKeyFrames[1].modelInfo.torso.position.x = 40.0;
	caminarAnimationKeyFrames[1].modelInfo.torso.position.z = -60.0;
	caminarAnimationKeyFrames[1].modelInfo.torso.rotation.y = 90.0f;
	caminarAnimationKeyFrames[1].stepsToFrame = caminarDelay1;
	
	caminarAnimationKeyFrames[2].modelInfo.torso.position.x = 60.0;
	caminarAnimationKeyFrames[2].modelInfo.torso.position.z = -60.0;
	caminarAnimationKeyFrames[2].modelInfo.torso.rotation.y = 0.0f;
	caminarAnimationKeyFrames[2].stepsToFrame = caminarDelay2;

	caminarAnimationKeyFrames[3].modelInfo.torso.position.x = 60.0;
	caminarAnimationKeyFrames[3].modelInfo.torso.position.z = 170.0;
	caminarAnimationKeyFrames[3].modelInfo.torso.rotation.y = 0.0f;
	caminarAnimationKeyFrames[3].stepsToFrame = caminarDelay1;

	caminarAnimationKeyFrames[4].modelInfo.torso.position.x = 60.0;
	caminarAnimationKeyFrames[4].modelInfo.torso.position.z = 170.0;
	caminarAnimationKeyFrames[4].modelInfo.torso.rotation.y = -90.0f;
	caminarAnimationKeyFrames[4].stepsToFrame = caminarDelay2;

	caminarAnimationKeyFrames[5].modelInfo.torso.position.x = 8.0;
	caminarAnimationKeyFrames[5].modelInfo.torso.position.z = 170;
	caminarAnimationKeyFrames[5].modelInfo.torso.rotation.y = -90.0f;
	caminarAnimationKeyFrames[5].stepsToFrame = caminarDelay1;

	caminarAnimationKeyFrames[6].modelInfo.torso.position.x = 8.0;
	caminarAnimationKeyFrames[6].modelInfo.torso.position.z = 170.0;
	caminarAnimationKeyFrames[6].modelInfo.torso.rotation.y = 90.0f;
	caminarAnimationKeyFrames[6].stepsToFrame = caminarDelay1;

	caminarAnimationKeyFrames[7].modelInfo.torso.position.x = -8.0;
	caminarAnimationKeyFrames[7].modelInfo.torso.position.z = 170.0;
	caminarAnimationKeyFrames[7].modelInfo.torso.position.y = -10.0;
	caminarAnimationKeyFrames[7].modelInfo.rightLeg.rotation.x = -60.0;
	caminarAnimationKeyFrames[7].modelInfo.subRightLeg.rotation.x = 60.0;
	caminarAnimationKeyFrames[7].modelInfo.leftLeg.rotation.x = -60.0;
	caminarAnimationKeyFrames[7].modelInfo.subLeftLeg.rotation.x = 60.0;
	caminarAnimationKeyFrames[7].modelInfo.rightLeg.rotation.x = -60.0;
	caminarAnimationKeyFrames[7].modelInfo.leftArm.rotation.x = -30.0;
	caminarAnimationKeyFrames[7].modelInfo.rightArm.rotation.x = 30.0;
	caminarAnimationKeyFrames[7].modelInfo.torso.rotation.y = 90.0f;
	caminarAnimationKeyFrames[7].stepsToFrame = caminarDelay2;

	caminarAnimationKeyFrames[8].modelInfo.torso.position.x = -8.0;
	caminarAnimationKeyFrames[8].modelInfo.torso.position.z = 170.0;
	caminarAnimationKeyFrames[8].modelInfo.torso.position.y = -10.0;
	caminarAnimationKeyFrames[8].modelInfo.rightLeg.rotation.x = -60.0;
	caminarAnimationKeyFrames[8].modelInfo.subRightLeg.rotation.x = 60.0;
	caminarAnimationKeyFrames[8].modelInfo.leftLeg.rotation.x = -60.0;
	caminarAnimationKeyFrames[8].modelInfo.subLeftLeg.rotation.x = 60.0;
	caminarAnimationKeyFrames[8].modelInfo.rightLeg.rotation.x = -60.0;
	caminarAnimationKeyFrames[8].modelInfo.leftArm.rotation.z = -30.0;
	caminarAnimationKeyFrames[8].modelInfo.rightArm.rotation.z = 30.0;
	caminarAnimationKeyFrames[8].modelInfo.torso.rotation.y = 180.0f;
	caminarAnimationKeyFrames[8].stepsToFrame = caminarDelay2;
	
}


void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;

	KeyFrame[FrameIndex].rotDog = rotDog;


	FrameIndex++;
}

void resetElements(void)
{
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;

	rotDog = KeyFrame[0].rotDog;

}
float interpolar(float a, float b, float t) {
	return (b - a) * t;
}

void interpolation()
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;

	

}

float interpolation2(float value, float next, int steps)
{

	
	return  (next - (value)) / steps;



}




// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	startUpFrames();
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Animacion maquina de estados", nullptr, nullptr);

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
	
	
	//models

	Model ProfesorTorso((char*)"Models/Profesor/ProfesorTorso.obj");
	Model ProfesorHead((char*)"Models/Profesor/ProfesorHead.obj");
	Model ProfesorRightArm((char*)"Models/Profesor/ProfesorRightArm.obj");
	Model ProfesorSubRightArm((char*)"Models/Profesor/ProfesorSubRightArm.obj");
	Model ProfesorLeftArm((char*)"Models/Profesor/ProfesorLeftArm.obj");
	Model ProfesorSubLeftArm((char*)"Models/Profesor/ProfesorSubLeftArm.obj");
	Model ProfesorRightLeg((char*)"Models/Profesor/ProfesorRightLeg.obj");
	Model ProfesorSubRightLeg((char*)"Models/Profesor/ProfesorSubRightLeg.obj");
	Model ProfesorLeftLeg((char*)"Models/Profesor/ProfesorLeftLeg.obj");
	Model ProfesorSubLeftLeg((char*)"Models/Profesor/ProfesorSubLeftLeg.obj");


	Model Piso((char*)"Models/escenario/piso.obj");
	Model Sillas((char*)"Models/test_chairs/test_chairs.obj");
	


	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dogPosX = 0;
		KeyFrame[i].dogPosY = 0;
		KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;
	}


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO,EBO;
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
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		glm::mat4 generalTransform = glm::mat4(1.0f);
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp1 = glm::mat4(1.0f);
		
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.6f,0.6f,0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.3f, 0.3f, 0.3f);


		// Point light 1
	    glm::vec3 lightColor;
		lightColor.x= abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= sin(glfwGetTime() *Light1.z);

		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));
		

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

	
		generalTransform = glm::scale(generalTransform, glm::vec3(0.01f, 0.01f, 0.01f));
		//Carga de modelo 
		
        view = camera.GetViewMatrix();	
		model = generalTransform;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);
		
		model = generalTransform;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sillas.Draw(lightingShader);

		model = generalTransform;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);


		//Profesor Torso




		model = generalTransform;
		model = glm::translate(model, profesor.torso.position);
		modelTemp = model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		modelTemp = model = glm::rotate(model, glm::radians(profesor.torso.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = model = glm::rotate(model, glm::radians(profesor.torso.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model = glm::rotate(model, glm::radians(profesor.torso.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ProfesorTorso.Draw(lightingShader);




		//Profesor Head



		model = generalTransform;
		model = glm::translate(modelTemp, glm::vec3(0.0f, 148.54f, 0.0f) + profesor.head.position);
		model = glm::rotate(model, glm::radians(profesor.head.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(profesor.head.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(profesor.head.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ProfesorHead.Draw(lightingShader);

		//Profesor Right Arm

		model = generalTransform;
		model = glm::translate(modelTemp, glm::vec3(-20.02f, 140.164f, 0.0f) + profesor.rightArm.position);
		modelTemp1 = model = glm::rotate(model, glm::radians(profesor.rightArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp1 = model = glm::rotate(model, glm::radians(profesor.rightArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp1 = model = glm::rotate(model, glm::radians(profesor.rightArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ProfesorRightArm.Draw(lightingShader);

		//Profesor Sub Right Arm

		model = generalTransform;
		model = glm::translate(modelTemp1, glm::vec3(-15.137, -16.795f, -0.456f) + profesor.subRightArm.position);
		model = glm::rotate(model, glm::radians(profesor.subRightArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(profesor.subRightArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(profesor.subRightArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ProfesorSubRightArm.Draw(lightingShader);




		////Profesor Left Arm
		model = generalTransform;
		model = glm::translate(modelTemp, glm::vec3(19.794f, 140.059f, -0.159f) + profesor.leftArm.position);
		modelTemp1 = model = glm::rotate(model, glm::radians(profesor.leftArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp1 = model = glm::rotate(model, glm::radians(profesor.leftArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp1 = model = glm::rotate(model, glm::radians(profesor.leftArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ProfesorLeftArm.Draw(lightingShader);

		//Profesor Sub Left Arm

		model = generalTransform;
		model = glm::translate(modelTemp1, glm::vec3(15.123f, -16.78f, -0.781f) + profesor.subLeftArm.position);
		model = glm::rotate(model, glm::radians(profesor.subLeftArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(profesor.subLeftArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(profesor.subLeftArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ProfesorSubLeftArm.Draw(lightingShader);



		//Profesor Right Leg
		model = generalTransform;
		model = glm::translate(modelTemp, glm::vec3(0.0f, 87.83f, -9.225f) + profesor.rightLeg.position);
		modelTemp1 = model = glm::rotate(model, glm::radians(profesor.rightLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp1 = model = glm::rotate(model, glm::radians(profesor.rightLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp1 = model = glm::rotate(model, glm::radians(profesor.rightLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ProfesorRightLeg.Draw(lightingShader);

		//Profesor Sub Right Leg

		model = generalTransform;
		model = glm::translate(modelTemp1, glm::vec3(-10.713f, -34.908f, 14.468f) + profesor.subRightLeg.position);
		model = glm::rotate(model, glm::radians(profesor.subRightLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(profesor.subRightLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(profesor.subRightLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ProfesorSubRightLeg.Draw(lightingShader);


		//Profesor Left Leg
		model = generalTransform;
		model = glm::translate(modelTemp, glm::vec3(9.817f, 87.906f, -9.521f) + profesor.leftLeg.position);
		modelTemp1 = model = glm::rotate(model, glm::radians(profesor.leftLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp1 = model = glm::rotate(model, glm::radians(profesor.leftLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp1 = model = glm::rotate(model, glm::radians(profesor.leftLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ProfesorLeftLeg.Draw(lightingShader);

		//Profesor Sub Left Leg

		model = generalTransform;
		model = glm::translate(modelTemp1, glm::vec3(0.272f, -36.701f, 17.087f) + profesor.subLeftLeg.position);
		model = glm::rotate(model, glm::radians(profesor.subLeftLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(profesor.subLeftLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(profesor.subLeftLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ProfesorSubLeftLeg.Draw(lightingShader);



		//model = generalTransform;
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		//model = glm::rotate(model, glm::radians(caminarTime), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	 //   Ball.Draw(lightingShader); 
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glBindVertexArray(0);
	

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = generalTransform;
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		
		model = generalTransform;
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
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
	//Dog Controls
	
	if (keys[GLFW_KEY_2])
	{
		
		profesor.torso.rotation.y += 1.0f;
		rotDog += 1.0f;

	}

	if (keys[GLFW_KEY_3])
	{
		
		profesor.torso.rotation.y -= 1.0f;
		rotDog -= 1.0f;

	}
			
	if (keys[GLFW_KEY_H])
	{
		profesor.torso.position.z += 0.01;
		dogPosX += 0.01;
	}

	if (keys[GLFW_KEY_Y])
	{
		profesor.torso.position.z -= 0.01;
		dogPosZ -= 0.01;
	}

	if (keys[GLFW_KEY_G])
	{
		profesor.torso.position.x -= 0.01;
		dogPosX -= 0.01;
	}

	if (keys[GLFW_KEY_J])
	{
		profesor.torso.position.x += 0.01;
		dogPosX += 0.01;
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

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}
	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{

	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}

	}



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
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
			
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}

	if (keys[GLFW_KEY_N])
	{
		playAnimationIndex = 0;

	}
	
	
}
void Animation() {


	switch (playAnimationIndex)
	{
	case 0:
		if (caminarAnimationKeyIndex == -1) {
			HumanoidModelInfo firstFrame = caminarAnimationKeyFrames[0].modelInfo;

			profesor.torso.position.x = firstFrame.torso.position.x;
			profesor.torso.position.z = firstFrame.torso.position.z;
			profesor.torso.position.y = firstFrame.torso.position.y;
			profesor.torso.rotation.y = firstFrame.torso.rotation.y;
			playCaminar = true;
			caminarAnimationKeyIndex = 0;
		}
		else if (caminarAnimationKeyIndex < caminarAnimationMaxKeyFrame) {
			if (caminarAnimationMaxStep == -1) {
				
				caminarAnimationCurrentStep = 0;

				if (caminarAnimationKeyIndex == 5) {

					playCaminar = false;
					caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.rightArm.rotation.x = profesor.rightArm.rotation.x;
					caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.leftArm.rotation.x = profesor.leftArm.rotation.x;
					caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.subRightArm.rotation.x = profesor.subRightArm.rotation.x;
					caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.subLeftArm.rotation.x = profesor.subLeftArm.rotation.x;
					caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.rightLeg.rotation.x = profesor.rightLeg.rotation.x;
					caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.leftLeg.rotation.x = profesor.leftLeg.rotation.x;
					caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.subRightLeg.rotation.x = profesor.subRightLeg.rotation.x;
					caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.subLeftLeg.rotation.x = profesor.subLeftLeg.rotation.x;
					caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.leftArm.rotation.z = profesor.leftArm.rotation.z;
					caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.rightArm.rotation.z = profesor.rightArm.rotation.z;
				}


				caminarAnimationMaxStep = caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].stepsToFrame;
				caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.position.x = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.torso.position.x, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.torso.position.x,caminarAnimationMaxStep);
				caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.position.y = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.torso.position.y, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.torso.position.y, caminarAnimationMaxStep);
				caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.position.z = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.torso.position.z, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.torso.position.z, caminarAnimationMaxStep);
				caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.rotation.y = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.torso.rotation.y, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.torso.rotation.y, caminarAnimationMaxStep);

				caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.rightArm.rotation.x = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.rightArm.rotation.x, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.rightArm.rotation.x, caminarAnimationMaxStep);
				caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.leftArm.rotation.x = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.leftArm.rotation.x, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.leftArm.rotation.x, caminarAnimationMaxStep);
				caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.subRightArm.rotation.x = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.subRightArm.rotation.x, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.subRightArm.rotation.x, caminarAnimationMaxStep);
				caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.subLeftArm.rotation.x = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.subLeftArm.rotation.x, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.subLeftArm.rotation.x, caminarAnimationMaxStep);
				caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.rightLeg.rotation.x = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.rightLeg.rotation.x, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.rightLeg.rotation.x, caminarAnimationMaxStep);
				caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.leftLeg.rotation.x = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.leftLeg.rotation.x, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.leftLeg.rotation.x, caminarAnimationMaxStep);
				caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.subRightLeg.rotation.x = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.subRightLeg.rotation.x, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.subRightLeg.rotation.x, caminarAnimationMaxStep);
				caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.subLeftLeg.rotation.x = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.subLeftLeg.rotation.x, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.subLeftLeg.rotation.x, caminarAnimationMaxStep);
				caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.leftArm.rotation.z = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.leftArm.rotation.z, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.leftArm.rotation.z, caminarAnimationMaxStep);
				caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.rightArm.rotation.z = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.rightArm.rotation.z, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.rightArm.rotation.z, caminarAnimationMaxStep);

			}
			else {
				if (caminarAnimationCurrentStep < caminarAnimationMaxStep) {
					profesor.torso.position.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.position.x;
					profesor.torso.position.y += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.position.y;
					profesor.torso.position.z += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.position.z;
					profesor.torso.rotation.y += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.rotation.y;

					
						profesor.rightArm.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.rightArm.rotation.x;
						profesor.leftArm.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.leftArm.rotation.x;
						profesor.subRightArm.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.subRightArm.rotation.x;
						profesor.subLeftArm.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.subLeftArm.rotation.x;
						profesor.rightLeg.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.rightLeg.rotation.x;
						profesor.leftLeg.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.leftLeg.rotation.x;

						profesor.subRightLeg.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.subRightLeg.rotation.x;
						profesor.subLeftLeg.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.subLeftLeg.rotation.x;

						profesor.leftArm.rotation.z += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.leftArm.rotation.z;
						profesor.rightArm.rotation.z += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.rightArm.rotation.z;
				



					caminarAnimationCurrentStep++;
				}
				else {
					if (caminarAnimationKeyIndex == 5) {
						
					
						
					}

					caminarAnimationMaxStep = -1;
					caminarAnimationCurrentStep = -1;
					caminarAnimationKeyIndex++;

					
				}
			}
			
		}
		else {
			caminarAnimationKeyIndex = -1;
			playCaminar = false;
			playAnimationIndex = -1;
		}
		break;

		 


	default:
		break;
	}

	
		

	
	if (playCaminar)
	{
		caminarTime += 100.0 * deltaTime;
		profesor.rightArm.rotation.x = 15.0f + sin(glm::radians(caminarTime)) * 30;
		profesor.leftArm.rotation.x = 15.0f - sin(glm::radians(caminarTime)) * 30;
		profesor.subRightArm.rotation.x = -15.0f + sin(glm::radians(caminarTime)) * 15;
		profesor.subLeftArm.rotation.x = -15.0f - sin(glm::radians(caminarTime)) * 15;
		profesor.rightLeg.rotation.x = -15.0f + sin(glm::radians(caminarTime)) * 15;
		profesor.leftLeg.rotation.x = -15.0f - sin(glm::radians(caminarTime)) * 15;

		profesor.subRightLeg.rotation.x = 30.0f + sin(glm::radians(caminarTime) - 0.5) * 30;
		profesor.subLeftLeg.rotation.x = 30.0f - sin(glm::radians(caminarTime) - 0.5) * 30;

		profesor.leftArm.rotation.z = -30.0f;
		profesor.rightArm.rotation.z = 30.0f;
	}
	else
	{
		// Si la animación está desactivada, restablecer las variables
		caminarTime = 0.0f;
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