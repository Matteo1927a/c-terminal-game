#include  <stdio.h>
#include  "gamelib.h"
#include  <stdlib.h>
#include  <time.h>
#include  <stdbool.h>
#include  <ctype.h>


Giocatore *giocatori[4];
static int num_giocatori;
struct Zona_mappa *pFirst=NULL;
struct Zona_mappa *pLast=NULL;
struct Zona_mappa *caravan_zone = NULL;
static int mappa_creata = 0;
static int num_zone=0;
static bool prova_EMF_trovata = false;
static bool prova_spirit_box_trovata = false;
static bool prova_videocamera_trovata = false;
static bool fantasma_presente=false;
static int livello = 0;
static enum Tipo_oggetto_iniziale *oggetti_scelti;
static int oggetti_allocati;
static bool fantasma_generato = false;
static int prova_raccolta = 0;
static bool usato_1 = false;
static bool usato_2 = false;
static bool usato_3 = false;
static bool usato_4 = false;
static bool usato_5 = false;
static bool oggetto_assegnato = false;
static bool tutti_impazziti = false;





static void chiudi_mappa(void) {
    mappa_creata = 1;
}

void libera_memoria(Giocatore *giocatori[4],enum Tipo_oggetto_iniziale *oggetti_scelti, int oggetti_allocati) {

for(int i=0; i<4; i++) {
  if(giocatori[i]!=NULL) {  //libera la memoria allocata per i giocatori
    free(giocatori[i]);
    giocatori[i]= NULL;

  }
}
if (oggetti_allocati) {
    // Se è stata allocata della memoria per gli oggetti scelti, la libero
    free(oggetti_scelti);
    oggetti_allocati = 0;
    // Imposto la variabile di controllo a 0 per indicare che la memoria è stata liberata
}



}

void libera_memoria_zone(void) {
  // Libera la memoria per le zone della mappa
  if (pFirst == NULL) {
       return;
   }
  struct Zona_mappa *zona_corrente = pFirst;
  struct Zona_mappa *zona_successiva;

  while (zona_corrente != pFirst) {
      zona_successiva = zona_corrente->prossima_zona;
      free(zona_corrente);
      zona_corrente = zona_successiva;
  }

  pFirst = NULL;
  pLast = NULL;

  if(caravan!=NULL){

  free(caravan_zone);

  }
}

static void stampa_zona(int j, struct Zona_mappa *zona_attuale) {



  printf("Zona attuale del giocatore %d: ", j + 1);;
    switch(zona_attuale->zona) {
        case caravan:
            printf("Caravan\n");
            break;
        case cucina:
            printf("Cucina\n");
            break;
        case soggiorno:
            printf("Soggiorno\n");
            break;
       case camera:
            printf("Camera\n" );
            break;
       case bagno:
            printf("Bagno\n" );
            break;
       case garage :
            printf("garage\n" );
            break;
      case seminterrato:
            printf("seminterrato\n" );
            break;
      default:
            printf("Zona sconosciuta\n" );
}
            printf("Oggetto presente nella zona: ");
    switch(zona_attuale->oggetto_zona) {
      case adrenalina:
            printf("Adrenalina\n");
            break;
        case cento_dollari:
            printf("Cento dollari\n");
            break;
        case coltello:
            printf("Coltello\n");
            break;
        default:
            printf("nessun_oggetto\n" );
}
            printf("Prova presente nella zona\n" );
            switch (zona_attuale->prova) {
              case prova_EMF:
              printf("Prova EMF\n");
              break;
              case prova_spirit_box:
              printf("Prova spirit_box\n" );
              break;
              case prova_videocamera:
              printf("Prova videocamera\n");
              break;
              default:
              printf("nessuna prova\n" );
            }

}







static void sanita__mentale(int j){ // gestisce la probabilità che ad ogni mossa dei giocatori la loro sanità mentale scenda di 15
  srand(time(NULL));

   int numero_casuale = rand() % 100;

   if (numero_casuale <= 20) {
     if (giocatori[j]->sanita_mentale > 0) {
    giocatori[j]->sanita_mentale -= 15 ;
if (giocatori[j]->sanita_mentale > 100) {
    giocatori[j]->sanita_mentale = 0;
}
}
     printf("Visto la tua mossa la sanità mentale è scesa di 15!!\n");

   }


}


static bool zaino_pieno (int j) {
    int contatore = 0;
    for(int i = 0; i < 4; i++) {
        if(giocatori[j]->zaino[i] == -1) {
            contatore++;
        }

    }

return contatore==0;


}


static void genera_fantsama(void){

   int probabilita_apparizione = 0;

   switch (livello) {
      case 1:
         probabilita_apparizione = 20; // 20% di probabilità di apparizione del fantasma
         break;
      case 2:
         probabilita_apparizione = 50; // 50% di probabilità di apparizione del fantasma
         break;
      case 3:
         probabilita_apparizione = 80; // 80% di probabilità di apparizione del fantasma
         break;
   }

   if (prova_raccolta >= 1 && prova_raccolta < 3) {
         probabilita_apparizione +=10;
      } else if (prova_raccolta >= 3) {
        probabilita_apparizione += 20;
      }

 srand(time(NULL));  // genera il fantasma in una zona in modo casuale in base alle zone create
   int numero_casuale = rand() % 100;
   if (numero_casuale < probabilita_apparizione) {
      int zona_fantasma = rand() % num_zone;
      struct Zona_mappa *pZona = pFirst;
      for (int i = 0; i < zona_fantasma; i++) {
         pZona = pZona->prossima_zona;
}

      fantasma_presente = true;

   }
   else{
     fantasma_presente= false;
   }
}


static void dim_sanita_mentale (int j){

//diminusice la sanità mentale dei giocatori quando incontrano il fantasma in base all livello di difficoltà

if(fantasma_presente==true){
  if(!fantasma_generato){
  switch (livello) {
    case 1:
    giocatori[j]->sanita_mentale-=20;
    printf("Hai incontrato il fantasma la tua sanità mentale è scesa di 20!!\n" );
    break;
    case 2:
    giocatori[j]->sanita_mentale-=30;
    printf("Hai incontrato il fantasma la tua sanità mentale è scesa di 30!!\n" );
    break;
    case 3:
    giocatori[j]->sanita_mentale-=50;
    printf("Hai incontrato il fantasma la tua sanità mentale è scesa di 50!!\n" );
    break;
  }
  fantasma_generato = true;
}
else {
  fantasma_generato = false;
}
}


  if(giocatori[j]->sanita_mentale > 100){
    giocatori[j]->sanita_mentale = 0;
  }



}


static void aggiorna_prova_zona(struct Zona_mappa *zona) {
// Genera la prova della zona con una certa probabilità
int random = rand() % 100; // Genera un numero casuale tra 0 e 99
if (random < 40) {
zona->prova = nessuna_prova;
} else {
random = (random - 40) % 4 + 9; // Genera un numero casuale tra 0 e 2
zona->prova = (enum Tipo_prova )random;
}
}



static bool check_fantasma(int j){
// controlla la presenza del fantasma per l'utili<<o del sale
	if (fantasma_presente){
	if (giocatori[j]->sale_utilizzato == false){
		dim_sanita_mentale(j);
	}
	else{
		printf("Hai incontrato il fantasma ma non hai perso sanità mentale grazie al sale\n");
		giocatori[j]->sale_utilizzato=false;
	}
}
return 0;
}

static void avanza(int j,struct Zona_mappa *zona_attuale){
  // fa avanzare il giocatore di una zona
if (giocatori[j]->posizione == NULL) {
printf("Posizione corrente non valida\n");
return;
}

  zona_attuale = giocatori[j]->posizione->prossima_zona;
giocatori[j]->posizione = zona_attuale;

       printf("Sei avanzato\n" );


aggiorna_prova_zona(giocatori[j]->posizione);
check_fantasma(j);

}


static void avanza2(int j,struct Zona_mappa *zona_attuale){
  if (giocatori[j]->posizione == NULL) {
  printf("Posizione corrente non valida\n");
  return;
  }

    zona_attuale = giocatori[j]->posizione->prossima_zona->prossima_zona;
  giocatori[j]->posizione = zona_attuale;

         printf("Sei avanzato di due zone\n");


  aggiorna_prova_zona(giocatori[j]->posizione);
  check_fantasma(j);

}


static void usa_calmanti(int j){
  if(usato_1){
  printf("Hai già utilizzato questo oggetto!\n");
  return;
  }
  usato_1= 1;
  if(giocatori[j]->sanita_mentale>60){
    printf("Per utilizzare i calmanti devi avere una sanità mentale minore di 60!!\n" );
    return;
  }
  static bool ha_calmanti = false;
  for (int a=0; a< 4; a++){
    if (giocatori[j]->zaino[a]== calmanti){
    ha_calmanti=true;
    break;
    }
  }
  if(ha_calmanti){
    for(int p = 0; p < 4; p++) {
    if (giocatori[j]->zaino[p] ==calmanti) {
    giocatori[j]->zaino[p] = -1;

    break;
  }
}
    printf("Hai utilizzato i calmanti la tua sanità mentale crescerà di 40!!\n" );
  giocatori[j]->sanita_mentale+=40;


}


else {
  printf("Non hai i calmanti nel tuo zaino\n" );
}

 sanita__mentale(j);

}

static void usa_sale(int j){
giocatori[j]->sale_utilizzato = true;
  if(usato_2){
  printf("Hai già utilizzato questo oggetto!\n");
  return;
  }
  usato_2 =1;
  static bool ha_sale = false;
  for (int a=0; a< 4; a++){
    if (giocatori[j]->zaino[a]== sale){
    ha_sale=true;
    break;
    }
  }
    if(ha_sale){
      for(int p = 0; p < 4; p++) {
      if (giocatori[j]->zaino[p] == sale) {
      giocatori[j]->zaino[p] = -1;

      break;
    }
  }

printf("Usi il sale, se ce il fantasma comunque la tua sanità mentale non scenderà\n" );

}


else{
  printf("Non hai il sale\n" );

}

 sanita__mentale(j);

}


static void usa_cento_dollari(int j) {


  if(usato_3){
  printf("Hai già utilizzato questo oggetto!\n");
  return;
  }
usato_3 = 1;
// controlla se il giocatore ha i cento dollari nello zaino
static bool ha_cento_dollari = false;
for(int p = 0; p < 4; p++) {
if (giocatori[j]->zaino[p] == cento_dollari) {
ha_cento_dollari = true;
break;
}
}

if(ha_cento_dollari) {
// rimuove i cento dollari dallo zaino
for(int p = 0; p < 4; p++) {
if (giocatori[j]->zaino[p] == cento_dollari) {
giocatori[j]->zaino[p] = -1;

break;
}
}

// genera un numero casuale tra 0 e 1
srand(time(NULL));
static int num_casuale;
num_casuale = rand() % 2 + 3;


for(int p=0; p<4;p++){
  if(oggetto_assegnato== false){
// assegna un oggetto casuale tra calmanti e sale
switch (num_casuale) {

  case 3:
if(giocatori[j]->zaino[p]==-1){
  giocatori[j]->zaino[p]=calmanti;
oggetto_assegnato=true;
  break;
}

  case 4:
if(giocatori[j]->zaino[p]==-1){
  giocatori[j]->zaino[p]=sale;
oggetto_assegnato=true;
  break;
}
}

}
}
 sanita__mentale(j);
printf("Hai ottenuto un oggetto casuale nello zaino!\n");
} else {
printf("Non hai i cento dollari nello zaino!\n");
}


}

static void usa_adrenalina(int j){
  if(usato_4){
  printf("Hai già utilizzato questo oggetto!\n");
  return;
  }
usato_4 =1;
  static bool ha_adrenalina = false;
  for(int g=0; g<4;g++){
    if(giocatori[j]->zaino[g]== adrenalina){
    ha_adrenalina = true;
    break;
  }
}

    if (ha_adrenalina){
      for(int p = 0; p < 4; p++) {
      if (giocatori[j]->zaino[p] == adrenalina) {
      giocatori[j]->zaino[p] = -1;

      break;
      }
    }
      printf("Con l'adrenlaina avanzi di un altra posizione\n" );
      avanza(j,giocatori[j]->posizione);
       sanita__mentale(j);
    }
    else
    {
      printf("Non hai l'adrenalina nel tuo zaino\n");
    }

  }

static void usa_coltello(int j){


  if(usato_5){
  printf("Hai già utilizzato questo oggetto!\n");
  return;
  }
usato_5 =1;

  static bool ha_coltello = false;
  for (int h=0;h<4;h++){
    if(giocatori[j]->zaino[h]==coltello){
      ha_coltello = true;
      break;
    }
  }
  printf("Usi il coltello, con esso se la tua sanità mentale è troppo bassa uccidi i tuoi compagni\n" );

  if (ha_coltello){
    for( int x = 0; x<4; x++){
      if(giocatori[j]->zaino[x] == coltello){
        giocatori[j]->zaino[x] = -1;
        break;
      }
    }

 sanita__mentale(j);

 if(giocatori[j]->sanita_mentale < 30){
  for(int i= 0; i<num_giocatori; i++){
    if( j!= i && giocatori[i]->posizione == giocatori[j]->posizione){
printf("Hai ucciso un giocatore, il giocatore %s è morto\n",giocatori[i]->nome_giocatore );
giocatori[i]->morto = true;

}




}


}


}
else {
  printf("Non hai il coltello nello zaino\n" );
}
}





  static int passa(void){
    printf("Non farai nulla passi il turno al prossimo giocatore\n" );
return 0;

  }




static void richiedi_livello_difficolta(void) {

  enum Livello_difficolta {
  dilettante,
  intermedio,
  incubo
};


do{
  printf("Scegli il livello di difficoltà:\n");
  printf("1. Dilettante\n");
  printf("2. Intermedio\n");
  printf("3. Incubo\n");
  printf("N.B qualunque altro carattere inserito non permette di impostare il gioco\n" );
  scanf("%d", &livello);
  while ((getchar() !='\n'));
} while(livello!= 1 && livello != 2 && livello !=3);
}

static void stampa_zaino(void){
for (int i = 0; i < num_giocatori; i++) {
    printf("Zaino del giocatore %s:", giocatori[i]->nome_giocatore);
    for (int j = 0; j < 4; j++) {
        switch (giocatori[i]->zaino[j]) {
            case EMF:
                printf(" EMF");
                break;
            case spirit_box:
                printf(" spirit box");
                break;
            case videocamera:
                printf(" videocamera");
                break;
            case calmanti:
                printf(" calmanti");
                break;
            case sale:
                printf(" sale");
                break;
            default:
                printf(" (vuoto)");
                break;
        }
    }
    printf("\n");
}
}






static void crea_caravan(void) {

  caravan_zone = malloc(sizeof(struct Zona_mappa));
  if (caravan_zone == NULL) {
        printf("Errore nell'allocazione della memoria per la zona caravan\n");
        return;
    }
caravan_zone->zona = caravan;
caravan_zone->oggetto_zona = nessun_oggetto;
caravan_zone->prova = nessuna_prova;
caravan_zone->prossima_zona = NULL;
}


static void crea_zona (enum Tipo_zona zona) {
struct Zona_mappa *nuova_zona = (struct Zona_mappa*)malloc(sizeof(struct Zona_mappa));
if (nuova_zona == NULL) {

    printf("Errore nell'allocazione della memoria per la nuova zona\n");
    return;
}
    nuova_zona->zona = zona;
    // Genera l'oggetto della zona casualmente
    int random = rand() % 4 + 5; // Genera un numero casuale tra 5 e 8
    nuova_zona->oggetto_zona = (enum Tipo_oggetto_zona) random;

    // Genera la prova della zona casualmente
    random = rand() % 5 + 9; // Genera un numero casuale tra 9 e 12
    nuova_zona->prova = (enum Tipo_prova) random;

    // Aggiunge la nuova zona alla lista
    nuova_zona->prossima_zona = NULL;
    if (pFirst == NULL) {
        pFirst = nuova_zona;
        pLast = nuova_zona;
    } else {
        pLast->prossima_zona = nuova_zona;
        pLast = nuova_zona;
    }

    // Chiude la lista circolare
    pLast->prossima_zona = pFirst;
    num_zone++;
}


static void stampa_mappa(void) {
  struct Zona_mappa *p = pFirst;
printf("Caravan - Zona sepciale -\n" );
  do {

       if (p->zona == cucina) {
          printf("Cucina\n");
      }
      else if (p->zona == soggiorno) {
          printf("Soggiorno\n");
      }
      else if (p->zona == camera) {
          printf("Camera\n");
      }
      else if (p->zona == bagno) {
          printf("Bagno\n");
      }
      else if (p->zona == garage) {
          printf("Garage\n");
      }
      else if (p->zona == seminterrato) {
          printf("Seminterrato\n");
      }

      p = p->prossima_zona;

  } while (p != pFirst);

}







static void inserisci_zona(void){

  // Alloca la memoria per la nuova zona
  struct Zona_mappa *nuova_zona = malloc(sizeof(struct Zona_mappa));
 if (nuova_zona == NULL) {
     printf("Errore nell'allocazione della memoria per la nuova zona\n");
     return;
 }

 // Genera il tipo di zona casualmente
 srand(time(NULL));
 int random = rand() % 6; // Genera un numero casuale tra 0 e 5
 if (random == 0) random = 1; // Se il numero generato è 0, sostituiscilo con 1
 nuova_zona->zona = (enum Tipo_zona )random;

 // Genera l'oggetto della zona casualmente
 random = rand() % 100; // Genera un numero casuale tra 0 e 99
 if (random < 40) {
     nuova_zona->oggetto_zona = nessun_oggetto;
 } else {
     random = (random - 40) % 4 + 5; // Genera un numero casuale tra 5 e 8
     nuova_zona->oggetto_zona = (enum Tipo_oggetto_zona )random;
 }

 // Genera la prova della zona con una certa probabilità
 random = rand() % 100; // Genera un numero casuale tra 0 e 99
 if (random < 50) {
     random = rand() % 4 + 9; // Genera un numero casuale tra 9 e 12
     nuova_zona->prova = (enum Tipo_prova )random;
 } else {
     nuova_zona->prova = nessuna_prova;
 }

 // Aggiunge la nuova zona alla lista
 nuova_zona->prossima_zona = NULL;
 if (pFirst == NULL) {
     pFirst = nuova_zona;
     pLast = nuova_zona;
 } else {
     pLast->prossima_zona = nuova_zona;
     pLast = nuova_zona;
 }

 // Chiude la lista circolare
 pLast->prossima_zona = pFirst;
 printf("Bene hai inesrito la nuova zona premi 3 per vedere la mappa completa.\n" );
num_zone++;
}





static void cancella_zona(void) {
    if (pFirst == NULL || pFirst==pLast) {
      printf("Non ci sono zone da cancellare (per giocare deve essere peresente minimo una zona!! LA MAPPA NON PUO' ESSERE VUOTA)\n" );
        // La lista è vuota, non c'è nulla da cancellare
        return;
    }

    struct Zona_mappa *zona_da_cancellare=NULL;

     if (pFirst == pLast) {
        // La lista contiene un solo elemento
        pFirst = NULL;
        pLast = NULL;
    } else {
        // La lista contiene più di un elemento
        zona_da_cancellare= pLast;
        pLast = pFirst;
        while (pLast != NULL && pLast->prossima_zona != zona_da_cancellare) {
            pLast = pLast->prossima_zona;
        }
        pLast->prossima_zona = pFirst;
    }

    // Libero la memoria occupata dalla zona cancellata
    if(zona_da_cancellare!=NULL){
    free(zona_da_cancellare);
}

}




void termina_gioco(void) {
  // termina il gioco e chiede se si vuole rigiocare
  printf("Hai terminato il gioco! Premere uno per rigiocare altrimenti premi due per terminare.\n" );
static int termina;
static int v;

  do{
    scanf("%d",&termina);
while ((getchar() !='\n'))
;
    if(termina==1){
      printf("Bene hai scleto di reimpostare il gioco\n" );
      libera_memoria(giocatori,oggetti_scelti,oggetti_allocati);
      libera_memoria_zone();


    printf("Operazione in corso..., sta avvenendo la cancellazione dei dati\n");
    printf("Bene ora puoi reimpostare il gioco premere 1 per giocare\n" );
    do{
      scanf("%d",&v );
      while ((getchar() !='\n'));
      if(v==1)
        imposta_gioco();
      }while(v!=1);
    }

    if(termina==2){
      printf("Il gioco è terminato grazie per aver giocato\n" );
exit(0);

    }




}while( termina < 1 || termina > 2);


return;
}





static void torna_caravan(int j) {

if(fantasma_presente==true){
  printf("Nella zona caravan c'è il fantasma non si possono lasciare le prove\n" );
  return;
}



    printf("Hai scelto di tornare al caravan, questa è la tua base d'appoggio dove ogni volta lascerai tutte le prove eventualmente tovate \n");
    struct Zona_mappa* pi=caravan_zone;
    giocatori[j]->posizione = pi;

    static int k = 0;
    static bool oggetto_presente1= false; // variabili per settare la presenza di una prova nello zaino
    static bool oggetto_presente2= false;
    static bool oggetto_presente3= false;

    for (int u = 0; u < 4; u++) {
      if(oggetto_presente1==false){
        if (giocatori[j]->zaino[u] == prova_EMF) {
            prova_EMF_trovata = true;
            printf("Hai lasciato la prova EMF\n");
            giocatori[j]->zaino[u] = -1; // reimposta lo slot dell zaino vuoto

            srand(time(NULL));
//generazione casuale di una delle altre due prove non ancora distribuite
            static int y;

            y= rand() % 2;

            if(y == 0){
              y= spirit_box;
              for(int b=0; b<4; b++){
              if(giocatori[j]->zaino[b] ==-1){
                giocatori[j]->zaino[b]=spirit_box;
                  oggetto_presente1= true;
                  break;

              }
            }

            }
            else{
              y= videocamera;
              for (int t=0; t<4; t++){
              if(giocatori[j]->zaino[t] ==-1){
                giocatori[j]->zaino[t]=videocamera;
  oggetto_presente1= true;
  break;
            }
          }
        }
            k++;
          }
        }

if(oggetto_presente2== false){
         if (giocatori[j]->zaino[u] == prova_spirit_box) {
            prova_spirit_box_trovata = true;
            printf("Hai lasciato la prova spiritbox\n");
            giocatori[j]->zaino[u] = -1;

            srand(time(NULL));
            static int h;
            h= rand() % 2;
            if(h == 0){
              h= spirit_box;
              for(int b=0; b<4; b++){
              if(giocatori[j]->zaino[b] ==-1){
                giocatori[j]->zaino[b]=EMF;
  oggetto_presente2= true;
  break;
              }
            }

            }
            else{
              h= videocamera;
              for (int t=0; t<4; t++){
              if(giocatori[j]->zaino[t] ==-1){
                giocatori[j]->zaino[t]=videocamera;
  oggetto_presente2= true;
  break;
            }
          }

          }
          k++;
}
}

if (oggetto_presente3==false){
           if (giocatori[j]->zaino[u] == prova_videocamera) {
            prova_videocamera_trovata = true;
            printf("Hai lasciato la prova videocamera\n");
            giocatori[j]->zaino[u] = -1;

            srand(time(NULL));
            static int q;
            if(q == 0){
              q= spirit_box;
              for(int b=0; b<4; b++){
              if(giocatori[j]->zaino[b] ==-1){
                giocatori[j]->zaino[b]=EMF;
oggetto_presente3=true;
break;
              }
            }

            }
            else{
              q= videocamera;
              for (int t=0; t<4; t++){
              if(giocatori[j]->zaino[t] ==-1){
                giocatori[j]->zaino[t]=spirit_box;
oggetto_presente3=true;
break;
            }
          }

          }
k++;


}

  }
}

    if (k == 0) {
        printf("Non ci sono prove da lasciare nel caravan.\n");
    }
    giocatori[j]->posizione = pFirst;




//condizione per vincere il gioco
if(prova_EMF_trovata == true && prova_spirit_box_trovata == true && prova_videocamera_trovata == true){
  printf("Tutte le prove si trovano nel caravan i giocatori hanno vinto la partita\n" );



  printf("I giocatori hanno trovato tutte le prove e hanno vinto\n");

  termina_gioco();


}




}





static void raccogli_oggetto(int j,struct Zona_mappa *zona) {


  if(zaino_pieno(j)){
  printf("lo zaino è pieno, non è possibile raccogliere ulteriori oggetti\n");
  return;
  }


for (int r =0; r<4; r++){
//verifica se ce posto nello zaino
  if(giocatori[j]->zaino[r]==-1){
    //vrifica se è presente l'adrenalina
    if (zona->oggetto_zona==adrenalina){

giocatori[j]->zaino[r]= adrenalina;
zona->oggetto_zona=nessun_oggetto;
printf("Hai raccolto l'adrenalina\n" );

break;
}
if (zona->oggetto_zona==cento_dollari){
giocatori[j]->zaino[r]=cento_dollari;
zona->oggetto_zona=nessun_oggetto;
printf("Hai raccolto cento dollari\n" );

break;
}
if(zona->oggetto_zona==coltello){

giocatori[j]->zaino[r]=coltello;
zona->oggetto_zona=nessun_oggetto;
printf("Hai raccolto il coltello\n" );

break;
}

}

}



return ;

}

static void  raccogli_prova(int j,struct Zona_mappa *zona){
  if(zaino_pieno(j)){
  printf("lo zaino è pieno, non è possibile raccogliere ulteriori oggetti\n");
  return;
  }


  static int u=0;

for (int l=0; l<4 ; l++){

    if(zona->prova==prova_EMF) {
   if(giocatori[j]->zaino[l]==EMF){

      giocatori[j]->zaino[l]=prova_EMF;
      zona->prova=-1;
      printf("Hai raccolto la prova EMF\n" );
u++;
prova_raccolta++;
      break;
    }

  }
  if(zona->prova==prova_spirit_box){
    if(giocatori[j]->zaino[l]==spirit_box){

      giocatori[j]->zaino[l]=prova_spirit_box;
      zona->prova=-1;
    printf("Hai raccolto la prova spiritbox\n" );
u++;
    prova_raccolta++;
break;
  }

}
if(zona->prova==prova_videocamera){
  if(giocatori[j]->zaino[l]==videocamera){

    giocatori[j]->zaino[l]=prova_videocamera;
    zona->prova=-1;
    printf("Hai raccolto la prova videocamera\n" );
u++;
prova_raccolta++;
break;

}

}
}

// la variabile u incrementa ogni volta che si raccoglie una prova
if (u>0) {
  genera_fantsama();
  if (fantasma_presente==true) {
    printf("Nella zona è presente il fantasma questo farà in modo che la tua sanità mentale scenderà\n");
  dim_sanita_mentale(j);
  }
}

check_fantasma(j);


return;
}



static void stampa_giocatore(int j) {
printf("Questi sono i valori di tutti i campi del giocatore %s\n",giocatori[j]->nome_giocatore );
printf("Sanità mentale %d\n",giocatori[j]->sanita_mentale);
printf("questo è il contenuto dell zaino\n" );

for(int k=0; k< 4; k++){
switch (giocatori[j]->zaino[k]) {
  case EMF:
  printf("EMF\n" );
  break;
  case spirit_box:
  printf("spirit_box\n" );
  break;
  case videocamera:
  printf("videocamera\n" );
  break;
  case calmanti:
  printf("calmanti\n" );
  break;
  case sale:
  printf("sale\n" );
  break;

    case adrenalina:
    printf("adrenalina\n" );
    break;
    case cento_dollari:
    printf("cento dollari\n" );
    break;
    case coltello:
    printf("coltello\n" );
    break;

  case prova_EMF:
  printf("Prova EMF\n" );
  break;
  case prova_spirit_box:
  printf("Prova spirit_box\n" );
  break;
  case prova_videocamera:
  printf("Prova videocamera\n" );
  break;
  default:
  printf("(vuoto)\n" );
  break;

}

}
j++;
}





static int usa_oggetto(int j){

printf("Questi sono gli oggetti a tua disposizione \n" );

for(int p=0; p<4; p++){
switch (giocatori[j]->zaino[p]) {
  case calmanti:
  printf("calmanti\n" );
  break;
  case sale:
  printf("sale\n" );
  break;
    case adrenalina:
    printf("adrenalina\n" );
    break;
    case cento_dollari:
    printf("cento dollari\n" );
    break;
    case coltello:
    printf("coltello\n" );
    break;
    default:
    printf("No oggetto\n" );
    break;
}
}
static int f;
do{

printf("Cosa vuoi usare\n" );
printf("1- calmanti\n" );
printf("2- sale\n" );
printf("3- adrenalina\n" );
printf("4- cento dollari\n" );
printf("5- coltello\n" );
printf("6 esci dal menu\n" );
scanf("%d",&f);
while ((getchar() !='\n'));

switch (f) {
  case 1:
usa_calmanti(j);
  break;
  case 2:
  usa_sale(j);
  break;
  case 3:
  usa_adrenalina(j);
  break;
  case 4 :
  usa_cento_dollari(j);
  break;
  case 5:
  usa_coltello(j);
  break;
  case 6:
  printf("Uscita dla menu in corso...\n" );
break;
default:
printf("Non puoi utilizzare nessun oggetto\n" );
break;

}


}while(f!=6);

return 0;

}






static int controllo_sanitamentale(void){

  bool tutti_impazziti_tmp = true;

  for(int i=0; i<num_giocatori; i++){
    if (giocatori[i]->sanita_mentale >0 && !giocatori[i]->morto)
    {
      tutti_impazziti_tmp = false;

    }
  }

  if (tutti_impazziti_tmp) {

	tutti_impazziti = true;
    printf("Tutti i giocatori hanno perso, il gioco termina qui\n");
    termina_gioco();
  }

  return 0;
}




 void imposta_gioco(void) {



  static int imput;

static int oggetti_allocati = 0;  // Variabile di controllo per verificare se è stata allocata della memoria per gli oggetti scelti

      printf("Benvenuto/a da questo momento puoi inziare ad impostare il gioco...\n\n" );
    do {  printf("Per prima cosa inserisci il numero di giocatori che puo variare da 1 a 4\n" );
    printf("N.B qualunque altro carattere inserito non permette di impostare il gioco\n" );
  scanf ("%d", &num_giocatori);
  while ((getchar() !='\n'));

  } while(num_giocatori <1 || num_giocatori >4);

  for (int i=0; i<4;i++) {
    giocatori[i]=NULL;
}

for(int i=0; i<num_giocatori; i++){
    giocatori[i]= (Giocatore*) malloc (sizeof (struct Giocatore));
    giocatori[i]->sale_utilizzato= false;
    giocatori[i]->morto = false;
  printf("Inserisci il nome del giocatore %d\n", i+1 );
  scanf("%s", giocatori[i]->nome_giocatore);
  giocatori[i]->sanita_mentale=100;

  }
  for (int i =0; i<4;i++) {
    if (giocatori[i]!=NULL) {
      printf("Giocatore %d : %s\n Sanità mentale: %d\n",i+1, giocatori[i]->nome_giocatore,giocatori[i]->sanita_mentale );
    }



  }
  for (int i = 0; i < num_giocatori; i++) {
      for (int j = 0; j < 4; j++) {
          giocatori[i]->zaino[j] = -1;
      }
  }


  richiedi_livello_difficolta();



  printf("Ora viene scelto casualmente un game master tra i giocatori creati il quale avrà il compito di creare la mappa in seguito...\n" );
  srand(time(NULL));
  int game_master_index = rand() % num_giocatori;
   Giocatore* game_master = giocatori[game_master_index]; //estraggo tra i giocatori creati chi sarà il game master
   printf("Il game master è: %s\n\n", game_master->nome_giocatore);




  printf("Ti stai sempre di più avvicinando all' inizio di questa MoStRuOsA avventura, premere 1 per continuare...\n" );
  do{
      printf("N.B qualunque altro carattere inserito non permette di impostare il gioco\n" );
  scanf("%d", &imput);
  while ((getchar() !='\n'));

  }while (imput!=1) ;

  printf("Operazione in corso ...\n" );
  printf("                                            / °  '             \n");
  printf("                                           )' __/              \n");
  printf("                                           ( ' '               \n");
  printf("                                            _' '__             \n");
  printf("                                          / ,_ '__'_           \n");
  printf("                                   ______/ / ' |    '          \n");
  printf("                                  /   _ '  __) /    |'__       \n");
  printf("                                  '''      ,O__:)     '''      \n");
  printf("                .          .              '     '              \n");
  printf("                 .        .              '       '             \n");
  printf("               .    .    .   .           c        c            \n");
  printf("                 (        (              '        '            \n");
  printf("                ( )      ( )             |        |            \n");
  printf("               __#,    __#,         ...,:o        o:,...       \n");





    printf("Ora è il momento di sceglire che cosa portare nello zaino, scegli con attenizone mi raccomando, premi 1 per continuare...\n" );

    do{
        printf("N.B qualunque altro carattere inserito non permette di impostare il gioco\n" );
    scanf("%d", &imput);
    while ((getchar() !='\n'));

    }while (imput!=1);
    printf("Operazione in corso... \n");
  printf("        _____            \n");
  printf("       |     |           \n");
  printf("       | ° ° |           \n");
  printf("       (_ ^ _)           \n");
  printf("        |mmm|            \n");
  printf("         '''             \n");


    printf("Bene ora puoi scegliere il tuo oggetto \n" );

  srand(time(NULL));

  enum Tipo_oggetto_iniziale oggetti[num_giocatori];
    for (int i = 0; i < num_giocatori; i++) {
      oggetti[i] = rand() % 5;
    }

  // sempre una videocamera tra gli oggetti
    oggetti[rand() % num_giocatori] = videocamera;

  //in questo modo estraggo casualmente i giocatori che possono scegliere cosa mettere all'interno del proprio zaino.
    int indici[num_giocatori];
     for (int i = 0; i < num_giocatori; i++) {
       indici[i] = i;
     }


     for (int i = num_giocatori - 1; i > 0; i--) {
       int j = rand() % (i + 1);
       int temp = indici[i];
       indici[i] = indici[j];
       indici[j] = temp;
     }

     // Alloca dinamicamente l'array di interi in questo modo tengo traccia degli oggetti scelti
      enum Tipo_oggetto_iniziale *oggetti_scelti = calloc(num_giocatori, sizeof(enum Tipo_oggetto_iniziale));
      if (oggetti_scelti != NULL) {
          oggetti_allocati = 1;  // La memoria è stata allocata con successo
      }


     for (int i = 0; i < num_giocatori; i++) {

       int indice_giocatori=indici[i];

       //enum Tipo_oggetto_iniziale scelta;

       static int scelta;
       while (1) {
         printf(" %s, scegli un oggetto da mettere nel tuo zaino (0-4):\n",giocatori[indice_giocatori]->nome_giocatore);
         for (int j = 0; j < num_giocatori; j++) {

           // Verifica se l'oggetto è stato già scelto
           if (oggetti_scelti[j]) {
             printf("  * ");  // Stampa un segno di asterisco per indicare che l'oggetto è stato già scelto
           }
           printf("%d: %s\n", j, oggetti[j] == EMF ? "EMF" : oggetti[j] == spirit_box ? "spirit box" : oggetti[j] == videocamera ? "videocamera" : oggetti[j] == sale ? "sale" : "calmanti" );
         }
         if (scanf("%d", &scelta) == 1 && scelta >= 0 && scelta < num_giocatori && !oggetti_scelti[scelta]) {
           // Se l'input è valido e l'oggetto non è stato ancora scelto, esci dal ciclo
           giocatori[indice_giocatori]->zaino[0] = oggetti[scelta];

           break;
         }
         printf("Input non valido o oggetto già scelto *, per favore scegli un altro oggetto.\n");
         // Svuota il buffer di input per evitare che il ciclo continui a ripetersi all'infinito
         int c;
         while ((c = getchar()) != '\n' && c != EOF);
       }


       oggetti_scelti[scelta] = 1;  // Imposta l'oggetto come scelto


indice_giocatori++;


}
stampa_zaino();

printf("Ora è il momento di creare la mappa, premere 2 per continuare\n");
do{
        printf("N.B qualunque altro carattere inserito non permette di impostare il gioco\n" );
    scanf("%d", &imput);
    while ((getchar() !='\n'));

  }while (imput!=2);
    printf("Operazione in corso... \n");
  printf("        _____            \n");
  printf("       |     |           \n");
  printf("       | ° ° |           \n");
  printf("       (_ ^ _)           \n");
  printf("        |mmm|            \n");
  printf("         '''             \n");


  crea_zona(cucina);
  crea_zona(soggiorno);
  crea_zona(camera);
  crea_zona(bagno);
  crea_zona(garage);
  crea_zona(seminterrato);
  crea_caravan();


printf("Tieni bene a mente che questa è la mappa di default del gioco che puoi modificare tramite questo menu per rendere la tua avventura ancora più avvincente\n" );
  printf("%s sei il game master scegli come meglio credi di modificare la mappa\n",game_master->nome_giocatore );
stampa_mappa();

//menu per creare e modificare la mappa
static int option;
do {

    printf("Questo è il menù per la creazione della mappa\n");
    printf(" ◊1 Inserimeto di una zona al termine \n");
    printf(" ◊2 Cancella ultima zona inserita\n");
    printf(" ◊3 Visualizza mappa\n");
    printf(" ◊4 Creazione mappa finita\n");

    if (scanf("%d", &option) != 1) {
        // L'input non è un numero, quindi genero errore
        printf("ATTENZIONE IL TASTO INSERITO NON COINCIDE CON QUELLI DEL MENU' PREGO INSERIRE UN NUMERO DA 1 A 4...\n");
        // Pulisco il buffer di input per evitare problemi nelle iterazioni successive
        while ((getchar() !='\n'));
        continue;
    }

    switch (option) {
        case 1:
            inserisci_zona();
          //inserisci_zona2();
            break;
        case 2:

            cancella_zona();
            //cancella_zona2();
            break;
        case 3:
            stampa_mappa();

            break;
        case 4:
            chiudi_mappa();

            printf("Bene la modifica della mappa ha avuto successo\n" );
            break;
        default:
            printf("ATTENZIONE IL TASTO INSERITO NON COINCIDE CON QUELLI DEL MENU' PREGO INSERIRE UN NUMERO DA 1 A 4...\n");
    }

} while (option !=4);
static int reimposto;
static int z;
printf("Bene ora desideri giocare o preferisci reimpostare il gioco, premere 1 per reimpostarlo alrimenti premere 2 se si vuole iniziare a giocare\n");
do{
scanf("%d",&reimposto);

if(reimposto==1){
  printf("Bene hai scelto di reimpostare il gioco\n" );
  libera_memoria(giocatori,oggetti_scelti,oggetti_allocati); //libero la memoria allocata per i giocatorei
  libera_memoria_zone();// libero la memoria allocata per le zone

  printf("Operazione in corso... stanno per essere cancellate tutte le impostazione precedentemente scelte\n" );
  printf("Bene ora puoi reimpostare il gioco premere 1 prego...\n" );
  do{

  scanf("%d",&z );
  if(z==1)
  imposta_gioco();
}while(z!=1);
}

if (reimposto==2)
{
  printf("Bene sei pronto per giocare\n");
  gioca ();
}
}while(reimposto< 1 || reimposto > 2);
}






    //fine dell'impostazione del gioco inizio della funzione gioca().
void gioca(void) {



  if(!mappa_creata){
    printf("La mappa non è stata creata, impossibile giocare!!\n" );
    return;
  }
  else{
  // Imposta tutti i giocatori alla prima zona

struct Zona_mappa* p = pFirst;
for (int j = 0; j < num_giocatori; j++) {
if (giocatori[j] != NULL) {
giocatori[j]->posizione = p;
}
}

printf("Bene tutti i giocatori inizieranno a giocare dalla prima posizione della mappa ovvero la cucina tenetevi pronti...\n");


// Inizializza il generatore di numeri casuali utilizzando il tempo corrente come seed
 srand(time(NULL));

 // Crea un array di indice per contenere l'ordine casuale dei giocatori
 int indice[num_giocatori];
 for (int i= 0; i < num_giocatori; i++) {
     indice[i] = i;
 }



 // Shuffle l'array di indice utilizzando la funzione rand()
 for (int i = num_giocatori - 1; i > 0; i--) {
     // Genera un numero casuale compreso tra 0 e i (inclusi)
     int j = rand() % (i + 1);
     // Scambia l'elemento i con l'elemento j nell'array di indice
     int temp = indice[i];
     indice[i] = indice[j];
     indice[j] = temp;
 }



static int k;

while (!(prova_EMF_trovata == true && prova_spirit_box_trovata == true && prova_videocamera_trovata == true) || (!tutti_impazziti)) {
do
{





usato_1 = false; // variabili reimpostate a 0 per gli oggetti utilizzati
usato_2 = false;
usato_3 = false;
usato_4 = false;
usato_5 = false;
oggetto_assegnato = false;

controllo_sanitamentale(); // controlla se i giocatori sono ancora in vita e se sono impazziti



for(int i =0; i<num_giocatori; i++){
  int j =indice[i];
  if (giocatori[j]->sanita_mentale <=0 || giocatori[j]->morto == true)
  {

    printf("il giocatore %s non può più giocare\n", giocatori[j]->nome_giocatore );


    continue;

  }

genera_fantsama();// genera ogni turno un fantasma in una zona diversa in base a determinate probabilità




  printf("Benvenuto nel menu di gioco, cosa vuoi fare??\n");
  printf("Questo è il turno di : %s\n", giocatori[j]->nome_giocatore );
  printf("(1) Tornare al caravan \n");
  printf("(2) Visualizzare i valori di tutti i campi del giocatore\n");
  printf("(3) Visualizzare i valori di tutti i campi della zona in cui ti trovi \n");
  printf("(4) Avanzare \n");
  printf("(5) Raccogli una prova\n");
  printf("(6) Raccogli un oggetto\n");
  printf("(7) Usa un oggetto a tua disposizione\n");
  printf("(8) Passa turno\n");
  printf("(9) Avanza di  due posizioni\n" );

  if(scanf("%d", &k)!=1){


  // L'input non è un numero, quindi genero errore
  printf("ATTENZIONE IL TASTO INSERITO NON COINCIDE CON QUELLI DEL MENU' PREGO INSERIRE UN NUMERO DA 1 A 4...\n");
  // Pulisco il buffer di input per evitare problemi nelle iterazioni successive
  while ((getchar() !='\n'));
  continue;
}

      switch (k) {
          case 1:
           torna_caravan(j);
           sanita__mentale(j);
           break;
          case 2:
           stampa_giocatore(j);
          
           break;

          case 3:
            stampa_zona(j,giocatori[j]->posizione);
            break;

          case 4:
            avanza(j,giocatori[j]->posizione);
            sanita__mentale(j);

            break;
          case 5:
           raccogli_prova(j,giocatori[j]->posizione);
           sanita__mentale(j);

            break;
          case 6:
           raccogli_oggetto(j,giocatori[j]->posizione);
           sanita__mentale(j);

            break;
          case 7:
           usa_oggetto(j);

            break;
          case 8:
           passa();
            break;
            case 9:
            avanza2(j,giocatori[j]->posizione);
            break;


           default:
           printf("ATTENZIONE IL TASTO INSERITO NON COINCIDE CON QUELLI DEL MENU' PREGO INSERIRE UN NUMERO DA 1 A 8...\n");
           break;


  }







}


}while(k <=1 || k >=9);
}
}

}
