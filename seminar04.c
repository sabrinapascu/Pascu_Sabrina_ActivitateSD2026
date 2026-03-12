
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

//creare structura pentru un nod dintr-o lista simplu inlantuita
typedef struct Nod Nod;

struct Nod{
	Masina info;
	Nod* next;
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

void afisareListaMasini(Nod* lista) {
	while (lista!=NULL) {
		afisareMasina(lista->info);
		lista=lista->next;
	}
}

void adaugaMasinaInLista(Nod** lista,  Masina masinaNoua) {
	Nod * nou=malloc(sizeof(Nod));
	nou->info=masinaNoua; //shallow copy
	nou->next=NULL;
	Nod* aux=*lista;
	if ((*lista)!=NULL) {
		while (aux->next!=NULL) {
			aux=aux->next;
		}
		aux->next=nou;
	}
	else {
		*lista=nou;
	}
}

void adaugaLaInceputInLista(Nod** lista, Masina masinaNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
	Nod* nou=malloc(sizeof(Nod));
	nou->info=masinaNoua;
	nou->next=*lista;
	*lista=nou;
}

//void* -pointer la orice, returneaza un pointer (este diferit de void)

void* citireListaMasiniDinFisier(const char* numeFisier) {
	FILE *f=fopen(numeFisier, "r");
	Nod* lista=NULL;
	while (!feof(f)) {
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
	}

	fclose(f);
	return lista;
}

void dezalocareListaMasini(Nod** lista) {
	//sunt dezalocate toate masinile si lista de elemente
	Nod* head=*lista;
	Nod* next=NULL;
	while (head!=NULL) {
		next=head->next;
		free(head->info.model);
		free(head->info.numeSofer);
		free(head);
		head=next;
	}
	*lista=NULL;
}

float calculeazaPretMediu(Nod* lista) {
	float suma=0;
	int nrMasini;
	while (lista!=NULL) {
		suma+=lista->info.pret;
		nrMasini++;
		lista=lista->next;
	}
	if (nrMasini>0)
		return suma/nrMasini;
	else return 0;
}

void stergeMasiniDinSeria(Nod** lista, char serieCautata) {
	if (*lista==NULL) return;
	Nod* actual=*lista;
	Nod* anterior=NULL;
	while (actual!=NULL) {
		if (actual->info.serie==serieCautata) {
			Nod* deSters=actual;
			if (anterior==NULL) {
				*lista=actual->next;
				actual=*lista;
			}
			else {
				anterior->next=actual->next;
				actual=anterior->next;
			}
			free(deSters->info.model);
			free(deSters->info.numeSofer);
			free(deSters);
		}
		else {
			anterior=actual;
			actual=actual->next;
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	float sum=0;
	while (lista!=NULL) {
		if (strcmp(lista->info.numeSofer, numeSofer)==0) {
			sum+=lista->info.pret;
		}
		lista=lista->next;
	}
	return sum;
}

int main() {

	Nod* lista=citireListaMasiniDinFisier("masini.txt");
	//afisareListaMasini(lista);
	FILE* f=fopen("masini.txt", "r");
	if (f) {
		adaugaLaInceputInLista(&lista, citireMasinaDinFisier(f));
		afisareListaMasini(lista);
	}
	else printf("eroare");

	float pretMEdiu=calculeazaPretMediu(lista);
	printf("\nPret mediu: %3.2f", pretMEdiu);

	stergeMasiniDinSeria(&lista, 'B');
	//afisareListaMasini(lista);
	 float suma=calculeazaPretulMasinilorUnuiSofer(lista, "Gigel");
	printf("\npretul masinilor soferului este: %3.2f", suma);

	dezalocareListaMasini(&lista);
	afisareListaMasini(lista);

	return 0;
}