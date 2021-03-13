#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_IP 21
#define MAX_O 31

typedef struct naucnik_st {
	char ime[MAX_IP];
	char prezime[MAX_IP];
	unsigned brojRadova;
	char oblast[MAX_O];
}naucnik;

typedef struct cvor_st {
	naucnik inf;
	struct cvor_st *levi;
	struct cvor_st *desni;
}BCVOR;

void init(BCVOR**);
FILE* otvori_fajl(char*, char*, int);
void ucitajIzFajla(FILE*, BCVOR**);
void dodaj(BCVOR**, naucnik);
int uporedi(naucnik, naucnik);
void ispisi(BCVOR*);
void podA(FILE*, BCVOR*, unsigned);
int nijeJedinstveno(BCVOR*, char*);
int duzinaPrezimena(BCVOR*, int*);
int brojRadovaZaOblast(BCVOR*, char*);
void obrisiStablo(BCVOR**);

int main(int brArg, char* arg[]) {
	BCVOR* koren;
	int n = 0;
	
	if(brArg != 4) {
		printf("Greska prilikom poziva programa!\n");
		exit(1);
	}
	
	char* ulazna_datoteka = arg[1];
	char* izlazna_datoteka = arg[2];
	unsigned brojRadova = (unsigned)(atoi(arg[3]));
	
	init(&koren);
	
	FILE* ulaz = otvori_fajl(ulazna_datoteka, "r", 2);
	ucitajIzFajla(ulaz, &koren);
	
	ispisi(koren);
	
	FILE* izlaz = otvori_fajl(izlazna_datoteka, "w", 3);
	podA(izlaz, koren, brojRadova);
	
	int duzina = duzinaPrezimena(koren, &n);
	printf("Prosecna duzina prezimena je: %.2f\n", (float)duzina/(float)n);
	
	char oblast[MAX_O];
	
	printf("Unesite zeljenu naucnu oblast: ");
	scanf("%s", oblast);
	
	char nazivFajlaZaOblast[MAX_O+4];
	
	strcpy(nazivFajlaZaOblast, oblast);
	strcat(nazivFajlaZaOblast, ".txt");
	
	FILE* izlaz2 = otvori_fajl(nazivFajlaZaOblast, "w", 4);
	
	int ukupanBrojRadova = brojRadovaZaOblast(koren, oblast);
	fprintf(izlaz2, "%d\n", ukupanBrojRadova);
	
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
		printf("Greska prilikom otvaranja datoteke %s\n", naziv_fajla);
		exit(status_greske);
	}
	
	return f;
}

void ucitajIzFajla(FILE* ulaz, BCVOR** koren) {
	naucnik x;
	
	while(fscanf(ulaz, "%s %s %u %s", x.ime, x.prezime, &x.brojRadova, x.oblast) != EOF) {
		dodaj(koren, x);
	}
}

void dodaj(BCVOR** koren, naucnik x) {
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
		novi->inf.brojRadova = x.brojRadova;
		strcpy(novi->inf.oblast, x.oblast);
		*koren = novi;
	}else{
		if(uporedi(x, (*koren)->inf)){
			dodaj(&(*koren)->desni, x);
		}else{
			dodaj(&(*koren)->levi, x);
		}
	}
}

int uporedi(naucnik a, naucnik b) {
	return strcmp(a.prezime, b.prezime) > 0;
}

void ispisi(BCVOR* koren) {
	if(koren != NULL) {
		ispisi(koren->levi);
		printf("%s %s %u %s\n", koren->inf.ime, koren->inf.prezime, koren->inf.brojRadova, koren->inf.oblast);
		ispisi(koren->desni);
	}
}

void podA(FILE* izlaz, BCVOR* koren, unsigned zadatiBrojRadova) {
	if(koren != NULL) {
		podA(izlaz, koren->levi, zadatiBrojRadova);
		if(koren->inf.brojRadova > zadatiBrojRadova) {
			fprintf(izlaz, "%s %s %u %s\n", koren->inf.ime, koren->inf.prezime, koren->inf.brojRadova, koren->inf.oblast);
		}
		podA(izlaz, koren->desni, zadatiBrojRadova);
	}
}

int nijeJedinstveno(BCVOR* koren, char* prezime){
    if(koren==NULL){
        return 0;
    }else{
        if(strcmp(koren->inf.prezime, prezime)==0){
            return 1;
        } else {
         int l = nijeJedinstveno(koren->levi, prezime);
         int d = nijeJedinstveno(koren->desni, prezime);
         return l || d;
        }
    }
}

int duzinaPrezimena(BCVOR* koren, int* n){
    if(koren!=NULL){
        int d = duzinaPrezimena(koren->desni, n);
        int l = duzinaPrezimena(koren->levi, n);
      if(nijeJedinstveno(koren->levi, koren->inf.prezime)){
           return l+d;
      }else{
           (*n)++;
           return l+d+strlen(koren->inf.prezime);
      }
    }
}

int brojRadovaZaOblast(BCVOR* koren, char* oblast) {
	if(koren == NULL) {
		return 0;
	}else{
		int l = brojRadovaZaOblast(koren->levi, oblast);
		int d = brojRadovaZaOblast(koren->desni, oblast);
		if(strcmp(koren->inf.oblast, oblast) == 0){
			return l+d+koren->inf.brojRadova;
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
