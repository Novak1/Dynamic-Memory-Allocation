#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_IP 21
#define MAX_D 4

typedef struct teniser_st {
	char ime[MAX_IP];
	char prezime[MAX_IP];
	int poeni;
	char drzava[MAX_D];
	int godine;
}teniser;

typedef struct cvor_st {
	teniser inf;
	struct cvor_st *levi;
	struct cvor_st *desni;
}BCVOR;

void init(BCVOR**);
FILE* otvori_fajl(char*, char*, int);
void ucitajIzFajla(FILE*, BCVOR**);
void dodaj(BCVOR**, teniser);
int uporedi(teniser, teniser);
void ispisi(BCVOR*);
void ispisFajl(FILE*, BCVOR*);
void najstarijiDrzava(FILE*, BCVOR*, char*, int*);
int zbirBodova(BCVOR*, int*);
void obrisiStablo(BCVOR**);

int main(int brArg, char* arg[]) {
	BCVOR* koren;
	int pronasao = 0;
	int brojac = 0;
	
	if(brArg != 4) {
		printf("Greska prilikom poziva programa!\n");
		exit(1);
	}
	
	char* ulazna_datoteka = arg[1];
	char* izlazna_datoteka = arg[2];
	char* zadataDrzava = arg[3];
	
	init(&koren);
	
	FILE* ulaz = otvori_fajl(ulazna_datoteka, "r", 2);
	ucitajIzFajla(ulaz, &koren);
	
	ispisi(koren);
	
	FILE* izlaz = otvori_fajl(izlazna_datoteka, "w", 3);
	ispisFajl(izlaz, koren);
	
	char drzavaFajl[5] = {'o', 'l', 'd', '_', '\0'};
	char pomocna[MAX_D+4];
	
	strcpy(pomocna, zadataDrzava);
	strcat(pomocna, ".txt");
	strcat(drzavaFajl, pomocna);
	
	FILE* izlaz2 = otvori_fajl(drzavaFajl, "w", 4);
	najstarijiDrzava(izlaz2, koren, zadataDrzava, &pronasao);
	
	printf("\nProsecna starost tenisera sa osvojenih vise od 5000 poena je: %.2f\n\n", (float)zbirBodova(koren, &brojac)/(float)brojac);
																																														
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
	teniser x;
	
	while(fscanf(ulaz, "%s %s %d %s %d", x.ime, x.prezime, &x.poeni, x.drzava, &x.godine) != EOF) {
		dodaj(koren, x);
	}
}

void dodaj(BCVOR** koren, teniser x) {
	if(*koren == NULL) {
		BCVOR* novi = (BCVOR*)malloc(sizeof(BCVOR));
		if(novi == NULL) {
			printf("Nema dovoljno slobodne memorije.\n");
			exit(22);
		}
		novi->levi = NULL;
		novi->desni = NULL;
		strcpy(novi->inf.ime, x.ime);
		strcpy(novi->inf.prezime, x.prezime);
		novi->inf.poeni = x.poeni;
		strcpy(novi->inf.drzava, x.drzava);
		novi->inf.godine = x.godine;
		*koren = novi;
	}else{
		if(uporedi(x, (*koren)->inf)) {
			dodaj(&(*koren)->desni, x);
		}else{
			dodaj(&(*koren)->levi, x);
		}
	}
}

int uporedi(teniser a, teniser b) {
	return a.godine > b.godine;
}

void ispisi(BCVOR* koren) {
	if(koren != NULL) {
		ispisi(koren->levi);
		printf("%s %s %d %s %d\n", koren->inf.ime, koren->inf.prezime, koren->inf.poeni, koren->inf.drzava, koren->inf.godine);
		ispisi(koren->desni);
	}
}

void ispisFajl(FILE* izlaz, BCVOR* koren) {
	if(koren != NULL) {
		ispisFajl(izlaz, koren->levi);
		if(koren->inf.godine < 28 && koren->inf.poeni > 5000) {
			fprintf(izlaz, "%s %s %d %s %d\n", koren->inf.ime, koren->inf.prezime, koren->inf.poeni, koren->inf.drzava, koren->inf.godine);
		}
		ispisFajl(izlaz, koren->desni);
	}
}

void najstarijiDrzava(FILE* izlaz2, BCVOR* koren, char* zadataDrzava, int* pronasao) {
	if(koren != NULL) {
		najstarijiDrzava(izlaz2, koren->desni, zadataDrzava, pronasao);
		if(strcmp(koren->inf.drzava, zadataDrzava) == 0 && *pronasao == 0) {
			fprintf(izlaz2, "%s %s %d %s %d\n\n", koren->inf.ime, koren->inf.prezime, koren->inf.poeni, koren->inf.drzava, koren->inf.godine);
			(*pronasao)++;
		}
		najstarijiDrzava(izlaz2, koren->levi, zadataDrzava, pronasao);
	}
}

int zbirBodova(BCVOR* koren, int* brojac) {
	if(koren == NULL) {
		return 0;
	}else{
		int l = zbirBodova(koren->levi, brojac);
		int d = zbirBodova(koren->desni, brojac);
		if(koren->inf.poeni > 5000) {
			(*brojac)++;
			return l+d+koren->inf.godine;
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
