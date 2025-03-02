#include<stdio.h>
#include<stdlib.h>

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
	strcpy_s(m.model, strlen(model) + 1, model);
	m.pret = pret;
	m.tipMotor = tipMotor;

	return m;
}

void afisare(struct Masina m) {
	printf("Id: %d,\nnrKm: %d,\nmodel: %s,\npret: %f,\ntipMotor: %c\n ", m.id, m.nrKm, m.model, m.pret, m.tipMotor);
}

void modifica_Atribut(struct Masina* m, float noulPret) {
	if (noulPret > 0)
	{
		m->pret = noulPret;
	}
}

void dezalocare(struct Masina* m) {
	free(m->model);
	m->model = NULL;

}

int main() {
	struct Masina masina;
	masina = initializare(1, 50000, "Logan", 3000.50, 'B');
	afisare(masina);
	modifica_Atribut(&masina, 4000);
	afisare(masina);
	dezalocare(&masina);
	afisare(masina);

	return 0;
}