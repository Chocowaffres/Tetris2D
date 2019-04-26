#include "headers/PecaS.hpp"
#include <iostream>

// Construtores
PecaS::PecaS() {};

PecaS::PecaS(int xPosInicial, int yPosInicial, int iHeight, int iWidth, int** gameGrid) {

	// Vari�veis da pe�a
	xCR = 1.5f;
	yCR = 0.5f;
	iPieceWidth = 3;
	iPieceHeight = 2;

	yPos = yPosInicial;
	xPosE = xPosInicial;
	xPosD = xPosInicial + iPieceWidth;

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
std::vector<GLfloat> PecaS::g_vertex_buffer_data = {
		//S
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
		1.0f,  1.0f,  0.0f,
		2.0f,  1.0f,  0.0f,
		1.0f,  2.0f,  0.0f,

		2.0f,  1.0f,  0.0f,
		1.0f,  2.0f,  0.0f,
		2.0f,  2.0f,  0.0f,

		//
		2.0f,  1.0f,  0.0f,
		3.0f,  1.0f,  0.0f,
		2.0f,  2.0f,  0.0f,

		3.0f,  1.0f,  0.0f,
		2.0f,  2.0f,  0.0f,
		3.0f,  2.0f,  0.0f,
};

// Cor da pe�a
std::vector<GLfloat> PecaS::g_color_buffer_data = {
		//S
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,

		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,

		//
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,

		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		
		//
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,

		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		
		//
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,

		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
};

std::vector<GLfloat> PecaS::g_real_vertex_buffer = {};

// Preenchimento do vertexBuffer de acordo com rota��o da pe�a e local de colis�o, para armazenamento do vertexBuffer
// das pe�as j� jogadas (em Projeto.cpp)
void PecaS::preencheRealVertex(GLfloat x, GLfloat y) {
	g_real_vertex_buffer.push_back(x);
	g_real_vertex_buffer.push_back(y);
	g_real_vertex_buffer.push_back(0.0f);
}

// Modifica��o de um bloco/quadricula da pe�a, repesentado por 2 triangulos
void PecaS::modificaQuadricula(GLfloat x, GLfloat y) {

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
void PecaS::realVertexBuffer() {
	int xPosAntes = xPosE;
	int yPosAntes = yPos;

	switch (iNumberRotate % 4) {
		case 0:

			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);
			break;
		case 1:

			yPos++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			yPos--;
			xPosE++;

			modificaQuadricula(xPosE, yPos);

			yPos--;
			modificaQuadricula(xPosE, yPos);
			break;
		case 2:

			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);
			break;
		case 3:

			yPos++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			yPos--;
			xPosE++;
			modificaQuadricula(xPosE, yPos);

			yPos--;
			modificaQuadricula(xPosE, yPos);
			break;
	}

	xPosE = xPosAntes;
	yPos = yPosAntes;
}

bool PecaS::preencheMatrizCaso0e2(int x, int y) {

	// Andar de baixo
	for (int i = 0; i < iPieceWidth - 1; i++) {
		gameGrid[x + i][y] = 1;
	}

	// Andar de cima

	// Atualiza��o de y
	y = y + 1;

	// Jogo acabou
	if (y > iHeight) {
		return true;
	}

	// Atualiza��o de x
	x = x + 1;

	for (int i = 0; i < iPieceWidth - 1; i++) {
		gameGrid[x + i][y] = 1;
	}
	return false;
}

bool PecaS::preencheMatrizCaso1e3(int x, int y) {

	// Lado esquerdo
	y = y + 1;
	
	// Jogo acabou
	if (y > iHeight) {
		return true;
	}
	for (int i = 0; i < iPieceHeight - 1; i++) {
		gameGrid[x][y + i] = 1;
		// Jogo acabou
		if (y > iHeight) {
			return true;
		}
	}

	// Lado direito

	// Atualiza��o de y
	y = y - 1;
	// Atualiza��o de x
	x = x + 1;

	for (int i = 0; i < iPieceHeight - 1; i++) {
		gameGrid[x][y + i] = 1;
	}
	return false;
}

bool PecaS::atualizaMatriz() {
	switch (iNumberRotate % 4) {
		case 0: return preencheMatrizCaso0e2(xPosE, yPos);
		case 1: return preencheMatrizCaso1e3(xPosE, yPos);
		case 2: return preencheMatrizCaso0e2(xPosE, yPos);
		case 3: return preencheMatrizCaso1e3(xPosE, yPos);
	}
}

bool PecaS::avaliaColisao() {
	// Colis�o com base de jogo
	if (yPos == 0) {
		return true;
	}

	// Colis�o com outras pe�as, tendo em considera��o a rota��o da pe�a
	switch (iNumberRotate % 4) {
		case 0:
			if ((gameGrid[xPosE][yPos - 1] == 1) || (gameGrid[xPosE + 1][yPos - 1] == 1) || (gameGrid[xPosE + 2][yPos] == 1)){
				return true;
			}
			break;
		case 1:
			if ((gameGrid[xPosE][yPos] == 1) || (gameGrid[xPosE + 1][yPos - 1] == 1)){
				return true;
			}
			break;
		case 2:
			if ((gameGrid[xPosE][yPos - 1] == 1) || (gameGrid[xPosE + 1][yPos - 1] == 1) || (gameGrid[xPosE + 2][yPos] == 1)){
				return true;
			}
			break;
		case 3:
			if ((gameGrid[xPosE][yPos] == 1) || (gameGrid[xPosE + 1][yPos - 1] == 1)){
				return true;
			}
			break;
	}
	
	return false;
}

void PecaS::atualizaPos() {
	switch (iNumberRotate % 4) {
		case 0:
			// Tamanho da pe�a
			iPieceWidth = 3;
			iPieceHeight = 2;

			// Ajuste de posi��es
			xPosD = xPosE + iPieceWidth;
	
			if (xPosE < 0) {
				// Garantir que pe�a se mant�m dentro da janela de visualiza��o 
				iNumberTranslation++;
				// Reajustar posi��o da pe�a resultante de ajuste
				xPosE++;
				xPosD++;
			}
			if (xPosD > iWidth) {
				// Garantir que pe�a se mant�m dentro da janela de visualiza��o 
				iNumberTranslation--;
				// Reajustar posi��o da pe�a resultante de ajuste
				xPosE--;
				xPosD--;
			}
			break;

		case 1:
			// Tamanho da pe�a
			iPieceWidth = 2;
			iPieceHeight = 3;

			// Ajuste de posi��es
			xPosE++; // Pe�a avan�a relativamente � posi��o standart da pe�a
			xPosD = xPosE + iPieceWidth;
			yPos--;
		
			break;
		case 2:
			// Tamanho da pe�a
			iPieceWidth = 3;
			iPieceHeight = 2;

			// Ajuste de posi��es
			xPosD = xPosE + iPieceWidth;
			yPos--;

			if (xPosE < 0) {
				// Garantir que pe�a se mant�m dentro da janela de visualiza��o
				iNumberTranslation++;
				// Reajustar posi��o da pe�a resultante de ajuste
				xPosE++;
				xPosD++;
			}
			break;

		case 3:
			// Tamanho da pe�a
			iPieceWidth = 2;
			iPieceHeight = 3;

			// Ajuste de posi��es
			xPosD = xPosE + iPieceWidth;
			yPos--;

			if (xPosE < 0) {
				// Garantir que pe�a se mant�m dentro da janela de visualiza��o
				iNumberTranslation++;
				// Reajustar posi��o da pe�a resultante de ajuste
				xPosE++;
				xPosD++;
			}
			break;
		}
}

// glm::mat4& rot => alternativa a  glm::mat4 *rot aqui, com chamada de fun�ao &rot, ao inves de rot
void PecaS::rotacaoPeca(glm::mat4& rot) {

	// Transladar para origem da pe�a, rodar 90 graus e retornar � sua posi��o original 
	rot = glm::translate(rot, glm::vec3(xCR, yCR, 0.0f));
	rot = glm::rotate(rot, glm::radians(iNumberRotate * -90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	rot = glm::translate(rot, glm::vec3(-xCR, -yCR, 0.0f));

}

void PecaS::translacaoPeca(glm::mat4& trans) {
	auto t_now = std::chrono::high_resolution_clock::now();
	// <int> -> For�ar a que a varia��o de tempo considerada seja a cada segundo
	int time = std::chrono::duration_cast<std::chrono::duration<int>>(t_now - t_start).count();

	// Pe�a desce pelo ecr�, a cada segundo
	yPos = yPosInicial - time - iNumberDown * .5;

	// Trata das transla��es para esquerda e direita ("+" -> direita, "-" -> esquerda)
	xPosE = xPosInicial + iNumberTranslation;

	trans = glm::translate(trans, glm::vec3(xPosE, yPos, 0.0f));

	/* Atualiza posi��es para avalia��o de colis�es, preenchimento de matriz e proxima itera��o de draw
	 (no caso de altera��o relativamente a iNumberTranslation) */
	atualizaPos();
}

void PecaS::drawObject() {
	glDrawArrays(GL_TRIANGLES, 0, 8*3);
}

// Getters
int** PecaS::getGameGrid() {
	return gameGrid;
}

int PecaS::getNumberRotate() {
	return iNumberRotate;
}

int PecaS::getNumberTranslation() {
	return iNumberTranslation;
}

int PecaS::getNumberDown() {
	return iNumberDown;
}

int PecaS::getXPosD() {
	return xPosD;
}

int PecaS::getXPosE() {
	return xPosE;
}

// Atualizadores

void PecaS::incNumberRotate() {
	iNumberRotate++;
}

void PecaS::incNumberTranslation() {
	iNumberTranslation++;
}

void PecaS::decNumberTranslation() {
	iNumberTranslation--;
}

void PecaS::incNumberDown() {
	iNumberDown++;
}
