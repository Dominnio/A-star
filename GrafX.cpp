/*
* GrafX.cpp
//============================================================================
// Nazwa		: projekt_AAL.cpp
// Autor		: Dominik Orliñski
// Wersja		: 1.0
// Nazwa		: Szeregowanie zadañ
//============================================================================
*/

#include "GrafX.h"
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <iostream>

GrafX::GrafX(int liczbaZadan) {
	if (liczbaZadan <= 0) {
		std::cout << "Liczba zadan musi byc wieksza od 0!\n";
		return;
	}
	this->liczbaZadan = liczbaZadan;
	short k = liczbaZadan * liczbaZadan;
	srand(time(NULL));
	unsigned long int w = liczbaZadan;
	this->tabelaStanow = new KomorkaX *[w];
	for (int i = 0; i < w; ++i) {
		tabelaStanow[i] = new KomorkaX[k];
		for (int j = 0, z = 1 ; j < k; ++j) {
			tabelaStanow[i][j].czyRozpatrzone = false;
			tabelaStanow[i][j].numerZadania = z;
			tabelaStanow[i][j].kolejnoscwykonania = j%liczbaZadan + 1;
			tabelaStanow[i][j].liczbaZadan = liczbaZadan;
			tabelaStanow[i][j].poprzednik = &tabelaStanow[i][j];
			if (tabelaStanow[i][j].kolejnoscwykonania == liczbaZadan) {
				z++;
			}
			if (tabelaStanow[i][j].kolejnoscwykonania != 1 && tabelaStanow[i][j].numerZadania != i + 1 || (tabelaStanow[i][j].kolejnoscwykonania == 1 && tabelaStanow[i][j].numerZadania == i + 1)) {
				tabelaStanow[i][j].koszt = 1 + rand() % 20;
			}
			else {
				tabelaStanow[i][j].koszt = 0;
			}
			/*
			tabelaStanow[i][j].sasiedzi = new int*[liczbaZadan - 1];
			for (int a = 0, b = 0; a <liczbaZadan - 1; ++a, ++b) {
				tabelaStanow[i][j].sasiedzi[a] = new int[2];
				if (b == j) {
					b++;
				}
				tabelaStanow[i][j].sasiedzi[a][0] = i;
				tabelaStanow[i][j].sasiedzi[a][1] = b;
			}
			*/
		}
	}
}
int GrafX::znajdzNajkrotszaSciezkeAlgorytmemA(char* nazwaPliku) {
	int w = liczbaZadan;
	int k = liczbaZadan*liczbaZadan;
	float* tabelaMinimalnychCzasowStanow = new float[k];
	float* tabelaMinimalnychCzasowZadan = new float[w];
	float* tabelaMinimalnychCzasowZadanPom = new float[w];

	float sumaHeurystyczna = 0;
	for (int i = 0; i<k; i++) {
		tabelaMinimalnychCzasowStanow[i] = -1;
	}
	for (int i = 0; i<w; i++) {
		tabelaMinimalnychCzasowZadan[i] = -1;
	}
	for (int i = 0; i < k; ++i) {
		for (int j = 0; j < w; ++j) {
			if (tabelaStanow[j][i].koszt != 0) {
				if (tabelaMinimalnychCzasowStanow[i] == -1 || tabelaStanow[j][i].koszt < tabelaMinimalnychCzasowStanow[i]) {
					tabelaMinimalnychCzasowStanow[i] = tabelaStanow[j][i].koszt;
				}
			}
		}
	}
	for (int i = 0, z = 0; i < k; ++i) {
		if (tabelaMinimalnychCzasowZadan[z] == -1 || tabelaMinimalnychCzasowStanow[i] < tabelaMinimalnychCzasowZadan[z]) {
			tabelaMinimalnychCzasowZadan[z] = tabelaMinimalnychCzasowStanow[i];
		}
		if (i%liczbaZadan == liczbaZadan - 1) {
			z++;
		}
	}
	std::cout << std::endl;
	
	for (int i = 0; i < w; ++i) {
		sumaHeurystyczna = sumaHeurystyczna + tabelaMinimalnychCzasowZadan[i];
	}
	for (int i = 0; i < w; ++i) {
		tabelaMinimalnychCzasowZadanPom[i] = tabelaMinimalnychCzasowZadan[i];
		std::cout << tabelaMinimalnychCzasowZadan[i] << " ";
	}

	std::cout << std::endl  << sumaHeurystyczna << std::endl;
	for (int z = 0; z < w; ++z) {
		for (int i = 0; i < w; ++i) {
			for (int j = 0; j < w; ++j) {
				for (int l = 0; l < w; ++l) {
					tabelaMinimalnychCzasowZadanPom[l] = tabelaMinimalnychCzasowZadan[l];
				}
				if (tabelaStanow[i][z*w + j].koszt == 0) {
					tabelaStanow[i][z*w + j].heurystyka = 0;
				}
				else {
					tabelaStanow[i][z*w + j].heurystyka = sumaHeurystyczna;
					tabelaMinimalnychCzasowZadanPom[i] = 0;
					tabelaMinimalnychCzasowZadanPom[z] = 0;
					tabelaStanow[i][z*w + j].heurystyka -= tabelaMinimalnychCzasowZadan[z];
					if (tabelaStanow[i][z*w + j].numerZadania != i + 1) {
						tabelaStanow[i][z*w + j].heurystyka -= tabelaMinimalnychCzasowZadan[i];
					}
					for (int n = 2; n < tabelaStanow[i][z*w + j].kolejnoscwykonania; ++n) {
						float najdluzszeZPozostalych = -1;
						int indeksNajdluzszegoZPozostalych;
						for (int l = 0; l < w; ++l) {
							if (najdluzszeZPozostalych == -1 || tabelaMinimalnychCzasowZadanPom[l] > najdluzszeZPozostalych) {
								najdluzszeZPozostalych = tabelaMinimalnychCzasowZadanPom[l];
								indeksNajdluzszegoZPozostalych = l;
							}
						}
						tabelaStanow[i][z*w + j].heurystyka -= tabelaMinimalnychCzasowZadanPom[indeksNajdluzszegoZPozostalych];
						tabelaMinimalnychCzasowZadanPom[indeksNajdluzszegoZPozostalych] = 0;
					}
				}
				std::cout << tabelaStanow[i][z*w + j].heurystyka << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	KomorkaX* pom;
	int a = liczbaZadan * liczbaZadan *liczbaZadan;
	KomorkaX* zbiorKoncowek = new KomorkaX[a];
	KomorkaX* zbiorOcenionych = new KomorkaX[a];
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < liczbaZadan; ++j) {
			zbiorKoncowek[i*liczbaZadan + j] = 0;
			zbiorOcenionych[i*liczbaZadan + j] = 0;
		}
	}
	for (int i = 0, j = 0; i < w; ++i, j = j + liczbaZadan) {
		zbiorKoncowek[w*i + k*i] = tabelaStanow[i][j];
	}
	float minimalnaHeurystyka = -1;
	int indeksMinimum;
	int indeksSasiada = 0;
	int flaga = true;
	
	while (1) {
		minimalnaHeurystyka = -1;
		std::cout << std::endl;
		for (int i = 0; i< a; i++) {
			if (zbiorKoncowek[i].koszt != 0) {
				std::cout << zbiorKoncowek[i].heurystyka + zbiorKoncowek[i].koszt << "(" << i << ")" << ", ";
				if ((zbiorKoncowek[i].heurystyka + zbiorKoncowek[i].koszt < minimalnaHeurystyka) || minimalnaHeurystyka == -1) {
					minimalnaHeurystyka = zbiorKoncowek[i].heurystyka + zbiorKoncowek[i].koszt;
					indeksMinimum = i;
				}
			}
		}
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout <<"Indeks min: "<< indeksMinimum << std::endl;
		if (zbiorKoncowek[indeksMinimum].kolejnoscwykonania == liczbaZadan) {
			zbiorOcenionych[indeksMinimum] = zbiorKoncowek[indeksMinimum];
			break;
		}
		zbiorOcenionych[indeksMinimum] = zbiorKoncowek[indeksMinimum];
		zbiorKoncowek[indeksMinimum] = 0;
		// s¹ sasiedzi ktorych dodwac nie powinnismy, powinnismy dodaæ nastepnikow bez tego zadania i bez poprzednika (jesli byl)
		for (int i = 0; i < w; i++) {
			// musze tak zrobic zeby dwa razy nie wpisac tego samego typa // w tej wersji chyba zbedne
			if (i + 1 != zbiorOcenionych[indeksMinimum].numerZadania && i != (indeksMinimum - indeksMinimum%k) / k) {
				indeksSasiada = k*(zbiorOcenionych[indeksMinimum].numerZadania -1) + i*w + zbiorOcenionych[indeksMinimum].kolejnoscwykonania;
			}
			else {
				continue;
			}
			if (zbiorOcenionych[indeksMinimum].kolejnoscwykonania != 2) {
				pom = zbiorOcenionych[indeksMinimum].poprzednik;
				flaga = true;
				for (int i = 0; i < zbiorOcenionych[indeksMinimum].kolejnoscwykonania - 2; ++i) {
					std::cout << "sprawdzam "<< tabelaStanow[(indeksSasiada - indeksSasiada%k) / k][indeksSasiada%k].numerZadania <<" | " << pom->poprzednik->numerZadania - 1 <<"\n";
					if (tabelaStanow[(indeksSasiada - indeksSasiada%k) / k][indeksSasiada%k].numerZadania == pom->poprzednik->numerZadania) {
						flaga = false;
						break;
					}
					else {
						pom = pom->poprzednik;
					}
				}
				if (flaga == false) {
					std::cout << "wywalilem\n";
					flaga = true;
					continue;
				}
			}
			std::cout << indeksSasiada << " | "<< (indeksSasiada - indeksSasiada%k)/k << " | " << indeksSasiada%k << std::endl;
			if (zbiorOcenionych[indeksSasiada].liczbaZadan != 0) {
				continue;
			}
			if (zbiorKoncowek[indeksSasiada].liczbaZadan == 0) {
				zbiorKoncowek[indeksSasiada] = tabelaStanow[(indeksSasiada - indeksSasiada%k)/k][indeksSasiada%k];
				zbiorKoncowek[indeksSasiada].koszt += zbiorOcenionych[indeksMinimum].koszt;
				zbiorKoncowek[indeksSasiada].poprzednik = &zbiorOcenionych[indeksMinimum];
			}
			else {
				if (zbiorKoncowek[indeksSasiada].koszt > zbiorOcenionych[indeksMinimum].koszt + tabelaStanow[(indeksSasiada - indeksSasiada%k) / k][indeksSasiada%k].koszt) {
					zbiorKoncowek[indeksSasiada].koszt = zbiorOcenionych[indeksMinimum].koszt + tabelaStanow[(indeksSasiada - indeksSasiada%k) / k][indeksSasiada%k].koszt;
					zbiorKoncowek[indeksSasiada].poprzednik = &zbiorOcenionych[indeksMinimum];
				}
			}
		}
	}
	
	std::fstream plikWyjsciowy;
	plikWyjsciowy.open(nazwaPliku, std::ios::in | std::ios::out | std::ios::app);

	for (int z = 0; z < a; ++z) {
		if (zbiorOcenionych[z].kolejnoscwykonania == liczbaZadan) {
			plikWyjsciowy << "\nOptymalny czas wykonania zadan: " << zbiorOcenionych[z].koszt << "\n";

			plikWyjsciowy << zbiorOcenionych[z].numerZadania << "(" << zbiorOcenionych[z].koszt << ") <- ";
			zbiorOcenionych[z].koszt = zbiorOcenionych[z].poprzednik->koszt;
			
			for (int w = 0; w<liczbaZadan - 1; ++w) {
				plikWyjsciowy << zbiorOcenionych[z].poprzednik->numerZadania << "(" << zbiorOcenionych[z].koszt << ") <- ";
				zbiorOcenionych[z].poprzednik = (zbiorOcenionych[z].poprzednik->poprzednik);
				zbiorOcenionych[z].koszt = zbiorOcenionych[z].poprzednik->koszt;
			}
			
			plikWyjsciowy << "START(0)\n";
		}
		
	}
	plikWyjsciowy.close();
	 
//	for (int k = 0; k<liczbaZadan; k++) {
//		delete[] zbiorOcenionych[0].sasiedzi[k];
//	}
//	delete[] zbiorOcenionych[0].sasiedzi;
//	delete[] zbiorKoncowek;
//	delete[] zbiorOcenionych;
//	delete[] tabelaMinimalnychCzasow;
	
}


void GrafX::wypiszGrafX() {
	if (liczbaZadan <= 0) {
		std::cout << "Liczba zadan musi byc wieksza od 0!\n";
		return;
	}
	int w = liczbaZadan;
	int k = liczbaZadan*liczbaZadan;

	for (int i = 0; i<w; ++i) {
		for (int j = 0; j<k; ++j) {
			std::cout << tabelaStanow[i][j].koszt;
			std::cout << "|" << tabelaStanow[i][j].kolejnoscwykonania;
			std::cout << "|" << tabelaStanow[i][j].numerZadania << "\t";

		}
		std::cout << std::endl;
	}
}

GrafX::~GrafX()
{
	short k = liczbaZadan;
	short w = liczbaZadan;
	for (int i = 0; i < w; ++i) {
		delete [] tabelaStanow[i];
	}
	delete[] tabelaStanow;
}