#include "headers/PecaZ.hpp"
#include "headers/Peca.hpp"
#include <iostream>

// Construtores
PecaZ::PecaZ() {};

PecaZ::PecaZ(int xPosInicial, int yPosInicial, int iHeight, int iWidth, int** gameGrid, int iGameLevel) {

	// Variáveis da peça
	xCR = 1.5f;
	yCR = 0.5f;
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
	bCollisionRight= false;
	bRotationAllowed = true;
	acertoPosicaoY = 0;

	gameLevel = iGameLevel;

	g_real_vertex_buffer = {};
};

// Variáveis de classe
// Desenho da peça
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

// Textura da peça
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

// Textura da posição de colisão da peça
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

// Preenchimento do vertexBuffer de acordo com rotação da peça e local de colisão, para armazenamento do vertexBuffer
// das peças já jogadas (em Projeto.cpp)
void PecaZ::preencheRealVertex(GLfloat x, GLfloat y) {
	g_real_vertex_buffer.push_back(x);
	g_real_vertex_buffer.push_back(y);
	g_real_vertex_buffer.push_back(0.0f);
}

// Modificação de um bloco/quadricula da peça, repesentado por 2 triangulos
void PecaZ::modificaQuadricula(GLfloat x, GLfloat y) {

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

	// Reposição de valores de variáveis
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

	// Atualização de y
	y = y + 1;
	// Jogo acabou
	if (y > iHeight) {
		return true;
	}

	// Atualização de x
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

	// Atualização de y
	y = y - 1;
	// Atualização de x
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

	// Acertos nas interações com limites laterais de janela de jogo
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

	// Atualização de y
	y = y + 1;
	// Jogo acabou
	if (y > iHeight) {
		return false;
	}

	// Atualização de x
	x = x - 1;

	for (int i = 0; i < iPieceWidth - 1; i++) {
		if (gameGrid[x + i][y] == 1) {
			return false;
		}
	}

	// É possivel fazer a rotação
	return true;
}

bool PecaZ::avaliaPotencialRotacaoCaso1e3(int x, int y, int iPieceHeight) {

	// Acertos nas interações com limites laterais de janela de jogo
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

	// Atualização de y
	y = y - 1;
	// Atualização de x
	x = x - 1;

	for (int i = 0; i < iPieceHeight - 1; i++) {
		if (gameGrid[x][y + i] == 1) {
			return false;
		}
	}

	// É possivel fazer a rotação
	return true;
}

bool PecaZ::avaliaColisao() {

	// Dimensões e localização da peça da próxima rotação
	int iVariavelCiclo_AvaliaRotacaoSeguinte;
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

			/* Cálculo de parâmetros de acordo com rotação seguinte, seguindo a mesma lógica de atualizaMatriz, com os ajustes necessários */
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

			// Verificar rotação por recurso a função
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

			/* Cálculo de parâmetros de acordo com rotação seguinte, seguindo a mesma lógica de atualizaMatriz, com os ajustes necessários */
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

			// Verificar rotação por recurso a função
			bRotationAllowed = avaliaPotencialRotacaoCaso0e2(
				xPosE_AvaliaRotacaoSeguinte, yPos_AvaliaRotacaoSeguinte, iVariavelCiclo_AvaliaRotacaoSeguinte
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

void PecaZ::atualizaPos() {
	switch (iNumberRotate % 4) {
		case 0:
			// Tamanho da peça
			iPieceWidth = 3;
			iPieceHeight = 2;

			// Ajuste de posições
			xPosD = xPosE + iPieceWidth;

			if (xPosE < 0) {
				// Garantir que peça se mantém dentro da janela de visualização 
				iNumberTranslation++;
				// Reajustar posição da peça resultante de ajuste
				xPosE++;
				xPosD++;
			}
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
			xPosE++; // Peça avança relativamente à posição standart da peça
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

			if (xPosE < 0) {
				// Garantir que peça se mantém dentro da janela de visualização
				iNumberTranslation++;
				// Reajustar posição da peça resultante de ajuste
				xPosE++;
				xPosD++;
			}

			// Caso em que é necessário garantir que peça não desce para fora da janela de visualização
			if (yPos <= 0) {
				// Garantir que peça se mantém dentro da janela de visualização 
				acertoPosicaoY += -yPos;
			}

			break;
	}
}

// glm::mat4& rot => alternativa a  glm::mat4 *rot aqui, com chamada de funçao &rot, ao inves de rot
void PecaZ::rotacaoPeca(glm::mat4& rot) {

	// Transladar para origem da peça, rodar 90 graus e retornar à sua posição original 
	rot = glm::translate(rot, glm::vec3(xCR, yCR, 0.0f));
	rot = glm::rotate(rot, glm::radians(iNumberRotate * -90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	rot = glm::translate(rot, glm::vec3(-xCR, -yCR, 0.0f));

}

// Modificar o valor de tempo de acordo com o nível de jogo
int PecaZ::dropAccordingToLevel(double x){
	// 0.5 fator de multiplicação, visando melhor jogabilidade
	// + 1, pois o primeiro nível é 0
	return x * (gameLevel*0.5 + 1); 
}

void PecaZ::translacaoPeca(glm::mat4& trans) {

	auto t_now = std::chrono::high_resolution_clock::now();
	double timeDouble = std::chrono::duration_cast<std::chrono::duration<double>>(t_now - t_start).count();
	int time = dropAccordingToLevel(timeDouble);

	/* Se houve colisão, não reajustar variável associada a tempo, para garantir que peça se mantém
	posição onde estava no momento de colisão. "time" influencia a descida da peça, daí ter esta avaliação */
	if (bCollisionBottom) {
		time = oldValueTime;
	}

	// Peça desce pelo ecrã, a cada segundo
	yPos = yPosInicial - time  - iNumberDown * .5 + acertoPosicaoY;

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

// ---------------------------------------------------------------------------------

int PecaZ::collisionYPos() {

	// Variáveis de acerto de acordo com rotação da peça
	int yPos_Atual = yPos;
	int iAltura, iLargura;
	int valorRetorno = 0;
	int xPosE_Acerto;

	/* Avaliar colisão de Y, mediante a rotação. Haverá acerto das posições de X e diferentes avaliações mediante os pontos 
	de colisão associadas. Haverá um valor de retorno por defeito em cada caso */
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
	// Dimensões e localização da peça da próxima rotação
	int xPosE_Acerto, yPos_Acerto;

	// Acerto de xPosE de acordo com rotação
	switch (iNumberRotate % 4) {
		case 0: xPosE_Acerto = xPosE; break;
		case 1: xPosE_Acerto = xPosE - 1; break;
		case 2: xPosE_Acerto = xPosE; break;
		case 3: xPosE_Acerto = xPosE; break;
	}
	// Altura do ponto de colisão 
	yPos_Acerto = collisionYPos();
	// Translação de peça de contornos para posição correta da grelha
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
