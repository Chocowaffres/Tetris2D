/////////////////////////////////////////////
// Developed by Tiago Roxo and Joana Costa //
/////////////////////////////////////////////

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
		// Variáveis de gerador
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
		int iGameLevel;

		int iGamePiece;
	public:
		GeradorPecas();
		GeradorPecas(int,int,int,int,int**, int);

		// Criar pecas
		void criaPecaZ(int**, int);
		void criaPecaT(int**, int);
		void criaPecaJ(int**, int);
		void criaPecaS(int**, int);
		void criaPecaO(int**, int);
		void criaPecaL(int**, int);
		void criaPecaI(int**, int);

		// Getters
		Peca& getPecaZ();
		Peca& getPecaT();
		Peca& getPecaJ();
		Peca& getPecaS();
		Peca& getPecaO();
		Peca& getPecaL();
		Peca& getPecaI();
};
