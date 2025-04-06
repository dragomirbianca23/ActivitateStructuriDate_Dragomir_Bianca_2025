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

struct Nod {
	Masina info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct listaDubla {
	Nod* first;
	Nod* last;
	int nrNoduri;
};
typedef struct listaDubla listaDubla;

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
//3
void afisareListaMasiniDeLaInceput(listaDubla list) {
	Nod* p = list.first;
	while (p)
	{
		afisareMasina(p->info);
		p = p->next;
	}
}

//3
void afisareListaMasiniDeLaSfarsit(listaDubla list) {
	Nod* p = list.last;
	while (p) {
		afisareMasina(p->info);
		p = p->prev;
	}
}

void adaugaMasinaInListaLaSfarsit(listaDubla* list, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->prev = list->last;
	nodNou->next = NULL;

	if (list->last) {
		list->last->next = nodNou;
	}
	else {
		list->first = nodNou;
	}

	list->last = nodNou;
}


//sortare lista
void sorteazaListaDupaPret(listaDubla* lista)
{
	int schimbare;
	do
	{
		schimbare = 0;
		Nod* p = lista->first;

		while (p->next)
		{
			if (p->info.pret > p->next->info.pret)
			{
				Masina aux = p->info;
				p->info = p->next->info;
				p->next->info = aux;
				schimbare = 1;
			}
			p = p->next;
		}
	} while (schimbare);

}

//2
void inserareInListaSortataDupaPret(listaDubla* lista, Masina masinaNoua)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = NULL;

	if (lista->first == NULL && lista->last == NULL)
	{
		lista->first = lista->last = nou;
	}

	Nod* p = lista->first;
	if (masinaNoua.pret < p->info.pret)
	{
		nou->next = p;
		p->prev = nou;
		lista->first = nou;
		return;
	}
	while (p->next && p->next->info.pret < masinaNoua.pret) {
		p = p->next;
	}

	if (p->next == NULL) {
		nou->prev = p;
		p->next = nou;
		lista->last = nou;
	}
	else {
		nou->next = p->next;
		nou->prev = p;
		p->next->prev = nou;
		p->next = nou;
	}
}

listaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	listaDubla lista;
	lista.first = NULL;
	lista.last = NULL;

	while (!feof(f)) {
		Masina aux = citireMasinaDinFisier(f);
		adaugaMasinaInListaLaSfarsit(&lista, aux);
	}
	fclose(f);
	return lista;
}

//1
void stergeNodPePozitie(listaDubla* lista, int pozitie) {
	if (!lista->first || pozitie < 0) return; 
	Nod* p = lista->first;
	int contor = 0;

	while (p && contor < pozitie) {
		p = p->next;
		contor++;
	}

	if (!p) return;  
	if (p == lista->first) {
		lista->first = p->next;
		if (lista->first) {
			lista->first->prev = NULL;
		}
	}
	else if (p == lista->last) {
		lista->last = p->prev;
		if (lista->last) {
			lista->last->next = NULL;
		}
	}
	else {
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}

	free(p->info.model);
	free(p->info.numeSofer);
	free(p);
}

void dezalocareLDMasini(listaDubla* lista) {
	while (lista->first) {
		Nod* p = lista->first;
		free(p->info.numeSofer);
		free(p->info.model);
		lista->first = lista->first->next;
		free(p);
	}
	lista->last = NULL;
}

void stergeMasinaDupaID(listaDubla* lista, int id) {
	if (!lista->first) return;

	Nod* p = lista->first;

	while (p) {
		if (p->info.id == id) {
			if (p->prev) {
				p->prev->next = p->next;
			}
			else {
				lista->first = p->next;
			}

			if (p->next) {
				p->next->prev = p->prev;
			}
			else {
				lista->last = p->prev;
			}

			free(p->info.model);
			free(p->info.numeSofer);
			free(p);
			return;
		}
		p = p->next;
	}
}



int main() {
	listaDubla lista = citireLDMasiniDinFisier("masini2.txt");

	printf("Afisare de la inceput:\n");
	afisareListaMasiniDeLaInceput(lista);
	printf("\n");
	printf("Afisare de la sfarsit:\n");
	afisareListaMasiniDeLaSfarsit(lista);

	stergeMasinaDupaID(&lista, 3);
	printf("Dupa stergere:\n");
	afisareListaMasiniDeLaInceput(lista);
	sorteazaListaDupaPret(&lista);
	printf("Masinile sortate in functie de pret:");
	afisareListaMasiniDeLaInceput(lista);

	stergeNodPePozitie(&lista, 2);
	dezalocareLDMasini(&lista);
	return 0;
}