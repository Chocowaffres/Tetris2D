#include "headers/PecaZ.hpp"
#include "headers/Peca.hpp"
#include <iostream>

// Construtores
PecaZ::PecaZ() {};

PecaZ::PecaZ(int xPosInicial, int yPosInicial, int iHeight, int iWidth, int** gameGrid, int iGameLevel) {

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
	bCollisionRight= false;
	bRotationAllowed = true;
	acertoPosicaoY = 0;

	gameLevel = iGameLevel;

	g_real_vertex_buffer = {};
};

// Vari�veis de classe
// Desenho da pe�a
std::vector<GLfloat> PecaZ::g_vertex_buffer_data = {
		//Z
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
};

// Textura da pe�a
std::vector<GLfloat> PecaZ::g_texture_buffer_data = {
		//Z
		0.0f,   0.0f,
		0.0f,   0.25f,
		0.25f,  0.0f,

		0.0f,   0.25f,
		0.25f,  0.0f,
		0.25f,  0.25f,

		//
		0.0f,   0.0f,
		0.0f,   0.25f,
		0.25f,  0.0f,

		0.0f,   0.25f,
		0.25f,  0.0f,
		0.25f,  0.25f,

		//
		0.0f,   0.0f,
		0.0f,   0.25f,
		0.25f,  0.0f,

		0.0f,   0.25f,
		0.25f,  0.0f,
		0.25f,  0.25f,

		//
		0.0f,   0.0f,
		0.0f,   0.25f,
		0.25f,  0.0f,

		0.0f,   0.25f,
		0.25f,  0.0f,
		0.25f,  0.25f,
};

std::vector<GLfloat> PecaZ::g_real_vertex_buffer = {};

// Textura da posi��o de colis�o da pe�a
std::vector<GLfloat> PecaZ::g_texture_buffer_dataPos = {
		//Z
		0.75f,  0.75f,
		1.0f,   0.75f,
		0.75f,  0.5f,

		1.0f,   0.75f,
		0.75f,  0.5f,
		1.0f,   0.5f,

		//
		0.75f,  0.75f,
		1.0f,   0.75f,
		0.75f,  0.5f,

		1.0f,   0.75f,
		0.75f,  0.5f,
		1.0f,   0.5f,

		//
		0.75f,  0.75f,
		1.0f,   0.75f,
		0.75f,  0.5f,

		1.0f,   0.75f,
		0.75f,  0.5f,
		1.0f,   0.5f,

		//
		0.75f,  0.75f,
		1.0f,   0.75f,
		0.75f,  0.5f,

		1.0f,   0.75f,
		0.75f,  0.5f,
		1.0f,   0.5f,
};

// Preenchimento do vertexBuffer de acordo com rota��o da pe�a e local de colis�o, para armazenamento do vertexBuffer
// das pe�as j� jogadas (em Projeto.cpp)
void PecaZ::preencheRealVertex(GLfloat x, GLfloat y) {
	g_real_vertex_buffer.push_back(x);
	g_real_vertex_buffer.push_back(y);
	g_real_vertex_buffer.push_back(0.0f);
}

// Modifica��o de um bloco/quadricula da pe�a, repesentado por 2 triangulos
void PecaZ::modificaQuadricula(GLfloat x, GLfloat y) {

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
void PecaZ::realVertexBuffer() {

	int xPosAntes = xPosE;
	int yPosAntes = yPos;

	switch (iNumberRotate % 4) {
		case 0:

			yPos++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			yPos--;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);
			break;
		case 1:

			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);
			break;
		case 2:

			yPos++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			yPos--;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);
			break;
		case 3:

			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);

			xPosE++;
			modificaQuadricula(xPosE, yPos);

			yPos++;
			modificaQuadricula(xPosE, yPos);
			break;
	}

	// Reposi��o de valores de vari�veis
	xPosE = xPosAntes;
	yPos = yPosAntes;
}

bool PecaZ::preencheMatrizCaso0e2(int x, int y) {
	// Andar de baixo
	x = x + 1;

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
	x = x - 1;

	for (int i = 0; i < iPieceWidth - 1; i++) {
		gameGrid[x + i][y] = 1;
	}
	return false;
}

bool PecaZ::preencheMatrizCaso1e3(int x, int y) {

	// Lado direito
	y = y + 1;
	x = x + 1;

	// Jogo acabou
	if (y > iHeight) {
		return true;
	}

	for (int i = 0; i < iPieceHeight - 1; i++) {
		// Jogo acabou
		if (y > iHeight) {
			return true;
		}
		gameGrid[x][y + i] = 1;	
	}

	// Lado esquerdo

	// Atualiza��o de y
	y = y - 1;
	// Atualiza��o de x
	x = x - 1;

	for (int i = 0; i < iPieceHeight - 1; i++) {
		gameGrid[x][y + i] = 1;
	}
	return false;
}

bool PecaZ::atualizaMatriz() {
	switch (iNumberRotate % 4) {
		case 0: return preencheMatrizCaso0e2(xPosE, yPos);
		case 1: return preencheMatrizCaso1e3(xPosE, yPos);
		case 2: return preencheMatrizCaso0e2(xPosE, yPos);
		case 3: return preencheMatrizCaso1e3(xPosE, yPos);
	}
}

bool PecaZ::avaliaPotencialRotacaoCaso0e2(int x, int y, int iPieceWidth) {

	// Acertos nas intera��es com limites laterais de janela de jogo
	if (x < 0) {
		x++;
	}
	if (x + iPieceWidth >= iWidth) {
		x--;
	}

	// Andar de baixo
	x = x + 1;
	
	for (int i = 0; i < iPieceWidth - 1; i++) {
		if (gameGrid[x + i][y] == 1) {
			return false;
		}
	}

	// Andar de cima

	// Atualiza��o de y
	y = y + 1;
	// Jogo acabou
	if (y > iHeight) {
		return false;
	}

	// Atualiza��o de x
	x = x - 1;

	for (int i = 0; i < iPieceWidth - 1; i++) {
		if (gameGrid[x + i][y] == 1) {
			return false;
		}
	}

	// � possivel fazer a rota��o
	return true;
}

bool PecaZ::avaliaPotencialRotacaoCaso1e3(int x, int y, int iPieceHeight) {

	// Acertos nas intera��es com limites laterais de janela de jogo
	if (x < 0) {
		x++;
	}
	// Largura de case 1 e 3 = (Altura de case 0 e 2) - 1
	if (x + (iPieceHeight - 1) >= iWidth) {
		x--;
	}

	// Lado direito
	y = y + 1;
	x = x + 1;


	// Jogo acabou
	if (y > iHeight) {
		return false;
	}

	for (int i = 0; i < iPieceHeight - 1; i++) {
		if (y > iHeight) {
			return false;
		}
		if (gameGrid[x][y + i] == 1) {
			return false;
		} 
	}

	// Lado esquerdo

	// Atualiza��o de y
	y = y - 1;
	// Atualiza��o de x
	x = x - 1;

	for (int i = 0; i < iPieceHeight - 1; i++) {
		if (gameGrid[x][y + i] == 1) {
			return false;
		}
	}

	// � possivel fazer a rota��o
	return true;
}

bool PecaZ::avaliaColisao() {

	// Dimens�es e localiza��o da pe�a da pr�xima rota��o
	int iVariavelCiclo_AvaliaRotacaoSeguinte;
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
		case 2:
			// Bottom collision
			if ((gameGrid[xPosE][yPos] == 1) || 
				(gameGrid[xPosE + 1][yPos - 1] == 1) || 
				(gameGrid[xPosE + 2][yPos - 1] == 1)){
					bCollisionBottom = true;
			}

			// Left collision
			if (xPosE - 1 >= 0) {
				if ((gameGrid[xPosE][yPos] == 1)  || (gameGrid[xPosE - 1][yPos + 1] == 1)) {
					bCollisionLeft = true;
				}
			}
	
			// Right collision
			if (xPosE + 3 < iWidth) {
				if ((gameGrid[xPosE + 3][yPos] == 1) || (gameGrid[xPosE + 2][yPos + 1] == 1)) {
					bCollisionRight = true;
				}
			}

			/* C�lculo de par�metros de acordo com rota��o seguinte, seguindo a mesma l�gica de atualizaMatriz, com os ajustes necess�rios */
			iVariavelCiclo_AvaliaRotacaoSeguinte = 3;
			yPos_AvaliaRotacaoSeguinte = 
				(iNumberRotate % 4 == 0) ?
				yPos - 1 : // Case 1
				yPos // Case 3
				; 
			xPosE_AvaliaRotacaoSeguinte = 
				(iNumberRotate % 4 == 0) ? 
					xPosE + 1: // Case 1
					xPosE // Case 3
				;

			// Verificar rota��o por recurso a fun��o
			bRotationAllowed = avaliaPotencialRotacaoCaso1e3(
				xPosE_AvaliaRotacaoSeguinte, yPos_AvaliaRotacaoSeguinte, iVariavelCiclo_AvaliaRotacaoSeguinte
			);

			break;
		case 1: 
		case 3:
			// Bottom collision
			if ((gameGrid[xPosE][yPos - 1] == 1) || 
				(gameGrid[xPosE + 1][yPos] == 1)) {
					bCollisionBottom = true;
			}

			// Left collision
			if (xPosE - 1 >= 0) {
				if ((gameGrid[xPosE - 1][yPos] == 1) ||
					(gameGrid[xPosE - 1][yPos + 1] == 1) ||
					(gameGrid[xPosE][yPos + 2] == 1)) {
						bCollisionLeft = true;
				}
			}

			// Right 
			if (gameGrid[xPosE + 1][yPos] == 1) {
				bRotationAllowed = false;
				bCollisionRight = true;
			}
			if (xPosE + 2 < iWidth) {
				if ((gameGrid[xPosE + 1][yPos] == 1) ||
					(gameGrid[xPosE + 2][yPos + 1] == 1) ||
					(gameGrid[xPosE + 2][yPos + 2] == 1)){
						bCollisionRight = true;
				}
			}

			/* C�lculo de par�metros de acordo com rota��o seguinte, seguindo a mesma l�gica de atualizaMatriz, com os ajustes necess�rios */
			iVariavelCiclo_AvaliaRotacaoSeguinte = 3;
			yPos_AvaliaRotacaoSeguinte = 
				(iNumberRotate % 4 == 1) ?
				yPos : // Case 2
				yPos + 1 // Case 0
				;
			xPosE_AvaliaRotacaoSeguinte = 
				(iNumberRotate % 4 == 1) ?
				xPosE - 1 : // Case 2
				xPosE // Case 0
				; 

			// Verificar rota��o por recurso a fun��o
			bRotationAllowed = avaliaPotencialRotacaoCaso0e2(
				xPosE_AvaliaRotacaoSeguinte, yPos_AvaliaRotacaoSeguinte, iVariavelCiclo_AvaliaRotacaoSeguinte
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

void PecaZ::atualizaPos() {
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

			// Caso em que � necess�rio garantir que pe�a n�o desce para fora da janela de visualiza��o
			if (yPos <= 0) {
				// Garantir que pe�a se mant�m dentro da janela de visualiza��o 
				acertoPosicaoY += -yPos;
			}

			break;
	}
}

// glm::mat4& rot => alternativa a  glm::mat4 *rot aqui, com chamada de fun�ao &rot, ao inves de rot
void PecaZ::rotacaoPeca(glm::mat4& rot) {

	// Transladar para origem da pe�a, rodar 90 graus e retornar � sua posi��o original 
	rot = glm::translate(rot, glm::vec3(xCR, yCR, 0.0f));
	rot = glm::rotate(rot, glm::radians(iNumberRotate * -90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	rot = glm::translate(rot, glm::vec3(-xCR, -yCR, 0.0f));

}

// Modificar o valor de tempo de acordo com o n�vel de jogo
int PecaZ::dropAccordingToLevel(double x){
	// 0.5 fator de multiplica��o, visando melhor jogabilidade
	// + 1, pois o primeiro n�vel � 0
	return x * (gameLevel*0.5 + 1); 
}

void PecaZ::translacaoPeca(glm::mat4& trans) {

	auto t_now = std::chrono::high_resolution_clock::now();
	double timeDouble = std::chrono::duration_cast<std::chrono::duration<double>>(t_now - t_start).count();
	int time = dropAccordingToLevel(timeDouble);

	/* Se houve colis�o, n�o reajustar vari�vel associada a tempo, para garantir que pe�a se mant�m
	posi��o onde estava no momento de colis�o. "time" influencia a descida da pe�a, da� ter esta avalia��o */
	if (bCollisionBottom) {
		time = oldValueTime;
	}

	// Pe�a desce pelo ecr�, a cada segundo
	yPos = yPosInicial - time  - iNumberDown * .5 + acertoPosicaoY;

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

int PecaZ::collisionYPos() {

	// Vari�veis de acerto de acordo com rota��o da pe�a
	int yPos_Atual = yPos;
	int iAltura, iLargura;
	int valorRetorno = 0;
	int xPosE_Acerto;

	/* Avaliar colis�o de Y, mediante a rota��o. Haver� acerto das posi��es de X e diferentes avalia��es mediante os pontos 
	de colis�o associadas. Haver� um valor de retorno por defeito em cada caso */
	switch (iNumberRotate % 4) {
		case 0:
			iAltura = 2;
			iLargura = 3;
			// Acerto na lateral da janela
			xPosE_Acerto = xPosE;
			if ((xPosE + iLargura) > iWidth) {
				xPosE_Acerto--;
			}
			for (int i = valorRetorno; i < yPos_Atual + iAltura - 1; i++) {
				if ((gameGrid[xPosE_Acerto][i] == 1) ||
					(gameGrid[xPosE_Acerto + 1][i - 1] == 1) ||
					(gameGrid[xPosE_Acerto + 2][i - 1] == 1)) {
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
				if ((gameGrid[xPosE_Acerto][i] == 1) ||
					(gameGrid[xPosE_Acerto + 1][i - 1] == 1) ||
					(gameGrid[xPosE_Acerto + 2][i - 1] == 1)) {
						valorRetorno = i;
				}
			}
			valorRetorno++;
			break;
		case 1:
		case 3:
			iAltura = 3;
			iLargura = 2;
			valorRetorno = 1;
			for (int i = valorRetorno; i < yPos_Atual + iAltura - 1; i++) {
				if ((gameGrid[xPosE][i - 2] == 1) || (gameGrid[xPosE + 1][i - 1] == 1)) {
					valorRetorno = i;
				}
			}
			break;
	}

	return valorRetorno;
}

void PecaZ::translacaoPecaContorno(glm::mat4& trans) {
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

void PecaZ::drawObject() {
	glDrawArrays(GL_TRIANGLES, 0, 8*3);
}

// Getters
int** PecaZ::getGameGrid() {
	return gameGrid;
}

int PecaZ::getNumberRotate() {
	return iNumberRotate;
}

int PecaZ::getNumberTranslation() {
	return iNumberTranslation;
}

int PecaZ::getNumberDown() {
	return iNumberDown;
}

int PecaZ::getXPosD() {
	return xPosD;
}

int PecaZ::getXPosE() {
	return xPosE;
}

bool PecaZ::hasCollidedBottom() {
	return bCollisionBottom;
}

bool PecaZ::hasCollidedLeft() {
	return bCollisionLeft;
}

bool PecaZ::hasCollidedRight() {
	return bCollisionRight;
}

bool PecaZ::rotationAllowed() {
	return bRotationAllowed;
}

// Atualizadores

void PecaZ::incNumberRotate() {
	iNumberRotate++;
}

void PecaZ::incNumberTranslation() {
	iNumberTranslation++;
}

void PecaZ::decNumberTranslation() {
	iNumberTranslation--;
}

void PecaZ::incNumberDown() {
	iNumberDown++;
}
