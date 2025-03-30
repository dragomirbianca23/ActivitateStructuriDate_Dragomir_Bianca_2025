#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct BoutiqueDaruri {
    int cod;
    char* denumire;
    int nrProduse;
    float* preturi;
};
typedef struct BoutiqueDaruri BoutiqueDaruri;

void afisareBoutiqueDaruri(BoutiqueDaruri bd) {
    printf("Cod: %d\n", bd.cod);
    printf("Denumire: %s\n", bd.denumire);
    printf("Numar Produse: %d\n", bd.nrProduse);
    printf("Preturi:\n");
    for (int i = 0; i < bd.nrProduse; i++) {
        printf("Produs %d: %.2f\n", i + 1, bd.preturi[i]);
    }
}

BoutiqueDaruri citireDeLaTastatura() {
    BoutiqueDaruri bd;
    bd.denumire = (char*)malloc(100 * sizeof(char));
    printf("Introduceti codul: ");
    scanf("%d", &bd.cod);
    printf("Introduceti denumirea magazinului: ");
    scanf(" %[^\n]%*c", bd.denumire);
    printf("Introduceti numarul de produse: ");
    scanf("%d", &bd.nrProduse);
    bd.preturi = (float*)malloc(bd.nrProduse * sizeof(float));
    for (int i = 0; i < bd.nrProduse; i++) {
        printf("Introduceti pretul pentru produsul %d: ", i + 1);
        scanf("%f", &bd.preturi[i]);
    }

    return bd;
}

float calculeazaPretMediu(BoutiqueDaruri bd)
{
    float suma = 0;
    for (int i = 0; i < bd.nrProduse; i++)
    {
        suma += bd.preturi[i];
    }
    return suma / bd.nrProduse;
}

void modificaPret(BoutiqueDaruri* bd, int nrProdus, float noulPret)
{
    if (nrProdus > 0 && noulPret > 0)
    {
        bd->preturi[nrProdus] = noulPret;
        printf("Noul pret pentru produsul %d este %5.2f: \n", nrProdus + 1, noulPret);
    }
}

int main() {
    BoutiqueDaruri bd = citireDeLaTastatura();
    afisareBoutiqueDaruri(bd);
    float f = calculeazaPretMediu(bd);
    printf("Pret mediu:", f);
    modificaPret(&bd, 1, 200);
    afisareBoutiqueDaruri(bd);

    return 0;
}