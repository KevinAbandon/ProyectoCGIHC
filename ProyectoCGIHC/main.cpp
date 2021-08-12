/*---------------------------------------------------------*/
/* ----------------  Proyecto Final -----------*/
/*-----------------    2021-2   ---------------------------*/
/*------------- Alumno: Lechuga Lopez Kevin Martin ---------------*/


#include <Windows.h>
#include <omp.h>
#include <math.h>

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
GLFWmonitor* monitors;

void getResolution(void);

int max_steps = 1000;

//Variables de animacion gato1
void animacionGato1(void);
int estadoGato1 = 0,
pataEstado1 = 0;
float gatoPos1 = 0.0f,
gatoRot1 = 0.0f,
gato1rot1 = 0.0f,
gato1rot2 = 0.0f;

//Variables de animacion gato1
void animacionGato2(void);
int estadoGato2 = 1,
pataEstado2 = 0;
float gatoPos2 = 0.0f,
gatoRot2 = 180.0f,
gato2rot1 = 0.0f,
gato2rot2 = 0.0f;

//Variables de animacion Ave
void animacionAve(void);
int estadoAve = 0, estadoAlas = 0;
float avePosX = -700.0f,
avePosZ = 0.0f,
aveRot = 0.0f,
rotAlas = 0.0f;

//Variables de animacion Auto
void animacionAuto(void);
int estadoAuto = 0;
float autoPosX = 0.0f,
autoPosZ = -625.0f,
rotAuto = 90.0f,
rotllanta = 0.0f;

//Variables de animacion Persona1
void animacionPersona1(void);
int estadoPersoan1 = 0;
float persona1PosX = 0.0f,
rotpersona1 = 90.0f;

//Variables de anoimacion Persona2
void animacionPersona2(void);
int estadoPersona2 = 0;
float rotpersona2 = 270.0f,
rotbrazos = 0.0f,
rotcabeza = 0.0f,
persona2PosZ = 0.0f,
contEstado = 0.0f;


// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
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
glm::vec3 lightPosition(0.0f, 700.0f, -50.0f);
glm::vec3 lightDirection(0.0f, -1.0f, 1.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;

//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
rotRodIzq = 0.0f,
giroMonito = 0.0f,
movBrazoIzq = 0.0f;
float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
rotInc = 0.0f,
giroMonitoInc = 0.0f,
movBrazoIzqInc = 0.0f;


#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
bool play = true;

void animate(void)
{
	if (play){
		animacionGato1();
		animacionGato2();
		animacionAve();
		animacionAuto();
		animacionPersona1();
		animacionPersona2();
	}
}

void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();


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
	// 
	// Skybox
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right3.jpg",
		"resources/skybox/left3.jpg",
		"resources/skybox/top3.jpg",
		"resources/skybox/bottom3.jpg",
		"resources/skybox/front3.jpg",
		"resources/skybox/back3.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	
	Model piso("resources/escenario1/escenario2.obj");
	//Gato
	Model gatotronco("resources/objects/gato/tronco.obj");
	Model patadelder("resources/objects/gato/patadelder.obj");
	Model patadelizq("resources/objects/gato/patadelizq.obj");
	Model patatrasder("resources/objects/gato/patatrasder.obj");
	Model patatrasizq("resources/objects/gato/patatrasizq.obj");
	//Ave
	Model aveTronco("resources/objects/pajaro/tronco.obj");
	Model alaDerecha("resources/objects/pajaro/aladerecha.obj");
	Model alaIzquierda("resources/objects/pajaro/alaizquierda.obj");
	//Auto
	Model chasis("resources/objects/Auto/chasis.obj");
	Model llanta("resources/objects/Auto/llanta.obj");
	//Hombre
	Model tronco("resources/objects/hombre/tronco.obj");
	Model cabeza("resources/objects/hombre/cabeza.obj");
	Model antebrazoder("resources/objects/hombre/antebrazoder.obj");
	Model antebrazoizq("resources/objects/hombre/antebrazoizq.obj");
	Model antepiernader("resources/objects/hombre/antepiernader.obj");
	Model antepiernaizq("resources/objects/hombre/antepiernaizq.obj");
	Model brazoder("resources/objects/hombre/brazoder.obj");
	Model brazoizq("resources/objects/hombre/brazoizq.obj");
	Model piernader("resources/objects/hombre/piernader.obj");
	Model piernaizq("resources/objects/hombre/piernaizq.obj");

	ModelAnim taunt("resources/objects/Taunt/Taunt.dae");
	taunt.initShaders(animShader.ID);

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Separacion de tareas en hilos
	#pragma omp parallel sections shared(window)
	{
		#pragma omp section
		{
			printf("Tareas de renderizado y dibujo por hilo %i.\n", omp_get_thread_num());
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
				staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
				staticShader.setVec3("dirLight.diffuse", glm::vec3(0.2f, 0.0f, 1.0f));
				staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

				staticShader.setVec3("pointLight[0].position", lightPosition);
				staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
				staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
				staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
				staticShader.setFloat("pointLight[0].constant", 0.000008f);
				staticShader.setFloat("pointLight[0].linear", 0.000009f);
				staticShader.setFloat("pointLight[0].quadratic", 0.002f);

				staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
				staticShader.setVec3("pointLight[1].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
				staticShader.setVec3("pointLight[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
				staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
				staticShader.setFloat("pointLight[1].constant", 1.0f);
				staticShader.setFloat("pointLight[1].linear", 0.009f);
				staticShader.setFloat("pointLight[1].quadratic", 0.032f);

				staticShader.setFloat("material_shininess", 32.0f);

				glm::mat4 model = glm::mat4(1.0f);
				glm::mat4 tmp = glm::mat4(1.0f);
				glm::mat4 tmp1 = glm::mat4(1.0f);
				glm::mat4 tmp2 = glm::mat4(1.0f);
				glm::mat4 tmp3 = glm::mat4(1.0f);
				glm::mat4 tmp4 = glm::mat4(1.0f);
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

				model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -200.0f, 0.0f));
				model = glm::scale(model, glm::vec3(1.0f));
				animShader.setMat4("model", model);
				taunt.Draw(animShader);

				// -------------------------------------------------------------------------------------------------------------------------
				// Escenario
				// -------------------------------------------------------------------------------------------------------------------------
				staticShader.use();
				staticShader.setMat4("projection", projection);
				staticShader.setMat4("view", view);

				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(200.0f));
				staticShader.setMat4("model", model);
				piso.Draw(staticShader);

				// -------------------------------------------------------------------------------------------------------------------------
				// Gato 1
				// -------------------------------------------------------------------------------------------------------------------------


				model = glm::translate(glm::mat4(1.0f), glm::vec3(-1500.0f, 0, gatoPos1));
				model = glm::translate(model, glm::vec3(posX, posY, posZ));
				tmp = model = glm::rotate(model, glm::radians(gatoRot1), glm::vec3(0.0f, 1.0f, 0.0));
				model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
				staticShader.setMat4("model", model);
				gatotronco.Draw(staticShader);

				//Pierna Der
				model = glm::translate(tmp, glm::vec3(-6.0f, 20.0f, -18.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
				model = glm::rotate(model, glm::radians(gato1rot1), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
				staticShader.setMat4("model", model);
				patatrasder.Draw(staticShader);


				//Pierna Izq
				model = glm::translate(tmp, glm::vec3(6.0f, 20.0f, -18.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(gato1rot2), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
				staticShader.setMat4("model", model);
				patatrasizq.Draw(staticShader);


				//Brazo derecho
				model = glm::translate(tmp, glm::vec3(-3.5f, 14.0f, 9.0f));
				model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(gato1rot1), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
				staticShader.setMat4("model", model);
				patadelder.Draw(staticShader);

				//Brazo izquierdo
				model = glm::translate(tmp, glm::vec3(3.5f, 14.0f, 9.0f));
				model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(gato1rot2), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
				staticShader.setMat4("model", model);
				patadelizq.Draw(staticShader);


				// -------------------------------------------------------------------------------------------------------------------------
				// Gato 2
				// -------------------------------------------------------------------------------------------------------------------------


				model = glm::translate(glm::mat4(1.0f), glm::vec3(1500.0f, 0, gatoPos2));
				model = glm::translate(model, glm::vec3(posX, posY, posZ));
				tmp = model = glm::rotate(model, glm::radians(gatoRot2), glm::vec3(0.0f, 1.0f, 0.0));
				model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
				staticShader.setMat4("model", model);
				gatotronco.Draw(staticShader);

				//Pierna Der
				model = glm::translate(tmp, glm::vec3(-6.0f, 20.0f, -18.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
				model = glm::rotate(model, glm::radians(gato2rot1), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
				staticShader.setMat4("model", model);
				patatrasder.Draw(staticShader);


				//Pierna Izq
				model = glm::translate(tmp, glm::vec3(6.0f, 20.0f, -18.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(gato2rot2), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
				staticShader.setMat4("model", model);
				patatrasizq.Draw(staticShader);


				//Brazo derecho
				model = glm::translate(tmp, glm::vec3(-3.5f, 14.0f, 9.0f));
				model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(gato2rot1), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
				staticShader.setMat4("model", model);
				patadelder.Draw(staticShader);

				//Brazo izquierdo
				model = glm::translate(tmp, glm::vec3(3.5f, 14.0f, 9.0f));
				model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(gato2rot2), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
				staticShader.setMat4("model", model);
				patadelizq.Draw(staticShader);


				// -------------------------------------------------------------------------------------------------------------------------
				// Ave
				// -------------------------------------------------------------------------------------------------------------------------


				model = glm::translate(glm::mat4(1.0f), glm::vec3(avePosX, 700.0f, avePosZ));
				model = glm::translate(model, glm::vec3(posX, posY, posZ));
				tmp = model = glm::rotate(model, glm::radians(aveRot), glm::vec3(0.0f, 1.0f, 0.0));
				model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
				staticShader.setMat4("model", model);
				aveTronco.Draw(staticShader);

				//Ala Derecha
				model = glm::translate(tmp, glm::vec3(2.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
				model = glm::rotate(model, glm::radians(rotAlas), glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
				staticShader.setMat4("model", model);
				alaDerecha.Draw(staticShader);


				//Ala Izquierda
				model = glm::translate(tmp, glm::vec3(-2.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(-rotAlas), glm::vec3(1.0f, 0.0f, 1.0f));
				model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
				staticShader.setMat4("model", model);
				alaIzquierda.Draw(staticShader);


				// -------------------------------------------------------------------------------------------------------------------------
				// Auto
				// -------------------------------------------------------------------------------------------------------------------------

				model = glm::translate(glm::mat4(1.0f), glm::vec3(autoPosX, 60.0f, autoPosZ));
				model = glm::translate(model, glm::vec3(posX, posY, posZ));
				tmp = model = glm::rotate(model, glm::radians(rotAuto), glm::vec3(0.0f, 1.0f, 0.0));
				model = glm::scale(model, glm::vec3(90.0f, 90.0f, 90.0f));
				staticShader.setMat4("model", model);
				chasis.Draw(staticShader);

				model = glm::translate(tmp, glm::vec3(-55.0f, -40.0f, 105.0f));
				model = glm::rotate(model, glm::radians(rotllanta), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(90.0f, 90.0f, 90.0f));
				staticShader.setMat4("model", model);
				llanta.Draw(staticShader); //Delantera derecha

				model = glm::translate(tmp, glm::vec3(-55.0f, -40.0f, -105.0f));
				model = glm::rotate(model, glm::radians(rotllanta), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(90.0f, 90.0f, 90.0f));
				staticShader.setMat4("model", model);
				llanta.Draw(staticShader); //Tracera derecha

				model = glm::translate(tmp, glm::vec3(60.0f, -40.0f, -105.0f));
				model = glm::rotate(model, glm::radians(rotllanta), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(90.0f, 90.0f, 90.0f));
				staticShader.setMat4("model", model);
				llanta.Draw(staticShader); //Tracera izquierda

				model = glm::translate(tmp, glm::vec3(60.0f, -40.0f, 105.0f));
				model = glm::rotate(model, glm::radians(rotllanta), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(90.0f, 90.0f, 90.0f));
				staticShader.setMat4("model", model);
				llanta.Draw(staticShader); //Delantera derecha


				// -------------------------------------------------------------------------------------------------------------------------
				// Hombre
				// -------------------------------------------------------------------------------------------------------------------------


				model = glm::translate(glm::mat4(1.0f), glm::vec3(persona1PosX, 110.0f, -800.0f));
				tmp = model = glm::rotate(model, glm::radians(rotpersona1), glm::vec3(0.0f, 1.0f, 0.0));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				tronco.Draw(staticShader); //Tronco

				model = glm::translate(tmp, glm::vec3(0.0f, 34.0f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				cabeza.Draw(staticShader); //Cabeza

				model = glm::translate(tmp, glm::vec3(-18.0f, 24.0f, 0.0f));
				model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				tmp1 = model = glm::rotate(model, glm::radians(gato1rot1), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				antebrazoder.Draw(staticShader); //Antebrazo derecho

				model = glm::translate(tmp, glm::vec3(18.0f, 24.0f, 0.0f));
				model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				tmp2 = model = glm::rotate(model, glm::radians(gato1rot2), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				antebrazoizq.Draw(staticShader); //Antebrazo izquierdo

				model = glm::translate(tmp, glm::vec3(-10.0f, -34.0f, 0.0f));
				tmp3 = model = glm::rotate(model, glm::radians(gato1rot2), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				antepiernader.Draw(staticShader); //Antepierna derecho

				model = glm::translate(tmp, glm::vec3(10.0f, -34.0f, 0.0f));
				tmp4 = model = glm::rotate(model, glm::radians(gato1rot1), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				antepiernaizq.Draw(staticShader); //Antepierna izquierdo

				model = glm::translate(tmp1, glm::vec3(-16.0f, -10.0f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				brazoder.Draw(staticShader); //Brazo derecha

				model = glm::translate(tmp2, glm::vec3(16.0f, -10.0f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				brazoizq.Draw(staticShader); //Brazo izquierdo

				model = glm::translate(tmp3, glm::vec3(-4.0f, -34.0f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				piernader.Draw(staticShader); //Pierna derecha

				model = glm::translate(tmp4, glm::vec3(4.0f, -34.0f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				piernaizq.Draw(staticShader); //Pierna izquierda


				// -------------------------------------------------------------------------------------------------------------------------
				// Hombre 2
				// -------------------------------------------------------------------------------------------------------------------------


				model = glm::translate(glm::mat4(1.0f), glm::vec3(-700.0f, 110.0f, persona2PosZ));
				tmp = model = glm::rotate(model, glm::radians(rotpersona2), glm::vec3(0.0f, 1.0f, 0.0));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				tronco.Draw(staticShader); //Tronco

				model = glm::translate(tmp, glm::vec3(0.0f, 34.0f, 0.0f));
				model = glm::rotate(model, glm::radians(rotcabeza), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				cabeza.Draw(staticShader); //Cabeza

				model = glm::translate(tmp, glm::vec3(-18.0f, 24.0f, 0.0f));
				model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				tmp1 = model = glm::rotate(model, glm::radians(rotbrazos), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				antebrazoder.Draw(staticShader); //Antebrazo derecho

				model = glm::translate(tmp, glm::vec3(18.0f, 24.0f, 0.0f));
				model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				tmp2 = model = glm::rotate(model, glm::radians(-rotbrazos), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				antebrazoizq.Draw(staticShader); //Antebrazo izquierdo

				model = glm::translate(tmp, glm::vec3(-10.0f, -34.0f, 0.0f));
				tmp3 = model = glm::rotate(model, glm::radians(-rotbrazos), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				antepiernader.Draw(staticShader); //Antepierna derecho

				model = glm::translate(tmp, glm::vec3(10.0f, -34.0f, 0.0f));
				tmp4 = model = glm::rotate(model, glm::radians(rotbrazos), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				antepiernaizq.Draw(staticShader); //Antepierna izquierdo

				model = glm::translate(tmp1, glm::vec3(-16.0f, -10.0f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				brazoder.Draw(staticShader); //Brazo derecha

				model = glm::translate(tmp2, glm::vec3(16.0f, -10.0f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				brazoizq.Draw(staticShader); //Brazo izquierdo

				model = glm::translate(tmp3, glm::vec3(-4.0f, -34.0f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				piernader.Draw(staticShader); //Pierna derecha

				model = glm::translate(tmp4, glm::vec3(4.0f, -34.0f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
				staticShader.setMat4("model", model);
				piernaizq.Draw(staticShader); //Pierna izquierda



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
		}
		#pragma omp section
		{
			//Control de audio
			printf("Tareas de audio por hilo %i.\n", omp_get_thread_num());
			PlaySound(TEXT("resources/audio/IchikaNito_Homesick.wav"), NULL, SND_ASYNC);
			float camX, camY, camZ, distAuto, distGato1, distGato2;
			while (!glfwWindowShouldClose(window)) {
				camX = camera.Position[0];
				camY = camera.Position[1];
				camZ = camera.Position[2];
				
				distAuto = sqrt(pow(camX - autoPosX,2) + pow(camY - 60.0f, 2) + pow(camZ - autoPosZ,2));
				distGato1 = sqrt(pow(camX - (-1500.0f), 2) + pow(camY, 2) + pow(camZ - gatoPos1, 2));
				distGato2 = sqrt(pow(camX - 1500.0f, 2) + pow(camY, 2) + pow(camZ - gatoPos2, 2));
				if (distAuto <= 200.0f) {
					printf("Claxon\n");
					PlaySound(TEXT("resources/audio/claxon.wav"), NULL, SND_ALIAS);
					PlaySound(TEXT("resources/audio/IchikaNito_Homesick.wav"), NULL, SND_ASYNC);
				}
				if (distGato1 <= 200.0f || distGato2 <= 200.0f) {
					printf("Gato\n");
					PlaySound(TEXT("resources/audio/maullido.wav"), NULL, SND_ALIAS);
					PlaySound(TEXT("resources/audio/IchikaNito_Homesick.wav"), NULL, SND_ASYNC);
				}
			}

		}
	}

	PlaySound(NULL, NULL, SND_ASYNC);

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
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

//Funciones de animaciones

void animacionGato1() {
	if (estadoGato1 == 0) {
		gatoPos1 += 1.0f;
		if (gatoPos1 >= 700) {
			gatoRot1 = 180;
			estadoGato1 = 1;
		}
	}
	else {
		gatoPos1 -= 1.0f;
		if (gatoPos1 <= -1000) {
			gatoRot1 = 0;
			estadoGato1 = 0;
		}
	}
	if (pataEstado1 == 0){
		gato1rot1 += 2;
		gato1rot2 = -1 * gato1rot1;
		if (gato1rot1 >= 50){
			pataEstado1 = 1;
		}
	}
	else {
		gato1rot1 -= 2;
		gato1rot2 = -1 * gato1rot1;
		if (gato1rot1 <= -50) {
			pataEstado1 = 0;
		}
	}
}

void animacionGato2() {
	if (estadoGato2 == 0) {
		gatoPos2 += 1.0f;
		if (gatoPos2 >= 700) {
			gatoRot2 = 180;
			estadoGato2 = 1;
		}
	}
	else {
		gatoPos2 -= 1.0f;
		if (gatoPos2 <= -1000) {
			gatoRot2 = 0;
			estadoGato2 = 0;
		}
	}
	if (pataEstado2 == 0) {
		gato2rot1 += 2;
		gato2rot2 = -1 * gato2rot1;
		if (gato2rot1 >= 50) {
			pataEstado2 = 1;
		}
	}
	else {
		gato2rot1 -= 2;
		gato2rot2 = -1 * gato2rot1;
		if (gato2rot1 <= -50) {
			pataEstado2 = 0;
		}
	}
}

void animacionAve() {
	if (estadoAve == 0) {
		avePosZ += 2.0f;
		if (avePosZ >= 500.0f) {
			aveRot = 90;
			estadoAve = 1;
		}
	}
	else if (estadoAve == 1) {
		avePosX += 2.0f;
		if (avePosX >= 700) {
			aveRot = 180;
			estadoAve = 2;
		}
	}
	else if (estadoAve == 2) {
		avePosZ -= 2.0f;
		if (avePosZ <= -500.0f) {
			aveRot = 270.0f;
			estadoAve = 3;
		}
	}
	else {
		avePosX -= 2.0f;
		if (avePosX <= -700.0f) {
			aveRot = 0.0f;
			estadoAve = 0;
		}
	}

	if (estadoAlas == 0) {
		rotAlas += 3.0f;
		if (rotAlas >= 40) {
			estadoAlas = 1;
		}
	}
	else {
		rotAlas -= 3.0f;
		if (rotAlas <= -40) {
			estadoAlas = 0;
		}
	}
}

void animacionAuto() {
	rotllanta += 1.0f;
	if (estadoAuto == 0) {
		autoPosX += 2.0f;
		if (autoPosX >= 500.0f) {
			estadoAuto = 1;
			rotAuto = 0.0f;
		}
	}
	else if (estadoAuto == 1) {
		autoPosZ += 2.0f;
		if (autoPosZ >= 625.0f) {
			estadoAuto = 2;
			rotAuto = 270.0f;
		}
	}
	else if (estadoAuto == 2) {
		autoPosX -= 2.0f;
		if (autoPosX <= -500.0f) {
			estadoAuto = 3;
			rotAuto = 180.0f;
		}
	}
	else {
		autoPosZ -= 2.0f;
		if (autoPosZ <= -625.0f) {
			estadoAuto = 0;
			rotAuto = 90.0f;
		}
	}
}

void animacionPersona1() {
	if (estadoPersoan1 == 0) {
		persona1PosX += 2.0f;
		if (persona1PosX >= 700.0f) {
			rotpersona1 = 270.0f;
			estadoPersoan1 = 1;
		}
	}
	else {
		persona1PosX -= 2.0f;
		if (persona1PosX <= -700.0f) {
			rotpersona1 = 90.0f;
			estadoPersoan1 = 0;
		}
	}
}

void animacionPersona2() {
	if (estadoPersona2 == 0) {
		rotcabeza = rotAlas/7;
		contEstado += 0.1f;
		if (contEstado >= 20.0f) {
			rotcabeza = 0.0f;
			contEstado = 0.0f;
			rotpersona2 = 180.0f;
			estadoPersona2 = 1;
		}
	}
	else if (estadoPersona2 == 1) {
		persona2PosZ -= 2.0f;
		rotbrazos = gato1rot1;
		if (persona2PosZ <= -300) {
			rotbrazos = 0.0f;
			rotpersona2 = 270.0f;
			estadoPersona2 = 2;
		}
	}
	else if (estadoPersona2 == 2) {
		rotcabeza = rotAlas / 7;
		contEstado += 0.1f;
		if (contEstado >= 20.0f) {
			rotcabeza = 0.0f;
			contEstado = 0.0f;
			rotpersona2 = 0.0f;
			estadoPersona2 = 3;
		}
	}
	else {
		persona2PosZ += 2.0f;
		rotbrazos = gato1rot1;
		if (persona2PosZ >= 0) {
			rotbrazos = 0.0f;
			rotpersona2 = 270.0f;
			estadoPersona2 = 0;
		}
	}
}