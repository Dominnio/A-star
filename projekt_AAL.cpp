//============================================================================
// Name        : projekt_AAL.cpp
// Author      : Dominik Orliñski
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#define _BSD_SOURCE

#include <sys/time.h>
#include <iostream>
#include <stdio.h>
#include "Graf.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <fstream>
using namespace std;

void generujTabele(int n, int skok, int lSkokow, int lPowtorzen, unsigned long* czasy);

int main(int argc, char* argv[]) {
	if(argc<3){
		std::cout<<"Za mala liczba argumentow.";
		return 0;
	}
	switch(atoi(argv[1])){
	case 1: // nazwaProgramu | trybWykonania | plikWejsciowy | plikWyjsciowy
	{
		std::cout << "\n=================\nProgram uruchomiony w trybie 1.\n=================\n";
		if(argc != 4){
			std::cout<<"Zla liczba argumentow.\n";
			return 0;
		}
		clock_t start = clock();
		Graf graf(argv[2]);
		//printf( "Czas utworzenia grafu: %lu ms\n", clock() - start);
		//std::cout.flush();
		//std::cout<<"\n\n=======================\n\n";
		start = clock();
		graf.znajdzNajkrotszaSciezkeAlgorytmemA(argv[3]);
		printf( "\nCzas wyszukiwania rozwiazania A*: %lu ms\n\n", clock() - start );
		break;
	}
	case 2: // nazwaProgramu | trybWykonania | wielkoscProblemu(N) | plikWyjsciowy
	{
		std::cout << "\n=================\nProgram uruchomiony w trybie 2.\n=================\n";
		if(argc != 4){
			std::cout<<"Zla liczba argumentow.\n";
			return 0;
		}
		clock_t start = clock();
		Graf graf(atoi(argv[2]));
		//printf( "Czas utworzenia grafu: %lu ms\n", clock() - start);
		//std::cout.flush();
		//std::cout<<"\n\n=======================\n\n";
		start = clock();
		graf.znajdzNajkrotszaSciezkeAlgorytmemA(argv[3]);
		printf( "\nCzas wyszukiwania rozwiazania A*: %lu ms\n\n", clock() - start );
		break;
	}
	case 3: // nazwaProgramu | trybWykonania | pierwszeN | coIleNSkok | ileSkokow | ilePowtorzenDlaKazdejInstancji || plikWyjsciowy
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		srand(tv.tv_usec);
		std::cout << "\n=================\nProgram uruchomiony w trybie 3.\n=================\n";
		if(argc != 7){
			std::cout<<"Zla liczba argumentow.\n";
			return 0;
		}
		clock_t start;
		unsigned long* czasy = new unsigned long[atoi(argv[4]) * atoi(argv[5])];
		for(int i=0;i<atoi(argv[4]);++i){
			for(int j=0;j<atoi(argv[5]);++j){
				start = clock();
				Graf graf(atoi(argv[2]) + i*atoi(argv[3]));
				//printf( "Czas utworzenia grafu: %lu ms\n", clock() - start);
				//std::cout.flush();
				//std::cout<<"\n\n=======================\n\n";
				start = clock();
				graf.znajdzNajkrotszaSciezkeAlgorytmemA(argv[6]);
				czasy[i*atoi(argv[5]) + j] = clock() - start;
				//printf( "\nCzas wyszukiwania rozwiazania A*: %lu ms\n\n", clock() - start );
			}
			std::cout << "Wykonano dla n rownego "<<atoi(argv[2]) + i*atoi(argv[3])<<"."<<std::endl;
		}
		const char* nazwaPliku; //= new char[strlen("plikWyjsciowy.txt")];
		nazwaPliku = "plikWyjsciowy.txt";
		generujTabele(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), czasy);
		break;
	}
	}
		/*
	std::cout<<"\n\n=======================\n\n";
	std::cout.flush();
	start = clock();
	graf.znajdzNajkrtoszaSciezkeAlgorytmemDijkstry();
	printf( "\nCzas wyszukiwania rozwiazania Dijkstra: %lu ms\n", clock() - start );
	*/
	return 0;
}

void generujTabele(int n, int skok, int lSkokow, int lPowtorzen, unsigned long* czasy){
	FILE* plikWyjsciowy;
	plikWyjsciowy = fopen("Tabela.txt", "a+");
	char a;
	float b;
	unsigned long sredniCzas = 0;
	unsigned long czasMediana = 0;
	float medianaZlozonosc = 0;
	float T;
	float E = 0;
	float V = 0;
	float tmp;
	float q;
	fprintf (plikWyjsciowy, "n\tt(n)[ms]\tq(n)\n");
	int nMediana = 0;
	if(lSkokow%2 != 0){
		nMediana = n + skok*(lSkokow-1)/2;
	}else{
		if(rand()%2 == 0){
			nMediana = n + skok*(lSkokow)/2;
		}else{
			nMediana = n + skok*(lSkokow)/2 - skok;
		}
	}
	for(int i = 0;i<lPowtorzen;++i){
		czasMediana += czasy[((nMediana-n)/skok)*lPowtorzen + i];
	}
	czasMediana /= lPowtorzen;
	V = nMediana * exp2(nMediana - 1) + 1;
	E = (V * (nMediana - 1) /2) + nMediana;
	medianaZlozonosc = (V+E)* log(V);
	for(int j =0;j<lSkokow;j++){
		T = 0;
		E = 0;
		V = 0;
		q = 0;
		sredniCzas = 0;
		for(int i = 0;i<lPowtorzen;++i){
			sredniCzas += czasy[j*lPowtorzen+i];
		}
		sredniCzas /= lPowtorzen;
		V = (n+j*skok) * exp2((n+j*skok) - 1) + 1;
		E = (V * ((n+j*skok) - 1) /2) + (n+j*skok);
		T =  (V+E)* log(V);
		q = sredniCzas * medianaZlozonosc / T / czasMediana;
		fprintf (plikWyjsciowy, "%d\t%lu\t\t%f\n",n+j*skok,sredniCzas,q);
	}
}
