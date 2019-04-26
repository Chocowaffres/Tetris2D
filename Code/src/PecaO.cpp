#include "headers/PecaO.hpp"
#include <iostream>

// Construtores
PecaO::PecaO() {};

PecaO::PecaO(int xPosInicial, int yPosInicial, int iHeight, int iWidth, int** gameGrid) {

	// Vari�veis da pe�a
	xCR = 1.0f;
	yCR = 1.0f;
	iPieceWidth = 2;
	iPieceHeight = 2;

	yPos = yPosInicial;
	xPosE = xPosInicial;
	xPosD = xPosInicial + iPieceWidth;

	// Vari�veis de tabuleiro
	this->xPosInicial = xPosInicial;
	this->yPosInicial = yPosInicial;
	this->iHeight = iHeight;
	this->iWidth = iWidth;

	// Vari�veis de tabuleiro
	this->xPosInicial = xPosInicial;
	this->yPosInicial = yPosInicial;
	this->iHeight = iHeight;
	this->iWidth = iWidth;

	this->gameGrid = (int**)calloc(iWidth, sizeof(int*));
	for (int i = 0; i < iWidth; i++) {
		this->gameGrid[i] = (int*)calloc(iHeight, sizeof(int));
	}

	for (int i = 0; i < iWidth; i++) {
		for (int j = 0; j < iHeight; j++) {
			this->gameGrid[i][j] = gameGrid[i][j];
		}
	}

	// Vari�veis de interacao com user
	iNumberRotate = 0;
	iNumberTranslation = 0;
	iNumberDown = 0;

	t_start = std::chrono::high_resolution_clock::now();

	g_real_vertex_buffer = {};
};

// Vari�veis de classe
// Desenho da pe�a
std::vector<GLfloat> PecaO::g_vertex_buffer_data = {
		//O
		0.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		0.0f,  1.0f,  0.0f,

		1.0f,  0.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,

		//
		1.0f,  0.0f,  0.0f,
		2.0f,  0.0f,  0.0f,
		1.0f,  1.0f,  0.0f,

		2.0f,  0.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		2.0f,  1.0f,  0.0f,

		//
		0.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		0.0f,  2.0f,  0.0f,

		1.0f,  1.0f,  0.0f,
		0.0f,  2.0f,  0.0f,
		1.0f,  2.0f,  0.0f,

		//
		1.0f,  1.0f,  0.0f,
		2.0f,  1.0f,  0.0f,
		1.0f,  2.0f,  0.0f,

		2.0f,  1.0f,  0.0f,
		1.0f,  2.0f,  0.0f,
		2.0f,  2.0f,  0.0f,
};

// Cor da pe�a
std::vector<GLfloat> PecaO::g_color_buffer_data = {
		//O
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,

		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,

		//
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,

		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		
		//
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,

		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		
		//
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,

		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
};

std::vector<GLfloat> PecaO::g_real_vertex_buffer = {};

// Preenchimento do vertexBuffer de acordo com rota��o da pe�a e local de colis�o, para armazenamento do vertexBuffer
// das pe�as j� jogadas (em Projeto.cpp)
void PecaO::preencheRealVertex(GLfloat x, GLfloat y) {
	g_real_vertex_buffer.push_back(x);
	g_real_vertex_buffer.push_back(y);
	g_real_vertex_buffer.push_back(0.0f);
}

// Modifica��o de um bloco/quadricula da pe�a, repesentado por 2 triangulos
void PecaO::modificaQuadricula(GLfloat x, GLfloat y) {

	// 3 v�rtice do triangulo inferior da quadricula
	preencheRealVertex(x, y);
	preencheRealVertex(x + 1, y);
	preencheRealVertex(x, y + 1);

	// 3 v�rtice do triangulo superior da quadricula
	preencheRealVertex(x + 1, y);
	preencheRealVertex(x, y + 1);
	preencheRealVertex(x + 1, y + 1);
}

// Pe�a quando desenhada est� na origem. A colis�o de pe�a, promove a invoca��o deste m�todo que tratar� de representar a pe�a,
// sob a forma de vertexBuffer, tendo em conta local onde esta colidiu e a rota��o da pe�a.
void PecaO::realVertexBuffer() {
	int xPosAntes = xPosE;
	int yPosAntes = yPos;

	// N�o necessita de considerar rota��es neste caso, devido � natureza da pe�a (O, um quadrado)
	modificaQuadricula(xPosE, yPos);

	xPosE++;
	modificaQuadricula(xPosE, yPos);

	yPos++;
	modificaQuadricula(xPosE, yPos);

	xPosE--;
	modificaQuadricula(xPosE, yPos);

	// Reposi��o de valores de vari�veis
	xPosE = xPosAntes;
	yPos = yPosAntes;
}

bool PecaO::preencheMatriz(int x, int y) {
	// Altura
	for (int i = 0; i < iPieceHeight; i++) {
		// Jogo acabou
		if (y + i > iHeight) {
			return true;
		}
		// Largura
		for (int j = 0; j < iPieceWidth; j++) {
			gameGrid[x+j][y+i] = 1;
		}
	}

	return false;
}

bool PecaO::atualizaMatriz() {
	return preencheMatriz(xPosE, yPos);
}

bool PecaO::avaliaColisao() {
	// Colis�o com base de jogo
	if (yPos == 0) {
		return true;
	}

	// Colis�o com outras pe�as
	if ((gameGrid[xPosE][yPos - 1] == 1) || (gameGrid[xPosE + 1][yPos - 1] == 1 || (gameGrid[xPosE][yPos] == 1))){
		return true;
	}

	return false;
}

void PecaO::atualizaPos() {

	// Todas as rota��es t�m o mesmo aspecto logo n�o � preciso distinguir entre os diferentes estados de rota��o poss�veis
		
	// Atualiza��o xPosD para garantir que pe�a n�o sai da janela de visualiza��o
	xPosD = xPosE + iPieceWidth;
}

// glm::mat4& rot => alternativa a  glm::mat4 *rot aqui, com chamada de fun�ao &rot, ao inves de rot
void PecaO::rotacaoPeca(glm::mat4& rot) {

	// Transladar para origem da pe�a, rodar 90 graus e retornar � sua posi��o original 
	rot = glm::translate(rot, glm::vec3(xCR, yCR, 0.0f));
	rot = glm::rotate(rot, glm::radians(iNumberRotate * -90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	rot = glm::translate(rot, glm::vec3(-xCR, -yCR, 0.0f));

}

void PecaO::translacaoPeca(glm::mat4& trans) {
	auto t_now = std::chrono::high_resolution_clock::now();
	// <int> -> For�ar a que a varia��o de tempo considerada seja a cada segundo
	int time = std::chrono::duration_cast<std::chrono::duration<int>>(t_now - t_start).count();

	// Pe�a desce pelo ecr�, a cada segundo
	yPos = yPosInicial - time - iNumberDown * .5;
	//cout << "yPos: " << yPos << ", yPosAjuste: " << yPosAjuste << endl;

	// Trata das transla��es para esquerda e direita ("+" -> direita, "-" -> esquerda)
	xPosE = xPosInicial + iNumberTranslation;

	trans = glm::translate(trans, glm::vec3(xPosE, yPos, 0.0f));

	/* Atualiza posi��es para avalia��o de colis�es, preenchimento de matriz e proxima itera��o de draw
	 (no caso de altera��o relativamente a iNumberTranslation) */
	atualizaPos();
}

void PecaO::drawObject() {
	glDrawArrays(GL_TRIANGLES, 0, 8*3);
}

// Getters
int** PecaO::getGameGrid() {
	return gameGrid;
}

int PecaO::getNumberRotate() {
	return iNumberRotate;
}

int PecaO::getNumberTranslation() {
	return iNumberTranslation;
}

int PecaO::getNumberDown() {
	return iNumberDown;
}

int PecaO::getXPosD() {
	return xPosD;
}

int PecaO::getXPosE() {
	return xPosE;
}

// Atualizadores

void PecaO::incNumberRotate() {
	iNumberRotate++;
}

void PecaO::incNumberTranslation() {
	iNumberTranslation++;
}

void PecaO::decNumberTranslation() {
	iNumberTranslation--;
}

void PecaO::incNumberDown() {
	iNumberDown++;
}
