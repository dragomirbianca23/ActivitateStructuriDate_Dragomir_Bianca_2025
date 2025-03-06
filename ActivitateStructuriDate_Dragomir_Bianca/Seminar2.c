#include<stdio.h>
#include<malloc.h>

struct Masina {
	int id;
	int nrKm;
	char* model;
	float pret;
	char tipMotor;
};

struct Masina initializare(int id, int nrKm, const char* model, float pret, char tipMotor) {
	struct Masina m;
	m.id = id;
	m.nrKm = nrKm;
	m.model = (char*)malloc(strlen(model) + 1);
	strcpy(m.model, model);
	m.pret = pret;
	m.tipMotor = tipMotor;
	return m;
}

void afisare(struct Masina m) {
	printf("Id: %d, nr km: %d, model: %s, pret: %f, tip motor: %c", m.id, m.nrKm, m.model, m.pret, m.tipMotor);

}

void afisareVector(struct Masina* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++)
	{
		afisare(vector[i]);
		printf("\n");
	}
}

struct Masina* copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int* nrElementeCopiate) {
	struct Masina* vectorNou;
	vectorNou = NULL;
	if (vector != NULL && (*nrElementeCopiate) > 0)
	{


		if (nrElemente < *nrElementeCopiate)
		{
			*nrElementeCopiate = nrElemente;
		}
		vectorNou = malloc(*nrElementeCopiate * sizeof(struct Masina));
		for (int i = 0; i < *nrElementeCopiate; i++)
		{
			vectorNou[i] = initializare(vector[i].id, vector[i].nrKm, vector[i].model, vector[i].pret, vector[i].tipMotor);
		}
	}
	return vectorNou;
}

void dezalocare(struct Masina** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++)
	{
		if ((*vector)[i].model != NULL)
		{
			free((*vector)[i].model); // deferentiere + deplasare + deferentiere + accesare
		}
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;

}

void copiazaMasiniIeftine(struct Masina* vector, char nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++)
	{
		if (vector[i].pret < prag)
		{
			(*dimensiune) += 1;
		}
	}

	*vectorNou = malloc((*dimensiune) * sizeof(struct Masina));
	int k = 0;
	for (int i = 0; i < nrElemente; i++)
	{
		if (vector[i].pret < prag)
		{
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].model = malloc(sizeof(char) * (strlen(vector[i].model) + 1));
			strcpy_s((*vectorNou)[k].model, (strlen(vector[i].model) + 1), vector[i].model);
			k += 1;
		}
	}
}

struct Masina getPrimulElementConditionat(struct Masina* vector, int nrElemente, const char* modelCautat) {
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(vector[i].model, modelCautat) == 0) {
			return vector[i];
		}
	}
	struct Masina m = { 0, 0, "", 0.0, ' ' };
	return m;
}



int main() {
	int nrMasini = 3;
	struct Masina* vector;
	vector = malloc(sizeof(struct Masina) * nrMasini);
	vector[0] = initializare(1, 100000, "Logan", 1020.4, 'D');
	vector[1] = initializare(2, 200000, "Passat", 2500.4, 'B');
	vector[2] = initializare(3, 3000, "Cyber Truck", 1000.0, 'E');

	afisareVector(vector, 3);

	int nrElementeCopiate = 5;
	struct Masina* vectorNou = copiazaPrimeleNElemente(vector, nrMasini, &nrElementeCopiate);
	printf("Elemente copiate:\n");
	afisareVector(vectorNou, nrElementeCopiate);

	dezalocare(&vectorNou, &nrElementeCopiate);

	printf("Masini ieftine:\n");
	copiazaMasiniIeftine(vector, nrMasini, 1500, &vectorNou, &nrElementeCopiate);
	afisareVector(vectorNou, nrElementeCopiate);

	dezalocare(&vectorNou, &nrElementeCopiate);

	printf("\nCautare model 'Logan':\n");
	struct Masina m = getPrimulElementConditionat(vector, nrMasini, "Logan");
	afisare(m);
	dezalocare(&vector, &nrMasini);

	return 0;
}

