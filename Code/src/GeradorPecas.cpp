#include "headers/GeradorPecas.hpp"

#include <memory>

GeradorPecas::GeradorPecas() {};

GeradorPecas::GeradorPecas(int xPosInicial, int yPosInicial, int iHeight, int iWidth, int** gameGrid) :
	pecaZ(xPosInicial,yPosInicial,iHeight, iWidth, gameGrid), 
	pecaT(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid),
	pecaJ(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid),
	pecaS(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid),
	pecaO(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid),
	pecaL(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid),
	pecaI(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid) {

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
}

// Criar pecas
void GeradorPecas::criaPecaZ(int** gameGrid) {
	pecaZ = PecaZ(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid);
}

void GeradorPecas::criaPecaT(int** gameGrid) {
	pecaT = PecaT(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid);
}

void GeradorPecas::criaPecaJ(int** gameGrid) {
	pecaJ = PecaJ(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid);
}

void GeradorPecas::criaPecaS(int** gameGrid) {
	pecaS = PecaS(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid);
}

void GeradorPecas::criaPecaO(int** gameGrid) {
	pecaO = PecaO(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid);
}

void GeradorPecas::criaPecaL(int** gameGrid) {
	pecaL = PecaL(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid);
}

void GeradorPecas::criaPecaI(int** gameGrid) {
	pecaI = PecaI(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid);
}


// Setter
void GeradorPecas::setGameGrid(int** gameGrid) {

	for (int i = 0; i < iWidth; i++) {
		for (int j = 0; j < iHeight; j++) {
			this->gameGrid[i][j] = gameGrid[i][j];
		}
	}
}

// Getters
Peca& GeradorPecas::getPecaZ() {
	return pecaZ;
}

Peca& GeradorPecas::getPecaT() {
	return pecaT;
}

Peca& GeradorPecas::getPecaJ() {
	return pecaJ;
}

Peca& GeradorPecas::getPecaS() {
	return pecaS;
}

Peca& GeradorPecas::getPecaO() {
	return pecaO;
}

Peca& GeradorPecas::getPecaL() {
	return pecaL;
}

Peca& GeradorPecas::getPecaI() {
	return pecaI;
}