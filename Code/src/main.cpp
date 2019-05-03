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
GLuint TextureID;

// Vertex buffer object (VBO)
GLuint vertexbuffer;
GLuint vertexbufferTot;
GLuint vertexbufferNextPiece;

// color buffer object (CBO)
GLuint texturebuffer;
GLuint texturebufferTot;
GLuint texturebufferNextPiece;

// GLSL program from the shaders
GLuint programID;

int const iWidth = 11;
int const iHeight = 16;
GLfloat WIDTH_PosXInicial = 11.f;
GLfloat HEIGHT_PosYInicial = 16.f;
GLfloat WIDTH = 15.f;
GLfloat HEIGHT = 16.f;
GLint WindowHeight = 600;
GLint WindowWidth = WIDTH / HEIGHT * WindowHeight;

char vertexShader[] = "shaders/vertexShader.vertexshader";
char fragmentShader[] = "shaders/fragmentShader.fragmentshader";
char WindowTitle[] = "Tetris";
const char* caTiles = "resources/tiles_large.png";

// Timer
std::chrono::time_point<std::chrono::steady_clock> t_start;
std::chrono::time_point<std::chrono::steady_clock> t_collision;
int timerCollision;

// Collision
bool bCollisionPiece = false;

int** gameGrid;
// Posição inicial da peça, no topo e ao centro
int xPosInicial = (int)WIDTH_PosXInicial / 2, yPosInicial = HEIGHT_PosYInicial;

GeradorPecas geraPecas;
int iRandPiece;
int iRandPieceNextPiece;

// Variáveis de textura
int iTexWidth;
int iTexHeight;
int iTexNumChannels;
unsigned char* ucaTexData;

// Vértices da peça atual
std::vector<GLfloat> g_vertex_buffer_data = {};

// Textura da peça atual
std::vector<GLfloat> g_texture_buffer_data = {};

// Vértices de todas as peças
std::vector<GLfloat> g_vertex_buffer_dataTot = {};

// Texturas de todas as peças
std::vector<GLfloat> g_texture_buffer_dataTot = {};

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

Peca* returnPeca(GeradorPecas& geraPecas, int switchValue) {
	Peca* pecaGenerica;
	switch (switchValue) {
		case 1:
			pecaGenerica = &(geraPecas.getPecaZ());
			geraPecas.criaPecaZ(gameGrid);
			return pecaGenerica;
		case 2:
			pecaGenerica = &(geraPecas.getPecaT());
			geraPecas.criaPecaT(gameGrid);
			return pecaGenerica;
		case 3:
			pecaGenerica = &(geraPecas.getPecaJ());
			geraPecas.criaPecaJ(gameGrid);
			return pecaGenerica;
		case 4:
			pecaGenerica = &(geraPecas.getPecaS());
			geraPecas.criaPecaS(gameGrid);
			return pecaGenerica;
		case 5:
			pecaGenerica = &(geraPecas.getPecaO());
			geraPecas.criaPecaO(gameGrid);
			return pecaGenerica;
		case 6:
			pecaGenerica = &(geraPecas.getPecaL());
			geraPecas.criaPecaL(gameGrid);
			return pecaGenerica;
		case 7:
			pecaGenerica = &(geraPecas.getPecaI());
			geraPecas.criaPecaI(gameGrid);
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
	switch (iRandPiece) {
	case 1: return &dynamic_cast<PecaZ&>(pPeca).g_real_vertex_buffer;
	case 2: return &dynamic_cast<PecaT&>(pPeca).g_real_vertex_buffer;
	case 3: return &dynamic_cast<PecaJ&>(pPeca).g_real_vertex_buffer;
	case 4: return &dynamic_cast<PecaS&>(pPeca).g_real_vertex_buffer;
	case 5: return &dynamic_cast<PecaO&>(pPeca).g_real_vertex_buffer;
	case 6: return &dynamic_cast<PecaL&>(pPeca).g_real_vertex_buffer;
	case 7: return &dynamic_cast<PecaI&>(pPeca).g_real_vertex_buffer;
	}
}

//--------------------------------------------------------------------------------

void transferDataToGPUMemory(Peca& pPeca, Peca& pNextPeca)
{

	// Recolha de informação de vertices e cor da peça atual
	g_vertex_buffer_data = vertexBufferPiece(pPeca, iRandPiece);
	g_texture_buffer_data = textureBufferPiece(pPeca, iRandPiece);

	// Recolha de informação de vertices e cor da próxima peça peça
	std::vector<GLfloat> g_vertex_buffer_data_next_piece = vertexBufferPiece(pNextPeca, iRandPieceNextPiece);
	std::vector<GLfloat> g_texture_buffer_data_next_piece = textureBufferPiece(pNextPeca, iRandPieceNextPiece);

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

	// Carregar informação próxima peça
	glGenBuffers(1, &vertexbufferNextPiece);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferNextPiece);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data_next_piece.size() * sizeof(float), g_vertex_buffer_data_next_piece.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &texturebufferNextPiece);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferNextPiece);
	glBufferData(GL_ARRAY_BUFFER, g_texture_buffer_data_next_piece.size() * sizeof(float), g_texture_buffer_data_next_piece.data(), GL_STATIC_DRAW);

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
	glDeleteVertexArrays(1, &VertexArrayID);
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

	// Avaliar colisão e verificar se a peça está colidida à mais de 1 segundo
	if (pPeca.avaliaColisao() && timerCollision > 1) {

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
		if (vLinhasAEliminar.size() != 0) {
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

bool drawObject(Peca& pPeca)
{
	// Use our shader
	glUseProgram(programID);

	// Definição de matrizes
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);

	// Aplicação de rotação à peça
	pPeca.rotacaoPeca(rot);
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

	// Se peça colidiu, não atualizar o tempo em que houve colisão
	if (pPeca.hasCollidedBottom()) {
		bCollisionPiece = true;
	}
	else {
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

void drawPreviousObjects() {

	// Use our shader
	glUseProgram(programID);

	glm::mat4 MVP_PecaAnteriormenteJogadas = Projection * View;

	// retrieve the matrix uniform locations
	glUniformMatrix4fv(MVP, 1, GL_FALSE, &MVP_PecaAnteriormenteJogadas[0][0]);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferTot);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferTot);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw all the previously played pieces
	glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer_dataTot.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

//--------------------------------------------------------------------------------

void drawNextPiece(Peca& pPeca) {

	// Use our shader
	glUseProgram(programID);

	glm::mat4 MVP_ProximaPecaAJogar = Projection * View;

	// retrieve the matrix uniform locations
	glUniformMatrix4fv(MVP, 1, GL_FALSE, &MVP_ProximaPecaAJogar[0][0]);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferNextPiece);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferNextPiece);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw all the previously played pieces
	glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer_data.size());

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
	ucaTexData = stbi_load(caTiles, &iTexWidth, &iTexHeight, &iTexNumChannels, 0);

	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iTexWidth, iTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ucaTexData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(ucaTexData);

	unsigned int globalTex = glGetUniformLocation(programID, "Texture");
	glUniform1i(globalTex, 0);
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

	// Criação de gerador de peças
	geraPecas = GeradorPecas(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid);

	// Gera valor aletório, correspondente a peça de tabuleiro
	//iRandPiece = randNum();
	int valorTeste = 7;
	iRandPiece = valorTeste;
	Peca* pPeca = returnPeca(geraPecas, iRandPiece);


	// Gera novo valor aletório, correspondente à próxima peça de tabuleiro
	//iRandPieceNextPiece = randNum();
	iRandPieceNextPiece = valorTeste;
	Peca* pNextpPeca = returnPeca(geraPecas, iRandPieceNextPiece);

	// transfer my data (vertices, colors, and shaders) to GPU side

	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// render scene for each frame
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		transferDataToGPUMemory(*pPeca, *pNextpPeca);

		// Register user inputs regarding piece movement
		registerUserInputs(*pPeca);

		// Draw previously played pieces into the board
		glViewport(0, 0, WindowWidth, WindowHeight);
		drawPreviousObjects();

		// drawing callback
		glViewport(0, 0, WindowWidth, WindowHeight);
		if (drawObject(*pPeca)) {

			bCollisionPiece = false;
			// Instanciar a mesma peça que "iRandPieceNextPiece" indicou que vinha
			iRandPiece = iRandPieceNextPiece;
			pPeca = returnPeca(geraPecas, iRandPiece);

			// Alterar variável aleatória para gerar próxima peça
			// iRandPieceNextPiece = randNum();
			iRandPieceNextPiece = valorTeste;

		}

		// Representa a próxima peça
		glViewport(WindowWidth*0.8, WindowHeight*0.5, WindowWidth*0.6, WindowHeight*0.6);
		drawNextPiece(*pNextpPeca);

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

