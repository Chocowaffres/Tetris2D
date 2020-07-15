/////////////////////////////////////////////
// Developed by Tiago Roxo and Joana Costa //
/////////////////////////////////////////////

#include "headers/GeradorPecas.hpp"

#include <memory>

GeradorPecas::GeradorPecas() {};

GeradorPecas::GeradorPecas(int xPosInicial, int yPosInicial, int iHeight, int iWidth, int** gameGrid, int iGameLevel) :
	pecaZ(xPosInicial,yPosInicial,iHeight, iWidth, gameGrid, iGameLevel),
	pecaT(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iGameLevel),
	pecaJ(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iGameLevel),
	pecaS(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iGameLevel),
	pecaO(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iGameLevel),
	pecaL(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iGameLevel),
	pecaI(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iGameLevel) {

	// Variáveis de tabuleiro
	this->xPosInicial = xPosInicial;
	this->yPosInicial = yPosInicial;
	this->iHeight = iHeight;
	this->iWidth = iWidth;
	this->iGameLevel = iGameLevel;

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
void GeradorPecas::criaPecaZ(int** gameGrid, int iGameLevel) {
	pecaZ = PecaZ(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iGameLevel);
}

void GeradorPecas::criaPecaT(int** gameGrid, int iGameLevel) {
	pecaT = PecaT(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iGameLevel);
}

void GeradorPecas::criaPecaJ(int** gameGrid, int iGameLevel) {
	pecaJ = PecaJ(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iGameLevel);
}

void GeradorPecas::criaPecaS(int** gameGrid, int iGameLevel) {
	pecaS = PecaS(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iGameLevel);
}

void GeradorPecas::criaPecaO(int** gameGrid, int iGameLevel) {
	pecaO = PecaO(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iGameLevel);
}

void GeradorPecas::criaPecaL(int** gameGrid, int iGameLevel) {
	pecaL = PecaL(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iGameLevel);
}

void GeradorPecas::criaPecaI(int** gameGrid, int iGameLevel) {
	pecaI = PecaI(xPosInicial, yPosInicial, iHeight, iWidth, gameGrid, iGameLevel);
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
