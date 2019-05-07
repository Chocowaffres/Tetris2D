#include "headers/PecaL.hpp"
#include "headers/PecaJ.hpp"
#include "headers/PecaS.hpp"
#include "headers/PecaI.hpp"
#include "headers/PecaZ.hpp"
#include "headers/PecaO.hpp"
#include "headers/PecaT.hpp"
#include "headers/Peca.hpp"
#include "headers/GeradorPecas.hpp" 

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

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

// Vertex array object (VAO)
GLuint VertexArrayID;

// Texture array object (TAO)
GLuint TextureID[3];

// Vertex buffer object (VBO)
GLuint vertexbuffer;
GLuint vertexbufferTot;
GLuint vertexbufferNextPiece;
GLuint vertexbufferSavedPiece;
GLuint vertexbufferBackground;
GLuint vertexbufferPosPiece;
GLuint vertexbufferGrid;

// Texture buffer object (TBO)
GLuint texturebuffer;
GLuint texturebufferTot;
GLuint texturebufferNextPiece;
GLuint texturebufferSavedPiece;
GLuint texturebufferBackground;
GLuint texturebufferPosPiece;
GLuint texturebufferGrid;

// GLSL program from the shaders
GLuint programID;

int const iWidth = 10;
int const iHeight = 20;
GLfloat WIDTH_PosXInicial = 10.f;
GLfloat HEIGHT_PosYInicial = 20.f;
GLfloat WIDTH = 14.f; // 15. 0 f
GLfloat HEIGHT = 20.f;
GLint WindowHeight = 600;
GLint WindowWidth = WIDTH / HEIGHT * WindowHeight;

char vertexShader[] = "shaders/vertexShader.vertexshader";
char fragmentShader[] = "shaders/fragmentShader.fragmentshader";
char WindowTitle[] = "Tetris";
const char* caTiles = "resources/tiles_large.png";
const char* caBackground = "resources/background.png";
const char* caGamegrid = "resources/gamegrid.png";

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
GeradorPecas geraPecas;
int iGamePiece;
int iGameNextPiece;
int iSavedPiece = 0;
bool bAlteraPecaEmJogo = false;
bool bDropPeca = false;

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
unsigned int globalTile;

// Vértices da peça atual
std::vector<GLfloat> g_vertex_buffer_data = {};

// Textura da peça atual
std::vector<GLfloat> g_texture_buffer_data = {};

// Vértices de todas as peças
std::vector<GLfloat> g_vertex_buffer_dataTot = {};

// Texturas de todas as peças
std::vector<GLfloat> g_texture_buffer_dataTot = {};

// Vértices de background
std::vector<GLfloat> g_vertex_buffer_dataBack = {
	0.0f,  0.0f,   0.1f,
	14.0f, 0.0f,   0.1f,
	0.0f,  20.0f,  0.1f,

	14.0f, 0.0f,   0.1f,
	0.0f,  20.0f,  0.1f,
	14.0f, 20.0f,  0.1f,
};

// Textura de background
std::vector<GLfloat> g_texture_buffer_dataBack = {
	0.0f,  1.0f,
	1.0f,  1.0f,
	0.0f,  0.0f,

	1.0f,  1.0f,
	0.0f,  0.0f,
	1.0f,  0.0f,
};

// Vértices da grelha de jogo
std::vector<GLfloat> g_vertex_buffer_dataGrid = {
	0.0f,  0.0f,   0.1f,
	10.0f, 0.0f,   0.1f,
	0.0f,  20.0f,  0.1f,

	10.0f, 0.0f,   0.1f,
	0.0f,  20.0f,  0.1f,
	10.0f, 20.0f,  0.1f,
};

// Textura da grelha de jogo
std::vector<GLfloat> g_texture_buffer_dataGrid = {
	0.0f,  1.0f,
	1.0f,  1.0f,
	0.0f,  0.0f,

	1.0f,  1.0f,
	0.0f,  0.0f,
	1.0f,  0.0f,
};

// Identificador de matriz MVP nos shaders
GLuint MVP;

// Matrizes associadas a MVP
glm::mat4 Projection = glm::mat4(1.0f);
glm::mat4 View = glm::mat4(1.0f);
// Model definido nas funções apropriadas

//--------------------------------------------------------------------------------

// Inicializar matriz a zeros
void incializaMatrizZero() {
	gameGrid = (int**)calloc(iWidth, sizeof(int*));
	for (int i = 0; i < iWidth; i++) {
		gameGrid[i] = (int*)calloc(iHeight, sizeof(int));
	}
}

// Generates random number from 0 to 6 (7 pieces)
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

void atualizaNivelJogo() {
	if (iNivelJogo < 10) {
		if (iTotalLinhasEliminadasEmNível >= (10 * (iNivelJogo + 1))) {
			iNivelJogo++;
			iTotalLinhasEliminadasEmNível = 0;
		}
	}
	else if (iNivelJogo < 16) {
		if (iTotalLinhasEliminadasEmNível >= 100) {
			iNivelJogo++;
			iTotalLinhasEliminadasEmNível = 0;
		}
	}
	else if (iNivelJogo < 26) {
		if (iTotalLinhasEliminadasEmNível >= (10 * (iNivelJogo - 5))) {
			iNivelJogo++;
			iTotalLinhasEliminadasEmNível = 0;
		}
	}
	else {
		if (iTotalLinhasEliminadasEmNível >= 200) {
			if (iNivelJogo < 29) {
				iNivelJogo++;
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

//--------------------------------------------------------------------------------

void transferDataToGPUMemory(Peca& pPeca, Peca& pNextPeca, Peca& pSavedPeca)
{

	// Recolha de informação de vertices e cor da peça atual
	g_vertex_buffer_data = vertexBufferPiece(pPeca, iGamePiece);
	g_texture_buffer_data = textureBufferPiece(pPeca, iGamePiece);

	// Recolha de informação de vertices e cor da próxima peça peça
	std::vector<GLfloat> g_vertex_buffer_data_next_piece = vertexBufferPiece(pNextPeca, iGameNextPiece);
	std::vector<GLfloat> g_texture_buffer_data_next_piece = textureBufferPiece(pNextPeca, iGameNextPiece);

	// Recolha de informação de vertices e cor da peça guardada por user (clique em "C")
	std::vector<GLfloat> g_vertex_buffer_data_saved_piece = vertexBufferPiece(pSavedPeca, iSavedPiece);
	std::vector<GLfloat> g_texture_buffer_data_saved_piece = textureBufferPiece(pSavedPeca, iSavedPiece);

	// Distinguir enter a primeira vez que se guarda peça (não havida nenhuma antes) e as restantes vezes
	if (iSavedPiece == 0) {
		std::vector<GLfloat> g_vertex_buffer_data_saved_piece = {};
		std::vector<GLfloat> g_texture_buffer_data_saved_piece = {};
	}

	// Recolha de informação de vertices e cor da poisção da peça aquando da colisão
	std::vector<GLfloat> g_vertex_buffer_data_pos_piece = vertexBufferPiece(pPeca, iGamePiece);
	std::vector<GLfloat> g_texture_buffer_data_pos_piece = textureBufferPosPiece(pPeca, iGamePiece);

	// Move vertex data to video memory; specifically to VBO called vertexbuffer,
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size() * sizeof(float), g_vertex_buffer_data.data(), GL_STATIC_DRAW);

	// Move color data to video memory; specifically to CBO called colorbuffer
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
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteTextures(3, TextureID);
	glDeleteProgram(programID);
}

//--------------------------------------------------------------------------------

int newSizeVertex;
int newSizeTexture;
// Caso haja uma linha completa, esta será eliminada na matriz de inteiros. Esta função garante que o observado no campo de jogo
// é concordante com o ocorrido na matriz de inteiros
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

// Atualiza matriz de inteiros, visando concordância com o observado no campo de jogo. Nesta função todas as linhas acima de iLinha,
// a linha a eliminar, serão substituidas pelas seguintes destas
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

// Verifica, na matriz de inteiros, linhas que apenas contenham 1's, ou seja, linhas que terão de ser eliminadas
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
			cout << "i: " << i << endl;
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
			cout << "GAME OVER" << endl;
			getchar();
			exit(0);
		}

		// Atualiza vertexBuffer da peça, mediante o local onde esta colidiu
		pPeca.realVertexBuffer();
		// Receber grelha de jogo (matriz de inteiros) atualizada
		gameGrid = pPeca.getGameGrid();

		cout << "COLISAO" << endl;
		for (int i = iHeight - 1; i >= 0; i--) {
			for (int j = 0; j < iWidth; j++) {
				cout << gameGrid[j][i] << ", ";
			}
			cout << endl;
		}

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

			cout << "iPontuacao: " << iPontuacao << ", Nivel: " << iNivelJogo << ", iTotalLinhasEliminadasEmNível: " << iTotalLinhasEliminadasEmNível << endl;

			// Começas a eliminar as linhas do topo e ir descendo, daí a inversão do vetor
			for (int i = vLinhasAEliminar.size() - 1; i >= 0; i--) {
				eliminaLinha(vLinhasAEliminar[i]);
				atualizaCampoJogo(vLinhasAEliminar[i]);
				g_vertex_buffer_dataTot.resize(newSizeVertex);
				g_texture_buffer_dataTot.resize(newSizeTexture);
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

	// Aplicação de rotação à peça
	pPeca.rotacaoPeca(rot);
	pPeca.translacaoPecaContorno(trans);

	glm::mat4 MVP_PecaColisao = Projection * View * trans * rot;

	// retrieve the matrix uniform locations
	glUniformMatrix4fv(MVP, 1, GL_FALSE, &MVP_PecaColisao[0][0]);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferPosPiece);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferPosPiece);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Desenha objeto
	pPeca.drawObject();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	trans = glm::mat4(1.0f);

	// Aplicação de translação à peça
	pPeca.translacaoPeca(trans);

	glm::mat4 MVP_PecaAtual = Projection * View * trans * rot;

	// retrieve the matrix uniform locations
	glUniformMatrix4fv(MVP, 1, GL_FALSE, &MVP_PecaAtual[0][0]);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : colors
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

	/* Retorna informação para main se deve instanciar nova peça, ou seja, se presente peça colidiu e se o tempo
	de colisão já foi excedido */
	return evaluatePieceCollision(pPeca);
}

//--------------------------------------------------------------------------------

/* Usar função comum para desenhar peças anteriormente jogadas, próxima peça e peça guardada. Estas são diferenciadas
pelo identificador (argumento da função) */
void drawObjects(int iIdentificador) {

	glUniform1i(globalBack, 0);

	// Distinguir o que desenhar por recurso a identificador
	GLuint iIDVertexBuffer;
	GLuint iIDTextureBuffer;
	int iDrawSize;
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

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, iIDTextureBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw all the previously played pieces
	glDrawArrays(GL_TRIANGLES, 0, iDrawSize);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

//--------------------------------------------------------------------------------
void drawBackground() {

	// Use our shader
	glUseProgram(programID);

	glUniform1i(globalBack, 1);

	glm::mat4 MVP_Background = Projection * View;

	// retrieve the matrix uniform locations
	glUniformMatrix4fv(MVP, 1, GL_FALSE, &MVP_Background[0][0]);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferBackground);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferBackground);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw all the previously played pieces
	glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer_dataBack.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

//--------------------------------------------------------------------------------
void drawGamegrid() {

	// Use our shader
	glUseProgram(programID);

	glUniform1i(globalBack, 2);

	glm::mat4 MVP_Background = Projection * View;

	// retrieve the matrix uniform locations
	glUniformMatrix4fv(MVP, 1, GL_FALSE, &MVP_Background[0][0]);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferGrid);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferGrid);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw all the previously played pieces
	glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer_dataGrid.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

//--------------------------------------------------------------------------------
void setMVP() {

	// VAO
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders(vertexShader, fragmentShader);

	// Get a handle for our "MVP" uniform
	MVP = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45∞ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT);

	// Texturas
	// (0,0) canto superior esquerdo da imagem
	glGenTextures(2, TextureID);

	ucaTexData = stbi_load(caTiles, &iTexWidth, &iTexHeight, &iTexNumChannels, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iTexWidth, iTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ucaTexData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(ucaTexData);

	ucaTexData = stbi_load(caBackground, &iTexWidth, &iTexHeight, &iTexNumChannels, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureID[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iTexWidth, iTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, ucaTexData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(ucaTexData);

	ucaTexData = stbi_load(caGamegrid, &iTexWidth, &iTexHeight, &iTexNumChannels, 0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureID[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iTexWidth, iTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, ucaTexData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(ucaTexData);

	// Move vertex data to video memory; specifically to VBO called vertexbufferBackground,
	glGenBuffers(1, &vertexbufferBackground);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferBackground);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_dataBack.size() * sizeof(float), g_vertex_buffer_dataBack.data(), GL_STATIC_DRAW);

	// Move color data to video memory; specifically to TBO called texturebufferBackground
	glGenBuffers(1, &texturebufferBackground);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferBackground);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_dataBack.size() * sizeof(float), g_texture_buffer_dataBack.data(), GL_STATIC_DRAW);

	// Move vertex data to video memory; specifically to VBO called vertexbufferBackground,
	glGenBuffers(1, &vertexbufferGrid);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferGrid);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_dataGrid.size() * sizeof(float), g_vertex_buffer_dataGrid.data(), GL_STATIC_DRAW);

	// Move color data to video memory; specifically to TBO called texturebufferBackground
	glGenBuffers(1, &texturebufferGrid);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferGrid);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_dataGrid.size() * sizeof(float), g_texture_buffer_dataGrid.data(), GL_STATIC_DRAW);

	// Use our shader
	glUseProgram(programID);

	globalBack = glGetUniformLocation(programID, "iBackground");
}


//--------------------------------------------------------------------------------

// Retains information of user inputs, regarding piece movement
void registerUserInputs(Peca& pPeca) {

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {
			if (pPeca.rotationAllowed()) {
				pPeca.incNumberRotate();
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
			if (pPeca.getXPosD() < iWidth && !pPeca.hasCollidedRight()) {
				pPeca.incNumberTranslation();
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) {
			if (pPeca.getXPosE() > 0 && !pPeca.hasCollidedLeft()) {
				pPeca.decNumberTranslation();
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (!pPeca.hasCollidedBottom()) {
			pPeca.incNumberDown();
			// Atualização de pontuação
			iPontuacao++;
		}
	}

	// Guardar peça
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
			bAlteraPecaEmJogo = true;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
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

	// Open a window
	window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, NULL, NULL);

	// Create window context
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	glewInit();

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Definir variáveis associadas a programa e MVP
	setMVP();

	// ------------------- Variáveis de peças -------------------------------//

	// Criação de gerador de peças
	geraPecas = GeradorPecas(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iNivelJogo);
	
	// Gera valor aletório, correspondente a peça de tabuleiro
	iGamePiece = randNum();
	Peca* pPeca = returnPeca(geraPecas, iGamePiece);

	// Gera novo valor aletório, correspondente à próxima peça de tabuleiro
	iGameNextPiece = randNum();
	iSavedPiece = 0; // Valor não associada a nenhuma peça
	Peca* pNextpPeca = returnPeca(geraPecas, iGameNextPiece);
	Peca* pSavedpPeca = returnPeca(geraPecas, iGameNextPiece);

	/* Distingue entre o que desenhar na peça drawObjects:
		> iIdentificador = 1 => desenhar todas as peças anteriormente jogadas
		> iIdentificador = 2 => desenhar próxima peça a jogar
		> iIdentificador = 3 => desenhar peça guardada pelo utilizador
	*/
	int iIdentificador;

	// ----------------------------------------------------------------------//

	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	// render scene for each frame
	do {

		// Register user inputs regarding piece movement
		registerUserInputs(*pPeca);

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

		// Draw background
		glViewport(0, 0, WindowWidth, WindowHeight);
		drawBackground();

		// Draw gamegrid
		glViewport(0, 0, WindowWidth, WindowHeight);
		drawGamegrid();

		// Draw previously played pieces into the board
		glViewport(0, 0, WindowWidth, WindowHeight);
		drawObjects(1); // 1 => Identificador associado a desenhar todas as peças anteriormente jogadas

		// drawing callback
		glViewport(0, 0, WindowWidth, WindowHeight);
		// Caso a peça tenha sido "dropped"
		if (bDropPeca) {
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
			if (drawCurrentObject(*pPeca)) {
				bCollisionPiece = false;
				// Instanciar a mesma peça que "iGameNextPiece" indicou que vinha
				iGamePiece = iGameNextPiece;
				pPeca = returnPeca(geraPecas, iGamePiece);

				// Alterar variável aleatória para gerar próxima peça
				iGameNextPiece = randNum();
			}
		}

		// Representa a próxima peça
		glViewport(WindowWidth*0.8, WindowHeight*0.5, WindowWidth*0.6, WindowHeight*0.6);
		drawObjects(2); // 2 => Identificador associado a desenhar próxima peça

		// Representa a peça guardada peça
		glViewport(WindowWidth*0.8, WindowHeight*0.8, WindowWidth*0.6, WindowHeight*0.6);
		drawObjects(3); // 3 => Identificador associado a desenhar peça guardada por utilizador

		// Swap buffer
		glfwSwapBuffers(window);

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

