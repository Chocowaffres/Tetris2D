#include "headers/PecaL.hpp"
#include <iostream>

// Construtores
PecaL::PecaL() {};

PecaL::PecaL(int xPosInicial, int yPosInicial, int iHeight, int iWidth, int** gameGrid) {

	// Variáveis da peça
	xCR = 1.5;
	yCR = 0.5;
	iPieceWidth = 3;
	iPieceHeight = 2;

	yPos = yPosInicial;
	xPosE = xPosInicial;
	xPosD = xPosInicial + iPieceWidth;

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

	// Variáveis associadas a temporizador de colisão, visando melhor jogabilidade
	oldValueTime = 0;
	bCollisionBottom = false;
	bCollisionLeft = false;
	bCollisionRight = false;
	bRotationAllowed = true;
	acertoPosicaoY = 0;

	g_real_vertex_buffer = {};
};

// Variáveis de classe
// Desenho da peça
std::vector<GLfloat> PecaL::g_vertex_buffer_data = {
		//L
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
		2.0f,  1.0f,  0.0f,
		3.0f,  1.0f,  0.0f,
		2.0f,  2.0f,  0.0f,

		3.0f,  1.0f,  0.0f,
		2.0f,  2.0f,  0.0f,
		3.0f,  2.0f,  0.0f,
};

// Textura da peça
std::vector<GLfloat> PecaL::g_texture_buffer_data = {
		//L
		0.25f,  0.25f,
		0.25f,  0.5f,
		0.5f,  0.25f,

		0.25f,  0.5f,
		0.5f,  0.25f,
		0.5f,  0.5f,

		//
		0.25f,  0.25f,
		0.25f,  0.5f,
		0.5f,  0.25f,

		0.25f,  0.5f,
		0.5f,  0.25f,
		0.5f,  0.5f,

		//
		0.25f,  0.25f,
		0.25f,  0.5f,
		0.5f,  0.25f,

		0.25f,  0.5f,
		0.5f,  0.25f,
		0.5f,  0.5f,

		//
		0.25f,  0.25f,
		0.25f,  0.5f,
		0.5f,  0.25f,

		0.25f,  0.5f,
		0.5f,  0.25f,
		0.5f,  0.5f,
};

std::vector<GLfloat> PecaL::g_real_vertex_buffer = {};

// Preenchimento do vertexBuffer de acordo com rotação da peça e local de colisão, para armazenamento do vertexBuffer
// das peças já jogadas (em Projeto.cpp)
void PecaL::preencheRealVertex(GLfloat x, GLfloat y) {
	g_real_vertex_buffer.push_back(x);
	g_real_vertex_buffer.push_back(y);
	g_real_vertex_buffer.push_back(0.0f);
}

// Modificação de um bloco/quadricula da peça, repesentado por 2 triangulos
void PecaL::modificaQuadricula(GLfloat x, GLfloat y) {

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
void PecaL::realVertexBuffer() {
	int xPosAntes = xPosE;
	int yPosAntes = yPos;

	switch (iNumberRotate % 4) {
		case 0:

			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);
			break;
		case 1:

			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			xPosE--;
			yPos++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);
			break;
		case 2:

			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);
			break;
		case 3:

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			xPosE--;
			modificaQuadricula(xPosE, yPos);
			break;
	}

	// Reposição de valores de variáveis
	xPosE = xPosAntes;
	yPos = yPosAntes;
}

bool PecaL::preencheMatriz(int x, int y, int xAjuste, int yAjuste) {
	// Altura
	for (int i = 0; i < iPieceHeight; i++) {
		// Jogo acabou
		if (y + i > iHeight) {
			return true;
		}
		gameGrid[x + xAjuste][y + i] = 1;
	}
	// Largura
	for (int i = xPosE; i < xPosD; i++) {
		gameGrid[i][y + yAjuste] = 1;
	}

	return false;
}

bool PecaL::atualizaMatriz() {
	switch (iNumberRotate % 4) {
		case 0: return preencheMatriz(xPosE, yPos, iPieceWidth - 1, 0);
		case 1: return preencheMatriz(xPosE, yPos, 0, 0);
		case 2: return preencheMatriz(xPosE, yPos, 0, iPieceHeight - 1);
		case 3: return preencheMatriz(xPosE, yPos, iPieceWidth - 1, iPieceHeight - 1);
	}
}

bool PecaL::avaliaPotencialRotacao(int x, int y, int xAjuste, int yAjuste, int iPieceHeight, int xPosE, int xPosD) {

	// Acertos nas interações com limites laterais de janela de jogo
	if (x < 0) {
		x++;
		xPosE++;
		xPosD++;
	}
	if (x + (xPosD - xPosE) >= iWidth) {
		x--;
		xPosE--;
		xPosD--;
	}

	// Altura
	for (int i = 0; i < iPieceHeight; i++) {
		// Rotação excediria ecra de jogo
		if (y + i > iHeight) {
			return false;
		}
		if (gameGrid[x + xAjuste][y + i] == 1) {
			return false;
		}
	}
	// Largura
	for (int i = xPosE; i < xPosD; i++) {
		if (gameGrid[i][y + yAjuste] == 1) {
			return false;
		}
	}

	// É possivel fazer a rotação
	return true;
}

bool PecaL::avaliaColisao() {

	// Dimensões e localização da peça da próxima rotação
	int iPieceHeight_AvaliaRotacaoSeguinte;
	int iPieceWidth_AvaliaRotacaoSeguinte;
	int yPos_AvaliaRotacaoSeguinte;
	int xPosE_AvaliaRotacaoSeguinte;

	// Reset de variáveis
	bCollisionBottom = false;
	bCollisionLeft = false;
	bCollisionRight = false;
	bRotationAllowed = true;

	// Colisão com base de jogo
	if (yPos == 0) {
		bCollisionBottom = true;
	}

	/* Colisão com outras peças, tendo em consideração a rotação da peça
	Diferentes variáveis são atualizadas, mediante o ponto de colisão, avaliadas em registerUserInputs, em main.cpp */
	switch (iNumberRotate % 4) {
		case 0:
			// Bottom collision
			if ((gameGrid[xPosE][yPos - 1] == 1) || 
				(gameGrid[xPosE + 1][yPos - 1] == 1) || 
				(gameGrid[xPosE + 2][yPos - 1] == 1)) {
				bCollisionBottom = true;
			}

			// Left collision
			if (xPosE - 1 >= 0) {
				if ((gameGrid[xPosE - 1][yPos] == 1) || 
					(gameGrid[xPosE + 1][yPos + 1] == 1)) {
						bCollisionLeft = true;
				}
			}

			// Right collision
			if (xPosE + 3 < iWidth) {
				if ((gameGrid[xPosE + 3][yPos] == 1) || 
					(gameGrid[xPosE + 3][yPos + 1] == 1)) {
						bCollisionRight = true;
				}
			}

			/* Cálculo de parâmetros de acordo com rotação seguinte, seguindo a mesma lógica de atualizaMatriz, com os ajustes necessários */
			iPieceHeight_AvaliaRotacaoSeguinte = 3;
			iPieceWidth_AvaliaRotacaoSeguinte = 2;
			xPosE_AvaliaRotacaoSeguinte = xPosE + 1;
			yPos_AvaliaRotacaoSeguinte = yPos - 1;

			// Verificar rotação por recurso a função
			bRotationAllowed = avaliaPotencialRotacao(
				xPosE_AvaliaRotacaoSeguinte, yPos_AvaliaRotacaoSeguinte, 0, 0,
				iPieceHeight_AvaliaRotacaoSeguinte, xPosE_AvaliaRotacaoSeguinte, 
				(xPosE_AvaliaRotacaoSeguinte + iPieceWidth_AvaliaRotacaoSeguinte)
			);
			break;

		case 1:
			// Bottom collision
			if ((gameGrid[xPosE][yPos - 1] == 1) || 
				(gameGrid[xPosE + 1][yPos - 1] == 1)) {
					bCollisionBottom = true;
			}

			// Left collision
			if (xPosE - 1 >= 0) {
				if ((gameGrid[xPosE - 1][yPos] == 1) ||
					(gameGrid[xPosE - 1][yPos + 1] == 1) || 
					(gameGrid[xPosE - 1][yPos + 2] == 1)) {
						bCollisionLeft = true;
				}
			}

			// Right collision
			if (xPosE + 2 < iWidth) {
				if ((gameGrid[xPosE + 2][yPos] == 1) ||
					(gameGrid[xPosE + 1][yPos + 1] == 1) ||
					(gameGrid[xPosE + 1][yPos + 2] == 1)) {
						bCollisionRight = true;
				}
			}

			/* Cálculo de parâmetros de acordo com rotação seguinte, seguindo a mesma lógica de atualizaMatriz, com os ajustes necessários */
			iPieceHeight_AvaliaRotacaoSeguinte = 2;
			iPieceWidth_AvaliaRotacaoSeguinte = 3;
			xPosE_AvaliaRotacaoSeguinte = xPosE - 1;
			yPos_AvaliaRotacaoSeguinte = yPos;

			// Verificar rotação por recurso a função
			bRotationAllowed = avaliaPotencialRotacao(
				xPosE_AvaliaRotacaoSeguinte, yPos_AvaliaRotacaoSeguinte, 0, iPieceHeight_AvaliaRotacaoSeguinte - 1,
				iPieceHeight_AvaliaRotacaoSeguinte, xPosE_AvaliaRotacaoSeguinte, 
				(xPosE_AvaliaRotacaoSeguinte + iPieceWidth_AvaliaRotacaoSeguinte)
				);
			break;

		case 2:
			// Bottom collision
			if ((gameGrid[xPosE][yPos - 1] == 1) || 
				(gameGrid[xPosE + 1][yPos] == 1) || 
				(gameGrid[xPosE + 2][yPos] == 1)) {
					bCollisionBottom = true;
			}

			// Left collision
			if (xPosE - 1 >= 0) {
				if ((gameGrid[xPosE - 1][yPos] == 1) ||
					(gameGrid[xPosE - 1][yPos + 1] == 1)) {
						bCollisionLeft = true;
				}
			}

			// Right collision
			if (xPosE + 3 < iWidth) {
				if ((gameGrid[xPosE + 1][yPos] == 1) ||
					(gameGrid[xPosE + 3][yPos + 1] == 1)) {
						bCollisionRight = true;
				}
			}

			/* Cálculo de parâmetros de acordo com rotação seguinte, seguindo a mesma lógica de atualizaMatriz, com os ajustes necessários */
			iPieceHeight_AvaliaRotacaoSeguinte = 3;
			iPieceWidth_AvaliaRotacaoSeguinte = 2;
			xPosE_AvaliaRotacaoSeguinte = xPosE;
			yPos_AvaliaRotacaoSeguinte = yPos;

			// Verificar rotação por recurso a função
			bRotationAllowed = avaliaPotencialRotacao(
				xPosE_AvaliaRotacaoSeguinte, yPos_AvaliaRotacaoSeguinte, iPieceWidth_AvaliaRotacaoSeguinte - 1, iPieceHeight_AvaliaRotacaoSeguinte - 1,
				iPieceHeight_AvaliaRotacaoSeguinte, xPosE_AvaliaRotacaoSeguinte,
				(xPosE_AvaliaRotacaoSeguinte + iPieceWidth_AvaliaRotacaoSeguinte)
			);
			break;

			break;
		case 3:
			// Bottom collision
			if ((gameGrid[xPosE][yPos + 1] == 1) || (gameGrid[xPosE + 1][yPos - 1] == 1)){
				bCollisionBottom = true;
			}

			// Left collision
			if (xPosE - 1 >= 0) {
				if ((gameGrid[xPosE][yPos] == 1) ||
					(gameGrid[xPosE][yPos + 1] == 1) ||
					(gameGrid[xPosE - 1][yPos + 2] == 1)) {
						bCollisionLeft = true;
				}
			}

			// Right collision
			if (xPosE + 2 < iWidth) {
				if ((gameGrid[xPosE + 2][yPos] == 1) ||
					(gameGrid[xPosE + 2][yPos + 1] == 1) ||
					(gameGrid[xPosE + 2][yPos + 2] == 1)) {
						bCollisionRight = true;
				}
			}

			/* Cálculo de parâmetros de acordo com rotação seguinte, seguindo a mesma lógica de atualizaMatriz, com os ajustes necessários */
			iPieceHeight_AvaliaRotacaoSeguinte = 2;
			iPieceWidth_AvaliaRotacaoSeguinte = 3;
			xPosE_AvaliaRotacaoSeguinte = xPosE;
			yPos_AvaliaRotacaoSeguinte = yPos + 1;

			// Verificar rotação por recurso a função
			bRotationAllowed = avaliaPotencialRotacao(
				xPosE_AvaliaRotacaoSeguinte, yPos_AvaliaRotacaoSeguinte, iPieceWidth_AvaliaRotacaoSeguinte - 1, 0,
				iPieceHeight_AvaliaRotacaoSeguinte, xPosE_AvaliaRotacaoSeguinte,
				(xPosE_AvaliaRotacaoSeguinte + iPieceWidth_AvaliaRotacaoSeguinte)
			);
			break;
	}
	/* Apena agora será retornado o valor de colisão para garantir que as restantes variáveis (bCollisionLeft e bCollisionRight)
	são atualizadas de acordo com a situação de colisão */
	if (bCollisionBottom) {
		return true;
	}
	return false;
}

void PecaL::atualizaPos() {
	// Posicao inicial
	switch (iNumberRotate % 4) {
		case 0:
			// Tamanho da peça
			iPieceWidth = 3;
			iPieceHeight = 2;

			// Ajuste de posições
			xPosD = xPosE + iPieceWidth;

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
			iPieceWidth = 2;
			iPieceHeight = 3;

			// Ajuste de posições
			xPosE++;
			xPosD = xPosE + iPieceWidth;
			yPos--;

			// Caso em que é necessário garantir que peça não desce para fora da janela de visualização
			if (yPos <= 0) {
				// Garantir que peça se mantém dentro da janela de visualização 
				acertoPosicaoY += -yPos;
			}

			break;
		
		case 2:
			// Tamanho da peça
			iPieceWidth = 3;
			iPieceHeight = 2;

			// Ajuste de posições
			xPosD = xPosE + iPieceWidth;
			yPos--;

			// Rotação junto da parede do lado esquerdo
			if (xPosE < 0) {
				// Garantir que peça se mantém dentro da janela de visualização
				iNumberTranslation++;
				// Reajustar posição da peça resultante de ajuste
				xPosE++;
				xPosD++;
			}
			
			break;
		
		case 3:
			// Tamanho da peça
			iPieceWidth = 2;
			iPieceHeight = 3;

			// Ajuste de posições
			xPosD = xPosE + iPieceWidth;
			yPos--;

			// Caso em que é necessário garantir que peça não desce para fora da janela de visualização
			if (yPos <= 0) {
				// Garantir que peça se mantém dentro da janela de visualização 
				acertoPosicaoY += -yPos;
			}

			break;
	}
}

// glm::mat4& rot => alternativa a  glm::mat4 *rot aqui, com chamada de funçao &rot, ao inves de rot
void PecaL::rotacaoPeca(glm::mat4& rot) {

	// Transladar para origem da peça, rodar 90 graus e retornar à sua posição original 
	rot = glm::translate(rot, glm::vec3(xCR, yCR, 0.0f));
	rot = glm::rotate(rot, glm::radians(iNumberRotate * -90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	rot = glm::translate(rot, glm::vec3(-xCR, -yCR, 0.0f));
}

void PecaL::translacaoPeca(glm::mat4& trans) {
	auto t_now = std::chrono::high_resolution_clock::now();
	// <int> -> Forçar a que a variação de tempo considerada seja a cada segundo
	int time = std::chrono::duration_cast<std::chrono::duration<int>>(t_now - t_start).count();

	/* Se houve colisão, não reajustar variável associada a tempo, para garantir que peça se mantém
	posição onde estava no momento de colisão. "time" influencia a descida da peça, daí ter esta avaliação */
	if (bCollisionBottom) {
		time = oldValueTime;
	}

	// Peça desce pelo ecrã, a cada segundo
	yPos = yPosInicial - time - iNumberDown * .5 + acertoPosicaoY;

	// Trata das translações para esquerda e direita ("+" -> direita, "-" -> esquerda)
	xPosE = xPosInicial + iNumberTranslation;

	trans = glm::translate(trans, glm::vec3(xPosE, yPos, 0.0f));

	/* Atualiza posições para avaliação de colisões, preenchimento de matriz e proxima iteração de draw
	 (no caso de alteração relativamente a iNumberTranslation) */
	atualizaPos();

	// Caso não tenha havido colisão, atualizar variável de tempo anterior.
	if (!bCollisionBottom) {
		oldValueTime = time;
	}
}

void PecaL::drawObject() {
	glDrawArrays(GL_TRIANGLES, 0, 8*3);
}

// Getters
int** PecaL::getGameGrid() {
	return gameGrid;
}

int PecaL::getNumberRotate() {
	return iNumberRotate;
}

int PecaL::getNumberTranslation() {
	return iNumberTranslation;
}

int PecaL::getNumberDown() {
	return iNumberDown;
}

int PecaL::getXPosD() {
	return xPosD;
}

int PecaL::getXPosE() {
	return xPosE;
}

bool PecaL::hasCollidedBottom() {
	return bCollisionBottom;
}

bool PecaL::hasCollidedLeft() {
	return bCollisionLeft;
}

bool PecaL::hasCollidedRight() {
	return bCollisionRight;
}

bool PecaL::rotationAllowed() {
	return bRotationAllowed;
}

// Atualizadores

void PecaL::incNumberRotate() {
	iNumberRotate++;
}

void PecaL::incNumberTranslation() {
	iNumberTranslation++;
}

void PecaL::decNumberTranslation() {
	iNumberTranslation--;
}

void PecaL::incNumberDown() {
	iNumberDown++;
}
