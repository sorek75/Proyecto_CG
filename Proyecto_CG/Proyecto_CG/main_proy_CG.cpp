
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 300.0f, 300.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
// mover carro 
float
movAuto_x = 0.0f,
movAuto_z = 0.0f,
orienta = 0.0f,
inclinaLlantas = 0.0f,
giroLlantas = 0.0f;
bool
mov_1 = false,
mov_2 = false,
mov_3 = false,
mov_4 = false,
mov_5 = false,
soundjeep = false,
animacionCar = false;
// mover ovni 
float
movOvni_x = 0.0f,
movOvni_y = 0.0f,
movOvni_z = 0.0f,
orientaOvni = 0.0f;
// animacion Ovni
bool
movOvni_1 = false,
movOvni_2 = false,
movOvni_3 = false,
movOvni_4 = false,
movOvni_5 = false,
soundovni = false,
animacionOvni = false;
// mover anciano 
float
movAnc_x = 0.0f,
movAnc_y = 0.0f,
movAnc_z = 0.0f,
tamAnc = 0.14f;
// mover soldado
float   movS_x = 119.0f,
movS_y = -7.0f,
movS_z = -268.0f,
rot = 180.0f;

// persecucion
float	movRemy_x = 0.0f,
movAlien_x = 0.0f;

// Patada Voladora
float	alienSalto_y = 3.0f,
alienSalto_z = -264.0,
alienPatada_z = 0.0f,
alienPatada_zz = -264.0f,
alienPatada_y = -80.0f,
soldadoEvade_y = 2.0f,
alienSalto_rot = 0.0f,
soldadoFall_y = -80.0f;

int		estadoPatada = 0;

bool	animacion = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
rotRodIzq = 0.0f,
giroMonito = 0.0f,

perroX = 119.0f,
perroY = -7.0f,
perroZ = -268.0f,
giroPataTI = 0.0f,
giroPataTD = 0.0f,
giroPataDI = 0.0f,
giroPataDD = 0.0f,
giroPerro = 0.0f;

float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
rotInc = 0.0f,
giroMonitoInc = 0.0f,

perroXInc = 0.0f,
perroYInc = 0.0f,
perroZInc = 0.0f,
giroPataTIInc = 0.0f,
giroPataTDInc = 0.0f,
giroPataDIInc = 0.0f,
giroPataDDInc = 0.0f,
giroPerroInc = 0.0f;

float	myVariable = 0.0f;


//Keyframes (Manipulación y dibujo)
//petera
float	peteraX = 345.0f,
		peteraY = 60.0f,
		peteraZ = 356.0f,
		peteraRot =0.0f,
		alaIRot = 0.0f,
		alaDRot = 0.0f;
#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}

void animate(void)
{
	//lightPosition.x = 100.0f * cos(myVariable);
	//lightPosition.z = 100.0f * sin(myVariable);

	//myVariable += 0.1f;


	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			i_curr_steps++;
		}
	}

	//Vehículo
	if (animacion)
	{
		movAuto_z += 3.0f;
	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model encierro_Trex("resources/objects/enc_terres/terrestre.obj");
	Model encierro_Herb("resources/objects/terrestreH/terrestreH.obj");
	Model starbucks("resources/objects/Starbucks/starbucks.obj");
	Model trex("resources/objects/dino2/trex.obj");
	Model trico("resources/objects/dino3/dino3.obj");
	Model edificioV2("resources/objects/EdificioV2/edificioV2.obj");
	Model arbol1("resources/objects/Arboles/Arbol1/Arbol1.obj");
	Model arbol2("resources/objects/Arboles/Arbol2/Arbol2.obj");
	Model arbol4("resources/objects/Arboles/Arbol4/Arbol4.obj");
	Model arbol6("resources/objects/Arboles/Arbol6/Arbol6.obj");
	Model arbol7("resources/objects/Arboles/Arbol7/Arbol7.obj");
	Model mesa("resources/objects/Mesa/mesa.obj");
	Model banca("resources/objects/Banca/banca.obj");
	//Model petera("resources/objects/dino4/dino4.obj");
	//Petera partes
	Model peteraAD("resources/objects/dino4/AlaDer/AlaDer.obj");
	Model peteraAI("resources/objects/dino4/AlaIzq/AlaIzq.obj");
	Model peteraT("resources/objects/dino4/TorsoDino4/TorsoDino4.obj");
	//
	Model raptor("resources/objects/dino1/dino1.obj");
	Model encierro_vol("resources/objects/enc_volador/enc_volador.obj"); 
	Model recuerdos("resources/objects/Recuerdos/recuerdos.obj");

	//Carro
	Model jeep("resources/objects/Carro/carro.obj");
	Model rueda("resources/objects/Carro/rueda.obj");

	ModelAnim soldadoFall("resources/objects/SoldierFall/SoldierFall.dae");
	soldadoFall.initShaders(animShader.ID);

	ModelAnim personRun("resources/objects/PersonRun/PersonRun.dae");
	personRun.initShaders(animShader.ID);
	
	

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));	//color para todo el ambiente
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));	//color para ciertas partes, depende de la direccion de la iluminacion 
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));	//brillo de los objetos

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);	//la luz viaja a mas distancia 

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 0.08f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[2].position", glm::vec3(80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 0.08f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.00032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f - movRemy_x, 3.0f, -70.0f));//movS_x, movS_y, movS_z));//
		model = glm::scale(model, glm::vec3(0.065f));
		model = glm::rotate(model, glm::radians(260.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		personRun.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(364.0f, soldadoFall_y, -190.0f));//movS_x, movS_y, movS_z));
		model = glm::scale(model, glm::vec3(0.15f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		soldadoFall.Draw(animShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 0.3f, -348.0f));
		model = glm::scale(model, glm::vec3(0.30f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		edificioV2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-170.0f, 0.2f, -320.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		starbucks.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-324.0f, 30.0f, -202.0f));
		model = glm::scale(model, glm::vec3(30.0f));
		staticShader.setMat4("model", model);
		recuerdos.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		encierro_Trex.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-184.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		encierro_Trex.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-365.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		encierro_Trex.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		encierro_Herb.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(120.0f, 0.0f, -391.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(120.0f, 0.0f, -356.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(120.0f, 0.0f, -322.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(363.0f, 37.0f, -163.0f));
		model = glm::scale(model, glm::vec3(16.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		trex.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-374.0f, 20.0f, 82.0f));
		model = glm::scale(model, glm::vec3(6.5f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		trico.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-255.0f, 20.0f, 130.0f));
		model = glm::scale(model, glm::vec3(6.5f));
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		trico.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Petera
		// ------------------------------------------------------------------------------------------------------------------------- 
		//Torso
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 12.0f, 0.0f));
		model = glm::translate(model, glm::vec3(peteraX, peteraY, peteraZ));
		tmp = model = glm::rotate(model, glm::radians(peteraRot), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		peteraT.Draw(staticShader);
		// coordenadas petera (345.0f, 60.0f, 356.0f));
		
		//AlaDer
		model = glm::translate(tmp, glm::vec3(-0.85f, -0.5f, 2.0f));
		//model = glm::translate(model, glm::vec3(movS_x, 0.0f, movS_z));
		model = glm::rotate(model, glm::radians(alaDRot), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		peteraAD.Draw(staticShader);

		//Ala Izq
		model = glm::translate(tmp, glm::vec3(0.85f, -0.5f, 2.0f));
		model = glm::rotate(model, glm::radians(alaIRot), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		peteraAI.Draw(staticShader);

		///////////////////////////////////////////////

		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-185.736f + movAuto_x, 0.0f, -250.228 + movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		jeep.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(-11.0f, 6.0f, 19.0f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(inclinaLlantas), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		rueda.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(11.0f, 6.0f, 19.0f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(inclinaLlantas), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		rueda.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-11.0f, 6.0f, -20.5f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		rueda.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(11.0f, 6.0f, -20.5f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		rueda.Draw(staticShader);	//Izq trase

		model = glm::translate(glm::mat4(1.0f), glm::vec3(117.0f, 0.0f, -21.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		raptor.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(37.0f, 0.0f, -14.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		raptor.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(54.0f, 0.0f, -150.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::rotate(model, glm::radians(-160.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		raptor.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(330.0f, 0.2f, 328.0f));
		model = glm::scale(model, glm::vec3(30.0f));
		staticShader.setMat4("model", model);
		encierro_vol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-449.0f, 0.0f, -43.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-137.0f, 0.0f, -43.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-137.0f, 0.0f, 276.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-457.0f, 0.0f, 461.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-293.0f, 0.0f, 98.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-297.0f, 0.0f, 366.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(111.0f, 0.0f, -230.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol4.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(78.0f, 0.0f, 328.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(78.0f, 0.0f, 480.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(78.0f, 0.0f, 400.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-116.0f, 0.0f, 189.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-116.0f, 0.0f, 69.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-472.0f, 0.0f, 69.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-472.0f, 0.0f, 221.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		arbol6.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(64.0f, 0.0f, -453.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		arbol7.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(166.0f, 0.0f, -453.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		arbol7.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		peteraZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		peteraZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		peteraX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		peteraX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		peteraRot--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		peteraRot++;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		alaDRot--;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		alaDRot++;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		alaIRot--;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		alaIRot++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
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
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}