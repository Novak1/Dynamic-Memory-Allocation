#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_IP 21
#define MAX_P 11

typedef struct student_st {
	char ime[MAX_IP];
	char prezime[MAX_IP];
	unsigned bodovi1;
	unsigned bodovi2;
	char predmet[MAX_P];
}student;

typedef struct cvor_st {
	student inf;
	struct cvor_st *levi;
	struct cvor_st *desni;
}BCVOR;

void init(BCVOR**);
FILE* otvori_fajl(char*, char*, int);
void ucitajIzFajla(FILE*, BCVOR**);
void dodaj(BCVOR**, student);
int uporedi(student, student);
void ispisi(BCVOR*);
void ispisFajl(FILE*, BCVOR*);
void najvecaApsolutRazlika(BCVOR*, int*);
void ispisNajvecaRazlika(BCVOR*, int);
int predmetBodovi(BCVOR*, char*);
void obrisiStablo(BCVOR**);

int main(int brArg, char* arg[]) {
	BCVOR* koren;
	int razlika = 0;

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
	ispisFajl(izlaz, koren);
	
	najvecaApsolutRazlika(koren, &razlika);
	//printf("%d\n\n", razlika);  //provera funkcije
	printf("\n\n");
	ispisNajvecaRazlika(koren, razlika);
	
	char zadatiPredmet[MAX_P];
	
	printf("Unesite predmet: ");
	scanf("%s", zadatiPredmet);
	
	char predmetFajl[MAX_P+4];
	
	strcpy(predmetFajl, zadatiPredmet);
	strcat(predmetFajl, ".txt");
	
	FILE* izlaz2 = otvori_fajl(predmetFajl, "w", 4);
	
	fprintf(izlaz2, "%d", predmetBodovi(koren, zadatiPredmet));
	
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
	student x;
	
	while(fscanf(ulaz, "%s %s %u %u %s", x.ime, x.prezime, &x.bodovi1, &x.bodovi2, x.predmet) != EOF) {
		dodaj(koren, x);
	}
}

void dodaj(BCVOR** koren, student x) {
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
		novi->inf.bodovi1 = x.bodovi1;
		novi->inf.bodovi2 = x.bodovi2;
		strcpy(novi->inf.predmet, x.predmet);
		*koren = novi;
	}else{
		if(uporedi(x, (*koren)->inf)) {
			dodaj(&(*koren)->desni, x);
		}else{
			dodaj(&(*koren)->levi, x);
		}
	}
}

int uporedi(student a, student b) {
	return abs(a.bodovi1 - a.bodovi2) > abs(b.bodovi1 - b.bodovi2); //odmah ih slozim po apsolutnim vrednostima razlike, zbog pod b
}

void ispisi(BCVOR* koren) {
	if(koren != NULL) {
		ispisi(koren->levi);
		printf("%s %s %u %u %s\n", koren->inf.ime, koren->inf.prezime, koren->inf.bodovi1, koren->inf.bodovi2, koren->inf.predmet);
		ispisi(koren->desni);
	}
}

void ispisFajl(FILE* izlaz, BCVOR* koren) {
	if(koren != NULL) {
		ispisFajl(izlaz, koren->levi);
		fprintf(izlaz, "%s %s %u %u %s %u\n", koren->inf.ime, koren->inf.prezime, koren->inf.bodovi1, koren->inf.bodovi2, 
														  koren->inf.predmet, koren->inf.bodovi1 + koren->inf.bodovi2);
		ispisFajl(izlaz, koren->desni);
	}
}

void najvecaApsolutRazlika(BCVOR* koren, int* razlika) { //sracunamo vrednost najvece apsolutne razlike(menjamo vrednost u main-u)
	if(koren != NULL) {
		najvecaApsolutRazlika(koren->desni, razlika);
		if(abs(koren->inf.bodovi1 - koren->inf.bodovi2) > (*razlika)) {
			*razlika = abs(koren->inf.bodovi1 - koren->inf.bodovi2);
		}
		najvecaApsolutRazlika(koren->levi, razlika);
	}
}

void ispisNajvecaRazlika(BCVOR* koren, int razlika) { //krenem od desnog coska i stampam one cija je apsolutna razlika jednaka najvecoj
	if(koren != NULL) {
		ispisNajvecaRazlika(koren->desni, razlika);
		if(abs(koren->inf.bodovi1 - koren->inf.bodovi2) == razlika) {
			printf("%s %s %u %u %s\n", koren->inf.ime, koren->inf.prezime, koren->inf.bodovi1, koren->inf.bodovi2, koren->inf.predmet);
		}
		ispisNajvecaRazlika(koren->levi, razlika);
	}		
}

int predmetBodovi(BCVOR* koren, char* zadatiPredmet) {
	if(koren == NULL) {
		return 0;
	}else{
		int l = predmetBodovi(koren->levi, zadatiPredmet);
		int d = predmetBodovi(koren->desni, zadatiPredmet);
		if(strcmp(koren->inf.predmet, zadatiPredmet) == 0 && koren->inf.bodovi1 + koren->inf.bodovi2 >= 30) {
			return l+d+1;
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
