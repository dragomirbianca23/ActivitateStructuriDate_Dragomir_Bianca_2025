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

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
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

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore != NULL)
	{
		int inaltimeStanga = calculeazaInaltimeArbore(arbore->stanga);
		int inaltimeDreapta = calculeazaInaltimeArbore(arbore->dreapta);
		return (inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta) + 1;
	}
	else
	{ 
		return  0;
	}
	return 0;
}

//ALTE FUNCTII NECESARE:
// - aici veti adauga noile functii de care aveti nevoie.
int calculeazaGradEchilibru(Nod* arbore)
{
	int gradEchilibru = calculeazaInaltimeArbore(arbore->stanga) - calculeazaInaltimeArbore(arbore->dreapta);
	return gradEchilibru;
}

void rotireStanga(Nod** arbore)
{
	Nod* aux = (*arbore)->dreapta;
	(*arbore)->dreapta = aux->stanga;
	aux->stanga = (*arbore);
	(*arbore) = aux;
}

void rotireDreapta(Nod** arbore)
{
	Nod* aux = (*arbore)->stanga;
	(*arbore)->stanga = aux->dreapta;
	aux->dreapta = (*arbore);
	(*arbore) = aux;
}

void adaugaMasinaInArboreEchilibrat(Nod** arbore, Masina masinaNoua) {
	if (*arbore)
	{
		if ((*arbore)->info.id < masinaNoua.id)
		{
			adaugaMasinaInArboreEchilibrat(&((*arbore)->dreapta), masinaNoua);
		}
		else
		{
			adaugaMasinaInArboreEchilibrat(&((*arbore)->stanga), masinaNoua);
		}
		int gradEchilibru = calculeazaGradEchilibru(*arbore);
		if (gradEchilibru == 2)
		{
			if (calculeazaGradEchilibru((*arbore)->stanga) == 1)
			{
				rotireDreapta(arbore);
			}
			else
			{
				rotireStanga(&((*arbore)->stanga));
				rotireDreapta(arbore);
			}
		}
		else if (gradEchilibru == -2)
		{
			if (calculeazaGradEchilibru((*arbore)->dreapta) != -1)
			{
				rotireDreapta(&(*arbore)->dreapta);

			}
			rotireStanga(arbore);
		}
	}
	else
	{
		(*arbore) = (Nod*)malloc(sizeof(Nod));
		(*arbore)->info = masinaNoua;
		(*arbore)->stanga = NULL;
		(*arbore)->dreapta = NULL;
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* nod = NULL;
	while (!feof(f))
	{
		adaugaMasinaInArboreEchilibrat(&nod, citireMasinaDinFisier(f));
	}
	fclose(f);
	return nod;
}

void afisarePreOrdineRSD(Nod* radacina) {
	if (radacina != NULL)
	{
		afisareMasina(radacina->info);
		afisarePreOrdineRSD(radacina->stanga);
		afisarePreOrdineRSD(radacina->dreapta);
	}
}

void dezalocareArboreDeMasini(Nod** nod) {
	if (*nod)
	{
		dezalocareArboreDeMasini(&(*nod)->dreapta);
		dezalocareArboreDeMasini(&(*nod)->stanga);
		free((*nod)->info.model);
		free((*nod)->info.numeSofer);
		free((*nod));
		(*nod) = NULL;
	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(Nod* nod, int id)
{
	Masina m;
	m.id = -1;
	if (nod)
	{
		if (id < nod->info.id)
		{
			return getMasinaByID(nod->stanga, id);
		}
		if (id > nod->info.id)
		{
			return getMasinaByID(nod->dreapta, id);
		}
		else
			return nod->info;
	}
	else return m;
}

int determinaNumarNoduri(Nod* nod)
{
	if (nod)
	{
		int noduriDreapta = determinaNumarNoduri(nod->dreapta);
		int noduriStanga = determinaNumarNoduri(nod->stanga);
		return 1 + noduriDreapta + noduriStanga;
	}
	else return 0;
}

float calculeazaPretTotal(Nod* radacina)
{
	if (radacina == NULL)
	{
		return 0;
	}
	else
	{
		float sumaStanga = calculeazaPretTotal(radacina->stanga);
		float sumaDreapta = calculeazaPretTotal(radacina->dreapta);

		return sumaStanga + sumaDreapta + radacina->info.pret;
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* nod, const char* numeSofer)
{
	float suma = 0;
	if (nod)
	{
		if (strcmp(nod->info.numeSofer, numeSofer) == 0)
		{
			suma += nod->info.pret;
		}
		suma += calculeazaPretulMasinilorUnuiSofer(nod->stanga, numeSofer);
		suma += calculeazaPretulMasinilorUnuiSofer(nod->dreapta, numeSofer);
		return suma;
	}
	else return 0;
}
int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisarePreOrdineRSD(arbore);
	printf("Pretul tuturor masinilor este: %.2f\n", calculeazaPretTotal(arbore));
	printf("Inaltimea arborelui este: %d\n", calculeazaInaltimeArbore(arbore));
	printf("Gradul de echilibru este: %d\n", calculeazaGradEchilibru(arbore));
	printf("Pretul masinilor unui sofer este: %.2f\n", calculeazaPretulMasinilorUnuiSofer(arbore, "Gigel"));
	Masina m = getMasinaByID(arbore, 8);
	printf("Masina cautata este: \n");
	afisareMasina(m);
	printf("Numarul de noduri este: %d\n", determinaNumarNoduri(arbore));
	dezalocareArboreDeMasini(&arbore);
	return 0;
}