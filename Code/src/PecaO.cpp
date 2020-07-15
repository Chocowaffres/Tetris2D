/////////////////////////////////////////////
// Developed by Tiago Roxo and Joana Costa //
/////////////////////////////////////////////

#include "headers/PecaO.hpp"
#include <iostream>

// Construtores
PecaO::PecaO() {};

PecaO::PecaO(int xPosInicial, int yPosInicial, int iHeight, int iWidth, int** gameGrid, int iGameLevel) {

	// Variaveis da peca
	xCR = 1.0f;
	yCR = 1.0f;
	iPieceWidth = 2;
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

/// Textura da peca
std::vector<GLfloat> PecaO::g_texture_buffer_data = {
		//O
		0.0f,   0.25f,
		0.0f,   0.5f,
		0.25f,  0.25f,

		0.0f,   0.5f,
		0.25f,  0.25f,
		0.25f,  0.5f,

		//
		0.0f,   0.25f,
		0.0f,   0.5f,
		0.25f,  0.25f,

		0.0f,   0.5f,
		0.25f,  0.25f,
		0.25f,  0.5f,

		//
		0.0f,   0.25f,
		0.0f,   0.5f,
		0.25f,  0.25f,

		0.0f,   0.5f,
		0.25f,  0.25f,
		0.25f,  0.5f,

		//
		0.0f,   0.25f,
		0.0f,   0.5f,
		0.25f,  0.25f,

		0.0f,   0.5f,
		0.25f,  0.25f,
		0.25f,  0.5f,
};

std::vector<GLfloat> PecaO::g_real_vertex_buffer = {};

/// Textura da posicao de colisao da peca
std::vector<GLfloat> PecaO::g_texture_buffer_dataPos = {
		//O
		0.25f,  1.0f,
		0.5f,   1.0f,
		0.25f,  0.75f,

		0.5f,   1.0f,
		0.25f,  0.75f,
		0.5f,  0.75f,

		//
		0.25f,  1.0f,
		0.5f,   1.0f,
		0.25f,  0.75f,

		0.5f,   1.0f,
		0.25f,  0.75f,
		0.5f,  0.75f,

		//
		0.25f,  1.0f,
		0.5f,   1.0f,
		0.25f,  0.75f,

		0.5f,   1.0f,
		0.25f,  0.75f,
		0.5f,  0.75f,

		//
		0.25f,  1.0f,
		0.5f,   1.0f,
		0.25f,  0.75f,

		0.5f,   1.0f,
		0.25f,  0.75f,
		0.5f,  0.75f,
};

// Preenchimento do vertexBuffer de acordo com rotacao da peca e local de colisao, para armazenamento do vertexBuffer
// das pecas ja jogadas (em Projeto.cpp)
void PecaO::preencheRealVertex(GLfloat x, GLfloat y) {
	g_real_vertex_buffer.push_back(x);
	g_real_vertex_buffer.push_back(y);
	g_real_vertex_buffer.push_back(0.0f);
}

// Modificacao de um bloco/quadricula da peca, repesentado por 2 triangulos
void PecaO::modificaQuadricula(GLfloat x, GLfloat y) {

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
void PecaO::realVertexBuffer() {
	int xPosAntes = xPosE;
	int yPosAntes = yPos;

	// Nao necessita de considerar rotacoes neste caso, devido a natureza da peca (O, um quadrado)
	modificaQuadricula(xPosE, yPos);

	xPosE++;
	modificaQuadricula(xPosE, yPos);

	yPos++;
	modificaQuadricula(xPosE, yPos);

	xPosE--;
	modificaQuadricula(xPosE, yPos);

	// Reposicao de valores de variaveis
	xPosE = xPosAntes;
	yPos = yPosAntes;
}

bool PecaO::preencheMatriz(int x, int y) {
	// Altura
	for (int i = 0; i < iPieceHeight; i++) {
		// Jogo acabou
		if (y + i > iHeight - 2) {
			return true;
		}
		// Largura
		for (int j = 0; j < iPieceWidth; j++) {
			gameGrid[x + j][y + i] = 1;
		}
	}

	return false;
}

bool PecaO::atualizaMatriz() {
	return preencheMatriz(xPosE, yPos);
}


bool PecaO::avaliaColisao() {
	// Reset de variaveis
	bCollisionBottom = false;
	bCollisionLeft = false;
	bCollisionRight = false;
	bRotationAllowed = true;

	// Colisao com base de jogo
	if (yPos == 0) {
		bCollisionBottom = true;
	}

	/* Colisao com outras pecas
	Diferentes variaveis sao atualizadas, mediante o ponto de colisao, avaliadas em registerUserInputs, em main.cpp */
	if ((gameGrid[xPosE][yPos - 1] == 1) || 
		(gameGrid[xPosE + 1][yPos - 1] == 1)){
			bCollisionBottom = true;
	}
	if (xPosE - 1 >= 0) {
		if ((gameGrid[xPosE - 1][yPos] == 1) || 
			(gameGrid[xPosE - 1][yPos + 1] == 1)) {
				bCollisionLeft = true;
		}
	}
	if (xPosE + 2 < iWidth) {
		if ((gameGrid[xPosE + 2][yPos] == 1) || 
			(gameGrid[xPosE + 2][yPos + 1] == 1)) {
				bCollisionRight = true;
		}
	}

	// Variavel bRotationAllowed nao precisa de ser negada pois esta peca nao varia mediante rotacao

	/* Apena agora sera retornado o valor de colisao para garantir que as restantes variaveis (bCollisionLeft e bCollisionRight)
	sao atualizadas de acordo com a situacao de colisao */
	if (bCollisionBottom) {
		return true;
	}

	return false;
}

void PecaO::atualizaPos() {

	// Todas as rotacoes tem o mesmo aspecto logo nao e preciso distinguir entre os diferentes estados de rotacao possiveis
		
	// Atualizacao xPosD para garantir que peca nao sai da janela de visualizacao
	xPosD = xPosE + iPieceWidth;
}

/// glm::mat4& rot => alternativa a  glm::mat4 *rot aqui, com chamada de funcao &rot, ao inves de rot
void PecaO::rotacaoPeca(glm::mat4& rot) {

	// Transladar para origem da peca, rodar 90 graus e retornar a sua posicao original 
	rot = glm::translate(rot, glm::vec3(xCR, yCR, 0.0f));
	rot = glm::rotate(rot, glm::radians(iNumberRotate * -90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	rot = glm::translate(rot, glm::vec3(-xCR, -yCR, 0.0f));

}

// Modificar o valor de tempo de acordo com o nivel de jogo
int PecaO::dropAccordingToLevel(double x) {
	// 0.5 fator de multiplicacao, visando melhor jogabilidade
	// + 1, pois o primeiro nivel e 0
	return x * (gameLevel*0.5 + 1);
}

void PecaO::translacaoPeca(glm::mat4& trans, bool bPause) {

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

int PecaO::collisionYPos() {

	// Variaveis de acerto de acordo com rotacao da peca
	int yPos_Atual = yPos;
	int iAltura = 2;
	int valorRetorno = 0;

	/* Avaliar colisao de Y, independente da rotacao (particularidade desta peca) */
	for (int i = 0; i < yPos_Atual + iAltura - 1; i++) {
		if ((gameGrid[xPosE][i - 1] == 1) ||
			(gameGrid[xPosE + 1][i - 1] == 1)) {
				valorRetorno = i;
		}
	}
	return valorRetorno;
}

void PecaO::translacaoPecaContorno(glm::mat4& trans) {
	// Dimensoes e localizacao da peca da proxima rotacao
	int xPosE_Acerto = xPosE, yPos_Acerto;

	// Altura do ponto de colisao 
	yPos_Acerto = collisionYPos();

	// Translacao de peca de contornos para posicao correta da grelha
	trans = glm::translate(trans, glm::vec3(xPosE_Acerto, yPos_Acerto, 0.0f));
}

// ---------------------------------------------------------------------------------

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

bool PecaO::hasCollidedBottom() {
	return bCollisionBottom;
}

bool PecaO::hasCollidedLeft() {
	return bCollisionLeft;
}

bool PecaO::hasCollidedRight() {
	return bCollisionRight;
}

bool PecaO::rotationAllowed() {
	return bRotationAllowed;
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
