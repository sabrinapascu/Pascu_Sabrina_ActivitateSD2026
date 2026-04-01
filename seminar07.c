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

//creare structura pentru un nod dintr-o lista simplu inlantuita
typedef struct Nod Nod;
struct Nod {
	Masina masina;
	Nod* next;
};

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste
struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
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
		afisareMasina((*lista).masina);
		lista=lista->next;
	}
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
	Nod* nou=malloc(sizeof(Nod));
	nou->masina=masinaNoua;
	nou->next=NULL;

	if (!(*lista)) {
		*lista=nou;
	}
	else {
		Nod* aux=*lista;
		while (aux->next) {
			aux=aux->next;
		}
		aux->next=nou;
	}
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim=dimensiune;
	ht.vector=malloc(sizeof(Nod*)*ht.dim);
	for (int i=0; i<ht.dim; i++) {
		ht.vector[i]=NULL;
	}
	return ht;
}

int calculeazaHash(int id,  int dimensiune) {
	// este calculat hash-ul in functie de dimensiunea tabelei si un atribut al masinii
	return (id*31)%dimensiune;
}

void inserareMasinaInTabela(HashTable ht, Masina masina) {
	//este folosit mecanismul CHAINING
	//este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
	int hash=calculeazaHash(masina.id, ht.dim);

	if (ht.vector[hash]!=NULL) {
		adaugaMasinaInLista(&(ht.vector[hash]), masina);
		return;
	}
	Nod* nod=malloc(sizeof(Nod));
	nod->masina=masina;
	nod->next=NULL;

	ht.vector[hash]=nod;
}

HashTable citireMasiniDinFisier(const char* numeFisier) {
	FILE* f=fopen(numeFisier, "r");
	HashTable ht=initializareHashTable(5);
	while (!feof(f)) {
		Masina masina=citireMasinaDinFisier(f);
		inserareMasinaInTabela(ht, masina);
	}
	fclose(f);
	return ht;
}

void afisareTabelaDeMasini(HashTable ht) {
	//sunt afisate toate masinile cu evidentierea clusterelor realizate
	for (int i=0; i<ht.dim; i++) {
		if (ht.vector[i]!=NULL) {
			printf("cluster: %d\n", i+1);
			Nod*aux=ht.vector[i];
			afisareListaMasini(aux);
			printf("\n----------------------");
		}
	}
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	for (int i=0; i<ht->dim; i++) {
		if (ht->vector[i]!=NULL) {
			Nod *p=ht->vector[i];
			Nod* next=NULL;

			while (p!=NULL) {
				free(p->masina.numeSofer);
				free(p->masina.model);

				next=p->next;
				free(p);
				p=next;
			}
		}
	}
	free(ht->vector);
	ht->dim=0;
	ht->vector=NULL;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	int nr=0;
	for (int i=0; i<ht.dim; i++) {
		if (ht.vector[i]!=NULL) {
			nr++;
		}
	}
	float* mediiPerCluster=malloc(nr*sizeof(float));
	int j=0;
	for (int i=0; i<ht.dim; i++) {
		if (ht.vector[i]!=NULL) {
			int nrNoduri=0;
			float sum=0;
			Nod* aux=ht.vector[i];
			while (aux!=NULL) {
				nrNoduri++;
				sum+=aux->masina.pret;
				aux=aux->next;
			}

			mediiPerCluster[j++]=sum/nrNoduri;

		}
	}
	*nrClustere=nr;
	return mediiPerCluster;
}

Masina getMasinaById(HashTable ht, int id) {
	Masina m;
	m.id=-1;
	int hash=calculeazaHash(id, ht.dim);
	Nod* aux=ht.vector[hash];
	while (aux!=NULL) {
		if (aux->masina.id==id) {
			m=aux->masina;
			m.model=malloc(sizeof(char)*(strlen(aux->masina.model)+1));
			strcpy(m.model, aux->masina.model);
			m.numeSofer=malloc(sizeof(char)*(strlen(aux->masina.numeSofer)+1));
			strcpy(m.numeSofer, aux->masina.numeSofer);

			return m;
		}
		aux=aux->next;
	}
	return m;
}

int main() {

	HashTable ht=citireMasiniDinFisier("masini.txt");
	afisareTabelaDeMasini(ht);
	afisareMasina(getMasinaById(ht, 3));

	int nrClustere=0;
	float* medii=calculeazaPreturiMediiPerClustere(ht, &nrClustere);

	for (int i=0; i<nrClustere; i++) {
		printf("%.2f\n",medii[i] );
	}

	dezalocareTabelaDeMasini(&ht);

	return 0;
}
