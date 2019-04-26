#include "headers/PecaI.hpp"
#include <iostream>

// Construtores
PecaI::PecaI() {};

PecaI::PecaI(int xPosInicial, int yPosInicial, int iHeight, int iWidth, int** gameGrid) {

	// Variáveis da peça
	xCR = 2.0f;
	yCR = 1.0f;
	iPieceWidth = 4;
	iPieceHeight = 1;

	yPos = yPosInicial;
	xPosE = xPosInicial;
	xPosD = xPosInicial + iPieceWidth;

	yPosAjuste = 0;

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

	// Ajuste de rotação
	//yPosAjuste = 0;

	// Variáveis de interacao com user
	iNumberRotate = 0;
	iNumberTranslation = 0;
	iNumberDown = 0;

	t_start = std::chrono::high_resolution_clock::now();

	g_real_vertex_buffer = {};
};

// Variáveis de classe
// Desenho da peça
std::vector<GLfloat> PecaI::g_vertex_buffer_data = {
		//I
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
		2.0f,  0.0f,  0.0f,
		3.0f,  0.0f,  0.0f,
		2.0f,  1.0f,  0.0f,

		3.0f,  0.0f,  0.0f,
		2.0f,  1.0f,  0.0f,
		3.0f,  1.0f,  0.0f,

		//
		3.0f,  0.0f,  0.0f,
		4.0f,  0.0f,  0.0f,
		3.0f,  1.0f,  0.0f,

		4.0f,  0.0f,  0.0f,
		3.0f,  1.0f,  0.0f,
		4.0f,  1.0f,  0.0f,
};

// Cor da peça
std::vector<GLfloat> PecaI::g_color_buffer_data = {
		//I
		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,

		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,
		
		//
		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,

		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,
		
		//
		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,

		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,
		
		//
		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,

		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,
		0.0f,  0.8f,  0.8f,
};

std::vector<GLfloat> PecaI::g_real_vertex_buffer = {};

// Preenchimento do vertexBuffer de acordo com rotação da peça e local de colisão, para armazenamento do vertexBuffer
// das peças já jogadas (em Projeto.cpp)
void PecaI::preencheRealVertex(GLfloat x, GLfloat y) {
	g_real_vertex_buffer.push_back(x);
	g_real_vertex_buffer.push_back(y);
	g_real_vertex_buffer.push_back(0.0f);
}

// Modificação de um bloco/quadricula da peça, repesentado por 2 triangulos
void PecaI::modificaQuadricula(GLfloat x, GLfloat y) {

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
void PecaI::realVertexBuffer() {
	int xPosAntes = xPosE;
	int yPosAntes = yPos;

	switch (iNumberRotate % 4) {
		case 0:

			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);
			break;
		case 1:

			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);
			break;
		case 2:

			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);
			break;
		case 3:

			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);
			break;
	}

	// Reposição de valores de variáveis
	xPosE = xPosAntes;
	yPos = yPosAntes;
}

bool PecaI::preencheMatriz(int x, int y) {
	// Altura
	for (int i = 0; i < iPieceHeight; i++) {
		// Jogo acabou
		if (y + i > iHeight) {
			return true;
		}
		gameGrid[x][y + i] = 1;
	}
	// Largura
	for (int i = xPosE; i < xPosD; i++) {
		gameGrid[i][y] = 1;
	}

	return false;
}

bool PecaI::atualizaMatriz() {
	return preencheMatriz(xPosE, yPos);
}

bool PecaI::avaliaColisao() {
	// Colisão com base de jogo
	if (yPos == 0) {
		return true;
	}

	// Colisão com outras peças, tendo em consideração a rotação da peça
	switch (iNumberRotate % 4) {
		case 0:
			if ((gameGrid[xPosE][yPos - 1] == 1) || (gameGrid[xPosE + 1][yPos - 1] == 1) || (gameGrid[xPosE + 2][yPos - 1] == 1) || (gameGrid[xPosE + 3][yPos - 1] == 1)){
				return true;
			}
			break;
		case 1:
			if ((gameGrid[xPosE][yPos - 1] == 1)){
				return true;
			}
			break;
		case 2:
			if ((gameGrid[xPosE][yPos - 1] == 1) || (gameGrid[xPosE + 1][yPos - 1] == 1) || (gameGrid[xPosE + 2][yPos - 1] == 1) || (gameGrid[xPosE + 3][yPos - 1] == 1)){
				return true;
			}
			break;
		case 3:
			if ((gameGrid[xPosE][yPos - 1] == 1)){
				return true;
			}
			break;
	}
	return false;
}

void PecaI::atualizaPos() {
	// Posicao inicial
	switch (iNumberRotate % 4) {
		case 0:
			// Tamanho da peça
			iPieceWidth = 4;
			iPieceHeight = 1;

			// Ajuste de posições
			xPosD = xPosE + iPieceWidth;

			// Rotação junto da parede do lado esquerdo
			if (xPosE < 0) {
				// Garantir que peça se mantém dentro da janela de visualização
				iNumberTranslation += 2;
				// Reajustar posição da peça resultante de ajuste
				xPosE += 2;
				xPosD +=2; 
			}

			// Rotação junto da parede do lado direito
			if (xPosD > iWidth) {
				// Garantir que peça se mantém dentro da janela de visualização
				iNumberTranslation--;
				// Reajustar posição da peça resultante de ajuste
				xPosE--;
				xPosD--;
			}

			break;

		case 1:
			// Tamanho da peça
			iPieceWidth = 1;
			iPieceHeight = 4;

			// Ajuste de posições
			xPosE++;
			xPosD = xPosE + iPieceWidth;
			yPos--;

			break;

		case 2:
			// Tamanho da peça
			iPieceWidth = 4;
			iPieceHeight = 1;

			// Ajuste de posições
			xPosD = xPosE + iPieceWidth;
			yPos++;

			// Rotação junto da parede do lado esquerdo
			if (xPosE < 0) {
				// Garantir que peça se mantém dentro da janela de visualização
				iNumberTranslation++;
				// Reajustar posição da peça resultante de ajuste
				xPosE++;
				xPosD++;
			} 

			// Rotação junto da parede do lado direito
			if (xPosD > iWidth) {
				// Garantir que peça se mantém dentro da janela de visualização
				iNumberTranslation--;
				// Reajustar posição da peça resultante de ajuste
				xPosE--;
				xPosD--;
			}

			break;

		case 3:
			// Tamanho da peça
			iPieceWidth = 1;
			iPieceHeight = 4;

			// Ajuste de posições
			xPosE += 2;
			xPosD = xPosE + iPieceWidth;
			yPos--;

			// Rotação junto ao fundo da janela
			if (yPos < 0) {
				/* Não atualizar posiçao de yPos neste caso
				 Este yPosAjuste irá atuar na próxima iteração de draw, que fará a peça subir,
			     garantindo que esta se mantém na janela de visualização. Se for alterado yPos para 
				 yPos++ irá dar colisão com a base da janela, nesta iteração, ficando apenas visível
				 3 blocos (ao invés dos 4, correspondentes à peça) */
				yPosAjuste = 1;
			}

			break;
	}
}

// glm::mat4& rot => alternativa a  glm::mat4 *rot aqui, com chamada de funçao &rot, ao inves de rot
void PecaI::rotacaoPeca(glm::mat4& rot) {

	// Transladar para origem da peça, rodar 90 graus e retornar à sua posição original 
	rot = glm::translate(rot, glm::vec3(xCR, yCR, 0.0f));
	rot = glm::rotate(rot, glm::radians(iNumberRotate * -90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	rot = glm::translate(rot, glm::vec3(-xCR, -yCR, 0.0f));

}

void PecaI::translacaoPeca(glm::mat4& trans) {

	auto t_now = std::chrono::high_resolution_clock::now();
	// <int> -> Forçar a que a variação de tempo considerada seja a cada segundo
	int time = std::chrono::duration_cast<std::chrono::duration<int>>(t_now - t_start).count();

	// Peça desce pelo ecrã, a cada segundo
	yPos = yPosInicial - time - iNumberDown * .5 + yPosAjuste;
	//cout << "yPos: " << yPos << ", yPosAjuste: " << yPosAjuste << endl;

	// Trata das translações para esquerda e direita ("+" -> direita, "-" -> esquerda)
	xPosE = xPosInicial + iNumberTranslation;

	trans = glm::translate(trans, glm::vec3(xPosE, yPos, 0.0f));

	/* Atualiza posições para avaliação de colisões, preenchimento de matriz e proxima iteração de draw
	 (no caso de alteração relativamente a iNumberTranslation) */
	atualizaPos();
}

void PecaI::drawObject() {
	glDrawArrays(GL_TRIANGLES, 0, 8*3);
}

// Getters
int** PecaI::getGameGrid() {
	return gameGrid;
}

int PecaI::getNumberRotate() {
	return iNumberRotate;
}

int PecaI::getNumberTranslation() {
	return iNumberTranslation;
}

int PecaI::getNumberDown() {
	return iNumberDown;
}

int PecaI::getXPosD() {
	return xPosD;
}

int PecaI::getXPosE() {
	return xPosE;
}

// Atualizadores

void PecaI::incNumberRotate() {
	iNumberRotate++;
}

void PecaI::incNumberTranslation() {
	iNumberTranslation++;
}

void PecaI::decNumberTranslation() {
	iNumberTranslation--;
}

void PecaI::incNumberDown() {
	iNumberDown++;
}
