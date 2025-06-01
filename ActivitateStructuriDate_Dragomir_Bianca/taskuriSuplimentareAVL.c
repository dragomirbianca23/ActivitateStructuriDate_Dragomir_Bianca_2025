#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

struct NodSimplu {
	Masina info;
	struct NodSimplu* next;
};
typedef struct NodSimplu NodSimplu;

struct NodDublu {
	Masina info;
	struct NodDublu* prev;
	struct NodDublu* next;
};
typedef struct NodDublu NodDublu;


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

//La inserare se verifica gradul de echilibru, iar in cazul in care exista un dezechilibru sa se
//relaizeze rotirile necesare, astfel incat arborele sa fie echilibrat.
//Sa se realizeze o functie care salveaza nodurile din arborele AVL intr-un vector, prin
//parcurgerea preordine, inordine, postordine.

void preordineVector(Nod* rad, Masina* vector, int* index) {
	if (rad) {
		vector[*index] = rad->info;
		(*index)++;
		preordineVector(rad->stanga, vector, index);
		preordineVector(rad->dreapta, vector, index);
	}
}

void inordineVector(Nod* rad, Masina* vector, int* index) {
	if (rad) {
		inordineVector(rad->stanga, vector, index);
		vector[*index] = rad->info;
		(*index)++;
		inordineVector(rad->dreapta, vector, index);
	}
}

void postordineVector(Nod* rad, Masina* vector, int* index) {
	if (rad) {
		postordineVector(rad->stanga, vector, index);
		postordineVector(rad->dreapta, vector, index);
		vector[*index] = rad->info;
		(*index)++;
	}
}

void inserareListaSimpla(NodSimplu** cap, Masina m) {
	NodSimplu* nou = (NodSimplu*)malloc(sizeof(NodSimplu));
	nou->info = m;
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		NodSimplu* temp = *cap;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
}

//Sa se realizeze o functie care salveaza nodurile din arborele AVL intr - o lista simplu inlantuita,
//prin parcurgerea preordine, inordine, postordine.

void preordineListaSimpla(Nod* rad, NodSimplu** cap) {
	if (rad) {
		inserareListaSimpla(cap, rad->info);
		preordineListaSimpla(rad->stanga, cap);
		preordineListaSimpla(rad->dreapta, cap);
	}
}

void inordineListaSimpla(Nod* rad, NodSimplu** cap) {
	if (rad) {
		inordineListaSimpla(rad->stanga, cap);
		inserareListaSimpla(cap, rad->info);
		inordineListaSimpla(rad->dreapta, cap);
	}
}

void postordineListaSimpla(Nod* rad, NodSimplu** cap) {
	if (rad) {
		postordineListaSimpla(rad->stanga, cap);
		postordineListaSimpla(rad->dreapta, cap);
		inserareListaSimpla(cap, rad->info);
	}
}

//Sa se realizeze o functie care salveaza nodurile din arborele AVL intr - o lista dublu inlantuita,
//prin parcurgerea preordine, inordine, postordine.
void inserareListaDubla(NodDublu** cap, Masina m) {
	NodDublu* nou = (NodDublu*)malloc(sizeof(NodDublu));
	nou->info = m;
	nou->next = NULL;
	nou->prev = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		NodDublu* temp = *cap;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
		nou->prev = temp;
	}
}

void preordineListaDubla(Nod* rad, NodDublu** cap) {
	if (rad) {
		inserareListaDubla(cap, rad->info);
		preordineListaDubla(rad->stanga, cap);
		preordineListaDubla(rad->dreapta, cap);
	}
}

void inordineListaDubla(Nod* rad, NodDublu** cap) {
	if (rad) {
		inordineListaDubla(rad->stanga, cap);
		inserareListaDubla(cap, rad->info);
		inordineListaDubla(rad->dreapta, cap);
	}
}

void postordineListaDubla(Nod* rad, NodDublu** cap) {
	if (rad) {
		postordineListaDubla(rad->stanga, cap);
		postordineListaDubla(rad->dreapta, cap);
		inserareListaDubla(cap, rad->info);
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


int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisarePreOrdineRSD(arbore);
	printf("Inaltimea arborelui este: %d\n", calculeazaInaltimeArbore(arbore));
	printf("Gradul de echilibru este: %d\n", calculeazaGradEchilibru(arbore));
	int nr = determinaNumarNoduri(arbore);
	Masina* vector = (Masina*)malloc(sizeof(Masina) * nr);
	int idx = 0;
	inordineVector(arbore, vector, &idx);
	for (int i = 0; i < nr; i++) {
		afisareMasina(vector[i]);
	}
	free(vector);

	NodSimplu* listaSimpla = NULL;
	preordineListaSimpla(arbore, &listaSimpla);

	NodDublu* listaDubla = NULL;
	postordineListaDubla(arbore, &listaDubla);

	dezalocareArboreDeMasini(&arbore);
	return 0;
}
