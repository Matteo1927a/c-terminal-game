#include <stdio.h>
#include <string.h>
#include "gamelib.h"
#include <stdbool.h>
void  grafica_menu(void) {


printf(" __         __      __   __   __        __   __   .  __   \n" );
printf("|__| |__|  |__| |  |__  |  | |__| |__| |  | |__|  | |__| \n" );
printf("|    |  |  |  | |__ __| |__| |    |  | |__| |___| | |  |\n" );
printf("    (     (    _____   (    (     (     (    (    (    (     \n");
printf("     )    )   |     |   )    )     )   )      )     )    )    \n");
printf("    (    (    | ◊ ◊ | (    (      (     )    (    (    (      \n");
printf("     )   (    (_ V _)  )    )      )  (   )    )    )    )  ( \n");
printf("|--------------|mmm|------------| (     (     ______  (   )   \n" );
printf("|              '---'            |  )     )  )|o     |    )    \n" );
printf("| » 1 Imposta gico              |    )   (  )|      |  (   )  \n" );
printf("| » 2 Gioca                     |  (    (    |      |         \n" );
printf("| » 3 Termina gioco             | (      )   |      |  (  (   \n" );
printf("|                               |   )  (    (|____  |     )   \n" );
printf("|                               |    )   )   )    | |   (   ) \n" );
printf("|-------------------------------|   )  (   (      |_|    (    \n" );


}

int main (void) {
int opzione;
bool gioco_impostato= false;
printf("                  BENVENUTO A....\n");
grafica_menu();


do {
if(scanf("%d",&opzione)!=1){
  printf("ATTENZIONE!!! IL TASTO INSERITO NON COINCIDE CON LE INDICAZIONI DEL MENU'!!\n" );
while ((getchar() !='\n'));
continue;
}
switch (opzione) {
  case 1:
imposta_gioco();
gioco_impostato=true;

  break;
  case 2:
  if(gioco_impostato) {

    gioca();

  }
else{
printf("Per favore, imposta il gioco prima di iniziare a giocare, premere 1 per iniziare a giocare...\n" );

}
    break;
  case 3:
if(gioco_impostato){
termina_gioco();

}
else{
  printf("Come puoi terminare prima di inziare\n" );
}

    break;
    default:

    printf("ATTENZIONE!!! IL TASTO INSERITO NON COINCIDE CON LE INDICAZIONI DEL MENU'!! INSERIRE UN NUMERO DA 1 A 3 PER CONTINUARE...\n" );
    grafica_menu();
    break;

  }

}

while(opzione !=3);

return 0;
}
