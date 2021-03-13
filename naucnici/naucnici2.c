#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define MAX_S 21
#define MAX_O 31

typedef struct naucnik_st{
    char ime[MAX_S];
    char prezime[MAX_S];
    unsigned broj;
    char oblast[MAX_O];
}naucnik;

typedef struct cvor_st {
    naucnik inf;
    struct cvor_st *levi;
    struct cvor_st *desni;
}BCVOR;

void init(BCVOR **koren);
FILE *sigurnoOtvaranje(char *imeFajla, char *rezim);
void ucitajIzFajla(FILE *ulaz, BCVOR **koren);
void dodaj(BCVOR **koren, naucnik x);
int uporedi(naucnik a, naucnik b);
void ispisi(BCVOR *koren);
void podA(BCVOR *koren, FILE *izlaz, int zadato);
void obrisiStablo(BCVOR **koren);
int brojRadovaZaOblast(BCVOR *koren, char *oblast);
int brojNacnikaIzOblasti(BCVOR *koren, char *oblast);
int duzinaPrezimena(BCVOR *koren,int *n);
int nijeJedinstveno(BCVOR *koren, char *prezime);

int main(int brArgs, char *args[]) {
    BCVOR *koren;
    FILE *ulaz, *izlaz, *izlazOblast;
    char oblast[MAX_S];
    int n=0;
    char *nazivUlaznogFajla = args[1];
    char *nazivIzlaznogFajla = args[2];
    int zadatiBrojRadova = atoi(args[3]);
    
    if(brArgs<4){
        printf("nije navedeno dovoljno argumenata\n");
        exit(EXIT_FAILURE);
    }

    init(&koren);    
    ulaz = sigurnoOtvaranje(nazivUlaznogFajla, "r");
    ucitajIzFajla(ulaz, &koren);
    
    ispisi(koren);
    
    izlaz = sigurnoOtvaranje(nazivIzlaznogFajla, "w");
    podA(koren, izlaz, zadatiBrojRadova);
    
    printf("Unesite zeljenu oblast\n");
    scanf("%s",oblast);
    
    int brRadova = brojRadovaZaOblast(koren, oblast);
    int brNaucnika = brojNacnikaIzOblasti(koren, oblast);
    char nazivFajlaZaOblast[MAX_O+4];
    
    strcpy(nazivFajlaZaOblast,oblast);
    strcat(nazivFajlaZaOblast, ".txt");
    
    izlazOblast=sigurnoOtvaranje(nazivFajlaZaOblast,"w");
    fprintf(izlazOblast, "%d\n", brRadova);
    //(double)brRadova/brNaucnika
    
    int duz = duzinaPrezimena(koren, &n);
    
    printf("Prosecna duzina prez. %.2lf\n",(double)duz/n);
    
    obrisiStablo(&koren);
    fclose(ulaz);
    fclose(izlaz);
    fclose(izlazOblast);

    return 0;
}

int nijeJedinstveno(BCVOR *koren, char *prezime){
    if(koren==NULL){
        return 0;
    }else{
        if(strcmp(koren->inf.prezime,prezime)==0){
            return 1;
        } else {
         int l = nijeJedinstveno(koren->levi, prezime);
         int d = nijeJedinstveno(koren->desni, prezime);
         return l || d;
        }
    }
}

int duzinaPrezimena(BCVOR *koren, int *n){
    if(koren!=NULL){
        int d = duzinaPrezimena(koren->desni, n);
        int l = duzinaPrezimena(koren->levi, n);
      if(nijeJedinstveno(koren->levi,koren->inf.prezime)){
           return l+d;
      }else{
           (*n)++;
           return l+d+strlen(koren->inf.prezime);
      }
    }
}

int brojNacnikaIzOblasti(BCVOR *koren, char *oblast){
    if(koren==NULL){
        return 0;
    }else{
        int l = brojNacnikaIzOblasti(koren->levi, oblast);
        int d = brojNacnikaIzOblasti(koren->desni, oblast);
        if(strcmp(koren->inf.oblast,oblast)==0){
            return l+d+1;
        }else{
            return l+d;
        }
    }
}

int brojRadovaZaOblast(BCVOR *koren, char *oblast){
    if(koren==NULL){
        return 0;
    }else{
        int l = brojRadovaZaOblast(koren->levi, oblast);
        int d = brojRadovaZaOblast(koren->desni, oblast);
        if(strcmp(koren->inf.oblast,oblast)==0){
            return l+d+koren->inf.broj;
        }else{
            return l+d;
        }
    }
}

void obrisiStablo(BCVOR **koren){
    if(*koren!=NULL){
        obrisiStablo(&(*koren)->levi);
        obrisiStablo(&(*koren)->desni);
        free(*koren);
        *koren=NULL;
    }
}

void podA(BCVOR *koren, FILE *izlaz, int zadato){
    if(koren!=NULL){
        podA(koren->levi,izlaz,zadato);
        if(koren->inf.broj > zadato){
            fprintf(izlaz,"%s %s %u %s\n", koren->inf.ime
             , koren->inf.prezime, koren->inf.broj
             , koren->inf.oblast);
        }
        podA(koren->desni,izlaz,zadato);
    }
}

void ispisi(BCVOR *koren){
    if(koren!=NULL){
        ispisi(koren->levi);
        printf("\nNaucnik\n");
        printf("%s %s %u %s\n", koren->inf.ime, 
                koren->inf.prezime, koren->inf.broj,
                koren->inf.oblast);
        ispisi(koren->desni);
    }
}

int uporedi(naucnik a, naucnik b){
    return strcmp(a.prezime, b.prezime)>0;
}

void dodaj(BCVOR **koren, naucnik x){
    if(*koren==NULL){
        BCVOR *novi = (BCVOR *)malloc(sizeof(BCVOR));
        if(novi==NULL){
            printf("Nije uspesno zauzimanje memorije\n");
            exit(errno);
        }
        novi->levi = NULL;
        novi->desni = NULL;
        strcpy(novi->inf.ime, x.ime);
        strcpy(novi->inf.prezime, x.prezime);
        strcpy(novi->inf.oblast, x.oblast);
        novi->inf.broj = x.broj;
        *koren=novi;
    }else{
        if(uporedi(x, (*koren)->inf)){
            dodaj(&(*koren)->desni, x);
        }else{
            dodaj(&(*koren)->levi, x);
        }
    }
}

void ucitajIzFajla(FILE *ulaz, BCVOR **koren){
    naucnik x;
    while(fscanf(ulaz, "%s %s %u %s", x.ime, x.prezime,
                             &x.broj, x.oblast)!=EOF){
        dodaj(koren, x);                         
    }
}

FILE *sigurnoOtvaranje(char *imeFajla, char *rezim){
    FILE *f = fopen(imeFajla, rezim);
    if(f==NULL){
        printf("Neuspesno otvaranje fajla %s\n",imeFajla);
        exit(errno);
    }
    
    return f;
}

void init(BCVOR **koren){
    *koren = NULL;
}


