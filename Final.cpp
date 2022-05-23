/*
	* PROYECTO FINAL - LAB. COMPUTACIÓN GRAFICA E INTERACCIÓN HUMANO-COMPUTADORA
	* Integrantes:
	* - Aguilar González Oscar
	* - Báez Cadena Diestefano Michel
	* - Hernández Rodríguez Alejandro
	* SEMESTRE 2022-2
*/

//--------------import Libraries-------------------
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

/*#pragma comment(lib, "winmm.lib")*/

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
glm::vec3 lightPosition(0.0f, 4.0f, -5.0f);
glm::vec3 lightDirection(-1.0f, 0.0f, 0.0f);

// posiciones
float	PI = 3.1416f,
		thetatest = -90.f,
		GRAVITY=9.1;

bool	activate_car_animation=false;

//variables del coche
float	car_pos_ini_x = 200.0f,
		car_pos_ini_z = -260.0f,
		car_x = 0.0f,
		car_z = 0.0f,
		pos_vuelta = 0.0f,
		car_direction = 0.0f,
		car_theta1 = 4.7123f;

int		car_state=0;

//variables del pez
		//para mover todo el pez
		//para posicion relativa
float	fish_pos_ini_x = 290.0f,
		fish_pos_ini_y = 1.5f,
		fish_pos_ini_z = 150.0f,
//avance respecto de la posicion relativa
		fish_x = 0.0f,
		fish_z = 0.0f,
		fish_y = 0.0f,
//estas son para rotarlo en cuanto a los ejes
		fish_theta_x = 0.0f,
		fish_theta_y = -90.0f,
		fish_theta_z = 0.0f,
//estas son para senos y cosenos
		fish_phi_mov = 0.0f,
		aumento=30.0f,
		decremento = 0.0f;

//para mover partes del pez en rotaciones
float	fish_angle_head_x = 0.0f,
		fish_angle_head_y = 0.0f,
		fish_angle_head_z = 0.0f,
		fish_angle_bot_x  = 0.0f,
		fish_angle_bot_y  = 0.0f,
		fish_angle_bot_z  = 0.0f,
		fish_angle_tail_x = 0.0f,
		fish_angle_tail_y = 0.0f,
		fish_angle_tail_z = 0.0f;

bool	activate_fish_animation = false;
int		fish_state = 0,
		fish_sub_state=0;

/********* variables de la persona *********/
bool	activate_guy_animation = false;
float	guy_angle_leg = 0.0f,
		guy_angle_arm = 0.0f,
		guy_pos_ini_x = 100.0f,
		guy_pos_ini_z = 100.0f,
		guy_posx = 0.0f,
		guy_posz = 0.0f,
		guy_phi = 0.0f,
		guy_oriented = -90.0f,
		guy_angle_neck = 0.0f;


int		guy_state_leg = 0,
		guy_state = 0;

//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f;

float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;

// Keyframes para el dino
float	dino_posX = -200.0f,
		dino_posY = 10.0f,
		dino_posZ = -140.0f,
		dino_rotPiernaIzq = 0.0f,
		dino_rotPiernaDer = 0.0f,
		dino_giro = 0.0f,
		dino_rotBoca = 0.0f,
		dino_rotCola = 0.0f;

float	dino_incX = 0.0f,
		dino_incY = 0.0f,
		dino_incZ = 0.0f,
		dino_rotPiernaIzqInc = 0.0f,
		dino_rotPiernaDerInc = 0.0f,
		dino_anguloIzq = 0.0f,
		dino_anguloDer = 0.0f,
		dino_anguloCola = 0.0f,
		dino_anguloBoca = 0.0f,
		dino_giroInc = 0.0f,
		dino_rotBocaInc = 0.0f,
		dino_rotColaInc = 0.0f;

//variables helicoptero
float	giro_helices = 0.0f,
		rot_hel = -90.0f,
		helicoptero_x = 0.0f,
		helicoptero_y = 0.0f,
		helicoptero_z = 0.0f,
		heliceGrande_x = -30.0f,
		heliceGrande_y = 0.0f,
		heliceGrande_z = 0.0f,
		heliceChica_x = 46.0f,
		heliceChica_y = 0.0f,
		heliceChica_z = 0.0f,
		hel_direccion = -90.0f;

int		hel_state = 0;
bool	activate_hel_animation = false;

#define MAX_FRAMES 15
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float dino_posX;		//Variable para PosicionX
	float dino_posY;		//Variable para PosicionY
	float dino_posZ;
	float rotRodIzq;
	float giroMonito;
	float dino_giro;
	float dino_rotPiernaDer;
	float dino_rotPiernaIzq;
	float dino_rotCola;
	float dino_rotBoca;
	int n;

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

	FrameIndex++;
}

void resetElements(void)
{
	dino_posX = KeyFrame[0].dino_posX;
	dino_posY = KeyFrame[0].dino_posY;
	dino_posZ = KeyFrame[0].dino_posZ;
	dino_giro = KeyFrame[0].dino_giro;
}


void interpolation(void)
{
	dino_incX = (KeyFrame[playIndex + 1].dino_posX - KeyFrame[playIndex].dino_posX) / i_max_steps;
	dino_incY = (KeyFrame[playIndex + 1].dino_posY - KeyFrame[playIndex].dino_posY) / i_max_steps;
	dino_incZ = (KeyFrame[playIndex + 1].dino_posZ - KeyFrame[playIndex].dino_posZ) / i_max_steps;
	dino_rotPiernaDerInc = 2 * KeyFrame[playIndex + 1].n * PI / i_max_steps;
	dino_rotPiernaIzqInc = 2 * KeyFrame[playIndex + 1].n * PI / i_max_steps;
	dino_rotBocaInc = 2 * (KeyFrame[playIndex + 1].n / 2) * PI / i_max_steps;
	dino_rotColaInc = 2 * (KeyFrame[playIndex + 1].n / 2) * PI / i_max_steps;
	dino_giroInc = (KeyFrame[playIndex + 1].dino_giro - KeyFrame[playIndex].dino_giro) / i_max_steps;
}

void animate(void)
{
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
			/************ ANIMACION DEL DINOSAURIO ****************/
			dino_posX += dino_incX;
			dino_posY += dino_incY;
			dino_posZ += dino_incZ;

			dino_anguloDer += dino_rotPiernaDerInc;
			dino_anguloIzq -= dino_rotPiernaIzqInc;

			dino_rotPiernaDer = 30 * sin(dino_anguloDer);
			dino_rotPiernaIzq = 30 * sin(dino_anguloIzq);

			dino_anguloBoca += dino_rotBocaInc;

			dino_anguloCola += dino_rotColaInc;

			dino_rotBoca = 15 * sin(dino_anguloBoca);

			dino_rotCola = 30 * sin(dino_anguloCola);

			dino_giro += dino_giroInc;

			i_curr_steps++;
		}
	}

	/************* ANIMACION VEHICULO ************/
	if (activate_car_animation) {
		//echar reversa
		if (car_state == 0) {
			car_z += 0.0f;
			car_x += 1.0f;
			cout << "state 0= " << car_state << std::endl;
			if(car_x >= 50.0f){
				car_state = 1;
				car_pos_ini_x = 250.f;
				car_x = 0.0f;
			}
		}
		//dar vuelta en reversa
		if (car_state == 1) {
			cout << "state1= " << car_state << std::endl;
			if (car_theta1 >= 4.6 && car_theta1 <= 6.28) {
				car_x = 100.0f * cos(car_theta1);
				car_z = (100.0f * sin(car_theta1)) + 100.f;
				car_theta1 += 0.01f;
				car_direction -= .55f;
				cout << "theta1= " << car_theta1 << std::endl;
				if (car_theta1 >= 6.28) {
					// cambio de estado reinicio de variables
					// y reinico de variables de cambio
					car_state = 2; 
					car_pos_ini_x = car_pos_ini_x + car_x;
					car_pos_ini_z = car_pos_ini_z + car_z;
					car_z = 0;
					car_x = 0;
				}
			}
		}
		//movimiento Recto
		if (car_state == 2) {
			cout << "state= " << car_state << std::endl;
			cout << "Z=  " << car_z << std::endl;
			car_x += 0.0f;
			car_z += 0.5f;
			car_direction -= .1f;
			if (car_z >= 50.0f) {
				car_state = 3;
				car_pos_ini_x = car_pos_ini_x + car_x;
				car_pos_ini_z = car_pos_ini_z + car_z;
				car_z = 0;
				car_x = 0;
				car_theta1 = 0;
			}
		}

		//Elipse 1
		if (car_state == 3) {
			car_x = (200.0f * cos(car_theta1))-200.0f;
			car_z = 70.0f * sin(car_theta1);
			car_theta1 += 0.01;
			car_direction -= .65f;
			if (car_theta1 >= 1.27) {
				car_state = 4;
				car_pos_ini_x = car_pos_ini_x + car_x;
				car_pos_ini_z = car_pos_ini_z + car_z;
				car_z = 0;
				car_x = 0;
				car_theta1 = 0;
			}
		}

		if (car_state == 4) {
			cout <<"X= "<<car_x << std::endl;
			car_x -= 0.6f;
			if (car_x <= -135.0f) {
				car_state = 5;
				car_pos_ini_x = car_pos_ini_x + car_x;
				car_pos_ini_z = car_pos_ini_z + car_z;
				car_z = 0;
				car_x = 0;
				cout << "state= " << car_state << std::endl;
				car_theta1 = 4.71238898;//inicializando variable para el siguiente estado
			}
		}

		if (car_state == 5) {
			cout << "angle= " << car_theta1 << std::endl;
			cout << "dir= " << car_direction << std::endl;
			car_x = 100.0f * cos(car_theta1);
			car_z = (100.0f * sin(car_theta1))+100.0f;
			car_theta1 -= 0.01f;
			car_direction += .65f;
			if (car_theta1 <= 2.6737f) {
				car_state = 6;
				car_pos_ini_x = car_pos_ini_x + car_x;
				car_pos_ini_z = car_pos_ini_z + car_z;
				car_z = 0;
				car_x = 0;
				car_theta1 = 0.0f;
			}
		}

		if (car_state ==6){
			cout << "Z= " << car_z << std::endl;
			cout << "dir= " << car_direction << std::endl;

			car_z += 0.3;
			if (car_direction <= -90.0f) {
				car_direction += 0.0f;
			}

			if (car_direction >= -90.0f) {
				car_direction -= 0.3f;
			}

			if (car_z >= 130.0f) {
				car_state = 7;
				car_pos_ini_x = car_pos_ini_x + car_x;
				car_pos_ini_z = car_pos_ini_z + car_z;
				car_z = 0;
				car_x = 0;
				car_theta1 = 0.0f;
			}
		}

		if (car_state == 7) {
			cout << "my angle= " << car_theta1 << std::endl;
			cout << "dir= " << car_direction << std::endl;
			car_x = (50.0f * cos(car_theta1)) - 50.0f;
			car_z = 50.0f * sin(car_theta1);
			car_theta1 -= 0.01f;
			car_direction += .82f;
			if (car_theta1<= -1.1f) {
				car_state = 8;
				car_pos_ini_x = car_pos_ini_x + car_x;
				car_pos_ini_z = car_pos_ini_z + car_z;
				car_z = 0;
				car_x = 0;
				car_theta1 = 0.0f;
			}
		}

		if (car_state == 8) {
			car_x -= 0.4f;
			if (car_x <= -70.0f) {
				car_state = 9;
				car_pos_ini_x = car_pos_ini_x + car_x;
				car_pos_ini_z = car_pos_ini_z + car_z;
				car_z = 0;
				car_x = 0;
				car_theta1 = 4.7123f;
				car_state = -1;
			}

		}


	}
/******************* ANIMACION PESCADO ********************/
	if (activate_fish_animation) {

		if (fish_state == 0) {
			fish_x = (aumento * cos(fish_phi_mov)) - 30.0f;
			fish_z =  aumento * sin(fish_phi_mov);
			aumento -= 0.04f;
			fish_phi_mov += 0.01f;
			fish_y += 0.03;
			fish_theta_y-=0.7;
			
			std::cout << "phi = " << fish_phi_mov << std::endl;
			std::cout << "x = " << fish_x << std::endl;
			std::cout << "z = " << fish_z << std::endl;
			std::cout << "state = " << fish_state << std::endl;
			
			//cabeza
			if (fish_y <= 16.0f) {
				if (fish_angle_head_y <= -45.0f) { fish_angle_head_y += 0.0f; }
				else { fish_angle_head_y--; }
				//cuerpo total
				if (fish_theta_x >= 30.0f) { fish_theta_x += 0.0f; }
				else { fish_theta_x += 0.05; }
				//cuerpo de abajo
				if (fish_angle_bot_y >= 15.0f) { fish_angle_bot_y += 0.0f; }
				else { fish_angle_bot_y += 1.0f; }
				//cola
				if (fish_angle_tail_y >= 10.0f) { fish_angle_tail_y += 0.0f; }
				else { fish_angle_tail_y += 1.0f; }
			
			}

			if (fish_y >= 16.0f) {
				if (fish_angle_head_y >= +10.0f) { fish_angle_head_y -= 0.0f; }
				else { fish_angle_head_y++; }
				//cuerpo de abajo
				if (fish_angle_bot_y <= -15.0f) { fish_angle_bot_y -= 0.0f; }
				else { fish_angle_bot_y -= 1.0f; }
				//cola
				if (fish_angle_tail_y <= -10.0f) { fish_angle_tail_y -= 0.0f; }
				else { fish_angle_tail_y -= 1.0f; }

			}

			if (fish_phi_mov>=6.28f) {
				fish_state = 1;
				fish_pos_ini_x = fish_pos_ini_x + fish_x;
				fish_pos_ini_y = fish_pos_ini_y + fish_y;
				fish_pos_ini_z = fish_pos_ini_z + fish_z;
				std::cout << " px: " << fish_pos_ini_x << " py: " << fish_pos_ini_y << " pz: " << fish_pos_ini_z << std::endl;
				fish_x = 0.0f;
				fish_y = 0.0f;
				fish_z = 0.0f;
				//preparar angulo para siguente estado
				fish_phi_mov = -1.570796327f;
			}

		}

		if (fish_state == 1) {
			fish_z = (5.0f * sin(fish_phi_mov - 0.2458f)) + 5.0f; //rotacion
			fish_y = ((5.0f * sin(-fish_phi_mov)) - 5.0f);
			fish_x = (5.0f * cos(-fish_phi_mov));
			fish_phi_mov -= 0.01f;
			
			fish_theta_y++;
			
			std::cout << "phi: " << fish_phi_mov<< std::endl;
			if (fish_phi_mov <= -3.1416) {
				//fish_state = -1;
				fish_state = 2;
				
				std::cout << " x: " << fish_x << " y: " << fish_y << " z: " << fish_z << std::endl;
				fish_pos_ini_x = fish_pos_ini_x + fish_x;
				fish_pos_ini_y = fish_pos_ini_y + fish_y;
				fish_pos_ini_z = fish_pos_ini_z + fish_z;
				std::cout << " px: " << fish_pos_ini_x << " py: " << fish_pos_ini_y << " pz: " << fish_pos_ini_z << std::endl;
				fish_x = 0.0f;
				fish_y = 0.0f;
				fish_z = 0.0f;
				//preparar angulo para siguente estado
				fish_phi_mov = -3.141592f; //TODO preparar para salir volando
			}
		}

		if (fish_state == 2) {
			//TODO arreglar lo de la posisicion y agregar rotaciones de los ejes al pez
			fish_z = (10.0f * sin(fish_phi_mov - 0.2458f)-2.43332); //rotacion
			fish_y = ((13.0f * sin(-fish_phi_mov)));
			fish_x = (8.0 * cos(-fish_phi_mov) +8.0f);
			std::cout << " x: " << fish_x<< " y: " << fish_y << " z: " << fish_z << std::endl;
			fish_theta_x -= 0.3; //2
			fish_theta_z += 0.2;
			fish_phi_mov -= 0.01f;
			
			if (fish_phi_mov<=-4.7123f) {
				fish_state = 3;
				std::cout << " x: " << fish_x << " y: " << fish_y << " z: " << fish_z << std::endl;
				fish_pos_ini_x = fish_pos_ini_x + fish_x;
				fish_pos_ini_y = fish_pos_ini_y + fish_y;
				fish_pos_ini_z = fish_pos_ini_z + fish_z;
				std::cout << " px: " << fish_pos_ini_x << " py: " << fish_pos_ini_y << " pz: " << fish_pos_ini_z << std::endl;
				fish_x = 0.0f;
				fish_y = 0.0f;
				fish_z = 0.0f;
				fish_phi_mov = 1.570796;
			}

		}
		
		if (fish_state == 3) {
			fish_z = (15.0f * sin(fish_phi_mov) - 15);
			fish_x = (15.0f * cos(fish_phi_mov));
			std::cout << " x: " << fish_x << " y: " << fish_y << " z: " << fish_z << std::endl;
			fish_phi_mov -= 0.01;
			fish_theta_y+=0.7f;
			
			if (fish_phi_mov <= 0.0f) {
				fish_state = 4;
				std::cout << " x: " << fish_x << " y: " << fish_y << " z: " << fish_z << std::endl;
				fish_pos_ini_x = fish_pos_ini_x + fish_x;
				fish_pos_ini_y = fish_pos_ini_y + fish_y;
				fish_pos_ini_z = fish_pos_ini_z + fish_z;
				std::cout << " px: " << fish_pos_ini_x << " py: " << fish_pos_ini_y << " pz: " << fish_pos_ini_z << std::endl;
				fish_x = 0.0f;
				fish_y = 0.0f;
				fish_z = 0.0f;
				fish_phi_mov = -3.14159265f;
			}
		}

		if (fish_state == 4) {
			fish_z = (60.0f * sin(fish_phi_mov));
			fish_x = (30.0f * cos(fish_phi_mov)+30.0f);
			fish_phi_mov+=0.01;
			
			//condiciones cada cuarta parte de la elipse
			if (fish_phi_mov >= -3.14159265f && fish_phi_mov <= -1.5707f) {
				//variar valores
				fish_theta_y--;
				fish_theta_z-=0.2;
				fish_angle_bot_y += 0.2;
				fish_angle_tail_y += 0.2;
				fish_angle_head_y -= 0.2;
			}
			
			if (fish_phi_mov >= -1.5707f && fish_phi_mov <= 0.0f) {
				fish_theta_y-=0.3;
				fish_theta_z -= 0.2;
				fish_angle_bot_y -= 0.2;
				fish_angle_tail_y -= 0.2;
				fish_angle_head_y += 0.2;
			}

			if (fish_phi_mov >= 0.0f && fish_phi_mov <= 1.5707f) {
				fish_theta_y -= 0.3;
				fish_theta_z += 0.3;
				fish_angle_bot_y += 0.2;
				fish_angle_tail_y += 0.2;
				fish_angle_head_y -= 0.2;
			}

			if (fish_phi_mov >= 1.5707f && fish_phi_mov <= 3.14159265f) {
				fish_theta_y -= 0.3;
				fish_theta_z += 0.6;
				fish_theta_x -= 0.6;
				fish_angle_bot_y -= 0.1;
				fish_angle_tail_y -= 0.1;
				fish_angle_head_y += 0.2;
			}

			std::cout << " x: " << fish_x << " y: " << fish_y << " z: " << fish_z << std::endl;
			if (fish_phi_mov>= 1.5707f) {
				fish_y += 0.2; //altura maxima debe ser 33
				if (fish_phi_mov >= 3.1415f) {
					fish_state = 5;
					std::cout << " x: " << fish_x << " y: " << fish_y << " z: " << fish_z << std::endl;
					fish_pos_ini_x = fish_pos_ini_x + fish_x;
					fish_pos_ini_y = fish_pos_ini_y + fish_y;
					fish_pos_ini_z = fish_pos_ini_z + fish_z;
					std::cout << " px: " << fish_pos_ini_x << " py: " << fish_pos_ini_y << " pz: " << fish_pos_ini_z << std::endl;
					fish_x = 0.0f;
					fish_y = 0.0f;
					fish_z = 0.0f;
				}
			}	
		}

		if (fish_state==5) {
			fish_y = fish_y - decremento;
			decremento += 0.005;
			fish_y += 0.2;
			fish_z -= 1.0f;
			
			fish_theta_y -= 0.3;
			fish_theta_z += 1;
			fish_theta_x -= 1;
			fish_angle_bot_y -= 0.3;
			fish_angle_tail_y -= 0.3;
			fish_angle_head_y += 0.2;

			if (fish_y <= (0.0f-fish_pos_ini_y)) {
				fish_state = -1;
				fish_pos_ini_x = fish_pos_ini_x + fish_x;
				fish_pos_ini_y = fish_pos_ini_y + fish_y;
				fish_pos_ini_z = fish_pos_ini_z + fish_z;
				fish_x = 0.0f;
				fish_y = 0.0f;
				fish_z = 0.0f;
			}
		}
	}

	/***************** ANIMACION HELICOPTERO *****************/
	if (activate_hel_animation) {
		if (hel_state == 0) {
			giro_helices += 8.0f;
			helicoptero_y += 3.0f;
			heliceGrande_y += 3.0f;
			heliceChica_y += 3.0f;

			if (helicoptero_y >= 500.0f) {
				hel_state = 1;
			}
		}

		if (hel_state == 1) {
			giro_helices += 8.0f;
			helicoptero_x -= 4.0f;
			heliceGrande_x -= 4.0f;
			heliceChica_x -= 4.0f;

			if (helicoptero_x <= -800.0f) {
				hel_direccion += 3.0f;
				if (hel_direccion == 0.0f) {
					hel_direccion += 0.0f;
					helicoptero_x = -800.0f;
					heliceGrande_x = -800.0f;
					heliceChica_x = -796.5f;
					helicoptero_z = -260.0f;
					heliceGrande_z = -230.0f;
					heliceChica_z = -306.0f;
					hel_state = 2;
				}

			}
		}

		if (hel_state == 2) {
			giro_helices += 8.0f;
			rot_hel = 0.0f;

			helicoptero_z += 4.0f;
			heliceGrande_z += 4.0f;
			heliceChica_z += 4.0f;

			if (helicoptero_z >= 540.0f) {
				hel_direccion += 3.0f;
				if (hel_direccion == 135.0f) {
					helicoptero_x = -800.0f;
					heliceGrande_x = -780.2737f;
					heliceChica_x = -834.552;// -869.957f;
					helicoptero_z = 540.0f;
					heliceGrande_z = 520.2737f;
					heliceChica_z = 570.569f;
					hel_state = 3;
				}
			}
		}

		if (hel_state == 3) {
			giro_helices += 8.0f;
			rot_hel = 135.0f;

			helicoptero_z -= 4.0f;
			heliceGrande_z -= 4.0f;
			heliceChica_z -= 4.0f;
			helicoptero_x += 4.0f;
			heliceGrande_x += 4.0f;
			heliceChica_x += 4.0f;

			if (helicoptero_x >= 0.0f) {
				hel_direccion += 3.0f;
				if (hel_direccion == 270.0f) {

					helicoptero_x = 0.0f,
						helicoptero_z = -260.0f,
						heliceGrande_x = -30.0f,
						heliceGrande_z = -260.0f,
						heliceChica_x = 46.0f,
						heliceChica_z = -256.5f,
						hel_state = 4;
				}
			}
		}
		if (hel_state == 4) {
			giro_helices += 8.0f;
			helicoptero_y -= 3.0f;
			heliceGrande_y -= 3.0f;
			heliceChica_y -= 3.0f;

			if (helicoptero_y <= 0.0f) {
				hel_state = -1;
				giro_helices = 0.0f;
			}
		}
	}

	/***************** ANIMACION PERONSA **********************/
	if (activate_guy_animation) {
		/***********estados de movimiento de extremidades**************/
		if (guy_state_leg == 0) {
			guy_angle_leg -= 1.0f;
			if (guy_angle_leg <= -30.0f) {
				guy_state_leg = 1;
			}

		}

		if (guy_state_leg == 1) {
			guy_angle_leg += 1.0f;
			if (guy_angle_leg >= 30.0f) {
				guy_state_leg = 0;
			}

		}

		if (guy_state_leg == 0) {
			guy_angle_arm -= 1.0f;
			if (guy_angle_leg <= -30.0f) {
				guy_state_leg = 1;
			}

		}

		if (guy_state_leg == 1) {
			guy_angle_arm += 1.0f;
			if (guy_angle_leg >= 30.0f) {
				guy_state_leg = 0;
			}

		}

		if (guy_state_leg == 3) {
			guy_angle_arm = 0.0f;
			guy_angle_leg = 0.0f;
		}
		/***********estados de movimiento total**************/
		if (guy_state == 0) {
			guy_posx -= 0.5f;
			if (guy_posx <= -70.0f) {
				guy_state_leg = 3;
				guy_state = 1;
			}
		}

		if (guy_state == 1) {
			guy_angle_neck -= 1.0f;
			if (guy_angle_neck <= -50.0f) {
				guy_state = 2;
				guy_state_leg = 0;
				guy_pos_ini_x = guy_pos_ini_x + guy_posx;
				guy_pos_ini_z = guy_pos_ini_z + guy_posz;
				guy_posx = 0.0f;
				guy_posz = 0.0f;
			}
		}

		if (guy_state == 2) {
			guy_angle_neck += 1.0f;
			guy_posx -= 0.4;
			guy_posz -= 0.7;
			guy_oriented = -135.0f;
			if (guy_angle_neck >= 0.0f) {
				guy_angle_neck = 0.0f;
			}
			if (guy_posx <= -90.0f) {
				guy_state = 3;
				guy_state_leg = 0;
				guy_pos_ini_x = guy_pos_ini_x + guy_posx;
				guy_pos_ini_z = guy_pos_ini_z + guy_posz;
				guy_posx = 0.0f;
				guy_posz = 0.0f;
			}
		}

		if (guy_state == 3) {
			guy_posx -= 0.5f;
			guy_oriented = -90.0f;
			if (guy_posx <= -100.0f) {
				guy_state = 4;
				guy_state_leg = 0;
				guy_pos_ini_x = guy_pos_ini_x + guy_posx;
				guy_pos_ini_z = guy_pos_ini_z + guy_posz;
				guy_posx = 0.0f;
				guy_posz = 0.0f;
			}
		}

		if (guy_state == 4) {
			guy_posx = 15 * sin(guy_phi);
			guy_posz = 15 * cos(guy_phi) - 15.0f;
			guy_phi -= 0.02;
			guy_oriented -= 1.1;
			if (guy_phi <= -1.5707) {
				guy_state = 5;
			}
		}

		if (guy_state == 5) {
			guy_posx = 15 * sin(guy_phi);
			guy_posz = 15 * cos(guy_phi) - 15.0f;
			guy_phi -= 0.02;
			guy_oriented -= 1.1;
			if (guy_phi <= -1.5707) {
				guy_state = 6;
				guy_state_leg = 0;
				guy_pos_ini_x = guy_pos_ini_x + guy_posx;
				guy_pos_ini_z = guy_pos_ini_z + guy_posz;
				guy_posx = 0.0f;
				guy_posz = 0.0f;

			}
		}

		if (guy_state == 6) {
			guy_posz -= 0.5;
			if (guy_posz <= -50.0f) {
				guy_state = -1;
				guy_state_leg = 3;
				guy_pos_ini_x = guy_pos_ini_x + guy_posx;
				guy_pos_ini_z = guy_pos_ini_z + guy_posz;
				guy_posx = 0.0f;
				guy_posz = 0.0f;
				guy_phi = 0.0f;
			}

		}

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
	glfwInit();
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
	Shader Rain("Shaders/particles_rain.vs", "Shaders/particles_rain.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.bmp",
		"resources/skybox/left.bmp",
		"resources/skybox/top.bmp",
		"resources/skybox/bottom.bmp",
		"resources/skybox/front.bmp",
		"resources/skybox/back.bmp"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// CARGA DE MODELOS
	/******** MODELO SUELO ***********/
	Model suelo("resources/objects/suelo/suelo.obj");

	/******** MODELOS AUTOS ***********/
	Model car_1("resources/objects/coche_azul/Muscle_Car_Pack_2.obj");
	Model car_2("resources/objects/coche_rojo/Muscle_Car_Pack_1.obj");

	/******** MODELO PESCADO ***********/
	Model fishComplete("resources/objects/fish/fish_complete/pez.obj");
	Model fish_head("resources/objects/fish/fish_head/fish_head.obj");
	Model fish_body_1("resources/objects/fish/fish_body1/fish_body1.obj");
	Model fish_body_2("resources/objects/fish/fish_body2/fish_body2.obj");
	Model fish_tail("resources/objects/fish/fish_body3/fish_body3.obj");

	/******** MODELOS CONSTRUCCIONES ***********/
	Model edificio1("resources/objects/edificio1/build3.obj");
	Model edificio2("resources/objects/edificio2/city3.obj");
	Model pilar("resources/objects/pilar/pilar.obj");
	Model pared("resources/objects/pared/pared.obj");
	/************MODELOS DE LA JAULA*************/

	/************MODELO DE ESCULTURA*************/

	/******** MODELO FUENTE ***********/
	Model escultura("resources/objects/escultura/estatua_dinosraurio.obj");
	
	/******** MODELOS ARBOLES ***********/
	Model arbol1("resources/objects/arbol1/arbol1.obj");
	Model arbol2("resources/objects/arbol2/arbol.obj");
	Model arbusto("resources/objects/arbusto/arbusto1.obj");

	/******** MODELOS PIEDRAS ***********/
	Model piedra1("resources/objects/piedra/piedra.obj");
	Model piedra2("resources/objects/piedrita/piedrita.obj");

	/******** MODELO CANCHA ***********/
	Model cancha("resources/objects/cancha/cancha.obj");

	/******** MODELO ALBERCA ***********/
	Model pool("resources/objects/alberca/picina.obj");

	/******** MODELO DINOSAURIO ANIMADO ***********/
	Model dino_brazo_izq("resources/objects/dino/brazo_izq/brazo_izq.obj");
	Model dino_brazo_der("resources/objects/dino/brazo_der/brazo_der.obj");
	Model dino_cola("resources/objects/dino/cola/cola.obj");
	Model dino_cuello("resources/objects/dino/cuello/cuello.obj");
	Model dino_pata_der("resources/objects/dino/pata_der/pata.obj");
	Model dino_pata_izq("resources/objects/dino/pata_izq/pata_izq.obj");
	Model dino_torso("resources/objects/dino/torso/torso.obj");
	Model dino_cabeza_inf("resources/objects/dino/cabeza_inf/cabeza_inf.obj");
	Model dino_cabeza_sup("resources/objects/dino/cabeza_sup/cabeza_sup.obj");
	/********** MODELO DE DINOSAURIOS ********/
	Model dino1("resources/objects/dino_TRI/tricera.obj");
	Model dino2("resources/objects/tricera/dino.obj");
	Model dino3("resources/objects/arlo/arlo.obj");
	Model dino4("resources/objects/Stego/Stegosaurus.obj");
	
	/************MODELO DE JAULA*************/
	Model jaula("resources/objects/dinojaula/jaulaGrande.obj");

	/******** MODELO HELICOPTERO ***********/
	Model helicoptero("resources/objects/helicoptero/helicoptero.obj");
	Model helice_arriba("resources/objects/helice/helice.obj");
	Model helice_tras("resources/objects/helice_tras/helice_tras.obj");
	/******** MODELO MURO DINOPARQUE *********/
	Model puerta("resources/objects/gate/modern_gaste.obj");
	Model gate_muro("resources/objects/gate/gate_p2.obj");
	Model reja("resources/objects/enrejado/enrejado.obj");
	/* MODELO PERSONA*/
	Model guy_head("resources/objects/guy_animation/guy_head.obj");
	Model guy_torso("resources/objects/guy_animation/guy_torso.obj");
	Model guy_brazo_izq("resources/objects/guy_animation/brazo_izq.obj");
	Model guy_brazo_der("resources/objects/guy_animation/brazo_der.obj");
	Model guy_leg_izq("resources/objects/guy_animation/guy_leg_izq.obj");
	Model guy_leg_der("resources/objects/guy_animation/guy_leg_right.obj");

	
	Model soldado("resources/objects/soldado/sold.obj");

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dino_posX = 0;
		KeyFrame[i].dino_posY = 0;
		KeyFrame[i].dino_posZ = 0;
		KeyFrame[i].dino_giro = 0;
	}

	KeyFrame[0].dino_posX = dino_posX;
	KeyFrame[0].dino_posY = dino_posY;
	KeyFrame[0].dino_posZ = dino_posZ;
	KeyFrame[0].n = 4;

	KeyFrame[1].dino_posX = -200.0f;
	KeyFrame[1].dino_posY = 10.0f;
	KeyFrame[1].dino_posZ = -80.0f;
	KeyFrame[1].dino_giro = 0.0f;
	KeyFrame[1].n = 4;

	KeyFrame[2].dino_posX = -180.0f;
	KeyFrame[2].dino_posY = 10.0f;
	KeyFrame[2].dino_posZ = 0.0f;
	KeyFrame[2].dino_giro = 90.0f;
	KeyFrame[2].n = 2;

	KeyFrame[3].dino_posX = 0.0f;
	KeyFrame[3].dino_posY = 10.0f;
	KeyFrame[3].dino_posZ = 0.0f;
	KeyFrame[3].dino_giro = 90.0f;
	KeyFrame[3].n = 4;

	KeyFrame[4].dino_posX = 50.0f;
	KeyFrame[4].dino_posY = 10.0f;
	KeyFrame[4].dino_posZ = 80.0f;
	KeyFrame[4].dino_giro = 0.0f;
	KeyFrame[4].n = 2;

	KeyFrame[5].dino_posX = 50.0f;
	KeyFrame[5].dino_posY = 10.0f;
	KeyFrame[5].dino_posZ = 160.0f;
	KeyFrame[5].dino_giro = 0.0f;
	KeyFrame[5].n = 4;

	KeyFrame[6].dino_posX = 60.0f;
	KeyFrame[6].dino_posY = 10.0f;
	KeyFrame[6].dino_posZ = 200.0f;
	KeyFrame[6].dino_giro = 90.0f;
	KeyFrame[6].n = 2;

	KeyFrame[7].dino_posX = 120.0f;
	KeyFrame[7].dino_posY = 10.0f;
	KeyFrame[7].dino_posZ = 200.0f;
	KeyFrame[7].dino_giro = 90.0f;
	KeyFrame[7].n = 4;
	
	KeyFrame[8].dino_posX = 150.0f;
	KeyFrame[8].dino_posY = 10.0f;
	KeyFrame[8].dino_posZ = 200.0f;
	KeyFrame[8].dino_giro = 180.0f;
	KeyFrame[8].n = 2;
	
	KeyFrame[9].dino_posX = 150.0f;
	KeyFrame[9].dino_posY = 10.0f;
	KeyFrame[9].dino_posZ = 40.0f;
	KeyFrame[9].dino_giro = 180.0f;
	KeyFrame[9].n = 4;

	KeyFrame[10].dino_posX = 110.0f;
	KeyFrame[10].dino_posY = 10.0f;
	KeyFrame[10].dino_posZ = 0.0f;
	KeyFrame[10].dino_giro = 270.0f;
	KeyFrame[10].n = 4;

	KeyFrame[11].dino_posX = -180.0f;
	KeyFrame[11].dino_posY = 10.0f;
	KeyFrame[11].dino_posZ = 0.0f;
	KeyFrame[11].dino_giro = 270.0f;
	KeyFrame[11].n = 4;

	KeyFrame[12].dino_posX = -200.0f;
	KeyFrame[12].dino_posY = 10.0f;
	KeyFrame[12].dino_posZ = -20.0f;
	KeyFrame[12].dino_giro = 180.0f;
	KeyFrame[12].n = 2;
	
	KeyFrame[13].dino_posX = -200.0f;
	KeyFrame[13].dino_posY = 10.0f;
	KeyFrame[13].dino_posZ = -100.0f;
	KeyFrame[13].dino_giro = 180.0f;
	KeyFrame[13].n = 4;

	FrameIndex = 14;

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
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.008f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.32f);
		
		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 0.5f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);
		
		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		//glm::mat4 tmp = glm::mat4(1.0f);
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
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		
		
		/******************* SUELO *******************/
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-80.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(12.5f, 1.0, 10.0));
		staticShader.setMat4("model", model);
		suelo.Draw(staticShader);

		/******************* CANCHA *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(115.0f, -1.72f, 163.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.7f, 4.0f, 4.0f));
		staticShader.setMat4("model", model);
		cancha.Draw(staticShader);

		/******************* EDIFICIO 1 *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(35.0f, -5.0f, -179.0f));
		model = glm::scale(model, glm::vec3(2.0f,2.0f,1.8f));
		staticShader.setMat4("model", model);
		edificio1.Draw(staticShader);

		/******************* EDIFICIO 2 *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, 15.0f, 350.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(9.0f, 10.0f, 9.0f));
		staticShader.setMat4("model", model);
		edificio2.Draw(staticShader);

		/******************* ALBERCA *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(285.0f, 0.0f, 170.0f));
		model = glm::scale(model, glm::vec3(14.9f, 2.0f, 11.8f));
		staticShader.setMat4("model", model);
		pool.Draw(staticShader);

		/******************* ARBOLES Y VEGETACION *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-320.0f, 0.0f, 190.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		staticShader.setMat4("model", model);
		arbol1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-330.0f, 0.0f, 190.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-330.0f, 0.0f, 180.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		staticShader.setMat4("model", model);
		arbusto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-340.0f, 2.0f, 265.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		staticShader.setMat4("model", model);
		piedra2.Draw(staticShader);
		
		/******************* BARDA DEL PARQUE *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, -1.75f, -75.0f));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pilar.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, -1.75f, -107.0f));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, -1.75f, -170.0f));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, -1.75f, -215.0f));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, -1.75f, -255.0f));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, -1.75f, -285.0f));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pilar.Draw(staticShader);

		/******************* PARED FRONTAL *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-75.0f, -1.75f, -75.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-135.0f, -1.75f, -75.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-270.0f, -1.75f, -75.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-330.0f, -1.75f, -75.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);
		
		/******************* PARED TRASERA *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-75.0f, -1.75f, -285.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-135.0f, -1.75f, -285.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-197.0f, -1.75f, -285.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-260.0f, -1.75f, -285.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, -1.75f, -285.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-330.0f, -1.75f, -285.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		/******************* ELEMENTOS DEL PARQUE *******************/
		/******************* Escultura*******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, -1.75f, -180.0f));
		model = glm::scale(model, glm::vec3(6.0));
		staticShader.setMat4("model", model);
		escultura.Draw(staticShader);

		//*************** ARBOL ******************
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, -100.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		arbol2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-320.0f, 0.0f, -100.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		arbol2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-330.0f, 0.0f, -270.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		arbol2.Draw(staticShader);

		/******************* AUTO AZUL *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(car_pos_ini_x + car_x, 5.5f, car_pos_ini_z + car_z));
		model = glm::rotate(model, glm::radians(90.0f + car_direction), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		car_1.Draw(staticShader);

		/******************* AUTO ROJO *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 5.5f, -200.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		car_2.Draw(staticShader);

		/******************* PESCADO *******************/
		/******************* CUERPO PESCADO *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(fish_pos_ini_x, fish_pos_ini_y, fish_pos_ini_z));
		model = glm::translate(model, glm::vec3(fish_x, fish_y, fish_z));
		model = glm::rotate(model, glm::radians(fish_theta_x), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(fish_theta_y), glm::vec3(0.0f, 1.0f, 0.0));
		tmp = model = glm::rotate(model, glm::radians(fish_theta_z), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		fish_body_1.Draw(staticShader);
		/******************* CABEZA PESCADO *******************/
		model = glm::translate(tmp, glm::vec3(3.0f, 0.0, 0.0f));
		model = glm::rotate(model, glm::radians(fish_angle_head_x), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(fish_angle_head_y), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(fish_angle_head_z), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		fish_head.Draw(staticShader);
		//bot
		model = glm::translate(tmp, glm::vec3(-3.0f, 0.0, 0.0f));
		model = glm::rotate(model, glm::radians(fish_angle_bot_x), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(fish_angle_bot_y), glm::vec3(0.0f, 1.0f, 0.0));
		tmp = model = glm::rotate(model, glm::radians(fish_angle_bot_z), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		fish_body_2.Draw(staticShader);
		/******************* COLA PESCADO *******************/
		model = glm::translate(tmp, glm::vec3(-4.0f, 0.0, 0.0f));
		model = glm::rotate(model, glm::radians(fish_angle_tail_x), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(fish_angle_tail_y), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(fish_angle_tail_z), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		fish_tail.Draw(staticShader);

		/******************* DINOSAURIO *******************/
		/******************* TORSO DINO *******************/
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(dino_posX, dino_posY, dino_posZ));
		model = glm::scale(model, glm::vec3(10.0f));
		tmp = model = glm::rotate(model, glm::radians(dino_giro), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		dino_torso.Draw(staticShader);

		/******************* PIERNA DERECHA *******************/
		model = glm::translate(tmp, glm::vec3(-0.2f, 0.1f, 0.45f));
		model = glm::rotate(model, glm::radians(dino_rotPiernaDer), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		dino_pata_der.Draw(staticShader);

		/******************* PIERNA IZQUIERDA *******************/
		model = glm::translate(tmp, glm::vec3(0.1f, 0.1f, 0.42f));
		model = glm::rotate(model, glm::radians(dino_rotPiernaIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		dino_pata_izq.Draw(staticShader);

		/******************* BRAZO DERECHO *******************/
		model = glm::translate(tmp, glm::vec3(-0.01f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		dino_brazo_der.Draw(staticShader);

		/******************* BRAZO IZQUIERDO *******************/
		model = glm::translate(tmp, glm::vec3(0.15f, 0.1f, 1.35f));
		staticShader.setMat4("model", model);
		dino_brazo_izq.Draw(staticShader);

		/******************* COLA DINOSAURIO *******************/
		model = glm::translate(tmp, glm::vec3(0.0f, 0.13f, 0.14f));
		model = glm::rotate(model, glm::radians(dino_rotCola), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		dino_cola.Draw(staticShader);

		/******************* CUELLO DINOSAURIO *******************/
		model = glm::translate(tmp, glm::vec3(0.0f, 0.32f, 1.3f));
		staticShader.setMat4("model", model);
		dino_cuello.Draw(staticShader);

		/******************* QUIJADA DINOSAURIO *******************/
		model = glm::translate(tmp, glm::vec3(0.0f, 0.65f, 1.72f));
		model = glm::rotate(model, glm::radians(dino_rotBoca), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		dino_cabeza_inf.Draw(staticShader);

		/******************* FRENTE DINOSAURIO *******************/
		model = glm::translate(tmp, glm::vec3(0.0f, 0.80f, 1.72f));
		staticShader.setMat4("model", model);
		dino_cabeza_sup.Draw(staticShader);

		/******************* HELICOPTERO *******************/
		/******************* CUERPO HELICOPTERO *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(helicoptero_x + 45.0f, helicoptero_y + 315.0f, helicoptero_z - 180.0f));
		model = glm::rotate(model, glm::radians(hel_direccion), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		staticShader.setMat4("model", model);
		helicoptero.Draw(staticShader);
		
		/******************* HELICE GRANDE *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(heliceGrande_x + 45.0f, heliceGrande_y + 322.0f, heliceGrande_z - 180.0f));
		model = glm::rotate(model, glm::radians(rot_hel), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giro_helices), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7));
		staticShader.setMat4("model", model);
		helice_arriba.Draw(staticShader);
		
		/******************* HELICE CHICA *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(heliceChica_x + 45.0f, heliceChica_y + 319.0f, heliceChica_z -176.5f));
		model = glm::rotate(model, glm::radians(hel_direccion), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giro_helices), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7));
		staticShader.setMat4("model", model);
		helice_tras.Draw(staticShader);
		/******************** DINOSAURIOS: MODEL0 1 ***********************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f, -1.5f, 230.0f));
		model = glm::scale(model, glm::vec3(0.5));
		staticShader.setMat4("model", model);
		dino1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f, -1.5f, 100.0f));
		model = glm::rotate(model, glm::radians(230.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5));
		staticShader.setMat4("model", model);
		dino1.Draw(staticShader);
		/******************** DINOSAURIOS: MODELO 2 ***********************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-430.0f, -1.5f, 230.0f));
		model = glm::scale(model, glm::vec3(0.5));
		staticShader.setMat4("model", model);
		dino2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-430.0f, -1.5f, 150.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5));
		staticShader.setMat4("model", model);
		dino2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-430.0f, -1.5f, -100.0f));
		model = glm::rotate(model, glm::radians(-100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5));
		staticShader.setMat4("model", model);
		dino2.Draw(staticShader);
		/******************** DINOSAURIOS: MODELO 3 ***********************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, -1.5f, -200.0f));
		model = glm::scale(model, glm::vec3(3.0));
		staticShader.setMat4("model", model);
		dino3.Draw(staticShader);

		/******************** DINOSAURIOS: MODELO 4 ***********************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-430.0f, 0.0f, -230.0f));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		dino4.Draw(staticShader);

		/****************** JAULA ********************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-430.0f, -1.5f, -230.0f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0));
		staticShader.setMat4("model", model);
		jaula.Draw(staticShader);
		/**************** ENREJADO *******************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-360.0f, 51.0f, 260.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0));
		staticShader.setMat4("model", model);
		reja.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-360.0f, 51.0f, 215.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0));
		staticShader.setMat4("model", model);
		reja.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-360.0f, 51.0f, 170.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0));
		staticShader.setMat4("model", model);
		reja.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-360.0f, 51.0f, 125.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0));
		staticShader.setMat4("model", model);
		reja.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-360.0f, 51.0f, 80.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0));
		staticShader.setMat4("model", model);
		reja.Draw(staticShader);

		/*segunda parte*/

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-360.0f, 51.0f, -265.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0));
		staticShader.setMat4("model", model);
		reja.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-360.0f, 51.0f, -220.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0));
		staticShader.setMat4("model", model);
		reja.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-360.0f, 51.0f, -175.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0));
		staticShader.setMat4("model", model);
		reja.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-360.0f, 51.0f, -130.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0));
		staticShader.setMat4("model", model);
		reja.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-360.0f, 51.0f, -98.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0));
		staticShader.setMat4("model", model);
		reja.Draw(staticShader);

		/********************** PARED DINOSARURIOS ********************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-360.0f, -1.5f, -10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(3.15f));
		staticShader.setMat4("model", model);
		puerta.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-360.0f, -1.5f, 226.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(3.15f));
		staticShader.setMat4("model", model);
		gate_muro.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-360.0f, -1.5f, -228.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(3.15f));
		staticShader.setMat4("model", model);
		gate_muro.Draw(staticShader);
		/************************* MODELO PERSONA **************************/
		//guy

		model = glm::translate(glm::mat4(1.0f), glm::vec3(guy_pos_ini_x + guy_posx, 11.0f, guy_pos_ini_z + guy_posz));
		tmp = model = glm::rotate(model, glm::radians(guy_oriented), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(6.0f));
		staticShader.setMat4("model", model);
		guy_torso.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.4f, 4.0f, 0.0f));
		model = glm::rotate(model, glm::radians(guy_angle_neck), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(6.0f));
		staticShader.setMat4("model", model);
		guy_head.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(2.1f, 2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(guy_angle_arm), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(6.0f));
		staticShader.setMat4("model", model);
		guy_brazo_izq.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(-2.2f, 2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(-guy_angle_arm), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(6.0f));
		staticShader.setMat4("model", model);
		guy_brazo_der.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(-0.1f, -4.5, 0.0f));
		model = glm::rotate(model, glm::radians(guy_angle_leg), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(6.0f));
		staticShader.setMat4("model", model);
		guy_leg_izq.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(-0.1f, -4.5, 0.0f));
		model = glm::rotate(model, glm::radians(-guy_angle_leg), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(6.0f));
		staticShader.setMat4("model", model);
		guy_leg_der.Draw(staticShader);

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
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		fish_theta_x += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		fish_theta_y += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		fish_theta_z += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		fish_theta_x -= 2.0f;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		fish_theta_y -= 2.0f;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		fish_theta_z -= 2.0f;

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	//variables de contruccion manual
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		car_theta1 += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		car_theta1 -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		car_direction += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		car_direction -= 0.1f;

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		fish_x += 1.1f;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		fish_x -= 1.1f;

	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		thetatest += 1.1f;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		thetatest -= 1.1f;

	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
		fish_theta_x += 0.5f;
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		fish_theta_x -= 0.5f;


	///******************* ANIMACION AUTO *******************/
	//if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	//	animacion ^= true;

	/******************* ANIMACION COMPLEJA AUTO *******************/
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
		activate_car_animation ^= true;// compuerta para cambiar el estado
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS && car_state == -1) {
		car_pos_ini_x = 200.0f;
		car_pos_ini_z = -260.0f;
		car_state = 0;
		activate_car_animation = false;
	}
	
	/******************* ANIMACION COMPLEJA PESCADO *******************/
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
		activate_fish_animation ^= true;// compuerta para cambiar el estado
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS && fish_state == -1) {
		activate_fish_animation = false;
		fish_pos_ini_x = 290.0f;
		fish_pos_ini_y = 1.5f;
		fish_pos_ini_z = 150.0f;
		fish_state = 0;

		fish_theta_x = 0.0f;
		fish_theta_y = -90.0f;
		fish_theta_z = 0.0f;

		aumento = 30.0f;
		decremento = 0.0f;

		fish_angle_head_x = 0.0f;
		fish_angle_head_y = 0.0f;
		fish_angle_head_z = 0.0f;
		fish_angle_bot_x = 0.0f;
		fish_angle_bot_y = 0.0f;
		fish_angle_bot_z = 0.0f;
		fish_angle_tail_x = 0.0f;
		fish_angle_tail_y = 0.0f;
		fish_angle_tail_z = 0.0f;
		fish_phi_mov = 0.0f;

	}
	/*************** ANIMACION PERSONA ******************/

	if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
		activate_guy_animation ^= true;
	}
		
	if (key == GLFW_KEY_F5 && action == GLFW_PRESS && guy_state == -1) {
		guy_angle_leg = 0.0f;
		guy_angle_arm = 0.0f;
		guy_pos_ini_x = 100.0f;
		guy_pos_ini_z = 100.0f;
		guy_posx = 0.0f;
		guy_posz = 0.0f;
		guy_phi = 0.0f;
		guy_oriented = -90.0f;
		guy_angle_neck = 0.0f;
		guy_state_leg = 0;
		guy_state = 0;
		activate_guy_animation = false;
	}

	//******************* ANIMACION COMPLEJA DINOSAURIO *******************/
	if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
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

	//******************* ANIMACION COMPLEJA HELICOPTERO *******************/
	if (key == GLFW_KEY_F4 && action == GLFW_PRESS)
		activate_hel_animation ^= true;// compuerta para cambiar el estado

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