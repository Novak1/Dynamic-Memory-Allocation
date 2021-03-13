#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_IP 21

typedef struct numera_st {
	char naziv[MAX_IP];
	char ime[MAX_IP];
	char prezime[MAX_IP];
	float trajanje;
	int godina;
}numera;

typedef struct cvor_st {
	numera inf;
	struct cvor_st *levi;
	struct cvor_st *desni;
}BCVOR;

void init(BCVOR**);
FILE* otvori_fajl(char*, char*, int);
void ucitajIzFajla(FILE*, BCVOR**);
void dodaj(BCVOR**, numera);
int uporedi(numera, numera);
void ispisi(BCVOR*);
void ispisTrajanje(BCVOR*, float);
void najkracaGodina(BCVOR*, int, BCVOR**, int*);
void ispisCD(FILE*, BCVOR*, float*);
void obrisiStablo(BCVOR**);
void brisanjeCvora(BCVOR**, int);
BCVOR* najmanji(BCVOR*);

int main(int brArg, char* arg[]) {
	BCVOR* koren;
	int pronasao = 0;
	float ukupnoTrajanje = 0.0;
	
	if(brArg != 4) {
		printf("Greska prilikom poziva programa!\n");
		exit(1);
	}

	char* ulazna_datoteka = arg[1];
	float zadatoTrajanje = atof(arg[2]);
	int zadataGodina = atoi(arg[3]);
	
	init(&koren);
	
	FILE* ulaz = otvori_fajl(ulazna_datoteka, "r", 2);
	ucitajIzFajla(ulaz, &koren);
	
	ispisi(koren);
	
	printf("\nNumere koje traju duze od %.2f sekundi su:\n", zadatoTrajanje);
	ispisTrajanje(koren, zadatoTrajanje);
	
	BCVOR* najkraca = NULL;
	
	najkracaGodina(koren, zadataGodina, &najkraca, &pronasao);//menja vrednost u main-u
	
	char izlazna_datoteka[MAX_IP+4];
	
	strcpy(izlazna_datoteka, najkraca->inf.naziv);
	strcat(izlazna_datoteka, ".txt");
	
	FILE* izlaz = otvori_fajl(izlazna_datoteka, "w", 3);
	fprintf(izlaz, "%s %s %s %.2f %d\n", najkraca->inf.naziv, najkraca->inf.ime, najkraca->inf.prezime, 
													 najkraca->inf.trajanje, najkraca->inf.godina);
													 
	FILE* izlaz2 = otvori_fajl("izlaz.txt", "w", 4);
	ispisCD(izlaz2, koren, &ukupnoTrajanje);
	
	brisanjeCvora(&koren, 2010);
	printf("\nIspis nakon brisanja:\n");
	ispisi(koren);
	
	obrisiStablo(&koren);
	
	fclose(ulaz);
	fclose(izlaz);
	fclose(izlaz2);

	return 0;
}

void init(BCVOR** koren) {
	*koren = NULL;
}

FILE* otvori_fajl(char* naziv_fajla, char* rezim, int status_greske) {
	FILE* f = fopen(naziv_fajla, rezim);
	
	if(f == NULL) {
		printf("Greska prilikom otvaranja datoteke %s!\n", naziv_fajla);
		exit(status_greske);
	}
	
	return f;
}

void ucitajIzFajla(FILE* ulaz, BCVOR** koren) {
	numera x;
	
	while(fscanf(ulaz, "%s %s %s %f %d", x.naziv, x.ime, x.prezime, &x.trajanje, &x.godina) != EOF) {
		dodaj(koren, x);
	}
}

void dodaj(BCVOR** koren, numera x) {
	if(*koren == NULL) {
		BCVOR* novi = (BCVOR*)malloc(sizeof(BCVOR));
		if(novi == NULL) {
			printf("Nema dovoljno slobodne memorije!\n");
			exit(22);
		}
		novi->levi = NULL;
		novi->desni = NULL;
		strcpy(novi->inf.naziv, x.naziv);
		strcpy(novi->inf.ime, x.ime);
		strcpy(novi->inf.prezime, x.prezime);
		novi->inf.trajanje = x.trajanje;
		novi->inf.godina = x.godina;
		*koren = novi;
	}else{
		if(uporedi(x, (*koren)->inf)) {
			dodaj(&(*koren)->desni, x);
		}else{
			dodaj(&(*koren)->levi, x);
		}
	}
}

int uporedi(numera a, numera b) {
	return a.trajanje > b.trajanje;
}

void ispisi(BCVOR* koren) {
	if(koren != NULL) {
		ispisi(koren->levi);
		printf("%s %s %s %.2f %d\n", koren->inf.naziv, koren->inf.ime, koren->inf.prezime, koren->inf.trajanje, koren->inf.godina);
		ispisi(koren->desni);
	}
}

void ispisTrajanje(BCVOR* koren, float zadatoTrajanje) {
	if(koren != NULL) {
		ispisTrajanje(koren->levi, zadatoTrajanje);
		if(koren->inf.trajanje*60 > zadatoTrajanje) {
			printf("%s %s %s %.2f %d\n", koren->inf.naziv, koren->inf.ime, koren->inf.prezime, koren->inf.trajanje, koren->inf.godina);
		}
		ispisTrajanje(koren->desni, zadatoTrajanje);
	}
}

void najkracaGodina(BCVOR* koren, int zadataGodina, BCVOR** najkraca, int* pronasao) {
	if(koren != NULL && (*pronasao) == 0) {
		najkracaGodina(koren->levi, zadataGodina, najkraca, pronasao);
		if(koren->inf.godina == zadataGodina && (*pronasao) == 0) {
			(*pronasao)++;
			(*najkraca) = koren;
		}
		najkracaGodina(koren->desni, zadataGodina, najkraca, pronasao);
	}
}

void ispisCD(FILE* izlaz2, BCVOR* koren, float* ukupnoTrajanje) {
	if(koren != NULL && (*ukupnoTrajanje) + koren->inf.trajanje <= 20) {
		ispisCD(izlaz2, koren->desni, ukupnoTrajanje);
		if((*ukupnoTrajanje) + koren->inf.trajanje <= 20) {
			(*ukupnoTrajanje) += koren->inf.trajanje;
			fprintf(izlaz2, "%s %s %s %.2f %d\n", koren->inf.naziv, koren->inf.ime, koren->inf.prezime, 
															  koren->inf.trajanje, koren->inf.godina);
		}
		ispisCD(izlaz2, koren->levi, ukupnoTrajanje);
	}
}

void obrisiStablo(BCVOR** koren) {
	if(*koren != NULL) {
		obrisiStablo(&(*koren)->levi);
		obrisiStablo(&(*koren)->desni);
		free(*koren);
		*koren = NULL;
	}
}

void brisanjeCvora(BCVOR** koren, int zadataGodina) {
	if(*koren == NULL) {
		return;
	}else if((*koren)->inf.godina == zadataGodina){
		if((*koren)->levi == NULL && (*koren)->desni == NULL) {
			free(*koren);
			*koren = NULL;
		}else if((*koren)->levi == NULL ^ (*koren)->desni == NULL){
			BCVOR* tmp = *koren;
			
			if((*koren)->levi == NULL) {
				*koren = tmp->desni;
			}else{
				*koren - tmp->levi;
				
				tmp->levi = NULL;
				tmp->desni = NULL;
				free(tmp);
			}
		}else{
			BCVOR* min = najmanji((*koren)->desni);
			
			strcpy((*koren)->inf.naziv, min->inf.naziv);
			strcpy((*koren)->inf.ime, min->inf.ime);
			strcpy((*koren)->inf.ime, min->inf.prezime);
			(*koren)->inf.trajanje = min->inf.trajanje;
			(*koren)->inf.godina = min->inf.godina;
			
			brisanjeCvora(&(*koren)->desni, min->inf.godina);
		}
	}else{
		((*koren)->inf.godina > zadataGodina) ? brisanjeCvora(&(*koren)->levi, zadataGodina) : brisanjeCvora(&(*koren)->desni, zadataGodina);
	}
}

BCVOR* najmanji(BCVOR* koren) {
	if(koren == NULL) {
		return NULL;
	}
	
	BCVOR* levi = najmanji(koren->levi);
	
	return (levi != NULL) ? levi : koren;
}
