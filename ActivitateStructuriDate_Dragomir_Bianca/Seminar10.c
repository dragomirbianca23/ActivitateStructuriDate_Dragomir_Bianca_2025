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

//STACK

struct Stiva {
	Masina info;
	struct Stiva* next;
};
typedef struct Stiva Stiva;

void pushStack(Stiva** nod, Masina masina)
{
	Stiva* aux = (Stiva*)malloc(sizeof(Stiva));
	aux->info = masina;
	aux->next = (*nod);
	(*nod) = aux;
}

Masina popStack(Stiva** nod) {
	Masina masina = { .id = -1 };
	if (*nod) {
		Stiva* aux = *nod;
		masina = aux->info;
		*nod = aux->next;
		free(aux);
	}
	return masina;
}

unsigned char emptyStack(Stiva* cap) {
	return cap == NULL;
}

Stiva* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Stiva* nod = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		if (m.id != 0)  
			pushStack(&nod, m);
	}
	fclose(f);
	return nod;
}

void dezalocareStivaDeMasini(Stiva** cap) {
	while (!emptyStack(*cap)) {
		Masina m = popStack(cap);
		free(m.model);
		free(m.numeSofer);
	}
}

int size(Stiva* cap) {
	int marime = 0;
	while (cap) {
		marime++;
		cap = cap->next;
	}
	return marime;
}

struct Nod {
	Masina info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct Coada {
	Nod* first;
	Nod* last;
};
typedef struct Coada Coada;

void enqueue(Coada* coada, Masina masina)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info.id = masina.id;
	nou->info.nrUsi = masina.nrUsi;
	nou->info.pret = masina.pret;
	nou->info.model = (char*)malloc(strlen(masina.model) + 1);
	strcpy(nou->info.model, masina.model);
	nou->info.numeSofer = (char*)malloc(strlen(masina.numeSofer) + 1);
	strcpy(nou->info.numeSofer, masina.numeSofer);
	nou->info.serie = masina.serie;
	nou->next = coada->first;
	nou->prev = NULL;
	if (coada->first)
	{
		coada->first->prev = nou;
	}
	else
	{
		coada->last = nou;
	}
	coada->first = nou;
}


Masina dequeue(Coada* coada) {
	Masina m = { .id = -1 };
	if (coada->last) {
		Nod* aux = coada->last;
		m = aux->info;

		if (aux->prev) {
			coada->last = aux->prev;
			coada->last->next = NULL;
		}
		else {
			coada->first = coada->last = NULL;
		}

		free(aux);
	}
	return m;
}

Coada citireCoadaDeMasiniDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Coada coada;
	coada.first = NULL;
	coada.last = NULL;
	while (!feof(f))
	{
		enqueue(&coada, citireMasinaDinFisier(f));
	}
	fclose(f);
	return coada;
}


void dezalocareCoadaDeMasini(Coada* coada) {
	while (coada->first) {
		Masina m = dequeue(coada);
		free(m.model);
		free(m.numeSofer);
	}
}

int main() {
	Stiva* stiva = citireStackMasiniDinFisier("masini.txt");
	printf("Stiva - element extras:\n");
	Masina m = popStack(&stiva);
	afisareMasina(m);
	printf("Size stiva ramasa: %d\n", size(stiva));
	dezalocareStivaDeMasini(&stiva);

	Coada coada = citireCoadaDeMasiniDinFisier("masini.txt");
	printf("Coada - element extras:\n");
	Masina m2 = dequeue(&coada);
	afisareMasina(m2);
	dezalocareCoadaDeMasini(&coada);

	return 0;
}