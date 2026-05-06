//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
#include <sys/syslimits.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;
typedef struct NodArbore NodArbore;

struct NodArbore {
	Masina info;
	NodArbore* left;
	NodArbore* right;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

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

int calculeazaInaltimeArbore(NodArbore* root) {
//	//calculeaza inaltimea arborelui care este data de
//	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if (root) {
		return fmax(calculeazaInaltimeArbore(root->left), calculeazaInaltimeArbore(root->right))+1;
	}
	else {
		return 0;
	}
}

char calculGE(NodArbore* root) {
	if (root) {
		return calculeazaInaltimeArbore(root->left)-calculeazaInaltimeArbore(root->right);
	}
	else return 0;
}

void rotireStanga(NodArbore** root) {
	NodArbore* aux=(*root)->right;
	(*root)->right=aux->left;
	aux->left=(*root);
	(*root)=aux;
}

void rotireDreapta(NodArbore** root) {
	NodArbore* aux=(*root)->left;
	(*root)->left=aux->right;
	aux->right=(*root);
	(*root)=aux;
}

void adaugaMasinaInArboreEchilibrat(NodArbore** root, Masina masinaNoua) {
	if (*root) {
		if (masinaNoua.id<(*root)->info.id) {
			adaugaMasinaInArboreEchilibrat(&(*root)->left, masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&(*root)->right, masinaNoua);
		}
	}
	else{
		NodArbore* nou=malloc(sizeof(NodArbore));
		nou->info=masinaNoua;
		nou->left=NULL;
		nou->right=NULL;
		(*root)=nou;
	}
	int grad=calculGE(*root);
	if (grad==2) {
		if (calculGE((*root)->left)==-1) {
			rotireStanga(&(*root)->left);
			rotireDreapta(root);
		}
		else {
			rotireDreapta(root);
		}
	}
	if (grad==-2) {
		if (calculGE((*root)->right)==1) {
			rotireDreapta(&(*root)->right);
		}
		rotireStanga(root);
	}

}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f=fopen(numeFisier, "r");
	NodArbore* root=NULL;
	while (!feof(f)) {
		Masina masina=citireMasinaDinFisier(f);
		adaugaMasinaInArboreEchilibrat(&root, masina);
	}
	fclose(f);
	return root;
}

void afisareMasiniDinArbore(NodArbore* root) {
	if (root) {
		afisareMasina(root->info);
		afisareMasiniDinArbore((*root).left);
		afisareMasiniDinArbore((*root).right);
	}
}

void dezalocareArboreDeMasini(NodArbore** root) {
	if (*root) {
		dezalocareArboreDeMasini(&(*root)->left);
		dezalocareArboreDeMasini(&(*root)->right);
		free((*root)->info.numeSofer);
		free((*root)->info.model);
		free(*root);
	}
	root=NULL;
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(/*arborele de masini*/int id);

int determinaNumarNoduri(/*arborele de masini*/);

float calculeazaPretTotal(/*arbore de masini*/);

float calculeazaPretulMasinilorUnuiSofer(NodArbore* root, const char* numeSofer) {
	float pret=0;
	if (root) {
		pret+=calculeazaPretulMasinilorUnuiSofer(root->left, numeSofer);
		pret+=calculeazaPretulMasinilorUnuiSofer(root->right, numeSofer);
		if (strcmp(root->info.numeSofer, numeSofer)==0) {
			pret+=root->info.pret;
		}
	}
	return pret;
}

int main() {

	NodArbore* arbore=citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(arbore);
	float pret=calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu");
	printf("pretul masinilor soferului ionescu este: %3.2f", pret);
	dezalocareArboreDeMasini(&arbore);
	return 0;
}