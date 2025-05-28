#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-un arbore binar de cautare
struct Nod
{
	Masina info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

//Masina citireMasinaDinFisier(FILE* file) {
//	char buffer[100];
//	char sep[3] = ",\n";
//	fgets(buffer, 100, file);
//	char* aux;
//	Masina m1;
//	aux = strtok(buffer, sep);
//	m1.id = atoi(aux);
//	m1.nrUsi = atoi(strtok(NULL, sep));
//	m1.pret= atof(strtok(NULL, sep));
//	aux = strtok(NULL, sep);
//	m1.model = malloc(strlen(aux) + 1);
//	strcpy_s(m1.model, strlen(aux) + 1, aux);
//
//	aux = strtok(NULL, sep);
//	m1.numeSofer = malloc(strlen(aux) + 1);
//	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);
//
//	m1.serie = *strtok(NULL, sep);
//	return m1;
//}

Masina citireMasinaDinFisier(FILE* file)
{
	Masina m;
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	m.id = atoi(strtok(buffer, sep));
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));
	char* aux;
	aux = strtok(NULL, sep);
	m.model = (char*)malloc(strlen(aux) + 1);
	strcpy(m.model, aux);
	aux = strtok(NULL, sep);
	m.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy(m.numeSofer, aux);
	m.serie = strtok(NULL, sep)[0];
	return m;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua)
{
	if (*radacina)
	{
		if ((*radacina)->info.id < masinaNoua.id)
		{
			adaugaMasinaInArbore(&(*radacina)->dreapta, masinaNoua);
		}
		if ((*radacina)->info.id > masinaNoua.id)
		{
			adaugaMasinaInArbore(&(*radacina)->stanga, masinaNoua);
		}
	}
	else
	{
		(*radacina) = (Nod*)malloc(sizeof(Nod));
		(*radacina)->stanga = NULL;
		(*radacina)->dreapta = NULL;
		(*radacina)->info.id = masinaNoua.id;
		(*radacina)->info.nrUsi = masinaNoua.nrUsi;
		(*radacina)->info.pret = masinaNoua.pret;
		(*radacina)->info.model = (char*)malloc(strlen(masinaNoua.model) + 1);
		strcpy((*radacina)->info.model, masinaNoua.model);
		(*radacina)->info.numeSofer = (char*)malloc(strlen(masinaNoua.numeSofer) + 1);
		strcpy((*radacina)->info.numeSofer, masinaNoua.numeSofer);
		(*radacina)->info.serie = masinaNoua.serie;
	}
}


//void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
//	if (*radacina)
//	{
//		if (masinaNoua.id > (*radacina)->info.id)
//		{
//			adaugaMasinaInArbore(&(*radacina)->dreapta, masinaNoua);
//		}
//		if (masinaNoua.id < (*radacina)->info.id)
//		{
//			adaugaMasinaInArbore(&(*radacina)->stanga, masinaNoua);
//		}
//	}
//	else
//	{
//		(*radacina) = (Nod*)malloc(sizeof(Nod));
//		(*radacina)->info = masinaNoua;
//		(*radacina)->dreapta = NULL;
//		(*radacina)->stanga = NULL;
//	}
//}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* nod = NULL;
	while (!feof(f))
	{
		adaugaMasinaInArbore(&nod, citireMasinaDinFisier(f));
	}
	fclose(f);
	return nod;
}

void afisarePreordineRSD(Nod* radacina)
{
	if (radacina)
	{
		afisareMasina(radacina->info);
		afisarePreordineRSD(radacina->stanga);
		afisarePreordineRSD(radacina->dreapta);
	}
}

void afisareInordineSRD(Nod* radacina)  // afiseaza sortat crescator
{
	if (radacina)
	{
		afisareInordineSRD(radacina->stanga);
		afisareMasina(radacina->info);
		afisareInordineSRD(radacina->dreapta);
	}
}

void afisarePostordineSDR(Nod* radacina)
{
	if (radacina)
	{
		afisarePostordineSDR(radacina->stanga);
		afisarePostordineSDR(radacina->dreapta);
		afisareMasina(radacina->info);
	}
}

void dezalocareArboreDeMasini(Nod** nod) {
	if (*nod)
	{
		dezalocareArboreDeMasini(&(*nod)->stanga);
		dezalocareArboreDeMasini(&(*nod)->dreapta);
		free((*nod)->info.numeSofer);
		free((*nod)->info.model);
		free((*nod));
		(*nod) = NULL;
	}
}

Masina getMasinaByID(Nod* nod, int id) {
	Masina m;
	m.id = -1;
	if (nod)
	{
		if (id < nod->info.id)
		{
			return getMasinaByID(nod->stanga, id);
		}
		else if (id > nod->info.id)
		{
			return getMasinaByID(nod->dreapta, id);
		}
		else
			return nod->info;
	}
	return m;
}

int determinaNumarNoduri(Nod* nod) {
	if (nod)
	{
		return 1 + determinaNumarNoduri(nod->dreapta) + determinaNumarNoduri(nod->stanga);
	}
	else
		return 0;
}

int calculeazaInaltimeArbore(Nod* nod) {
	if (nod)
	{
		int inaltimeStanga = calculeazaInaltimeArbore(nod->stanga);
		int inaltimeDreapta = calculeazaInaltimeArbore(nod->dreapta);
		if (inaltimeDreapta > inaltimeStanga)
		{
			return 1 + inaltimeDreapta;
		}
		else
			return 1 + inaltimeStanga;
	}
	else
		return 0;
}

float calculeazaPretTotal(Nod* nod) {
	if (nod)
	{
		float totalStanga = calculeazaPretTotal(nod->stanga);
		float totalDreapta = calculeazaPretTotal(nod->dreapta);
		return nod->info.pret + totalDreapta + totalStanga;
	}
	else return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* nod, const char* numeSofer) {
	float suma = 0;
	if (nod)
	{
		if (strcmp(nod->info.numeSofer, numeSofer) == 0)
		{
			suma += nod->info.pret;
		}
		suma += calculeazaPretulMasinilorUnuiSofer(nod->stanga, numeSofer);
		suma += calculeazaPretulMasinilorUnuiSofer(nod->dreapta, numeSofer);
	}
	else return suma;
}

//Sa se implementeze o functie care pentru o radacina primita ca parametru, returneaza
//radacina subarborelui stang sau drept care are inaltimea mai mare.
Nod* subarboreCuInaltimeaCeaMaiMare(Nod* nod)
{
	if (nod)
	{
		int inaltimeSt = calculeazaInaltimeArbore(nod->stanga);
		int inaltimeDr = calculeazaInaltimeArbore(nod->dreapta);
		if (inaltimeSt > inaltimeDr)
		{
			nod->stanga;
		}
		else return nod->dreapta;
	}
	else return NULL;
}

//Sa se realizeze o functie care pentru un nod radacina primit, determina numarul de noduri
//aflate in subordine.
int numarNoduriSubordine(Nod* nod)
{
	if (nod)
	{
		return determinaNumarNoduri(nod) - 1;
	}
	else return 0;
}


//Sa se realizeze o functie care pentru un nod radacina primit ca parametru, returneaza
//radacina subarborelui stang sau drept care are mai multe noduri in subordine.
Nod* subarboreCuMaiMultiDescendenti(Nod* nod)
{
	if (nod)
	{
		int nrSt = determinaNumarNoduri(nod->stanga);
		int nrDr = determinaNumarNoduri(nod->dreapta);
		if (nrSt > nrDr)
		{
			return nod->stanga;
		}
		else return nod->dreapta;
	}
	else return NULL;
}

int main() {
	Nod* radacina = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareInordineSRD(radacina);
	Masina m = getMasinaByID(radacina, 8);
	printf("Masina cautata este: \n");
	afisareMasina(m);
	printf("Numarul de noduri este: %d\n ", determinaNumarNoduri(radacina));
	printf("Pretul total al masinilor din arbore este: %.2f\n", calculeazaPretTotal(radacina));
	printf("Pretul masinilor unui sofer este: %.2f\n", calculeazaPretulMasinilorUnuiSofer(radacina, "Gigel"));
	printf("Inaltimea arborelui este: %d\n", calculeazaInaltimeArbore(radacina));

	dezalocareArboreDeMasini(&radacina);
	return 0;
}