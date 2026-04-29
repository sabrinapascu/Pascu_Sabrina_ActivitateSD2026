//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-un arbore binar de cautare
typedef struct Nod Nod;

struct Nod {
	Nod* stanga;
	Nod* dreapta;
	Masina info;
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


void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	if ((*radacina)==NULL) {
		Nod* nou=malloc(sizeof(Nod));
		nou->dreapta=NULL;
		nou->stanga=NULL;
		nou->info=masinaNoua;
		(*radacina)=nou;
		return;
	}
	if (masinaNoua.id<(*radacina)->info.id) {
		adaugaMasinaInArbore(&(*radacina)->stanga, masinaNoua);
	}
	else {
		adaugaMasinaInArbore(&(*radacina)->dreapta, masinaNoua);
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f=fopen(numeFisier, "r");
	Nod* radacina=NULL;
	while (!feof(f)) {
		adaugaMasinaInArbore(&radacina, citireMasinaDinFisier(f));
	}
	fclose(f);
	return radacina;
}

void afisareMasiniDinArbore(Nod* radacina) {
	if (radacina!=NULL) {
		afisareMasiniDinArbore(radacina->stanga);
		afisareMasina(radacina->info);
		afisareMasiniDinArbore(radacina->dreapta);
	}
}

void afisarePostordine(Nod* radacina) {
	if (radacina) {
		afisareMasiniDinArbore(radacina->stanga);
		afisareMasiniDinArbore(radacina->dreapta);
		afisareMasina(radacina->info);
	}
}

void dezalocareArboreDeMasini(Nod** radacina) {
	//sunt dezalocate toate masinile si arborele de elemente
	if ((*radacina)) {
		dezalocareArboreDeMasini(&(*radacina)->stanga);
		dezalocareArboreDeMasini(&(*radacina)->dreapta);

		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free((*radacina));
		*radacina=NULL;
	}
}

Masina getMasinaByID(Nod* radacina, int id) {
	Masina m;
	if (radacina) {
		if (radacina->info.id==id) {
			m=radacina->info;
			m.model=malloc(sizeof(char)*(strlen(radacina->info.model)+1));
			strcpy(m.model, radacina->info.model);

			m.numeSofer=malloc(sizeof(char)*(strlen(radacina->info.numeSofer)+1));
			strcpy(m.numeSofer, radacina->info.numeSofer);

			return m;
		}
		else {
			if (radacina->info.id<=id) {
				m=getMasinaByID(radacina->dreapta, id);
			}
			else {
				m=getMasinaByID(radacina->stanga, id);
			}
		}
	}
	else {
		m.id=-1;
	}
	return m;
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina) {
		int nrNoduriSt=determinaNumarNoduri(radacina->stanga);
		int nrNoduriDr=determinaNumarNoduri(radacina->dreapta);

		return 1+nrNoduriSt+nrNoduriDr;
	}
	else {
		return 0;
	}
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina==NULL) {
		return -1;
	}
	else {
		return 1+fmax(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta));
	}
	return 0;
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina==NULL) return 0;
	else {
		float prStanga=calculeazaPretTotal(radacina->stanga);
		float prDreapta=calculeazaPretTotal(radacina->dreapta);

		return radacina->info.pret+prDreapta+prStanga;
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	if (radacina==NULL) return 0;
	float pret=0;
		if (strcmp(radacina->info.numeSofer, numeSofer)==0) {
			pret=radacina->info.pret;
		}
	return pret+calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer)+
		calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);
}

int main() {

	//FILE* f=fopen("masini_arbore.txt", "r");
	Nod* arbore=citireArboreDeMasiniDinFisier("masini_arbore.txt");

	afisareMasiniDinArbore(arbore);
	afisareMasina(getMasinaByID(arbore,2));
	printf("nr total de noduri: %d", determinaNumarNoduri(arbore));
	printf("\ninaltime arbore %d", calculeazaInaltimeArbore(arbore));
	printf("\npret total: %3.2f", calculeazaPretTotal(arbore));
	printf("\npretul masinilor soferului Ionescu: %3.2f", calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu"));

	dezalocareArboreDeMasini(&arbore);

	return 0;
}
