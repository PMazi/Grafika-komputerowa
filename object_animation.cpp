// Object.cpp

#include "object.h"
#include "constants.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>



Object::Object(const char* file) : filename(file) {
	
	loadOBJ(file, vertices, textures, normals);
}


glm::mat4 Object::draw_object(int objectType, int number, glm::mat4 matrix1, glm::mat4 matrix2, ShaderProgram* sp, float angle_y, float angle_x, float angle, float zaworAngle1, float zaworAngle2, float angleZaworBok1, float angleZaworBok2, float angleGornyWal, GLuint tex0, GLuint tex1) {


	enum objecttype {
		obc = 0,
		tlok = 1,
		korbowod = 2,
		wal = 3,
		zawor = 4,
		gornywal = 5
	};
	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glUniform1i(sp->u("textureMap1"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);


	glm::mat4 M_base = glm::mat4(1.0f);
	M_base = translate(M_base, glm::vec3(0.0f, -5.0f, 5.0f));
	M_base = glm::rotate(M_base, angle_y, glm::vec3(1.0f, 0.0f, 0.0f));
	M_base = glm::rotate(M_base, angle_x, glm::vec3(0.0f, 1.0f, 0.0f));
	M_base = translate(M_base, glm::vec3(0.0f, 0.0f, -8.0f));

	if (objectType == obc) {
		M_base = rotate(M_base, 3.5f * angle, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	if (objectType == tlok) {
		float yCube = cos(angle * 3.5f);
		if (number == 2 || number == 3) {
			M_base = translate(M_base, glm::vec3(0.0f, 1.4f * yCube, 0.0f));
			M_base = translate(M_base, glm::vec3(0.0f, -1.3f, 0.0f));
		}
		else {
			M_base = translate(M_base, glm::vec3(0.0f, 1.4f * -yCube, 0.0f));
			M_base = translate(M_base, glm::vec3(0.0f, 1.3f, 0.0f));
		}
	}


	if (objectType == korbowod) {

		M_base = translate(matrix1, glm::vec3(0.0f, 0.0f, 0.0f));
		M_base = translate(M_base, glm::vec3(0.0f, 2.8f, 0.0f));
		if (number == 1 || number == 4) {
			M_base = translate(M_base, glm::vec3(-0.05f, 0.0f, 0.0f));
			M_base = rotate(M_base, 0.237f * sin(angle * 3.5f), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else {
			M_base = translate(M_base, glm::vec3(0.05f, 0.0f, 0.0f));
			M_base = translate(M_base, glm::vec3(0.0f, 1.9f, 0.0f));
			M_base = rotate(M_base, 0.267f * -sin(angle * 3.5f), glm::vec3(0.0f, 0.0f, 1.0f));
			M_base = translate(M_base, glm::vec3(0.0f, -2.0f, 0.0f));
		}
		M_base = translate(M_base, glm::vec3(0.0f, -4.3f, 0.0f));
	}

	if (objectType == wal) {
		M_base = rotate(M_base, 3.5f * angle, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	if (objectType == zawor) {

		if (number == 2 || number == 7) {
			M_base = translate(M_base, glm::vec3(0.0f, 0.38f, 0.0f));
		}
		else {
			M_base = translate(M_base, glm::vec3(0.0f, -0.32f, 0.0f));
		}

		float yWal;
		if (number == 3 || number == 4 || number == 5 || number == 6) { //środkowe tłoki

			if (number == 3) {
				yWal = cos(zaworAngle2 * 3.5);
				yWal = 0.3 * yWal;
			}
			if (number == 4) {
				yWal = cos(zaworAngle1 * 3.5);
				yWal = 0.3 * yWal;
			}
			if (number == 5) {
				yWal = cos(zaworAngle1 * 3.5);
				yWal = 0.3 * yWal;
			}
			if (number == 6) {
				yWal = cos(zaworAngle2 * 3.5);
				yWal = 0.3 * yWal;
			}
			M_base = translate(M_base, glm::vec3(0.0f, yWal, 0.0f));
		}
		if (number == 1 || number == 2 || number == 7 || number == 8){ //boczne tloki

			if (number == 1) {
				yWal = cos(angleZaworBok2 * 3.5);
				yWal = 0.3 * yWal;
			}
			if (number == 2) {
				yWal = -cos(angleZaworBok1 * 3.5);
				yWal = 0.3 * yWal;
			}
			if (number == 7) {
				yWal = -cos(angleZaworBok1 * 3.5);
				yWal = 0.3 * yWal;
			}
			if (number == 8) {
				yWal = cos(angleZaworBok2 * 3.5);
				yWal = 0.3 * yWal;
				}

			M_base = translate(M_base, glm::vec3(0.0f, yWal, 0.0f));
			}
				if (number % 2 == 0) {
					M_base = translate(M_base, glm::vec3(0.5f * yWal, 0.0f, 0.0f));
				}
				else {
					M_base = translate(M_base, glm::vec3(0.5f * -yWal, 0.0f, 0.0f));
				}
	}




	if (objectType == gornywal) {

		//Umieszczenie obiektu w odpowiednim miejscu
		if (number == 1) {
			M_base = translate(M_base, glm::vec3(-1.6f, 11.3f, 0.0f));
		}
		else {
			M_base = translate(M_base, glm::vec3(1.6f, 11.3f, 0.0f));
		}

		M_base = rotate(M_base, 3.5f * angleGornyWal, glm::vec3(0.0f, 0.0f, 1.0f));

		//Wyrównanie osi obrotu
		if (number == 1) {
			M_base = rotate(M_base, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			M_base = translate(M_base, glm::vec3(2.0f, -11.4f, 0.0f));

		}
		else {
			M_base = rotate(M_base, glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			M_base = translate(M_base, glm::vec3(-2.0f, -11.4f, 0.0f));
		}
	}

	M_base = translate(M_base, glm::vec3(0.0f, 1.5f, 0.0f));

	glm::mat4 copyM_base = M_base;


	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M_base));
	glm::vec3 lightPos1(0.0f, 0.0f, -10.0f);
	glm::vec3 lightPos2(-10.0f, 0.0f, -10.0f); //pozycja źródła światła 1
	glUniform3fv(sp->u("lightPos1"), 1, glm::value_ptr(lightPos1));
	glUniform3fv(sp->u("lightPos2"), 1, glm::value_ptr(lightPos2));

	glEnableVertexAttribArray(sp->a("vertex"));  //W��cz przesy�anie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, static_cast<float*>(glm::value_ptr(vertices.front())));


	glEnableVertexAttribArray(sp->a("normal"));  //W��cz przesy�anie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, static_cast<float*>(glm::value_ptr(normals.front())));

	glEnableVertexAttribArray(sp->a("texCoord0"));  //W��cz przesy�anie danych do atrybutu texCoord
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, static_cast<float*>(glm::value_ptr(textures.front())));

	glDrawArrays(GL_TRIANGLES, 0, vertices.size()); //Narysuj obiekt


	return copyM_base;
}


