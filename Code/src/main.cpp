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

// Vertex array object (VAO)
GLuint VertexArrayID;

// Vertex buffer object (VBO)
GLuint vertexbuffer;
GLuint vertexbufferTot;

// color buffer object (CBO)
GLuint colorbuffer;
GLuint colorbufferTot;

// GLSL program from the shaders
GLuint programID;

int const iWidth = 11;
int const iHeight = 16;
GLfloat WIDTH = 11.f;
GLfloat HEIGHT = 16.f;
GLint WindowHeight = 600;
GLint WindowWidth = WIDTH / HEIGHT * WindowHeight;

char vertexShader[] = "shaders/vertexShader.vertexshader";
char fragmentShader[] = "shaders/fragmentShader.fragmentshader";
char WindowTitle[] = "Tetris";

std::chrono::time_point<std::chrono::steady_clock> t_start;
int** gameGrid;
// Posição inicial da peça, no topo e ao centro
int xPosInicial = (int)WIDTH / 2, yPosInicial = HEIGHT;

GeradorPecas geraPecas;
int iRandPiece;


// Inicializar matriz a zeros
void incializaMatrizZero() {
	gameGrid = (int**)calloc(iWidth, sizeof(int*));
	for (int i = 0; i < iWidth; i++) {
		gameGrid[i] = (int*)calloc(iHeight, sizeof(int));
	}
}
  

// Vértices da peça
std::vector<GLfloat> g_vertex_buffer_data = {};

// Cor da peça
std::vector<GLfloat> g_color_buffer_data = {};

// Vértices de todas as peças
std::vector<GLfloat> g_vertex_buffer_dataTot = {};

// Cores de todas as peças
std::vector<GLfloat> g_color_buffer_dataTot = {};


//--------------------------------------------------------------------------------

// Generates random number from 0 to 6 (7 pieces)
void randNum() {
	// Conferir aleatoriedade
	srand(time(NULL));
	iRandPiece = (std::rand() % 7) + 1;
	//iRandPiece = 7;
	cout << " Random: " << iRandPiece << endl;
}

//--------------------------------------------------------------------------------

Peca* returnPeca(GeradorPecas& geraPecas) {
	Peca* pecaGenerica;
	switch (iRandPiece) {
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

std::vector<GLfloat> vertexBufferPiece(Peca& plPeca) {
	switch (iRandPiece){
		case 1: return dynamic_cast<PecaZ&>(plPeca).g_vertex_buffer_data;
		case 2: return dynamic_cast<PecaT&>(plPeca).g_vertex_buffer_data;
		case 3: return dynamic_cast<PecaJ&>(plPeca).g_vertex_buffer_data;
		case 4: return dynamic_cast<PecaS&>(plPeca).g_vertex_buffer_data;
		case 5: return dynamic_cast<PecaO&>(plPeca).g_vertex_buffer_data;
		case 6: return dynamic_cast<PecaL&>(plPeca).g_vertex_buffer_data;
		case 7: return dynamic_cast<PecaI&>(plPeca).g_vertex_buffer_data;
	}
}

std::vector<GLfloat> colorBufferPiece(Peca& plPeca) {
	switch (iRandPiece){
		case 1: return dynamic_cast<PecaZ&>(plPeca).g_color_buffer_data;
		case 2: return dynamic_cast<PecaT&>(plPeca).g_color_buffer_data;
		case 3: return dynamic_cast<PecaJ&>(plPeca).g_color_buffer_data;
		case 4: return dynamic_cast<PecaS&>(plPeca).g_color_buffer_data;
		case 5: return dynamic_cast<PecaO&>(plPeca).g_color_buffer_data;
		case 6: return dynamic_cast<PecaL&>(plPeca).g_color_buffer_data;
		case 7: return dynamic_cast<PecaI&>(plPeca).g_color_buffer_data;
	}
}


std::vector<GLfloat>* realVertexBufferPiece(Peca& plPeca) {
	switch (iRandPiece){
		case 1: return &dynamic_cast<PecaZ&>(plPeca).g_real_vertex_buffer;
		case 2: return &dynamic_cast<PecaT&>(plPeca).g_real_vertex_buffer;
		case 3: return &dynamic_cast<PecaJ&>(plPeca).g_real_vertex_buffer;
		case 4: return &dynamic_cast<PecaS&>(plPeca).g_real_vertex_buffer;
		case 5: return &dynamic_cast<PecaO&>(plPeca).g_real_vertex_buffer;
		case 6: return &dynamic_cast<PecaL&>(plPeca).g_real_vertex_buffer;
		case 7: return &dynamic_cast<PecaI&>(plPeca).g_real_vertex_buffer;
	}
}

//--------------------------------------------------------------------------------

void transferDataToGPUMemoryOfPiece(Peca& plPeca)
{

	// Recolha de informação de vertices e cor da peça
	g_vertex_buffer_data = vertexBufferPiece(plPeca);
	g_color_buffer_data = colorBufferPiece(plPeca);

	// Move vertex data to video memory; specifically to VBO called vertexbuffer,
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size() * sizeof(float), g_vertex_buffer_data.data(), GL_STATIC_DRAW);

	// Move color data to video memory; specifically to CBO called colorbuffer
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, g_color_buffer_data.size() * sizeof(float), g_color_buffer_data.data(), GL_STATIC_DRAW);

}

//--------------------------------------------------------------------------------
void cleanupDataFromGPU()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteBuffers(1, &vertexbufferTot);
	glDeleteBuffers(1, &colorbufferTot);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
}

//--------------------------------------------------------------------------------

int newSize;
// Caso haja uma linha completa, esta será eliminada na matriz de inteiros. Esta função garante que o observado no campo de jogo
// é concordante com o ocorrido na matriz de inteiros
void eliminaLinha(int iLinha) 
{
	int i;
	newSize = g_vertex_buffer_dataTot.size();
	for (i = 0; i < newSize; i += 18){
		// Limpar bloco a bloco
		if (g_vertex_buffer_dataTot.at(i+1) == iLinha){

			for (int k = i; k < g_vertex_buffer_dataTot.size() - 18; k++) {
				g_vertex_buffer_dataTot.at(k) = g_vertex_buffer_dataTot.at(k + 18);
				g_color_buffer_dataTot.at(k) = g_color_buffer_dataTot.at(k + 18);
			}

			// Atualização de variáveis
			newSize -= 18;
			i -= 18;
		}
		else {
			if (g_vertex_buffer_dataTot.at(i + 1) > iLinha) {
				for (int j = 1; j < 18; j += 3){
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
	for (int i = iLinha; i < iHeight - 1 ; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			gameGrid[j][i] = gameGrid[j][i+1];
		}
	}
}
//--------------------------------------------------------------------------------

// Verifica, na matriz de inteiros, linhas que apenas contenham 1's, ou seja, linhas que terão de ser eliminadas
std::vector <int> avaliaEliminacaoLinhas(int** gameGrid) {
	int j;
	std::vector <int> vetorLinhasAEliminar;
	for (int i = 0; i < iHeight; i++){
		for (j = 0; j < iWidth; j++){
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

bool evaluatePieceCollision(Peca& plPeca) {

	// Avaliar colisão
	if (plPeca.avaliaColisao()) {

		// Avalia se jogo terminou
		if (plPeca.atualizaMatriz()) {
			cout << "GAME OVER" << endl;
			getchar();
			exit(0);
		}

		// Atualiza vertexBuffer da peça, mediante o local onde esta colidiu
		plPeca.realVertexBuffer();
		// Receber grelha de jogo (matriz de inteiros) atualizada
		gameGrid = plPeca.getGameGrid();

		cout << "COLISAO" << endl;
		for (int i = iHeight - 1; i >= 0; i--) {
			for (int j = 0; j < iWidth; j++) {
				cout << gameGrid[j][i] << ", ";
			}
			cout << endl;
		}

		// Recolher vertexBuffer de peça colidida para armazenar no vertexBuffer de todas as outras peças anteriormente jogadas
		// O vertexBuffer recebido tem em conta o posicionamento da peça aquando da colisão
		std::vector<GLfloat>* vertexBuffer = realVertexBufferPiece(plPeca);

		// Anexar informação de nova peça colidida com peças anteriormente jogadas
		g_vertex_buffer_dataTot.insert(g_vertex_buffer_dataTot.end(), (*vertexBuffer).begin(), (*vertexBuffer).end());
		g_color_buffer_dataTot.insert(g_color_buffer_dataTot.end(), g_color_buffer_data.begin(), g_color_buffer_data.end());

		// Verificar se nova colisão de peça promove eliminação de linhas
		std::vector<int> vLinhasAEliminar = avaliaEliminacaoLinhas(gameGrid);
		if (vLinhasAEliminar.size() != 0) {
			// Começas a eliminar as linhas do topo e ir descendo, daí a inversão do vetor
			for (int i = vLinhasAEliminar.size()-1; i >= 0; i--){
				eliminaLinha(vLinhasAEliminar[i]);
				atualizaCampoJogo(vLinhasAEliminar[i]);
				g_vertex_buffer_dataTot.resize(newSize);
				g_color_buffer_dataTot.resize(newSize);
			}
		}

		// Carregar informação de todas as peças jogadas, com informação de nova peça colidida
		glGenBuffers(1, &colorbufferTot);
		glBindBuffer(GL_ARRAY_BUFFER, colorbufferTot);
		glBufferData(GL_ARRAY_BUFFER, g_color_buffer_dataTot.size() * sizeof(float), g_color_buffer_dataTot.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &vertexbufferTot);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferTot);
		glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_dataTot.size() * sizeof(float), g_vertex_buffer_dataTot.data(), GL_STATIC_DRAW);

		return true;
	}

	// Peça não colidiu com nenhuma outra ou base de campo de jogo
	return false;
}

//--------------------------------------------------------------------------------

bool drawObject(Peca& plPeca)
{
	// Use our shader
	glUseProgram(programID);

	// Variável auxiliar para que nos shaders sejam aplicada translações e rotações a peça atual
	unsigned int globalBool = glGetUniformLocation(programID, "bPreviousPieces");
	glUniform1i(globalBool, false);

	// Definição de matrizes
	glm::mat4 mvp = glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT);
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);

	// Aplicação de rotação à peça
	plPeca.rotacaoPeca(rot);
	// Aplicação de translação à peça
	plPeca.translacaoPeca(trans);

	// retrieve the matrix uniform locations
	unsigned int globalMVP = glGetUniformLocation(programID, "mvp");
	glUniformMatrix4fv(globalMVP, 1, GL_FALSE, &mvp[0][0]);

	unsigned int globalTrans = glGetUniformLocation(programID, "trans");
	glUniformMatrix4fv(globalTrans, 1, GL_FALSE, &trans[0][0]);
	
	unsigned int globalRot = glGetUniformLocation(programID, "rot");
	glUniformMatrix4fv(globalRot, 1, GL_FALSE, &rot[0][0]);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Desenha objeto
	plPeca.drawObject();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// Retorna informação para main se deve instanciar nova peça, ou seja, se presente peça colidiu
	return evaluatePieceCollision(plPeca);
}

//--------------------------------------------------------------------------------

void drawPreviousObjects(Peca& pPeca) {

	// Use our shader
	glUseProgram(programID);

	// Variável auxiliar para que nos shaders não sejam aplicada translações e rotações a peças anteriormente jogadas
	unsigned int globalBool = glGetUniformLocation(programID, "bPreviousPieces");
	glUniform1i(globalBool, true);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferTot);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbufferTot);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw all the previously played pieces
	glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer_dataTot.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

//--------------------------------------------------------------------------------

// Retains information of user inputs, regarding piece movement
void registerUserInputs(Peca& plPeca) {

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {
			plPeca.incNumberRotate();
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
			if (plPeca.getXPosD() < iWidth) {
				plPeca.incNumberTranslation();
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) {
			if (plPeca.getXPosE() > 0) {
				plPeca.decNumberTranslation();
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		plPeca.incNumberDown();
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
	//PecaT plPeca = PecaT(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid);
	geraPecas = GeradorPecas(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid);

	// Gera valor aletório, correspondente a peça de tabuleiro
	randNum();

	Peca* plPeca = returnPeca(geraPecas);

	// VAO
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders(vertexShader, fragmentShader);

	// transfer my data (vertices, colors, and shaders) to GPU side
	transferDataToGPUMemoryOfPiece(*plPeca);
	
	// render scene for each frame
	do {

		glClear(GL_COLOR_BUFFER_BIT);

		// Register user inputs regarding piece movement
		registerUserInputs(*plPeca);

		// Draw previously played pieces into the board
		drawPreviousObjects(*plPeca);

		// drawing callback
		if (drawObject(*plPeca)) {
			randNum();
			cout << " RANDOM PIECE: " << iRandPiece << endl;
			
			// Retorna a peça aleatoriamente criada
			plPeca = returnPeca(geraPecas);

			transferDataToGPUMemoryOfPiece(*plPeca);
			drawObject(*plPeca);
		}

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

