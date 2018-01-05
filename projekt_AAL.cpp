//============================================================================
// Nazwa		: projekt_AAL.cpp
// Autor		: Dominik Orliñski
// Wersja		: 1.0
// Nazwa		: Szeregowanie zadañ
//============================================================================

#define _BSD_SOURCE

#include <sys/time.h>
#include <iostream>
#include <stdio.h>
#include "Graf.h"
#include "GrafX.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <fstream>
using namespace std;

void generujTabele(int n, int skok, int lSkokow, int lPowtorzen, unsigned long* czasy, int* liczniki);

int main(int argc, char* argv[]) {
	if(argc<3){
		std::cout<<"Za mala liczba argumentow.";
		return 0;
	}
	switch(atoi(argv[1])){
	case 0:
	{
		std::cout << "\n=================\nProgram uruchomiony w trybie 0.\n=================\n";
		GrafX graf(atoi(argv[2]));
		graf.wypiszGrafX();
		graf.znajdzNajkrotszaSciezkeAlgorytmemA(argv[3]);
		break;
	}

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
		int licznik = 0;
		unsigned long* czasy = new unsigned long[atoi(argv[4]) * atoi(argv[5])];
		int* liczniki = new int[atoi(argv[4])];
		for(int i=0;i<atoi(argv[4]);++i){
			for(int j=0;j<atoi(argv[5]);++j){
				Graf graf(atoi(argv[2]) + i*atoi(argv[3]));
				//printf( "Czas utworzenia grafu: %lu ms\n", clock() - start);
				//std::cout.flush();
				//std::cout<<"\n\n=======================\n\n";
				start = clock();
				licznik += graf.znajdzNajkrotszaSciezkeAlgorytmemA(argv[6]);
				czasy[i*atoi(argv[5]) + j] = clock() - start;
				//graf.~Graf();
				//printf( "\nCzas wyszukiwania rozwiazania A*: %lu ms\n\n", clock() - start );
			}
			std::cout << "Wykonano dla n rownego "<<atoi(argv[2]) + i*atoi(argv[3])<<"."<<std::endl;
			licznik /= atoi(argv[5]);
			std::cout << licznik << std::endl;
			liczniki[i] = licznik;
			licznik = 0;
		}
		generujTabele(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), czasy, liczniki);
		delete[] liczniki;
		delete [] czasy;
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

void generujTabele(int n, int skok, int lSkokow, int lPowtorzen, unsigned long* czasy, int* liczniki) {
	FILE* plikWyjsciowy;
	plikWyjsciowy = fopen("Tabela.txt", "a+");
	float sredniCzas = 0;
	float czasMediana = 0;
	float medianaZlozonosc = 0;
	float T;
	float E = 0;
	float V = 0;
	float q;
	fprintf (plikWyjsciowy, "n\tt(n)[ms]\tq(n)\n");
	int nMediana = 0;
	if(lSkokow%2 != 0){
		nMediana = n + skok*(lSkokow-1)/2;
	}else{
		if(rand()%2 == 0){
			nMediana = n + skok*(lSkokow)/2;
		}//else{
			//nMediana = n + skok*(lSkokow)/2 - skok;
		//}
	}
	for(int i = 0;i<lPowtorzen;++i){
		czasMediana += czasy[((nMediana-n)/skok)*lPowtorzen + i];
	}
	czasMediana /= lPowtorzen;
	V = nMediana * exp2(nMediana - 1) + 1;
	float tmp = nMediana;
	E += tmp;
	for(int i=nMediana-1,k=1;i>0;--i,k++){
		tmp *=i;
		E += tmp;
		tmp /=k;
	}
	medianaZlozonosc = 30 + 14*V + liczniki[((nMediana - n) / skok)] * (6 + 4 * V + n);
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
		tmp = (n+j*skok);
		E += tmp;
		for(int i=(n+j*skok)-1,k=1;i>0;--i,k++){
			tmp *=i;
			E += tmp;
			tmp /=k;
		}
		T = 30 + 14 * V + liczniki[j]*(6+4*V+n);
		std::cout <<"V: " << V<<"\tE: "<<E<<"\tT_m / T: "<<medianaZlozonosc/T<<"\tt_m / t: "<<sredniCzas/czasMediana<<std::endl;
		q = sredniCzas * medianaZlozonosc / T / czasMediana;
		fprintf (plikWyjsciowy, "%d\t%.2f\t\t%f\n",n+j*skok,sredniCzas,q);
	}
}
