#include "headers/PecaL.hpp"
#include <iostream>

// Construtores
PecaL::PecaL() {};

PecaL::PecaL(int xPosInicial, int yPosInicial, int iHeight, int iWidth, int** gameGrid, int iGameLevel) {

	// Vari�veis da pe�a
	xCR = 1.5;
	yCR = 0.5;
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

	gameLevel = iGameLevel;

	g_real_vertex_buffer = {};
};

// Vari�veis de classe
// Desenho da pe�a
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

// Textura da pe�a
std::vector<GLfloat> PecaL::g_texture_buffer_data = {
		//L
		0.25f,  0.25f,
		0.25f,  0.5f,
		0.5f,   0.25f,

		0.25f,  0.5f,
		0.5f,   0.25f,
		0.5f,   0.5f,

		//
		0.25f,  0.25f,
		0.25f,  0.5f,
		0.5f,   0.25f,

		0.25f,  0.5f,
		0.5f,   0.25f,
		0.5f,   0.5f,

		//
		0.25f,  0.25f,
		0.25f,  0.5f,
		0.5f,   0.25f,

		0.25f,  0.5f,
		0.5f,   0.25f,
		0.5f,   0.5f,

		//
		0.25f,  0.25f,
		0.25f,  0.5f,
		0.5f,   0.25f,

		0.25f,  0.5f,
		0.5f,   0.25f,
		0.5f,   0.5f,
};

std::vector<GLfloat> PecaL::g_real_vertex_buffer = {};

// Textura da posi��o de colis�o da pe�a
std::vector<GLfloat> PecaL::g_texture_buffer_dataPos = {
		//L
		0.25f,  0.75f,
		0.5f,   0.75f,
		0.25f,  0.5f,

		0.5f,   0.75f,
		0.25f,  0.5f,
		0.5f,   0.5f,

		//
		0.25f,  0.75f,
		0.5f,   0.75f,
		0.25f,  0.5f,

		0.5f,   0.75f,
		0.25f,  0.5f,
		0.5f,   0.5f,

		//
		0.25f,  0.75f,
		0.5f,   0.75f,
		0.25f,  0.5f,

		0.5f,   0.75f,
		0.25f,  0.5f,
		0.5f,   0.5f,

		//
		0.25f,  0.75f,
		0.5f,   0.75f,
		0.25f,  0.5f,

		0.5f,   0.75f,
		0.25f,  0.5f,
		0.5f,   0.5f,
};

// Preenchimento do vertexBuffer de acordo com rota��o da pe�a e local de colis�o, para armazenamento do vertexBuffer
// das pe�as j� jogadas (em Projeto.cpp)
void PecaL::preencheRealVertex(GLfloat x, GLfloat y) {
	g_real_vertex_buffer.push_back(x);
	g_real_vertex_buffer.push_back(y);
	g_real_vertex_buffer.push_back(0.0f);
}

// Modifica��o de um bloco/quadricula da pe�a, repesentado por 2 triangulos
void PecaL::modificaQuadricula(GLfloat x, GLfloat y) {

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

	// Reposi��o de valores de vari�veis
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
		// Rota��o excediria ecra de jogo
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

bool PecaL::avaliaColisao() {

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

	// Colis�o com base de jogo
	if (yPos == 0) {
		bCollisionBottom = true;
	}

	/* Colis�o com outras pe�as, tendo em considera��o a rota��o da pe�a
	Diferentes vari�veis s�o atualizadas, mediante o ponto de colis�o, avaliadas em registerUserInputs, em main.cpp */
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

			/* C�lculo de par�metros de acordo com rota��o seguinte, seguindo a mesma l�gica de atualizaMatriz, com os ajustes necess�rios */
			iPieceHeight_AvaliaRotacaoSeguinte = 3;
			iPieceWidth_AvaliaRotacaoSeguinte = 2;
			xPosE_AvaliaRotacaoSeguinte = xPosE + 1;
			yPos_AvaliaRotacaoSeguinte = yPos - 1;

			// Verificar rota��o por recurso a fun��o
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

			/* C�lculo de par�metros de acordo com rota��o seguinte, seguindo a mesma l�gica de atualizaMatriz, com os ajustes necess�rios */
			iPieceHeight_AvaliaRotacaoSeguinte = 2;
			iPieceWidth_AvaliaRotacaoSeguinte = 3;
			xPosE_AvaliaRotacaoSeguinte = xPosE - 1;
			yPos_AvaliaRotacaoSeguinte = yPos;

			// Verificar rota��o por recurso a fun��o
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

			/* C�lculo de par�metros de acordo com rota��o seguinte, seguindo a mesma l�gica de atualizaMatriz, com os ajustes necess�rios */
			iPieceHeight_AvaliaRotacaoSeguinte = 3;
			iPieceWidth_AvaliaRotacaoSeguinte = 2;
			xPosE_AvaliaRotacaoSeguinte = xPosE;
			yPos_AvaliaRotacaoSeguinte = yPos;

			// Verificar rota��o por recurso a fun��o
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

			/* C�lculo de par�metros de acordo com rota��o seguinte, seguindo a mesma l�gica de atualizaMatriz, com os ajustes necess�rios */
			iPieceHeight_AvaliaRotacaoSeguinte = 2;
			iPieceWidth_AvaliaRotacaoSeguinte = 3;
			xPosE_AvaliaRotacaoSeguinte = xPosE;
			yPos_AvaliaRotacaoSeguinte = yPos + 1;

			// Verificar rota��o por recurso a fun��o
			bRotationAllowed = avaliaPotencialRotacao(
				xPosE_AvaliaRotacaoSeguinte, yPos_AvaliaRotacaoSeguinte, iPieceWidth_AvaliaRotacaoSeguinte - 1, 0,
				iPieceHeight_AvaliaRotacaoSeguinte, xPosE_AvaliaRotacaoSeguinte,
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

void PecaL::atualizaPos() {
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
void PecaL::rotacaoPeca(glm::mat4& rot) {

	// Transladar para origem da pe�a, rodar 90 graus e retornar � sua posi��o original 
	rot = glm::translate(rot, glm::vec3(xCR, yCR, 0.0f));
	rot = glm::rotate(rot, glm::radians(iNumberRotate * -90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	rot = glm::translate(rot, glm::vec3(-xCR, -yCR, 0.0f));
}

// Modificar o valor de tempo de acordo com o n�vel de jogo
int PecaL::dropAccordingToLevel(double x) {
	// 0.5 fator de multiplica��o, visando melhor jogabilidade
	// + 1, pois o primeiro n�vel � 0
	return x * (gameLevel*0.5 + 1);
}

void PecaL::translacaoPeca(glm::mat4& trans) {

	auto t_now = std::chrono::high_resolution_clock::now();
	double timeDouble = std::chrono::duration_cast<std::chrono::duration<double>>(t_now - t_start).count();
	int time = dropAccordingToLevel(timeDouble);

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

// ---------------------------------------------------------------------------------

int PecaL::collisionYPos() {

	// Vari�veis de acerto de acordo com rota��o da pe�a
	int yPos_Atual = yPos;
	int iAltura, iLargura;
	int valorRetorno = 0;
	int xPosE_Acerto;

	/* Avaliar colis�o de Y, mediante a rota��o. Haver� acerto das posi��es de X e diferentes avalia��es mediante os pontos
	de colis�o associadas. Haver� um valor de retorno por defeito em cada caso */
	switch (iNumberRotate % 4) {
		case 0:
			iAltura = 3;
			iLargura = 2;
			// Acerto na lateral da janela
			xPosE_Acerto = xPosE;
			if ((xPosE + iLargura) >= iWidth) {
				xPosE_Acerto--;
			}
			for (int i = valorRetorno; i < yPos_Atual + iAltura - 1; i++) {
				if ((gameGrid[xPosE_Acerto][i - 1] == 1) ||
					(gameGrid[xPosE_Acerto + 1][i - 1] == 1) ||
					(gameGrid[xPosE_Acerto + 2][i - 1] == 1)) {
					valorRetorno = i;
				}
			}
			break;
		case 1:
			iAltura = 3;
			iLargura = 2;
			valorRetorno = 1;
			for (int i = valorRetorno; i < yPos_Atual + iAltura - 1; i++) {
				if ((gameGrid[xPosE][i - 2] == 1) ||
					(gameGrid[xPosE + 1][i - 2] == 1)) {
					valorRetorno = i;
				}
			}
			break;
		case 2:
			iAltura = 2;
			iLargura = 3;
			// Acerto na lateral da janela
			xPosE_Acerto = xPosE;
			if ((xPosE + iLargura) > iWidth) {
				xPosE_Acerto--;
			}

			for (int i = valorRetorno; i < yPos_Atual + iAltura - 1; i++) {
				if ((gameGrid[xPosE_Acerto][i - 1] == 1) ||
					(gameGrid[xPosE_Acerto + 1][i] == 1) ||
					(gameGrid[xPosE_Acerto + 2][i] == 1)) {
					valorRetorno = i;
				}
			}
			valorRetorno++;
			break;
		case 3:
			iAltura = 3;
			iLargura = 2;
			valorRetorno = 1;
			for (int i = valorRetorno; i < yPos_Atual + iAltura - 1; i++) {
				if ((gameGrid[xPosE][i] == 1) ||
					(gameGrid[xPosE + 1][i - 2] == 1)) {
					valorRetorno = i;
				}
			}
			break;
	}

	return valorRetorno;
}

void PecaL::translacaoPecaContorno(glm::mat4& trans) {
	// Dimens�es e localiza��o da pe�a da pr�xima rota��o
	int xPosE_Acerto, yPos_Acerto;

	// Acerto de xPosE de acordo com rota��o
	switch (iNumberRotate % 4) {
		case 0: xPosE_Acerto = xPosE; break;
		case 1: xPosE_Acerto = xPosE - 1; break;
		case 2: xPosE_Acerto = xPosE; break;
		case 3: xPosE_Acerto = xPosE; break;
	}
	// Altura do ponto de colis�o 
	yPos_Acerto = collisionYPos();
	// Transla��o de pe�a de contornos para posi��o correta da grelha
	trans = glm::translate(trans, glm::vec3(xPosE_Acerto, yPos_Acerto, 0.0f));
}

// ---------------------------------------------------------------------------------

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
