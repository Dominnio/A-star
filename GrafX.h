/*
* GrafX.h
//============================================================================
// Nazwa		: projekt_AAL.cpp
// Autor		: Dominik Orliñski
// Wersja		: 1.0
// Nazwa		: Szeregowanie zadañ
//============================================================================
*/
#include<iostream>

#ifndef GRAFX_H_
#define GRAFX_H_
class KomorkaX {
public:
	//KomorkaX();
	float koszt;
	float heurystyka;
	short numerZadania;
	short kolejnoscwykonania;
	short liczbaZadan;
	bool czyRozpatrzone;
	int** sasiedzi;
	KomorkaX* poprzednik;

	KomorkaX& operator= (int a)
	{
		koszt = 0;
		kolejnoscwykonania = 0;
		heurystyka = 0;
		liczbaZadan = 0;
		numerZadania = 0;
		czyRozpatrzone = false;
		return *this;
	}

	KomorkaX& operator= (KomorkaX const& ex)
	{
		koszt = ex.koszt;
		kolejnoscwykonania = ex.kolejnoscwykonania;
		heurystyka = ex.heurystyka;
		liczbaZadan = ex.liczbaZadan;
		numerZadania = ex.numerZadania;
		czyRozpatrzone = ex.czyRozpatrzone;
		//	poprzednik = new Komorka();
		poprzednik = *(&ex.poprzednik);
		sasiedzi = *&(ex.sasiedzi);//new int*[liczbaZadan - ex.kolejnoscwykonania ];
								   //	for(int i=0;i<liczbaZadan - ex.kolejnoscwykonania;i++){
								   //		sasiedzi[i] = new int[2];
								   //	}
								   //	for(int i=0;i<liczbaZadan - ex.kolejnoscwykonania;i++){
								   //		sasiedzi[i][0] = ex.sasiedzi[i][0];
								   //	sasiedzi[i][1] = ex.sasiedzi[i][1];
								   //	}
		return *this;
	}
};


class GrafX {
public:
	GrafX(int liczbaZadan);
	//GrafX(char* nazwaPliku);
	//void znajdzNajkrtoszaSciezkeAlgorytmemDijkstry();
	int znajdzNajkrotszaSciezkeAlgorytmemA(char* nazwaPliku);
	void wypiszGrafX();
	virtual ~GrafX();
private:
	KomorkaX** tabelaStanow;
	int liczbaZadan;

};

#endif /* GRAF_H_ */
