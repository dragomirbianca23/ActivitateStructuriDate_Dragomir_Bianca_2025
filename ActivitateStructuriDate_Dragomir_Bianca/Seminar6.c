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

typedef struct Nod Nod;

struct Nod 
{
	Masina info;
	Nod* next;
};


struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

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

void afisareListaMasini(Nod* cap) {
	while (cap != NULL)
	{
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod**cap, Masina masinaNoua) {
	Nod* temp = (Nod*)malloc(sizeof(Nod));
	temp->info = masinaNoua; //shallow copy
	temp->next = NULL;

	if ((*cap) == NULL)
	{
		(*cap) = temp;
	}
	else
	{
		Nod* aux = *cap;
		while ((aux->next))
		{
			aux = aux->next;
		}
		aux->next = temp;

	}
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++)
	{
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* numeSofer, int dimensiune) {
	unsigned int suma = 0;
	if (dimensiune != 0 && dimensiune < 0)
	{
		return -1;
	}
	for (int i = 0; i < strlen(numeSofer); i++)
	{
		suma += (int)numeSofer[i];
	}
	suma %= dimensiune;
	return suma;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	//cheia este numele Soferului
	int pozitie = calculeazaHash(masina.numeSofer, hash.dim);
	//verificam daca avem coliziune
	if (hash.vector[pozitie] != NULL)
	{
		//nu avem coliziune
		adaugaMasinaInLista(&(hash.vector[pozitie]), masina);
	}
	else
	{
		//avem coliziune
		adaugaMasinaInLista(&(hash.vector[pozitie]), masina);
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dimensiune) {
	FILE* f = fopen(numeFisier, "r");
	HashTable hash = initializareHashTable(dimensiune);
	while (!feof(f))
	{
		Masina masina = citireMasinaDinFisier(f);
		inserareMasinaInTabela(hash, masina);
	}
	fclose(f);
	return hash;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++)
	{
		printf("Suma lantului %d\n", i + 1);
		afisareListaMasini(ht.vector[i]);
	}

}

void dezalocareListaMasini(Nod** cap)
{
	Nod* p = (*cap);
	while (p)
	{
		Nod* aux = p;
		p = p->next;
		if (aux->info.model)
		{
			free(aux->info.model);
		}
		if (aux->info.numeSofer)
		{
			free(aux->info.numeSofer);
		}
		free(aux);
	}
	*cap = NULL;
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	for (int i = 0; i < ht->dim; i++)
	{
		dezalocareListaMasini(&(ht->vector[i]));
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

float calculeazaMedieLista(Nod* cap)
{
	float sum = 0;
	int contor = 0;
	while (cap)
	{
		sum += cap->info.pret;
		contor++;
		cap = cap->next;
	}
	if (contor != 0)
	{
		return sum / contor;
	}
	else
		return -1;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	float* preturi = 0; 
	*nrClustere = 0; 
	//aflam cate clustere sunt ocupate
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			(*nrClustere)++;
		}
	}
	preturi = (float*)malloc(sizeof(float) * (*nrClustere));
	int contor = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			preturi[contor] = calculeazaMedieLista(ht.vector[i]);
			contor++;
		}
	}
	return preturi;
}

Masina getMasinaDupaCheie(HashTable ht, const char* numeSofer) {
	Masina m;
	m.id = -1;
	int pozitie = calculeazaHash(numeSofer, ht.dim);
	if (pozitie <= 0 || pozitie >= ht.dim || ht.vector[pozitie]==NULL)
	{
		return m;
	}
	Nod* aux = ht.vector[pozitie];
	while (aux != NULL && strcmp(aux->info.numeSofer, numeSofer) != 0)
	{
		aux = aux->next;
	}
	if (aux != NULL)
	{
		m.numeSofer = (char*)malloc(strlen(numeSofer) + 1);
		strcpy(m.numeSofer, numeSofer);
		m.model = (char*)malloc(strlen(aux->info.model) + 1);
		strcpy(m.model, aux->info.model);
		m.id = aux->info.id;
		m.pret = aux->info.pret;
		m.nrUsi = aux->info.nrUsi;
		m.serie = aux->info.serie;
		;
	}
	return m;
}

int main() {
	HashTable ht;
	ht = citireMasiniDinFisier("masini2.txt", 5);
	afisareTabelaDeMasini(ht);
	printf("Masina lui Ionescu este: ");
	Masina IonescuMasina = getMasinaDupaCheie(ht, "Ionescu");
	if (IonescuMasina.id != -1)
	{
		afisareMasina(IonescuMasina);
		free(IonescuMasina.model);
		free(IonescuMasina.numeSofer);
	}
	else
	{
		printf("Nu am gasit masina");
	}

	int nrClustere = 0;
	float *preturi = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	printf("Preturile medii per clustere sunt:\n");
	for (int i = 0; i < nrClustere; i++)
	{
		printf("%.2f  ", preturi[i]);
	}
	dezalocareTabelaDeMasini(&ht);
	return 0;

}