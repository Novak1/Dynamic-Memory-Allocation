#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXIME 20

typedef struct naucnik_st{
    char ime[MAXIME];
    char prezime[MAXIME];
    int brojRadova;
    char oblast[MAXIME];
}TIP;

typedef struct cvor_st{
    TIP inf;
    struct cvor_st *levi;
    struct cvor_st *desni;
}BCVOR;

int uporedi(TIP a, TIP b);
void dodaj(BCVOR *novi, BCVOR **pkoren);
void iscitajIzFajla(FILE *ul, BCVOR **koren);
void ispisi(BCVOR *koren);
void obrisi_stablo(BCVOR *koren);
void podA(BCVOR *koren,int zadato,FILE *iz);
int podB(int *n, BCVOR *koren);
int prebrojPrezime(BCVOR *koren, char prezime[]);
int podC(BCVOR *koren, char oblast[]);

int main(int brArg, char *args[]){
    FILE *ulaz, *izlaz;
    BCVOR *koren;
    int n=0;
    koren=NULL;
    
    ulaz = fopen(args[1],"r");
    
    if(ulaz==NULL){
        printf("Nismo uspeli da otvorimo ulaznu datoteku\n");
        return 1;
    }
    
    izlaz = fopen(args[2],"w");
    
    if(izlaz==NULL){
        printf("Nismo uspeli da otvorimo izlaznu datoteku\n");
        return 1;
    }
    
    iscitajIzFajla(ulaz, &koren);
    ispisi(koren);
    
    printf("\nNaucnici sa brojem radova vecim od zadatog\n");
    podA(koren, atoi(args[3]),izlaz);
    puts("");
    int duzina = podB(&n,koren);

    printf("Srednja duzina prezimena %.2lf\n",(double)duzina/n);
    
    char oblast[MAXIME];
    printf("Unesite oblast\n");
    scanf("%s",oblast);
    int br = podC(koren,oblast);
    FILE *izl2 = fopen(strcat(oblast,".txt"),"w");
    fprintf(izl2,"%d\n",br);
    
    fclose(izl2);
    fclose(ulaz);
    fclose(izlaz);
    
    obrisi_stablo(koren);
    
    return 0;
}

void ispisi(BCVOR *koren){
    if(koren!=NULL){
        ispisi(koren->levi);
        printf("naucnik: %s %s %d %s\n",koren->inf.ime,
                                    koren->inf.prezime,
                                    koren->inf.brojRadova,
                                    koren->inf.oblast);
        ispisi(koren->desni);
     }
}

void podA(BCVOR *koren,int zadato,FILE *iz){
    if(koren!=NULL){
        podA(koren->levi,zadato,iz);
        if(koren->inf.brojRadova>zadato){
            fprintf(iz,"naucnik: %s %s %d %s\n",koren->inf.ime,
                                    koren->inf.prezime,
                                    koren->inf.brojRadova,
                                    koren->inf.oblast);
        }
        podA(koren->desni,zadato,iz);
     }
}

int prebrojPrezime(BCVOR *koren, char prezime[]){
    if(koren!=NULL){
        if(strcmp(koren->inf.prezime,prezime)==0)
            return 1+prebrojPrezime(koren->levi,prezime)
                    +prebrojPrezime(koren->desni,prezime);
        else
            return prebrojPrezime(koren->levi,prezime)
                   +prebrojPrezime(koren->desni,prezime);
    }
    return 0;
}

int podC(BCVOR *koren, char oblast[]){
    if(koren!=NULL){
        if(strcmp(koren->inf.oblast,oblast)==0)
            return podC(koren->levi,oblast)+
                    podC(koren->desni,oblast)+
                    koren->inf.brojRadova;
        else
            return podC(koren->levi,oblast)+
                    podC(koren->desni,oblast);
    }
    return 0;
}

int podB(int *n, BCVOR *koren){
    if(koren!=NULL){
       if(prebrojPrezime(koren->desni,koren->inf.prezime)==0){
            *n+=1;
            return podB(n,koren->levi) +
                   podB(n,koren->desni) +
                   strlen(koren->inf.prezime);
       }else{
            return podB(n,koren->levi) +
                   podB(n,koren->desni);
       
       }
    }
    return 0;
}

void iscitajIzFajla(FILE *ul, BCVOR **koren){
    char ime[MAXIME];
    char prezime[MAXIME];
    int brojRadova;
    char oblast[MAXIME];
    BCVOR *novi;
    
    while(fscanf(ul,"%s %s %d %s",ime,prezime,&brojRadova,oblast)!=EOF){  
        novi = (BCVOR *)malloc(sizeof(BCVOR));
        strcpy(novi->inf.ime,ime);
        strcpy(novi->inf.prezime,prezime);
        novi->inf.brojRadova = brojRadova;
        strcpy(novi->inf.oblast,oblast);
        dodaj(novi,koren);
    }
}


//funkcija koja vraca da li je levi element manji od desnog
int uporedi(TIP a, TIP b){
    if(strcmp(a.prezime,b.prezime)<0)
        return 1;
    else
        return 0;
}

void dodaj(BCVOR *novi, BCVOR **pkoren)
{
  /* ako je stablo prazno ili ubacujemo kao necije podstablo */
  if (*pkoren == NULL)
  {
    *pkoren = novi;
    novi->levi = NULL;
    novi->desni = NULL;
  }
  else {
    if (uporedi(novi->inf,(*pkoren)->inf))
    {
      /* ako je novi manji od tekuceg,
         ubacujemo ga u levo podstablo */
      dodaj(novi, &(*pkoren)->levi);
    }
    else
    {
      /* ako je novi veci ili jednak od tekuceg,
         ubacujemo ga u desno podstablo */
      dodaj(novi, &(*pkoren)->desni);
    }
  }
}

void obrisi_stablo(BCVOR *koren)
{
  if (koren != NULL)
  {
    obrisi_stablo(koren->levi);
    obrisi_stablo(koren->desni);
    //printf("brisem: %s\n", koren->inf.ime);
    free(koren);
  }
}



