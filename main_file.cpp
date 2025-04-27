


#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include "myTeapot.h"
#include "objloader.h"
#include "object.h"

float speed = 0;
float speed_x = 0;
float speed_y = 0;
float aspectRatio = 1;

ShaderProgram* sp;

std::vector<glm::vec4> out_vertices_1;
std::vector<glm::vec2> out_uvs_1;
std::vector<glm::vec4> out_normals_1;




Object tlok1 = Object("objects/tlok1.obj");
Object tlok2 = Object("objects/tlok2.obj");
Object tlok3 = Object("objects/tlok3.obj");
Object tlok4 = Object("objects/tlok4.obj");

Object obc1 = Object("objects/obc1.obj");
Object obc2 = Object("objects/obc2.obj");
Object obc3 = Object("objects/obc3.obj");
Object obc4 = Object("objects/obc4.obj");

Object korbowod1 = Object("objects/korbowod1.obj");
Object korbowod2 = Object("objects/korbowod2.obj");
Object korbowod3 = Object("objects/korbowod3.obj");
Object korbowod4 = Object("objects/korbowod4.obj");

Object wal1 = Object("objects/wal1.obj");
Object wal2 = Object("objects/wal2.obj");
Object wal3 = Object("objects/wal3.obj");
Object wal4 = Object("objects/wal4.obj");

Object zawor11 = Object("objects/zawory11.obj");
Object zawor12 = Object("objects/zawory12.obj");
Object zawor21 = Object("objects/zawory21.obj");
Object zawor22 = Object("objects/zawory22.obj");
Object zawor31 = Object("objects/zawory31.obj");
Object zawor32 = Object("objects/zawory32.obj");
Object zawor41 = Object("objects/zawory41.obj");
Object zawor42 = Object("objects/zawory42.obj");

Object gornywal1 = Object("objects/gornywal1.obj");
Object gornywal2 = Object("objects/gornywal2.obj");

GLuint tex0_0;
GLuint tex0_1;
GLuint tex1_0;
GLuint tex1_1;
GLuint tex2_0;
GLuint tex2_1;


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -25.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 0.5f;


float yaw = 90.0f; // Początkowy kąt obrotu w osi Y (w lewo/prawo)
float yaw_speed = 0; // Początkowy kąt obrotu w osi X (w górę/dół)
const float sensitivity = 1.0f; // Czułość obrotu kamery



//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

enum key {
	w, s, a, d, n
}klawisz;


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_RELEASE) {
		float speedMultiplier = (action == GLFW_PRESS) ? 1.0f : 0.0f;
		if (key == GLFW_KEY_LEFT) speed_x = -PI / 2 * speedMultiplier;
		if (key == GLFW_KEY_RIGHT) speed_x = PI / 2 * speedMultiplier;
		if (key == GLFW_KEY_UP) speed_y = PI / 2 * speedMultiplier;
		if (key == GLFW_KEY_DOWN) speed_y = -PI / 2 * speedMultiplier;
		if (key == GLFW_KEY_F) speed = PI / 2 * speedMultiplier;
		if (key == GLFW_KEY_W) klawisz = w;
		if (key == GLFW_KEY_S) klawisz = s;
		if (key == GLFW_KEY_A) klawisz = a;
		if (key == GLFW_KEY_D) klawisz = d;
		if (key == GLFW_KEY_E) yaw_speed = -sensitivity * speedMultiplier;
		if (key == GLFW_KEY_Q) yaw_speed = sensitivity * speedMultiplier;
		if (key == GLFW_KEY_R) {
			cameraPos = glm::vec3(0.0f, 0.0f, -25.0f);
			cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
			cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
			yaw = 90.0f;
		}
		if (action == GLFW_RELEASE)
			klawisz = n;

	}
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}


GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, keyCallback);

	klawisz = n;
	sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
	tex0_0 = readTexture("steel_diffuse_map.png");
	tex0_1 = readTexture("steel_specular_map.png");
	tex1_0 = readTexture("metal_diffuse_map.png");
	tex1_1 = readTexture("metal_specular_map.png");
	tex2_0 = readTexture("steel2_diffuse_map.png");
	tex2_1 = readTexture("steel2_specular_map.png");
}

enum objectType {
	obc = 0,
	tlok = 1,
	korbowod = 2,
	wal = 3,
	zawor = 4,
	gornywal = 5
};


glm::mat4 tloki[4];
glm::mat4 obcs[4];
glm::mat4 korbowody[4];
glm::mat4 waly[4];
glm::mat4 zawory[4][2];
glm::mat4 gornewaly[2];

void draw_objects(float angle_x, float angle_y, float angle, float angleZaworSrodek1, float angleZaworSrodek2, float angleZaworBok1, float angleZaworBok2, float angleGornyWal) {

	tloki[0] = tlok1.draw_object(tlok, 1, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex0_0, tex0_1);
	tloki[1] = tlok2.draw_object(tlok, 2, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex0_0, tex0_1);
	tloki[2] = tlok3.draw_object(tlok, 3, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex0_0, tex0_1);
	tloki[3] = tlok4.draw_object(tlok, 4, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex0_0, tex0_1);

	obcs[0] = obc1.draw_object(obc, 1, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex1_0, tex1_1);
	obcs[1] = obc2.draw_object(obc, 2, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex1_0, tex1_1);
	obcs[2] = obc3.draw_object(obc, 3, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex1_0, tex1_1);
	obcs[3] = obc4.draw_object(obc, 4, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex1_0, tex1_1);

	korbowody[0] = korbowod1.draw_object(korbowod, 1, tloki[0], obcs[0], sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex0_0, tex0_1);
	korbowody[1] = korbowod2.draw_object(korbowod, 2, tloki[1], obcs[1], sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex0_0, tex0_1);
	korbowody[2] = korbowod3.draw_object(korbowod, 3, tloki[2], obcs[2], sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex0_0, tex0_1);
	korbowody[3] = korbowod4.draw_object(korbowod, 4, tloki[3], obcs[3], sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex0_0, tex0_1);


	waly[0] = wal1.draw_object(wal, 1, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex1_0, tex1_1);
	waly[1] = wal2.draw_object(wal, 2, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex1_0, tex1_1);
	waly[2] = wal3.draw_object(wal, 3, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex1_0, tex1_1);
	waly[3] = wal4.draw_object(wal, 4, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, angle, 0, 0, 0, 0, 0, tex1_0, tex1_1);

	zawory[0][0] = zawor11.draw_object(zawor, 1, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, 0, angleZaworSrodek1, angleZaworSrodek2, angleZaworBok1, angleZaworBok2, 0, tex2_0, tex2_1);
	zawory[0][1] = zawor12.draw_object(zawor, 2, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, 0, angleZaworSrodek1, angleZaworSrodek2, angleZaworBok1, angleZaworBok2, 0, tex2_0, tex2_1);
	zawory[1][0] = zawor21.draw_object(zawor, 3, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, 0, angleZaworSrodek1, angleZaworSrodek2, angleZaworBok1, angleZaworBok2, 0, tex2_0, tex2_1);
	zawory[1][1] = zawor22.draw_object(zawor, 4, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, 0, angleZaworSrodek1, angleZaworSrodek2, angleZaworBok1, angleZaworBok2, 0, tex2_0, tex2_1);
	zawory[2][0] = zawor31.draw_object(zawor, 5, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, 0, angleZaworSrodek1, angleZaworSrodek2, angleZaworBok1, angleZaworBok2, 0, tex2_0, tex2_1);
	zawory[2][1] = zawor32.draw_object(zawor, 6, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, 0, angleZaworSrodek1, angleZaworSrodek2, angleZaworBok1, angleZaworBok2, 0, tex2_0, tex2_1);
	zawory[3][0] = zawor41.draw_object(zawor, 7, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, 0, angleZaworSrodek1, angleZaworSrodek2, angleZaworBok1, angleZaworBok2, 0, tex2_0, tex2_1);
	zawory[3][1] = zawor42.draw_object(zawor, 8, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, 0, angleZaworSrodek1, angleZaworSrodek2, angleZaworBok1, angleZaworBok2, 0, tex2_0, tex2_1);

	gornewaly[0] = gornywal1.draw_object(gornywal, 1, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, angle, 0, 0, 0, 0, angleGornyWal, tex1_0, tex1_1);
	gornewaly[1] = gornywal2.draw_object(gornywal, 2, glm::mat4(1.0f), glm::mat4(1.0f), sp, angle_y, angle_x, angle, 0, 0, 0, 0, angleGornyWal, tex1_0, tex1_1);
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {

	delete sp;
}


//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle_x, float angle_y, float angle, float angleZaworSrodek1, float angleZaworSrodek2, float angleZaworBok1, float angleZaworBok2, float angleGornyWal) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glm::vec3 front;
	yaw += yaw_speed;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(0.0f));
	front.y = 0.0f;
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(0.0f));
	cameraFront = glm::normalize(front);

	switch (klawisz) {
	case w:
		cameraPos += cameraSpeed * cameraFront;
		break;
	case s:
		cameraPos -= cameraSpeed * cameraFront;
		break;
	case a:
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	case d:
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	default:
		break;
	}

	glm::mat4 V = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	glm::mat4 P = glm::perspective(glm::radians(50.0f), aspectRatio, 0.1f, 200.0f);

	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, angle_y, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, angle_x, glm::vec3(0.0f, 1.0f, 0.0f));

	sp->use();

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	draw_objects(angle_x, angle_y, angle, angleZaworSrodek1, angleZaworSrodek2, angleZaworBok1, angleZaworBok2, angleGornyWal);



	glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące


	float angle_x = 0;
	float angle_y = 0;

	float angle = 0;

	float angleZaworSrodek1 = 0;
	float angleZaworSrodek2 = 0;
	float angleZaworBok1 = 0;
	float angleZaworBok2 = 0;
	float angleGornyWal = 0;

	int zaworStateSrodek = 0;
	int zaworStateBok = 0;

	glfwSetTime(0); //Zeruj timer

	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{

		angle += speed * glfwGetTime();
		angle_x += speed_x * glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
		angle_y += speed_y * glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki

		angleGornyWal += 0.5 * speed * glfwGetTime();

		if (angleGornyWal > 3.6) {
			angleGornyWal = 0;
		}

		//Sterowanie środkowymi zaworami-------------------
		if (angleZaworSrodek1 > 1.8) {
			angleZaworSrodek1 = 0;
			angle = 0;
			zaworStateSrodek = 1;
		}

		if (angleZaworSrodek2 > 1.8) {
			angleZaworSrodek2 = 0;
			angle = 0;
			zaworStateSrodek = 0;
		}

		if (zaworStateSrodek == 0) {
			angleZaworSrodek1 += speed * glfwGetTime();
		}
		if (zaworStateSrodek == 1) {
			angleZaworSrodek2 += speed * glfwGetTime();
		}

		//-------------------------------------------------
		//Sterowanie boczymi zaworami----------------------
		if (zaworStateBok == 0) {
			angleZaworBok1 += speed * glfwGetTime();
		}

		if (angleZaworBok1 > 0.9) {
			angleZaworBok1 = 0.9;
			zaworStateBok = 1;
		}

		if (angleZaworBok2 < -1.8) {
			angleZaworBok2 = 0;
			zaworStateBok = 2;
		}
		if (angleZaworBok1 < -0.9) {
			angleZaworBok1 = 0.9;
			zaworStateBok = 1;
		}
		if (zaworStateBok == 1) {
			angleZaworBok2 -= speed * glfwGetTime();
		}
		if (zaworStateBok == 2) {
			angleZaworBok1 -= speed * glfwGetTime();
		}

		glfwSetTime(0); //Zeruj timer
		drawScene(window, angle_x, angle_y, angle, angleZaworSrodek1, angleZaworSrodek2, angleZaworBok1, angleZaworBok2, angleGornyWal); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}