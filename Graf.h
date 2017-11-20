/*
 * Graf.h
 *
 *  Created on: 28 paü 2017
 *      Author: Dominik
 */
#include<iostream>

#ifndef GRAF_H_
#define GRAF_H_

class Komorka{
public:
	//Komorka();
	float koszt;
	float heurystyka;
	int numerZadania;
	int kolejnoscwykonania;
	int liczbaZadan;
	int** sasiedzi;
	bool* argumenty;
	long long int pomocniczyIndeks;
	Komorka* poprzednik;

	Komorka& operator= (int a)
		{
			koszt = -1;
			kolejnoscwykonania = -1;
			heurystyka = -1;
			liczbaZadan = -1;
			numerZadania = -1;
			pomocniczyIndeks = -1;
			return *this;
		}

	Komorka& operator= (Komorka const& ex)
	{
		koszt = ex.koszt;
		kolejnoscwykonania = ex.kolejnoscwykonania;
		heurystyka = ex.heurystyka;
		liczbaZadan = ex.liczbaZadan;
		numerZadania = ex.numerZadania;
		pomocniczyIndeks = ex.pomocniczyIndeks;
		poprzednik = new Komorka();
		poprzednik = *(&ex.poprzednik);
		argumenty = new bool[liczbaZadan];
		for(int i=0;i<liczbaZadan;i++){
			argumenty[i] = ex.argumenty[i];
		}
		sasiedzi = new int*[liczbaZadan - ex.kolejnoscwykonania ];
		for(int i=0;i<liczbaZadan - ex.kolejnoscwykonania;i++){
			sasiedzi[i] = new int[2];
		}
		for(int i=0;i<liczbaZadan - ex.kolejnoscwykonania;i++){
			sasiedzi[i][0] = ex.sasiedzi[i][0];
			sasiedzi[i][1] = ex.sasiedzi[i][1];
		}
		return *this;
	}
};


class Graf {
public:
	Graf(int liczbaZadan);
	Graf(char* nazwaPliku);
	void znajdzNajkrtoszaSciezkeAlgorytmemDijkstry();
	void znajdzNajkrotszaSciezkeAlgorytmemA(char* nazwaPliku);
	void wypiszGraf();
	virtual ~Graf();
private:
	Komorka** tabelaStanow;
	int liczbaZadan;

};

#endif /* GRAF_H_ */
