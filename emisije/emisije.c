#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_E 21
#define MAX_T 31

typedef struct emisija_st {
	char naziv[MAX_E];
	float frekvencija;
	unsigned trajanje;
	char tip[MAX_T];
}emisija;

typedef struct cvor_st {
	emisija inf;
	struct cvor_st *levi;
	struct cvor_st *desni;
}BCVOR;

void init(BCVOR**);
FILE* otvori_fajl(char*, char*, int);
void ucitajIzFajla(FILE*, BCVOR**);
void dodaj(BCVOR**, emisija);
int uporedi(emisija, emisija);
void ispisi(BCVOR*);
void ispisFajl(FILE*, BCVOR*);
void najkracaNaFrekvenciji(BCVOR*, float, int*);
int zbirTrajanja(BCVOR*, char*, int*);
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
	float zadataFrekvencija = atof(arg[3]);
	
	init(&koren);
	
	FILE* ulaz = otvori_fajl(ulazna_datoteka, "r", 2);
	ucitajIzFajla(ulaz, &koren);
	
	ispisi(koren); //provera
	
	FILE* izlaz = otvori_fajl(izlazna_datoteka, "w", 3);
	ispisFajl(izlaz, koren);
	
	najkracaNaFrekvenciji(koren, zadataFrekvencija, &pronasao);
	
	char zadatiTip[MAX_T];
	
	printf("Unesite tip emisije: ");
	scanf("%s", zadatiTip);
	
	char tipFajl[MAX_T+4];
	
	strcpy(tipFajl, zadatiTip);
	strcat(tipFajl, ".txt");
	
	FILE* izlaz2 = otvori_fajl(tipFajl, "w", 4);
	
	fprintf(izlaz2, "%.2f", (float)zbirTrajanja(koren, zadatiTip, &brojac)/(float)brojac);
	
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
	emisija x;
	
	while(fscanf(ulaz, "%s %f %u %s", x.naziv, &x.frekvencija, &x.trajanje, x.tip) != EOF) {
		dodaj(koren, x);
	}
}

void dodaj(BCVOR** koren, emisija x) {
	if(*koren == NULL) {
		BCVOR* novi = (BCVOR*)malloc(sizeof(BCVOR));
		if(novi == NULL) {
			printf("Nema dovoljno slobodne memorije!\n");
			exit(22);
		}
		novi->levi = NULL;
		novi->desni = NULL;
		strcpy(novi->inf.naziv, x.naziv);
		novi->inf.frekvencija = x.frekvencija;
		novi->inf.trajanje = x.trajanje;
		strcpy(novi->inf.tip, x.tip);
		*koren = novi;
	}else{
		if(uporedi(x, (*koren)->inf)) {
			dodaj(&(*koren)->desni, x);
		}else{
			dodaj(&(*koren)->levi, x);
		}
	}
}

int uporedi(emisija a, emisija b) {
	if (a.trajanje == b.trajanje) {
		return a.frekvencija > b.frekvencija;
	}
	else {
		return a.trajanje > b.trajanje;
	}
}

void ispisi(BCVOR* koren) {
	if(koren != NULL) {
		ispisi(koren->levi);
		printf("%s %.2f %u %s\n", koren->inf.naziv, koren->inf.frekvencija, koren->inf.trajanje, koren->inf.tip);
		ispisi(koren->desni);
	}
}

void ispisFajl(FILE* izlaz, BCVOR* koren) {
	if(koren != NULL) {
		ispisFajl(izlaz, koren->levi);
		fprintf(izlaz, "%s %.1f %.2f %s\n", koren->inf.naziv, koren->inf.frekvencija, (float)koren->inf.trajanje/60.0, koren->inf.tip);
		ispisFajl(izlaz, koren->desni);
	}
}

void najkracaNaFrekvenciji(BCVOR* koren, float zadataFrekvencija, int* pronasao) {
	if(koren != NULL) {
		najkracaNaFrekvenciji(koren->levi, zadataFrekvencija, pronasao);
		if(koren->inf.frekvencija > zadataFrekvencija && *pronasao == 0) {
			printf("\nNajkraca emisija na frekvenciji vecoj od %.1f je:\n", zadataFrekvencija);
			printf("%s %.1f %u %s\n\n", koren->inf.naziv, koren->inf.frekvencija, koren->inf.trajanje, koren->inf.tip);
			(*pronasao)++;
		}
		najkracaNaFrekvenciji(koren->desni, zadataFrekvencija, pronasao);
	}
}

int zbirTrajanja(BCVOR* koren, char* zadatiTip, int* brojac) {
	if(koren == NULL) {
		return 0;
	}else{
		int l = zbirTrajanja(koren->levi, zadatiTip, brojac);
		int d = zbirTrajanja(koren->desni, zadatiTip, brojac);
		if(strcmp(koren->inf.tip, zadatiTip) == 0) {
			(*brojac)++;
			return l+d+koren->inf.trajanje;
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
