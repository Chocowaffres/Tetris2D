#include "headers/PecaS.hpp"
#include <iostream>

// Construtores
PecaS::PecaS() {};

PecaS::PecaS(int xPosInicial, int yPosInicial, int iHeight, int iWidth, int** gameGrid, int iGameLevel) {

	// Variaveis da peca
	xCR = 1.5f;
	yCR = 0.5f;
	iPieceWidth = 3;
	iPieceHeight = 2;

	yPos = yPosInicial;
	xPosE = xPosInicial;
	xPosD = xPosInicial + iPieceWidth;

	this->gameGrid = (int**)calloc(iWidth, sizeof(int*));
	for (int i = 0; i < iWidth; i++) {
		this->gameGrid[i] = (int*)calloc(iHeight, sizeof(int));
	}

	for (int i = 0; i < iWidth; i++) {
		for (int j = 0; j < iHeight; j++) {
			this->gameGrid[i][j] = gameGrid[i][j];
		}
	}

	// Verificar se, aquando da instanciacao da peca, esta iria colidir com alguma ja presente no tabuleiro
	// Para tal, acertar yPosInicial, subindo uma posicao na posicao de instancia de peca
	for (int i = 0; i < iWidth; i++) {
		if (gameGrid[i][yPos] == 1) {
			yPosInicial++;
			yPos = yPosInicial;
			break;
		}
	}

	// Variaveis de tabuleiro
	this->xPosInicial = xPosInicial;
	this->yPosInicial = yPosInicial;
	this->iHeight = iHeight;
	this->iWidth = iWidth;

	// Variaveis de interacao com user
	iNumberRotate = 0;
	iNumberTranslation = 0;
	iNumberDown = 0;

	t_start = std::chrono::high_resolution_clock::now();

	// Variaveis associadas a temporizador de colisao, visando melhor jogabilidade
	oldValueTime = 0;
	bCollisionBottom = false;
	bCollisionLeft = false;
	bCollisionRight = false;
	bRotationAllowed = true;
	acertoPosicaoY = 0;

	gameLevel = iGameLevel;

	g_real_vertex_buffer = {};
};

// Variaveis de classe
/// Desenho da peca
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

/// Textura da peca
std::vector<GLfloat> PecaS::g_texture_buffer_data = {
		//S
		0.0f,   0.5f,
		0.0f,   0.75f,
		0.25f,  0.5f,

		0.0f,   0.75f,
		0.25f,  0.5f,
		0.25f,  0.75f,

		//
		0.0f,   0.5f,
		0.0f,   0.75f,
		0.25f,  0.5f,

		0.0f,   0.75f,
		0.25f,  0.5f,
		0.25f,  0.75f,

		//
		0.0f,   0.5f,
		0.0f,   0.75f,
		0.25f,  0.5f,

		0.0f,   0.75f,
		0.25f,  0.5f,
		0.25f,  0.75f,

		//
		0.0f,   0.5f,
		0.0f,   0.75f,
		0.25f,  0.5f,

		0.0f,   0.75f,
		0.25f,  0.5f,
		0.25f,  0.75f,
};

std::vector<GLfloat> PecaS::g_real_vertex_buffer = {};

/// Textura da posicao de colisao da peca
std::vector<GLfloat> PecaS::g_texture_buffer_dataPos = {
		//S
		0.0f,   1.0f,
		0.25f,  1.0f,
		0.0f,   0.75f,

		0.25f,  1.0f,
		0.0f,   0.75f,
		0.25f,  0.75f,

		//
		0.0f,   1.0f,
		0.25f,  1.0f,
		0.0f,   0.75f,

		0.25f,  1.0f,
		0.0f,   0.75f,
		0.25f,  0.75f,

		//
		0.0f,   1.0f,
		0.25f,  1.0f,
		0.0f,   0.75f,

		0.25f,  1.0f,
		0.0f,   0.75f,
		0.25f,  0.75f,

		//
		0.0f,   1.0f,
		0.25f,  1.0f,
		0.0f,   0.75f,

		0.25f,  1.0f,
		0.0f,   0.75f,
		0.25f,  0.75f,
};

/// Preenchimento do vertexBuffer de acordo com rotacao da peca e local de colisao, para armazenamento do vertexBuffer
/// das pecas ja jogadas (em Projeto.cpp)
void PecaS::preencheRealVertex(GLfloat x, GLfloat y) {
	g_real_vertex_buffer.push_back(x);
	g_real_vertex_buffer.push_back(y);
	g_real_vertex_buffer.push_back(0.0f);
}

// Modificacao de um bloco/quadricula da peca, repesentado por 2 triangulos
void PecaS::modificaQuadricula(GLfloat x, GLfloat y) {

	// 3 vertice do triangulo inferior da quadricula
	preencheRealVertex(x, y);
	preencheRealVertex(x + 1, y);
	preencheRealVertex(x, y + 1);

	// 3 vertice do triangulo superior da quadricula
	preencheRealVertex(x + 1, y);
	preencheRealVertex(x, y + 1);
	preencheRealVertex(x + 1, y + 1);
}

/// Peca quando desenhada esta na origem. A colisao de peca, promove a invocacao deste metodo que tratara de representar a peca,
/// sob a forma de vertexBuffer, tendo em conta local onde esta colidiu e a rotacao da peca.
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

	// Atualizacao de y
	y = y + 1;

	// Jogo acabou
	if (y > iHeight - 2) {
		return true;
	}

	// Atualizacao de x
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
	if (y > iHeight - 2) {
		return true;
	}
	for (int i = 0; i < iPieceHeight - 1; i++) {
		gameGrid[x][y + i] = 1;
		// Jogo acabou
		if (y > iHeight - 2) {
			return true;
		}
	}

	// Lado direito

	// Atualizacao de y
	y = y - 1;
	// Atualizacao de x
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

bool PecaS::avaliaPotencialRotacaoCaso0e2(int x, int y, int iPieceWidth) {

	// Acertos nas interacoes com limites laterais de janela de jogo
	if (x < 0) {
		x++;
	}
	if (x + iPieceWidth >= iWidth) {
		x--;
	}

	// Andar de baixo
	for (int i = 0; i < iPieceWidth - 1; i++) {
		if (gameGrid[x + i][y] == 1) {
			return false;
		}
	}

	// Andar de cima

	// Atualizacao de y
	y = y + 1;

	// Jogo acabou
	if (y > iHeight - 2) {
		return true;
	}

	// Atualizacao de x
	x = x + 1;

	for (int i = 0; i < iPieceWidth - 1; i++) {
		if (gameGrid[x + i][y] == 1) {
			return false;
		}
	}
	
	// e possivel fazer a rotacao
	return true;
}

bool PecaS::avaliaPotencialRotacaoCaso1e3(int x, int y, int iPieceHeight) {

	// Acertos nas interacoes com limites laterais de janela de jogo
	if (x < 0) {
		x++;
	}

	// Largura de case 1 e 3 = (Altura de case 0 e 2) - 1
	if (x + (iPieceHeight - 1) >= iWidth) {
		x--;
	}

	// Lado esquerdo
	y = y + 1;

	// Jogo acabou
	if (y > iHeight - 2) {
		return true;
	}
	for (int i = 0; i < iPieceHeight - 1; i++) {
		// Jogo acabou
		if (y > iHeight - 2) {
			return true;
		}
		if (gameGrid[x][y + i] == 1) {
			return false;
		}
	}

	// Lado direito

	// Atualizacao de y
	y = y - 1;
	// Atualizacao de x
	x = x + 1;

	// Acertos nas interacoes com limites laterais de janela de jogo
	if (x >= iWidth) {
		x--;
	}


	for (int i = 0; i < iPieceHeight - 1; i++) {
		if (gameGrid[x][y + i] == 1) {
			return false;
		}
	}

	// e possivel fazer a rotacao
	return true;
}


bool PecaS::avaliaColisao() {

	// Dimensoes e localizacao da peca da proxima rotacao
	int iVariavelCiclo_AvaliaRotacaoSeguinte;
	int yPos_AvaliaRotacaoSeguinte;
	int xPosE_AvaliaRotacaoSeguinte;

	// Reset de variaveis
	bCollisionBottom = false;
	bCollisionLeft = false;
	bCollisionRight = false;
	bRotationAllowed = true;

	// Colisao com base de jogo
	if (yPos == 0) {
		bCollisionBottom = true;
	}

	/* Colisao com outras pecas, tendo em consideracao a rotacao da peca
	Diferentes variaveis sao atualizadas, mediante o ponto de colisao, avaliadas em registerUserInputs, em main.cpp */
	switch (iNumberRotate % 4) {
		case 0:
		case 2:
			// Bottom collision
			if ((gameGrid[xPosE][yPos - 1] == 1) ||
				(gameGrid[xPosE + 1][yPos - 1] == 1) ||
				(gameGrid[xPosE + 2][yPos] == 1)){
					bCollisionBottom = true;
			}

			// Left collision
			if (xPosE - 1 >= 0) {
				if ((gameGrid[xPosE - 1][yPos] == 1) ||
					(gameGrid[xPosE][yPos + 1] == 1)) {
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
			/* Calculo de parametros de acordo com rotacao seguinte, seguindo a mesma logica de atualizaMatriz, com os ajustes necessarios */
			iVariavelCiclo_AvaliaRotacaoSeguinte = 3;
			yPos_AvaliaRotacaoSeguinte = 
				(iNumberRotate % 4 == 0) ?
				yPos - 1 : // Case 1
				yPos // Case 3
				;
			xPosE_AvaliaRotacaoSeguinte =
				(iNumberRotate % 4 == 0) ?
				xPosE + 1 : // Case 1
				xPosE // Case 3
				;

			// Verificar rotacao por recurso a funcao
			bRotationAllowed = avaliaPotencialRotacaoCaso1e3(
				xPosE_AvaliaRotacaoSeguinte, yPos_AvaliaRotacaoSeguinte, iVariavelCiclo_AvaliaRotacaoSeguinte
			);

			break;
		case 1:
		case 3:
			// Bottom collision
			if ((gameGrid[xPosE][yPos] == 1) ||
				(gameGrid[xPosE + 1][yPos - 1] == 1)){
					bCollisionBottom = true;
			}


			// Left collision
			if (xPosE - 1 >= 0) {
				if ((gameGrid[xPosE][yPos] == 1) ||
					(gameGrid[xPosE - 1][yPos + 1] == 1) || 
					(gameGrid[xPosE - 1][yPos + 2] == 1)) {
						bCollisionLeft = true;
				}
			}

			// Right collision
			if (xPosE + 2 < iWidth) {
				if ((gameGrid[xPosE + 1][yPos + 2] == 1) ||
					(gameGrid[xPosE + 2][yPos] == 1) ||
					(gameGrid[xPosE + 2][yPos + 1] == 1)) {
						bCollisionRight = true;
				}
			}

			/* Calculo de parametros de acordo com rotacao seguinte, seguindo a mesma logica de atualizaMatriz, com os ajustes necessarios */
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

			// Verificar rotacao por recurso a funcao
			bRotationAllowed = avaliaPotencialRotacaoCaso0e2(
				xPosE_AvaliaRotacaoSeguinte, yPos_AvaliaRotacaoSeguinte, iVariavelCiclo_AvaliaRotacaoSeguinte
			);

			break;
	}

	/* Apena agora sera retornado o valor de colisao para garantir que as restantes variaveis (bCollisionLeft e bCollisionRight)
	sao atualizadas de acordo com a situacao de colisao */
	if (bCollisionBottom) {
		return true;
	}
	return false;
}

void PecaS::atualizaPos() {
	switch (iNumberRotate % 4) {
		case 0:
			// Tamanho da peca
			iPieceWidth = 3;
			iPieceHeight = 2;

			// Ajuste de posicoes
			xPosD = xPosE + iPieceWidth;
	
			if (xPosE < 0) {
				// Garantir que peca se mantem dentro da janela de visualizacao 
				iNumberTranslation++;
				// Reajustar posicao da peca resultante de ajuste
				xPosE++;
				xPosD++;
			}
			if (xPosD > iWidth) {
				// Garantir que peca se mantem dentro da janela de visualizacao 
				iNumberTranslation--;
				// Reajustar posicao da peca resultante de ajuste
				xPosE--;
				xPosD--;
			}
			break;

		case 1:
			// Tamanho da peca
			iPieceWidth = 2;
			iPieceHeight = 3;

			// Ajuste de posicoes
			xPosE++; // Peca avanca relativamente a posicao standart da peca
			xPosD = xPosE + iPieceWidth;
			yPos--;

			// Caso em que e necessario garantir que peca nao desce para fora da janela de visualizacao
			if (yPos <= 0) {
				// Garantir que peca se mantem dentro da janela de visualizacao 
				acertoPosicaoY += -yPos;
			}
		
			break;
		case 2:
			// Tamanho da peca
			iPieceWidth = 3;
			iPieceHeight = 2;

			// Ajuste de posicoes
			xPosD = xPosE + iPieceWidth;
			yPos--;

			if (xPosE < 0) {
				// Garantir que peca se mantem dentro da janela de visualizacao
				iNumberTranslation++;
				// Reajustar posicao da peca resultante de ajuste
				xPosE++;
				xPosD++;
			}
			break;

		case 3:
			// Tamanho da peca
			iPieceWidth = 2;
			iPieceHeight = 3;

			// Ajuste de posicoes
			xPosD = xPosE + iPieceWidth;
			yPos--;

			if (xPosE < 0) {
				// Garantir que peca se mantem dentro da janela de visualizacao
				iNumberTranslation++;
				// Reajustar posicao da peca resultante de ajuste
				xPosE++;
				xPosD++;
			}

			// Caso em que e necessario garantir que peca nao desce para fora da janela de visualizacao
			if (yPos <= 0) {
				// Garantir que peca se mantem dentro da janela de visualizacao 
				acertoPosicaoY += -yPos;
			}
			break;
		}
}

/// glm::mat4& rot => alternativa a  glm::mat4 *rot aqui, com chamada de funcao &rot, ao inves de rot
void PecaS::rotacaoPeca(glm::mat4& rot) {

	// Transladar para origem da peca, rodar 90 graus e retornar a sua posicao original 
	rot = glm::translate(rot, glm::vec3(xCR, yCR, 0.0f));
	rot = glm::rotate(rot, glm::radians(iNumberRotate * -90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	rot = glm::translate(rot, glm::vec3(-xCR, -yCR, 0.0f));

}

/// Modificar o valor de tempo de acordo com o nivel de jogo
int PecaS::dropAccordingToLevel(double x) {
	// 0.5 fator de multiplicacao, visando melhor jogabilidade
	// + 1, pois o primeiro nivel e 0
	return x * (gameLevel*0.5 + 1);
}

void PecaS::translacaoPeca(glm::mat4& trans, bool bPause) {

	auto t_now = std::chrono::high_resolution_clock::now();
	double timeDouble = std::chrono::duration_cast<std::chrono::duration<double>>(t_now - t_start).count();
	int time = dropAccordingToLevel(timeDouble);

	/* Se houve colisao, nao reajustar variavel associada a tempo, para garantir que peca se mantem
	posicao onde estava no momento de colisao. "time" influencia a descida da peca, dai ter esta avaliacao */
	if (bCollisionBottom) {
		time = oldValueTime;
	}

	// Peca desce pelo ecra, a cada segundo
	yPos = yPosInicial - time - iNumberDown * .5 + acertoPosicaoY;

	// Parar a peca em caso de pause
	if (bPause) {
		yPosInicial = yPos;
		iNumberDown = 0;
		t_start = std::chrono::high_resolution_clock::now();
	}

	// Trata das translacoes para esquerda e direita ("+" -> direita, "-" -> esquerda)
	xPosE = xPosInicial + iNumberTranslation;

	trans = glm::translate(trans, glm::vec3(xPosE, yPos, 0.0f));

	/* Atualiza posicoes para avaliacao de colisoes, preenchimento de matriz e proxima iteracao de draw
	 (no caso de alteracao relativamente a iNumberTranslation) */
	atualizaPos();

	// Caso nao tenha havido colisao, atualizar variavel de tempo anterior.
	if (!bCollisionBottom) {
		oldValueTime = time;
	}
}

// ---------------------------------------------------------------------------------

int PecaS::collisionYPos() {

	// Variaveis de acerto de acordo com rotacao da peca
	int yPos_Atual = yPos;
	int iAltura, iLargura;
	int valorRetorno = 0;
	int xPosE_Acerto;

	/* Avaliar colisao de Y, mediante a rotacao. Havera acerto das posicoes de X e diferentes avaliacoes mediante os pontos
	de colisao associadas. Havera um valor de retorno por defeito em cada caso */
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
				(gameGrid[xPosE_Acerto + 2][i] == 1)) {
				valorRetorno = i;
			}
		}
		break;

	case 2:
		iAltura = 2;
		iLargura = 3;
		valorRetorno = valorRetorno;
		// Acerto na lateral da janela
		xPosE_Acerto = xPosE;
		if ((xPosE + iLargura) > iWidth) {
			xPosE_Acerto--;
		}
		
		for (int i = 0; i < yPos_Atual + iAltura - 1; i++) {
			if ((gameGrid[xPosE_Acerto][i - 1] == 1) ||
				(gameGrid[xPosE_Acerto + 1][i - 1] == 1) ||
				(gameGrid[xPosE_Acerto + 2][i] == 1)) {
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
			if ((gameGrid[xPosE][i - 1] == 1) ||
				(gameGrid[xPosE + 1][i - 2] == 1)) {
				valorRetorno = i;
			}
		}
		break;
	}

	return valorRetorno;
}

void PecaS::translacaoPecaContorno(glm::mat4& trans) {
	// Dimensoes e localizacao da peca da proxima rotacao
	int xPosE_Acerto, yPos_Acerto;

	// Acerto de xPosE de acordo com rotacao
	switch (iNumberRotate % 4) {
	case 0: xPosE_Acerto = xPosE; break;
	case 1: xPosE_Acerto = xPosE - 1; break;
	case 2: xPosE_Acerto = xPosE; break;
	case 3: xPosE_Acerto = xPosE; break;
	}
	// Altura do ponto de colisao 
	yPos_Acerto = collisionYPos();
	// Translacao de peca de contornos para posicao correta da grelha
	trans = glm::translate(trans, glm::vec3(xPosE_Acerto, yPos_Acerto, 0.0f));
}

// ---------------------------------------------------------------------------------

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

bool PecaS::hasCollidedBottom() {
	return bCollisionBottom;
}

bool PecaS::hasCollidedLeft() {
	return bCollisionLeft;
}

bool PecaS::hasCollidedRight() {
	return bCollisionRight;
}

bool PecaS::rotationAllowed() {
	return bRotationAllowed;
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
