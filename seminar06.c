//sablon 10
//stive/cozi
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
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
	if (masina.id==-1) {
		printf("masina nu exista");
		return;
	}
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

//STACK
typedef struct NodSimplu NodSimplu;
struct NodSimplu {
	Masina info;
	NodSimplu* next;
};

void pushStack(NodSimplu** stack, Masina masina) {
	NodSimplu* nou=malloc(sizeof(NodSimplu));
	nou->info=masina;
	nou->next=*stack;
	*stack=nou;
}

Masina popStack(NodSimplu** stack) {
	Masina masina;
	masina.id=-1;
	if (*stack!=NULL) {
		NodSimplu* aux=*stack;
		*stack=(*stack)->next;
		masina=aux->info;
		free(aux);
	}
	return masina;
}

char emptyStack(NodSimplu* stack) {
	return stack!=NULL; //return !stack
}

NodSimplu* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* f=fopen(numeFisier, "r");
	NodSimplu* stack=NULL;
	while (!feof(f)) {
		pushStack(&stack, citireMasinaDinFisier(f));
	}
	fclose(f);
	return stack;
}

void dezalocareStivaDeMasini(/*stiva*/) {
	//sunt dezalocate toate masinile si stiva de elemente
}

int size(/*stiva*/) {
	//returneaza numarul de elemente din stiva
}

//QUEUE
typedef struct Nod Nod;
typedef struct Queue Queue;
struct Nod {
	Masina masina;
	Nod* next;
	Nod* prev;
};

struct Queue {
	Nod* start;
	Nod* end;
};

void enqueue(Queue* queue, Masina masina) {
	Nod* nod=malloc(sizeof(Nod));
	nod->masina=masina;
	nod->next=queue->start;
	nod->prev=NULL;
	if (queue->start!=NULL) {
		queue->start->prev=nod;
	}
	else {
		queue->end=nod;
	}
	queue->start=nod;
}

Masina dequeue(Queue* queue) {
	Masina masina=queue->end->masina;
	if (queue->start==NULL) {
		masina.id=-1;
		return masina;
	}

	if (queue->start==queue->end) { //if(queue->end->prev==NULL)
		free(queue->start);

		queue->start=NULL;
		queue->end=NULL;
	}
	else {
		queue->end=queue->end->prev;
		free(queue->end->next);
		queue->end->next=NULL;
	}

	return masina;
}

Queue citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	Queue queue;
	queue.start=NULL;
	queue.end=NULL;

	FILE* f=fopen(numeFisier, "r");
	while (!feof(f)) {
		Masina masina=citireMasinaDinFisier(f);
		enqueue(&queue, masina);
	}
	fclose(f);
	return queue;
}

void dezalocareCoadaDeMasini(/*coada*/) {
	//sunt dezalocate toate masinile si coada de elemente
}


//metode de procesare
Masina getMasinaByID(Queue* queue, int id) {
	Queue aux;
	aux.end=NULL;
	aux.start=NULL;

	Masina rezultat;
	while (queue->start!=NULL) {
		Masina masina=dequeue(queue);

		if (masina.id==id) {
			rezultat=masina;
			rezultat.model=malloc(sizeof(char)*(strlen(masina.model)+1));
			strcpy(rezultat.model, masina.model);
			rezultat.numeSofer=malloc(sizeof(char)*(strlen(masina.numeSofer)+1));
			strcpy(rezultat.numeSofer, masina.numeSofer);
		}

		enqueue(&aux, masina);
	}
	queue->start=aux.start;

	return rezultat;

}

float calculeazaPretTotal(NodSimplu** stack) {
	NodSimplu* aux=NULL;
	float sum=0;
	while (!emptyStack(*stack)) {
		Masina masina=popStack(stack);
		sum+=masina.pret;
		pushStack(&aux, masina);
	}

	while (aux!=NULL) {
		pushStack(stack, popStack(&aux));
	}
}

int main() {

	Queue queue=citireCoadaDeMasiniDinFisier("masini.txt");
	afisareMasina(dequeue(&queue));
	afisareMasina(dequeue(&queue));
	Masina masina=getMasinaByID(&queue, 7);
	afisareMasina(masina);
	free(masina.model);
	free(masina.numeSofer);

	return 0;
}
