/////////////////////////////////////////////
// Developed by Tiago Roxo and Joana Costa //
/////////////////////////////////////////////

#pragma once

#include "headers/Peca.hpp"

#define GLEW_STATIC
#include <GL/glew.h>

#include <chrono>
#include <vector>

// GLM header file
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Peca
{
	public:
		Peca() {};
		virtual ~Peca() {};

	virtual void preencheRealVertex(GLfloat, GLfloat) = 0;
	virtual void modificaQuadricula(GLfloat, GLfloat) = 0;
	virtual void realVertexBuffer() = 0;

	virtual bool atualizaMatriz() = 0;
	virtual bool avaliaColisao() = 0;
	virtual void atualizaPos() = 0;

	virtual void rotacaoPeca(glm::mat4& rot) = 0;
	virtual void translacaoPeca(glm::mat4& trans) = 0;
	virtual void translacaoPecaContorno(glm::mat4& trans) = 0;

	virtual int dropAccordingToLevel(double) = 0;

	virtual int collisionYPos() = 0;

	// Getters
	virtual int** getGameGrid() = 0;

	virtual int getNumberRotate() = 0;
	virtual int getNumberTranslation() = 0;
	virtual int getNumberDown() = 0;

	virtual int getXPosD() = 0;
	virtual int getXPosE() = 0;

	virtual bool hasCollidedBottom() = 0;
	virtual bool hasCollidedLeft() = 0;
	virtual bool hasCollidedRight() = 0;
	virtual bool rotationAllowed() = 0;

	// Atualizadores
	virtual void incNumberRotate() = 0;
	virtual void incNumberTranslation() = 0;
	virtual void decNumberTranslation() = 0;
	virtual void incNumberDown() = 0;

	// Desenho geométrico
	virtual void drawObject() = 0;

	// Variáveis de classe
	// Desenho da peça
	static std::vector<GLfloat> g_vertex_buffer_data;

	// Cor da peça
	static std::vector<GLfloat> g_texture_buffer_data;

	static std::vector<GLfloat> g_real_vertex_buffer;

};
