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

typedef struct NodLS NodLS;
typedef struct NodLP NodLP;
struct NodLS {
	NodLP* nodPrincipal;
	NodLS* next;
};

struct NodLP {
	Masina info;
	NodLS* vecini;
	NodLP* next;
};

void inserareInLP(NodLP** cap, Masina masina)
{
	NodLP* nou = (NodLP*)malloc(sizeof(NodLP));
	nou->vecini = NULL;
	nou->info = masina;
	nou->next = NULL;
	if ((*cap) == NULL)
	{
		(*cap) = nou;
	}
	else
	{
		NodLP* temp = (*cap);
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserareLS(NodLS** cap, NodLP* nodPrincipal)
{
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->nodPrincipal = nodPrincipal;
	nou->next = *cap;
	(*cap) = nou;
}

NodLP* cautaNodDupaID(NodLP* listaPrincipala, int id) {
	while (listaPrincipala && listaPrincipala->info.id != id)
	{
		listaPrincipala=listaPrincipala->next;
	}
	return listaPrincipala;
}

void inserareMuchie(NodLP* listaPrincipala, int idStart, int idStop) {
	NodLP* nodStart = cautaNodDupaID(listaPrincipala, idStart);
	NodLP* nodStop = cautaNodDupaID(listaPrincipala, idStop);

	if (nodStart != NULL && nodStop != NULL)
	{
		inserareLS(&(nodStart->vecini), nodStop);  
		inserareLS(&(nodStart->vecini), nodStart); // doar pt. nod neorientat
	}
}


void* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodLP* cap = NULL;
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		inserareInLP(&cap, m);
	}
	fclose(f);
	return cap;
}

void citireMuchiiDinFisier(NodLP* cap, const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste 
	//toate id-urile de start si stop pentru fiecare muchie
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	int idStart, idStop;
	while (!feof(f))
	{
		fscanf(f, "%d %d", &idStart, &idStop);
		inserareMuchie(cap, idStart, idStop);
	}
	fclose(f);
}

NodLP* citesteGraf(const char* numeFisierMasini, const char* numeFisierMuchii)
{
	NodLP* graf = citireNoduriMasiniDinFisier(numeFisierMasini);
	citireMuchiiDinFisier(graf, numeFisierMuchii);
	return graf;
}

void dezalocareListaAdiacentaLS(NodLS* listaPrincipala)
{
	while (listaPrincipala != NULL)
	{
		NodLS* temp = listaPrincipala->next;
		free(listaPrincipala);
		listaPrincipala = temp;
	}
}

void dezalocareNoduriGraf(NodLP** listaPrincipala) {
	while (*listaPrincipala)
	{
		NodLP* temp = (*listaPrincipala)->next;
		dezalocareListaAdiacentaLS((*listaPrincipala)->vecini);
		free((*listaPrincipala)->info.model);
		free((*listaPrincipala)->info.numeSofer);
		(*listaPrincipala)->vecini = NULL;
		free(*listaPrincipala);
		(listaPrincipala) = temp;

	}
}

int main() {
	NodLP* graf = citesteGraf("masini.txt", "muchii.txt");
	dezalocareNoduriGraf(&graf);

	return 0;
}