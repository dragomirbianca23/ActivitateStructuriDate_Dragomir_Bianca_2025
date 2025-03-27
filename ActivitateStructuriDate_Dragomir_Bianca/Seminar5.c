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

struct nod {
	Masina info;
	struct nod* next;
	struct nod* prev;
};
typedef struct nod nod;

struct listaDubla {
	nod* first;
	nod* last;
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

void afisareListaMasiniDeLaInceput(listaDubla list) {
	nod* temp = list.first;
	while (temp)
	{
		afisareMasina(temp->info);
		temp = temp->next;
	}
}

void afisareListaMasiniDeLaSfarsit(listaDubla list) {
	nod* temp = list.last;
	while (temp) {
		afisareMasina(temp->info);
		temp = temp->prev;
	}
}

void adaugaMasinaInListaLaSfarsit(listaDubla* list, Masina masinaNoua) {
	nod* nodNou = (nod*)malloc(sizeof(nod));
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

void adaugaMasinaInListaLaInceput(listaDubla* list, Masina masinaNoua) {
	nod* temp = (nod*)malloc(sizeof(nod));
	temp->info = masinaNoua;
	temp->next = list->first;
	temp->prev = NULL;

	if (list->first) {
		list->first->prev = temp;
	}
	else {
		list->last = temp;
	}

	list->first = temp;

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

void dezalocareLDMasini(listaDubla* lista) {
	while (lista->first) {
		nod* temp = lista->first;
		free(temp->info.numeSofer);
		free(temp->info.model);
		lista->first = lista->first->next;
		free(temp);
	}
	lista->last = NULL;
}

float calculeazaPretMediu(listaDubla lista) {
	float pretMediu;
	float suma = 0;
	int contor = 0;
	nod* temp = lista.first;

	while (temp) {
		contor++;
		suma = suma + temp->info.pret;
		temp = temp->next;
	}

	if (contor == 0)
		return 0;

	pretMediu = suma / contor;
	return pretMediu;
}

void stergeMasinaDupaID(listaDubla* lista, int id) {
	if (!lista->first) return; 

	nod* temp = lista->first;

	while (temp) {
		if (temp->info.id == id) {
			if (temp->prev) {
				temp->prev->next = temp->next;
			}
			else {
				lista->first = temp->next;
			}

			if (temp->next) {
				temp->next->prev = temp->prev;
			}
			else {
				lista->last = temp->prev;
			}

			free(temp->info.model);
			free(temp->info.numeSofer);
			free(temp);
			return; 
		}
		temp = temp->next;
	}
}

char* getNumeSoferMasinaScumpa(listaDubla lista) {
	if (!lista.first) return NULL; 

	nod* temp = lista.first;
	nod* max = lista.first;

	while (temp) {
		if (temp->info.pret > max->info.pret) {
			max = temp;
		}
		temp = temp->next;
	}

	char* numeSofer = malloc(strlen(max->info.numeSofer) + 1);
	strcpy(numeSofer, max->info.numeSofer);

	return numeSofer;
}

int main() {
	listaDubla LD = citireLDMasiniDinFisier("masini2.txt");

	printf("Afisare de la inceput:\n");
	afisareListaMasiniDeLaInceput(LD);
	printf("\n");
	printf("Afisare de la sfarsit:\n");
	afisareListaMasiniDeLaSfarsit(LD);

	printf("Pretul mediu: %f\nSofermax: %s", calculeazaPretMediu(LD), getNumeSoferMasinaScumpa(LD));
	stergeMasinaDupaID(&LD, 3); 
	printf("Dupa stergere:\n");
	afisareListaMasiniDeLaInceput(LD);
	dezalocareLDMasini(&LD);
	return 0;
}