#include "headers/PecaO.hpp"
#include <iostream>

// Construtores
PecaO::PecaO() {};

PecaO::PecaO(int xPosInicial, int yPosInicial, int iHeight, int iWidth, int** gameGrid) {

	// Variáveis da peça
	xCR = 1.0f;
	yCR = 1.0f;
	iPieceWidth = 2;
	iPieceHeight = 2;

	yPos = yPosInicial;
	xPosE = xPosInicial;
	xPosD = xPosInicial + iPieceWidth;

	// Variáveis de tabuleiro
	this->xPosInicial = xPosInicial;
	this->yPosInicial = yPosInicial;
	this->iHeight = iHeight;
	this->iWidth = iWidth;

	// Variáveis de tabuleiro
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

	// Variáveis de interacao com user
	iNumberRotate = 0;
	iNumberTranslation = 0;
	iNumberDown = 0;

	t_start = std::chrono::high_resolution_clock::now();

	g_real_vertex_buffer = {};
};

// Variáveis de classe
// Desenho da peça
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

// Cor da peça
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

// Preenchimento do vertexBuffer de acordo com rotação da peça e local de colisão, para armazenamento do vertexBuffer
// das peças já jogadas (em Projeto.cpp)
void PecaO::preencheRealVertex(GLfloat x, GLfloat y) {
	g_real_vertex_buffer.push_back(x);
	g_real_vertex_buffer.push_back(y);
	g_real_vertex_buffer.push_back(0.0f);
}

// Modificação de um bloco/quadricula da peça, repesentado por 2 triangulos
void PecaO::modificaQuadricula(GLfloat x, GLfloat y) {

	// 3 vértice do triangulo inferior da quadricula
	preencheRealVertex(x, y);
	preencheRealVertex(x + 1, y);
	preencheRealVertex(x, y + 1);

	// 3 vértice do triangulo superior da quadricula
	preencheRealVertex(x + 1, y);
	preencheRealVertex(x, y + 1);
	preencheRealVertex(x + 1, y + 1);
}

// Peça quando desenhada está na origem. A colisão de peça, promove a invocação deste método que tratará de representar a peça,
// sob a forma de vertexBuffer, tendo em conta local onde esta colidiu e a rotação da peça.
void PecaO::realVertexBuffer() {
	int xPosAntes = xPosE;
	int yPosAntes = yPos;

	// Não necessita de considerar rotações neste caso, devido à natureza da peça (O, um quadrado)
	modificaQuadricula(xPosE, yPos);

	xPosE++;
	modificaQuadricula(xPosE, yPos);

	yPos++;
	modificaQuadricula(xPosE, yPos);

	xPosE--;
	modificaQuadricula(xPosE, yPos);

	// Reposição de valores de variáveis
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
	// Colisão com base de jogo
	if (yPos == 0) {
		return true;
	}

	// Colisão com outras peças
	if ((gameGrid[xPosE][yPos - 1] == 1) || (gameGrid[xPosE + 1][yPos - 1] == 1 || (gameGrid[xPosE][yPos] == 1))){
		return true;
	}

	return false;
}

void PecaO::atualizaPos() {

	// Todas as rotações têm o mesmo aspecto logo não é preciso distinguir entre os diferentes estados de rotação possíveis
		
	// Atualização xPosD para garantir que peça não sai da janela de visualização
	xPosD = xPosE + iPieceWidth;
}

// glm::mat4& rot => alternativa a  glm::mat4 *rot aqui, com chamada de funçao &rot, ao inves de rot
void PecaO::rotacaoPeca(glm::mat4& rot) {

	// Transladar para origem da peça, rodar 90 graus e retornar à sua posição original 
	rot = glm::translate(rot, glm::vec3(xCR, yCR, 0.0f));
	rot = glm::rotate(rot, glm::radians(iNumberRotate * -90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	rot = glm::translate(rot, glm::vec3(-xCR, -yCR, 0.0f));

}

void PecaO::translacaoPeca(glm::mat4& trans) {
	auto t_now = std::chrono::high_resolution_clock::now();
	// <int> -> Forçar a que a variação de tempo considerada seja a cada segundo
	int time = std::chrono::duration_cast<std::chrono::duration<int>>(t_now - t_start).count();

	// Peça desce pelo ecrã, a cada segundo
	yPos = yPosInicial - time - iNumberDown * .5;
	//cout << "yPos: " << yPos << ", yPosAjuste: " << yPosAjuste << endl;

	// Trata das translações para esquerda e direita ("+" -> direita, "-" -> esquerda)
	xPosE = xPosInicial + iNumberTranslation;

	trans = glm::translate(trans, glm::vec3(xPosE, yPos, 0.0f));

	/* Atualiza posições para avaliação de colisões, preenchimento de matriz e proxima iteração de draw
	 (no caso de alteração relativamente a iNumberTranslation) */
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
