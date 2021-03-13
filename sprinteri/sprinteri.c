#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_IP 21
#define MAX_D 4

typedef struct sprinter_st {
	char ime[MAX_IP];
	char prezime[MAX_IP];
	char drzava[MAX_D];
	float vreme;
	int godine;
}sprinter;

typedef struct cvor_st {
	sprinter inf;
	struct cvor_st *levi;
	struct cvor_st *desni;
}BCVOR;

void init(BCVOR**);
FILE* otvori_fajl(char*, char*, int);
void ucitajIzFajla(FILE*, BCVOR**);
void dodaj(BCVOR**, sprinter);
int uporedi(sprinter, sprinter);
void ispisi(BCVOR*);
void ispisVreme(BCVOR*, float);
BCVOR* najbrziNajmladji(BCVOR*, char*, int*);
float zbirVremena(BCVOR*);
int brojSprintera(BCVOR*);
void najmanjaRazlika(BCVOR*, float*, float);
void ispisNajmanjaRazlika(FILE*, BCVOR*, float, float);
void obrisiStablo(BCVOR**);

int main(int brArg, char* arg[]) {
	BCVOR* koren;
	int pronasao = 0;
	float razlika = 1000.0;
	
	if(brArg != 4) {
		printf("Greska prilikom poziva programa!\n");
		exit(1);
	}
	
	char* ulazna_datoteka = arg[1];
	float zadatoVreme = atof(arg[2]);
	char* zadataDrzava = arg[3];
	
	
	init(&koren);
	
	FILE* ulaz = otvori_fajl(ulazna_datoteka, "r", 2);
	ucitajIzFajla(ulaz, &koren);
	
	ispisi(koren);
	
	printf("\nSprinteri koji mogu da zavrse trku za manje od %.2fs su:\n", zadatoVreme);
	ispisVreme(koren, zadatoVreme);
	
	BCVOR* najNaj = najbrziNajmladji(koren, zadataDrzava, &pronasao);
	
	//printf("\n%s %s %s %.2f %d\n", najNaj->inf.ime, najNaj->inf.prezime, najNaj->inf.drzava, najNaj->inf.vreme, najNaj->inf.godine);
	
	char izlazna_datoteka[MAX_IP*2+4];
	char pomocna[MAX_IP];
	
	strcpy(izlazna_datoteka, najNaj->inf.ime);
	strcat(izlazna_datoteka, najNaj->inf.prezime);
	strcat(izlazna_datoteka, ".txt");

	FILE* izlaz = otvori_fajl(izlazna_datoteka, "w", 3);
	fprintf(izlaz, "%s %s %s %.2f %d\n", najNaj->inf.ime, najNaj->inf.prezime, najNaj->inf.drzava, najNaj->inf.vreme, najNaj->inf.godine);
	
	FILE* izlaz2 = otvori_fajl("izlaz.txt", "w", 4);
	
	float prosecnoVreme = zbirVremena(koren)/(float)brojSprintera(koren);
	//printf("%.2f\n", prosecnoVreme); //provera
	najmanjaRazlika(koren, &razlika, prosecnoVreme);
	//printf("%.2f\n", razlika); //provera
	
	ispisNajmanjaRazlika(izlaz2, koren, razlika, prosecnoVreme);
	
	obrisiStablo(&koren);
	
	fclose(ulaz); // nemoj zaboraviti konju
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
	sprinter x;
	
	while(fscanf(ulaz, "%s %s %s %f %d", x.ime, x.prezime, x.drzava, &x.vreme, &x.godine) != EOF) {
		dodaj(koren, x);
	}
}

void dodaj(BCVOR** koren, sprinter x) {
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
		strcpy(novi->inf.drzava, x.drzava);
		novi->inf.vreme = x.vreme;
		novi->inf.godine = x.godine;
		*koren = novi;
	}else{
		if(uporedi(x, (*koren)->inf)){
			dodaj(&(*koren)->desni, x);
		}else{
			dodaj(&(*koren)->levi, x);
		}
	}
}

int uporedi(sprinter a, sprinter b) { //sortirano po vremenu, ako je vreme jednako, prednost ima mladji
	if(a.vreme == b.vreme) {
		return a.godine > b.godine;
	}else{
		return a.vreme > b.vreme;
	}
}

void ispisi(BCVOR* koren) {
	if(koren != NULL) {
		ispisi(koren->levi);
		printf("%s %s %s %.2f %d\n", koren->inf.ime, koren->inf.prezime, koren->inf.drzava, koren->inf.vreme, koren->inf.godine);
		ispisi(koren->desni);
	}
}

void ispisVreme(BCVOR* koren, float zadatoVreme) {
	if(koren != NULL) {
		ispisVreme(koren->levi, zadatoVreme);
		if(koren->inf.vreme < zadatoVreme) {
			printf("%s %s %s %.2f %d\n", koren->inf.ime, koren->inf.prezime, koren->inf.drzava, koren->inf.vreme, koren->inf.godine);
		}
		ispisVreme(koren->desni, zadatoVreme);
	}
}

BCVOR* najbrziNajmladji(BCVOR* koren, char* zadataDrzava, int* pronasao) {
	if(koren == NULL) {
		return NULL;
	}
	najbrziNajmladji(koren->levi, zadataDrzava, pronasao);
	if(strcmp(koren->inf.drzava, zadataDrzava) == 0 && *pronasao == 0) {
		(*pronasao)++;
		return koren;
	}
	najbrziNajmladji(koren->desni, zadataDrzava, pronasao);
}

float zbirVremena(BCVOR* koren) {
	if(koren == NULL) {
		return 0;
	}else{
		float l = zbirVremena(koren->levi);
		float d = zbirVremena(koren->desni);
		return l+d+koren->inf.vreme;
	}
}

int brojSprintera(BCVOR* koren) {
	if(koren == NULL) {
		return 0;
	}else{
		int l = brojSprintera(koren->levi);
		int d = brojSprintera(koren->desni);
		return l+d+1;
	}
}

void najmanjaRazlika(BCVOR* koren, float* razlika, float prosecnoVreme) {
	if(koren != NULL) {
		najmanjaRazlika(koren->levi, razlika, prosecnoVreme);
		if((koren->inf.vreme - prosecnoVreme) <= (*razlika)) {
			*razlika = koren->inf.vreme - prosecnoVreme;
			if(*razlika < 0) { //abs vraca int, mora 'vako
				(*razlika) *= -1;
			}
			//printf("%.2f\n", *razlika); //provera
			//printf("%.2f\n%.2f\n\n", koren->inf.vreme, prosecnoVreme); //provera
		}
		najmanjaRazlika(koren->desni, razlika, prosecnoVreme);
	}
}

void ispisNajmanjaRazlika(FILE* izlaz2, BCVOR* koren, float razlika, float prosecnoVreme) {
	if(koren != NULL) {
		ispisNajmanjaRazlika(izlaz2, koren->levi, razlika, prosecnoVreme);
		if(koren->inf.vreme - prosecnoVreme == razlika) {
			fprintf(izlaz2, "%s %s %s %.2f %d\n", koren->inf.ime, koren->inf.prezime, koren->inf.drzava, koren->inf.vreme, koren->inf.godine);
		}
		ispisNajmanjaRazlika(izlaz2, koren->desni, razlika, prosecnoVreme);
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
