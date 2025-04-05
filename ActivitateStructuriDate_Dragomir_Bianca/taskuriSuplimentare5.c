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

typedef struct Nod {
    Masina info;
    struct Nod* next;
} Nod;

Nod* creareNod(Masina masina)
{
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masina;
    nou->next = NULL;
    return nou;
}

void stergereNodPozitie(Nod** cap, int pozitie)
{
    if ((*cap) == NULL)
    {
        printf("Pozitia este goala.");
        return;
    }

    Nod* p = (*cap);

    if (pozitie == 0)
    {
        *cap = p->next;
        free(p->info.model);
        free(p->info.numeSofer);
        free(p);
        return;
    }

    int index = 0;
    while (p != NULL && index < pozitie - 1)
    {
        p = p->next;
        index++;
    }
    if (p == NULL || p->next == NULL) {
        printf("Pozitia %d este invalida. Lista are mai putine noduri.\n", pozitie);
        return;
    }

    Nod* nodDeSters = p->next;

    p->next = nodDeSters->next;

    free(nodDeSters->info.model);
    free(nodDeSters->info.numeSofer);
    free(nodDeSters);
}

void sorteazaListaMasini(Nod** cap)
{
    if ((*cap) == NULL || (*cap)->next == NULL)
    {
        return;
    }

    int verificare = 1;
    Nod* p = (*cap);

    while (verificare)
    {
        verificare = 0;
        while (p->info.pret > p->next->info.pret)
        {
            Masina tempMasina = p->info;
            p->info = p->next->info;
            p->next->info = tempMasina;
            verificare = 1;
        }
        p = p->next;
    }
}

void inserareOrdonataDupaPret(Nod** cap, Masina masinaNoua)
{
    Nod* nou = creareNod(masinaNoua);

    if ((*cap) == NULL || (*cap)->info.pret > masinaNoua.pret)
    {
        nou->next = (*cap);
        *cap = nou;
        return;
    }

    Nod* p = *cap;
    while (p->next && p->next->info.pret < masinaNoua.pret)
    {
        p = p->next;
    }
    nou->next = p->next;
    p->next = nou;
}

void interschimbareNoduri(Nod** cap, int poz1, int poz2)
{
    Nod* nod1 = (*cap);
    Nod* nod2 = (*cap);
    
    int index1 = 0;
    int index2 = 0;
    if (!nod1 || !nod2) {
        printf("Pozitii invalide.\n");
    }
    else
    {
        while (nod1 && index1 < poz1)
        {
            nod1 = nod1->next;
            index1++;
        }
        while (nod2 && index2 < poz2) {
            nod2 = nod2->next;
            index2++;
        }
    }
    
    Masina aux = nod1->info;
    nod1->info = nod2->info;
    nod2->info = aux;
}

int main()
{
    Nod* lista = NULL;

    Masina m1 = { 1, 4, 15000, "BMW", "Gigel", 'A' };
    Masina m2 = { 2, 2, 10000, "Audi", "Ion", 'B' };
    Masina m3 = { 3, 5, 20000, "Mercedes", "Vasile", 'C' };

    adaugaMasinaInLista(&lista, m1);
    adaugaMasinaInLista(&lista, m2);
    adaugaMasinaInLista(&lista, m3);

    printf("Lista inainte de interschimbare:\n");
    afisareListaMasini(lista);

    // Interschimbăm două poziții (ex: 0 și 2)
    interschimbareNoduri(lista, 0, 2);

    printf("\nLista dupa interschimbare:\n");
    afisareListaMasini(lista);

    return 0;
}