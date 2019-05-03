#include "headers/PecaT.hpp"
#include "headers/Peca.hpp"
#include <iostream>

// Construtores
PecaT::PecaT() {};

PecaT::PecaT(int xPosInicial, int yPosInicial, int iHeight, int iWidth, int** gameGrid) {

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

	// Vari�veis associadas a temporizador de colis�o, visando melhor jogabilidade
	oldValueTime = 0;
	bCollisionBottom = false;
	bCollisionLeft = false;
	bCollisionRight = false;
	bRotationAllowed = true;
	acertoPosicaoY = 0;

	g_real_vertex_buffer = {};

};

// Vari�veis de classe
// Desenho da pe�a
std::vector<GLfloat> PecaT::g_vertex_buffer_data = {
	//T
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
		1.0f,  1.0f,  0.0f,
		2.0f,  1.0f,  0.0f,
		1.0f,  2.0f,  0.0f,

		2.0f,  1.0f,  0.0f,
		1.0f,  2.0f,  0.0f,
		2.0f,  2.0f,  0.0f,
};

// Textura da pe�a
std::vector<GLfloat> PecaT::g_texture_buffer_data = {
		//T
		0.25f,  0.0f,
		0.25f,  0.25f,
		0.5f,  0.0f,

		0.25f,  0.25f,
		0.5f,  0.0f,
		0.5f,  0.25f,

		//
		0.25f,  0.0f,
		0.25f,  0.25f,
		0.5f,  0.0f,

		0.25f,  0.25f,
		0.5f,  0.0f,
		0.5f,  0.25f,

		//
		0.25f,  0.0f,
		0.25f,  0.25f,
		0.5f,  0.0f,

		0.25f,  0.25f,
		0.5f,  0.0f,
		0.5f,  0.25f,

		//
		0.25f,  0.0f,
		0.25f,  0.25f,
		0.5f,  0.0f,

		0.25f,  0.25f,
		0.5f,  0.0f,
		0.5f,  0.25f,
};

std::vector<GLfloat> PecaT::g_real_vertex_buffer = {};

// Preenchimento do vertexBuffer de acordo com rota��o da pe�a e local de colis�o, para armazenamento do vertexBuffer
// das pe�as j� jogadas (em Projeto.cpp)
void PecaT::preencheRealVertex(GLfloat x, GLfloat y) {
	g_real_vertex_buffer.push_back(x);
	g_real_vertex_buffer.push_back(y);
	g_real_vertex_buffer.push_back(0.0f);
}

// Modifica��o de um bloco/quadricula da pe�a, repesentado por 2 triangulos
void PecaT::modificaQuadricula(GLfloat x, GLfloat y) {

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
void PecaT::realVertexBuffer() {

	int xPosAntes = xPosE;
	int yPosAntes = yPos;

	switch (iNumberRotate % 4) {
		case 0: 

			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			xPosE--;
			yPos++;
			modificaQuadricula(xPosE, yPos);

			break;
		case 1: 

			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			yPos--;
			modificaQuadricula(xPosE, yPos);
			break;
		case 2:
		
			xPosE++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			xPosE--;
			modificaQuadricula(xPosE, yPos);

			xPosE += 2;
			modificaQuadricula(xPosE, yPos);
			break;
		case 3: 

			yPos++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			yPos--;
			modificaQuadricula(xPosE, yPos);

			yPos += 2;
			modificaQuadricula(xPosE, yPos);
			break;
		}

	// Reposi��o de valores de vari�veis
	xPosE = xPosAntes;
	yPos = yPosAntes;
}

bool PecaT::preencheMatriz(int x, int y, int xAjuste, int yAjuste) {
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

bool PecaT::atualizaMatriz() {
	switch (iNumberRotate % 4) {
		case 0: return preencheMatriz(xPosE, yPos, trunc(iPieceWidth/2), 0);
		case 1: return preencheMatriz(xPosE, yPos, 0, trunc(iPieceHeight / 2));
		case 2: return preencheMatriz(xPosE, yPos, trunc(iPieceWidth / 2), iPieceHeight - 1);
		case 3: return preencheMatriz(xPosE, yPos, iPieceWidth - 1, trunc(iPieceHeight / 2));
	}
}

bool PecaT::avaliaPotencialRotacao(int x, int y, int xAjuste, int yAjuste, int iPieceHeight, int xPosE, int xPosD) {

	// Acertos nas intera��es com limites laterais de janela de jogo
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
		// Jogo acabou
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

	// � possivel fazer a rota��o
	return true;
}

bool PecaT::avaliaColisao() {

	// Dimens�es e localiza��o da pe�a da pr�xima rota��o
	int iPieceHeight_AvaliaRotacaoSeguinte;
	int iPieceWidth_AvaliaRotacaoSeguinte;
	int xPosE_AvaliaRotacaoSeguinte;

	// Reset de vari�veis
	bCollisionBottom = false;
	bCollisionLeft = false;
	bCollisionRight = false;
	bRotationAllowed = true;

	// Colis�o com base de jogo
	if (yPos == 0) {
		bCollisionBottom = true;
	}

	/* Colis�o com outras pe�as, tendo em considera��o a rota��o da pe�a
	Diferentes vari�veis s�o atualizadas, mediante o ponto de colis�o, avaliadas em registerUserInputs, em main.cpp */
	switch (iNumberRotate % 4) {
		case 0: 
			// Bottom collision
			if ((gameGrid[xPosE][yPos-1] == 1) || 
				(gameGrid[xPosE + 1][yPos-1] == 1) || 
				(gameGrid[xPosE + 2][yPos-1] == 1)){
					bCollisionBottom = true;
			}

			// Left collision
			if (xPosE - 1 >= 0) {
				if ((gameGrid[xPosE][yPos + 1] == 1) ||
					(gameGrid[xPosE - 1][yPos] == 1)) {
						bCollisionLeft = true;
				}
			}

			// Right collision
			if (xPosE + 3 < iWidth) {
				if ((gameGrid[xPosE + 2][yPos + 1] == 1) ||
					(gameGrid[xPosE + 3][yPos] == 1)) {
						bCollisionRight = true;
				}
			}

			/* C�lculo de par�metros de acordo com rota��o seguinte, seguindo a mesma l�gica de atualizaMatriz, com os ajustes necess�rios */
			iPieceHeight_AvaliaRotacaoSeguinte = 3;
			iPieceWidth_AvaliaRotacaoSeguinte = 2;
			xPosE_AvaliaRotacaoSeguinte = xPosE + 1;

			// Verificar rota��o por recurso a fun��o
			bRotationAllowed = avaliaPotencialRotacao(
				xPosE_AvaliaRotacaoSeguinte, (yPos - 1), 0, trunc(iPieceHeight_AvaliaRotacaoSeguinte / 2),
				iPieceWidth_AvaliaRotacaoSeguinte, xPosE_AvaliaRotacaoSeguinte,
				(xPosE_AvaliaRotacaoSeguinte + iPieceWidth_AvaliaRotacaoSeguinte)
			);

			break;

		case 1:
			// Bottom collision
			if ((gameGrid[xPosE][yPos - 1] == 1) || 
				(gameGrid[xPosE + 1][yPos] == 1)){
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
				if ((gameGrid[xPosE + 1][yPos] == 1) || 
					(gameGrid[xPosE + 1][yPos + 2]) ||
					(gameGrid[xPosE + 2][yPos + 1] == 1)) {
						bCollisionRight = true;
				}
			}
			/* C�lculo de par�metros de acordo com rota��o seguinte, seguindo a mesma l�gica de atualizaMatriz, com os ajustes necess�rios */
			iPieceHeight_AvaliaRotacaoSeguinte = 2;
			iPieceWidth_AvaliaRotacaoSeguinte = 3;
			xPosE_AvaliaRotacaoSeguinte = xPosE - 1;

			// Verificar rota��o por recurso a fun��o
			bRotationAllowed = avaliaPotencialRotacao(
				xPosE_AvaliaRotacaoSeguinte, yPos, trunc(iPieceWidth_AvaliaRotacaoSeguinte / 2), iPieceHeight_AvaliaRotacaoSeguinte - 1,
				iPieceWidth_AvaliaRotacaoSeguinte, xPosE_AvaliaRotacaoSeguinte,
				(xPosE_AvaliaRotacaoSeguinte + iPieceWidth_AvaliaRotacaoSeguinte)
			);
			break;

		case 2:
			// Bottom collision
			if ((gameGrid[xPosE][yPos] == 1) || 
				(gameGrid[xPosE + 1][yPos - 1] == 1) || 
				(gameGrid[xPosE + 2][yPos] == 1)){
					bCollisionBottom = true;
			}

			// Left collision
			if (xPosE - 1 >= 0) {
				if ((gameGrid[xPosE][yPos] == 1) ||
					(gameGrid[xPosE - 1][yPos + 1] == 1)) {
						bCollisionLeft = true;
				}

			}

			// Right collision
			if (xPosE + 3 < iWidth) {
				if ((gameGrid[xPosE + 2][yPos] == 1) ||
					(gameGrid[xPosE + 3][yPos + 1] == 1)) {
						bCollisionRight = true;
				}
			}

			/* C�lculo de par�metros de acordo com rota��o seguinte, seguindo a mesma l�gica de atualizaMatriz, com os ajustes necess�rios */
			iPieceHeight_AvaliaRotacaoSeguinte = 3;
			iPieceWidth_AvaliaRotacaoSeguinte = 2;
			xPosE_AvaliaRotacaoSeguinte = xPosE;

			// Verificar rota��o por recurso a fun��o
			bRotationAllowed = avaliaPotencialRotacao(
				xPosE_AvaliaRotacaoSeguinte, yPos, iPieceWidth_AvaliaRotacaoSeguinte - 1, trunc(iPieceHeight_AvaliaRotacaoSeguinte / 2),
				iPieceWidth_AvaliaRotacaoSeguinte, xPosE_AvaliaRotacaoSeguinte,
				(xPosE_AvaliaRotacaoSeguinte + iPieceWidth_AvaliaRotacaoSeguinte)
			);
			break;
		case 3:
			// Bottom collision
			if ((gameGrid[xPosE][yPos] == 1) || 
				(gameGrid[xPosE + 1][yPos - 1] == 1)){
					bCollisionBottom = true;
			}

			// Left collision
			if (xPosE - 1 >= 0) {
				if ((gameGrid[xPosE][yPos] == 1) ||
					(gameGrid[xPosE][yPos + 2] == 1) ||
					(gameGrid[xPosE - 1][yPos+1] == 1)) {
						bCollisionLeft = true;
				}
			}

			// Right collision
			if (xPosE + 2 < iWidth) {
				if ((gameGrid[xPosE + 2][yPos] == 1) ||
					(gameGrid[xPosE + 2][yPos + 1] == 1) || 
					(gameGrid[xPosE + 2][yPos + 2])) {
						bCollisionRight = true;
				}
			}

			/* C�lculo de par�metros de acordo com rota��o seguinte, seguindo a mesma l�gica de atualizaMatriz, com os ajustes necess�rios */
			iPieceHeight_AvaliaRotacaoSeguinte = 2;
			iPieceWidth_AvaliaRotacaoSeguinte = 3;
			xPosE_AvaliaRotacaoSeguinte = xPosE;

			// Verificar rota��o por recurso a fun��o
			bRotationAllowed = avaliaPotencialRotacao(
				xPosE_AvaliaRotacaoSeguinte, yPos + 1, trunc(iPieceWidth_AvaliaRotacaoSeguinte / 2), 0,
				iPieceWidth_AvaliaRotacaoSeguinte, xPosE_AvaliaRotacaoSeguinte,
				(xPosE_AvaliaRotacaoSeguinte + iPieceWidth_AvaliaRotacaoSeguinte)
			);
			break;
	}
	/* Apena agora ser� retornado o valor de colis�o para garantir que as restantes vari�veis (bCollisionLeft e bCollisionRight)
	s�o atualizadas de acordo com a situa��o de colis�o */
	if (bCollisionBottom) {
		return true;
	}
	return false;
}

void PecaT::atualizaPos() {
	// Posicao inicial
	switch (iNumberRotate % 4) {
		case 0:
			// Tamanho da pe�a
			iPieceWidth = 3;
			iPieceHeight = 2;

			// Ajuste de posi��es
			xPosD = xPosE + iPieceWidth;

			// Rota��o junto da parede do lado direito
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
			xPosE++;
			xPosD = xPosE + iPieceWidth;
			yPos--;

			// Caso em que � necess�rio garantir que pe�a n�o desce para fora da janela de visualiza��o
			if (yPos <= 0) {
				// Garantir que pe�a se mant�m dentro da janela de visualiza��o 
				acertoPosicaoY += -yPos;
			}

			break;

		case 2:
			// Tamanho da pe�a
			iPieceWidth = 3;
			iPieceHeight = 2;

			// Ajuste de posi��es
			xPosD = xPosE + iPieceWidth;
			yPos--;

			// Rota��o junto da parede do lado esquerdo
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

			// Caso em que � necess�rio garantir que pe�a n�o desce para fora da janela de visualiza��o
			if (yPos <= 0) {
				// Garantir que pe�a se mant�m dentro da janela de visualiza��o 
				acertoPosicaoY += -yPos;
			}

			break;
		}
}

// glm::mat4& rot => alternativa a  glm::mat4 *rot aqui, com chamada de fun�ao &rot, ao inves de rot
void PecaT::rotacaoPeca(glm::mat4& rot) {

	// Transladar para origem da pe�a, rodar 90 graus e retornar � sua posi��o original 
	rot = glm::translate(rot, glm::vec3(xCR, yCR, 0.0f));
	rot = glm::rotate(rot, glm::radians(iNumberRotate * -90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	rot = glm::translate(rot, glm::vec3(-xCR, -yCR, 0.0f));

}

void PecaT::translacaoPeca(glm::mat4& trans) {
	auto t_now = std::chrono::high_resolution_clock::now();
	// <int> -> For�ar a que a varia��o de tempo considerada seja a cada segundo
	int time = std::chrono::duration_cast<std::chrono::duration<int>>(t_now - t_start).count();

	/* Se houve colis�o, n�o reajustar vari�vel associada a tempo, para garantir que pe�a se mant�m
	posi��o onde estava no momento de colis�o. "time" influencia a descida da pe�a, da� ter esta avalia��o */
	if (bCollisionBottom) {
		time = oldValueTime;
	}

	// Pe�a desce pelo ecr�, a cada segundo
	yPos = yPosInicial - time - iNumberDown * .5 + acertoPosicaoY;

	// Trata das transla��es para esquerda e direita ("+" -> direita, "-" -> esquerda)
	xPosE = xPosInicial + iNumberTranslation;

	trans = glm::translate(trans, glm::vec3(xPosE, yPos, 0.0f));

	/* Atualiza posi��es para avalia��o de colis�es, preenchimento de matriz e proxima itera��o de draw
	(no caso de altera��o relativamente a iNumberTranslation) */
	atualizaPos();

	// Caso n�o tenha havido colis�o, atualizar vari�vel de tempo anterior.
	if (!bCollisionBottom) {
		oldValueTime = time;
	}
}

void PecaT::drawObject() {
	glDrawArrays(GL_TRIANGLES, 0, 8*3);
}

// Getters
int** PecaT::getGameGrid() {
	return gameGrid;
}

int PecaT::getNumberRotate() {
	return iNumberRotate;
}

int PecaT::getNumberTranslation() {
	return iNumberTranslation;
}

int PecaT::getNumberDown() {
	return iNumberDown;
}

int PecaT::getXPosD() {
	return xPosD;
}

int PecaT::getXPosE() {
	return xPosE;
}

bool PecaT::hasCollidedBottom() {
	return bCollisionBottom;
}

bool PecaT::hasCollidedLeft() {
	return bCollisionLeft;
}

bool PecaT::hasCollidedRight() {
	return bCollisionRight;
}

bool PecaT::rotationAllowed() {
	return bRotationAllowed;
}

// Atualizadores

void PecaT::incNumberRotate() {
	iNumberRotate++;
}

void PecaT::incNumberTranslation() {
	iNumberTranslation++;
}

void PecaT::decNumberTranslation() {
	iNumberTranslation--;
}

void PecaT::incNumberDown() {
	iNumberDown++;
}
