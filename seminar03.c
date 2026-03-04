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

void afisareMasina(Masina masina) {
	printf("\n Id masina: %d", masina.id);
	printf("\n Nr usi: %d ", masina.nrUsi);
	printf("\n pret: %f ", masina.pret);
	printf("\n model:%s", masina.model);
	printf("\n nume sofer: %s ", masina.numeSofer);
	printf("\n serie: %c ", masina.serie);
	printf("\n");
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i=0; i<nrMasini; i++) {
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int * nrMasini, Masina masinaNoua) {
	(*nrMasini)++;
	Masina* aux=malloc(sizeof(Masina)*(*nrMasini));
	for (int i=0; i<(*nrMasini)-1; i++) {
		aux[i]=(*masini)[i]; //shallow copy
	}
	aux[(*nrMasini)-1]=masinaNoua;
	free(*masini);
	*masini= aux;
}

Masina citireMasinaFisier(FILE* file) {
	char linie[50];
	fgets(linie,  50, file);
	char delimitator[3]=",\n";
	Masina masina;
	masina.id=atoi(strtok(linie, delimitator));
	masina.nrUsi=atoi(strtok(NULL, delimitator));
	masina.pret=atof(strtok(NULL, delimitator));

	char* model=strtok(NULL, delimitator);
	masina.model=malloc((strlen(model)+1)*sizeof(char));
	strcpy(masina.model, model);
	//!!stergem cu free doar elementele pentru care am dat malloc
	char* numeSofer=strtok(NULL, delimitator);
	masina.numeSofer=malloc((strlen(numeSofer)+1)*sizeof(char));
	strcpy(masina.numeSofer, numeSofer);

	char *serie=strtok(NULL, delimitator);
	masina.serie=serie[0];

	return masina;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* file=fopen(numeFisier, "r");
	(*nrMasiniCitite)=0;
	Masina* masini=NULL;
	while (!feof(file)) {
		adaugaMasinaInVector(&masini, nrMasiniCitite, citireMasinaFisier(file));
	}
	fclose(file);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i=0; i<*nrMasini; i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);
	*vector=NULL;
	*nrMasini=0;
}

int main() {

	Masina* masini;
	int nr=0;
	masini=citireVectorMasiniFisier("masini.txt", &nr);
	afisareVectorMasini(masini, nr);

	dezalocareVectorMasini(&masini, &nr);
	afisareVectorMasini(masini, nr);
	return 0;
}
