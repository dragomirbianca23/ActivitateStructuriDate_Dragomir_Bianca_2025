//1. Sa se construiasca si sa se afiseze o lista simpla cu n noduri continand informatii despre studentii unei facultati.
//Informatiile aferente nodurilor sunt citite dintr - un fisier text, iar fiecare student ar trebui sa fie identificat prin numar matricol(int), nume(char*), medie(float), precum si alte campuri la alegere.
//2. Sa se dezaloce toate structurile de date dinamice construite.Operatia de dezalocare va fi insotita de afisarea continutului structurilor in fisier text, inainte de dezalocarea memoriei heap.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student
{
	int nrMatricol;
	char* nume;
	float medie;
	int an;
};
typedef struct Student Student;

typedef struct Nod Nod;
struct Nod
{
	Student info;
	struct Nod* next;
};

void adaugaLaInceputInLista(Nod** cap, Student studentNou)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = studentNou;
	nou->next = (*cap);
	(*cap) = nou;
}

void adaugaLaFinalInLista(Nod** cap, Student studentNou)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = studentNou;
	nou->next = NULL;
	if (*cap)
	{
		Nod* p = *cap;
		while (p->next)
		{
			p = p->next;
		}
		p->next = nou;
	}
	else
	{
		*cap = nou;
	}
}

Student citireStudentDinFisier(FILE* f)
{
	char buffer[50];
	char sep[3] = ",\n";
	fgets(buffer, 50, f);
	char* aux;
	Student s;
	s.nrMatricol = atoi(strtok(buffer, sep));
	aux = strtok(NULL, sep);
	s.nume = (char*)malloc(strlen(aux) + 1);
	strcpy(s.nume, aux);
	s.medie = atof(strtok(NULL, sep));
	s.an = atoi(strtok(NULL, sep));
	return s;
}

Nod* citireStudentiFisier(const char* numeFisier)
{
	Nod* cap = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			adaugaLaFinalInLista(&cap, citireStudentDinFisier(f));
		}
	}
	fclose(f);
	return cap;
}

void afisareStudenti(Student student)
{
	printf("Nr. matricol:%d\n", student.nrMatricol);
	printf("Nume:%s\n", student.nume);
	printf("Medie:%.2f\n", student.medie);
	printf("An:%d\n\n", student.an);
}

void afisareListaStudenti(Nod* cap)
{
	while (cap)
	{
		afisareStudenti(cap->info);
		cap = cap->next;
	}
}

float mediaStudentilor(Nod* cap)
{
	float suma = 0;
	int contor = 0;
	while (cap)
	{
		suma += cap->info.medie;
		contor++;
		cap = cap->next;
	}
	if (contor > 0)
	{
		return suma / contor;
	}
	else
		return -1;
}

void scrieStudentiInFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "w");
	if (!f)
	{
		printf("Nu am putut deschide fisierul.");
	}
	int numarStudenti;
	printf("Introduceti numarul de studenti.");
	scanf("%d", &numarStudenti);
	for (int i = 0; i < numarStudenti; i++)
	{
		Student s;
		char buffer[100];
		printf("Introduceti numarul matricol");
		scanf("%d", &s.nrMatricol);
		printf("Introduceti numele: ");
		scanf(" %[^\n]", buffer); 
		s.nume = (char*)malloc(strlen(buffer) + 1);
		strcpy(s.nume, buffer);
		printf("Introduceti media:");
		scanf("%f", &s.medie);
		printf("Introuduceti anul: ");
		scanf("%d", &s.an);

		fprintf(f, "%d,%s,%.2f,%d\n", s.nrMatricol, s.nume, s.medie, s.an);
		free(s.nume);
	}
	fclose(f);
}


void dezalocareLista(Nod** cap)
{
	while (*cap)
	{
		Nod* p = *cap;
		(*cap) = p->next;
		if (p->info.nume)
		{
			free(p->info.nume);
		}
		free(p);
	}
}

int main()
{
	Nod* cap = citireStudentiFisier("Studenti.txt");
	afisareListaStudenti(cap);
	printf("Media mediei fiecareui student este: %.2f", mediaStudentilor(cap));
    scrieStudentiInFisier("StudentiNou.txt");
	dezalocareLista(&cap);

	return 0;
}