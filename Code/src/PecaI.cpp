#include "headers/PecaI.hpp"
#include <iostream>

// Construtores
PecaI::PecaI() {};

PecaI::PecaI(int xPosInicial, int yPosInicial, int iHeight, int iWidth, int** gameGrid) {

	// Vari�veis da pe�a
	xCR = 2.0f;
	yCR = 1.0f;
	iPieceWidth = 4;
	iPieceHeight = 1;

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

// Textura da pe�a
std::vector<GLfloat> PecaI::g_texture_buffer_data = {
		//I
		0.5f,  0.25f,
		0.5f,  0.5f,
		0.75f,  0.25f,

		0.5f,  0.5f,
		0.75f,  0.25f,
		0.75f,  0.5f,

		//
		0.5f,  0.25f,
		0.5f,  0.5f,
		0.75f,  0.25f,

		0.5f,  0.5f,
		0.75f,  0.25f,
		0.75f,  0.5f,

		//
		0.5f,  0.25f,
		0.5f,  0.5f,
		0.75f,  0.25f,

		0.5f,  0.5f,
		0.75f,  0.25f,
		0.75f,  0.5f,

		//
		0.5f,  0.25f,
		0.5f,  0.5f,
		0.75f,  0.25f,

		0.5f,  0.5f,
		0.75f,  0.25f,
		0.75f,  0.5f,
};

std::vector<GLfloat> PecaI::g_real_vertex_buffer = {};

// Preenchimento do vertexBuffer de acordo com rota��o da pe�a e local de colis�o, para armazenamento do vertexBuffer
// das pe�as j� jogadas (em Projeto.cpp)
void PecaI::preencheRealVertex(GLfloat x, GLfloat y) {
	g_real_vertex_buffer.push_back(x);
	g_real_vertex_buffer.push_back(y);
	g_real_vertex_buffer.push_back(0.0f);
}

// Modifica��o de um bloco/quadricula da pe�a, repesentado por 2 triangulos
void PecaI::modificaQuadricula(GLfloat x, GLfloat y) {

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

	// Reposi��o de valores de vari�veis
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

bool PecaI::avaliaPotencialRotacao(int x, int y, int iPieceHeight, int xPosE, int xPosD) {

	int ** gameGridTemp = (int**)calloc(iWidth, sizeof(int*));
	for (int i = 0; i < iWidth; i++) {
		gameGridTemp[i] = (int*)calloc(iHeight, sizeof(int));
	}

	for (int i = 0; i < iWidth; i++) {
		for (int j = 0; j < iHeight; j++) {
			gameGridTemp[i][j] = gameGrid[i][j];
		}
	}
	// Acertos nas intera��es com limites laterais de janela de jogo
	if (x < 0) {
		x++;
		xPosE++;
		xPosD++;
	}
	if (x >= iWidth) {
		x--;
		xPosE--;
		xPosD--;
	}

	// Altura
	for (int i = 0; i < iPieceHeight; i++) {
		if (y + i > iHeight) {
			return false;
		}
		if (gameGrid[x][y + i] == 1) {
			gameGridTemp[x][y + i] = 3;
			for (int i = iHeight - 1; i >= 0; i--) {
				for (int j = 0; j < iWidth; j++) {
					cout << gameGridTemp[j][i] << ", ";
				}
				cout << endl;
			}
			cout << endl;
			return false;

		}
	}
	// Largura
	for (int i = xPosE; i < xPosD; i++) {
		if (gameGrid[i][y] == 1) {
			gameGridTemp[i][y] = 2;
			for (int i = iHeight - 1; i >= 0; i--) {
				for (int j = 0; j < iWidth; j++) {
					cout << gameGridTemp[j][i] << ", ";
				}
				cout << endl;
			}
			cout << endl;
			return false;
		}
	}

	// � possivel fazer a rota��o
	return true;
}

bool PecaI::avaliaColisao() {

	// Dimens�es e localiza��o da pe�a da pr�xima rota��o
	int iPieceHeight_AvaliaRotacaoSeguinte;
	int iPieceWidth_AvaliaRotacaoSeguinte;
	int yPos_AvaliaRotacaoSeguinte;
	int xPosE_AvaliaRotacaoSeguinte;

	// Reset de vari�veis
	bCollisionBottom = false;
	bCollisionLeft = false;
	bCollisionRight = false;
	bRotationAllowed = true;

	// Colis�o com bordas da janela de visualiza��o
	if (xPosE == 0) {
		bCollisionLeft = true;
	}
	if (xPosD == iWidth - 1) {
		bCollisionRight = true;
	}

	// Colis�o com base de jogo
	if (yPos == 0) {
		bCollisionBottom = true;
	}

	/* Colis�o com outras pe�as, tendo em considera��o a rota��o da pe�a
	Diferentes vari�veis s�o atualizadas, mediante o ponto de colis�o, avaliadas em registerUserInputs, em main.cpp */
	switch (iNumberRotate % 4) {
		case 0:
		case 2:
			// Bottom collision
			if ((gameGrid[xPosE][yPos - 1] == 1) || 
				(gameGrid[xPosE + 1][yPos - 1] == 1) ||
				(gameGrid[xPosE + 2][yPos - 1] == 1) ||
				(gameGrid[xPosE + 3][yPos - 1] == 1)){
					bCollisionBottom = true;
			}

			// Left collision
			if (xPosE - 1 >= 0) {
				if (gameGrid[xPosE - 1][yPos] == 1) {
					bCollisionLeft = true;
				}
			}

			// Right collision
			if(xPosE + 4 < iWidth) {
				if (gameGrid[xPosE + 4][yPos] == 1) {
					bCollisionRight = true;
				}
			}

			/* Calculando par�metros de acordo com rota��o seguinte, seguindo a mesma l�gica de atualizaMatriz, com os ajustes
			de atualizaPos */
			iPieceHeight_AvaliaRotacaoSeguinte = 4;
			iPieceWidth_AvaliaRotacaoSeguinte = 1;
			yPos_AvaliaRotacaoSeguinte = yPos - 1; // Igual em case 1 e 3
			xPosE_AvaliaRotacaoSeguinte =
				(iNumberRotate % 4 == 0) ?
				xPosE + 1 : // Case 1
				xPosE + 2 // Case 3
				;

			// Verificar rota��o por recurso a fun��o
			bRotationAllowed = avaliaPotencialRotacao(
				xPosE_AvaliaRotacaoSeguinte, yPos_AvaliaRotacaoSeguinte, 
				iPieceHeight_AvaliaRotacaoSeguinte, xPosE_AvaliaRotacaoSeguinte,
				(xPosE_AvaliaRotacaoSeguinte + iPieceWidth_AvaliaRotacaoSeguinte)
			);

			break;
		case 1:
		case 3:
			// Bottom collision
			if (gameGrid[xPosE][yPos - 1] == 1){
				bCollisionBottom = true;
			}

			// Left collision
			if (xPosE - 1 >= 0) {
				if ((gameGrid[xPosE - 1][yPos] == 1) || 
					(gameGrid[xPosE - 1][yPos + 1] == 1) ||
					(gameGrid[xPosE - 1][yPos + 2] == 1) ||
					(gameGrid[xPosE - 1][yPos + 3] == 1)) {
						bCollisionLeft = true;
				}
			}

			// Right collision
			if (xPosE + 1 < iWidth) {
				if ((gameGrid[xPosE + 1][yPos] == 1) || 
					(gameGrid[xPosE + 1][yPos + 1] == 1) ||
					(gameGrid[xPosE + 1][yPos + 2] == 1) || 
					(gameGrid[xPosE + 1][yPos + 3] == 1)) {
						bCollisionRight = true;
				}
			}

			/* Calculando par�metros de acordo com rota��o seguinte, seguindo a mesma l�gica de atualizaMatriz, com os ajustes
			de atualizaPos */
			iPieceHeight_AvaliaRotacaoSeguinte = 4;
			iPieceWidth_AvaliaRotacaoSeguinte = 1;
			xPosE_AvaliaRotacaoSeguinte = xPosE; // Igual em case 1 e 3
			yPos_AvaliaRotacaoSeguinte =
				(iNumberRotate % 4 == 1) ?
				yPos + 1 : // Case 2
				yPos // Case 0
				;

			// Verificar rota��o por recurso a fun��o
			bRotationAllowed = avaliaPotencialRotacao(
				xPosE_AvaliaRotacaoSeguinte, yPos_AvaliaRotacaoSeguinte,
				iPieceHeight_AvaliaRotacaoSeguinte, xPosE_AvaliaRotacaoSeguinte,
				(xPosE_AvaliaRotacaoSeguinte + iPieceWidth_AvaliaRotacaoSeguinte)
			);
		

			break;;
	}

	/* Apena agora ser� retornado o valor de colis�o para garantir que as restantes vari�veis (bCollisionLeft e bCollisionRight)
	s�o atualizadas de acordo com a situa��o de colis�o */
	if (bCollisionBottom) {
		return true;
	}
	return false;
}

void PecaI::atualizaPos() {
	// Posicao inicial
	switch (iNumberRotate % 4) {
		case 0:
			// Tamanho da pe�a
			iPieceWidth = 4;
			iPieceHeight = 1;

			// Ajuste de posi��es
			xPosD = xPosE + iPieceWidth;

			// Rota��o junto da parede do lado esquerdo
			if (xPosE < 0) {
				// Garantir que pe�a se mant�m dentro da janela de visualiza��o
				iNumberTranslation += 2;
				// Reajustar posi��o da pe�a resultante de ajuste
				xPosE += 2;
				xPosD +=2; 
			}

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
			iPieceWidth = 1;
			iPieceHeight = 4;

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
			iPieceWidth = 4;
			iPieceHeight = 1;

			// Ajuste de posi��es
			xPosD = xPosE + iPieceWidth;
			yPos++;

			// Rota��o junto da parede do lado esquerdo
			if (xPosE < 0) {
				// Garantir que pe�a se mant�m dentro da janela de visualiza��o
				iNumberTranslation++;
				// Reajustar posi��o da pe�a resultante de ajuste
				xPosE++;
				xPosD++;
			} 

			// Rota��o junto da parede do lado direito
			if (xPosD > iWidth) {
				// Garantir que pe�a se mant�m dentro da janela de visualiza��o
				iNumberTranslation--;
				// Reajustar posi��o da pe�a resultante de ajuste
				xPosE--;
				xPosD--;
			}

			break;

		case 3:
			// Tamanho da pe�a
			iPieceWidth = 1;
			iPieceHeight = 4;

			// Ajuste de posi��es
			xPosE += 2;
			xPosD = xPosE + iPieceWidth;
			yPos--;

			// Rota��o junto ao fundo da janela
			if (yPos <= 0) {
				// Garantir que pe�a se mant�m dentro da janela de visualiza��o 
				acertoPosicaoY += -yPos;
			}

			break;
	}
}

// glm::mat4& rot => alternativa a  glm::mat4 *rot aqui, com chamada de fun�ao &rot, ao inves de rot
void PecaI::rotacaoPeca(glm::mat4& rot) {

	// Transladar para origem da pe�a, rodar 90 graus e retornar � sua posi��o original 
	rot = glm::translate(rot, glm::vec3(xCR, yCR, 0.0f));
	rot = glm::rotate(rot, glm::radians(iNumberRotate * -90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	rot = glm::translate(rot, glm::vec3(-xCR, -yCR, 0.0f));

}

void PecaI::translacaoPeca(glm::mat4& trans) {

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

bool PecaI::hasCollidedBottom() {
	return bCollisionBottom;
}

bool PecaI::hasCollidedLeft() {
	return bCollisionLeft;
}

bool PecaI::hasCollidedRight() {
	return bCollisionRight;
}

bool PecaI::rotationAllowed() {
	return bRotationAllowed;
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
