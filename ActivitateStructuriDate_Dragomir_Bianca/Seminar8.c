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

struct Nod
{
	Masina info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;


//creare structura pentru un nod dintr-un arbore binar de cautare

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}


void adaugaMasinaInArbore(Nod** nod, Masina masinaNoua) {
	if (*nod != NULL)
	{
		if (masinaNoua.id > (*nod)->info.id)
		{
			adaugaMasinaInArbore(&(*nod)->dr, masinaNoua);
		}
		else if (masinaNoua.id < (*nod)->info.id)
		{
			adaugaMasinaInArbore(&(*nod)->st, masinaNoua);
		}
	}
	else
	{
		(*nod) = (Nod*)malloc(sizeof(Nod));
		(*nod)->info = masinaNoua;
		(*nod)->st = NULL;
		(*nod)->dr = NULL;
	}
	
}

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

void afisarePreordine(Nod* nod)
{
	if (nod)
	{
		afisareMasina(nod->info);
		afisarePreordine(nod->st);
		afisarePreordine(nod->dr);
	}
}

void afisareInordine(Nod* nod)
{
	if (nod)
	{
		afisarePreordine(nod->st);
		afisareMasina(nod->info);
		afisarePreordine(nod->dr);
	}
}

void afisarePostOrdineSDR(Nod* nod)
{
	if (nod)
	{
		afisarePostOrdineSDR(nod->st);
		afisarePostOrdineSDR(nod->dr);
		afisareMasina(nod->info);
	}
}

void dezalocareArboreDeMasini(Nod** nod) {
	if (*nod)
	{
		dezalocareArboreDeMasini(&(*nod)->st);
		dezalocareArboreDeMasini(&(*nod)->dr);
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
			return getMasinaByID(nod->st, id);
		}
		else
		{
			if (id < nod->info.id)
			{
				return getMasinaByID(nod->st, id);
			}
			else
			{
				return nod->info;
			}
		}
	}
	return m;
}

int determinaNumarNoduri(Nod* nod) {
	int nrNoduri = 0;
	if (nod != NULL)
	{
		nrNoduri += determinaNumarNoduri(nod->st);
		nrNoduri += determinaNumarNoduri(nod->dr);
		nrNoduri += 1;
	}
	return nrNoduri;
}

int calculeazaInaltimeArbore(Nod* nod) {
	if (nod)
	{
		int inaltimeStanga = calculeazaInaltimeArbore(nod->st);
		int inaltimeDreapta = calculeazaInaltimeArbore(nod->dr);
		if (inaltimeDreapta > inaltimeStanga)
		{
			return 1 + inaltimeDreapta;
		}
		else return 1 + inaltimeStanga;
	}
	else return 0;
}

float calculeazaPretTotal(Nod* nod) {
	if (nod)
	{
		int pretStanga = calculeazaPretTotal(nod->st);
		int pretDreapta = calculeazaPretTotal(nod->dr);
		return pretDreapta + pretStanga + nod->info.pret;
	}
	else return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* nod, const char* numeSofer) {
	int suma = 0;
	if (nod)
	{
		if (strcmp(nod->info.numeSofer, numeSofer) == 0)
		{
			suma += nod->info.pret;
		}
		suma += calculeazaPretulMasinilorUnuiSofer(nod->st, numeSofer);
		suma += calculeazaPretulMasinilorUnuiSofer(nod->dr, numeSofer);
	}
	else return suma;
}

int main() {
	Nod* rad = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisarePreordine(rad);
	printf("--------------------\n");
	afisareInordine(rad);
	printf("--------------------\n");
	afisareMasina(getMasinaByID(rad, 1));
	printf("Nr noduri:%d\n", determinaNumarNoduri(rad));
	printf("Inaltimea arborelui: %d\n", calculeazaInaltimeArbore(rad));
	printf("Pretul total al masinilor este: %.2f\n", calculeazaPretTotal(rad));
	printf("Pretul masinilor unui sofer este: %.2f\n", calculeazaPretulMasinilorUnuiSofer(rad, "Gigel"));

	return 0;
}