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

typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* next;
};


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

void afisareListaMasini(Nod* cap) {  // pointer transmis prin valoare, nu vrem sa modificam adresa dar putem modifica ce se gaseste la adresa respectiva
	while (cap != NULL)
	{
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) { //avem ** pt posibil o sa modificam adresa
	Nod* nou;
	nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	if ((*cap) != NULL) {
		Nod* aux = (*cap);
		while (aux->next != NULL)
		{
			aux = aux->next;
		}
		aux->next = nou;
	}
	else
	{
		*cap = nou;
	}

}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = (*cap);
	(*cap) = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* cap = NULL;
	while (!feof(f)) {
		Masina m;
		m = citireMasinaDinFisier(f);
		adaugaLaInceputInLista(&cap, m);
	}
	fclose(f);
	return cap;
}

void dezalocareListaMasini(Nod** cap) {
	while ((*cap) != NULL) {
		Nod* temp;
		temp = (*cap)->next;
		free((*cap)->info.model);
		free((*cap)->info.numeSofer);
		free((*cap));
		(*cap) = temp; //nu initializam *cap cu NULL pt ca la un moment dat o sa ajunga null cand ajunge la finalul listei
	}

}

float calculeazaPretMediu(Nod* cap) {
	float suma = 0;
	int k = 0;
	while (cap != NULL)
	{
		suma += cap->info.pret;
		k++;
		cap = cap->next;
	}
	if (k != 0) {
		float medie = suma / k;
		return medie;
	}
	else
		return 0;
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
	Nod* temp = *cap;
	Nod* nodAnterior = NULL;
	while (temp != NULL && temp->info.serie == serieCautata) {
		*cap = temp->next;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
		temp = *cap;
	}
	while (temp != NULL) {
		while (temp != NULL && temp->info.serie != serieCautata) {
			nodAnterior = temp;
			temp = temp->next;
		}
		if (temp == NULL)
		{
			return 0;
		}
		nodAnterior->next = temp->next;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
		temp = nodAnterior->next;
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
	float suma = 0;
	while (cap != NULL) {
		if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
			suma += cap->info.pret;
		}
		cap = cap->next;
	}
	return suma;
}


int main() {
	Nod* cap = citireListaMasiniDinFisier("masini2.txt");
	afisareListaMasini(cap);
	float medie = calculeazaPretMediu(cap);
	printf("Media este: %f\n", medie);
	stergeMasiniDinSeria(&cap, 'A');
	printf("Dupa stergere:\n");
	afisareListaMasini(cap);

	float totalPret = calculeazaPretulMasinilorUnuiSofer(cap, "Gheorghe");
	printf("Pretul total al masinilor lui Gheorghe este: %f\n", totalPret);
	dezalocareListaMasini(&cap);

	return 0;
}