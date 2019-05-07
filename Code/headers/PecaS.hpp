// Garante que ficheiro atual apenas é incluído uma vez durante o processo de compilação
#pragma once 

#include "headers/Peca.hpp"

#define GLEW_STATIC
#include <GL/glew.h>

#include <chrono>
#include <vector>
using namespace std;

// GLM header file
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
using namespace glm;

class PecaS : public Peca
{
	private:
		// Variáveis da peça
		float xCR;
		float yCR;
		int iPieceWidth;
		int iPieceHeight;

		int yPos;
		int xPosE;
		int xPosD;

		// Variáveis de tabuleiro
		int** gameGrid;
		int xPosInicial;
		int yPosInicial;
		int iHeight;
		int iWidth;

		// Variáveis de interacao com user
		int iNumberRotate;
		int iNumberTranslation;
		int iNumberDown;

		// Variáveis associadas a temporizador de colisão, visando melhor jogabilidade
		int oldValueTime;
		bool bCollisionBottom;
		bool bCollisionLeft;
		bool bCollisionRight;
		bool bRotationAllowed;
		int acertoPosicaoY;

		int gameLevel;

		std::chrono::time_point<std::chrono::steady_clock> t_start;

	public:
		PecaS();
		PecaS(int, int, int, int, int**, int);

		void preencheRealVertex(GLfloat, GLfloat);
		void modificaQuadricula(GLfloat, GLfloat);
		void realVertexBuffer();

		bool preencheMatrizCaso0e2(int, int);
		bool preencheMatrizCaso1e3(int, int);
		bool atualizaMatriz();
		bool avaliaPotencialRotacaoCaso0e2(int, int, int);
		bool avaliaPotencialRotacaoCaso1e3(int, int, int);
		bool avaliaColisao();
		void atualizaPos();

		void rotacaoPeca(glm::mat4& rot);
		void translacaoPeca(glm::mat4& trans);

		int collisionYPos();
		void translacaoPecaContorno(glm::mat4& trans);

		// Atualizar queda de peça de acordo com nível de jogo
		int dropAccordingToLevel(double);

		// Getters
		int** getGameGrid();

		int getNumberRotate();
		int getNumberTranslation();
		int getNumberDown();

		int getXPosD();
		int getXPosE();

		bool hasCollidedBottom();
		bool hasCollidedLeft();
		bool hasCollidedRight();
		bool rotationAllowed();

		// Atualizadores
		void incNumberRotate();
		void incNumberTranslation();
		void decNumberTranslation();
		void incNumberDown();

		// Desenho geométrico
		void drawObject();

		// Variáveis de classe
		// Desenho da peça
		static std::vector<GLfloat> g_vertex_buffer_data;

		// Cor da peça
		static std::vector<GLfloat> g_texture_buffer_data;

		static std::vector<GLfloat> g_real_vertex_buffer;

		// Desenho da peça
		static std::vector<GLfloat> g_vertex_buffer_dataPos;

		// Textura da posição de colisão da peça
		static std::vector<GLfloat> g_texture_buffer_dataPos;
};