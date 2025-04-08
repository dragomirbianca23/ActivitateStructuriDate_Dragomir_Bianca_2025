#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Cladire {
    int id;
    int anConstructie;
    char* adresa;
    float suprafata;
};
typedef struct Cladire Cladire;

typedef struct Nod Nod;
struct Nod
{
    Nod* next;
    Cladire info;
};

struct HashTable
{
    int dim;
    Nod** vector;
};
typedef struct HashTable HashTable;

int calculeazaHash(int an, int dim)
{
    return an % dim;
}

HashTable initiealizeazaHashTable(int dim)
{
    HashTable ht;
    ht.dim = dim;
    ht.vector = (Nod**)malloc(sizeof(Nod*) * dim);
    for (int i = 0; i < ht.dim; i++)
    {
        ht.vector[i] = NULL;
    }
    return ht;
}

void inserare(HashTable* ht, Cladire cladireNoua)
{
    int poz = calculeazaHash(cladireNoua.anConstructie, ht->dim);
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = cladireNoua;
    nou->next = ht->vector[poz];
    ht->vector[poz] = nou;
}

//2.
void afisareClustereAn(HashTable ht, int an)
{
    int poz = calculeazaHash(an, ht.dim);
    Nod* p = ht.vector[poz];
    while (p)
    {
        if (p->info.anConstructie == an)
        {
            printf("ID: %d, Adresa: %s, Suprafata: %.2f\n", p->info.id, p->info.adresa, p->info.suprafata);
        }
        p = p->next;
    }
}

Nod* stergeNod(Nod* cap, int id, int an)
{
    while (cap && cap->info.id == id && cap->info.anConstructie == an)
    {
        Nod* temp = cap;
        cap = cap->next;
        free(temp->info.adresa);
        free(temp);
    }

    Nod* p = cap;
    while (p && p->next)
    {
        if (p->next->info.id == id && p->next->info.anConstructie == an)
        {
            Nod* temp = p->next;
            p->next = temp->next;
            free(temp->info.adresa);
            free(temp);
        }
        else
        {
            p = p->next;
        }
    }

    return cap;
}
//3
void stergeCladire(HashTable* ht, int an, int id)
{
    int poz = calculeazaHash(an, ht->dim);
    if (poz >= 0 && poz < ht->dim)
    {
        stergeNod(&(ht->vector[poz]), id, an);
    }
}

//4
Nod* stergeNodId(Nod* cap, int id)
{
    while (cap && cap->info.id == id)
    {
        Nod* temp = cap;
        cap = cap->next;
        free(temp->info.adresa);
        free(temp);
    }
    Nod* p = cap;
    while (p && p->next)
    {
        if (p->next->info.id == id)
        {
            Nod* temp = p->next;
            p->next = temp->next;
            free(temp->info.adresa);
            free(temp);
        }
        else
        {
            p = p->next;
        }
    }
    return cap;
}

void stergeCladireId(HashTable* ht, int id)
{
    for (int i = 0; i < ht->dim; i++)
    {
        ht->vector[i] = stergeNodId(ht->vector[i], id);
    }
}
//diferenta intre 3 si 4 este ca atunci cand stergem dupa id trebuie sa parcurgem tot vectorul
//iar atunci cand cautam si dupa an si id, deoarece cu anul construirii calculam hash-ul, parcurgem doar lista asociata

Cladire creareCladire(int id, int anConstructie, const char* adresa) {
    Cladire cladire;
    cladire.id = id;
    cladire.anConstructie = anConstructie;
    cladire.adresa = (char*)malloc(strlen(adresa) + 1);
    strcpy(cladire.adresa, adresa);
    return cladire;
}

Nod* creareNod(Cladire info) {
    Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->info = info;
    nodNou->next = NULL;
    return nodNou;
}

void inserareCladireInTabela(HashTable* ht, Cladire cladire) {
    int poz = calculeazaHash(cladire.anConstructie, ht->dim);
    Nod* nodNou = creareNod(cladire);

    if (ht->vector[poz] == NULL) {
        ht->vector[poz] = nodNou;
    }
    else {
        nodNou->next = ht->vector[poz];
        ht->vector[poz] = nodNou;
    }
}

void afisareListaCladiri(Nod* cap) {
    while (cap) {
        printf("ID: %d\n", cap->info.id);
        printf("Anul constructiei: %d\n", cap->info.anConstructie);
        printf("Adresa: %s\n", cap->info.adresa);
        cap = cap->next;
    }
}

void afisareTabelaDeCladiri(HashTable ht) {
    for (int i = 0; i < ht.dim; i++) {
        if (ht.vector[i] != NULL) {
            printf("Clusterul %d:\n", i);
            afisareListaCladiri(ht.vector[i]);
        }
    }
}
int main()
{
    HashTable ht = initiealizeazaHashTable(10);

    // Creem cateva cladiri si le inseram in tabela
    Cladire cladire1 = creareCladire(1, 1990, "Strada 1");
    Cladire cladire2 = creareCladire(2, 2000, "Strada 2");
    Cladire cladire3 = creareCladire(3, 1990, "Strada 3");
    Cladire cladire4 = creareCladire(4, 2010, "Strada 4");

    inserareCladireInTabela(&ht, cladire1);
    inserareCladireInTabela(&ht, cladire2);
    inserareCladireInTabela(&ht, cladire3);
    inserareCladireInTabela(&ht, cladire4);

    printf("Tabela de clădiri:\n");
    afisareTabelaDeCladiri(ht);

    printf("\nStergem cladirea cu ID 3 si Anul 1990\n");
    stergeCladire(&ht, 3, 1990);

    printf("\nTabela dupa stergere:\n");
    afisareTabelaDeCladiri(ht);

    printf("\nStergem cladirea cu ID 2\n");
    stergeCladireId(&ht, 2);

    printf("\nTabela dupa stergere:\n");
    afisareTabelaDeCladiri(ht);

    for (int i = 0; i < ht.dim; i++) {
        Nod* p = ht.vector[i];
        while (p) {
            Nod* temp = p;
            p = p->next;
            free(temp->info.adresa);
            free(temp);
        }
    }
    free(ht.vector);

    return 0;
}