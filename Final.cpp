/*
* PROYECTO FINAL
* Integrantes:
* -Aguilar González Oscar
* -Michel Baez
* -Alejandro Hernández Rodríguez

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
//glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightPosition(0.0f, 4.0f, -5.0f);
glm::vec3 lightDirection(-1.0f, 0.0f, 0.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
// 
//variables importantes
float	PI = 3.1416f,
		thetatest = -90.f,
		GRAVITY=9.1;
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		orienta = 0.0f;
bool	animacion = false,
		activate_car_animation=false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;

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

//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;

float myVariable = 0.0f;

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

	//myVariable += 0.1;

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
		/**/
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



			//std::cout << "ANGLE HEAD GOOOOO: " << fish_angle_head_y << std::endl;
			
				

			//std::cout << "X: " << fish_x << "Z: " << fish_z << std::endl;
			//std::cout << fish_phi_mov << std::endl;
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
			
			//std::cout << "PTX: "<< fish_theta_x << "PTY: "<< fish_theta_y << "PTZ: "<< fish_theta_z << std::endl;

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
			//
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
				//preparar angulo para siguente estado
				//fish_phi_mov = -3.14159265f;
				//fish_phi_mov = 4.71238898f;
				fish_phi_mov = 1.570796;
			}

		}
		//implementar salto con gravedad
		/*
		if (fish_state == 3) {
			fish_z = (15.0f * sin(fish_phi_mov)-15);
			fish_x = (15.0f * cos(fish_phi_mov));
			std::cout << " x: " << fish_x << " y: " << fish_y << " z: " << fish_z << std::endl;
		
			fish_phi_mov -= 0.01;
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
				fish_phi_mov = 1.570796;
			
			}


		}
		*/


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
			
			//variables implementar
			//fish_angle_bot_y += 0.2;
			//fish_angle_tail_y += 0.2;
			//fish_angle_head_y -= 0.2;
			
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
			//else {}
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
			//else {}

			std::cout << " x: " << fish_x << " y: " << fish_y << " z: " << fish_z << std::endl;
			if (fish_phi_mov>= 1.5707f) {
				//fish_state = 5;
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
			decremento += 0.009;
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
		 
		//implemetar animacion del pez fuera del agua


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

	Model suelo("resources/objects/suelo/suelo.obj");
	Model car_1("resources/objects/coche_azul/Muscle_Car_Pack_2.obj");
	Model car_2("resources/objects/coche_rojo/Muscle_Car_Pack_1.obj");
<<<<<<< HEAD
	Model car_3("resources/objects/coche_verde/Muscle_Car_Pack_3.obj");
	Model dino_1("resources/objects/Dilophosaurus/dilophosaurus.obj");
=======

	Model fishComplete("resources/objects/fish/fish_complete/pez.obj");

	Model fish_head("resources/objects/fish/fish_head/fish_head.obj");
	Model fish_body_1("resources/objects/fish/fish_body1/fish_body1.obj");
	Model fish_body_2("resources/objects/fish/fish_body2/fish_body2.obj");
	Model fish_tail("resources/objects/fish/fish_body3/fish_body3.obj");
	//Model helice("resources/objects/helice/Helice.obj");
	//Model helice_tras("resources/objects/helice/helice_tras.obj");


	//Model car_3("resources/objects/coche_verde/Muscle_Car_Pack_3.obj");
>>>>>>> main
	//Model car_1("resources/objects/coche_negro/");

	//Model piso("resources/objects/piso/piso.obj");
	/*
	Model botaDer("resources/objects/Personaje/bota.obj");
	Model piernaDer("resources/objects/Personaje/piernader.obj");
	Model piernaIzq("resources/objects/Personaje/piernader.obj");
	Model torso("resources/objects/Personaje/torso.obj");
	Model brazoDer("resources/objects/Personaje/brazoder.obj");
	Model brazoIzq("resources/objects/Personaje/brazoizq.obj");
	Model cabeza("resources/objects/Personaje/cabeza.obj");
	Model carro("resources/objects/lambo/carroceria.obj");
	
	Model casaVieja("resources/objects/casa/OldHouse.obj");
	//Model cubo("resources/objects/cubo/cube02.obj");
	Model casaDoll("resources/objects/casa/DollHouse.obj");*/
	//Model cubito("resources/objects/cubito/cubito.obj");
	//Model llanta("resources/objects/lambo/Wheel.obj");
	

	//Model casabrujas("resources/objects/casabrujas/casabrujas.obj");
	//ModelAnim amy("resources/objects/Amy/amy.dae");
	//amy.initShaders(animShader.ID);
	/*ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	animacionPersonaje.initShaders(animShader.ID);

	ModelAnim ninja("resources/objects/ZombieWalk/ZombieWalk.dae");
	ninja.initShaders(animShader.ID);
	*/
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
		glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
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
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);
		
		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 0.5f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);
		
		staticShader.setVec3("pointLight[2].position", lightPosition);
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 1.0f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.032f);


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
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		
		
		//________________________________________________________________________________________________________
		//____________________________________ SUELO ________________________________________________________
		//________________________________________________________________________________________________________
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		suelo.Draw(staticShader);

		//________________________________________________________________________________________________________
		//____________________________________ ESTATICOS ________________________________________________________
		//________________________________________________________________________________________________________


		//________________________________________________________________________________________________________
		//____________________________________ COCHE AZUL ________________________________________________________
		//________________________________________________________________________________________________________
		model = glm::translate(glm::mat4(1.0f), glm::vec3(car_pos_ini_x + car_x, 5.5f, car_pos_ini_z + car_z));
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(car_x, 5.5f, car_z));
		model = glm::rotate(model, glm::radians(90.0f + car_direction), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		car_1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 5.5f, -200.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		car_2.Draw(staticShader);

<<<<<<< HEAD
		// Dino 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.75f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		dino_1.Draw(staticShader);

		/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
=======
		//________________________________________________________________________________________________________
		//________________________________________ PEZ ___________________________________________________________
		//________________________________________________________________________________________________________
		
		//body
		model = glm::translate(glm::mat4(1.0f), glm::vec3(fish_pos_ini_x, fish_pos_ini_y, fish_pos_ini_z));
		model = glm::translate(model, glm::vec3(fish_x, fish_y, fish_z));
		model = glm::rotate(model, glm::radians(fish_theta_x), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(fish_theta_y), glm::vec3(0.0f, 1.0f, 0.0));
		tmp = model = glm::rotate(model, glm::radians(fish_theta_z), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(2.0f));
>>>>>>> main
		staticShader.setMat4("model", model);
		fish_body_1.Draw(staticShader);
		//head
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
		//tail
		model = glm::translate(tmp, glm::vec3(-4.0f, 0.0, 0.0f));
		model = glm::rotate(model, glm::radians(fish_angle_tail_x), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(fish_angle_tail_y), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(fish_angle_tail_z), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		fish_tail.Draw(staticShader);

		//helice
		/*
	
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.5f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(thetatest), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		helice.Draw(staticShader);

		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.5f, 0.0f));
		model = glm::rotate(model, glm::radians(thetatest), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		helice_tras.Draw(staticShader);
		*/




		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------

		/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 2.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		fish_head.Draw(staticShader);
		*/
		/*
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));//estado inicial
		model = glm::translate(model, glm::vec3(15.0f + movAuto_x, -1.0f, movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq trase
		*/

		

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		torso.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaDer.Draw(staticShader);

		//Pie Der
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		botaDer.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaIzq.Draw(staticShader);

		//Pie Iz
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		botaDer.Draw(staticShader);	//Izq trase

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoDer.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoIzq.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", model);
		cabeza.Draw(staticShader);

		//mi cubito
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 20.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		cubito.Draw(staticShader);
		*/
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
	/*
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		fish_theta_x +=2.0f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		fish_theta_y += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		fish_theta_z += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		fish_theta_x -= 2.0f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		fish_theta_y -= 2.0f;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		fish_theta_z -= 2.0f;
	*/
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

	/*
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		fish_angle_head_x++;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		fish_angle_head_y++;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		fish_angle_head_z++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		fish_angle_head_x--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		fish_angle_head_y--;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		fish_angle_head_z--;
	*/
	
	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
		thetatest--;
	if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
		thetatest++;

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


	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	//animacion compleja de auto
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
		activate_car_animation ^= true;// compuerta para cambiar el estado
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS && car_state == -1) {
		car_pos_ini_x = 200.0f;
		car_pos_ini_z = -260.0f;
		car_state = 0;
		activate_car_animation = false;
	}
	// animacion compleja del Pez
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