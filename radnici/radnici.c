#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_IP 21
#define MAX_Z 31

typedef struct radnik_st {
	char ime[MAX_IP];
	char prezime[MAX_IP];
	float plata;
	char zanimanje[MAX_Z];
}radnik;

typedef struct cvor_st {
	radnik inf;
	struct cvor_st *levi;
	struct cvor_st *desni;
}BCVOR;

void init(BCVOR**);
FILE* otvori_fajl(char*, char*, int);
void ucitajIzFajla(FILE*, BCVOR**);
void dodaj(BCVOR**, radnik);
int uporedi(radnik, radnik);
void ispisi(BCVOR*);
void ispisNovePlate(FILE*, BCVOR*);
float zbirPlata(BCVOR*);
float zbirNovihPlata(BCVOR*);
float prosecnaPlata(BCVOR*, char*, int*);
void obrisiStablo(BCVOR**);

int main(int brArg, char* arg[]) {
	BCVOR* koren;
	int brojac = 0;
	
	if(brArg != 3) {
		printf("Greska prilikom poziva programa!\n");
		exit(1);
	}
	
	char* ulazna_datoteka = arg[1];
	char* izlazna_datoteka = arg[2];
	
	init(&koren);
	
	FILE* ulaz = otvori_fajl(ulazna_datoteka, "r", 2);
	ucitajIzFajla(ulaz, &koren);
	
	ispisi(koren);
	
	FILE* izlaz = otvori_fajl(izlazna_datoteka, "w", 3);
	ispisNovePlate(izlaz, koren);
	
	printf("\nUsteda na godisnjem nivou je: %.2f\n\n", 12*0.1*zbirPlata(koren)); // moze se i od pocetnog zbira oduzeti zbir nakon snizenja pa pomnoziti sa 12
	
	char zadatoZanimanje[MAX_Z];
	
	printf("Unesite zanimanje: ");
	scanf("%s", zadatoZanimanje);
	
	char zanimanjeFajl[MAX_Z+4];
	
	strcpy(zanimanjeFajl, zadatoZanimanje);
	strcat(zanimanjeFajl, ".txt");
	
	FILE* izlaz2 = otvori_fajl(zanimanjeFajl, "w", 4);
	
	fprintf(izlaz2, "%.2f", prosecnaPlata(koren, zadatoZanimanje, &brojac)/(float)brojac);
	
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
	radnik x;
	
	while(fscanf(ulaz, "%s %s %f %s", x.ime, x.prezime, &x.plata, x.zanimanje) != EOF) {
		dodaj(koren, x);
	}
}

void dodaj(BCVOR** koren, radnik x) {
	if(*koren == NULL) {
		BCVOR* novi = (BCVOR*)malloc(sizeof(BCVOR));
		if(novi == NULL) {
			printf("Nema dovoljno memorije!\n");
			exit(22);
		}
		novi->levi = NULL;
		novi->desni = NULL;
		strcpy(novi->inf.ime, x.ime);
		strcpy(novi->inf.prezime, x.prezime);
		novi->inf.plata = x.plata;
		strcpy(novi->inf.zanimanje, x.zanimanje);
		*koren = novi;
	}else{
		if(uporedi(x, (*koren)->inf)) {
			dodaj(&(*koren)->desni, x);
		}else{
			dodaj(&(*koren)->levi, x);
		}
	}
}

int uporedi(radnik a, radnik b) {
	return strcmp(a.prezime, b.prezime) > 0;
}

void ispisi(BCVOR* koren) {
	if(koren != NULL) {
		ispisi(koren->levi);
		printf("%s %s %.2f %s\n", koren->inf.ime, koren->inf.prezime, koren->inf.plata, koren->inf.zanimanje);
		ispisi(koren->desni);
	}
}

void ispisNovePlate(FILE* izlaz, BCVOR* koren) {
	if(koren != NULL) {
		ispisNovePlate(izlaz, koren->levi);
		fprintf(izlaz, "%s %s %.2f %s\n", koren->inf.ime, koren->inf.prezime, koren->inf.plata*0.9, koren->inf.zanimanje);
		ispisNovePlate(izlaz, koren->desni);
	}
}

float zbirPlata(BCVOR* koren) {
	if(koren == NULL) {
		return 0;
	}else{
		float l = zbirPlata(koren->levi);
		float d = zbirPlata(koren->desni);
		return l+d+koren->inf.plata;
	}
}

/*float zbirNovihPlata(BCVOR* koren) {
	if(koren == NULL) {
		return 0;
	}else{
		float l = zbirNovihPlata(koren->levi);
		float d = zbirNovihPlata(koren->desni);
		return l+d+(koren->inf.plata*0.9);
	}
}*/

float prosecnaPlata(BCVOR* koren, char* zadatoZanimanje, int* brojac) {
	if(koren == NULL) {
		return 0;
	}else{
		float l = prosecnaPlata(koren->levi, zadatoZanimanje, brojac);
		float d = prosecnaPlata(koren->desni, zadatoZanimanje, brojac);
		if(strcmp(koren->inf.zanimanje, zadatoZanimanje) == 0) {
			(*brojac)++;
			return l+d+koren->inf.plata*0.9;
		}else{
			return l+d;
		}
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
