#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void imposta_gioco(void); // funzione che permette di iniziare ad impostare il gioco

void gioca (void); // funzione che permette di gioccare

void termina_gioco (void); // funzione che permette di terminare il gioco
typedef struct Giocatore {

unsigned char nome_giocatore [30];
unsigned char sanita_mentale;
struct Zona_mappa* posizione;
char zaino [4];
bool sale_utilizzato;// variabile per tenere traccia del sale utilizzato dai giocatori
bool morto;// variabile per controllare la morte dei giocatori
} Giocatore;


enum Tipo_oggetto_iniziale {

  EMF=0,
  spirit_box=1,
  videocamera=2,
  calmanti=3,
  sale=4,




};


enum Tipo_oggetto_zona {

  adrenalina=5,
  cento_dollari=6,
  coltello=7,
  nessun_oggetto=8

};


enum Tipo_zona {

  caravan,
  cucina,
  soggiorno,
  camera,
  bagno,
  garage,
  seminterrato

};


enum Tipo_prova {

prova_EMF=9,
prova_spirit_box=10,
prova_videocamera=11,
nessuna_prova=12

};




struct Zona_mappa {

  enum Tipo_zona zona;
  enum Tipo_oggetto_zona oggetto_zona;
  enum Tipo_prova prova;
  struct Zona_mappa* prossima_zona;

};
