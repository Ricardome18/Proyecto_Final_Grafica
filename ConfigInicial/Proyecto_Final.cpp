/*
PROYECTO FINAL  COMPUTACIÓN GRÁFICA
LABORATORIO SALA D
EQUIPO 5
AXEL
RICARDO
JEREMIAS

*/


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
#include "Texture.h"//Skybox



//Structs modelo humanide


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
struct HumanoidAnimation {
	int maxKeyFrame = 0;
	int currentKeyFrame = -1;
	int step = -1;
	int maxStep = -1;
	HumanoidFrame keyFrames[10];

};





// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
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
// para el laboratorio nuevo
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

// Posiciones para las luces de tipo puntual para el laboratorio viejo
glm::vec3 pointLightPositions2[] = {
	glm::vec3(-33.633f, 32.758f, -51.231f),			//C1-F1
	glm::vec3(-33.633f, 32.758f, -21.477f),			//C1-F2
	glm::vec3(-33.633f, 32.758f, 6.14f),			//C1-F3
	glm::vec3(-33.633f, 32.758f, 33.598f),			//C1-F4
	glm::vec3(-33.633f, 32.758f, 58.76f),			//C1-F5

	glm::vec3(-0.843f, 32.758f, -51.231f),			//C2-F1
	glm::vec3(-0.843f, 32.758f, -21.477f),			//C2-F2
	glm::vec3(-0.843f, 32.758f, 6.14F),				//C2-F3
	glm::vec3(-0.843f, 32.758f, 33.598f),			//C2-F4
	glm::vec3(-0.843f, 32.758f, 58.76f),			//C2-F5

	glm::vec3(33.138f, 32.758f, -51.231f),			//C3-F1
	glm::vec3(33.138f, 32.758f, -21.477f),			//C3-F2
	glm::vec3(33.138f, 32.758f, 6.14F),				//C3-F3
	glm::vec3(33.138f, 32.758f, 33.598f),			//C3-F4
	//glm::vec3(33.138f, 32.758f, 33.598f),			//C3-F5
};

glm::vec3 spotLightDir(0.0f, -0.1f, 0.0f); // Dirección inicial
float spotPitch = 13.0f; // ángulo inicial en grados para la luz tipo spotlight del proyector

//Variable global para intercambio entre escenarios
bool mostrarEscenarioNuevo = false;
//Controla si las lucesnestan presentes durante la transicion del escenario

bool mostrarLuces = true;


HumanoidModelInfo personas[3];

HumanoidAnimation animations[100];

//Transicion escenario

float rotarYEscenario = 0.0f;
float rotarYLabViejo = 0.0f;
float tornadoScale = 0.0f;
float tornadoMove = 0.0f;
float labViejoMove = 0.0f;
float labNuevoMove = 0.0f;
float labNuevoBounce = 0.0f;
int transicionLab = -1;


int playAnimationIndex = -1;

int FrameIndex = 0;	//introducir datos
bool playCaminar = false;
bool play = false;
int playIndex = 0;

//Animacion 0 

float caminarTime = 0.0f;

const int caminarAnimationMaxKeyFrame = 8;
HumanoidFrame caminarAnimationKeyFrames[caminarAnimationMaxKeyFrame];
int caminarAnimationKeyIndex = -1;
int caminarAnimationMaxStep = -1;
int caminarAnimationCurrentStep = -1;


glm::vec3 Light1 = glm::vec3(0);//Luces

//Variables para Animacion
float rotBall = 1.0472;
bool AnimBall = false;
float salto = 0.0f;
float rot = 0.0f;
bool renderAlumno1 = false;
float animationSpeed = 0.06f;  // Controla la velocidad global de la animación. Un valor más bajo significa más lento.


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame


void startUpFrames() {

	personas[1].torso.rotation.y = 90.0f;
	personas[1].torso.position.z = -55.412f;
	personas[1].torso.position.y = 0.5f;
	personas[1].rightArm.rotation.x = -75.0f;
	personas[1].leftArm.rotation.x = 75.0f;

	personas[2].torso.position = glm::vec3(0.0f, -4.028f, 0.059f);
	personas[2].torso.rotation.y = -90.0f;
	personas[2].leftArm.rotation.x = 30.0f;
	personas[2].rightArm.rotation.x = -30.0f;
	personas[2].rightLeg.rotation.z = -80.0f;
	personas[2].leftLeg.rotation.z = -80.0f;
	personas[2].subRightLeg.rotation.z = 30.0f;
	personas[2].subLeftLeg.rotation.z = 30.0f;

	//animation 0

	if (true) {
		int caminarDelay1 = 3000;
		int caminarDelay2 = 1000;
		float doorZCoordinate = -54.0;
		float chairXCoordinate = -18.0;
		float chairZCoordinate = -23.0;

		caminarAnimationKeyFrames[0].modelInfo.torso.position.x = -100.0;
		caminarAnimationKeyFrames[0].modelInfo.torso.position.z = doorZCoordinate;
		caminarAnimationKeyFrames[0].modelInfo.torso.rotation.y = 90.0f;

		caminarAnimationKeyFrames[1].modelInfo.torso.position.x = 0.0;
		caminarAnimationKeyFrames[1].modelInfo.torso.position.z = doorZCoordinate;
		caminarAnimationKeyFrames[1].modelInfo.torso.rotation.y = 90.0f;
		caminarAnimationKeyFrames[1].stepsToFrame = caminarDelay1;

		caminarAnimationKeyFrames[2].modelInfo.torso.position.x = 0.0;
		caminarAnimationKeyFrames[2].modelInfo.torso.position.z = doorZCoordinate;
		caminarAnimationKeyFrames[2].modelInfo.torso.rotation.y = 0.0f;
		caminarAnimationKeyFrames[2].stepsToFrame = caminarDelay2 / 3.0;

		caminarAnimationKeyFrames[3].modelInfo.torso.position.x = 0.0;
		caminarAnimationKeyFrames[3].modelInfo.torso.position.z = chairZCoordinate;
		caminarAnimationKeyFrames[3].modelInfo.torso.rotation.y = 0.0f;
		caminarAnimationKeyFrames[3].stepsToFrame = 1530;

		caminarAnimationKeyFrames[4].modelInfo.torso.position.x = 0.0;
		caminarAnimationKeyFrames[4].modelInfo.torso.position.z = chairZCoordinate;
		caminarAnimationKeyFrames[4].modelInfo.torso.rotation.y = 90.0f;
		caminarAnimationKeyFrames[4].stepsToFrame = caminarDelay2 / 5.0;

		caminarAnimationKeyFrames[5].modelInfo.torso.position.x = -10.0;
		caminarAnimationKeyFrames[5].modelInfo.torso.position.z = chairZCoordinate;
		caminarAnimationKeyFrames[5].modelInfo.torso.rotation.y = 90.0f;
		caminarAnimationKeyFrames[5].stepsToFrame = caminarDelay1 / 5.0;

		caminarAnimationKeyFrames[6].modelInfo.torso.position.x = chairXCoordinate;
		caminarAnimationKeyFrames[6].modelInfo.torso.position.z = chairZCoordinate;
		caminarAnimationKeyFrames[6].modelInfo.torso.position.y = -5.0;
		caminarAnimationKeyFrames[6].modelInfo.rightLeg.rotation.x = -60.0;
		caminarAnimationKeyFrames[6].modelInfo.subRightLeg.rotation.x = 30.0;
		caminarAnimationKeyFrames[6].modelInfo.leftLeg.rotation.x = -60.0;
		caminarAnimationKeyFrames[6].modelInfo.subLeftLeg.rotation.x = 30.0;
		caminarAnimationKeyFrames[6].modelInfo.rightLeg.rotation.x = -60.0;
		caminarAnimationKeyFrames[6].modelInfo.leftArm.rotation.x = -30.0;
		caminarAnimationKeyFrames[6].modelInfo.rightArm.rotation.x = 30.0;
		caminarAnimationKeyFrames[6].modelInfo.torso.rotation.y = 90.0f;
		caminarAnimationKeyFrames[6].stepsToFrame = caminarDelay2;

		caminarAnimationKeyFrames[7].modelInfo.torso.position.x = chairXCoordinate;
		caminarAnimationKeyFrames[7].modelInfo.torso.position.z = chairZCoordinate;
		caminarAnimationKeyFrames[7].modelInfo.torso.position.y = -5.0;
		caminarAnimationKeyFrames[7].modelInfo.rightLeg.rotation.x = -60.0;
		caminarAnimationKeyFrames[7].modelInfo.subRightLeg.rotation.x = 30.0;
		caminarAnimationKeyFrames[7].modelInfo.leftLeg.rotation.x = -60.0;
		caminarAnimationKeyFrames[7].modelInfo.subLeftLeg.rotation.x = 30.0;
		caminarAnimationKeyFrames[7].modelInfo.rightLeg.rotation.x = -60.0;
		caminarAnimationKeyFrames[7].modelInfo.leftArm.rotation.z = -30.0;
		caminarAnimationKeyFrames[7].modelInfo.rightArm.rotation.z = 30.0;
		caminarAnimationKeyFrames[7].modelInfo.torso.rotation.y = 180.0f;
		caminarAnimationKeyFrames[7].stepsToFrame = caminarDelay2;
	}
	//animation 1
	if (true) {

		animations[1].maxKeyFrame = 10;

		animations[1].keyFrames[0].modelInfo.torso.position = glm::vec3(0.0f, -4.028f, 0.059f);
		animations[1].keyFrames[0].modelInfo.torso.rotation.y = -90.0f;
		animations[1].keyFrames[0].modelInfo.leftArm.rotation.x = 30.0f;
		animations[1].keyFrames[0].modelInfo.rightArm.rotation.x = -30.0f;
		animations[1].keyFrames[0].modelInfo.rightLeg.rotation.z = -80.0f;
		animations[1].keyFrames[0].modelInfo.leftLeg.rotation.z = -80.0f;
		animations[1].keyFrames[0].modelInfo.subRightLeg.rotation.z = 30.0f;
		animations[1].keyFrames[0].modelInfo.subLeftLeg.rotation.z = 30.0f;


		//Toca pc con mano izquierda
		animations[1].keyFrames[1].modelInfo.torso.position = glm::vec3(0.0f, -4.028f, 0.059f);
		animations[1].keyFrames[1].modelInfo.torso.rotation.y = -90.0f;
		animations[1].keyFrames[1].modelInfo.torso.rotation.z = 20.0f;
		animations[1].keyFrames[1].modelInfo.leftArm.rotation.x = 0.0f;
		animations[1].keyFrames[1].modelInfo.leftArm.rotation.y = -34.0f;
		animations[1].keyFrames[1].modelInfo.leftArm.rotation.z = -100.0f;
		animations[1].keyFrames[1].modelInfo.rightArm.rotation.x = -30.0f;
		animations[1].keyFrames[1].modelInfo.rightLeg.rotation.z = -80.0f;
		animations[1].keyFrames[1].modelInfo.leftLeg.rotation.z = -80.0f;
		animations[1].keyFrames[1].modelInfo.subRightLeg.rotation.z = 30.0f;
		animations[1].keyFrames[1].modelInfo.subLeftLeg.rotation.z = 30.0f;
		animations[1].keyFrames[1].stepsToFrame = 250;

		animations[1].keyFrames[2].modelInfo.torso.position = glm::vec3(0.0f, -4.028f, 0.059f);
		animations[1].keyFrames[2].modelInfo.torso.rotation.y = -90.0f;
		animations[1].keyFrames[2].modelInfo.torso.rotation.z = 20.0f;
		animations[1].keyFrames[2].modelInfo.leftArm.rotation.x = 0.0f;
		animations[1].keyFrames[2].modelInfo.leftArm.rotation.y = -34.0f;
		animations[1].keyFrames[2].modelInfo.leftArm.rotation.z = -100.0f;
		animations[1].keyFrames[2].modelInfo.rightArm.rotation.x = -30.0f;
		animations[1].keyFrames[2].modelInfo.rightLeg.rotation.z = -80.0f;
		animations[1].keyFrames[2].modelInfo.leftLeg.rotation.z = -80.0f;
		animations[1].keyFrames[2].modelInfo.subRightLeg.rotation.z = 30.0f;
		animations[1].keyFrames[2].modelInfo.subLeftLeg.rotation.z = 30.0f;
		animations[1].keyFrames[2].stepsToFrame = 250;

		//Toca mouse con mano derecha
		animations[1].keyFrames[3].modelInfo.torso.position = glm::vec3(0.0f, -4.028f, 0.059f);
		animations[1].keyFrames[3].modelInfo.torso.rotation.y = -90.0f;
		animations[1].keyFrames[3].modelInfo.torso.rotation.z = 15.0f;
		animations[1].keyFrames[3].modelInfo.leftArm.rotation.x = 30.0f;
		animations[1].keyFrames[3].modelInfo.rightArm.rotation.y = -20.0f;
		animations[1].keyFrames[3].modelInfo.rightArm.rotation.z = -30.0f;
		animations[1].keyFrames[3].modelInfo.subRightArm.rotation.y = 60.0f;
		animations[1].keyFrames[3].modelInfo.subRightArm.position.y = 0.05f;
		animations[1].keyFrames[3].modelInfo.rightLeg.rotation.z = -80.0f;
		animations[1].keyFrames[3].modelInfo.leftLeg.rotation.z = -80.0f;
		animations[1].keyFrames[3].modelInfo.subRightLeg.rotation.z = 30.0f;
		animations[1].keyFrames[3].modelInfo.subLeftLeg.rotation.z = 30.0f;
		animations[1].keyFrames[3].stepsToFrame = 250;

		animations[1].keyFrames[4].modelInfo.torso.position = glm::vec3(0.0f, -4.028f, -0.059f);
		animations[1].keyFrames[4].modelInfo.torso.rotation.y = -90.0f;
		animations[1].keyFrames[4].modelInfo.torso.rotation.z = 15.0f;
		animations[1].keyFrames[4].modelInfo.leftArm.rotation.x = 30.0f;
		animations[1].keyFrames[4].modelInfo.rightArm.rotation.y = -20.0f;
		animations[1].keyFrames[4].modelInfo.rightArm.rotation.z = -30.0f;
		animations[1].keyFrames[4].modelInfo.subRightArm.rotation.y = 60.0f;
		animations[1].keyFrames[4].modelInfo.subRightArm.position.y = 0.05f;
		animations[1].keyFrames[4].modelInfo.rightLeg.rotation.z = -80.0f;
		animations[1].keyFrames[4].modelInfo.leftLeg.rotation.z = -80.0f;
		animations[1].keyFrames[4].modelInfo.subRightLeg.rotation.z = 30.0f;
		animations[1].keyFrames[4].modelInfo.subLeftLeg.rotation.z = 30.0f;
		animations[1].keyFrames[4].stepsToFrame = 250;

		//Manos al teclado

		animations[1].keyFrames[5].modelInfo.torso.position = glm::vec3(0.0f, -4.028f, -3.496f);
		animations[1].keyFrames[5].modelInfo.torso.rotation.y = -90.0f;
		animations[1].keyFrames[5].modelInfo.leftArm.rotation.y = -20.0f;
		animations[1].keyFrames[5].modelInfo.leftArm.rotation.z = -20.0f;
		animations[1].keyFrames[5].modelInfo.subLeftArm.rotation.y = -60.0f;
		animations[1].keyFrames[5].modelInfo.subLeftArm.position.y = 0.05f;
		animations[1].keyFrames[5].modelInfo.rightArm.rotation.y = 20.0f;
		animations[1].keyFrames[5].modelInfo.rightArm.rotation.z = -20.0f;
		animations[1].keyFrames[5].modelInfo.subRightArm.rotation.y = 60.0f;
		animations[1].keyFrames[5].modelInfo.subRightArm.position.y = 0.05f;
		animations[1].keyFrames[5].modelInfo.rightLeg.rotation.z = -80.0f;
		animations[1].keyFrames[5].modelInfo.leftLeg.rotation.z = -80.0f;
		animations[1].keyFrames[5].modelInfo.subRightLeg.rotation.z = 30.0f;
		animations[1].keyFrames[5].modelInfo.subLeftLeg.rotation.z = 30.0f;
		animations[1].keyFrames[5].stepsToFrame = 250;

		//escribir

		animations[1].keyFrames[6].modelInfo.torso.position = glm::vec3(0.0f, -4.028f, -3.496f);
		animations[1].keyFrames[6].modelInfo.torso.rotation.y = -90.0f;
		animations[1].keyFrames[6].modelInfo.leftArm.rotation.y = -20.0f;
		animations[1].keyFrames[6].modelInfo.leftArm.rotation.z = -30.0f;
		animations[1].keyFrames[6].modelInfo.subLeftArm.rotation.y = -60.0f;
		animations[1].keyFrames[6].modelInfo.subLeftArm.position.y = 0.05f;
		animations[1].keyFrames[6].modelInfo.rightArm.rotation.y = 20.0f;
		animations[1].keyFrames[6].modelInfo.rightArm.rotation.z = -10.0f;
		animations[1].keyFrames[6].modelInfo.subRightArm.rotation.y = 60.0f;
		animations[1].keyFrames[6].modelInfo.subRightArm.position.y = 0.05f;
		animations[1].keyFrames[6].modelInfo.rightLeg.rotation.z = -80.0f;
		animations[1].keyFrames[6].modelInfo.leftLeg.rotation.z = -80.0f;
		animations[1].keyFrames[6].modelInfo.subRightLeg.rotation.z = 30.0f;
		animations[1].keyFrames[6].modelInfo.subLeftLeg.rotation.z = 30.0f;
		animations[1].keyFrames[6].stepsToFrame = 200;


		animations[1].keyFrames[7].modelInfo.torso.position = glm::vec3(0.0f, -4.028f, -3.496f);
		animations[1].keyFrames[7].modelInfo.torso.rotation.y = -90.0f;
		animations[1].keyFrames[7].modelInfo.leftArm.rotation.y = -20.0f;
		animations[1].keyFrames[7].modelInfo.leftArm.rotation.z = -10.0f;
		animations[1].keyFrames[7].modelInfo.subLeftArm.rotation.y = -60.0f;
		animations[1].keyFrames[7].modelInfo.subLeftArm.position.y = 0.05f;
		animations[1].keyFrames[7].modelInfo.rightArm.rotation.y = 20.0f;
		animations[1].keyFrames[7].modelInfo.rightArm.rotation.z = -30.0f;
		animations[1].keyFrames[7].modelInfo.subRightArm.rotation.y = 60.0f;
		animations[1].keyFrames[7].modelInfo.subRightArm.position.y = 0.05f;
		animations[1].keyFrames[7].modelInfo.rightLeg.rotation.z = -80.0f;
		animations[1].keyFrames[7].modelInfo.leftLeg.rotation.z = -80.0f;
		animations[1].keyFrames[7].modelInfo.subRightLeg.rotation.z = 30.0f;
		animations[1].keyFrames[7].modelInfo.subLeftLeg.rotation.z = 30.0f;
		animations[1].keyFrames[7].stepsToFrame = 200;


		animations[1].keyFrames[8].modelInfo.torso.position = glm::vec3(0.0f, -4.028f, -3.496f);
		animations[1].keyFrames[8].modelInfo.torso.rotation.y = -90.0f;
		animations[1].keyFrames[8].modelInfo.leftArm.rotation.y = -20.0f;
		animations[1].keyFrames[8].modelInfo.leftArm.rotation.z = -30.0f;
		animations[1].keyFrames[8].modelInfo.subLeftArm.rotation.y = -60.0f;
		animations[1].keyFrames[8].modelInfo.subLeftArm.position.y = 0.05f;
		animations[1].keyFrames[8].modelInfo.rightArm.rotation.y = 20.0f;
		animations[1].keyFrames[8].modelInfo.rightArm.rotation.z = -10.0f;
		animations[1].keyFrames[8].modelInfo.subRightArm.rotation.y = 60.0f;
		animations[1].keyFrames[8].modelInfo.subRightArm.position.y = 0.05f;
		animations[1].keyFrames[8].modelInfo.rightLeg.rotation.z = -80.0f;
		animations[1].keyFrames[8].modelInfo.leftLeg.rotation.z = -80.0f;
		animations[1].keyFrames[8].modelInfo.subRightLeg.rotation.z = 30.0f;
		animations[1].keyFrames[8].modelInfo.subLeftLeg.rotation.z = 30.0f;
		animations[1].keyFrames[8].stepsToFrame = 200;


		animations[1].keyFrames[9].modelInfo.torso.position = glm::vec3(0.0f, -4.028f, 0.059f);
		animations[1].keyFrames[9].modelInfo.torso.rotation.y = -90.0f;
		animations[1].keyFrames[9].modelInfo.leftArm.rotation.x = 30.0f;
		animations[1].keyFrames[9].modelInfo.rightArm.rotation.x = -30.0f;
		animations[1].keyFrames[9].modelInfo.rightLeg.rotation.z = -80.0f;
		animations[1].keyFrames[9].modelInfo.leftLeg.rotation.z = -80.0f;
		animations[1].keyFrames[9].modelInfo.subRightLeg.rotation.z = 30.0f;
		animations[1].keyFrames[9].modelInfo.subLeftLeg.rotation.z = 30.0f;
		animations[1].keyFrames[9].stepsToFrame = 200;

		//regreso a posicion inicial



	}
	//animation 2
	if (true)
	{
		animations[2].maxKeyFrame = 8;

		animations[2].keyFrames[0].modelInfo.torso.rotation.y = 90.0f;
		animations[2].keyFrames[0].modelInfo.torso.position.z = -55.412f;
		animations[2].keyFrames[0].modelInfo.torso.position.y = 0.5;
		animations[2].keyFrames[0].modelInfo.rightArm.rotation.x = -75.0f;
		animations[2].keyFrames[0].modelInfo.leftArm.rotation.x = 75.0f;

		animations[2].keyFrames[1].modelInfo.torso.rotation.y = 90.0f;
		animations[2].keyFrames[1].modelInfo.torso.position.z = -55.412f;
		animations[2].keyFrames[1].modelInfo.torso.position.y = 0.5;
		animations[2].keyFrames[1].modelInfo.rightArm.rotation.x = 45.0f;
		animations[2].keyFrames[1].modelInfo.subRightArm.rotation.z = -90.0f;
		animations[2].keyFrames[1].modelInfo.subRightArm.rotation.x = 70.0f;
		animations[2].keyFrames[1].modelInfo.leftArm.rotation.x = 75.0f;
		animations[2].keyFrames[1].stepsToFrame = 500;

		animations[2].keyFrames[2].modelInfo.torso.rotation.y = 90.0f;
		animations[2].keyFrames[2].modelInfo.torso.position.z = -55.412f;
		animations[2].keyFrames[2].modelInfo.torso.position.y = 0.5;
		animations[2].keyFrames[2].modelInfo.rightArm.rotation.x = -75.0f;
		animations[2].keyFrames[2].modelInfo.leftArm.rotation.x = 75.0f;
		animations[2].keyFrames[2].stepsToFrame = 500;

		animations[2].keyFrames[3].modelInfo.torso.rotation.y = -90.0f;
		animations[2].keyFrames[3].modelInfo.torso.position.z = -55.412f;
		animations[2].keyFrames[3].modelInfo.torso.position.y = 0.5;
		animations[2].keyFrames[3].modelInfo.rightArm.rotation.x = -75.0f;
		animations[2].keyFrames[3].modelInfo.leftArm.rotation.x = 75.0f;
		animations[2].keyFrames[3].stepsToFrame = 500;


		animations[2].keyFrames[4].modelInfo.head.rotation.y = -50.0f;
		animations[2].keyFrames[4].modelInfo.torso.rotation.y = -50.0f;
		animations[2].keyFrames[4].modelInfo.torso.position.z = -55.412f;
		animations[2].keyFrames[4].modelInfo.torso.position.y = 0.5;
		animations[2].keyFrames[4].modelInfo.rightArm.rotation.x = 25.0f;
		animations[2].keyFrames[4].modelInfo.subRightArm.rotation.x = 40.0f;
		animations[2].keyFrames[4].modelInfo.leftArm.rotation.x = 75.0f;
		animations[2].keyFrames[4].stepsToFrame = 500;

		animations[2].keyFrames[5].modelInfo.head.rotation.y = -70.0f;
		animations[2].keyFrames[5].modelInfo.torso.rotation.y = -60.0f;
		animations[2].keyFrames[5].modelInfo.torso.position.z = -55.412f;
		animations[2].keyFrames[5].modelInfo.torso.position.x = -5.236;
		animations[2].keyFrames[5].modelInfo.torso.position.y = 0.5;
		animations[2].keyFrames[5].modelInfo.rightArm.rotation.x = -60.0f;
		animations[2].keyFrames[5].modelInfo.subRightArm.rotation.x = 40.0f;
		animations[2].keyFrames[5].modelInfo.leftArm.rotation.x = 75.0f;
		animations[2].keyFrames[5].stepsToFrame = 500;


		animations[2].keyFrames[6].modelInfo.torso.rotation.y = 90.0f;
		animations[2].keyFrames[6].modelInfo.torso.position.z = -55.412f;
		animations[2].keyFrames[6].modelInfo.torso.position.x = -5.236;
		animations[2].keyFrames[6].modelInfo.torso.position.y = 0.5;
		animations[2].keyFrames[6].modelInfo.rightArm.rotation.x = -75.0f;
		animations[2].keyFrames[6].modelInfo.leftArm.rotation.x = 0.0f;
		animations[2].keyFrames[6].modelInfo.subLeftArm.rotation.x = -40.0f;
		animations[2].keyFrames[6].stepsToFrame = 500;

		animations[2].keyFrames[7].modelInfo.torso.rotation.y = 90.0f;
		animations[2].keyFrames[7].modelInfo.torso.position.z = -55.412f;
		animations[2].keyFrames[7].modelInfo.torso.position.y = 0.5;
		animations[2].keyFrames[7].modelInfo.rightArm.rotation.x = -75.0f;
		animations[2].keyFrames[7].modelInfo.leftArm.rotation.x = 75.0f;
		animations[2].keyFrames[7].stepsToFrame = 500;


	}
}


float interpolar(float a, float b, float t) {
	return (b - a) * t;
}


float interpolation2(float value, float next, int steps)
{


	return  (next - (value)) / steps;



}



int main() {

	startUpFrames();
	// Init GLFW
	glfwInit();


	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Equipo 5. Proyecto Final Grafica", nullptr, nullptr);

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
	Shader skyboxShader("Shader/SkyBox.vs", "Shader/SkyBox.frag");//Skybox


	// ------------------- MODELO PROFESOR -------------------------------------------
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


	// ------------------- MODELO ALUMNO 1 -------------------------------------------
	Model Alumno1Torso((char*)"Models/Alumno1/ProfesorTorso.obj");
	Model Alumno1Head((char*)"Models/Alumno1/ProfesorHead.obj");
	Model Alumno1RightArm((char*)"Models/Alumno1/ProfesorRightArm.obj");
	Model Alumno1SubRightArm((char*)"Models/Alumno1/ProfesorSubRightArm.obj");
	Model Alumno1LeftArm((char*)"Models/Alumno1/ProfesorLeftArm.obj");
	Model Alumno1SubLeftArm((char*)"Models/Alumno1/ProfesorSubLeftArm.obj");
	Model Alumno1RightLeg((char*)"Models/Alumno1/ProfesorRightLeg.obj");
	Model Alumno1SubRightLeg((char*)"Models/Alumno1/ProfesorSubRightLeg.obj");
	Model Alumno1LeftLeg((char*)"Models/Alumno1/ProfesorLeftLeg.obj");
	Model Alumno1SubLeftLeg((char*)"Models/Alumno1/ProfesorSubLeftLeg.obj");

	// ------------------- MODELO ALUMNO 2 -------------------------------------------
	Model Alumno2Torso((char*)"Models/Alumno2/Alumno2Torso.obj");
	Model Alumno2Head((char*)"Models/Alumno2/Alumno2Head.obj");
	Model Alumno2RightArm((char*)"Models/Alumno2/Alumno2RightArm.obj");
	Model Alumno2SubRightArm((char*)"Models/Alumno2/Alumno2SubRightArm.obj");
	Model Alumno2LeftArm((char*)"Models/Alumno2/Alumno2LeftArm.obj");
	Model Alumno2SubLeftArm((char*)"Models/Alumno2/Alumno2SubLeftArm.obj");
	Model Alumno2RightLeg((char*)"Models/Alumno2/Alumno2RightLeg.obj");
	Model Alumno2SubRightLeg((char*)"Models/Alumno2/Alumno2SubRightLeg.obj");
	Model Alumno2LeftLeg((char*)"Models/Alumno2/Alumno2LeftLeg.obj");
	Model Alumno2SubLeftLeg((char*)"Models/Alumno2/Alumno2SubLeftLeg.obj");

	// ------------------- MODELOS LAB VIEJO -----------------------------------------
	Model tornado((char*)"Models/Tornado/tornado.obj");

	Model cuartoOld((char*)"Models/Escenario_Viejo/CuartoViejo/salonViejo.obj");		// LAB VIEJO
	Model ventanasOld((char*)"Models/Escenario_Viejo/ventanasOLD/ventanasOLD.obj");		// VENTANAS OLD
	Model mesaOld((char*)"Models/Escenario_Viejo/mesaOLD/mesaOLD.obj");					// MESA VIEJA
	Model sillaOld((char*)"Models/Escenario_Viejo/sillaOLD/sillaOLD.obj");				// SILLA VIEJA
	Model oldCPU((char*)"Models/Escenario_Viejo/oldCPU/oldCPU.obj");					// CPU VIEJO
	Model monitorOld((char*)"Models/Escenario_Viejo/monitorViejo/monitorViejo.obj");	// MONITOR VIEJO
	Model tecladoOld((char*)"Models/Escenario_Viejo/tecladoViejo/teclado.obj");			// TECLADO VIEJO
	Model mouseOld((char*)"Models/Escenario_Viejo/mouseViejo/mouseViejo.obj");			// MOUSE VIEJO
	Model puertaOld((char*)"Models/Escenario_Viejo/puertaOLD/puertaOLD.obj");			// PUERTA VIEJA

	//Model cuartoOld((char*)"Models/placeholder.obj");		// LAB VIEJO
	//Model ventanasOld((char*)"Models/placeholder.obj");		// VENTANAS OLD
	//Model mesaOld((char*)"Models/placeholder.obj");					// MESA VIEJA
	//Model sillaOld((char*)"Models/placeholder.obj");				// SILLA VIEJA
	//Model oldCPU((char*)"Models/placeholder.obj");					// CPU VIEJO
	//Model monitorOld((char*)"Models/placeholder.obj");	// MONITOR VIEJO
	//Model tecladoOld((char*)"Models/placeholder.obj");			// TECLADO VIEJO
	//Model mouseOld((char*)"Models/placeholder.obj");			// MOUSE VIEJO
	//Model puertaOld((char*)"Models/placeholder.obj");			// PUERTA VIEJA


	// ------------------- MODELOS LAB NUEVO -----------------------------------------
	Model Cuarto((char*)"Models/Escenario_Nuevo/Cuarto/Cuartov2.obj");
	Model Ventana((char*)"Models/Escenario_Nuevo/Ventanas_Vidrio/Ventanas.obj");
	Model Mesa((char*)"Models/Escenario_Nuevo/Mesa/Mesa.obj");
	Model Silla((char*)"Models/Escenario_Nuevo/Silla/Silla.obj");
	Model CPU_1((char*)"Models/Escenario_Nuevo/CPU/CPU1.obj");
	Model Mon1((char*)"Models/Escenario_Nuevo/Monitor/Mon1.obj");
	Model Teclado((char*)"Models/Escenario_Nuevo/Teclado/Teclado.obj");
	Model Mouse((char*)"Models/Escenario_Nuevo/Mouse/Mouse.obj");
	Model MonitorProf((char*)"Models/Escenario_Nuevo/Mon_Prof/Mon_Prof.obj");
	Model MesaProf((char*)"Models/Escenario_Nuevo/Mesa_Profe/Mesa_Prof.obj");

	Model DetachTable((char*)"Models/Escenario_Nuevo/Detach_Table/Table1.obj");
	Model DetachTable2((char*)"Models/Escenario_Nuevo/Detach_Table/Table2.obj");

	// ------------------ MODELOS DECORACIONES --------------------------------------
	Model aloe((char*)"Models/Decoracion/aloe/aloe.obj");
	Model piso((char*)"Models/Decoracion/piso/piso.obj");
	Model arbol((char*)"Models/Decoracion/Tree/Tree.obj");
	// ------------------------------------------------------------------------------


	// ----------------- PLACEHOLDERS -----------------------------------------------
	/*Model Cuarto((char*)"Models/placeholder.obj");
	Model Ventana((char*)"Models/placeholder.obj");
	Model Alumno1((char*)"Models/placeholder.obj");
	Model Mesa((char*)"Models/placeholder.obj");
	Model Silla((char*)"Models/placeholder.obj");
	Model CPU_1((char*)"Models/placeholder.obj");
	Model Mon1((char*)"Models/placeholder.obj");
	Model Teclado((char*)"Models/placeholder.obj");
	Model Mouse((char*)"Models/placeholder.obj");
	Model MonitorProf((char*)"Models/placeholder.obj");
	Model MesaProf((char*)"Models/placeholder.obj");

	Model DetachTable((char*)"Models/placeholder.obj");
	Model DetachTable2((char*)"Models/placeholder.obj");*/
	// ------------------------------------------------------------------------------

	GLfloat skyboxVertices[] = {//Skybox
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =//Skybox
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};



	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;//Skybox
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);//Skybox


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//Skybox
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//Skybox


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


	//Skybox
	GLuint  skyboxVBO, skyboxVAO;//Skybox
	glGenVertexArrays(1, &skyboxVAO);//Skybox
	glGenBuffers(1, &skyboxVBO);//Skybox
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);//Skybox
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);//Skybox
	glEnableVertexAttribArray(0);//Skybox
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);//Skybox

	//load texture skybox
	vector<const GLchar*> faces;//Skybox
	faces.push_back("SkyBox_Desierto/right.png");//Skybox
	faces.push_back("SkyBox_Desierto/left.png");//Skybox
	faces.push_back("SkyBox_Desierto/top.png");//Skybox
	faces.push_back("SkyBox_Desierto/bottom.png");//Skybox
	faces.push_back("SkyBox_Desierto/front.png");//Skybox
	faces.push_back("SkyBox_Desierto/back.png");//Skybox

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);//Skybox




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

		// --------------------------------------------------------------------------------------------
			// ---------------------- DIBUJO LUCES ESCENARIO NUEVO ----------------------------------------
			// --------------------------------------------------------------------------------------------
		if (mostrarEscenarioNuevo) {

			// Directional light-Luz general al escenario
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 20.2f, 13.0f, 10.3f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.3f, 0.3f, 0.3f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.7f, 0.7f, 0.7f);



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

			for (int i = 9; i < 16; ++i) {
				std::string num = std::to_string(i);
				std::string base = "pointLights[" + num + "]";
				glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".ambient").c_str()), 0.0f, 0.0f, 0.0f);
				glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".diffuse").c_str()), 0.0f, 0.0f, 0.0f);
				glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".specular").c_str()), 0.0f, 0.0f, 0.0f);
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
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.1f, 0.1f, 15.0f);     // azul intenso visible
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.3f, 0.3f, 1.0f);    // reflejo azulado
			// Atenuación más suave para mayor alcance
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
			// Ángulos más amplios (más área iluminada)
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(30.0f)));
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(40.0f)));


		}
		// ============================================================================================

		// --------------------------------------------------------------------------------------------
		// ---------------------- DIBUJO LUCES ESCENARIO VIEJO ----------------------------------------
		// --------------------------------------------------------------------------------------------
		else {


			// Directional light-Luz general al escenario
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 20.2f, 13.0f, 10.3f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.4f, 0.4f, 0.4f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.2f, 0.2f, 0.2f);


			//Luz puntual-Luz de lamparas

			for (int i = 0; i < 16; i++) {
				std::string number = std::to_string(i);
				glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].position").c_str()), pointLightPositions2[i].x, pointLightPositions2[i].y, pointLightPositions2[i].z);
				glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].ambient").c_str()), 0.09f, 0.09f, 0.09f);
				glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].diffuse").c_str()), 0.9f, 0.85f, 0.85f);// Blanco intenso
				glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].specular").c_str()), 0.3f, 0.3f, 0.3f);// Reflejo blanco
				//calculo de la atenuacion
				glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].constant").c_str()), 1.0f); // no  tocar mucho
				glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].linear").c_str()), 0.05f);// más = menos alcance
				glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].quadratic").c_str()), 0.009f);// más = menos alcance
			}




			// SpotLight - intensificada para proyector
			// Dirección calculada rotando en eje X
			spotLightDir.x = 0.0f;
			spotLightDir.y = sin(glm::radians(spotPitch));
			spotLightDir.z = -cos(glm::radians(spotPitch));  // apunta hacia "adelante"


			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), 3.0f, 12.3f, -40.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), spotLightDir.x, spotLightDir.y, spotLightDir.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), spotLightDir.x, spotLightDir.y, spotLightDir.z);
			// Intensidades más brillantes
			// Valores nulos para el proyector apagado
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
			// Atenuación más suave para mayor alcance
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
			// Ángulos más amplios (más área iluminada)
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(30.0f)));
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(40.0f)));

		}
		// --------------------------------------------------------------------------------------------




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

		glm::mat4 modelLabViejoYTornado(1);
		modelLabViejoYTornado = glm::translate(modelLabViejoYTornado, glm::vec3(tornadoMove, 0.0f, 0.0f));
		modelLabViejoYTornado = glm::rotate(modelLabViejoYTornado, glm::radians(rotarYEscenario), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 modelLabNuevo(1.0f);
		modelLabNuevo = glm::translate(modelLabNuevo, glm::vec3(0.0f, labNuevoMove, 0.0f));
		modelLabNuevo = glm::rotate(modelLabNuevo, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 modelLabViejo = modelLabViejoYTornado;
		modelLabViejo = glm::translate(modelLabViejo, glm::vec3(labViejoMove, labViejoMove, 0.0f));
		modelLabViejo = glm::rotate(modelLabViejo, glm::radians(rotarYLabViejo), glm::vec3(0.0f, 1.0f, 0.0f));




		// ================PARTE DE CODIGO DE RENDERIZADO DE SKYBOX PARA EL MANEJO DE TRANSPARENCIA=========================================

		// 1. Limpiar buffers al inicio del frame
		// Se borran tanto el color buffer como el depth buffer para empezar un nuevo frame limpio
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// 2. Dibujar el skybox sin escribir en el depth buffer
		// Se desactiva la escritura en el depth buffer para que el skybox no bloquee otros objetos
		glDepthMask(GL_FALSE);
		// Se permite dibujar píxeles con profundidad menor o igual, necesario para el skybox
		glDepthFunc(GL_LEQUAL);
		// Activar el shader del skybox
		skyboxShader.Use();

		// Se elimina la componente de traslación de la cámara para que el skybox no se mueva con ella
		glm::mat4 viewSkybox = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		// Se pasan las matrices de vista y proyección al shader del skybox
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewSkybox));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Se enlaza el VAO y la textura cúbica del skybox
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		// Se dibuja el cubo del skybox
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Se reactiva la escritura en el depth buffer
		glDepthMask(GL_TRUE);
		// Se restaura el comportamiento estándar del depth test
		glDepthFunc(GL_LESS);
		// 3. Dibujar modelos opacos (sin transparencia)
		// Aquí se activan los shaders de iluminación y se dibujan los modelos normales
		lightingShader.Use();
		// 4. Dibujar modelos transparentes (de más lejos a más cerca)
		// Se activa el blending para permitir transparencias (mezclar colores con fondo)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// ... aquí se dibujan modelos con texturas o materiales que contienen canal alfa ...
		glDisable(GL_BLEND);

		// ================================================================================================
		// ============================= MODELADO DECORACIONES ============================================
		// ================================================================================================
		glm::mat4 modelPiso(1.0f);
		modelPiso = glm::translate(modelPiso, glm::vec3(0.0f, 0.0f, 0.0f));
		//modemodelPisolAloe = glm::scale(modelPiso, glm::vec3(1572.0f, 1572.0f, 1572.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPiso));
		piso.Draw(lightingShader);

		glm::mat4 modelEmpty(1.0f);


		glm::mat4 modelArbol(1.0f);
		modelArbol = glm::translate(modelArbol, glm::vec3(100.0f, 0.0f, 0.0f));
		modelArbol = glm::scale(modelArbol, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbol));
		arbol.Draw(lightingShader);


		modelArbol = modelEmpty;
		modelArbol = glm::translate(modelArbol, glm::vec3(80.0f, 0.0f, 60.0f));
		modelArbol = glm::scale(modelArbol, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbol));
		arbol.Draw(lightingShader);


		modelArbol = modelEmpty;
		modelArbol = glm::translate(modelArbol, glm::vec3(70.0f, 0.0f, -80.0f));
		modelArbol = glm::scale(modelArbol, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbol));
		arbol.Draw(lightingShader);

		modelArbol = modelEmpty;
		modelArbol = glm::translate(modelArbol, glm::vec3(-90.0f, 0.0f, 65.0f));
		modelArbol = glm::scale(modelArbol, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbol));
		arbol.Draw(lightingShader);

		modelArbol = modelEmpty;
		modelArbol = glm::translate(modelArbol, glm::vec3(85.0f, 0.0f, 95.0f));
		modelArbol = glm::scale(modelArbol, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbol));
		arbol.Draw(lightingShader);

		modelArbol = modelEmpty;
		modelArbol = glm::translate(modelArbol, glm::vec3(-70.0f, 0.0f, -100.0f));
		modelArbol = glm::scale(modelArbol, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbol));
		arbol.Draw(lightingShader);

		modelArbol = modelEmpty;
		modelArbol = glm::translate(modelArbol, glm::vec3(100.0f, 0.0f, 70.0f));
		modelArbol = glm::scale(modelArbol, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbol));
		arbol.Draw(lightingShader);

		modelArbol = modelEmpty;
		modelArbol = glm::translate(modelArbol, glm::vec3(-100.0f, 0.0f, -90.0f));
		modelArbol = glm::scale(modelArbol, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbol));
		arbol.Draw(lightingShader);

		modelArbol = modelEmpty;
		modelArbol = glm::translate(modelArbol, glm::vec3(95.0f, 0.0f, -65.0f));
		modelArbol = glm::scale(modelArbol, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbol));
		arbol.Draw(lightingShader);

		modelArbol = modelEmpty;
		modelArbol = glm::translate(modelArbol, glm::vec3(-80.0f, 0.0f, 85.0f));
		modelArbol = glm::scale(modelArbol, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbol));
		arbol.Draw(lightingShader);

		modelArbol = modelEmpty;
		modelArbol = glm::translate(modelArbol, glm::vec3(60.0f, 0.0f, 100.0f));
		modelArbol = glm::scale(modelArbol, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbol));
		arbol.Draw(lightingShader);





		//Condicion para intercambiar entre los dos escenarios

		if (mostrarEscenarioNuevo) {



			// =========================================================================================
			// ================================ INICIO DE  MODELADO LAB NUEVO =================================
			// =========================================================================================




		//Modelo cuarto
			glm::mat4 modelCuarto = modelLabNuevo;
			modelCuarto = glm::scale(modelCuarto, glm::vec3(1.0f, 1.0f, 0.9f));
			modelCuarto = glm::translate(modelCuarto, glm::vec3(0.351f, 17.922f, 1.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCuarto));
			Cuarto.Draw(lightingShader);
			
			if (true) {

				//Profesor Torso




				model = modelLabNuevo;
				model = glm::translate(model, personas[1].torso.position);
				modelTemp = model = glm::scale(model, glm::vec3(10.0f));
				modelTemp = model = glm::rotate(model, glm::radians(personas[1].torso.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp = model = glm::rotate(model, glm::radians(personas[1].torso.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp = model = glm::rotate(model, glm::radians(personas[1].torso.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				ProfesorTorso.Draw(lightingShader);




				//Profesor Head



				model = modelLabNuevo;
				model = glm::translate(modelTemp, glm::vec3(0.0f, 1.994f, 0.0f) + personas[1].head.position);
				model = glm::rotate(model, glm::radians(personas[1].head.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[1].head.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[1].head.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				ProfesorHead.Draw(lightingShader);

				//Profesor Right Arm

				model = modelLabNuevo;
				model = glm::translate(modelTemp, glm::vec3(0.039f, 1.903f, -0.282f) + personas[1].rightArm.position);
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].rightArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].rightArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].rightArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				ProfesorRightArm.Draw(lightingShader);

				//Profesor Sub Right Arm

				model = modelLabNuevo;
				model = glm::translate(modelTemp1, glm::vec3(-0.002, 0.011f, -0.281f) + personas[1].subRightArm.position);
				model = glm::rotate(model, glm::radians(personas[1].subRightArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[1].subRightArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[1].subRightArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				ProfesorSubRightArm.Draw(lightingShader);




				////Profesor Left Arm
				model = modelLabNuevo;
				model = glm::translate(modelTemp, glm::vec3(0.04f, 1.9f, 0.24f) + personas[1].leftArm.position);
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].leftArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].leftArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].leftArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				ProfesorLeftArm.Draw(lightingShader);

				//Profesor Sub Left Arm

				model = modelLabNuevo;
				model = glm::translate(modelTemp1, glm::vec3(-0.026f, 0.018f, 0.371f) + personas[1].subLeftArm.position);
				model = glm::rotate(model, glm::radians(personas[1].subLeftArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[1].subLeftArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[1].subLeftArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				ProfesorSubLeftArm.Draw(lightingShader);



				//Profesor Right Leg
				model = modelLabNuevo;
				model = glm::translate(modelTemp, glm::vec3(0.163f, 1.094f, -0.137f) + personas[1].rightLeg.position);
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].rightLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].rightLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].rightLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				ProfesorRightLeg.Draw(lightingShader);

				//Profesor Sub Right Leg

				model = modelLabNuevo;
				model = glm::translate(modelTemp1, glm::vec3(-0.231f, -0.333f, 0.0f) + personas[1].subRightLeg.position);
				model = glm::rotate(model, glm::radians(personas[1].subRightLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[1].subRightLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[1].subRightLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				ProfesorSubRightLeg.Draw(lightingShader);


				//Profesor Left Leg
				model = modelLabNuevo;
				model = glm::translate(modelTemp, glm::vec3(0.151f, 1.114f, 0.153f) + personas[1].leftLeg.position);
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].leftLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].leftLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].leftLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				ProfesorLeftLeg.Draw(lightingShader);

				//Profesor Sub Left Leg

				model = modelLabNuevo;
				model = glm::translate(modelTemp1, glm::vec3(-0.224f, -0.364f, 0.0f) + personas[1].subLeftLeg.position);
				model = glm::rotate(model, glm::radians(personas[1].subLeftLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[1].subLeftLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[1].subLeftLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				ProfesorSubLeftLeg.Draw(lightingShader);

				//Profesor Left Leg
				model = modelLabNuevo;
				model = glm::translate(modelTemp, glm::vec3(0.151f, 1.114f, 0.153f) + personas[1].leftLeg.position);
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].leftLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].leftLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[1].leftLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				ProfesorLeftLeg.Draw(lightingShader);

				//Profesor Sub Left Leg

				model = modelLabNuevo;
				model = glm::translate(modelTemp1, glm::vec3(-0.224f, -0.364f, 0.0f) + personas[1].subLeftLeg.position);
				model = glm::rotate(model, glm::radians(personas[1].subLeftLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[1].subLeftLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[1].subLeftLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				ProfesorSubLeftLeg.Draw(lightingShader);


			}


			if (renderAlumno1) {

				//Alumno1 Torso




				model = modelLabNuevo;
				model = glm::translate(model, personas[0].torso.position);
				modelTemp = model = glm::scale(model, glm::vec3(0.15f));
				modelTemp = model = glm::rotate(model, glm::radians(personas[0].torso.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp = model = glm::rotate(model, glm::radians(personas[0].torso.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp = model = glm::rotate(model, glm::radians(personas[0].torso.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno1Torso.Draw(lightingShader);




				//Alumno1 Head



				model = modelLabNuevo;
				model = glm::translate(modelTemp, glm::vec3(0.0f, 148.54f, 0.0f) + personas[0].head.position);
				model = glm::rotate(model, glm::radians(personas[0].head.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[0].head.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[0].head.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno1Head.Draw(lightingShader);

				//Alumno1 Right Arm

				model = modelLabNuevo;
				model = glm::translate(modelTemp, glm::vec3(-20.02f, 140.164f, 0.0f) + personas[0].rightArm.position);
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[0].rightArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[0].rightArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[0].rightArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno1RightArm.Draw(lightingShader);

				//Alumno1 Sub Right Arm

				model = modelLabNuevo;
				model = glm::translate(modelTemp1, glm::vec3(-15.137, -16.795f, -0.456f) + personas[0].subRightArm.position);
				model = glm::rotate(model, glm::radians(personas[0].subRightArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[0].subRightArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[0].subRightArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno1SubRightArm.Draw(lightingShader);




				////Alumno1 Left Arm
				model = modelLabNuevo;
				model = glm::translate(modelTemp, glm::vec3(19.794f, 140.059f, -0.159f) + personas[0].leftArm.position);
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[0].leftArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[0].leftArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[0].leftArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno1LeftArm.Draw(lightingShader);

				//Alumno1 Sub Left Arm

				model = modelLabNuevo;
				model = glm::translate(modelTemp1, glm::vec3(15.123f, -16.78f, -0.781f) + personas[0].subLeftArm.position);
				model = glm::rotate(model, glm::radians(personas[0].subLeftArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[0].subLeftArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[0].subLeftArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno1SubLeftArm.Draw(lightingShader);



				//Alumno1 Right Leg
				model = modelLabNuevo;
				model = glm::translate(modelTemp, glm::vec3(0.0f, 87.83f, -9.225f) + personas[0].rightLeg.position);
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[0].rightLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[0].rightLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[0].rightLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno1RightLeg.Draw(lightingShader);

				//Alumno1 Sub Right Leg

				model = modelLabNuevo;
				model = glm::translate(modelTemp1, glm::vec3(-10.713f, -34.908f, 14.468f) + personas[0].subRightLeg.position);
				model = glm::rotate(model, glm::radians(personas[0].subRightLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[0].subRightLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[0].subRightLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno1SubRightLeg.Draw(lightingShader);


				//Alumno1 Left Leg
				model = modelLabNuevo;
				model = glm::translate(modelTemp, glm::vec3(9.817f, 87.906f, -9.521f) + personas[0].leftLeg.position);
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[0].leftLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[0].leftLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[0].leftLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno1LeftLeg.Draw(lightingShader);

				//Alumno1 Sub Left Leg

				model = modelLabNuevo;
				model = glm::translate(modelTemp1, glm::vec3(0.272f, -36.701f, 17.087f) + personas[0].subLeftLeg.position);
				model = glm::rotate(model, glm::radians(personas[0].subLeftLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[0].subLeftLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[0].subLeftLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno1SubLeftLeg.Draw(lightingShader);

			}

			//Modelos de accesorios profesor
			glm::mat4 modelMonitorProf = modelLabNuevo;
			modelMonitorProf = glm::translate(modelMonitorProf, glm::vec3(45.0f, 10.1f, -49.0f));
			modelMonitorProf = glm::rotate(modelMonitorProf, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMonitorProf));
			MonitorProf.Draw(lightingShader);


			glm::mat4 modelMesaProf = modelLabNuevo;
			modelMesaProf = glm::translate(modelMesaProf, glm::vec3(45.0f, 5.31f, -48.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesaProf));
			MesaProf.Draw(lightingShader);


			glm::mat4 modelSillaProf = modelLabNuevo;
			modelSillaProf = glm::translate(modelSillaProf, glm::vec3(47.0f, 0.12f, -57.0));
			modelSillaProf = glm::rotate(modelSillaProf, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSillaProf));
			Silla.Draw(lightingShader);


			glm::mat4 modelCPU1_Prof = modelLabNuevo;
			modelCPU1_Prof = glm::translate(modelCPU1_Prof, glm::vec3(47.0f, 12.8f, -46.5f));
			modelCPU1_Prof = glm::rotate(modelCPU1_Prof, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCPU1_Prof));
			CPU_1.Draw(lightingShader);


			glm::mat4 modelTecladoProfe = modelLabNuevo;
			modelTecladoProfe = glm::scale(modelTecladoProfe, glm::vec3(1.1f, 1.1f, 1.1f));
			modelTecladoProfe = glm::translate(modelTecladoProfe, glm::vec3(42.0f, 9.0f, -46.5f));
			modelTecladoProfe = glm::rotate(modelTecladoProfe, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTecladoProfe));
			Teclado.Draw(lightingShader); // Mismo modelo "Teclado"


			glm::mat4 modelMouseProfe = modelLabNuevo;
			modelMouseProfe = glm::translate(modelMouseProfe, glm::vec3(41.0f, 9.9f, -51.0f));
			modelMouseProfe = glm::rotate(modelMouseProfe, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMouseProfe));
			Mouse.Draw(lightingShader);


			//------Inicio de Modelos Separados de columna 1,fila 4 y modelos respectivos 3 y 4------//

			//Mesa 4 Izquierda
			glm::mat4 modelMesa4_Izq = modelLabNuevo;
			modelMesa4_Izq = glm::translate(modelMesa4_Izq, glm::vec3(-31.0f, 5.31f, 42.22f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa4_Izq));
			DetachTable2.Draw(lightingShader);

			//Sillas

			glm::mat4 modelSilla3_3 = modelLabNuevo;
			modelSilla3_3 = glm::translate(modelSilla3_3, glm::vec3(-27.529f, 0.12f, 52.643f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSilla3_3));
			Silla.Draw(lightingShader);

			glm::mat4 modelSilla3_4 = modelLabNuevo;
			modelSilla3_4 = glm::translate(modelSilla3_4, glm::vec3(-17.529f, 0.12f, 52.643f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSilla3_4));
			Silla.Draw(lightingShader);

			//CPUs

			glm::mat4 modelCPU3_3 = modelLabNuevo;
			modelCPU3_3 = glm::translate(modelCPU3_3, glm::vec3(-27.25f, 12.8f, 39.736f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCPU3_3));
			CPU_1.Draw(lightingShader);

			glm::mat4 modelCPU3_4 = modelLabNuevo;
			modelCPU3_4 = glm::translate(modelCPU3_4, glm::vec3(-17.25f, 12.8f, 39.736f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCPU3_4));
			CPU_1.Draw(lightingShader);

			//Monitores

			glm::mat4 modelMon3_3 = modelLabNuevo;
			modelMon3_3 = glm::translate(modelMon3_3, glm::vec3(-26.529f, 10.1f, 41.324f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMon3_3));
			Mon1.Draw(lightingShader);

			glm::mat4 modelMon3_4 = modelLabNuevo;
			modelMon3_4 = glm::translate(modelMon3_4, glm::vec3(-16.529f, 10.1f, 41.324f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMon3_4));
			Mon1.Draw(lightingShader);

			//Teclados

			glm::mat4 modelTeclado3_3 = modelLabNuevo;
			modelTeclado3_3 = glm::translate(modelTeclado3_3, glm::vec3(-26.529f, 10.0f, 44.0f));
			modelTeclado3_3 = glm::scale(modelTeclado3_3, glm::vec3(1.1f, 1.1f, 1.1f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTeclado3_3));
			Teclado.Draw(lightingShader);

			glm::mat4 modelTeclado3_4 = modelLabNuevo;
			modelTeclado3_4 = glm::translate(modelTeclado3_4, glm::vec3(-16.529f, 10.0f, 44.0f));
			modelTeclado3_4 = glm::scale(modelTeclado3_4, glm::vec3(1.1f, 1.1f, 1.1f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTeclado3_4));
			Teclado.Draw(lightingShader);


			//Mouse

			glm::mat4 modelMouse3_3 = modelLabNuevo;
			modelMouse3_3 = glm::translate(modelMouse3_3, glm::vec3(-22.0f, 10.0f, 44.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMouse3_3));
			Mouse.Draw(lightingShader);

			glm::mat4 modelMouse3_4 = modelLabNuevo;
			modelMouse3_4 = glm::translate(modelMouse3_4, glm::vec3(-12.0f, 10.0f, 44.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMouse3_4));
			Mouse.Draw(lightingShader);


			//------FIN de Modelos Separados de columna 1,fila 4 y modelos respectivos 3 y 4------//

			if (true) {

				//Alumno2 Torso




				model = modelSilla3_3;
				model = glm::translate(model, personas[2].torso.position);
				modelTemp = model = glm::scale(model, glm::vec3(10.0f));
				modelTemp = model = glm::rotate(model, glm::radians(personas[2].torso.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp = model = glm::rotate(model, glm::radians(personas[2].torso.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp = model = glm::rotate(model, glm::radians(personas[2].torso.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno2Torso.Draw(lightingShader);




				//Alumno2 Head



				model = modelSilla3_3;
				model = glm::translate(modelTemp, glm::vec3(0.0f, 2.203f, 0.0f) + personas[2].head.position);
				model = glm::rotate(model, glm::radians(personas[2].head.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[2].head.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[2].head.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno2Head.Draw(lightingShader);

				//Alumno2 Right Arm

				model = modelSilla3_3;
				model = glm::translate(modelTemp, glm::vec3(0.0f, 2.002f, -0.281f) + personas[2].rightArm.position);
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[2].rightArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[2].rightArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[2].rightArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno2RightArm.Draw(lightingShader);

				//Alumno2 Sub Right Arm

				model = modelSilla3_3;
				model = glm::translate(modelTemp1, glm::vec3(0.152, -0.23f, -0.207f) + personas[2].subRightArm.position);
				model = glm::rotate(model, glm::radians(personas[2].subRightArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[2].subRightArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[2].subRightArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno2SubRightArm.Draw(lightingShader);




				////Alumno2 Left Arm
				model = modelSilla3_3;
				model = glm::translate(modelTemp, glm::vec3(0.0f, 2.008f, 0.277f) + personas[2].leftArm.position);
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[2].leftArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[2].leftArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[2].leftArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno2LeftArm.Draw(lightingShader);

				//Alumno2 Sub Left Arm

				model = modelSilla3_3;
				model = glm::translate(modelTemp1, glm::vec3(0.147f, -0.254f, 0.227f) + personas[2].subLeftArm.position);
				model = glm::rotate(model, glm::radians(personas[2].subLeftArm.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[2].subLeftArm.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[2].subLeftArm.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno2SubLeftArm.Draw(lightingShader);



				//Alumno2 Right Leg
				model = modelSilla3_3;
				model = glm::translate(modelTemp, glm::vec3(0.156f, 1.148f, -0.155f) + personas[2].rightLeg.position);
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[2].rightLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[2].rightLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[2].rightLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno2RightLeg.Draw(lightingShader);

				//Alumno2 Sub Right Leg

				model = modelSilla3_3;
				model = glm::translate(modelTemp1, glm::vec3(-0.223f, -0.398f, -0.053f) + personas[2].subRightLeg.position);
				model = glm::rotate(model, glm::radians(personas[2].subRightLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[2].subRightLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[2].subRightLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno2SubRightLeg.Draw(lightingShader);


				//Alumno2 Left Leg
				model = modelSilla3_3;
				model = glm::translate(modelTemp, glm::vec3(0.164f, 1.152f, 0.134f) + personas[2].leftLeg.position);
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[2].leftLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[2].leftLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				modelTemp1 = model = glm::rotate(model, glm::radians(personas[2].leftLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno2LeftLeg.Draw(lightingShader);

				//Alumno2 Sub Left Leg

				model = modelSilla3_3;
				model = glm::translate(modelTemp1, glm::vec3(-0.262f, -0.393f, 0.057f) + personas[2].subLeftLeg.position);
				model = glm::rotate(model, glm::radians(personas[2].subLeftLeg.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[2].subLeftLeg.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(personas[2].subLeftLeg.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Alumno2SubLeftLeg.Draw(lightingShader);


			}




			// --- Modelos de mesas ---
			// ---Columna 1 de mesas ---
			glm::mat4 modelMesa1 = modelLabNuevo;
			modelMesa1 = glm::translate(modelMesa1, glm::vec3(-31.0f, 5.31f, -33.176f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa1));
			Mesa.Draw(lightingShader);

			glm::mat4 modelMesa2 = modelLabNuevo;
			modelMesa2 = glm::translate(modelMesa2, glm::vec3(-31.0f, 5.31f, -8.044f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa2));
			Mesa.Draw(lightingShader);



			glm::mat4 modelMesa3 = modelLabNuevo;
			modelMesa3 = glm::translate(modelMesa3, glm::vec3(-31.0f, 5.31f, 17.088f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa3));
			Mesa.Draw(lightingShader);



			//Mesa 4 Derecha
			glm::mat4 modelMesa4_Der = modelLabNuevo;
			modelMesa4_Der = glm::translate(modelMesa4_Der, glm::vec3(-31.0f, 5.31f, 42.22f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa4_Der));
			DetachTable.Draw(lightingShader);




			/// --- Columna 2 de mesas ---
			glm::mat4 modelMesa5 = modelLabNuevo;
			modelMesa5 = glm::scale(modelMesa5, glm::vec3(1.12f, 1.0f, 1.0f)); // Aumenta el tamaño en X, deja Y y Z igual
			modelMesa5 = glm::translate(modelMesa5, glm::vec3(27.8f, 5.31f, -33.176f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa5));
			Mesa.Draw(lightingShader);


			glm::mat4 modelMesa6 = modelLabNuevo;
			modelMesa6 = glm::scale(modelMesa6, glm::vec3(1.12f, 1.0f, 1.0f)); // ajusta según tamaño real del modelo
			modelMesa6 = glm::translate(modelMesa6, glm::vec3(27.8f, 5.31f, -8.044f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa6));
			Mesa.Draw(lightingShader);


			glm::mat4 modelMesa7 = modelLabNuevo;
			modelMesa7 = glm::scale(modelMesa7, glm::vec3(1.12f, 1.0f, 1.0f)); // ajusta según tamaño real del modelo
			modelMesa7 = glm::translate(modelMesa7, glm::vec3(27.8f, 5.31f, 17.088f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa7));
			Mesa.Draw(lightingShader);


			glm::mat4 modelMesa8 = modelLabNuevo;
			modelMesa8 = glm::scale(modelMesa8, glm::vec3(1.12f, 1.0f, 1.0f));  // ajusta según tamaño real del modelo
			modelMesa8 = glm::translate(modelMesa8, glm::vec3(27.8f, 5.31f, 42.22f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMesa8));
			Mesa.Draw(lightingShader);


			// --- Modelos de sillas --- 
			//COLUMNA 1
			int numFilas = 3;         // Número de filas de sillas
			int sillasPorFila = 4;    // Número de sillas en cada fila

			float xBase = -47.529f;         // X de la primera silla de cada fila
			float yBase = 0.12f;          // Y (altura fija)
			float zBase = -23.176f;         // Z de la primera fila
			float separacionX = 10.0f;      // Separación entre sillas (en X)
			float separacionZ = 25.273f;       // Separación entre filas (en Z)

			for (int fila = 0; fila < numFilas; ++fila) {
				float zActual = zBase + fila * separacionZ;
				for (int i = 0; i < sillasPorFila; ++i) {
					glm::mat4 modelSilla = modelLabNuevo;
					modelSilla = glm::translate(modelSilla, glm::vec3(xBase + i * separacionX, yBase, zActual));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSilla));
					Silla.Draw(lightingShader);
				}
			}
			glm::mat4 modelSilla3_1 = modelLabNuevo;
			modelSilla3_1 = glm::translate(modelSilla3_1, glm::vec3(-47.529f, 0.12f, 52.643f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSilla3_1));
			Silla.Draw(lightingShader);

			glm::mat4 modelSilla3_2 = modelLabNuevo;
			modelSilla3_2 = glm::translate(modelSilla3_2, glm::vec3(-37.529f, 0.12f, 52.643f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSilla3_2));
			Silla.Draw(lightingShader);


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
					glm::mat4 modelSilla = modelLabNuevo;
					modelSilla = glm::translate(modelSilla, glm::vec3(xBase_2 + i * separacionX_2, yBase_2, zActual_2));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSilla));
					Silla.Draw(lightingShader);
				}
			}



			// --- Modelos de CPU ---
			//Columna 1
			int numFilas_CPU1 = 3;         // Número de filas de CPU's
			int CPU_PorFila = 4;    // Número de cpu's en cada fila

			float xBase_CPU1 = -47.25f;         // X del primer CPU  de cada fila
			float yBase_CPU1 = 12.8f;          // Y (altura fija)
			float zBase_CPU1 = -35.6f;         // Z de la primera fila
			float separacionX_CPU1 = 10.0F;      // Separación entre cpus (en X)
			float separacionZ_CPU1 = 25.112f;       // Separación entre cpus (en Z)

			for (int fila = 0; fila < numFilas_CPU1; ++fila) {
				float zActual = zBase_CPU1 + fila * separacionZ_CPU1;
				for (int i = 0; i < CPU_PorFila; ++i) {
					glm::mat4 modelCPU1 = modelLabNuevo;
					modelCPU1 = glm::translate(modelCPU1, glm::vec3(xBase_CPU1 + i * separacionX_CPU1, yBase_CPU1, zActual));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCPU1));
					CPU_1.Draw(lightingShader);
				}
			}


			glm::mat4 modelCPU3_1 = modelLabNuevo;
			modelCPU3_1 = glm::translate(modelCPU3_1, glm::vec3(-47.25f, 12.8f, 39.736f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCPU3_1));
			CPU_1.Draw(lightingShader);

			glm::mat4 modelCPU3_2 = modelLabNuevo;
			modelCPU3_2 = glm::translate(modelCPU3_2, glm::vec3(-37.25f, 12.8f, 39.736f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCPU3_2));
			CPU_1.Draw(lightingShader);



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
					glm::mat4 modelCPU2 = modelLabNuevo;
					modelCPU2 = glm::translate(modelCPU2, glm::vec3(xBase_CPU2 + i * separacionX_CPU2, yBase_CPU2, zActual));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCPU2));
					CPU_1.Draw(lightingShader);
				}
			}




			// --- Modelos de Monitores ---

			// Columna 1 (4 monitores por fila, 4 filas)
			int numFilas_Mon1 = 3;
			int Mon1_PorFila = 4;

			float xBase_Mon1 = -46.529f;         // X del primer monitor de la primera columna
			float yBase_Mon1 = 10.1f;           // Y (altura fija)
			float zBase_Mon1 = -33.676f;         // Z inicial de la primera fila
			float separacionX_Mon1 = 10.0f;      // Separación entre monitores (X)
			float separacionZ_Mon1 = 25.0f;      // Separación entre filas (Z)

			for (int fila = 0; fila < numFilas_Mon1; ++fila) {
				float zActual = zBase_Mon1 + fila * separacionZ_Mon1;
				for (int i = 0; i < Mon1_PorFila; ++i) {
					glm::mat4 modelMon1 = modelLabNuevo;
					modelMon1 = glm::translate(modelMon1, glm::vec3(xBase_Mon1 + i * separacionX_Mon1, yBase_Mon1, zActual));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMon1));
					Mon1.Draw(lightingShader);
				}
			}

			glm::mat4 modelMon3_1 = modelLabNuevo;
			modelMon3_1 = glm::translate(modelMon3_1, glm::vec3(-46.529f, 10.1f, 41.324f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMon3_1));
			Mon1.Draw(lightingShader);

			glm::mat4 modelMon3_2 = modelLabNuevo;
			modelMon3_2 = glm::translate(modelMon3_2, glm::vec3(-36.529f, 10.1f, 41.324f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMon3_2));
			Mon1.Draw(lightingShader);



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
					glm::mat4 modelMon2 = modelLabNuevo;
					modelMon2 = glm::translate(modelMon2, glm::vec3(xBase_Mon2 + i * separacionX_Mon2, yBase_Mon2, zActual));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMon2));
					Mon1.Draw(lightingShader);
				}
			}



			// --- Modelos de Teclados ---

			// Columna 1 (4 teclados por fila)
			int numFilas_Teclado1 = 3;
			int Teclados1_PorFila = 4;

			float xBase_Teclado1 = -46.529f;         // X inicial de la primera columna
			float yBase_Teclado1 = 10.0f;             // Altura (ajústala si quieres que esté sobre la mesa)
			float zBase_Teclado1 = -31.0f;         // Z inicial
			float separacionX_Teclado1 = 10.0f;      // Separación entre teclados (X)
			float separacionZ_Teclado1 = 25.0f;      // Separación entre filas (Z)

			for (int fila = 0; fila < numFilas_Teclado1; ++fila) {
				float zActual = zBase_Teclado1 + fila * separacionZ_Teclado1;
				for (int i = 0; i < Teclados1_PorFila; ++i) {
					glm::mat4 modelTeclado1 = modelLabNuevo;
					modelTeclado1 = glm::translate(modelTeclado1, glm::vec3(xBase_Teclado1 + i * separacionX_Teclado1, yBase_Teclado1, zActual));
					modelTeclado1 = glm::scale(modelTeclado1, glm::vec3(1.1f, 1.1f, 1.1f));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTeclado1));
					Teclado.Draw(lightingShader); // Asegúrate que tu modelo de teclado se llame "Teclado"
				}
			}
			glm::mat4 modelTeclado3_1 = modelLabNuevo;
			modelTeclado3_1 = glm::translate(modelTeclado3_1, glm::vec3(-46.529f, 10.0f, 44.0f));
			modelTeclado3_1 = glm::scale(modelTeclado3_1, glm::vec3(1.1f, 1.1f, 1.1f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTeclado3_1));
			Teclado.Draw(lightingShader);

			glm::mat4 modelTeclado3_2 = modelLabNuevo;
			modelTeclado3_2 = glm::translate(modelTeclado3_2, glm::vec3(-36.529f, 10.0f, 44.0f));
			modelTeclado3_2 = glm::scale(modelTeclado3_2, glm::vec3(1.1f, 1.1f, 1.1f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTeclado3_2));
			Teclado.Draw(lightingShader);



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
					glm::mat4 modelTeclado2 = modelLabNuevo;
					modelTeclado2 = glm::translate(modelTeclado2, glm::vec3(xBase_Teclado2 + i * separacionX_Teclado2, yBase_Teclado2, zActual));
					modelTeclado2 = glm::scale(modelTeclado2, glm::vec3(1.1f, 1.1f, 1.1f));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTeclado2));
					Teclado.Draw(lightingShader); // Mismo modelo "Teclado"
				}
			}

			// --- Modelos de Mouse ---

			// Columna 1 (4 mouses por fila)
			int numFilas_Mouse1 = 3;
			int Mouses1_PorFila = 4;

			float xBase_Mouse1 = -42.0f;
			float yBase_Mouse1 = 10.0f;
			float zBase_Mouse1 = -31.0f;
			float separacionX_Mouse1 = 10.0f;
			float separacionZ_Mouse1 = 25.0f;

			for (int fila = 0; fila < numFilas_Mouse1; ++fila) {
				float zActual = zBase_Mouse1 + fila * separacionZ_Mouse1;
				for (int i = 0; i < Mouses1_PorFila; ++i) {
					glm::mat4 modelMouse1 = modelLabNuevo;
					modelMouse1 = glm::translate(modelMouse1, glm::vec3(xBase_Mouse1 + i * separacionX_Mouse1, yBase_Mouse1, zActual));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMouse1));
					Mouse.Draw(lightingShader); // Modelo de Mouse
				}
			}

			glm::mat4 modelMouse3_1 = modelLabNuevo;
			modelMouse3_1 = glm::translate(modelMouse3_1, glm::vec3(-42.0f, 10.0f, 44.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMouse3_1));
			Mouse.Draw(lightingShader);

			glm::mat4 modelMouse3_2 = modelLabNuevo;
			modelMouse3_2 = glm::translate(modelMouse3_2, glm::vec3(-32.0f, 10.0f, 44.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMouse3_2));
			Mouse.Draw(lightingShader);


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
					glm::mat4 modelMouse2 = modelLabNuevo;
					modelMouse2 = glm::translate(modelMouse2, glm::vec3(xBase_Mouse2 + i * separacionX_Mouse2, yBase_Mouse2, zActual));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMouse2));
					Mouse.Draw(lightingShader);
				}
			}


			//--- Modelo ventanas con trasnparencia---

			glm::mat4 modelVentanas = modelLabNuevo;
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

		}
		else {

			// =========================================================================================
		// ===================== MODELADO LABORATORIO VIEJO ========================================
		// =========================================================================================


		//Tornado

			glm::mat4 tornadoModel = modelLabViejoYTornado;
			tornadoModel = glm::scale(tornadoModel, glm::vec3(tornadoScale, tornadoScale, tornadoScale));
			//tornadoModel = glm::translate(tornadoModel, glm::vec3(0.0, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(tornadoModel));
			tornado.Draw(lightingShader);

			// ------------- Modelado salón viejo ----------------------------------------------
			glm::mat4 modelCuartoOld = modelLabViejo;
			modelCuartoOld = glm::scale(modelCuartoOld, glm::vec3(1.0f, 1.0f, 1.0f));
			modelCuartoOld = glm::translate(modelCuartoOld, glm::vec3(0.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCuartoOld));
			cuartoOld.Draw(lightingShader);
			// ventanas y puerta se dibujan al final para activar la transparencia

		   // ---------------------------------------------------------------------------------

		   // ------------- Modelado escritorio Alumno1 ----------------------------------------------
		   // Escritorio 1 (personas[0])
			glm::mat4 modelOldMesa1 = modelLabViejo;
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa1));
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa1));
			modelOldMesa1 = glm::translate(modelOldMesa1, glm::vec3(38.224f, 6.758f, -59.289f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa1));
			mesaOld.Draw(lightingShader);

			// CPU personas[0]
			glm::mat4 modeloldCPU1 = modelLabViejo;
			modeloldCPU1 = glm::translate(modeloldCPU1, glm::vec3(42.13f, 14.443f, -59.904f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU1));
			oldCPU.Draw(lightingShader);

			// monitor Alumno1
			glm::mat4 modeloldMonitor1 = modelLabViejo;
			modeloldMonitor1 = glm::translate(modeloldMonitor1, glm::vec3(37.701f, 14.434f, -60.837f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor1));
			monitorOld.Draw(lightingShader);

			// teclado Alumno1
			glm::mat4 modeloldTeclado1 = modelLabViejo;
			modeloldTeclado1 = glm::translate(modeloldTeclado1, glm::vec3(37.659f, 12.327f, -57.878f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado1));
			tecladoOld.Draw(lightingShader);

			// mouse Alumno1
			glm::mat4 modeloldMouse1 = modelLabViejo;
			modeloldMouse1 = glm::translate(modeloldMouse1, glm::vec3(38.811f, 12.405f, -59.408f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse1));
			mouseOld.Draw(lightingShader);

			// silla Alumno1
			glm::mat4 modeloldSilla1 = modelLabViejo;
			modeloldSilla1 = glm::translate(modeloldSilla1, glm::vec3(37.575f, 6.377f, -49.771f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla1));
			sillaOld.Draw(lightingShader);


			// ---------------------------------------------------------------------------------
			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// ------------- Modelado fila 1 columna 1 ----------------------------------------------


			// Escritorio 2
			glm::mat4 modelOldMesa2 = modelLabViejo;
			modelOldMesa2 = glm::translate(modelOldMesa2, glm::vec3(-40.424f, 6.758f, -28.937f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa2));
			mesaOld.Draw(lightingShader);

			// Escritorio 3
			glm::mat4 modelOldMesa3 = modelLabViejo;
			modelOldMesa3 = glm::translate(modelOldMesa3, glm::vec3(-20.105f, 6.758f, -28.937f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa3));
			mesaOld.Draw(lightingShader);


			// ----------- CPUs --------------------
			// CPU 2
			glm::mat4 modeloldCPU2 = modelLabViejo;
			modeloldCPU2 = glm::translate(modeloldCPU2, glm::vec3(-49.072f, 14.443f, -30.699f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU2));
			oldCPU.Draw(lightingShader);
			// CPU 3
			glm::mat4 modeloldCPU3 = modelLabViejo;
			modeloldCPU3 = glm::translate(modeloldCPU3, glm::vec3(-39.384f, 14.443f, -30.699f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU3));
			oldCPU.Draw(lightingShader);
			// CPU 4
			glm::mat4 modeloldCPU4 = modelLabViejo;
			modeloldCPU4 = glm::translate(modeloldCPU4, glm::vec3(-28.76f, 14.443f, -30.699f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU4));
			oldCPU.Draw(lightingShader);
			// CPU 5
			glm::mat4 modeloldCPU5 = modelLabViejo;
			modeloldCPU5 = glm::translate(modeloldCPU5, glm::vec3(-19.072f, 14.443f, -30.699f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU5));
			oldCPU.Draw(lightingShader);


			// ------------ monitores ------------
			// monitor 2
			glm::mat4 modeloldMonitor2 = modelLabViejo;
			modeloldMonitor2 = glm::translate(modeloldMonitor2, glm::vec3(-44.764f, 14.434f, -31.492f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor2));
			monitorOld.Draw(lightingShader);
			// monitor 3
			glm::mat4 modeloldMonitor3 = modelLabViejo;
			modeloldMonitor3 = glm::translate(modeloldMonitor3, glm::vec3(-34.541f, 14.434f, -31.492f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor3));
			monitorOld.Draw(lightingShader);
			// monitor 4
			glm::mat4 modeloldMonitor4 = modelLabViejo;
			modeloldMonitor4 = glm::translate(modeloldMonitor4, glm::vec3(-24.452f, 14.434f, -31.492f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor4));
			monitorOld.Draw(lightingShader);
			// monitor 5
			glm::mat4 modeloldMonitor5 = modelLabViejo;
			modeloldMonitor5 = glm::translate(modeloldMonitor5, glm::vec3(-14.616f, 14.434f, -31.492f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor5));
			monitorOld.Draw(lightingShader);


			// -------- teclados -----------------
			// teclado 2
			glm::mat4 modeloldTeclado2 = modelLabViejo;
			modeloldTeclado2 = glm::translate(modeloldTeclado2, glm::vec3(-45.233f, 12.299f, -28.673f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado2));
			tecladoOld.Draw(lightingShader);
			// teclado 3
			glm::mat4 modeloldTeclado3 = modelLabViejo;
			modeloldTeclado3 = glm::translate(modeloldTeclado3, glm::vec3(-35.143f, 12.299f, -28.896f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado3));
			tecladoOld.Draw(lightingShader);
			// teclado 4
			glm::mat4 modeloldTeclado4 = modelLabViejo;
			modeloldTeclado4 = glm::translate(modeloldTeclado4, glm::vec3(-24.92f, 12.299f, -28.673f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado4));
			tecladoOld.Draw(lightingShader);
			// teclado 5
			glm::mat4 modeloldTeclado5 = modelLabViejo;
			modeloldTeclado5 = glm::translate(modeloldTeclado5, glm::vec3(-14.831f, 12.299f, -28.896f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado5));
			tecladoOld.Draw(lightingShader);


			// ---------- mouses -------------------
			// mouse 2
			glm::mat4 modeloldMouse2 = modelLabViejo;
			modeloldMouse2 = glm::translate(modeloldMouse2, glm::vec3(-43.789f, 12.405f, -30.203f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse2));
			mouseOld.Draw(lightingShader);
			// mouse 3
			glm::mat4 modeloldMouse3 = modelLabViejo;
			modeloldMouse3 = glm::translate(modeloldMouse3, glm::vec3(-33.594f, 12.405f, -30.203f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse3));
			mouseOld.Draw(lightingShader);
			// mouse 4
			glm::mat4 modeloldMouse4 = modelLabViejo;
			modeloldMouse4 = glm::translate(modeloldMouse4, glm::vec3(-23.476f, 12.405f, -30.203f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse4));
			mouseOld.Draw(lightingShader);
			// mouse 5
			glm::mat4 modeloldMouse5 = modelLabViejo;
			modeloldMouse5 = glm::translate(modeloldMouse5, glm::vec3(-13.281f, 12.405f, -30.203f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse5));
			mouseOld.Draw(lightingShader);


			// -------------- sillas ------------------------------
			// silla 2
			glm::mat4 modeloldSilla2 = modelLabViejo;
			modeloldSilla2 = glm::translate(modeloldSilla2, glm::vec3(-45.472f, 6.377f, -23.339f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla2));
			sillaOld.Draw(lightingShader);
			// silla 3
			glm::mat4 modeloldSilla3 = modelLabViejo;
			modeloldSilla3 = glm::translate(modeloldSilla3, glm::vec3(-35.524f, 6.377f, -23.339f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla3));
			sillaOld.Draw(lightingShader);
			// silla 4
			glm::mat4 modeloldSilla4 = modelLabViejo;
			modeloldSilla4 = glm::translate(modeloldSilla4, glm::vec3(-25.261f, 6.377f, -23.339f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla4));
			sillaOld.Draw(lightingShader);
			// silla 5
			glm::mat4 modeloldSilla5 = modelLabViejo;
			modeloldSilla5 = glm::translate(modeloldSilla5, glm::vec3(-15.112f, 6.377f, -23.339f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla5));
			sillaOld.Draw(lightingShader);


			// ---------------------------------------------------------------------------------
			// ---------------------------------------------------------------------------------
			// 
			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// ------------- Modelado fila 2 columna 1 ----------------------------------------------

			// Escritorio 4
			glm::mat4 modelOldMesa4 = modelLabViejo;
			modelOldMesa4 = glm::translate(modelOldMesa4, glm::vec3(-40.424f, 6.758f, -2.779f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa4));
			mesaOld.Draw(lightingShader);

			// Escritorio 5
			glm::mat4 modelOldMesa5 = modelLabViejo;
			modelOldMesa5 = glm::translate(modelOldMesa5, glm::vec3(-20.105f, 6.758f, -2.779f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa5));
			mesaOld.Draw(lightingShader);

			// ----------- CPUs --------------------
			// CPU 6
			glm::mat4 modeloldCPU6 = modelLabViejo;
			modeloldCPU6 = glm::translate(modeloldCPU6, glm::vec3(-49.072f, 14.443f, -3.594f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU6));
			oldCPU.Draw(lightingShader);
			// CPU 7
			glm::mat4 modeloldCPU7 = modelLabViejo;
			modeloldCPU7 = glm::translate(modeloldCPU7, glm::vec3(-39.384f, 14.443f, -3.594f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU7));
			oldCPU.Draw(lightingShader);
			// CPU 8
			glm::mat4 modeloldCPU8 = modelLabViejo;
			modeloldCPU8 = glm::translate(modeloldCPU8, glm::vec3(-28.76f, 14.443f, -3.594f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU8));
			oldCPU.Draw(lightingShader);
			// CPU 9
			glm::mat4 modeloldCPU9 = modelLabViejo;
			modeloldCPU9 = glm::translate(modeloldCPU9, glm::vec3(-19.072f, 14.443f, -3.594f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU9));
			oldCPU.Draw(lightingShader);
			// ------------ monitores ------------
			// monitor 6
			glm::mat4 modeloldMonitor6 = modelLabViejo;
			modeloldMonitor6 = glm::translate(modeloldMonitor6, glm::vec3(-44.764f, 14.434f, -4.386f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor6));
			monitorOld.Draw(lightingShader);
			// monitor 7
			glm::mat4 modeloldMonitor7 = modelLabViejo;
			modeloldMonitor7 = glm::translate(modeloldMonitor7, glm::vec3(-34.541f, 14.434f, -4.386f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor7));
			monitorOld.Draw(lightingShader);
			// monitor 8
			glm::mat4 modeloldMonitor8 = modelLabViejo;
			modeloldMonitor8 = glm::translate(modeloldMonitor8, glm::vec3(-24.452f, 14.434f, -4.386f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor8));
			monitorOld.Draw(lightingShader);
			// monitor 9
			glm::mat4 modeloldMonitor9 = modelLabViejo;
			modeloldMonitor9 = glm::translate(modeloldMonitor9, glm::vec3(-14.616f, 14.434f, -4.386f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor9));
			monitorOld.Draw(lightingShader);
			// -------- teclados -----------------
			// teclado 6
			glm::mat4 modeloldTeclado6 = modelLabViejo;
			modeloldTeclado6 = glm::translate(modeloldTeclado6, glm::vec3(-45.233f, 12.299f, -1.568f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado6));
			tecladoOld.Draw(lightingShader);
			// teclado 7
			glm::mat4 modeloldTeclado7 = modelLabViejo;
			modeloldTeclado7 = glm::translate(modeloldTeclado7, glm::vec3(-35.143f, 12.299f, -1.568f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado7));
			tecladoOld.Draw(lightingShader);
			// teclado 8
			glm::mat4 modeloldTeclado8 = modelLabViejo;
			modeloldTeclado8 = glm::translate(modeloldTeclado8, glm::vec3(-24.92f, 12.299f, -1.568f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado8));
			tecladoOld.Draw(lightingShader);
			// teclado 9
			glm::mat4 modeloldTeclado9 = modelLabViejo;
			modeloldTeclado9 = glm::translate(modeloldTeclado9, glm::vec3(-14.831f, 12.299f, -1.568f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado9));
			tecladoOld.Draw(lightingShader);
			// ---------- mouses -------------------
			// mouse 6
			glm::mat4 modeloldMouse6 = modelLabViejo;
			modeloldMouse6 = glm::translate(modeloldMouse6, glm::vec3(-43.789f, 12.405f, -3.098f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse6));
			mouseOld.Draw(lightingShader);
			// mouse 7
			glm::mat4 modeloldMouse7 = modelLabViejo;
			modeloldMouse7 = glm::translate(modeloldMouse7, glm::vec3(-33.594f, 12.405f, -3.098f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse7));
			mouseOld.Draw(lightingShader);
			// mouse 4
			glm::mat4 modeloldMouse8 = modelLabViejo;
			modeloldMouse8 = glm::translate(modeloldMouse8, glm::vec3(-23.476f, 12.405f, -3.098f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse8));
			mouseOld.Draw(lightingShader);
			// mouse 9
			glm::mat4 modeloldMouse9 = modelLabViejo;
			modeloldMouse9 = glm::translate(modeloldMouse9, glm::vec3(-13.281f, 12.405f, -3.098f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse9));
			mouseOld.Draw(lightingShader);
			// ----------- sillas ----------------------------
			// silla 6
			glm::mat4 modeloldSilla6 = modelLabViejo;
			modeloldSilla6 = glm::translate(modeloldSilla6, glm::vec3(-45.472f, 6.377f, 5.122f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla6));
			sillaOld.Draw(lightingShader);
			// silla 7
			glm::mat4 modeloldSilla7 = modelLabViejo;
			modeloldSilla7 = glm::translate(modeloldSilla7, glm::vec3(-35.524f, 6.377f, 5.122f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla7));
			sillaOld.Draw(lightingShader);
			// silla 8
			glm::mat4 modeloldSilla8 = modelLabViejo;
			modeloldSilla8 = glm::translate(modeloldSilla8, glm::vec3(-25.261f, 6.377f, 5.122f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla8));
			sillaOld.Draw(lightingShader);
			// silla 9
			glm::mat4 modeloldSilla9 = modelLabViejo;
			modeloldSilla9 = glm::translate(modeloldSilla9, glm::vec3(-15.112f, 6.377f, 5.122f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla9));
			sillaOld.Draw(lightingShader);


			// ---------------------------------------------------------------------------------

			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// ------------- Modelado fila 3 columna 1 ----------------------------------------------
			// 

			// Escritorio 6
			glm::mat4 modelOldMesa6 = modelLabViejo;
			modelOldMesa6 = glm::translate(modelOldMesa6, glm::vec3(-40.424f, 6.758f, 22.489f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa6));
			mesaOld.Draw(lightingShader);
			// Escritorio 7
			glm::mat4 modelOldMesa7 = modelLabViejo;
			modelOldMesa7 = glm::translate(modelOldMesa7, glm::vec3(-20.105f, 6.758f, 22.489f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa7));
			mesaOld.Draw(lightingShader);
			// ----------- CPUs --------------------
			// CPU 10
			glm::mat4 modeloldCPU10 = modelLabViejo;
			modeloldCPU10 = glm::translate(modeloldCPU10, glm::vec3(-49.072f, 14.443f, 21.984f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU10));
			oldCPU.Draw(lightingShader);
			// CPU 11
			glm::mat4 modeloldCPU11 = modelLabViejo;
			modeloldCPU11 = glm::translate(modeloldCPU11, glm::vec3(-39.384f, 14.443f, 21.984f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU11));
			oldCPU.Draw(lightingShader);
			// CPU 12
			glm::mat4 modeloldCPU12 = modelLabViejo;
			modeloldCPU12 = glm::translate(modeloldCPU12, glm::vec3(-28.76f, 14.443f, 21.984f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU12));
			oldCPU.Draw(lightingShader);
			// CPU 13
			glm::mat4 modeloldCPU13 = modelLabViejo;
			modeloldCPU13 = glm::translate(modeloldCPU13, glm::vec3(-19.072f, 14.443f, 21.984f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU13));
			oldCPU.Draw(lightingShader);
			// ------------ monitores ------------
			// monitor 10
			glm::mat4 modeloldMonitor10 = modelLabViejo;
			modeloldMonitor10 = glm::translate(modeloldMonitor10, glm::vec3(-44.764f, 14.434f, 21.191f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor10));
			monitorOld.Draw(lightingShader);
			// monitor 11
			glm::mat4 modeloldMonitor11 = modelLabViejo;
			modeloldMonitor11 = glm::translate(modeloldMonitor11, glm::vec3(-34.541f, 14.434f, 21.191f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor11));
			monitorOld.Draw(lightingShader);
			// monitor 12
			glm::mat4 modeloldMonitor12 = modelLabViejo;
			modeloldMonitor12 = glm::translate(modeloldMonitor12, glm::vec3(-24.452f, 14.434f, 21.191f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor12));
			monitorOld.Draw(lightingShader);
			// monitor 13
			glm::mat4 modeloldMonitor13 = modelLabViejo;
			modeloldMonitor13 = glm::translate(modeloldMonitor13, glm::vec3(-14.616f, 14.434f, 21.191f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor13));
			monitorOld.Draw(lightingShader);
			// -------- teclados -----------------
			// teclado 10
			glm::mat4 modeloldTeclado10 = modelLabViejo;
			modeloldTeclado10 = glm::translate(modeloldTeclado10, glm::vec3(-45.233f, 12.299f, 24.009f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado10));
			tecladoOld.Draw(lightingShader);
			// teclado 11
			glm::mat4 modeloldTeclado11 = modelLabViejo;
			modeloldTeclado11 = glm::translate(modeloldTeclado11, glm::vec3(-35.143f, 12.299f, 23.786f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado11));
			tecladoOld.Draw(lightingShader);
			// teclado 12
			glm::mat4 modeloldTeclado12 = modelLabViejo;
			modeloldTeclado12 = glm::translate(modeloldTeclado12, glm::vec3(-24.92f, 12.299f, 23.786f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado12));
			tecladoOld.Draw(lightingShader);
			// teclado 13
			glm::mat4 modeloldTeclado13 = modelLabViejo;
			modeloldTeclado13 = glm::translate(modeloldTeclado13, glm::vec3(-14.831f, 12.299f, 24.009f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado13));
			tecladoOld.Draw(lightingShader);
			// ---------- mouses -------------------
			// mouse 10
			glm::mat4 modeloldMouse10 = modelLabViejo;
			modeloldMouse10 = glm::translate(modeloldMouse10, glm::vec3(-43.789f, 12.405f, 22.479f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse10));
			mouseOld.Draw(lightingShader);
			// mouse 11
			glm::mat4 modeloldMouse11 = modelLabViejo;
			modeloldMouse11 = glm::translate(modeloldMouse11, glm::vec3(-33.594f, 12.405f, 22.254f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse11));
			mouseOld.Draw(lightingShader);
			// mouse 12
			glm::mat4 modeloldMouse12 = modelLabViejo;
			modeloldMouse12 = glm::translate(modeloldMouse12, glm::vec3(-23.476f, 12.405f, 22.479f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse12));
			mouseOld.Draw(lightingShader);
			// mouse 13
			glm::mat4 modeloldMouse13 = modelLabViejo;
			modeloldMouse13 = glm::translate(modeloldMouse13, glm::vec3(-13.281f, 12.405f, 22.254f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse13));
			mouseOld.Draw(lightingShader);
			// ----------- sillas ----------------------------
			// silla 10
			glm::mat4 modeloldSilla10 = modelLabViejo;
			modeloldSilla10 = glm::translate(modeloldSilla10, glm::vec3(-45.472f, 6.377f, 29.252f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla10));
			sillaOld.Draw(lightingShader);
			// silla 11
			glm::mat4 modeloldSilla11 = modelLabViejo;
			modeloldSilla11 = glm::translate(modeloldSilla11, glm::vec3(-35.524f, 6.377f, 29.252f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla11));
			sillaOld.Draw(lightingShader);
			// silla 12
			glm::mat4 modeloldSilla12 = modelLabViejo;
			modeloldSilla12 = glm::translate(modeloldSilla12, glm::vec3(-25.261f, 6.377f, 29.252f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla12));
			sillaOld.Draw(lightingShader);
			// silla 13
			glm::mat4 modeloldSilla13 = modelLabViejo;
			modeloldSilla13 = glm::translate(modeloldSilla13, glm::vec3(-15.112f, 6.377f, 29.252f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla13));
			sillaOld.Draw(lightingShader);


			// ---------------------------------------------------------------------------------

			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// ------------- Modelado fila 4 columna 1 ----------------------------------------------


			// Escritorio 8
			glm::mat4 modelOldMesa8 = modelLabViejo;
			modelOldMesa8 = glm::translate(modelOldMesa8, glm::vec3(-40.424f, 6.758f, 48.24f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa8));
			mesaOld.Draw(lightingShader);
			// Escritorio 9
			glm::mat4 modelOldMesa9 = modelLabViejo;
			modelOldMesa9 = glm::translate(modelOldMesa9, glm::vec3(-20.105f, 6.758f, 48.24f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa9));
			mesaOld.Draw(lightingShader);


			// ----------- CPUs --------------------
			// CPU 14
			glm::mat4 modeloldCPU14 = modelLabViejo;
			modeloldCPU14 = glm::translate(modeloldCPU14, glm::vec3(-49.072f, 14.443f, 47.706f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU14));
			oldCPU.Draw(lightingShader);
			// CPU 15
			glm::mat4 modeloldCPU15 = modelLabViejo;
			modeloldCPU15 = glm::translate(modeloldCPU15, glm::vec3(-39.384f, 14.443f, 47.706f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU15));
			oldCPU.Draw(lightingShader);
			// CPU 16
			glm::mat4 modeloldCPU16 = modelLabViejo;
			modeloldCPU16 = glm::translate(modeloldCPU16, glm::vec3(-28.76f, 14.443f, 47.706f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU16));
			oldCPU.Draw(lightingShader);
			// CPU 17
			glm::mat4 modeloldCPU17 = modelLabViejo;
			modeloldCPU17 = glm::translate(modeloldCPU17, glm::vec3(-19.072f, 14.443f, 47.706f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU17));
			oldCPU.Draw(lightingShader);
			// ------------ monitores ------------
			// monitor 14
			glm::mat4 modeloldMonitor14 = modelLabViejo;
			modeloldMonitor14 = glm::translate(modeloldMonitor14, glm::vec3(-44.764f, 14.434f, 46.913f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor14));
			monitorOld.Draw(lightingShader);
			// monitor 15
			glm::mat4 modeloldMonitor15 = modelLabViejo;
			modeloldMonitor15 = glm::translate(modeloldMonitor15, glm::vec3(-34.541f, 14.434f, 46.913f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor15));
			monitorOld.Draw(lightingShader);
			// monitor 16
			glm::mat4 modeloldMonitor16 = modelLabViejo;
			modeloldMonitor16 = glm::translate(modeloldMonitor16, glm::vec3(-24.452f, 14.434f, 46.913f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor16));
			monitorOld.Draw(lightingShader);
			// monitor 17
			glm::mat4 modeloldMonitor17 = modelLabViejo;
			modeloldMonitor17 = glm::translate(modeloldMonitor17, glm::vec3(-14.616f, 14.434f, 46.913f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor17));
			monitorOld.Draw(lightingShader);
			// -------- teclados -----------------
			// teclado 14
			glm::mat4 modeloldTeclado14 = modelLabViejo;
			modeloldTeclado14 = glm::translate(modeloldTeclado14, glm::vec3(-45.233f, 12.299f, 49.709f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado14));
			tecladoOld.Draw(lightingShader);
			// teclado 15
			glm::mat4 modeloldTeclado15 = modelLabViejo;
			modeloldTeclado15 = glm::translate(modeloldTeclado15, glm::vec3(-35.143f, 12.299f, 49.532f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado15));
			tecladoOld.Draw(lightingShader);
			// teclado 16
			glm::mat4 modeloldTeclado16 = modelLabViejo;
			modeloldTeclado16 = glm::translate(modeloldTeclado16, glm::vec3(-24.92f, 12.299f, 49.532f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado16));
			tecladoOld.Draw(lightingShader);
			// teclado 17
			glm::mat4 modeloldTeclado17 = modelLabViejo;
			modeloldTeclado17 = glm::translate(modeloldTeclado17, glm::vec3(-14.831f, 12.299f, 49.709f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado17));
			tecladoOld.Draw(lightingShader);
			// ---------- mouses -------------------
			// mouse 14
			glm::mat4 modeloldMouse14 = modelLabViejo;
			modeloldMouse14 = glm::translate(modeloldMouse14, glm::vec3(-43.789f, 12.405f, 48.202f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse14));
			mouseOld.Draw(lightingShader);
			// mouse 15
			glm::mat4 modeloldMouse15 = modelLabViejo;
			modeloldMouse15 = glm::translate(modeloldMouse15, glm::vec3(-33.594f, 12.405f, 48.202f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse15));
			mouseOld.Draw(lightingShader);
			// mouse 16
			glm::mat4 modeloldMouse16 = modelLabViejo;
			modeloldMouse16 = glm::translate(modeloldMouse16, glm::vec3(-23.476f, 12.405f, 48.202f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse16));
			mouseOld.Draw(lightingShader);
			// mouse 17
			glm::mat4 modeloldMouse17 = modelLabViejo;
			modeloldMouse17 = glm::translate(modeloldMouse17, glm::vec3(-13.281f, 12.405f, 48.202f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse17));
			mouseOld.Draw(lightingShader);
			// ----------- sillas ----------------------------
			// silla 14
			glm::mat4 modeloldSilla14 = modelLabViejo;
			modeloldSilla14 = glm::translate(modeloldSilla14, glm::vec3(-45.472f, 6.377f, 56.301f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla14));
			sillaOld.Draw(lightingShader);
			// silla 15
			glm::mat4 modeloldSilla15 = modelLabViejo;
			modeloldSilla15 = glm::translate(modeloldSilla15, glm::vec3(-35.524f, 6.377f, 56.301f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla15));
			sillaOld.Draw(lightingShader);
			// silla 16
			glm::mat4 modeloldSilla16 = modelLabViejo;
			modeloldSilla16 = glm::translate(modeloldSilla16, glm::vec3(-25.261f, 6.377f, 56.301f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla16));
			sillaOld.Draw(lightingShader);
			// silla 17
			glm::mat4 modeloldSilla17 = modelLabViejo;
			modeloldSilla17 = glm::translate(modeloldSilla17, glm::vec3(-15.112f, 6.377f, 56.301f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla17));
			sillaOld.Draw(lightingShader);


			// ---------------------------------------------------------------------------------

			// ---------------------------------------------------------------------------------
			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// ------------- Modelado fila 1 columna 2 ----------------------------------------------


			// Escritorio 10
			glm::mat4 modelOldMesa10 = modelLabViejo;
			modelOldMesa10 = glm::translate(modelOldMesa10, glm::vec3(18.113f, 6.758f, -28.937f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa10));
			mesaOld.Draw(lightingShader);
			// Escritorio 11
			glm::mat4 modelOldMesa11 = modelLabViejo;
			modelOldMesa11 = glm::translate(modelOldMesa11, glm::vec3(38.431f, 6.758f, -28.937f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa11));
			mesaOld.Draw(lightingShader);

			// ----------- CPUs --------------------
			// CPU 18
			glm::mat4 modeloldCPU18 = modelLabViejo;
			modeloldCPU18 = glm::translate(modeloldCPU18, glm::vec3(15.345f, 14.443f, -29.801f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU18));
			oldCPU.Draw(lightingShader);
			// CPU 19
			glm::mat4 modeloldCPU19 = modelLabViejo;
			modeloldCPU19 = glm::translate(modeloldCPU19, glm::vec3(24.0f, 14.443f, -29.801f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU19));
			oldCPU.Draw(lightingShader);
			// CPU 20
			glm::mat4 modeloldCPU20 = modelLabViejo;
			modeloldCPU20 = glm::translate(modeloldCPU20, glm::vec3(32.721f, 14.443f, -29.801f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU20));
			oldCPU.Draw(lightingShader);
			// CPU 21
			glm::mat4 modeloldCPU21 = modelLabViejo;
			modeloldCPU21 = glm::translate(modeloldCPU21, glm::vec3(41.355f, 14.443f, -29.801f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU21));
			oldCPU.Draw(lightingShader);
			// CPU 22
			glm::mat4 modeloldCPU22 = modelLabViejo;
			modeloldCPU22 = glm::translate(modeloldCPU22, glm::vec3(49.108f, 3.487f, -29.801f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU22));
			oldCPU.Draw(lightingShader);
			// ------------ monitores ------------
			// monitor 18
			glm::mat4 modeloldMonitor18 = modelLabViejo;
			modeloldMonitor18 = glm::translate(modeloldMonitor18, glm::vec3(10.916f, 14.434f, -30.734f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor18));
			monitorOld.Draw(lightingShader);
			// monitor 19
			glm::mat4 modeloldMonitor19 = modelLabViejo;
			modeloldMonitor19 = glm::translate(modeloldMonitor19, glm::vec3(19.539f, 14.434f, -30.734f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor19));
			monitorOld.Draw(lightingShader);
			// monitor 20
			glm::mat4 modeloldMonitor20 = modelLabViejo;
			modeloldMonitor20 = glm::translate(modeloldMonitor20, glm::vec3(28.307f, 14.434f, -30.734f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor20));
			monitorOld.Draw(lightingShader);
			// monitor 21
			glm::mat4 modeloldMonitor21 = modelLabViejo;
			modeloldMonitor21 = glm::translate(modeloldMonitor21, glm::vec3(36.955f, 14.434f, -30.734f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor21));
			monitorOld.Draw(lightingShader);
			// monitor 22
			glm::mat4 modeloldMonitor22 = modelLabViejo;
			modeloldMonitor22 = glm::translate(modeloldMonitor22, glm::vec3(45.651f, 14.434f, -31.018f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor22));
			monitorOld.Draw(lightingShader);
			// -------- teclados -----------------
			// teclado 18
			glm::mat4 modeloldTeclado18 = modelLabViejo;
			modeloldTeclado18 = glm::translate(modeloldTeclado18, glm::vec3(10.874f, 12.299f, -27.775f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado18));
			tecladoOld.Draw(lightingShader);
			// teclado 19
			glm::mat4 modeloldTeclado19 = modelLabViejo;
			modeloldTeclado19 = glm::translate(modeloldTeclado19, glm::vec3(19.207f, 12.299f, -27.775f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado19));
			tecladoOld.Draw(lightingShader);
			// teclado 20
			glm::mat4 modeloldTeclado20 = modelLabViejo;
			modeloldTeclado20 = glm::translate(modeloldTeclado20, glm::vec3(27.908f, 12.299f, -27.775f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado20));
			tecladoOld.Draw(lightingShader);
			// teclado 21
			glm::mat4 modeloldTeclado21 = modelLabViejo;
			modeloldTeclado21 = glm::translate(modeloldTeclado21, glm::vec3(36.87f, 12.299f, -27.775f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado21));
			tecladoOld.Draw(lightingShader);
			// teclado 22
			glm::mat4 modeloldTeclado22 = modelLabViejo;
			modeloldTeclado22 = glm::translate(modeloldTeclado22, glm::vec3(45.076f, 12.299f, -28.491f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado22));
			tecladoOld.Draw(lightingShader);
			// ---------- mouses -------------------
			// mouse 18
			glm::mat4 modeloldMouse18 = modelLabViejo;
			modeloldMouse18 = glm::translate(modeloldMouse18, glm::vec3(12.026f, 12.405f, -29.305f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse18));
			mouseOld.Draw(lightingShader);
			// mouse 19
			glm::mat4 modeloldMouse19 = modelLabViejo;
			modeloldMouse19 = glm::translate(modeloldMouse19, glm::vec3(20.356f, 12.405f, -29.305f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse19));
			mouseOld.Draw(lightingShader);
			// mouse 20
			glm::mat4 modeloldMouse20 = modelLabViejo;
			modeloldMouse20 = glm::translate(modeloldMouse20, glm::vec3(29.09f, 12.405f, -29.619f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse20));
			mouseOld.Draw(lightingShader);
			// mouse 21
			glm::mat4 modeloldMouse21 = modelLabViejo;
			modeloldMouse21 = glm::translate(modeloldMouse21, glm::vec3(38.015f, 12.405f, -29.619f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse21));
			mouseOld.Draw(lightingShader);
			// mouse 22
			glm::mat4 modeloldMouse22 = modelLabViejo;
			modeloldMouse22 = glm::translate(modeloldMouse22, glm::vec3(46.045f, 12.405f, -29.305f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse22));
			mouseOld.Draw(lightingShader);
			// -------------- sillas ------------------------------
			// silla 18
			glm::mat4 modeloldSilla18 = modelLabViejo;
			modeloldSilla18 = glm::translate(modeloldSilla18, glm::vec3(11.455, 6.377f, -21.457f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla18));
			sillaOld.Draw(lightingShader);
			// silla 19
			glm::mat4 modeloldSilla19 = modelLabViejo;
			modeloldSilla19 = glm::translate(modeloldSilla19, glm::vec3(19.541f, 6.377f, -21.457f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla19));
			sillaOld.Draw(lightingShader);
			// silla 20
			glm::mat4 modeloldSilla20 = modelLabViejo;
			modeloldSilla20 = glm::translate(modeloldSilla20, glm::vec3(27.603f, 6.377f, -21.457f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla20));
			sillaOld.Draw(lightingShader);
			// silla 21
			glm::mat4 modeloldSilla21 = modelLabViejo;
			modeloldSilla21 = glm::translate(modeloldSilla21, glm::vec3(36.099f, 6.377f, -21.457f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla21));
			sillaOld.Draw(lightingShader);
			// silla 22
			glm::mat4 modeloldSilla22 = modelLabViejo;
			modeloldSilla22 = glm::translate(modeloldSilla22, glm::vec3(44.156f, 6.377f, -21.457f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla22));
			sillaOld.Draw(lightingShader);


			// ---------------------------------------------------------------------------------

			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// ------------- Modelado fila 2 columna 2 ----------------------------------------------

			// Escritorio 12
			glm::mat4 modelOldMesa12 = modelLabViejo;
			modelOldMesa12 = glm::translate(modelOldMesa12, glm::vec3(18.113f, 6.758f, -2.779f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa12));
			mesaOld.Draw(lightingShader);
			// Escritorio 13
			glm::mat4 modelOldMesa13 = modelLabViejo;
			modelOldMesa13 = glm::translate(modelOldMesa13, glm::vec3(38.431f, 6.758f, -2.779f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa13));
			mesaOld.Draw(lightingShader);
			// ----------- CPUs --------------------
			// CPU 23
			glm::mat4 modeloldCPU23 = modelLabViejo;
			modeloldCPU23 = glm::translate(modeloldCPU23, glm::vec3(15.345f, 14.443f, -3.545f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU23));
			oldCPU.Draw(lightingShader);
			// CPU 24
			glm::mat4 modeloldCPU24 = modelLabViejo;
			modeloldCPU24 = glm::translate(modeloldCPU24, glm::vec3(24.0f, 14.443f, -3.545f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU24));
			oldCPU.Draw(lightingShader);
			// CPU 25
			glm::mat4 modeloldCPU25 = modelLabViejo;
			modeloldCPU25 = glm::translate(modeloldCPU25, glm::vec3(32.721f, 14.443f, -3.545f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU25));
			oldCPU.Draw(lightingShader);
			// CPU 26
			glm::mat4 modeloldCPU26 = modelLabViejo;
			modeloldCPU26 = glm::translate(modeloldCPU26, glm::vec3(41.355f, 14.443f, -3.545f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU26));
			oldCPU.Draw(lightingShader);
			// CPU 27
			glm::mat4 modeloldCPU27 = modelLabViejo;
			modeloldCPU27 = glm::translate(modeloldCPU27, glm::vec3(49.108f, 3.487f, -3.545f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU27));
			oldCPU.Draw(lightingShader);
			// ------------ monitores ------------
			// monitor 23
			glm::mat4 modeloldMonitor23 = modelLabViejo;
			modeloldMonitor23 = glm::translate(modeloldMonitor23, glm::vec3(10.916f, 14.434f, -4.479f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor23));
			monitorOld.Draw(lightingShader);
			// monitor 24
			glm::mat4 modeloldMonitor24 = modelLabViejo;
			modeloldMonitor24 = glm::translate(modeloldMonitor24, glm::vec3(19.539f, 14.434f, -4.479f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor24));
			monitorOld.Draw(lightingShader);
			// monitor 25
			glm::mat4 modeloldMonitor25 = modelLabViejo;
			modeloldMonitor25 = glm::translate(modeloldMonitor25, glm::vec3(28.307f, 14.434f, -4.479f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor25));
			monitorOld.Draw(lightingShader);
			// monitor 26
			glm::mat4 modeloldMonitor26 = modelLabViejo;
			modeloldMonitor26 = glm::translate(modeloldMonitor26, glm::vec3(36.955f, 14.434f, -4.479f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor26));
			monitorOld.Draw(lightingShader);
			// monitor 27
			glm::mat4 modeloldMonitor27 = modelLabViejo;
			modeloldMonitor27 = glm::translate(modeloldMonitor27, glm::vec3(45.651f, 14.434f, -4.763f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor27));
			monitorOld.Draw(lightingShader);
			// -------- teclados -----------------
			// teclado 23
			glm::mat4 modeloldTeclado23 = modelLabViejo;
			modeloldTeclado23 = glm::translate(modeloldTeclado23, glm::vec3(10.874f, 12.299f, -1.519f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado23));
			tecladoOld.Draw(lightingShader);
			// teclado 24
			glm::mat4 modeloldTeclado24 = modelLabViejo;
			modeloldTeclado24 = glm::translate(modeloldTeclado24, glm::vec3(19.207f, 12.299f, -1.519f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado24));
			tecladoOld.Draw(lightingShader);
			// teclado 25
			glm::mat4 modeloldTeclado25 = modelLabViejo;
			modeloldTeclado25 = glm::translate(modeloldTeclado25, glm::vec3(27.908f, 12.299f, -1.519f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado25));
			tecladoOld.Draw(lightingShader);
			// teclado 26
			glm::mat4 modeloldTeclado26 = modelLabViejo;
			modeloldTeclado26 = glm::translate(modeloldTeclado26, glm::vec3(36.87f, 12.299f, -1.519f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado26));
			tecladoOld.Draw(lightingShader);
			// teclado 27
			glm::mat4 modeloldTeclado27 = modelLabViejo;
			modeloldTeclado27 = glm::translate(modeloldTeclado27, glm::vec3(45.076f, 12.299f, -2.226f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado27));
			tecladoOld.Draw(lightingShader);
			// ---------- mouses -------------------
			// mouse 23
			glm::mat4 modeloldMouse23 = modelLabViejo;
			modeloldMouse23 = glm::translate(modeloldMouse23, glm::vec3(12.026f, 12.405f, -3.05f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse23));
			mouseOld.Draw(lightingShader);
			// mouse 24
			glm::mat4 modeloldMouse24 = modelLabViejo;
			modeloldMouse24 = glm::translate(modeloldMouse24, glm::vec3(20.356f, 12.405f, -3.05f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse24));
			mouseOld.Draw(lightingShader);
			// mouse 25
			glm::mat4 modeloldMouse25 = modelLabViejo;
			modeloldMouse25 = glm::translate(modeloldMouse25, glm::vec3(29.09f, 12.405f, -3.05f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse25));
			mouseOld.Draw(lightingShader);
			// mouse 26
			glm::mat4 modeloldMouse26 = modelLabViejo;
			modeloldMouse26 = glm::translate(modeloldMouse26, glm::vec3(38.015f, 12.405f, -3.05f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse26));
			mouseOld.Draw(lightingShader);
			// mouse 27
			glm::mat4 modeloldMouse27 = modelLabViejo;
			modeloldMouse27 = glm::translate(modeloldMouse27, glm::vec3(46.045f, 12.405f, -3.05f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse27));
			mouseOld.Draw(lightingShader);
			// -------------- sillas ------------------------------
			// silla 23
			glm::mat4 modeloldSilla23 = modelLabViejo;
			modeloldSilla23 = glm::translate(modeloldSilla23, glm::vec3(11.455, 6.377f, 5.179f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla23));
			sillaOld.Draw(lightingShader);
			// silla 24
			glm::mat4 modeloldSilla24 = modelLabViejo;
			modeloldSilla24 = glm::translate(modeloldSilla24, glm::vec3(19.541f, 6.377f, 5.179f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla24));
			sillaOld.Draw(lightingShader);
			// silla 25
			glm::mat4 modeloldSilla25 = modelLabViejo;
			modeloldSilla25 = glm::translate(modeloldSilla25, glm::vec3(27.603f, 6.377f, 5.179f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla25));
			sillaOld.Draw(lightingShader);
			// silla 26
			glm::mat4 modeloldSilla26 = modelLabViejo;
			modeloldSilla26 = glm::translate(modeloldSilla26, glm::vec3(36.099f, 6.377f, 5.179f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla26));
			sillaOld.Draw(lightingShader);
			// silla 27
			glm::mat4 modeloldSilla27 = modelLabViejo;
			modeloldSilla27 = glm::translate(modeloldSilla27, glm::vec3(44.156f, 6.377f, 5.179f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla27));
			sillaOld.Draw(lightingShader);
			// ---------------------------------------------------------------------------------

			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// ------------- Modelado fila 3 columna 2 ----------------------------------------------
			// Escritorio 14
			glm::mat4 modelOldMesa14 = modelLabViejo;
			modelOldMesa14 = glm::translate(modelOldMesa14, glm::vec3(18.113f, 6.758f, 22.489f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa14));
			mesaOld.Draw(lightingShader);
			// Escritorio 15
			glm::mat4 modelOldMesa15 = modelLabViejo;
			modelOldMesa15 = glm::translate(modelOldMesa15, glm::vec3(38.431f, 6.758f, 22.489f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa15));
			mesaOld.Draw(lightingShader);
			// ----------- CPUs --------------------
			// CPU 28
			glm::mat4 modeloldCPU28 = modelLabViejo;
			modeloldCPU28 = glm::translate(modeloldCPU28, glm::vec3(15.345f, 14.443f, 21.519f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU28));
			oldCPU.Draw(lightingShader);
			// CPU 29
			glm::mat4 modeloldCPU29 = modelLabViejo;
			modeloldCPU29 = glm::translate(modeloldCPU29, glm::vec3(24.0f, 14.443f, 21.519f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU29));
			oldCPU.Draw(lightingShader);
			// CPU 30
			glm::mat4 modeloldCPU30 = modelLabViejo;
			modeloldCPU30 = glm::translate(modeloldCPU30, glm::vec3(32.721f, 14.443f, 21.519f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU30));
			oldCPU.Draw(lightingShader);
			// CPU 31
			glm::mat4 modeloldCPU31 = modelLabViejo;
			modeloldCPU31 = glm::translate(modeloldCPU31, glm::vec3(41.355f, 14.443f, 21.519f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU31));
			oldCPU.Draw(lightingShader);
			// CPU 32
			glm::mat4 modeloldCPU32 = modelLabViejo;
			modeloldCPU32 = glm::translate(modeloldCPU32, glm::vec3(49.108f, 3.487f, 21.519f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU32));
			oldCPU.Draw(lightingShader);
			// ------------ monitores ------------
			// monitor 28
			glm::mat4 modeloldMonitor28 = modelLabViejo;
			modeloldMonitor28 = glm::translate(modeloldMonitor28, glm::vec3(10.916f, 14.434f, 20.585f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor28));
			monitorOld.Draw(lightingShader);
			// monitor 29
			glm::mat4 modeloldMonitor29 = modelLabViejo;
			modeloldMonitor29 = glm::translate(modeloldMonitor29, glm::vec3(19.539f, 14.434f, 20.585f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor29));
			monitorOld.Draw(lightingShader);
			// monitor 30
			glm::mat4 modeloldMonitor30 = modelLabViejo;
			modeloldMonitor30 = glm::translate(modeloldMonitor30, glm::vec3(28.307f, 14.434f, 20.585f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor30));
			monitorOld.Draw(lightingShader);
			// monitor 31
			glm::mat4 modeloldMonitor31 = modelLabViejo;
			modeloldMonitor31 = glm::translate(modeloldMonitor31, glm::vec3(36.955f, 14.434f, 20.585f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor31));
			monitorOld.Draw(lightingShader);
			// monitor 32
			glm::mat4 modeloldMonitor32 = modelLabViejo;
			modeloldMonitor32 = glm::translate(modeloldMonitor32, glm::vec3(45.651f, 14.434f, 20.301f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor32));
			monitorOld.Draw(lightingShader);
			// -------- teclados -----------------
			// teclado 28
			glm::mat4 modeloldTeclado28 = modelLabViejo;
			modeloldTeclado28 = glm::translate(modeloldTeclado28, glm::vec3(10.874f, 12.299f, 23.545f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado28));
			tecladoOld.Draw(lightingShader);
			// teclado 24
			glm::mat4 modeloldTeclado29 = modelLabViejo;
			modeloldTeclado29 = glm::translate(modeloldTeclado29, glm::vec3(19.207f, 12.299f, 23.545f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado29));
			tecladoOld.Draw(lightingShader);
			// teclado 30
			glm::mat4 modeloldTeclado30 = modelLabViejo;
			modeloldTeclado30 = glm::translate(modeloldTeclado30, glm::vec3(27.908f, 12.299f, 23.545f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado30));
			tecladoOld.Draw(lightingShader);
			// teclado 31
			glm::mat4 modeloldTeclado31 = modelLabViejo;
			modeloldTeclado31 = glm::translate(modeloldTeclado31, glm::vec3(36.87f, 12.299f, 23.545f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado31));
			tecladoOld.Draw(lightingShader);
			// teclado 32
			glm::mat4 modeloldTeclado32 = modelLabViejo;
			modeloldTeclado32 = glm::translate(modeloldTeclado32, glm::vec3(45.076f, 12.299f, 22.838f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado32));
			tecladoOld.Draw(lightingShader);
			// ---------- mouses -------------------
			// mouse 28
			glm::mat4 modeloldMouse28 = modelLabViejo;
			modeloldMouse28 = glm::translate(modeloldMouse28, glm::vec3(12.026f, 12.405f, 22.014f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse28));
			mouseOld.Draw(lightingShader);
			// mouse 29
			glm::mat4 modeloldMouse29 = modelLabViejo;
			modeloldMouse29 = glm::translate(modeloldMouse29, glm::vec3(20.356f, 12.405f, 21.7f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse29));
			mouseOld.Draw(lightingShader);
			// mouse 30
			glm::mat4 modeloldMouse30 = modelLabViejo;
			modeloldMouse30 = glm::translate(modeloldMouse30, glm::vec3(29.09f, 12.405f, 22.014f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse30));
			mouseOld.Draw(lightingShader);
			// mouse 31
			glm::mat4 modeloldMouse31 = modelLabViejo;
			modeloldMouse31 = glm::translate(modeloldMouse31, glm::vec3(38.015f, 12.405f, 21.7f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse31));
			mouseOld.Draw(lightingShader);
			// mouse 32
			glm::mat4 modeloldMouse32 = modelLabViejo;
			modeloldMouse32 = glm::translate(modeloldMouse32, glm::vec3(46.045f, 12.405f, 22.014f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse32));
			mouseOld.Draw(lightingShader);
			// -------------- sillas ------------------------------
			// silla 28
			glm::mat4 modeloldSilla28 = modelLabViejo;
			modeloldSilla28 = glm::translate(modeloldSilla28, glm::vec3(11.455, 6.377f, 30.964f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla28));
			sillaOld.Draw(lightingShader);
			// silla 29
			glm::mat4 modeloldSilla29 = modelLabViejo;
			modeloldSilla29 = glm::translate(modeloldSilla29, glm::vec3(19.541f, 6.377f, 30.964f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla29));
			sillaOld.Draw(lightingShader);
			// silla 30
			glm::mat4 modeloldSilla30 = modelLabViejo;
			modeloldSilla30 = glm::translate(modeloldSilla30, glm::vec3(27.603f, 6.377f, 30.964f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla30));
			sillaOld.Draw(lightingShader);
			// silla 31
			glm::mat4 modeloldSilla31 = modelLabViejo;
			modeloldSilla31 = glm::translate(modeloldSilla31, glm::vec3(36.099f, 6.377f, 30.964f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla31));
			sillaOld.Draw(lightingShader);
			// silla 32
			glm::mat4 modeloldSilla32 = modelLabViejo;
			modeloldSilla32 = glm::translate(modeloldSilla32, glm::vec3(44.156f, 6.377f, 30.964f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla32));
			sillaOld.Draw(lightingShader);
			// ---------------------------------------------------------------------------------

			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// ------------- Modelado fila 3 columna 2 ----------------------------------------------
			// Escritorio 16
			glm::mat4 modelOldMesa16 = modelLabViejo;
			modelOldMesa16 = glm::translate(modelOldMesa16, glm::vec3(18.113f, 6.758f, 48.24f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa16));
			mesaOld.Draw(lightingShader);
			// Escritorio 17
			glm::mat4 modelOldMesa17 = modelLabViejo;
			modelOldMesa17 = glm::translate(modelOldMesa17, glm::vec3(38.431f, 6.758f, 48.24f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOldMesa17));
			mesaOld.Draw(lightingShader);
			// ----------- CPUs --------------------
			// CPU 33
			glm::mat4 modeloldCPU33 = modelLabViejo;
			modeloldCPU33 = glm::translate(modeloldCPU33, glm::vec3(15.345f, 14.443f, 47.706f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU33));
			oldCPU.Draw(lightingShader);
			// CPU 34
			glm::mat4 modeloldCPU34 = modelLabViejo;
			modeloldCPU34 = glm::translate(modeloldCPU34, glm::vec3(24.0f, 14.443f, 47.706f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU34));
			oldCPU.Draw(lightingShader);
			// CPU 35
			glm::mat4 modeloldCPU35 = modelLabViejo;
			modeloldCPU35 = glm::translate(modeloldCPU35, glm::vec3(32.721f, 14.443f, 47.706f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU35));
			oldCPU.Draw(lightingShader);
			// CPU 36
			glm::mat4 modeloldCPU36 = modelLabViejo;
			modeloldCPU36 = glm::translate(modeloldCPU36, glm::vec3(41.355f, 14.443f, 47.006f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU36));
			oldCPU.Draw(lightingShader);
			// CPU 37
			glm::mat4 modeloldCPU37 = modelLabViejo;
			modeloldCPU37 = glm::translate(modeloldCPU37, glm::vec3(49.108f, 3.487f, 47.706f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldCPU37));
			oldCPU.Draw(lightingShader);
			// ------------ monitores ------------
			// monitor 33
			glm::mat4 modeloldMonitor33 = modelLabViejo;
			modeloldMonitor33 = glm::translate(modeloldMonitor33, glm::vec3(10.916f, 14.434f, 46.773f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor33));
			monitorOld.Draw(lightingShader);
			// monitor 34
			glm::mat4 modeloldMonitor34 = modelLabViejo;
			modeloldMonitor34 = glm::translate(modeloldMonitor34, glm::vec3(19.539f, 14.434f, 46.773f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor34));
			monitorOld.Draw(lightingShader);
			// monitor 35
			glm::mat4 modeloldMonitor35 = modelLabViejo;
			modeloldMonitor35 = glm::translate(modeloldMonitor35, glm::vec3(28.307f, 14.434f, 46.773f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor35));
			monitorOld.Draw(lightingShader);
			// monitor 36
			glm::mat4 modeloldMonitor36 = modelLabViejo;
			modeloldMonitor36 = glm::translate(modeloldMonitor36, glm::vec3(36.955f, 14.434f, 46.773f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor36));
			monitorOld.Draw(lightingShader);
			// monitor 37
			glm::mat4 modeloldMonitor37 = modelLabViejo;
			modeloldMonitor37 = glm::translate(modeloldMonitor37, glm::vec3(45.651f, 14.434f, 46.773f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMonitor37));
			monitorOld.Draw(lightingShader);
			// -------- teclados -----------------
			// teclado 33
			glm::mat4 modeloldTeclado33 = modelLabViejo;
			modeloldTeclado33 = glm::translate(modeloldTeclado33, glm::vec3(10.874f, 12.299f, 49.732f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado33));
			tecladoOld.Draw(lightingShader);
			// teclado 34
			glm::mat4 modeloldTeclado34 = modelLabViejo;
			modeloldTeclado34 = glm::translate(modeloldTeclado34, glm::vec3(19.207f, 12.299f, 49.732f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado34));
			tecladoOld.Draw(lightingShader);
			// teclado 35
			glm::mat4 modeloldTeclado35 = modelLabViejo;
			modeloldTeclado35 = glm::translate(modeloldTeclado35, glm::vec3(27.908f, 12.299f, 49.732f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado35));
			tecladoOld.Draw(lightingShader);
			// teclado 36
			glm::mat4 modeloldTeclado36 = modelLabViejo;
			modeloldTeclado36 = glm::translate(modeloldTeclado36, glm::vec3(36.87f, 12.299f, 49.732f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado36));
			tecladoOld.Draw(lightingShader);
			// teclado 37
			glm::mat4 modeloldTeclado37 = modelLabViejo;
			modeloldTeclado37 = glm::translate(modeloldTeclado37, glm::vec3(45.076f, 12.299f, 49.025f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldTeclado37));
			tecladoOld.Draw(lightingShader);
			// ---------- mouses -------------------
			// mouse 33
			glm::mat4 modeloldMouse33 = modelLabViejo;
			modeloldMouse33 = glm::translate(modeloldMouse33, glm::vec3(12.026f, 12.405f, 48.202f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse33));
			mouseOld.Draw(lightingShader);
			// mouse 34
			glm::mat4 modeloldMouse34 = modelLabViejo;
			modeloldMouse34 = glm::translate(modeloldMouse34, glm::vec3(20.356f, 12.405f, 47.888f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse34));
			mouseOld.Draw(lightingShader);
			// mouse 35
			glm::mat4 modeloldMouse35 = modelLabViejo;
			modeloldMouse35 = glm::translate(modeloldMouse35, glm::vec3(29.09f, 12.405f, 47.888f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse35));
			mouseOld.Draw(lightingShader);
			// mouse 36
			glm::mat4 modeloldMouse36 = modelLabViejo;
			modeloldMouse36 = glm::translate(modeloldMouse36, glm::vec3(38.015f, 12.405f, 47.888f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse36));
			mouseOld.Draw(lightingShader);
			// mouse 37
			glm::mat4 modeloldMouse37 = modelLabViejo;
			modeloldMouse37 = glm::translate(modeloldMouse37, glm::vec3(46.045f, 12.405f, 48.202f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldMouse37));
			mouseOld.Draw(lightingShader);
			// -------------- sillas ------------------------------
			// silla 33
			glm::mat4 modeloldSilla33 = modelLabViejo;
			modeloldSilla33 = glm::translate(modeloldSilla33, glm::vec3(11.455, 6.377f, 56.301f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla33));
			sillaOld.Draw(lightingShader);
			// silla 34
			glm::mat4 modeloldSilla34 = modelLabViejo;
			modeloldSilla34 = glm::translate(modeloldSilla34, glm::vec3(19.541f, 6.377f, 56.301f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla34));
			sillaOld.Draw(lightingShader);
			// silla 35
			glm::mat4 modeloldSilla35 = modelLabViejo;
			modeloldSilla35 = glm::translate(modeloldSilla35, glm::vec3(27.603f, 6.377f, 56.301f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla35));
			sillaOld.Draw(lightingShader);
			// silla 36
			glm::mat4 modeloldSilla36 = modelLabViejo;
			modeloldSilla36 = glm::translate(modeloldSilla36, glm::vec3(36.099f, 6.377f, 56.301f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla36));
			sillaOld.Draw(lightingShader);
			// silla 37
			glm::mat4 modeloldSilla37 = modelLabViejo;
			modeloldSilla37 = glm::translate(modeloldSilla37, glm::vec3(44.156f, 6.377f, 56.301f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeloldSilla37));
			sillaOld.Draw(lightingShader);



			// -----------------------------------------------------------------
			// ventanas con transparencia
			glm::mat4 modelVentanasOld = modelLabViejo;
			glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelVentanasOld));
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			modelCuartoOld = glm::scale(modelCuartoOld, glm::vec3(1.0f, 1.0f, 1.0f));
			modelCuartoOld = glm::translate(modelCuartoOld, glm::vec3(0.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelVentanasOld));
			ventanasOld.Draw(lightingShader);
			glDisable(GL_BLEND);
			glBindVertexArray(0);


			// Puerta del salón
			glm::mat4 modelPuertaOld = modelLabViejo;
			glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPuertaOld));
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			modelPuertaOld = glm::scale(modelCuartoOld, glm::vec3(1.0f, 1.0f, 1.0f));
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

	// Delete all the objects as they're been cleared as well
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);//Skybox
	glDeleteBuffers(1, &EBO);//Skybox
	glDeleteVertexArrays(1, &skyboxVAO);//Skybox
	glDeleteBuffers(1, &skyboxVAO);//Skybox


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		std::cout << "W PRESSED - deltaTime: " << deltaTime << std::endl;
		camera.ProcessKeyboard(FORWARD, deltaTime);
		std::cout << "Camera moved FORWARD\n";
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

void resetHumanoidAnimation(HumanoidAnimation& animation) {
	animation.currentKeyFrame = -1;
	animation.step = -1;
	animation.maxStep = -1;
	return;
}
void resetCaminarAnimation() {
	caminarAnimationKeyIndex = -1;
	caminarAnimationMaxStep = -1;
	caminarAnimationCurrentStep = -1;
	playCaminar = false;
	caminarTime = 0.0f;
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

	//Nueva tecla para intercambiar entre escenarios
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {

		if (transicionLab == -1) {
			transicionLab = 0;
		}
		else if (transicionLab == 3) {
			transicionLab = -1;
			mostrarEscenarioNuevo = false;
		}
		else {
			rotarYEscenario = 0.0f;
			rotarYLabViejo = 0.0f;
			tornadoScale = 0.0f;
			tornadoMove = 0.0f;
			labViejoMove = 0.0f;
			labNuevoMove = 0.0f;
			labNuevoBounce = 0.0f;
			mostrarEscenarioNuevo = true;
			transicionLab = 3;
			renderAlumno1 = false;
		}



	}

	if (key == GLFW_KEY_0 && action == GLFW_PRESS && mostrarEscenarioNuevo)
	{
		renderAlumno1 = true;
		playAnimationIndex = 0;
		resetCaminarAnimation(); // <--- IMPORTANTE


	}

	if (key == GLFW_KEY_1 && action == GLFW_PRESS && mostrarEscenarioNuevo)
	{
		playAnimationIndex = 1;
		resetHumanoidAnimation(animations[1]); // IMPORTANTE



	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS && mostrarEscenarioNuevo)
	{
		playAnimationIndex = 2;
		resetHumanoidAnimation(animations[2]); // IMPORTANTE



	}
	std::cout << "Tecla presionada: " << key << ", action: " << action << std::endl;




}



bool progressHumanoidAnimation(int animation, int model) {

	if (animations[animation].currentKeyFrame == -1) {
		HumanoidModelInfo firstFrame = animations[animation].keyFrames[0].modelInfo;

		personas[model].torso.rotation.x = firstFrame.torso.rotation.x;
		personas[model].torso.rotation.z = firstFrame.torso.rotation.z;
		personas[model].torso.rotation.y = firstFrame.torso.rotation.y;
		personas[model].torso.position.x = firstFrame.torso.position.x;
		personas[model].torso.position.z = firstFrame.torso.position.z;
		personas[model].torso.position.y = firstFrame.torso.position.y;
		personas[model].head.rotation.x = firstFrame.head.rotation.x;
		personas[model].head.rotation.z = firstFrame.head.rotation.z;
		personas[model].head.rotation.y = firstFrame.head.rotation.y;
		personas[model].head.position.x = firstFrame.head.position.x;
		personas[model].head.position.z = firstFrame.head.position.z;
		personas[model].head.position.y = firstFrame.head.position.y;
		personas[model].rightArm.rotation.x = firstFrame.rightArm.rotation.x;
		personas[model].rightArm.rotation.z = firstFrame.rightArm.rotation.z;
		personas[model].rightArm.rotation.y = firstFrame.rightArm.rotation.y;
		personas[model].rightArm.position.x = firstFrame.rightArm.position.x;
		personas[model].rightArm.position.z = firstFrame.rightArm.position.z;
		personas[model].rightArm.position.y = firstFrame.rightArm.position.y;
		personas[model].subRightArm.rotation.x = firstFrame.subRightArm.rotation.x;
		personas[model].subRightArm.rotation.z = firstFrame.subRightArm.rotation.z;
		personas[model].subRightArm.rotation.y = firstFrame.subRightArm.rotation.y;
		personas[model].subRightArm.position.x = firstFrame.subRightArm.position.x;
		personas[model].subRightArm.position.z = firstFrame.subRightArm.position.z;
		personas[model].subRightArm.position.y = firstFrame.subRightArm.position.y;
		personas[model].leftArm.rotation.x = firstFrame.leftArm.rotation.x;
		personas[model].leftArm.rotation.z = firstFrame.leftArm.rotation.z;
		personas[model].leftArm.rotation.y = firstFrame.leftArm.rotation.y;
		personas[model].leftArm.position.x = firstFrame.leftArm.position.x;
		personas[model].leftArm.position.z = firstFrame.leftArm.position.z;
		personas[model].leftArm.position.y = firstFrame.leftArm.position.y;
		personas[model].subLeftArm.rotation.x = firstFrame.subLeftArm.rotation.x;
		personas[model].subLeftArm.rotation.z = firstFrame.subLeftArm.rotation.z;
		personas[model].subLeftArm.rotation.y = firstFrame.subLeftArm.rotation.y;
		personas[model].subLeftArm.position.x = firstFrame.subLeftArm.position.x;
		personas[model].subLeftArm.position.z = firstFrame.subLeftArm.position.z;
		personas[model].subLeftArm.position.y = firstFrame.subLeftArm.position.y;
		personas[model].rightLeg.rotation.x = firstFrame.rightLeg.rotation.x;
		personas[model].rightLeg.rotation.z = firstFrame.rightLeg.rotation.z;
		personas[model].rightLeg.rotation.y = firstFrame.rightLeg.rotation.y;
		personas[model].rightLeg.position.x = firstFrame.rightLeg.position.x;
		personas[model].rightLeg.position.z = firstFrame.rightLeg.position.z;
		personas[model].rightLeg.position.y = firstFrame.rightLeg.position.y;
		personas[model].subRightLeg.rotation.x = firstFrame.subRightLeg.rotation.x;
		personas[model].subRightLeg.rotation.z = firstFrame.subRightLeg.rotation.z;
		personas[model].subRightLeg.rotation.y = firstFrame.subRightLeg.rotation.y;
		personas[model].subRightLeg.position.x = firstFrame.subRightLeg.position.x;
		personas[model].subRightLeg.position.z = firstFrame.subRightLeg.position.z;
		personas[model].subRightLeg.position.y = firstFrame.subRightLeg.position.y;
		personas[model].leftLeg.rotation.x = firstFrame.leftLeg.rotation.x;
		personas[model].leftLeg.rotation.z = firstFrame.leftLeg.rotation.z;
		personas[model].leftLeg.rotation.y = firstFrame.leftLeg.rotation.y;
		personas[model].leftLeg.position.x = firstFrame.leftLeg.position.x;
		personas[model].leftLeg.position.z = firstFrame.leftLeg.position.z;
		personas[model].leftLeg.position.y = firstFrame.leftLeg.position.y;
		personas[model].subLeftLeg.rotation.x = firstFrame.subLeftLeg.rotation.x;
		personas[model].subLeftLeg.rotation.z = firstFrame.subLeftLeg.rotation.z;
		personas[model].subLeftLeg.rotation.y = firstFrame.subLeftLeg.rotation.y;
		personas[model].subLeftLeg.position.x = firstFrame.subLeftLeg.position.x;
		personas[model].subLeftLeg.position.z = firstFrame.subLeftLeg.position.z;
		personas[model].subLeftLeg.position.y = firstFrame.subLeftLeg.position.y;

		animations[animation].currentKeyFrame = 0;
	}
	else if (animations[animation].currentKeyFrame < animations[animation].maxKeyFrame) {
		if (animations[animation].maxStep == -1) {

			animations[animation].step = 0;




			animations[animation].maxStep = animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].stepsToFrame;

			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.torso.position.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.torso.position.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.torso.position.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.torso.position.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.torso.position.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.torso.position.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.torso.position.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.torso.position.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.torso.position.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.torso.rotation.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.torso.rotation.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.torso.rotation.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.torso.rotation.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.torso.rotation.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.torso.rotation.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.torso.rotation.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.torso.rotation.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.torso.rotation.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.head.position.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.head.position.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.head.position.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.head.position.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.head.position.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.head.position.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.head.position.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.head.position.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.head.position.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.head.rotation.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.head.rotation.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.head.rotation.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.head.rotation.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.head.rotation.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.head.rotation.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.head.rotation.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.head.rotation.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.head.rotation.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightArm.position.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.rightArm.position.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.rightArm.position.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightArm.position.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.rightArm.position.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.rightArm.position.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightArm.position.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.rightArm.position.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.rightArm.position.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightArm.rotation.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.rightArm.rotation.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.rightArm.rotation.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightArm.rotation.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.rightArm.rotation.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.rightArm.rotation.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightArm.rotation.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.rightArm.rotation.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.rightArm.rotation.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightArm.position.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subRightArm.position.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subRightArm.position.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightArm.position.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subRightArm.position.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subRightArm.position.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightArm.position.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subRightArm.position.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subRightArm.position.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightArm.rotation.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subRightArm.rotation.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subRightArm.rotation.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightArm.rotation.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subRightArm.rotation.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subRightArm.rotation.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightArm.rotation.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subRightArm.rotation.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subRightArm.rotation.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftArm.position.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.leftArm.position.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.leftArm.position.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftArm.position.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.leftArm.position.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.leftArm.position.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftArm.position.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.leftArm.position.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.leftArm.position.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftArm.rotation.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.leftArm.rotation.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.leftArm.rotation.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftArm.rotation.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.leftArm.rotation.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.leftArm.rotation.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftArm.rotation.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.leftArm.rotation.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.leftArm.rotation.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftArm.position.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subLeftArm.position.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subLeftArm.position.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftArm.position.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subLeftArm.position.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subLeftArm.position.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftArm.position.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subLeftArm.position.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subLeftArm.position.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftArm.rotation.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subLeftArm.rotation.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subLeftArm.rotation.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftArm.rotation.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subLeftArm.rotation.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subLeftArm.rotation.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftArm.rotation.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subLeftArm.rotation.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subLeftArm.rotation.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightLeg.position.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.rightLeg.position.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.rightLeg.position.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightLeg.position.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.rightLeg.position.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.rightLeg.position.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightLeg.position.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.rightLeg.position.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.rightLeg.position.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightLeg.rotation.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.rightLeg.rotation.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.rightLeg.rotation.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightLeg.rotation.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.rightLeg.rotation.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.rightLeg.rotation.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightLeg.rotation.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.rightLeg.rotation.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.rightLeg.rotation.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightLeg.position.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subRightLeg.position.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subRightLeg.position.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightLeg.position.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subRightLeg.position.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subRightLeg.position.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightLeg.position.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subRightLeg.position.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subRightLeg.position.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightLeg.rotation.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subRightLeg.rotation.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subRightLeg.rotation.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightLeg.rotation.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subRightLeg.rotation.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subRightLeg.rotation.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightLeg.rotation.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subRightLeg.rotation.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subRightLeg.rotation.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftLeg.position.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.leftLeg.position.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.leftLeg.position.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftLeg.position.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.leftLeg.position.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.leftLeg.position.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftLeg.position.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.leftLeg.position.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.leftLeg.position.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftLeg.rotation.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.leftLeg.rotation.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.leftLeg.rotation.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftLeg.rotation.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.leftLeg.rotation.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.leftLeg.rotation.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftLeg.rotation.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.leftLeg.rotation.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.leftLeg.rotation.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftLeg.position.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subLeftLeg.position.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subLeftLeg.position.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftLeg.position.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subLeftLeg.position.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subLeftLeg.position.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftLeg.position.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subLeftLeg.position.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subLeftLeg.position.z, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftLeg.rotation.x = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subLeftLeg.rotation.x, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subLeftLeg.rotation.x, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftLeg.rotation.y = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subLeftLeg.rotation.y, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subLeftLeg.rotation.y, animations[animation].maxStep);
			animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftLeg.rotation.z = interpolation2(animations[animation].keyFrames[animations[animation].currentKeyFrame].modelInfo.subLeftLeg.rotation.z, animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].modelInfo.subLeftLeg.rotation.z, animations[animation].maxStep);

		}
		else {
			if (animations[animation].step < animations[animation].maxStep) {

				personas[model].torso.rotation.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.torso.rotation.x;
				personas[model].torso.rotation.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.torso.rotation.y;
				personas[model].torso.rotation.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.torso.rotation.z;
				personas[model].torso.position.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.torso.position.x;
				personas[model].torso.position.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.torso.position.y;
				personas[model].torso.position.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.torso.position.z;
				personas[model].head.rotation.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.head.rotation.x;
				personas[model].head.rotation.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.head.rotation.y;
				personas[model].head.rotation.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.head.rotation.z;
				personas[model].head.position.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.head.position.x;
				personas[model].head.position.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.head.position.y;
				personas[model].head.position.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.head.position.z;
				personas[model].rightArm.rotation.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightArm.rotation.x;
				personas[model].rightArm.rotation.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightArm.rotation.y;
				personas[model].rightArm.rotation.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightArm.rotation.z;
				personas[model].rightArm.position.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightArm.position.x;
				personas[model].rightArm.position.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightArm.position.y;
				personas[model].rightArm.position.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightArm.position.z;
				personas[model].subRightArm.rotation.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightArm.rotation.x;
				personas[model].subRightArm.rotation.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightArm.rotation.y;
				personas[model].subRightArm.rotation.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightArm.rotation.z;
				personas[model].subRightArm.position.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightArm.position.x;
				personas[model].subRightArm.position.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightArm.position.y;
				personas[model].subRightArm.position.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightArm.position.z;
				personas[model].leftArm.rotation.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftArm.rotation.x;
				personas[model].leftArm.rotation.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftArm.rotation.y;
				personas[model].leftArm.rotation.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftArm.rotation.z;
				personas[model].leftArm.position.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftArm.position.x;
				personas[model].leftArm.position.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftArm.position.y;
				personas[model].leftArm.position.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftArm.position.z;
				personas[model].subLeftArm.rotation.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftArm.rotation.x;
				personas[model].subLeftArm.rotation.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftArm.rotation.y;
				personas[model].subLeftArm.rotation.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftArm.rotation.z;
				personas[model].subLeftArm.position.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftArm.position.x;
				personas[model].subLeftArm.position.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftArm.position.y;
				personas[model].subLeftArm.position.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftArm.position.z;
				personas[model].rightLeg.rotation.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightLeg.rotation.x;
				personas[model].rightLeg.rotation.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightLeg.rotation.y;
				personas[model].rightLeg.rotation.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightLeg.rotation.z;
				personas[model].rightLeg.position.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightLeg.position.x;
				personas[model].rightLeg.position.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightLeg.position.y;
				personas[model].rightLeg.position.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.rightLeg.position.z;
				personas[model].subRightLeg.rotation.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightLeg.rotation.x;
				personas[model].subRightLeg.rotation.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightLeg.rotation.y;
				personas[model].subRightLeg.rotation.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightLeg.rotation.z;
				personas[model].subRightLeg.position.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightLeg.position.x;
				personas[model].subRightLeg.position.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightLeg.position.y;
				personas[model].subRightLeg.position.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subRightLeg.position.z;
				personas[model].leftLeg.rotation.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftLeg.rotation.x;
				personas[model].leftLeg.rotation.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftLeg.rotation.y;
				personas[model].leftLeg.rotation.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftLeg.rotation.z;
				personas[model].leftLeg.position.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftLeg.position.x;
				personas[model].leftLeg.position.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftLeg.position.y;
				personas[model].leftLeg.position.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.leftLeg.position.z;
				personas[model].subLeftLeg.rotation.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftLeg.rotation.x;
				personas[model].subLeftLeg.rotation.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftLeg.rotation.y;
				personas[model].subLeftLeg.rotation.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftLeg.rotation.z;
				personas[model].subLeftLeg.position.x += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftLeg.position.x;
				personas[model].subLeftLeg.position.y += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftLeg.position.y;
				personas[model].subLeftLeg.position.z += animations[animation].keyFrames[animations[animation].currentKeyFrame + 1].inc.subLeftLeg.position.z;




				animations[animation].step++;
			}
			else {

				animations[animation].maxStep = -1;
				animations[animation].step = -1;
				animations[animation].currentKeyFrame++;

				if (animations[animation].currentKeyFrame >= animations[animation].maxKeyFrame) {
					//playAnimationIndex = -1;
					return false;
				}

			}
		}
		return false;
	}
	else {
		resetHumanoidAnimation(animations[animation]);
		playAnimationIndex = -1;
		return true;
	}
}

void Animation() {
	//--------------------Animacion 0
	//printf("El valor es: %f\n", playAnimationIndex);

	int steps = int(floor(500.0 * deltaTime));

	for (int i = 0; i < steps; i++) {
		switch (playAnimationIndex)
		{
		case 0:
			if (caminarAnimationKeyIndex == -1) {
				HumanoidModelInfo firstFrame = caminarAnimationKeyFrames[0].modelInfo;

				personas[0].torso.position.x = firstFrame.torso.position.x;
				personas[0].torso.position.z = firstFrame.torso.position.z;
				personas[0].torso.position.y = firstFrame.torso.position.y;
				personas[0].torso.rotation.y = firstFrame.torso.rotation.y;
				playCaminar = true;
				caminarAnimationKeyIndex = 0;
			}
			else if (caminarAnimationKeyIndex < caminarAnimationMaxKeyFrame) {
				if (caminarAnimationMaxStep == -1) {

					caminarAnimationCurrentStep = 0;

					if (caminarAnimationKeyIndex == 4) {

						playCaminar = false;
						caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.rightArm.rotation.x = personas[0].rightArm.rotation.x;
						caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.leftArm.rotation.x = personas[0].leftArm.rotation.x;
						caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.subRightArm.rotation.x = personas[0].subRightArm.rotation.x;
						caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.subLeftArm.rotation.x = personas[0].subLeftArm.rotation.x;
						caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.rightLeg.rotation.x = personas[0].rightLeg.rotation.x;
						caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.leftLeg.rotation.x = personas[0].leftLeg.rotation.x;
						caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.subRightLeg.rotation.x = personas[0].subRightLeg.rotation.x;
						caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.subLeftLeg.rotation.x = personas[0].subLeftLeg.rotation.x;
						caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.leftArm.rotation.z = personas[0].leftArm.rotation.z;
						caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.rightArm.rotation.z = personas[0].rightArm.rotation.z;
					}


					caminarAnimationMaxStep = caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].stepsToFrame;
					caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.position.x = interpolation2(caminarAnimationKeyFrames[caminarAnimationKeyIndex].modelInfo.torso.position.x, caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].modelInfo.torso.position.x, caminarAnimationMaxStep);
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
						personas[0].torso.position.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.position.x;
						personas[0].torso.position.y += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.position.y;
						personas[0].torso.position.z += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.position.z;
						personas[0].torso.rotation.y += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.rotation.y;

						caminarTime += 15.0 * sqrt(pow(caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.position.x, 2.0) + pow(caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.torso.position.z, 2.0));

						personas[0].rightArm.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.rightArm.rotation.x;
						personas[0].leftArm.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.leftArm.rotation.x;
						personas[0].subRightArm.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.subRightArm.rotation.x;
						personas[0].subLeftArm.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.subLeftArm.rotation.x;
						personas[0].rightLeg.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.rightLeg.rotation.x;
						personas[0].leftLeg.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.leftLeg.rotation.x;

						personas[0].subRightLeg.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.subRightLeg.rotation.x;
						personas[0].subLeftLeg.rotation.x += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.subLeftLeg.rotation.x;

						personas[0].leftArm.rotation.z += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.leftArm.rotation.z;
						personas[0].rightArm.rotation.z += caminarAnimationKeyFrames[caminarAnimationKeyIndex + 1].inc.rightArm.rotation.z;




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
		case 1:
			//printf("El número es: %d\n", playAnimationIndex);
			if (progressHumanoidAnimation(1, 2)) {
				//playAnimationIndex = -1;
			}


			break;

		case 2:
			//printf("El número es: %d\n", playAnimationIndex);
			if (progressHumanoidAnimation(2, 1)) {
				//playAnimationIndex = -1;
			}


			break;


		default:
			break;
		}


	}

	if (playCaminar)
	{

		personas[0].rightArm.rotation.x = 15.0f + sin(glm::radians(caminarTime)) * 30;
		personas[0].leftArm.rotation.x = 15.0f - sin(glm::radians(caminarTime)) * 30;
		personas[0].subRightArm.rotation.x = -15.0f + sin(glm::radians(caminarTime)) * 15;
		personas[0].subLeftArm.rotation.x = -15.0f - sin(glm::radians(caminarTime)) * 15;
		personas[0].rightLeg.rotation.x = -15.0f + sin(glm::radians(caminarTime)) * 15;
		personas[0].leftLeg.rotation.x = -15.0f - sin(glm::radians(caminarTime)) * 15;

		personas[0].subRightLeg.rotation.x = 30.0f + sin(glm::radians(caminarTime) - 0.5) * 30;
		personas[0].subLeftLeg.rotation.x = 30.0f - sin(glm::radians(caminarTime) - 0.5) * 30;

		personas[0].leftArm.rotation.z = -30.0f;
		personas[0].rightArm.rotation.z = 30.0f;
	}
	else
	{
		// Si la animación está desactivada, restablecer las variables
		caminarTime = 0.0f;
	}
	//------------Transicion de laboratorios
	if (transicionLab > -1 && transicionLab < 3) {
		mostrarLuces = false;
		if (rotarYEscenario < 360) {
			rotarYEscenario += 100.0 * deltaTime;
		}
		else {
			rotarYEscenario = 0.0f;
		}

		if (rotarYLabViejo < 360) {
			rotarYLabViejo += 100.0 * deltaTime;
		}
		else {
			rotarYLabViejo = 0.0f;
		}
	}
	else {
		rotarYEscenario = 0.0f;
		rotarYLabViejo = 0.0f;
		tornadoScale = 0.0f;
		tornadoMove = 0.0f;
		labViejoMove = 0.0f;
		labNuevoMove = 0.0f;
		labNuevoBounce = 0.0f;
		mostrarLuces = true;
	}

	switch (transicionLab) {
	case 0:

		tornadoScale += 0.2 * deltaTime;
		labViejoMove = 200.0 * tornadoScale;


		if (tornadoScale >= 1.0) {
			transicionLab = 1;
		}



		break;

	case 1:

		tornadoMove += 100.0 * deltaTime;

		if (tornadoMove >= 300.0) {
			mostrarEscenarioNuevo = true;
			transicionLab = 2;

		}



		break;

	case 2:
		float maxBounce = 270.0 * 2.0f;

		labNuevoBounce += 3.0f * 80.0f * deltaTime;
		labNuevoMove = abs(100.0f * cos(glm::radians(labNuevoBounce)) * ((maxBounce - labNuevoBounce) / maxBounce));

		if (labNuevoBounce >= maxBounce) {
			renderAlumno1 = false;
			transicionLab = 3;

		}



		break;
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