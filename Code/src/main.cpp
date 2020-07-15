/////////////////////////////////////////////
// Developed by Tiago Roxo and Joana Costa //
/////////////////////////////////////////////

#include "headers/PecaL.hpp"
#include "headers/PecaJ.hpp"
#include "headers/PecaS.hpp"
#include "headers/PecaI.hpp"
#include "headers/PecaZ.hpp"
#include "headers/PecaO.hpp"
#include "headers/PecaT.hpp"
#include "headers/Peca.hpp"
#include "headers/GeradorPecas.hpp" 
#include "headers/Font.hpp"

// Include standard headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <ctime>
using namespace std;

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// GLM header file
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
using namespace glm;

// shaders header file
#include <common/shader.cpp>

// Imagens
#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

// Sons
#pragma comment(lib, "irrKlang.lib")
#include <irrKlang.h>
using namespace irrklang;

/// Vertex array object (VAO)
GLuint VertexArrayID;

/// Texture array object (TAO)
GLuint TextureID[4];

/// Vertex buffer object (VBO)
GLuint vertexbuffer;
GLuint vertexbufferTot;
GLuint vertexbufferNextPiece;
GLuint vertexbufferSavedPiece;
GLuint vertexbufferBackground;
GLuint vertexbufferPosPiece;
GLuint vertexbufferGrid;
GLuint vertexbufferScore;
GLuint vertexbufferScoreNum;
GLuint vertexbufferLevel;
GLuint vertexbufferLevelNum;
GLuint vertexbufferLines;
GLuint vertexbufferLinesNum;
GLuint vertexbufferNext;
GLuint vertexbufferHold;
GLuint vertexbufferPause;
GLuint vertexbufferGameOver;
GLuint vertexbufferControls;

/// Texture buffer object (TBO)
GLuint texturebuffer;
GLuint texturebufferTot;
GLuint texturebufferNextPiece;
GLuint texturebufferSavedPiece;
GLuint texturebufferBackground;
GLuint texturebufferPosPiece;
GLuint texturebufferGrid;
GLuint texturebufferScore;
GLuint texturebufferScoreNum;
GLuint texturebufferLevel;
GLuint texturebufferLevelNum;
GLuint texturebufferLines;
GLuint texturebufferLinesNum;
GLuint texturebufferNext;
GLuint texturebufferHold;
GLuint texturebufferPause;
GLuint texturebufferGameOver;
GLuint texturebufferControls;

/// GLSL program from the shaders
GLuint programID;

int const iWidth = 10;
int const iHeight = 20;
GLfloat WIDTH_PosXInicial = 10.f;
GLfloat HEIGHT_PosYInicial = 18.f;
GLfloat WIDTH = 14.f; 
GLfloat HEIGHT = 20.f;
GLint WindowHeight = 600;
GLint WindowWidth = WIDTH / HEIGHT * WindowHeight;

// Caminhos para os diversos recursos
char vertexShader[] = "shaders/vertexShader.vertexshader";
char fragmentShader[] = "shaders/fragmentShader.fragmentshader";
char WindowTitle[] = "Tetris";
const char* caTiles = "resources/images/tiles_large.tga";
const char* caBackground = "resources/images/wallpaper.tga";
const char* caGamegrid = "resources/images/gamegrid.tga";
const char* caPause = "resources/images/pause.tga";
const char* caGameOver = "resources/images/gameOver.tga";
const char* caControls = "resources/images/controls.tga";

// Timer
std::chrono::time_point<std::chrono::steady_clock> t_start;
std::chrono::time_point<std::chrono::steady_clock> t_collision;
int timerCollision;

// Collision
bool bCollisionPiece = false;

int** gameGrid;
// Posição inicial da peça, no topo e ao centro
int xPosInicial = (int)WIDTH_PosXInicial / 2 - 1, yPosInicial = HEIGHT_PosYInicial;

// Variáveis de jogo
Peca* pPeca;
Peca* pNextpPeca;
Peca* pSavedpPeca;

GeradorPecas geraPecas;
int iGamePiece;
int iGameNextPiece;
int iSavedPiece = 0;
bool bAlteraPecaEmJogo = false;
bool bDropPeca = false;
bool bPause = false;
bool bGameOver = false;
bool bControls = true; // Representado logo ao inicio

// Variaveis auxiliares para input do rato
bool bMouse = false;
bool bRotateMouse = false;
double dXPosAnt = 135;
bool bFirstTime = false;

// Variaveis de eliminar linhas
int newSizeVertex;
int newSizeTexture;

// Variáveis de textura
int iTexWidth;
int iTexHeight;
int iTexNumChannels;
unsigned char* ucaTexData;

// Pontuação
int iPontuacao = 0;
int iNivelJogo = 0;
int iTotalLinhasEliminadasEmNível = 0;

unsigned int globalBack;

ISoundEngine *SoundEngine = createIrrKlangDevice();
ISound *sound;
/// Variável de som para impedir que a limpeza de linhas toque quando se passa de nível
bool bPassagemNivel = false;


/// Vértices da peça atual
std::vector<GLfloat> g_vertex_buffer_data = {};

/// Textura da peça atual
std::vector<GLfloat> g_texture_buffer_data = {};

/// Vértices de todas as peças
std::vector<GLfloat> g_vertex_buffer_dataTot = {};

/// Texturas de todas as peças
std::vector<GLfloat> g_texture_buffer_dataTot = {};

/// Vértices de background
std::vector<GLfloat> g_vertex_buffer_dataBack = {
	0.0f,  0.0f,   0.1f,
	14.0f, 0.0f,   0.1f,
	0.0f,  20.0f,  0.1f,

	14.0f, 0.0f,   0.1f,
	0.0f,  20.0f,  0.1f,
	14.0f, 20.0f,  0.1f,
};

/// Vértices de Pause
std::vector<GLfloat> g_vertex_buffer_dataPause = {
	0.0f,  0.0f,   0.1f,
	14.0f, 0.0f,   0.1f,
	0.0f,  20.0f,  0.1f,

	14.0f, 0.0f,   0.1f,
	0.0f,  20.0f,  0.1f,
	14.0f, 20.0f,  0.1f,
};

/// Vértices de Game Over
std::vector<GLfloat> g_vertex_buffer_dataGameOver = {
	0.0f,  0.0f,   0.1f,
	14.0f, 0.0f,   0.1f,
	0.0f,  20.0f,  0.1f,

	14.0f, 0.0f,   0.1f,
	0.0f,  20.0f,  0.1f,
	14.0f, 20.0f,  0.1f,
};

/// Vértices de Controls
std::vector<GLfloat> g_vertex_buffer_dataControls = {
	0.0f,  0.0f,   0.1f,
	14.0f, 0.0f,   0.1f,
	0.0f,  20.0f,  0.1f,

	14.0f, 0.0f,   0.1f,
	0.0f,  20.0f,  0.1f,
	14.0f, 20.0f,  0.1f,
};


// -----------------------------------------------


/// Textura de Controls
std::vector<GLfloat> g_texture_buffer_dataControls = {
	0.0f,  1.0f,
	1.0f,  1.0f,
	0.0f,  0.0f,

	1.0f,  1.0f,
	0.0f,  0.0f,
	1.0f,  0.0f,
};

/// Textura de Game Over
std::vector<GLfloat> g_texture_buffer_dataGameOver = {
	0.0f,  1.0f,
	1.0f,  1.0f,
	0.0f,  0.0f,

	1.0f,  1.0f,
	0.0f,  0.0f,
	1.0f,  0.0f,
};

/// Textura de background
std::vector<GLfloat> g_texture_buffer_dataBack = {
	0.0f,  1.0f,
	1.0f,  1.0f,
	0.0f,  0.0f,

	1.0f,  1.0f,
	0.0f,  0.0f,
	1.0f,  0.0f,
};

/// Textura de Pause
std::vector<GLfloat> g_texture_buffer_dataPause = {
	0.0f,  1.0f,
	1.0f,  1.0f,
	0.0f,  0.0f,

	1.0f,  1.0f,
	0.0f,  0.0f,
	1.0f,  0.0f,
};

/// Vértices da grelha de jogo
std::vector<GLfloat> g_vertex_buffer_dataGrid = {
	0.0f,  0.0f,   0.1f,
	10.0f, 0.0f,   0.1f,
	0.0f,  20.0f,  0.1f,

	10.0f, 0.0f,   0.1f,
	0.0f,  20.0f,  0.1f,
	10.0f, 20.0f,  0.1f,
};

/// Textura da grelha de jogo
std::vector<GLfloat> g_texture_buffer_dataGrid = {
	0.0f,  1.0f,
	1.0f,  1.0f,
	0.0f,  0.0f,

	1.0f,  1.0f,
	0.0f,  0.0f,
	1.0f,  0.0f,
};

/// Vértices de todos os algarismos da pontuação
std::vector<GLfloat> g_vertex_buffer_dataScoreNum = {};

/// Texturas de todos os algarismos da pontuação
std::vector<GLfloat> g_texture_buffer_dataScoreNum = {};

/// Vértices de todos os algarismos do nível
std::vector<GLfloat> g_vertex_buffer_dataLevelNum = {};

/// Texturas de todos os algarismos do nível
std::vector<GLfloat> g_texture_buffer_dataLevelNum = {};

/// Vértices de todos os algarismos do nível
std::vector<GLfloat> g_vertex_buffer_dataLinesNum = {};

/// Texturas de todos os algarismos do nível
std::vector<GLfloat> g_texture_buffer_dataLinesNum = {};

/// Identificador de matriz MVP nos shaders
GLuint MVP;

// Matrizes associadas a MVP
glm::mat4 Projection = glm::mat4(1.0f);
glm::mat4 View = glm::mat4(1.0f);
// Model definido nas funções apropriadas

//--------------------------------------------------------------------------------

/// Inicializar matriz a zeros
void incializaMatrizZero() {
	gameGrid = (int**)calloc(iWidth, sizeof(int*));
	for (int i = 0; i < iWidth; i++) {
		gameGrid[i] = (int*)calloc(iHeight, sizeof(int));
	}
}

/// Generates random number from 0 to 6 (7 pieces)
int randNum() {
	// Conferir aleatoriedade
	srand(time(NULL));
	int iRandValue = (std::rand() % 7) + 1;
	return iRandValue;
}

//--------------------------------------------------------------------------------

int pontosPorLinhasEliminadas (int iNumbLinhasEliminadas) {
	int iTotal = 0;
	switch (iNumbLinhasEliminadas) {
		case 1: iTotal = 40 * (iNivelJogo + 1); break;
		case 2: iTotal = 100 * (iNivelJogo + 1); break;
		case 3: iTotal = 300 * (iNivelJogo + 1); break;
		case 4: iTotal = 1200 * (iNivelJogo + 1); break;
	}
	return iTotal;
}

// Sons sem repetição
void somPassagemNivel() {
	ISound* sound = SoundEngine->play2D("resources/audio/levelUp.wav", GL_FALSE, GL_FALSE, GL_TRUE);
	if (sound) {
		sound->setVolume(0.25);
	}
	bPassagemNivel = true;
}

void atualizaNivelJogo() {
	if (iNivelJogo < 10) {
		if (iTotalLinhasEliminadasEmNível >= (10 * (iNivelJogo + 1))) {
			iNivelJogo++;
			somPassagemNivel();
			iTotalLinhasEliminadasEmNível = 0;
		}
	}
	else if (iNivelJogo < 16) {
		if (iTotalLinhasEliminadasEmNível >= 100) {
			iNivelJogo++;
			somPassagemNivel();
			iTotalLinhasEliminadasEmNível = 0;
		}
	}
	else if (iNivelJogo < 26) {
		if (iTotalLinhasEliminadasEmNível >= (10 * (iNivelJogo - 5))) {
			iNivelJogo++;
			somPassagemNivel();
			iTotalLinhasEliminadasEmNível = 0;
		}
	}
	else {
		if (iTotalLinhasEliminadasEmNível >= 200) {
			if (iNivelJogo < 29) {
				iNivelJogo++;
				somPassagemNivel();
				iTotalLinhasEliminadasEmNível = 0;
			}
		}
	}
}
	
//--------------------------------------------------------------------------------

Peca* returnPeca(GeradorPecas& geraPecas, int switchValue) {
	Peca* pecaGenerica;
	switch (switchValue) {
		case 1:
			geraPecas.criaPecaZ(gameGrid, iNivelJogo);
			pecaGenerica = &(geraPecas.getPecaZ());
			return pecaGenerica;
		case 2:
			geraPecas.criaPecaT(gameGrid, iNivelJogo);
			pecaGenerica = &(geraPecas.getPecaT());
			return pecaGenerica;
		case 3:
			geraPecas.criaPecaJ(gameGrid, iNivelJogo);
			pecaGenerica = &(geraPecas.getPecaJ());
			return pecaGenerica;
		case 4:
			geraPecas.criaPecaS(gameGrid, iNivelJogo);
			pecaGenerica = &(geraPecas.getPecaS());
			return pecaGenerica;
		case 5:
			geraPecas.criaPecaO(gameGrid, iNivelJogo);
			pecaGenerica = &(geraPecas.getPecaO());
			return pecaGenerica;
		case 6:
			geraPecas.criaPecaL(gameGrid, iNivelJogo);
			pecaGenerica = &(geraPecas.getPecaL());
			return pecaGenerica;
		case 7:
			geraPecas.criaPecaI(gameGrid, iNivelJogo);
			pecaGenerica = &(geraPecas.getPecaI());
			return pecaGenerica;
	}
}

std::vector<GLfloat> vertexBufferPiece(Peca& pPeca, int switchValue) {
	switch (switchValue) {
		case 1: return dynamic_cast<PecaZ&>(pPeca).g_vertex_buffer_data;
		case 2: return dynamic_cast<PecaT&>(pPeca).g_vertex_buffer_data;
		case 3: return dynamic_cast<PecaJ&>(pPeca).g_vertex_buffer_data;
		case 4: return dynamic_cast<PecaS&>(pPeca).g_vertex_buffer_data;
		case 5: return dynamic_cast<PecaO&>(pPeca).g_vertex_buffer_data;
		case 6: return dynamic_cast<PecaL&>(pPeca).g_vertex_buffer_data;
		case 7: return dynamic_cast<PecaI&>(pPeca).g_vertex_buffer_data;
	}
}

std::vector<GLfloat> textureBufferPiece(Peca& pPeca, int switchValue) {
	switch (switchValue) {
		case 1: return dynamic_cast<PecaZ&>(pPeca).g_texture_buffer_data;
		case 2: return dynamic_cast<PecaT&>(pPeca).g_texture_buffer_data;
		case 3: return dynamic_cast<PecaJ&>(pPeca).g_texture_buffer_data;
		case 4: return dynamic_cast<PecaS&>(pPeca).g_texture_buffer_data;
		case 5: return dynamic_cast<PecaO&>(pPeca).g_texture_buffer_data;
		case 6: return dynamic_cast<PecaL&>(pPeca).g_texture_buffer_data;
		case 7: return dynamic_cast<PecaI&>(pPeca).g_texture_buffer_data;
	}
}

std::vector<GLfloat>* realVertexBufferPiece(Peca& pPeca) {
	switch (iGamePiece) {
	case 1: return &dynamic_cast<PecaZ&>(pPeca).g_real_vertex_buffer;
	case 2: return &dynamic_cast<PecaT&>(pPeca).g_real_vertex_buffer;
	case 3: return &dynamic_cast<PecaJ&>(pPeca).g_real_vertex_buffer;
	case 4: return &dynamic_cast<PecaS&>(pPeca).g_real_vertex_buffer;
	case 5: return &dynamic_cast<PecaO&>(pPeca).g_real_vertex_buffer;
	case 6: return &dynamic_cast<PecaL&>(pPeca).g_real_vertex_buffer;
	case 7: return &dynamic_cast<PecaI&>(pPeca).g_real_vertex_buffer;
	}
}

std::vector<GLfloat> textureBufferPosPiece(Peca& pPeca, int switchValue) {
	switch (switchValue) {
	case 1: return dynamic_cast<PecaZ&>(pPeca).g_texture_buffer_dataPos;
	case 2: return dynamic_cast<PecaT&>(pPeca).g_texture_buffer_dataPos;
	case 3: return dynamic_cast<PecaJ&>(pPeca).g_texture_buffer_dataPos;
	case 4: return dynamic_cast<PecaS&>(pPeca).g_texture_buffer_dataPos;
	case 5: return dynamic_cast<PecaO&>(pPeca).g_texture_buffer_dataPos;
	case 6: return dynamic_cast<PecaL&>(pPeca).g_texture_buffer_dataPos;
	case 7: return dynamic_cast<PecaI&>(pPeca).g_texture_buffer_dataPos;
	}
}

std::vector<GLfloat> vertexBufferNumber(int switchValue) {
	switch (switchValue) {
		case 1: return One::g_vertex_buffer_data;
		case 2: return Two::g_vertex_buffer_data;
		case 3: return Three::g_vertex_buffer_data;
		case 4: return Four::g_vertex_buffer_data;
		case 5: return Five::g_vertex_buffer_data;
		case 6: return Six::g_vertex_buffer_data;
		case 7: return Seven::g_vertex_buffer_data;
		case 8: return Eight::g_vertex_buffer_data;
		case 9: return Nine::g_vertex_buffer_data;
		case 0: return Zero::g_vertex_buffer_data;
		case 10: return Score::g_vertex_buffer_data;
		case 11: return Level::g_vertex_buffer_data;
		case 12: return Lines::g_vertex_buffer_data;
		case 13: return Next::g_vertex_buffer_data;
		case 14: return Hold::g_vertex_buffer_data;
	}
}

std::vector<GLfloat> textureBufferNumber(int switchValue) {
	switch (switchValue) {
		case 1: return One::g_texture_buffer_data;
		case 2: return Two::g_texture_buffer_data;
		case 3: return Three::g_texture_buffer_data;
		case 4: return Four::g_texture_buffer_data;
		case 5: return Five::g_texture_buffer_data;
		case 6: return Six::g_texture_buffer_data;
		case 7: return Seven::g_texture_buffer_data;
		case 8: return Eight::g_texture_buffer_data;
		case 9: return Nine::g_texture_buffer_data;
		case 0: return Zero::g_texture_buffer_data;
		case 10: return Score::g_texture_buffer_data;
		case 11: return Level::g_texture_buffer_data;
		case 12: return Lines::g_texture_buffer_data;
		case 13: return Next::g_texture_buffer_data;
		case 14: return Hold::g_texture_buffer_data;
	}
}

//--------------------------------------------------------------------------------

void transferDataToGPUMemory(Peca& pPeca, Peca& pNextPeca, Peca& pSavedPeca)
{

	// Recolha de informação de vertices e textura da peça atual
	g_vertex_buffer_data = vertexBufferPiece(pPeca, iGamePiece);
	g_texture_buffer_data = textureBufferPiece(pPeca, iGamePiece);

	// Recolha de informação de vertices e textura da próxima peça peça
	std::vector<GLfloat> g_vertex_buffer_data_next_piece = vertexBufferPiece(pNextPeca, iGameNextPiece);
	std::vector<GLfloat> g_texture_buffer_data_next_piece = textureBufferPiece(pNextPeca, iGameNextPiece);

	// Recolha de informação de vertices e textura da peça guardada por user (clique em "C")
	std::vector<GLfloat> g_vertex_buffer_data_saved_piece = vertexBufferPiece(pSavedPeca, iSavedPiece);
	std::vector<GLfloat> g_texture_buffer_data_saved_piece = textureBufferPiece(pSavedPeca, iSavedPiece);

	// Distinguir enter a primeira vez que se guarda peça (não havida nenhuma antes) e as restantes vezes
	if (iSavedPiece == 0) {
		std::vector<GLfloat> g_vertex_buffer_data_saved_piece = {};
		std::vector<GLfloat> g_texture_buffer_data_saved_piece = {};
	}

	// Recolha de informação de vertices e textura da poisção da peça aquando da colisão
	std::vector<GLfloat> g_vertex_buffer_data_pos_piece = vertexBufferPiece(pPeca, iGamePiece);
	std::vector<GLfloat> g_texture_buffer_data_pos_piece = textureBufferPosPiece(pPeca, iGamePiece);

	// Move vertex data to video memory; specifically to VBO called vertexbuffer,
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size() * sizeof(float), g_vertex_buffer_data.data(), GL_STATIC_DRAW);

	// Move texture data to video memory; specifically to CBO called texturebuffer
	glGenBuffers(1, &texturebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_data.size() * sizeof(float), g_texture_buffer_data.data(), GL_STATIC_DRAW);


	// Carregar informação de todas as peças anteriormente jogadas
	glGenBuffers(1, &vertexbufferTot);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferTot);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_dataTot.size() * sizeof(float), g_vertex_buffer_dataTot.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &texturebufferTot);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferTot);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_dataTot.size() * sizeof(float), g_texture_buffer_dataTot.data(), GL_STATIC_DRAW);


	// Carregar informação da próxima peça
	glGenBuffers(1, &vertexbufferNextPiece);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferNextPiece);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data_next_piece.size() * sizeof(float), g_vertex_buffer_data_next_piece.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &texturebufferNextPiece);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferNextPiece);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_data_next_piece.size() * sizeof(float), g_texture_buffer_data_next_piece.data(), GL_STATIC_DRAW);


	// Carregar informação peça guardada por utilizador
	glGenBuffers(1, &vertexbufferSavedPiece);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferSavedPiece);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data_saved_piece.size() * sizeof(float), g_vertex_buffer_data_saved_piece.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &texturebufferSavedPiece);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferSavedPiece);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_data_saved_piece.size() * sizeof(float), g_texture_buffer_data_saved_piece.data(), GL_STATIC_DRAW);


	// Carregar informação posição da peça na colisão
	glGenBuffers(1, &vertexbufferPosPiece);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferPosPiece);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data_pos_piece.size() * sizeof(float), g_vertex_buffer_data_pos_piece.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &texturebufferPosPiece);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferPosPiece);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_data_pos_piece.size() * sizeof(float), g_texture_buffer_data_pos_piece.data(), GL_STATIC_DRAW);
	
}

//--------------------------------------------------------------------------------

void cleanupDataFromGPU()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &texturebuffer);
	glDeleteBuffers(1, &vertexbufferTot);
	glDeleteBuffers(1, &texturebufferTot);
	glDeleteBuffers(1, &vertexbufferNextPiece);
	glDeleteBuffers(1, &texturebufferNextPiece);
	glDeleteBuffers(1, &vertexbufferSavedPiece);
	glDeleteBuffers(1, &texturebufferSavedPiece);
	glDeleteBuffers(1, &vertexbufferBackground);
	glDeleteBuffers(1, &texturebufferBackground);
	glDeleteBuffers(1, &vertexbufferPosPiece);
	glDeleteBuffers(1, &texturebufferPosPiece);
	glDeleteBuffers(1, &vertexbufferGrid);
	glDeleteBuffers(1, &texturebufferGrid);
	glDeleteBuffers(1, &vertexbufferScore);
	glDeleteBuffers(1, &texturebufferScore);
	glDeleteBuffers(1, &vertexbufferScoreNum);
	glDeleteBuffers(1, &texturebufferScoreNum);
	glDeleteBuffers(1, &vertexbufferLevel);
	glDeleteBuffers(1, &texturebufferLevel);
	glDeleteBuffers(1, &vertexbufferLevelNum);
	glDeleteBuffers(1, &texturebufferLevelNum);
	glDeleteBuffers(1, &vertexbufferLines);
	glDeleteBuffers(1, &texturebufferLines);
	glDeleteBuffers(1, &vertexbufferLinesNum);
	glDeleteBuffers(1, &texturebufferLinesNum);
	glDeleteBuffers(1, &vertexbufferNext);
	glDeleteBuffers(1, &texturebufferNext);
	glDeleteBuffers(1, &vertexbufferHold);
	glDeleteBuffers(1, &texturebufferHold);
	glDeleteBuffers(1, &vertexbufferPause);
	glDeleteBuffers(1, &texturebufferPause);
	glDeleteBuffers(1, &vertexbufferControls);
	glDeleteBuffers(1, &texturebufferControls);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteTextures(3, TextureID);
	glDeleteProgram(programID);
}

//--------------------------------------------------------------------------------

void drawScore(int iScore) {
	int iTotAlgarismos = 6;
	std::vector<GLfloat> vertex_aux;
	std::vector<GLfloat> texture_aux;

	g_vertex_buffer_dataScoreNum = {};
	g_texture_buffer_dataScoreNum = {};

	for (int i = 0; i < iTotAlgarismos; i++)
	{
		int iNum = iScore % 10;

		vertex_aux = vertexBufferNumber(iNum);
		for (int j = 0; j < vertex_aux.size(); j += 3)
			vertex_aux.at(j) = vertex_aux.at(j) + (iTotAlgarismos - i) * 1.1;
		texture_aux = textureBufferNumber(iNum);
		g_vertex_buffer_dataScoreNum.insert(g_vertex_buffer_dataScoreNum.end(), vertex_aux.begin(), vertex_aux.end());
		g_texture_buffer_dataScoreNum.insert(g_texture_buffer_dataScoreNum.end(), texture_aux.begin(), texture_aux.end());

		iScore /= 10;
	}

	// Carregar informação dos vértices associados a pontuação para memória vídeo
	glGenBuffers(1, &vertexbufferScoreNum);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferScoreNum);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_dataScoreNum.size() * sizeof(float), g_vertex_buffer_dataScoreNum.data(), GL_STATIC_DRAW);

	// Carregar informação de textura associad a pontuação para memória vídeo
	glGenBuffers(1, &texturebufferScoreNum);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferScoreNum);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_dataScoreNum.size() * sizeof(float), g_texture_buffer_dataScoreNum.data(), GL_STATIC_DRAW);
	
}

void drawLevel(int iLevel) {

	int iTotAlgarismos = 2;
	std::vector<GLfloat> vertex_aux;
	std::vector<GLfloat> texture_aux;

	g_vertex_buffer_dataLevelNum = {};
	g_texture_buffer_dataLevelNum = {};

	for (int i = 0; i < iTotAlgarismos; i++)
	{
		int iNum = iLevel % 10;

		vertex_aux = vertexBufferNumber(iNum);
		for (int j = 0; j < vertex_aux.size(); j += 3)
			vertex_aux.at(j) = vertex_aux.at(j) + (iTotAlgarismos - i) * 1.1;
		texture_aux = textureBufferNumber(iNum);
		g_vertex_buffer_dataLevelNum.insert(g_vertex_buffer_dataLevelNum.end(), vertex_aux.begin(), vertex_aux.end());
		g_texture_buffer_dataLevelNum.insert(g_texture_buffer_dataLevelNum.end(), texture_aux.begin(), texture_aux.end());

		iLevel /= 10;
	}

	// Move vertex data to video memory; specifically to VBO called vertexbuffer,
	glGenBuffers(1, &vertexbufferLevelNum);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferLevelNum);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_dataLevelNum.size() * sizeof(float), g_vertex_buffer_dataLevelNum.data(), GL_STATIC_DRAW);

	// Move texture data to video memory; specifically to CBO called texturebuffer
	glGenBuffers(1, &texturebufferLevelNum);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferLevelNum);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_dataLevelNum.size() * sizeof(float), g_texture_buffer_dataLevelNum.data(), GL_STATIC_DRAW);
}

void drawLines(int iLines) {

	int iTotAlgarismos = 3;
	std::vector<GLfloat> vertex_aux;
	std::vector<GLfloat> texture_aux;

	g_vertex_buffer_dataLinesNum = {};
	g_texture_buffer_dataLinesNum = {};

	for (int i = 0; i < iTotAlgarismos; i++)
	{
		int iNum = iLines % 10;

		vertex_aux = vertexBufferNumber(iNum);
		for (int j = 0; j < vertex_aux.size(); j += 3)
			vertex_aux.at(j) = vertex_aux.at(j) + (iTotAlgarismos - i) * 1.1;
		texture_aux = textureBufferNumber(iNum);
		g_vertex_buffer_dataLinesNum.insert(g_vertex_buffer_dataLinesNum.end(), vertex_aux.begin(), vertex_aux.end());
		g_texture_buffer_dataLinesNum.insert(g_texture_buffer_dataLinesNum.end(), texture_aux.begin(), texture_aux.end());

		iLines /= 10;
	}

	// Move vertex data to video memory; specifically to VBO called vertexbuffer,
	glGenBuffers(1, &vertexbufferLinesNum);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferLinesNum);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_dataLinesNum.size() * sizeof(float), g_vertex_buffer_dataLinesNum.data(), GL_STATIC_DRAW);

	// Move texture data to video memory; specifically to CBO called texturebuffer
	glGenBuffers(1, &texturebufferLinesNum);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferLinesNum);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_dataLinesNum.size() * sizeof(float), g_texture_buffer_dataLinesNum.data(), GL_STATIC_DRAW);
}

//--------------------------------------------------------------------------------

void desenhaAmbiente();

/// Caso haja uma linha completa, esta será eliminada na matriz de inteiros. Esta função garante que o observado no campo de jogo
/// é concordante com o ocorrido na matriz de inteiros
void eliminaLinha(int iLinha)
{
	int i;
	newSizeVertex = g_vertex_buffer_dataTot.size();
	newSizeTexture = g_texture_buffer_dataTot.size();
	for (i = 0; i < newSizeVertex; i += 18) {
		// Limpar bloco a bloco
		if (g_vertex_buffer_dataTot.at(i + 1) == iLinha) {

			for (int k = i; k < g_vertex_buffer_dataTot.size() - 18; k++) {
				g_vertex_buffer_dataTot.at(k) = g_vertex_buffer_dataTot.at(k + 18);
			}

			// Proporção: por cada ponto no vertexbuffer há 3 valores, no entanto no texturebuffer só há 2 valores
			for (int k = (i / 3 * 2); k < g_texture_buffer_dataTot.size() - 12; k++)
			{
				g_texture_buffer_dataTot.at(k) = g_texture_buffer_dataTot.at(k + 12);
			}

			// Atualização de variáveis
			newSizeVertex -= 18;
			newSizeTexture -= 12;
			i -= 18;

			// Atualiza tamanho de texture e vertice buffer e desenha peças envolvidas
			// Confere efeito de desaparecimento de blocos aquando da eliminação de linhas
			g_vertex_buffer_dataTot.resize(newSizeVertex);
			g_texture_buffer_dataTot.resize(newSizeTexture);
			desenhaAmbiente();
		}
		else {
			if (g_vertex_buffer_dataTot.at(i + 1) > iLinha) {
				for (int j = 1; j < 18; j += 3) {
					// Atualizar altura (y) de todos os vértices de blocos superiores à linha a eliminar
					g_vertex_buffer_dataTot.at(i + j) = g_vertex_buffer_dataTot.at(i + j) - 1;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------

/// Atualiza matriz de inteiros, visando concordância com o observado no campo de jogo. Nesta função todas as linhas acima de iLinha,
/// a linha a eliminar, serão substituidas pelas seguintes destas
void atualizaCampoJogo(int iLinha)
{
	for (int i = iLinha; i < iHeight - 1; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			gameGrid[j][i] = gameGrid[j][i + 1];
		}
	}
}
//--------------------------------------------------------------------------------

/// Verifica, na matriz de inteiros, linhas que apenas contenham 1's, ou seja, linhas que terão de ser eliminadas
std::vector <int> avaliaEliminacaoLinhas(int** gameGrid) {
	int j;
	std::vector <int> vetorLinhasAEliminar;
	for (int i = 0; i < iHeight; i++) {
		for (j = 0; j < iWidth; j++) {
			if (gameGrid[j][i] == 1) {
				continue;
			}
			break;
		}
		if (j == iWidth) {
			vetorLinhasAEliminar.push_back(i);
		}
	}
	return vetorLinhasAEliminar;
}

//--------------------------------------------------------------------------------


bool evaluatePieceCollision(Peca& pPeca) {

	// Sempre que haja colisão promover alterações a grelha de jogo, buffers (vertex e texture) e matriz de colisão
	bool bCondicaoIf = pPeca.avaliaColisao();
	/* Caso a peça não tenha sido "Dropped" (pelo utilizador, pressionando "space"), condição de If terá de contabilizar
	também com o tempo de colisão da peça, visando melhor jogabilidade */
	if (!bDropPeca) {
		bCondicaoIf = (bCondicaoIf && timerCollision > 1);
	}

	// Avaliar colisão e verificar se a peça está colidida à mais de 1 segundo
	if (bCondicaoIf) {

		// Avalia se jogo terminou
		if (pPeca.atualizaMatriz()) {
			bGameOver = true;
			// Som de fim de jogo
			ISound* soundGO = SoundEngine->play2D("resources/audio/gameOver.wav", GL_FALSE, GL_FALSE, GL_TRUE);
			if (soundGO) {
				soundGO->setVolume(0.25);
			}

			sound->stop();
		}

		// Atualiza vertexBuffer da peça, mediante o local onde esta colidiu
		pPeca.realVertexBuffer();
		// Receber grelha de jogo (matriz de inteiros) atualizada
		gameGrid = pPeca.getGameGrid();

		// Recolher vertexBuffer de peça colidida para armazenar no vertexBuffer de todas as outras peças anteriormente jogadas
		// O vertexBuffer recebido tem em conta o posicionamento da peça aquando da colisão
		std::vector<GLfloat>* vertexBuffer = realVertexBufferPiece(pPeca);

		// Anexar informação de nova peça colidida com peças anteriormente jogadas
		g_vertex_buffer_dataTot.insert(g_vertex_buffer_dataTot.end(), (*vertexBuffer).begin(), (*vertexBuffer).end());
		g_texture_buffer_dataTot.insert(g_texture_buffer_dataTot.end(), g_texture_buffer_data.begin(), g_texture_buffer_data.end());

		// Verificar se nova colisão de peça promove eliminação de linhas
		std::vector<int> vLinhasAEliminar = avaliaEliminacaoLinhas(gameGrid);
		int iNúmeroLinhasAEliminar = vLinhasAEliminar.size();

		if (iNúmeroLinhasAEliminar != 0) {

			// Atualizar variável de linhas eliminadas por nível
			iTotalLinhasEliminadasEmNível += iNúmeroLinhasAEliminar;

			// Atualizar pontuação
			iPontuacao += pontosPorLinhasEliminadas(iNúmeroLinhasAEliminar);

			// Atualizar nível de jogo
			atualizaNivelJogo();

			// Não reproduzir som de limpeza de linhas caso se tenha passado de nível
			if (!bPassagemNivel) {
				// Som sem repetição
				ISound* sound = SoundEngine->play2D("resources/audio/clearLine.wav", GL_FALSE, GL_FALSE, GL_TRUE);
				if (sound) {
					sound->setVolume(0.25);
				}
			}

			// Reset de variável
			bPassagemNivel = false;


			// Começa a eliminar as linhas do topo e ir descendo, daí a inversão do vetor
			for (int i = vLinhasAEliminar.size() - 1; i >= 0; i--) {
				eliminaLinha(vLinhasAEliminar[i]);
				atualizaCampoJogo(vLinhasAEliminar[i]);

			}
		}

		return true;
	}

	// Peça não colidiu com nenhuma outra ou base de campo de jogo
	return false;
}

//--------------------------------------------------------------------------------

bool drawCurrentObject(Peca& pPeca)
{
	// Use our shader
	glUseProgram(programID);

	// Definição de matrizes
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);

	// Aplicação de rotação e translacao onde a peça ira cair
	pPeca.rotacaoPeca(rot);
	pPeca.translacaoPecaContorno(trans);

	glm::mat4 MVP_PecaColisao = Projection * View * trans * rot;

	// retrieve the matrix uniform locations
	glUniformMatrix4fv(MVP, 1, GL_FALSE, &MVP_PecaColisao[0][0]);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferPosPiece);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : textures
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferPosPiece);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Desenha objeto
	pPeca.drawObject();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	trans = glm::mat4(1.0f);

	// Aplicação de translação da peça (aproveitar rot de representacao anterior)
	bool bVariable = (bPause || bControls ? true : false);
	pPeca.translacaoPeca(trans, bVariable);
	glm::mat4 MVP_PecaAtual = Projection * View * trans * rot;

	// retrieve the matrix uniform locations
	glUniformMatrix4fv(MVP, 1, GL_FALSE, &MVP_PecaAtual[0][0]);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : textures
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Desenha objeto
	pPeca.drawObject();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// Se peça colidiu, atualizar variável associada e não re-inicar contagem de tempo de colisão
	if (pPeca.hasCollidedBottom()) {
		bCollisionPiece = true;
	}
	else {
		// Caso não tenha, resetar variável e iniciar contagem de tempo, destino a contabilizar o tempo de colisão
		bCollisionPiece = false;
		if (!bCollisionPiece) {
			t_collision = std::chrono::high_resolution_clock::now();
		}
	}

	// Avaliar diferença entre tempo atual e tempo inicial de colisão. timerCollision é avaliado em evaluatePieceCollision
	auto t_now = std::chrono::high_resolution_clock::now();
	timerCollision = std::chrono::duration_cast<std::chrono::duration<int>>(t_now - t_collision).count();

	// Retorna informação para main se deve instanciar nova peça, ou seja, se presente peça colidiu e se o tempo
	// de colisão já foi excedido
	return evaluatePieceCollision(pPeca);
}

//--------------------------------------------------------------------------------

/// Usar função comum para desenhar peças anteriormente jogadas, próxima peça e peça guardada. Estas são diferenciadas
/// pelo identificador (argumento da função)
void drawObjects(int iIdentificador) {

	/// Distingue entre o que desenhar na peça drawObjects:\n
	///	> iIdentificador = 1 => desenhar todas as peças anteriormente jogadas\n
	///	> iIdentificador = 2 => desenhar próxima peça a jogar\n
	///	> iIdentificador = 3 => desenhar peça guardada pelo utilizador\n
	///	> iIdentificador = 4 => desenhar a palavra "SCORE"\n
	///	> iIdentificador = 5 => desenhar números da pontuação\n
	///	> iIdentificador = 6 => desenhar a palavra "LEVEL"\n
	///	> iIdentificador = 7 => desenhar números do nível\n
	///	> iIdentificador = 8 => desenhar a palavra "LINES"\n
	///	> iIdentificador = 9 => desenhar números das linhas eliminadas\n
	///	> iIdentificador = 10 => desenhar a palavra "NEXT"\n
	///	> iIdentificador = 11 => desenhar a palavra "HOLD"\n
	///	> iIdentificador = 12 => desenhar background\n
	///	> iIdentificador = 13 => desenhar grelha de jogo\n
	///	> iIdentificador = 14 => desenhar ecra de pausa\n
	///	> iIdentificador = 15 => desenhar ecra de fim de jogo\n
	///	> iIdentificador = 16 => desenhar ecra de controlos\n

	glUniform1i(globalBack, 0);

	// Distinguir o que desenhar por recurso a identificador
	GLuint iIDVertexBuffer;
	GLuint iIDTextureBuffer;
	int iDrawSize;

	// Auxiliar para obter tamanho de buffer
	std::vector<GLfloat> vertexBuffer;

	switch (iIdentificador) {
		// Desenhar peças anteriormente jogadas
		case 1:
			iIDVertexBuffer = vertexbufferTot;
			iIDTextureBuffer = texturebufferTot;
			iDrawSize = g_vertex_buffer_dataTot.size();
			break;
		// Desenhar próxima peça
		case 2:
			iIDVertexBuffer = vertexbufferNextPiece;
			iIDTextureBuffer = texturebufferNextPiece;
			iDrawSize = g_vertex_buffer_data.size();
			break;
		// Desenhar peça guardada
		case 3:
			iIDVertexBuffer = vertexbufferSavedPiece;
			iIDTextureBuffer = texturebufferSavedPiece;
			// Distinguir entre a primeira vez que se guarda peça (não havida nenhuma antes) e as restantes vezes
			iDrawSize = (iSavedPiece == 0) ? 0 : g_vertex_buffer_data.size();
			break;
		// Desenhar "SCORE"
		case 4:
			iIDVertexBuffer = vertexbufferScore;
			iIDTextureBuffer = texturebufferScore;
			vertexBuffer = vertexBufferNumber(10);
			iDrawSize = vertexBuffer.size();
			break;
		// Desenhar pontuação números
		case 5:
			iIDVertexBuffer = vertexbufferScoreNum;
			iIDTextureBuffer = texturebufferScoreNum;
			iDrawSize = g_vertex_buffer_dataScoreNum.size();
			break;
		// Desenhar "LEVEL"
		case 6:
			iIDVertexBuffer = vertexbufferLevel;
			iIDTextureBuffer = texturebufferLevel;
			vertexBuffer = vertexBufferNumber(11);
			iDrawSize = vertexBuffer.size();
			break;
		// Desenhar nível números
		case 7:
			iIDVertexBuffer = vertexbufferLevelNum;
			iIDTextureBuffer = texturebufferLevelNum;
			iDrawSize = g_vertex_buffer_dataLevelNum.size();
			break;
		// Desenhar "LINES"
		case 8:
			iIDVertexBuffer = vertexbufferLines;
			iIDTextureBuffer = texturebufferLines;
			vertexBuffer = vertexBufferNumber(12);
			iDrawSize = vertexBuffer.size();
			break;
		// Desenhar linhas números
		case 9:
			iIDVertexBuffer = vertexbufferLinesNum;
			iIDTextureBuffer = texturebufferLinesNum;
			iDrawSize = g_vertex_buffer_dataLinesNum.size();
			break;
		// Desenhar "NEXT"
		case 10:
			iIDVertexBuffer = vertexbufferNext;
			iIDTextureBuffer = texturebufferNext;
			vertexBuffer = vertexBufferNumber(13);
			iDrawSize = vertexBuffer.size();
			break;
		// Desenhar "HOLD"
		case 11:
			iIDVertexBuffer = vertexbufferHold;
			iIDTextureBuffer = texturebufferHold;
			vertexBuffer = vertexBufferNumber(14);
			iDrawSize = vertexBuffer.size();
			break;
		// Desenhar Background
		case 12:
			glUniform1i(globalBack, 1);
			iIDVertexBuffer = vertexbufferBackground;
			iIDTextureBuffer = texturebufferBackground;
			iDrawSize = g_vertex_buffer_dataBack.size();
			break;
		// Desenhar grelha de jogo
		case 13:
			glUniform1i(globalBack, 2);
			iIDVertexBuffer = vertexbufferGrid;
			iIDTextureBuffer = texturebufferGrid;
			iDrawSize = g_vertex_buffer_dataGrid.size();
			break;
		// Desenhar ecra de Pause
		case 14:
			glUniform1i(globalBack, 3);
			iIDVertexBuffer = vertexbufferPause;
			iIDTextureBuffer = texturebufferPause;
			iDrawSize = g_vertex_buffer_dataPause.size();
			break;
		// Desenhar ecra de fim de jogo
		case 15:
			glUniform1i(globalBack, 4);
			iIDVertexBuffer = vertexbufferGameOver;
			iIDTextureBuffer = texturebufferGameOver;
			iDrawSize = g_vertex_buffer_dataGameOver.size();
			break;
		// Desenhar ecra de controlos
		case 16:
			glUniform1i(globalBack, 5);
			iIDVertexBuffer = vertexbufferControls;
			iIDTextureBuffer = texturebufferControls;
			iDrawSize = g_vertex_buffer_dataControls.size();
			break;
	}

	// Use our shader
	glUseProgram(programID);

	glm::mat4 MVP_Matrix = Projection * View;

	// retrieve the matrix uniform locations
	glUniformMatrix4fv(MVP, 1, GL_FALSE, &MVP_Matrix[0][0]);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, iIDVertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : textures
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, iIDTextureBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw all the previously played pieces
	glDrawArrays(GL_TRIANGLES, 0, iDrawSize);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


//--------------------------------------------------------------------------------
void setMVP_And_UniqueLoads() {

	// VAO
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders(vertexShader, fragmentShader);

	// Get a handle for our "MVP" uniform
	MVP = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45∞ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT);


	// ------------------------------------------------------------------------------------------------------------
	// Texturas
	// (0,0) canto superior esquerdo da imagem
	glGenTextures(6, TextureID);

	// Texturas de peças
	ucaTexData = stbi_load(caTiles, &iTexWidth, &iTexHeight, &iTexNumChannels, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iTexWidth, iTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ucaTexData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(ucaTexData);

	// ------------------------------------------------------------------------------------------------------------

	// Texturas de background
	ucaTexData = stbi_load(caBackground, &iTexWidth, &iTexHeight, &iTexNumChannels, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureID[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iTexWidth, iTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ucaTexData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(ucaTexData);

	// Move vertex data to video memory; specifically to VBO called vertexbufferBackground,
	glGenBuffers(1, &vertexbufferBackground);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferBackground);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_dataBack.size() * sizeof(float), g_vertex_buffer_dataBack.data(), GL_STATIC_DRAW);

	// Move texture data to video memory; specifically to TBO called texturebufferBackground
	glGenBuffers(1, &texturebufferBackground);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferBackground);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_dataBack.size() * sizeof(float), g_texture_buffer_dataBack.data(), GL_STATIC_DRAW);

	// ------------------------------------------------------------------------------------------------------------

	// Texturas de grelha de jogo
	ucaTexData = stbi_load(caGamegrid, &iTexWidth, &iTexHeight, &iTexNumChannels, 0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureID[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iTexWidth, iTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, ucaTexData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(ucaTexData);

	// Move vertex data to video memory; specifically to VBO called vertexbufferGrid,
	glGenBuffers(1, &vertexbufferGrid);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferGrid);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_dataGrid.size() * sizeof(float), g_vertex_buffer_dataGrid.data(), GL_STATIC_DRAW);

	// Move texture data to video memory; specifically to TBO called texturebufferGrid
	glGenBuffers(1, &texturebufferGrid);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferGrid);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_dataGrid.size() * sizeof(float), g_texture_buffer_dataGrid.data(), GL_STATIC_DRAW);

	// ------------------------------------------------------------------------------------------------------------

	// Texturas de ecra de pausa
	ucaTexData = stbi_load(caPause, &iTexWidth, &iTexHeight, &iTexNumChannels, 0);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TextureID[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iTexWidth, iTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, ucaTexData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // necessário para opacidade

	stbi_image_free(ucaTexData);

	// Move vertex data to video memory; specifically to VBO called vertexbufferPause,
	glGenBuffers(1, &vertexbufferPause);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferPause);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_dataPause.size() * sizeof(float), g_vertex_buffer_dataPause.data(), GL_STATIC_DRAW);

	// Move texture data to video memory; specifically to TBO called texturebufferPause
	glGenBuffers(1, &texturebufferPause);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferPause);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_dataPause.size() * sizeof(float), g_texture_buffer_dataPause.data(), GL_STATIC_DRAW);

	// ------------------------------------------------------------------------------------------------------------

	// Texturas de ecra de fim de jogo
	ucaTexData = stbi_load(caGameOver, &iTexWidth, &iTexHeight, &iTexNumChannels, 0);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, TextureID[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iTexWidth, iTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, ucaTexData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // necessário para opacidade

	stbi_image_free(ucaTexData);

	// Move vertex data to video memory; specifically to VBO called vertexbufferGameOver,
	glGenBuffers(1, &vertexbufferGameOver);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferGameOver);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_dataGameOver.size() * sizeof(float), g_vertex_buffer_dataGameOver.data(), GL_STATIC_DRAW);

	// Move texture data to video memory; specifically to TBO called texturebufferGameOver
	glGenBuffers(1, &texturebufferGameOver);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferGameOver);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_dataGameOver.size() * sizeof(float), g_texture_buffer_dataGameOver.data(), GL_STATIC_DRAW);

	// ------------------------------------------------------------------------------------------------------------

	// Texturas de ecra de controlos
	ucaTexData = stbi_load(caControls, &iTexWidth, &iTexHeight, &iTexNumChannels, 0);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, TextureID[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iTexWidth, iTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, ucaTexData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // necessário para opacidade

	stbi_image_free(ucaTexData);

	// Move vertex data to video memory; specifically to VBO called vertexbufferControls,
	glGenBuffers(1, &vertexbufferControls);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferControls);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_dataControls.size() * sizeof(float), g_vertex_buffer_dataControls.data(), GL_STATIC_DRAW);

	// Move texture data to video memory; specifically to TBO called texturebufferControls
	glGenBuffers(1, &texturebufferControls);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferControls);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_dataControls.size() * sizeof(float), g_texture_buffer_dataControls.data(), GL_STATIC_DRAW);

	// ------------------------------------------------------------------------------------------------------------

	// Buffers
	std::vector<GLfloat> vertexBuffer;
	std::vector<GLfloat> textureBuffer;

	// ------------------------------------------------------------------------------------------------------------
	// Representacao de palavra "Score"

	vertexBuffer = vertexBufferNumber(10);
	textureBuffer = textureBufferNumber(10);

	// Move vertex data to video memory; specifically to VBO called vertexbufferScore,
	glGenBuffers(1, &vertexbufferScore);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferScore);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), vertexBuffer.data(), GL_STATIC_DRAW);

	// Move texture data to video memory; specifically to TBO called texturebufferScore
	glGenBuffers(1, &texturebufferScore);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferScore);
	glBufferData(GL_ARRAY_BUFFER, textureBuffer.size() * sizeof(float), textureBuffer.data(), GL_STATIC_DRAW);

	// ------------------------------------------------------------------------------------------------------------
	// Representacao de palavra "Level"

	vertexBuffer = vertexBufferNumber(11);
	textureBuffer = textureBufferNumber(11);

	// Move vertex data to video memory; specifically to VBO called vertexbufferLevel,
	glGenBuffers(1, &vertexbufferLevel);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferLevel);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), vertexBuffer.data(), GL_STATIC_DRAW);

	// Move texture data to video memory; specifically to TBO called texturebufferLevel
	glGenBuffers(1, &texturebufferLevel);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferLevel);
	glBufferData(GL_ARRAY_BUFFER, textureBuffer.size() * sizeof(float), textureBuffer.data(), GL_STATIC_DRAW);

	// ------------------------------------------------------------------------------------------------------------
	// Representacao de palavra "Level"

	vertexBuffer = vertexBufferNumber(12);
	textureBuffer = textureBufferNumber(12);

	// Move vertex data to video memory; specifically to VBO called vertexbufferLines,
	glGenBuffers(1, &vertexbufferLines);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferLines);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), vertexBuffer.data(), GL_STATIC_DRAW);

	// Move texture data to video memory; specifically to TBO called texturebufferLines
	glGenBuffers(1, &texturebufferLines);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferLines);
	glBufferData(GL_ARRAY_BUFFER, textureBuffer.size() * sizeof(float), textureBuffer.data(), GL_STATIC_DRAW);

	// ------------------------------------------------------------------------------------------------------------
	// Representacao de palavra "Next"

	vertexBuffer = vertexBufferNumber(13);
	textureBuffer = textureBufferNumber(13);

	// Move vertex data to video memory; specifically to VBO called vertexbufferNext,
	glGenBuffers(1, &vertexbufferNext);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferNext);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), vertexBuffer.data(), GL_STATIC_DRAW);

	// Move texture data to video memory; specifically to TBO called texturebufferNext
	glGenBuffers(1, &texturebufferNext);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferNext);
	glBufferData(GL_ARRAY_BUFFER, textureBuffer.size() * sizeof(float), textureBuffer.data(), GL_STATIC_DRAW);

	// ------------------------------------------------------------------------------------------------------------
	// Representacao de palavra "Hold"

	vertexBuffer = vertexBufferNumber(14);
	textureBuffer = textureBufferNumber(14);

	// Move vertex data to video memory; specifically to VBO called vertexbufferHold,
	glGenBuffers(1, &vertexbufferHold);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferHold);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), vertexBuffer.data(), GL_STATIC_DRAW);

	// Move texture data to video memory; specifically to TBO called texturebufferHold
	glGenBuffers(1, &texturebufferHold);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferHold);
	glBufferData(GL_ARRAY_BUFFER, textureBuffer.size() * sizeof(float), textureBuffer.data(), GL_STATIC_DRAW);

	// ------------------------------------------------------------------------------------------------------------


	// Use our shader
	glUseProgram(programID);

	globalBack = glGetUniformLocation(programID, "iBackground");
}


//--------------------------------------------------------------------------------


/// Retains information of user inputs, regarding piece movement
void registerUserInputs(Peca& pPeca) {

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {
			if (pPeca.rotationAllowed() && !bPause && !bGameOver && !bControls) {
				pPeca.incNumberRotate();
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
			if (pPeca.getXPosD() < iWidth && !pPeca.hasCollidedRight() && !bPause && !bGameOver && !bControls) {
				pPeca.incNumberTranslation();
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) {
			if (pPeca.getXPosE() > 0 && !pPeca.hasCollidedLeft() && !bPause && !bGameOver && !bControls) {
				pPeca.decNumberTranslation();
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (!pPeca.hasCollidedBottom() && !bPause && !bGameOver && !bControls) {
			pPeca.incNumberDown();
			// Atualização de pontuação
			iPontuacao++;
		}
	}

	// Guardar peça
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
			if (!bPause && !bGameOver && !bControls) {
				bAlteraPecaEmJogo = true;
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
			if (!bPause && !bGameOver && !bControls) {
				bDropPeca = true;
				// Descer peça até colidir. Retornará true caso colida
				while (!drawCurrentObject(pPeca)) {
					// Descer posição de peça enquanto não colidir
					pPeca.incNumberDown();
					// Atualização de pontuação
					iPontuacao++;
				}
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
			if (!bGameOver && !bControls) {
				bPause = !bPause;
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
			if (!bGameOver && !bPause) {
				bControls = !bControls;
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE) {
			if (!bGameOver && !bPause && !bControls) {
				bMouse = !bMouse;
				bFirstTime = !bFirstTime;
			}
		}
	}

	if (bMouse) {

		double xpos, ypos;
		//getting cursor position
		glfwGetCursorPos(window, &xpos, &ypos);

		int iLarguraPeca = pPeca.getXPosD() - pPeca.getXPosE() - 1;

		// 30 = tamanho da quadrícula de grelha de jogo
		if (xpos >= 0 && xpos <= (300 - iLarguraPeca * 30))
		{
			if (bRotateMouse) {
				int xPos = pPeca.getXPosD() - (pPeca.getXPosD() - pPeca.getXPosE());
				dXPosAnt = xPos * 30 + 15;
				glfwSetCursorPos(window, dXPosAnt, ypos);
			}

			bRotateMouse = false;

			// Calcular o número de quadrículas a movimentar
			int iMov = (xpos - dXPosAnt) / 30;

			// Se o rato se movimentou a distância de uma quadrícula
			if (abs(iMov) > 0)
			{
				// Transladar a peça o número de quadrículas movimentadas pelo rato
				for (int i = 0; i < abs(iMov); i++) {
					if (pPeca.getXPosD() < iWidth && !pPeca.hasCollidedRight() && iMov > 0 && !bPause && !bGameOver && !bControls) {
						pPeca.incNumberTranslation();
						dXPosAnt += 30;
					}

					if (pPeca.getXPosE() > 0 && !pPeca.hasCollidedLeft() && iMov < 0 && !bPause && !bGameOver && !bControls) {
						pPeca.decNumberTranslation();
						dXPosAnt -= 30;
					}
				}
			}
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) {
				if (pPeca.rotationAllowed() && !bPause && !bGameOver && !bControls) {
					pPeca.incNumberRotate();
					bRotateMouse = true;
				}
			}
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
				if (!bPause && !bGameOver && !bControls && !bFirstTime) {
					bDropPeca = true;
					// Descer peça até colidir. Retornará true caso colida
					while (!drawCurrentObject(pPeca)) {
						// Descer posição de peça enquanto não colidir
						pPeca.incNumberDown();
						// Atualização de pontuação
						iPontuacao++;
					}
				}
				bFirstTime = false;
			}
		}
	}
}

//--------------------------------------------------------------------------------

int main(void)
{
	incializaMatrizZero();
	// Initialise GLFW
	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// Open a window
	window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, NULL, NULL);

	// Create window context
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	glewInit();

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Definir variáveis associadas a programa e MVP
	setMVP_And_UniqueLoads();

	glfwSetCursorPos(window, 135, 0);
	
	// ------------------- Variáveis de peças -------------------------------//

	// Criação de gerador de peças
	geraPecas = GeradorPecas(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iNivelJogo);
	
	// Gera valor aletório, correspondente a peça de tabuleiro
	iGamePiece = randNum();
	pPeca = returnPeca(geraPecas, iGamePiece);

	// Gera novo valor aletório, correspondente à próxima peça de tabuleiro
	iGameNextPiece = randNum();
	iSavedPiece = 0; // Valor não associada a nenhuma peça
	pNextpPeca = returnPeca(geraPecas, iGameNextPiece);
	pSavedpPeca = returnPeca(geraPecas, iGameNextPiece);

	// Distingue entre o que desenhar na peça drawObjects:
	//	> iIdentificador = 1 => desenhar todas as peças anteriormente jogadas
	//	> iIdentificador = 2 => desenhar próxima peça a jogar
	//	> iIdentificador = 3 => desenhar peça guardada pelo utilizador
	//	> iIdentificador = 4 => desenhar a palavra "SCORE"
	//	> iIdentificador = 5 => desenhar números da pontuação
	//	> iIdentificador = 6 => desenhar a palavra "LEVEL"
	//	> iIdentificador = 7 => desenhar números do nível
	//	> iIdentificador = 8 => desenhar a palavra "LINES"
	//	> iIdentificador = 9 => desenhar números das linhas eliminadas
	//	> iIdentificador = 10 => desenhar a palavra "NEXT"
	//	> iIdentificador = 11 => desenhar a palavra "HOLD"
	//	> iIdentificador = 12 => desenhar background
	//	> iIdentificador = 13 => desenhar grelha de jogo
	//	> iIdentificador = 14 => desenhar ecra de pausa
	//	> iIdentificador = 15 => desenhar ecra de fim de jogo
	//	> iIdentificador = 16 => desenhar ecra de controlos

	// ----------------------------------------------------------------------//

	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);

	// Música ambiente
	sound = SoundEngine->play2D("resources/audio/tetris.wav",GL_TRUE, GL_FALSE, GL_TRUE);

	if (sound) {
		sound->setVolume(0.10);
	}

	do {

		// Utilizar clicou em "C" e pretende guardar a peça atual de jogo e instanciar a próxima
		if (bAlteraPecaEmJogo) {

			/* Verificar se havia uma peça guardada antes:
				> Se houver, trocar peça pela atual
				> Caso não haja, guardar peça atual em iSavedPiece e carregar para jogo próxima peça
					(iGameNextPiece)
			*/
			// Variável temporária para guardar informação de iGamePiece, para a atribuir a iSavedPiece
			int iPieceTempVariable = iGamePiece;

			// Alterar peça de jogo
			iGamePiece = (iSavedPiece == 0) ? iGameNextPiece : iSavedPiece;
			/* Caso não haja peças guardadas (primeira vez que se prime "C"), nova próxima peça terá de ser instanciada
			pois peça atual de jogo será a que estava destinada a ser a próxima */
			if (iSavedPiece == 0) {
				iGameNextPiece = randNum();
			}

			// Guardar antiga peça de jogo em iSavedPiece
			iSavedPiece = iPieceTempVariable;

			// Instanciar peça
			pPeca = returnPeca(geraPecas, iGamePiece);

			// Resetar variável
			bAlteraPecaEmJogo = false;

			bCollisionPiece = false;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Transferir informação para GPU, relativamente às peças desenhadas
		transferDataToGPUMemory(*pPeca, *pNextpPeca, *pSavedpPeca);

		// Draw Gamegrid
		glViewport(0, 0, WindowWidth, WindowHeight);
		drawObjects(13); // 13 => Identificador associado a desenhar grelha de jogo

		// Draw previously played pieces into the board
		glViewport(0, 0, WindowWidth, WindowHeight);
		drawObjects(1); // 1 => Identificador associado a desenhar todas as peças anteriormente jogadas

		// drawing callback
		glViewport(0, 0, WindowWidth, WindowHeight);
		
		// Caso a peça tenha sido "dropped"
		if (bDropPeca) {

			// Definição da posição inicial
			dXPosAnt = 135;
			glfwSetCursorPos(window, 135, 0);

			bCollisionPiece = false;
			// Instanciar a mesma peça que "iGameNextPiece" indicou que vinha
			iGamePiece = iGameNextPiece;
			pPeca = returnPeca(geraPecas, iGamePiece);

			// Alterar variável aleatória para gerar próxima peça
			iGameNextPiece = randNum();

			// Reset de variável
			bDropPeca = false;
		}
		// Descida natural da peça ou por cliquem em "Down" button, not "Dropped" (press space)
		else {
			if (!bGameOver) {
				if (drawCurrentObject(*pPeca)) {

					// Definição da posição inicial
					dXPosAnt = 135;
					glfwSetCursorPos(window, 135, 0);

					bCollisionPiece = false;
					// Instanciar a mesma peça que "iGameNextPiece" indicou que vinha
					iGamePiece = iGameNextPiece;
					pPeca = returnPeca(geraPecas, iGamePiece);

					// Alterar variável aleatória para gerar próxima peça
					iGameNextPiece = randNum();
				}
			}
		}

		// Draw Background
		glViewport(WindowWidth*0.72, 0, WindowWidth*0.6, WindowHeight);
		drawObjects(12);

		// Representa "SCORE"
		glViewport(WindowWidth*0.75, WindowHeight*0.93, WindowWidth*0.6, WindowHeight*0.6);
		drawObjects(4); // 4 => Identificador associado a desenhar a palavra "SCORE"

		// Representa números da pontuação
		glViewport(WindowWidth*0.67, WindowHeight*0.87, WindowWidth*0.6, WindowHeight*0.6);
		drawScore(iPontuacao);
		drawObjects(5); // 5 => Identificador associado a desenhar os números da pontuação

		// Representa "HOLD"
		glViewport(WindowWidth*0.77, WindowHeight*0.77, WindowWidth*0.6, WindowHeight*0.6);
		drawObjects(11); // 11 => Identificador associado a desenhar a palavra "HOLD"

		// Representa a peça guardada peça
		glViewport(WindowWidth*0.8, WindowHeight*0.7, WindowWidth*0.6, WindowHeight*0.6);
		drawObjects(3); // 3 => Identificador associado a desenhar peça guardada por utilizador

		// Representa "NEXT"
		glViewport(WindowWidth*0.77, WindowHeight*0.57, WindowWidth*0.6, WindowHeight*0.6);
		drawObjects(10); // 10 => Identificador associado a desenhar a palavra "NEXT"

		// Representa a próxima peça
		glViewport(WindowWidth*0.8, WindowHeight*0.5, WindowWidth*0.6, WindowHeight*0.6);
		drawObjects(2); // 2 => Identificador associado a desenhar próxima peça

		// Representa "LEVEL"
		glViewport(WindowWidth*0.75, WindowHeight*0.4, WindowWidth*0.6, WindowHeight*0.6);
		drawObjects(6); // 6 => Identificador associado a desenhar a palavra "LEVEL"

		// Representa números do nível
		glViewport(WindowWidth*0.77, WindowHeight*0.34, WindowWidth*0.6, WindowHeight*0.6);
		drawLevel(iNivelJogo);
		drawObjects(7); // 7 => Identificador associado a desenhar os números do nível

		// Representa "LINES"
		glViewport(WindowWidth*0.75, WindowHeight*0.27, WindowWidth*0.6, WindowHeight*0.6);
		drawObjects(8); // 8 => Identficador associado a desenhar a palavra "LINES"

		// Representa números do linhas
		glViewport(WindowWidth*0.75, WindowHeight*0.20, WindowWidth*0.6, WindowHeight*0.6);
		drawLines(iTotalLinhasEliminadasEmNível);
		drawObjects(9); // 9 => Identificador associado a desenhar os números das linhas eliminadas

		// Representa Pause
		if (bPause) {
			glViewport(0, 0, WindowWidth, WindowHeight);
			drawObjects(14);
		}

		// Representa Pause
		if (bGameOver) {
			glViewport(0, 0, WindowWidth, WindowHeight);
			drawObjects(15);
		}

		// Representa Controlos
		if (bControls) {
			glViewport(0, 0, WindowWidth, WindowHeight);
			drawObjects(16);
		}

		// Swap buffer
		glfwSwapBuffers(window);

		// Register user inputs regarding piece movement
		registerUserInputs(*pPeca);

		// looking for events
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VAO, VBOs, and shaders from GPU
	cleanupDataFromGPU();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

//--------------------------------------------------------------------------------

/// Função para desenhar todo o ambiente, sem instanciar peças (usado aquando da eliminação de linhas)
void desenhaAmbiente() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transferir informação para GPU, relativamente às peças desenhadas
	transferDataToGPUMemory(*pPeca, *pNextpPeca, *pSavedpPeca);

	// Draw Gamegrid
	glViewport(0, 0, WindowWidth, WindowHeight);
	drawObjects(13); // 13 => Identificador associado a desenhar grelha de jogo

	// Draw previously played pieces into the board
	glViewport(0, 0, WindowWidth, WindowHeight);
	drawObjects(1); // 1 => Identificador associado a desenhar todas as peças anteriormente jogadas

	// Draw Background
	glViewport(WindowWidth*0.72, 0, WindowWidth*0.6, WindowHeight);
	drawObjects(12); // 12 => Identificador associado a desenhar background

	// Representa "SCORE"
	glViewport(WindowWidth*0.75, WindowHeight*0.93, WindowWidth*0.6, WindowHeight*0.6);
	drawObjects(4); // 4 => Identificador associado a desenhar a palavra "SCORE"

	// Representa números da pontuação
	glViewport(WindowWidth*0.67, WindowHeight*0.87, WindowWidth*0.6, WindowHeight*0.6);
	drawScore(iPontuacao);
	drawObjects(5); // 5 => Identificador associado a desenhar os números da pontuação

	// Representa "HOLD"
	glViewport(WindowWidth*0.77, WindowHeight*0.77, WindowWidth*0.6, WindowHeight*0.6);
	drawObjects(11); // 11 => Identificador associado a desenhar a palavra "HOLD"

	// Representa a peça guardada peça
	glViewport(WindowWidth*0.8, WindowHeight*0.7, WindowWidth*0.6, WindowHeight*0.6);
	drawObjects(3); // 3 => Identificador associado a desenhar peça guardada por utilizador

	// Representa "NEXT"
	glViewport(WindowWidth*0.77, WindowHeight*0.57, WindowWidth*0.6, WindowHeight*0.6);
	drawObjects(10); // 10 => Identificador associado a desenhar a palavra "NEXT"

	// Representa a próxima peça
	glViewport(WindowWidth*0.8, WindowHeight*0.5, WindowWidth*0.6, WindowHeight*0.6);
	drawObjects(2); // 2 => Identificador associado a desenhar próxima peça

	// Representa "LEVEL"
	glViewport(WindowWidth*0.75, WindowHeight*0.4, WindowWidth*0.6, WindowHeight*0.6);
	drawObjects(6); // 6 => Identificador associado a desenhar a palavra "LEVEL"

	// Representa números do nível
	glViewport(WindowWidth*0.77, WindowHeight*0.34, WindowWidth*0.6, WindowHeight*0.6);
	drawLevel(iNivelJogo);
	drawObjects(7); // 7 => Identificador associado a desenhar os números do nível

	// Representa "LINES"
	glViewport(WindowWidth*0.75, WindowHeight*0.27, WindowWidth*0.6, WindowHeight*0.6);
	drawObjects(8); // 8 => Identficador associado a desenhar a palavra "LINES"

	// Representa números do linhas
	glViewport(WindowWidth*0.75, WindowHeight*0.20, WindowWidth*0.6, WindowHeight*0.6);
	drawLines(iTotalLinhasEliminadasEmNível);
	drawObjects(9); // 9 => Identificador associado a desenhar os números das linhas eliminadas

	// Swap buffer
	glfwSwapBuffers(window);
}
