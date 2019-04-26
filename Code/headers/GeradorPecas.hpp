#pragma once
#include "headers/PecaL.hpp"
#include "headers/PecaJ.hpp"
#include "headers/PecaS.hpp"
#include "headers/PecaI.hpp"
#include "headers/PecaZ.hpp"
#include "headers/PecaO.hpp"
#include "headers/PecaT.hpp"
#include "headers/Peca.hpp"


class GeradorPecas {

	private:
		// Vari�veis de gerador
		PecaZ pecaZ;
		PecaT pecaT;
		PecaJ pecaJ;
		PecaS pecaS;
		PecaO pecaO;
		PecaL pecaL;
		PecaI pecaI;


		int xPosInicial;
		int yPosInicial;
		int iHeight;
		int iWidth;
		int** gameGrid;

		int iRandPiece;
	public:
		GeradorPecas();
		GeradorPecas(int,int,int,int,int**);

		// Criar pecas
		void criaPecaZ(int**);
		void criaPecaT(int**);
		void criaPecaJ(int**);
		void criaPecaS(int**);
		void criaPecaO(int**);
		void criaPecaL(int**);
		void criaPecaI(int**);

		// Setter
		void setGameGrid(int**);

		// Getters
		Peca& getPecaZ();
		Peca& getPecaT();
		Peca& getPecaJ();
		Peca& getPecaS();
		Peca& getPecaO();
		Peca& getPecaL();
		Peca& getPecaI();
};