#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define ancho 17
#define alto 12
#define velocidad 900

char tetrominos[7][4][4] ={ //Se declaran los tetrominos
	{
		"..x.",//I
		"..x.",
		"..x.",
		"..x.",
	},
	{
		"....",//O
		".xx.",
		".xx.",
		"....",
	},
	{
		"..x.",//S
		".xx.",
		".x..",
		"....",
	},
	{
		".x..",//Z
		".xx.",
		"..x.",
		"....",
	},
	{
		".x..",//T
		".xx.",
		".x..",
		"...."
	},
	{
		".x..",//L
		".x..",
		".xx.",
		"...."
	},
	{
		"..x.",//J
		"..x.",
		".xx.",
		"...."
	}
};


/*FUNCIONES DEL JUEGO*/
int rotar(int x,int y,int r){ /*Funcion que rota la pieza*/
	int i=0;
	switch(r)
	{
		case 1:
            i=3 -y +(4*x);
			break;
		case 2:
			i=15-(y*4)-x;
			break;
		case 3:
			i=12+y-(x*4);
			break;
		case 4:
            i=y*4 +x;
			break;
	}
	return i;
}

void crear_pieza(int actualx,int actualy,int ti,int r,int pi[17],char tets[17]){
	int s=0;
	for (int y=0; y<4; y++){
		for(int x=0; x<4; x++){
			int i=(actualy + y)*ancho +(actualx+x);
			int j=rotar(x,y,r);
			pi[s]=i;
			s++;
			tets[j]=tetrominos[ti][y][x];
		}
	}
}

void copiar_pieza(char tets[17],int pi[17],unsigned char pos[ancho*alto]){//ES MATRIZ POS???????????????
	for(int j=0; j<16; j++){
		if(tets[j]=='x'){
			pos[pi[j]]=tets[j];
		}
	}
}

void borde(unsigned char pos[ancho * alto]) { /*Funcion que crea el borde*/

	for (int y = 0; y < alto; y++) {
		for (int x=0; x<ancho; x++)	{
			if ( x == 0 || x == ancho - 2 || y == alto - 1) { //2 para las paredes, 1 para el piso
				pos[y * ancho + x] =186; //Crea el borde y base con ASCII
			}
			else if(pos[y * ancho + x] != 'x') {
				pos[y * ancho + x]='.';
			}
		}
		pos[y*ancho + ancho-1]='\n';
	}
}

void poner(unsigned char pos[ancho*alto],int pi[17]){ //Guarda la pieza en el tablero?????
	for(int i=0; i<16; i++){
		if(pos[pi[i]]=='x'){
			pos[pi[i]]='.';
		}
	}
}

void guardar(unsigned char pos[ancho * alto],int pi[17]){ //Junta las piezas que hay en el tablero con la que cayo o el piso y cambia el simbolo para diferenciar las que ya cayeron
	for (int i=0; i<16; i++){
		if(pos[pi[i]]=='x'){
			pos[pi[i]]=254;
		}
	}
}

int chequear(int actualx,int actualy,int ti,int rotation,char tets[17],unsigned char pos[ancho*alto], int pi[17]){
	crear_pieza(actualx,actualy,ti,rotation, pi, tets);
	for(int i=0; i<16; i++){
		if(tets[i]!='.' && pos[pi[i]]!='.'){
			return 0;
		}
	}
	return 1;
}

void lineallena(int actualy,unsigned char pos[ancho*alto], int nivel, int puntaje){
	int contador=0;
	for(int y=0; y<4; y++){
		if((actualy+y) < (alto-1)){
			int bandera=1;
			for(int x=1; x<ancho-2; x++){
				bandera &=(pos[(actualy+y)*ancho + x]!='.');
			}
			if(bandera){
				for(int x=1; x<ancho-2; x++){
					pos[(actualy+y)*ancho+ x]='.';
				}
			}
			if(bandera){
				for(int yy=actualy+y; yy>1; yy--){
					for(int xx=1; xx<ancho-2; xx++){
						pos[(yy)*ancho + xx]=pos[(yy-1)*ancho +xx];
					}
				}
			}
		}
	}
}

void tetromino_azar(int grupo[7]){ //Va a generar 7 tetrominos al azar
	for (int i=0; i<7; i++){
		grupo[i]=rand()%7;
	}
}

int guardar_tetromino(int grupo[7]){ //Retorna el primer tetromino al azar y desplaza el arreglo hacia la izquierda //Sirve tambien para ayudar a la funcion proxima_pieza
     //Si no está, se crea solo un tetromino
	int tt=grupo[0];
	for(int i=0; i<6; i++){
		grupo[i]=grupo[i+1];
	}
	return tt;
}

void proxima_pieza(int grupo[7]){ //Funcion que muestra la pieza que pieza viene despues

	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
				printf("%c", tetrominos[grupo[1]][i][j]);
				}
			printf("\n");
		}
	int contador=0;
	int b=grupo[0+contador];
	contador++;
}


/* FUNCIONES QUE DAN COLOR A LOS PRINTF */
void colorazul() {
  printf("\033[0;34m");
}

void colorrojo() {
	printf("\033[1;31m");
}

void coloramarillo() {
  printf("\033[1;33m");
}

void colorverde() {
	printf("\033[0;32m");
}

void colorbase() {
  printf("\033[0m");
}


int jugardenuevo(){ //Funcion que permite volver a jugar el tetris
	char c;
	do
	{
		getchar();
		colorazul();
		printf("\nQuieres jugar de nuevo?:");
		colorbase();
		colorrojo(); 
		printf("\nOPCIONES");
		colorbase();
		printf("\ns->SI o n->NO: ");
		scanf("%c",&c);
	}while(c!='s' && c!='n');
	if(c=='s')
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

int main() {
	
	int pi[17];
	unsigned char pos[ancho * alto];
	char tets[17];
	int grupo[7];
	int bandera=1;
	int nivel=0;
	int opcion;
	int puntaje=0;
	coloramarillo();
	printf("------------------------------------------\n");
	printf("#####  #####  ######  #####   #####  #####\n");
	printf("  #    #        #     #   #     #    #    \n");
	printf("  #    #####    #     # ##      #    #####\n");
	printf("  #    #        #     #  #      #        #\n");
	printf("  #    #####    #     #   #   #####  #####\n");
	printf("------------------------------------------\n");
	printf("        #   #  ##      #####  #####       \n");
	printf("        #   #  #  ##   #      #           \n");
	printf("        #   #  #    #  ###    #           \n");
	printf("        #   #  #  ##   #      #           \n");
	printf("        #####  ##      #####  #####       \n");
	printf("------------------------------------------\n");
	printf("               cargando...                \n");
	colorbase();
	Sleep(5000);
	system("cls");
	do{
		coloramarillo();
		printf( "\n                  MENU TETRIS UDEC           ");
		colorbase();
		printf( "\n   1. Empezar a jugar.");
        printf( "\n   2. Creditos.");
        printf( "\n   3. Salir." );
		do
        {
			colorazul();
            printf( "\n   Introduzca opcion (1-3): ");
			colorbase();
            scanf( "%d", &opcion );

        } while ( opcion < 1 || opcion > 3 );
		switch(opcion){

			case 1: // ---------LUGAR: Todo lo del juego y su logica ------------
			do{
				srand(time(0));
				system("cls");
				puntaje=0;
				char tecla;
				borde(pos);
				system("cls");
				printf("%s",pos);
				int finjuego=1;
				int velocidad_actual=velocidad;
				while(finjuego==1){
					int actualx=ancho/2-2;
					int actualy=0;
					int rotacion=1;
					/*Aqui se crean las piezas al azar*/
					tetromino_azar(grupo);
					int ti=guardar_tetromino(grupo);//MMMMMMM
					printf("\nSIGUIENTE PIEZA: ");
					printf("\n");
					proxima_pieza(grupo);
					colorrojo();
					printf("\nPUNTOS: %d",puntaje);
					Sleep(5000);
					printf("\n");
					colorbase();
					//aqui no va puntaje+10
					while(1){
						/*Necesario para contar el tiempo - time.h*/
						clock_t reloj = clock();     
						
						do{
							if (_kbhit() ){ /*Funcion de conio.h*/
							int banderasalida=1;
							//Hubo que usar _getch porque getch no funcionaba
							//Las teclas de direccion necesitan dos getch, uno para el char y otro para el valor numerico
							tecla=_getch();
							if(tecla==0 || tecla==224)
							{
								tecla=256+_getch();
							}
							if(tecla==72)
							{
								rotacion++;
								if(rotacion==5)
								{
									rotacion=1;
								}
								if(!chequear(actualx,actualy,ti,rotacion,tets,pos, pi))
								{
									rotacion--;
								}
							}
							else if(tecla==75){
								actualx--;
								if(!chequear(actualx,actualy,ti,rotacion,tets,pos, pi))
								{
									actualx++;
								}
							}
							else if(tecla==77){
								actualx++;
								if(!chequear(actualx,actualy,ti,rotacion,tets,pos, pi))
								{
									actualx--;
								}
							}
							else if(tecla==27){
								if(banderasalida==1){
									banderasalida=0;
									_getch();
								} else {
									break;
									}
									} else if(tecla==80){
										break;
									} else {
										continue;
									}
									crear_pieza(actualx,actualy,ti,rotacion,pi,tets);
									copiar_pieza(tets, pi, pos);
									system("cls");
									printf("%s",pos);
									poner(pos, pi);
									//aqui no va puntaje
									}
									}while(clock() < (reloj + velocidad_actual));
									if(chequear(actualx,actualy+1,ti,rotacion,tets, pos, pi))
									{
										actualy++;
										}
										else{
											crear_pieza(actualx,actualy,ti,rotacion,pi,tets);
											finjuego=chequear(actualx,actualy,ti,rotacion,tets,pos, pi);
											copiar_pieza(tets,pi,pos);
											guardar(pos,pi);
											system("cls");
											printf("%s",pos);
											//aqui tampoco va puntaje+10
											lineallena(actualy, pos, nivel, puntaje);
											break;
											//aqui tampoco va puntaje
											}
											crear_pieza(actualx,actualy,ti,rotacion,pi,tets);
											copiar_pieza(tets,pi,pos);
											system("cls");
											printf("%s",pos);
											poner(pos,pi);
											//aqui no va puntaje+10
											}
											puntaje +=1; //Por cada pieza que toca el fondo del tablero o a otra pieza
											}
											//system("cls");
											coloramarillo();
											printf("\n          Se termino el juego F");
											Sleep(3000);
											system("cls");
											colorbase();
											if(jugardenuevo()==1){
												bandera=1;
											} else {
												bandera=0;
											}
											system("cls");
											}while(bandera!=0);
											break;

 			case 2: //Los creditos
			 system("cls");
			 printf("------------------------------------------------------\n");
             printf("#####  #####  #####  ####   #####  #####  #####  #####\n");
             printf("#      #   #  #      #   #    #      #    #   #  #    \n");
             printf("#      #####  #####  #   #    #      #    #   #  #####\n");
             printf("#      #   #  #      #   #    #      #    #   #      #\n");
             printf("#####  #   #  #####  ####   #####    #    #####  #####\n");
             printf("------------------------------------------------------\n");
             printf("----Proyecto Final de la asignatura Programacion I----\n");
             printf("-------------Videojuego realizado en lenguaje C-------\n");
             printf("---Autores:Vicente Tuki-Vicente Rodriguez-Jose Toledo--\n");
			 printf("------------------Agradecimientos a:-------------------\n");
			 printf("----------Profesor: Roberto Javier Asin Acha.----------\n");
			 printf("----------Ayudante: Fabian Cid.------------------------\n");
			 printf("------que nos fue de gran ayuda y nos facilito---------\n");
			 printf("---------en la realizacion del proyecto Tetris.-------\n");
			 Sleep(10000);
			 system("cls");
		}
		system("cls");
	}while(opcion!=3);
	
	return 0;
}
