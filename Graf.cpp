/*
 * Graf.cpp
 *
 *  Created on: 28 paü 2017
 *      Author: Dominik
 */

#include "Graf.h"
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <iostream>

Graf::Graf(int liczbaZadan) {
	if(liczbaZadan<=0){
		std::cout << "Liczba zadan musi byc wieksza od 0!\n";
		return;
	}
	this->liczbaZadan = liczbaZadan;
	int k = liczbaZadan;
	bool flagaMinalesJedynke = false;
	int pozycjaOstatniegoMinetegoZera = 0;
	int pozycjaPierwszejPrawidlowejJedynki = 0;
	long long int w = exp2(liczbaZadan - 1);
	this->tabelaStanow = new Komorka *[w];
	for (int i = 0; i < w; ++i) {
		tabelaStanow[i] = new Komorka[k];
		for (int j = 0; j < k; ++j) {
			tabelaStanow[i][j].numerZadania = j + 1;
			tabelaStanow[i][j].argumenty = new bool[liczbaZadan];
			tabelaStanow[i][j].kolejnoscwykonania = 0;
			tabelaStanow[i][j].liczbaZadan = liczbaZadan;
			tabelaStanow[i][j].poprzednik = new Komorka;
			tabelaStanow[i][j].poprzednik = &tabelaStanow[i][j];
			flagaMinalesJedynke = false;
			if(i == 0){
				for(int h=0;h<liczbaZadan;++h){
					tabelaStanow[i][j].argumenty[h] = false;
				}
				tabelaStanow[i][j].argumenty[j] = true;
				++tabelaStanow[i][j].kolejnoscwykonania;
			}else{
				for(int h=0;h<liczbaZadan;++h){
					tabelaStanow[i][j].argumenty[h] = tabelaStanow[i-1][j].argumenty[h];
					tabelaStanow[i][j].kolejnoscwykonania = tabelaStanow[i-1][j].kolejnoscwykonania;
				}
				for(int h=0;h<liczbaZadan;++h){
					if(tabelaStanow[i][j].argumenty[h] == false){
						flagaMinalesJedynke = false;
						pozycjaOstatniegoMinetegoZera = h;
					}
					if(tabelaStanow[i][j].argumenty[h] == true && tabelaStanow[i][j].numerZadania != h+1){
						if(flagaMinalesJedynke == false){
							flagaMinalesJedynke = true;
							pozycjaPierwszejPrawidlowejJedynki = h;
						}
						if(h+1 == liczbaZadan){
							for(int a=liczbaZadan-1;a >= pozycjaPierwszejPrawidlowejJedynki;a--){
								if(a + 1 != tabelaStanow[i][j].numerZadania){
									tabelaStanow[i][j].argumenty[a] = false;
									tabelaStanow[i][j].kolejnoscwykonania--;
								}
								if(a + 1 == tabelaStanow[i][j].numerZadania && tabelaStanow[i][j].numerZadania == liczbaZadan){
									tabelaStanow[i][j].argumenty[a] = false;
									tabelaStanow[i][j].kolejnoscwykonania--;
								}
							}
							tabelaStanow[i][j].argumenty[pozycjaOstatniegoMinetegoZera] = true;
							tabelaStanow[i][j].kolejnoscwykonania++;
							break;
						}
						if(h+2 == liczbaZadan && tabelaStanow[i][j].numerZadania == liczbaZadan){
							for(int a=liczbaZadan-2;a >= pozycjaPierwszejPrawidlowejJedynki;a--){
								if(a + 1 != tabelaStanow[i][j].numerZadania){
									tabelaStanow[i][j].argumenty[a] = false;
									tabelaStanow[i][j].kolejnoscwykonania--;
								}
								if(a + 1 == tabelaStanow[i][j].numerZadania && tabelaStanow[i][j].numerZadania == liczbaZadan){
									tabelaStanow[i][j].argumenty[a] = false;
									tabelaStanow[i][j].kolejnoscwykonania--;
								}
							}
							tabelaStanow[i][j].argumenty[pozycjaOstatniegoMinetegoZera] = true;
							tabelaStanow[i][j].kolejnoscwykonania++;
							break;
						}

					}
					if(h+1 == liczbaZadan ){
						tabelaStanow[i][j].argumenty[pozycjaOstatniegoMinetegoZera] = true;
						tabelaStanow[i][j].kolejnoscwykonania++;
						tabelaStanow[i][j].pomocniczyIndeks = 0;
						break;
					}
				}

			}
			int suma = 0;
			float tmp = (liczbaZadan*(tabelaStanow[i][j].kolejnoscwykonania) + (rand()%(liczbaZadan+1)));
			tabelaStanow[i][j].koszt = pow(liczbaZadan,3)/(tmp);

			tabelaStanow[i][j].sasiedzi = new int*[liczbaZadan-tabelaStanow[i][j].kolejnoscwykonania];
			for (int a = 0; a <liczbaZadan-tabelaStanow[i][j].kolejnoscwykonania; ++a) {
				tabelaStanow[i][j].sasiedzi[a] = new int[2];
			}
			for(int a=0,d=0;a<liczbaZadan;++a){
				if(a != j && tabelaStanow[i][j].argumenty[a] == false){
					for(int b=0, c=0;b<liczbaZadan;++b){
						if(tabelaStanow[i][j].argumenty[b] == true && b!=a){
							suma = suma + exp2(liczbaZadan - 2 - c);
						}
						if(b!=a){
							++c;
						}
					}
					tabelaStanow[i][j].sasiedzi[d][0] = suma;
					tabelaStanow[i][j].sasiedzi[d][1] = a;
					suma = 0;
					++d;
				}
			}
		}
	}
}

Graf::Graf(char* nazwaPliku) {
	std::fstream plikWejsciowy;
	plikWejsciowy.open( nazwaPliku, std::ios::in | std::ios::out );
	if( plikWejsciowy.good() == true )
	{
	    //std::cout << "Uzyskano dostep do pliku!" << std::endl;
	} else {
		std::cout << "Dostep do pliku zostal zabroniony!" << std::endl;
		return;
	}
	int liczbaZadan = 0;
	plikWejsciowy >> liczbaZadan;
	if(liczbaZadan <= 0){
		std::cout << "Liczba zadan musi byc wieksza od 0!\n";
		return;
	}
	this->liczbaZadan = liczbaZadan;
	srand(time(NULL));
	int k = liczbaZadan;
	bool flagaMinalesJedynke = false;
	int pozycjaOstatniegoMinetegoZera = 0;
	int pozycjaPierwszejPrawidlowejJedynki = 0;
	long long int w = exp2(liczbaZadan - 1);
	this->tabelaStanow = new Komorka *[w];
	for (int i = 0; i < w; ++i) {
		tabelaStanow[i] = new Komorka[k];
		for (int j = 0; j < k; ++j) {
			tabelaStanow[i][j].numerZadania = j + 1;
			tabelaStanow[i][j].argumenty = new bool[liczbaZadan];
			tabelaStanow[i][j].kolejnoscwykonania = 0;
			tabelaStanow[i][j].liczbaZadan = liczbaZadan;
			tabelaStanow[i][j].poprzednik = new Komorka;
			tabelaStanow[i][j].poprzednik = &tabelaStanow[i][j];
			flagaMinalesJedynke = false;
			if(i == 0){
				for(int h=0;h<liczbaZadan;++h){
					tabelaStanow[i][j].argumenty[h] = false;
				}
				tabelaStanow[i][j].argumenty[j] = true;
				++tabelaStanow[i][j].kolejnoscwykonania;
			}else{
				for(int h=0;h<liczbaZadan;++h){
					tabelaStanow[i][j].argumenty[h] = tabelaStanow[i-1][j].argumenty[h];
					tabelaStanow[i][j].kolejnoscwykonania = tabelaStanow[i-1][j].kolejnoscwykonania;
				}
				for(int h=0;h<liczbaZadan;++h){
					if(tabelaStanow[i][j].argumenty[h] == false){
						flagaMinalesJedynke = false;
						pozycjaOstatniegoMinetegoZera = h;
					}
					if(tabelaStanow[i][j].argumenty[h] == true && tabelaStanow[i][j].numerZadania != h+1){
						if(flagaMinalesJedynke == false){
							flagaMinalesJedynke = true;
							pozycjaPierwszejPrawidlowejJedynki = h;
						}
						if(h+1 == liczbaZadan){
							for(int a=liczbaZadan-1;a >= pozycjaPierwszejPrawidlowejJedynki;a--){
								if(a + 1 != tabelaStanow[i][j].numerZadania){
									tabelaStanow[i][j].argumenty[a] = false;
									tabelaStanow[i][j].kolejnoscwykonania--;
								}
								if(a + 1 == tabelaStanow[i][j].numerZadania && tabelaStanow[i][j].numerZadania == liczbaZadan){
									tabelaStanow[i][j].argumenty[a] = false;
									tabelaStanow[i][j].kolejnoscwykonania--;
								}
							}
							tabelaStanow[i][j].argumenty[pozycjaOstatniegoMinetegoZera] = true;
							tabelaStanow[i][j].kolejnoscwykonania++;
							break;
						}
						if(h+2 == liczbaZadan && tabelaStanow[i][j].numerZadania == liczbaZadan){
							for(int a=liczbaZadan-2;a >= pozycjaPierwszejPrawidlowejJedynki;a--){
								if(a + 1 != tabelaStanow[i][j].numerZadania){
									tabelaStanow[i][j].argumenty[a] = false;
									tabelaStanow[i][j].kolejnoscwykonania--;
								}
								if(a + 1 == tabelaStanow[i][j].numerZadania && tabelaStanow[i][j].numerZadania == liczbaZadan){
									tabelaStanow[i][j].argumenty[a] = false;
									tabelaStanow[i][j].kolejnoscwykonania--;
								}
							}
							tabelaStanow[i][j].argumenty[pozycjaOstatniegoMinetegoZera] = true;
							tabelaStanow[i][j].kolejnoscwykonania++;
							break;
						}

					}
					if(h+1 == liczbaZadan ){
						tabelaStanow[i][j].argumenty[pozycjaOstatniegoMinetegoZera] = true;
						tabelaStanow[i][j].kolejnoscwykonania++;
						tabelaStanow[i][j].pomocniczyIndeks = 0;
						break;
					}
				}

			}
			int suma = 0;
			//float tmp = (liczbaZadan*(tabelaStanow[i][j].kolejnoscwykonania) + (rand()%(liczbaZadan+1)));
			plikWejsciowy >> tabelaStanow[i][j].koszt;

			tabelaStanow[i][j].sasiedzi = new int*[liczbaZadan-tabelaStanow[i][j].kolejnoscwykonania];
			for (int a = 0; a <liczbaZadan-tabelaStanow[i][j].kolejnoscwykonania; ++a) {
				tabelaStanow[i][j].sasiedzi[a] = new int[2];
			}
			for(int a=0,d=0;a<liczbaZadan;++a){
				if(a != j && tabelaStanow[i][j].argumenty[a] == false){
					for(int b=0, c=0;b<liczbaZadan;++b){
						if(tabelaStanow[i][j].argumenty[b] == true && b!=a){
							suma = suma + exp2(liczbaZadan - 2 - c);
						}
						if(b!=a){
							++c;
						}
					}
					tabelaStanow[i][j].sasiedzi[d][0] = suma;
					tabelaStanow[i][j].sasiedzi[d][1] = a;
					suma = 0;
					++d;
				}
			}
		}
	}
}


void Graf::znajdzNajkrotszaSciezkeAlgorytmemA(char* nazwaPliku){
	float* tabelaMinimalnychCzasow = new float[liczbaZadan];
	float sumaHeurystyczna = 0;
	for(int i=0;i<liczbaZadan;i++){
		tabelaMinimalnychCzasow[i] = -1;
	}
	int w = exp2(liczbaZadan - 1);
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < liczbaZadan; ++j) {
			if(tabelaMinimalnychCzasow[tabelaStanow[i][j].kolejnoscwykonania-1] == -1 || tabelaStanow[i][j].koszt < tabelaMinimalnychCzasow[tabelaStanow[i][j].kolejnoscwykonania-1]){
				tabelaMinimalnychCzasow[tabelaStanow[i][j].kolejnoscwykonania-1] = tabelaStanow[i][j].koszt;
			}
		}
	}
	for(int i=0;i<liczbaZadan;++i){
		sumaHeurystyczna = sumaHeurystyczna + tabelaMinimalnychCzasow[i];
	}
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < liczbaZadan; ++j) {
			tabelaStanow[i][j].heurystyka = sumaHeurystyczna;
			for(int k=0;k<tabelaStanow[i][j].kolejnoscwykonania;k++){
				tabelaStanow[i][j].heurystyka -= tabelaMinimalnychCzasow[k];
			}
		}
	}
	Komorka biezaceZadanie;
	biezaceZadanie.kolejnoscwykonania = 0;
	biezaceZadanie.numerZadania = 0;
	biezaceZadanie.liczbaZadan = this->liczbaZadan;
	biezaceZadanie.koszt = 0;
	biezaceZadanie.heurystyka = sumaHeurystyczna;
	biezaceZadanie.pomocniczyIndeks = 0;
	biezaceZadanie.sasiedzi = new int*[liczbaZadan];
	for (int a = 0; a <liczbaZadan; ++a) {
		biezaceZadanie.sasiedzi[a] = new int[2];
	}
	biezaceZadanie.argumenty = new bool[liczbaZadan];
	for(int a=0;a<liczbaZadan;++a){
		biezaceZadanie.sasiedzi[a][0] = 0;
		biezaceZadanie.sasiedzi[a][1] = a;
	}
	int a = this->liczbaZadan * exp2(this->liczbaZadan - 1);

	Komorka* zbiorKoncowek = new Komorka[a+1];
	int liczbaKoncowek = 1;
	Komorka* zbiorOcenionych = new Komorka[a+1];
	zbiorKoncowek[0] = biezaceZadanie;
	zbiorOcenionych[0] = 0;
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < liczbaZadan; ++j) {
			zbiorKoncowek[i*liczbaZadan + j + 1] = -1;
			zbiorOcenionych[i*liczbaZadan + j + 1] = -1;
		}
	}
	float minimalnaHeurystyka = -1;
	int indeksMinimum;
	int indeksSasiada = 0;
	while(liczbaKoncowek != 0){
		minimalnaHeurystyka = -1;
		for(int i=0;i<a+1;i++){
			if(zbiorKoncowek[i].heurystyka != -1){
				if((zbiorKoncowek[i].heurystyka + zbiorKoncowek[i].koszt < minimalnaHeurystyka) || minimalnaHeurystyka == -1){
					minimalnaHeurystyka = zbiorKoncowek[i].heurystyka + zbiorKoncowek[i].koszt;
					indeksMinimum = i;
				}
			}
		}
		if(zbiorKoncowek[indeksMinimum].kolejnoscwykonania == liczbaZadan){
			zbiorOcenionych[indeksMinimum] = zbiorKoncowek[indeksMinimum];
			break;
		}
		zbiorOcenionych[indeksMinimum] = zbiorKoncowek[indeksMinimum];
		zbiorKoncowek[indeksMinimum] = 0;
		liczbaKoncowek--;
		for(int i=0;i<liczbaZadan - zbiorOcenionych[indeksMinimum].kolejnoscwykonania;i++){
			indeksSasiada = zbiorOcenionych[indeksMinimum].sasiedzi[i][0]*liczbaZadan + zbiorOcenionych[indeksMinimum].sasiedzi[i][1] +1;
			if(zbiorOcenionych[indeksSasiada].liczbaZadan != -1){
				continue;
			}
			if(zbiorKoncowek[indeksSasiada].liczbaZadan == -1){
				zbiorKoncowek[indeksSasiada] = tabelaStanow[zbiorOcenionych[indeksMinimum].sasiedzi[i][0]][zbiorOcenionych[indeksMinimum].sasiedzi[i][1]];
				zbiorKoncowek[indeksSasiada].koszt += zbiorOcenionych[indeksMinimum].koszt;
				zbiorKoncowek[indeksSasiada].poprzednik = &zbiorOcenionych[indeksMinimum];
				liczbaKoncowek++;
			}else{
				if(zbiorKoncowek[indeksSasiada].koszt > zbiorOcenionych[indeksMinimum].koszt + tabelaStanow[zbiorOcenionych[indeksMinimum].sasiedzi[i][0]][zbiorOcenionych[indeksMinimum].sasiedzi[i][1]].koszt){
					zbiorKoncowek[indeksSasiada].koszt = zbiorOcenionych[indeksMinimum].koszt + tabelaStanow[zbiorOcenionych[indeksMinimum].sasiedzi[i][0]][zbiorOcenionych[indeksMinimum].sasiedzi[i][1]].koszt;
					zbiorKoncowek[indeksSasiada].poprzednik = &zbiorOcenionych[indeksMinimum];
				}
			}
		}
	}
	std::fstream plikWyjsciowy;
	plikWyjsciowy.open( nazwaPliku, std::ios::in | std::ios::out | std::ios::app);

	for(int z=0;z<a+1;++z){
		if(zbiorOcenionych[z].kolejnoscwykonania == liczbaZadan){
			plikWyjsciowy << "\nOptymalny czas wykonania zadan: "<< zbiorOcenionych[z].koszt << "\n";

			plikWyjsciowy<<zbiorOcenionych[z].numerZadania<<"("<< zbiorOcenionych[z].koszt <<") <- ";
			zbiorOcenionych[z].koszt = zbiorOcenionych[z].poprzednik->koszt;
			for(int w=0;w<liczbaZadan-1;++w){
				plikWyjsciowy<<zbiorOcenionych[z].poprzednik->numerZadania<<"("<< zbiorOcenionych[z].koszt <<") <- ";
				zbiorOcenionych[z].poprzednik = (zbiorOcenionych[z].poprzednik->poprzednik);
				zbiorOcenionych[z].koszt = zbiorOcenionych[z].poprzednik->koszt;
			}
			plikWyjsciowy<<"START(0)\n";
		}
	}
	plikWyjsciowy.close();
	delete [] zbiorKoncowek;
	delete [] zbiorOcenionych;
}

void Graf::wypiszGraf(){
	if(liczbaZadan<=0){
		std::cout << "Liczba zadan musi byc wieksza od 0!\n";
		return;
	}
	int w = exp2(liczbaZadan - 1);
	for(int i=0;i<w;++i){
		for(int j=0;j<liczbaZadan;++j){
			std::cout << tabelaStanow[i][j].koszt << "|";
			for(int b=0;b<liczbaZadan;++b){
			std::cout << tabelaStanow[i][j].argumenty[b];
			}
			std::cout << "|" << tabelaStanow[i][j].numerZadania << "|" << tabelaStanow[i][j].kolejnoscwykonania << "\t";

		}
		std::cout << std::endl;
	}
}

void Graf::znajdzNajkrtoszaSciezkeAlgorytmemDijkstry() {
	if(liczbaZadan<=0){
		std::cout << "Liczba zadan musi byc wieksza od 0!\n";
		return;
	}
	int a = this->liczbaZadan * exp2(this->liczbaZadan - 1);
	int b = exp2(this->liczbaZadan - 1);
	Komorka* zbiorS = new Komorka[a + 1];
	Komorka* zbiorQ = new Komorka[a + 1];
	// to chyba nie jest
	// do konca
	// potrzebne
	// to  na dole
	//for(int i=0;i<a+1;++i){
		zbiorS[0].argumenty = new bool[liczbaZadan];
		zbiorQ[0].argumenty = new bool[liczbaZadan];
//	}
	zbiorQ[0].kolejnoscwykonania = 0;
	zbiorQ[0].numerZadania = 0;
	zbiorQ[0].liczbaZadan = this->liczbaZadan;
	zbiorQ[0].koszt = 0;
	zbiorQ[0].pomocniczyIndeks = 0;
	for(int c=0;c<liczbaZadan;++c){
		zbiorQ[0].argumenty[c] = false;
	}
	zbiorQ[0].sasiedzi = new int*[liczbaZadan];
	for (int a = 0; a <liczbaZadan; ++a) {
		zbiorQ[0].sasiedzi[a] = new int[2];
	}
	for(int a=0;a<liczbaZadan;++a){
		zbiorQ[0].sasiedzi[a][0] = 0;
		zbiorQ[0].sasiedzi[a][1] = a;
	}
	for (int w = 0,i=1; w < b; ++w) {
		for (int k = 0; k < liczbaZadan; ++k, ++i) {
			zbiorQ[i] = tabelaStanow[w][k];
			zbiorQ[i].koszt = -1;
			zbiorQ[i].pomocniczyIndeks = i;
		}
	}
	float najmniejszyKosztDojscia = 0;
	int indeksWierzcholkaNajmniejszegoKosztuDojscia = 0;
	int indeksSasiada = 0;
	for(int k=0;k != a+1;++k){
		indeksWierzcholkaNajmniejszegoKosztuDojscia = 0;
		najmniejszyKosztDojscia = -1;
		for (int j = 0; j < a + 1; ++j) {
			if(zbiorQ[j].numerZadania == -1){
				continue;
			}
			else{
				if (zbiorQ[j].koszt != -1
						&& (zbiorQ[j].koszt <= najmniejszyKosztDojscia || najmniejszyKosztDojscia == -1)) {
					zbiorS[k] = zbiorQ[j];
					indeksWierzcholkaNajmniejszegoKosztuDojscia = j;
					najmniejszyKosztDojscia = zbiorQ[j].koszt;
				}
			}
		}
		if(zbiorS[k].kolejnoscwykonania == liczbaZadan){
			break;
		}
		for(int z=0;z<liczbaZadan - zbiorS[k].kolejnoscwykonania ;++z){
			indeksSasiada = zbiorS[k].sasiedzi[z][0]*liczbaZadan + zbiorS[k].sasiedzi[z][1] +1;
			if(zbiorQ[indeksSasiada].numerZadania == -1){
				continue;
			}
			if(zbiorQ[indeksSasiada].koszt == -1 ){
				zbiorQ[indeksSasiada].koszt = zbiorS[k].koszt + tabelaStanow[((zbiorQ[indeksSasiada].pomocniczyIndeks-1) - (zbiorQ[indeksSasiada].pomocniczyIndeks-1)%(liczbaZadan))/liczbaZadan][(zbiorQ[indeksSasiada].pomocniczyIndeks-1)%(liczbaZadan)].koszt;
				zbiorQ[indeksSasiada].poprzednik = &zbiorS[k];
			}
		}
		zbiorQ[indeksWierzcholkaNajmniejszegoKosztuDojscia].numerZadania = -1;
	}
	for(int z=0;z<a+1;++z){
		if(zbiorS[z].kolejnoscwykonania == liczbaZadan){
			std::cout << "\nOptymalny czas wykonania zadan: "<< zbiorS[z].koszt << "\n\n";
			std::cout<<zbiorS[z].numerZadania<<"("<< zbiorS[z].koszt <<") <- ";
			zbiorS[z].koszt = zbiorS[z].poprzednik->koszt;
			for(int w=0;w<liczbaZadan-1;++w){
				std::cout<<zbiorS[z].poprzednik->numerZadania<<"("<< zbiorS[z].koszt <<") <- ";
				zbiorS[z].poprzednik = (zbiorS[z].poprzednik->poprzednik);
				zbiorS[z].koszt = zbiorS[z].poprzednik->koszt;
			}
			std::cout<<"START(0)\n";
		}
	}

}

	Graf::~Graf()
	{
		long long int w = exp2(liczbaZadan - 1);
		for(int i=0;i<w;++i)
			delete [] tabelaStanow[i];
		delete [] tabelaStanow;
		// TODO Auto-generated destructor stub
	}

