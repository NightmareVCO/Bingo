// Vladimir Osvaldo Curiel Ovalles 1014-1415.
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <conio.c>
#include <time.h>

#define INITIAL 0   // Valor inicial de los tableros. (constante)
#define DIF 15      // Diferencia entre las letras de los bolos.(Constante)
#define DIST 3      // Valor entre los valores del carton. (Constante)
#define YP 6        // Posición inicial de y. (Modificable)
#define XP 20       // Para mantener la distancia entre tablas de jugadores. (Modificable)
#define FREE 0      // Valor para indicar los bolos que no se han usado. (evitar repeticiones)
#define USED 1      // Valor para indicar que el status del un numero del bingo ya ha salido.
#define WIN 5       // Numero para definir al ganar. (constante)
#define MAXCOL 5    // Cantidad de valores que tendrá los arreglos del bingo (constante)
#define MAXBOLO 76  // El mayor valor de bolo que puede salir (será 75)
#define MAXBOARDS 4 // Cantidad limite de cartones por jugador. (Modificable)
#define XPOS 35     // Controlar la posición de los textos en pantalla en eje X. (Modificables)
#define YPOS 4      // Controlar la posición de los textos en pantalla en eje Y. (Modificables)

#define ESC 27
#define ENTER 13

#define TRUE 1
#define FALSE 0

typedef struct
{
   int value;
   int status;
} BOXES;

typedef struct
{
   BOXES B[MAXCOL],
       I[MAXCOL],
       N[MAXCOL],
       G[MAXCOL],
       O[MAXCOL];
} BOARD;

typedef struct
{
   int quanBoards;
   BOARD *boards;
} GAME;

int verifNumber(GAME *, int, int);
int randRange(int, int);
int getBolo(int[]);
int verifWin(GAME *, int);
int verifWinHor(GAME *, int);
int verifWinVer(GAME *, int);
int verifWinDiag(GAME *, int);
int verifFullWin(GAME *, int);
void verifPlay(GAME *, int);
void showBoards(GAME *, int, int[]);
void fillBoards(GAME *, int);
void setColor(int, int);
void showBolo(int);
void showTimeXY(int, int, int);
void colorDefault(void);
void winner(GAME *, int, int);
void gameBingo(GAME *, int);
void showWinners(int[], int);

int main()
{
   GAME *players;
   int quanPlayers;
   srand(time(NULL));

   printf("Cantidad de jugadores: ");
   scanf("%d", &quanPlayers);
   players = (GAME *)malloc(quanPlayers * sizeof(GAME));

   gameBingo(players, quanPlayers);
   free(players);

   return 0;
}
/*
   Función:    gameBingo
   Argumentos: GAME *, estructura a usar.
               int quanPlayers, cantidad de jugadores.
   Objetivo:   Llevar el juego.
   Retorno:    Ninguno.
*/
void gameBingo(GAME *players, int quanPlayers)
{
   int seg, key, bolo, full = 0, win = 0, bolos[MAXBOLO], winners[quanPlayers], fullPlayers[quanPlayers];
   bolo = getBolo(bolos);
   // Bucle para inicializar las victorias en 0.
   for (int pos = 0; pos < quanPlayers; pos++)
      winners[pos] = 0;

   do
   {
      seg = time(NULL);
      // Bucle para preguntar la cantidad de cartones por jugador.
      for (int pos = 0; pos < quanPlayers; pos++)
      {
         do
         {
            printf("Cantidad de cartones jugador #%d: ", pos + 1);
            scanf("%d", &(players + pos)->quanBoards);
            if ((players + pos)->quanBoards > MAXBOARDS)
               printf("El limite de cartones es 4\n");
         } while ((players + pos)->quanBoards > MAXBOARDS);
         (players + pos)->boards = (BOARD *)malloc((players + pos)->quanBoards * sizeof(BOARD)); // Dimensionamos todo el espacio para cada jugador.
      }
      // Bucle para decidir los jugadores a evaluar. Inicia evaluándolos a todos, más adelante cambiara individualmente.
      for (int pos = 0; pos < quanPlayers; pos++)
         fullPlayers[pos] = TRUE;
      fillBoards(players, quanPlayers);
      system("cls");
      showBoards(players, quanPlayers, fullPlayers);
      // Asignación de bolos libres
      for (int pos = 0; pos < MAXBOLO; pos++)
         bolos[pos] = FREE;
      win = 0;
      full = 0;
      do
      {
         showTimeXY(time(NULL) - seg, XPOS, YPOS - 3);
         _setcursortype(0);
         do
         {
            fflush(stdin);
            key = tolower(getch());
         } while (key != ENTER && key != ESC);
         if (key == ESC)
         {
            system("cls");
            showWinners(winners, quanPlayers);
            system("pause");
            system("cls");
         }
         if (key == ENTER)
         {
            bolo = getBolo(bolos);
            showBolo(bolo);
            for (int pos = 0; pos < quanPlayers; pos++)
               verifPlay((players + pos), bolo);
            showBoards(players, quanPlayers, fullPlayers);
            for (int pos = 0; pos < quanPlayers; pos++)
            {
               if (verifWin((players + pos), pos))
               {
                  win = 1;
                  winners[pos]++;
                  // break;
               }
            }
         }
      } while (!win);
      win = 0;
      gotoxy(XPOS, YPOS);
      printf("%cDesean jugar a carton completo%c (s)%c %c (n)o", 168, 63, 161, 162);
      do
      {
         fflush(stdin);
         key = tolower(getch());
      } while (key != 's' && key != 'n');
      if (key == 's')
      {
         gotoxy(XPOS, YPOS);
         printf("                                                                    ");
         for (int pos = 0; pos < quanPlayers; pos++)
         {
            gotoxy(XPOS, YPOS);
            printf("%cJugador #%d%c (s)%c %c (n)o", 168, pos + 1, 63, 161, 162);
            do
            {
               fflush(stdin);
               key = tolower(getch());
            } while (key != 's' && key != 'n');
            // Se modificara el arreglo que se lleno al principio para solo imprimir los jugadores que van a jugar a carton completo.
            if (key == 's')
            {
               fullPlayers[pos] = 1;
               full = 1;
            }
            else if (key == 'n')
               fullPlayers[pos] = 0;
         }
      }
      gotoxy(XPOS, YPOS);
      system("cls");
      // Version de las cosas que debe hacer pero con los jugadores que dijeron que sí.
      if (full)
      {
         do
         {
            showTimeXY(time(NULL) - seg, XPOS, YPOS - 3);
            _setcursortype(0);
            do
            {
               fflush(stdin);
               key = tolower(getch());
            } while (key != ENTER && key != ESC);
            if (key == ESC)
            {
               system("cls");
               showWinners(winners, quanPlayers);
               system("pause");
               system("cls");
            }
            if (key == ENTER)
            {
               bolo = getBolo(bolos);
               showBolo(bolo);
               for (int pos = 0; pos < quanPlayers; pos++)
                  if (fullPlayers[pos])
                     verifPlay((players + pos), bolo);

               showBoards(players, quanPlayers, fullPlayers);

               for (int pos = 0; pos < quanPlayers; pos++)
               {
                  if (verifFullWin((players + pos), pos))
                  {
                     win = 1;
                     winners[pos]++;
                  }
               }
            }
         } while (!win);
      }
      gotoxy(XPOS, YPOS);
      printf("%cDesea jugar otra vez%c (s)%c %c (n)o", 168, 63, 161, 162);
      do
      {
         fflush(stdin);
         key = tolower(getch());
      } while (key != 's' && key != 'n');
      if (key == 's')
         for (int pos = 0; pos < quanPlayers; pos++)
            free((players + pos)->boards); // Limpia la memoria de los cartones.
      system("cls");
   } while (key != 'n');
   gotoxy(30, 10);
   setColor(YELLOW, RED);
   printf("%cGracias por jugar al bingo, vuelva pronto%c", 173, 33);
   sleep(2);
}
/*
   Función:    fillBoards
   Argumentos: GAME *, estructura a evaluar.
               int quanPlayers, cantidad de jugadores.
   Objetivo:   Llenar los tableros con valores aleatorios.
   Retorno:    Ninguno.
*/
void fillBoards(GAME *players, int quanPlayers)
{
   int value;
   for (int pos = 0; pos < quanPlayers; pos++)                    // # de Jugador
      for (int ind = 0; ind < (players + pos)->quanBoards; ind++) // Carton del jugador #
         for (int arg = 0; arg < MAXCOL; arg++)                   // valor de arreglo.
         {
            do
            {
               value = randRange(1, DIF) + 0 * DIF;
            } while (!verifNumber((players + pos), value, ind));
            (players + pos)->boards[ind].B[arg].value = value;
            do
            {
               value = randRange(1, DIF) + 1 * DIF;
            } while (!verifNumber((players + pos), value, ind));
            (players + pos)->boards[ind].I[arg].value = value;
            do
            {
               value = randRange(1, DIF) + 2 * DIF;
            } while (!verifNumber((players + pos), value, ind));
            (players + pos)->boards[ind].N[arg].value = value;
            do
            {
               value = randRange(1, DIF) + 3 * DIF;
            } while (!verifNumber((players + pos), value, ind));
            (players + pos)->boards[ind].G[arg].value = value;
            do
            {
               value = randRange(1, DIF) + 4 * DIF;
            } while (!verifNumber((players + pos), value, ind));
            (players + pos)->boards[ind].O[arg].value = value;
            // Para asegurarnos de que el centro estará lleno.
            (players + pos)->boards[ind].N[2].value = INITIAL;
            (players + pos)->boards[ind].N[2].status = USED;
         }
   return;
}
/*
   Función:    showBoards
   Argumentos: GAME *, estructura a evaluar.
               int quanPlayers, cantidad de jugadores.
               int full[], verificador para imprimir jugadores.
   Objetivo:   Llenar los tableros con valores aleatorios.
   Retorno:    Ninguno.
*/
void showBoards(GAME *players, int quanPlayers, int full[])
{
   int xpos, ypos = YP, def;
   for (int pos = 0; pos < quanPlayers; pos++)
   {
      if (full[pos])
      {
         for (int ind = 0; ind < (players + pos)->quanBoards; ind++)
         {
            // Para definir al jugador en posición.
            xpos = XP * (pos + 1);
            // Para mantener la distancia entre tablas de jugadores.
            def = xpos;
            gotoxy(xpos, ypos);
            setColor(CYAN, BLACK);
            printf("      %d-%d", pos + 1, ind + 1);
            ypos++;
            gotoxy(xpos, ypos);
            setColor(RED, WHITE);
            printf(" B  I  N  G  O ");
            ypos++;
            for (int arg = 0; arg < MAXCOL; arg++)
            {
               gotoxy(xpos, ypos);
               if ((players + pos)->boards[ind].B[arg].status != USED)
                  setColor(BLUE, LIGHTGRAY);
               else if ((players + pos)->boards[ind].B[arg].status == USED)
                  setColor(BLUE, YELLOW);
               printf("%3d", (players + pos)->boards[ind].B[arg].value);

               xpos += DIST; // Movimiento en X deseado, controlado por DIST
               gotoxy(xpos, ypos);

               if ((players + pos)->boards[ind].I[arg].status != USED)
                  setColor(BLUE, LIGHTGRAY);
               else if ((players + pos)->boards[ind].I[arg].status == USED)
                  setColor(BLUE, YELLOW);
               printf("%3d", (players + pos)->boards[ind].I[arg].value);

               xpos += DIST;
               gotoxy(xpos, ypos);

               if ((players + pos)->boards[ind].N[arg].status != USED)
                  setColor(BLUE, LIGHTGRAY);
               else if ((players + pos)->boards[ind].N[arg].status == USED)
                  setColor(BLUE, YELLOW);
               if ((players + pos)->boards[ind].N[arg].value == INITIAL)
                  setColor(RED, RED);

               printf("%3d", (players + pos)->boards[ind].N[arg].value);

               xpos += DIST;
               gotoxy(xpos, ypos);

               if ((players + pos)->boards[ind].G[arg].status != USED)
                  setColor(BLUE, LIGHTGRAY);
               else if ((players + pos)->boards[ind].G[arg].status == USED)
                  setColor(BLUE, YELLOW);
               printf("%3d", (players + pos)->boards[ind].G[arg].value);

               xpos += DIST;
               gotoxy(xpos, ypos);

               if ((players + pos)->boards[ind].O[arg].status != USED)
                  setColor(BLUE, LIGHTGRAY);
               else if ((players + pos)->boards[ind].O[arg].status == USED)
                  setColor(BLUE, YELLOW);
               printf("%3d", (players + pos)->boards[ind].O[arg].value);

               // Para cuadrar cada tabla donde va
               xpos = def;
               ypos++;
            }
            colorDefault();
         }
         // Reiniciamos el eje horizontal y.
         ypos = YP;
      }
   }
   return;
}
/*
   Función:    verifNumber
   Argumentos: GAME *, estructura a evaluar.
               int value, valor del bolo actual.
               int board, tablero actual a verificar.
   Objetivo:   Llenar los tableros con valores aleatorios sin repetición.
   Retorno:    int True or False.
*/
int verifNumber(GAME *players, int value, int board)
{
   for (int pos = 0; pos < MAXCOL; pos++)
   {
      if (players->boards[board].B[pos].value == value)
         return 0;
      if (players->boards[board].I[pos].value == value)
         return 0;
      if (players->boards[board].N[pos].value == value)
         return 0;
      if (players->boards[board].G[pos].value == value)
         return 0;
      if (players->boards[board].O[pos].value == value)
         return 0;
   }
   return 1;
}
/*
   Función:    verifPlay
   Argumentos: GAME *, estructura a evaluar.
               int bolo, bolo(numero) a verificar en los arreglos.
   Objetivo:   Verificar en los arreglos del jugador mandado.
   Retorno:    Ninguno.
*/
void verifPlay(GAME *players, int bolo)
{
   for (int ind = 0; ind < players->quanBoards; ind++)
      for (int arg = 0; arg < MAXCOL; arg++)
      {
         if ((players)->boards[ind].B[arg].value == bolo)
            (players)->boards[ind].B[arg].status = USED;
         if ((players)->boards[ind].I[arg].value == bolo)
            (players)->boards[ind].I[arg].status = USED;
         if ((players)->boards[ind].N[arg].value == bolo)
            (players)->boards[ind].N[arg].status = USED;
         if ((players)->boards[ind].G[arg].value == bolo)
            (players)->boards[ind].G[arg].status = USED;
         if ((players)->boards[ind].O[arg].value == bolo)
            (players)->boards[ind].O[arg].status = USED;
      }
   return;
}
/*
   Función:    verifWin
   Argumentos: GAME *, estructura a evaluar.
               int numbPlayer, jugador especifico a evaluar.
   Objetivo:   Verificar si el jugador ganó.
   Retorno:    int, True or False.
*/
int verifWin(GAME *players, int numbPlayer)
{

   if (verifWinHor(players, numbPlayer))
      return 1;
   if (verifWinVer(players, numbPlayer))
      return 1;
   if (verifWinDiag(players, numbPlayer))
      return 1;

   return 0;
}
/*
   Función:    verifWin
   Argumentos: GAME *, estructura a evaluar.
               int numbPlayer, jugador especifico a evaluar.
   Objetivo:   Verificar si el jugador ganó en horizontal.
   Retorno:    int, True or False.
*/
/*
   Función:    verifWin
   Argumentos: GAME *, estructura a evaluar.
               int numbPlayer, jugador especifico a evaluar.
   Objetivo:   Verificar si el jugador ganó en vertical.
   Retorno:    int, True or False.
*/
int verifWinVer(GAME *players, int numbPlayer)
{
   int win = 0;

   for (int ind = 0; ind < players->quanBoards; ind++)
   {
      for (int arg = 0; arg < MAXCOL; arg++)
      {
         if (players->boards[ind].B[arg].status == USED)
            win++;
         else
            win = 0;
         if (win == WIN)
         {
            winner(players, numbPlayer, ind);
            return 1;
         }
      }
      win = 0;
      for (int arg = 0; arg < MAXCOL; arg++)
      {
         if (players->boards[ind].I[arg].status == USED)
            win++;
         else
            win = 0;
         if (win == WIN)
         {
            winner(players, numbPlayer, ind);
            return 1;
         }
      }
      win = 0;
      for (int arg = 0; arg < MAXCOL; arg++)
      {
         if (players->boards[ind].N[arg].status == USED)
            win++;
         else
            win = 0;
         if (win == WIN)
         {
            winner(players, numbPlayer, ind);
            return 1;
         }
      }
      win = 0;
      for (int arg = 0; arg < MAXCOL; arg++)
      {
         if (players->boards[ind].G[arg].status == USED)
            win++;
         else
            win = 0;
         if (win == WIN)
         {
            winner(players, numbPlayer, ind);
            return 1;
         }
      }
      win = 0;
      for (int arg = 0; arg < MAXCOL; arg++)
      {
         if (players->boards[ind].O[arg].status == USED)
            win++;
         else
            win = 0;
         if (win == WIN)
         {
            winner(players, numbPlayer, ind);
            return 1;
         }
      }
      win = 0;
   }
   return 0;
}
int verifWinHor(GAME *players, int numbPlayer)
{
   int win = 0;

   for (int ind = 0; ind < players->quanBoards; ind++)
   {
      for (int arg = 0; arg < MAXCOL; arg++)
      {
         if (players->boards[ind].B[arg].status == USED)
         {
            win++;
            if (players->boards[ind].I[arg].status == USED)
            {
               win++;
               if (players->boards[ind].N[arg].status == USED)
               {
                  win++;
                  if (players->boards[ind].G[arg].status == USED)
                  {
                     win++;
                     if (players->boards[ind].O[arg].status == USED)
                     {
                        win++;
                        if (win == WIN)
                        {
                           winner(players, numbPlayer, ind);
                           return 1;
                        }
                     }
                     else
                        win = 0;
                  }
                  else
                     win = 0;
               }
               else
                  win = 0;
            }
            else
               win = 0;
         }
         else
            win = 0;
      }
      win = 0;
   }

   return 0;
}
/*
   Función:    verifWin
   Argumentos: GAME *, estructura a evaluar.
               int numbPlayer, jugador especifico a evaluar.
   Objetivo:   Verificar si el jugador ganó en diagonal.
   Retorno:    int, True or False.
*/
int verifWinDiag(GAME *players, int numbPlayer)
{
   int win = 0;
   // Principal
   for (int ind = 0; ind < players->quanBoards; ind++)
   {
      for (int arg = 0; arg < MAXCOL; arg++)
      {
         if (players->boards[ind].B[0].status == USED)
         {
            win++;
            if (players->boards[ind].I[1].status == USED)
            {
               win++;
               if (players->boards[ind].N[2].status == USED)
               {
                  win++;
                  if (players->boards[ind].G[3].status == USED)
                  {
                     win++;
                     if (players->boards[ind].O[4].status == USED)
                     {
                        win++;
                        if (win == WIN)
                        {
                           winner(players, numbPlayer, ind);
                           return 1;
                        }
                     }
                     else
                        win = 0;
                  }
                  else
                     win = 0;
               }
               else
                  win = 0;
            }
            else
               win = 0;
         }
         else
            win = 0;
      }
      win = 0;
   }
   win = 0;
   // Secundaria.
   for (int ind = 0; ind < players->quanBoards; ind++)
   {
      for (int arg = 0; arg < MAXCOL; arg++)
      {
         if (players->boards[ind].B[4].status == USED)
         {
            win++;
            if (players->boards[ind].I[3].status == USED)
            {
               win++;
               if (players->boards[ind].N[2].status == USED)
               {
                  win++;
                  if (players->boards[ind].G[1].status == USED)
                  {
                     win++;
                     if (players->boards[ind].O[0].status == USED)
                     {
                        win++;
                        if (win == WIN)
                        {
                           winner(players, numbPlayer, ind);
                           return 1;
                        }
                     }
                     else
                        win = 0;
                  }
                  else
                     win = 0;
               }
               else
                  win = 0;
            }
            else
               win = 0;
         }
         else
            win = 0;
      }
      win = 0;
   }
   return 0;
}
/*
   Función:    verifWin
   Argumentos: GAME *, estructura a evaluar.
               int numbPlayer, jugador especifico a evaluar.
   Objetivo:   Verificar si el jugador ganó en a carton completo.
   Retorno:    int, True or False.
*/
int verifFullWin(GAME *players, int numbPlayer)
{
   int win = 0;

   for (int ind = 0; ind < players->quanBoards; ind++)
   {
      for (int arg = 0; arg < MAXCOL; arg++)
      {
         if (players->boards[ind].B[arg].status == USED)
         {
            win++;
            if (players->boards[ind].I[arg].status == USED)
            {
               win++;
               if (players->boards[ind].N[arg].status == USED)
               {
                  win++;
                  if (players->boards[ind].G[arg].status == USED)
                  {
                     win++;
                     if (players->boards[ind].O[arg].status == USED)
                     {
                        win++;
                        if (win == WIN * WIN)
                        {
                           winner(players, numbPlayer, ind);
                           return 1;
                        }
                     }
                     else
                        win = 0;
                  }
                  else
                     win = 0;
               }
               else
                  win = 0;
            }
            else
               win = 0;
         }
         else
            win = 0;
      }
      win = 0;
   }

   return 0;
}
/*
   Función:    winner
   Argumentos: GAME *, estructura a evaluar.
               int numbPlayer, jugador especifico a evaluar.
               int board, carton  especifico del jugador.
   Objetivo:   Imprimir en pantalla el jugador que ganó.
   Retorno:    Ninguno.
*/
void winner(GAME *players, int numbPlayer, int board)
{
   setColor(RED, WHITE);
   gotoxy(XPOS, YPOS - 1);
   printf("Ha ganado Jugador #%d en carton #%d (%d-%d).\n", numbPlayer + 1, board + 1, numbPlayer + 1, board + 1);
   colorDefault();
   gotoxy(XPOS, YPOS);
   system("pause");
   gotoxy(XPOS, YPOS);
   printf("                                              ");
   gotoxy(XPOS, XPOS);
   printf("                                              ");
   return;
}
/*
   Función:    showWinners
   Argumentos: int winners[], Arreglos que guarda cuantas veces ha ganado un jugador.
               int quanPlayers, cantidad de jugadores.
   Objetivo:   Imprimir en pantalla las estadísticas de los jugadores.
   Retorno:    Ninguno.
*/
void showWinners(int winners[], int quanPlayers)
{
   for (int pos = 0; pos < quanPlayers; pos++)
      printf("El jugador %d, lleva %d victoria(s)\n", pos + 1, winners[pos]);
   return;
}
/*
   Función:    randRange
   Argumentos: int minLimit, primer valor (rango inferior).
               int maxLimit, segundo valor (rango superior).
   Objetivo:   Crear un número aleatorio con la formula.
   Retorno:    int, un número aleatorio
*/
int randRange(int minLimit, int maxLimit)
{
   return rand() % (maxLimit - minLimit + 1) + minLimit;
}
/*
   Función:    showBolo
   Argumentos: int bolo, el numero a imprimir.
   Objetivo:   Presentar los bolos con su letra significativa.
   Retorno:    Ninguno.
*/
void showBolo(int bolo)
{
   setColor(WHITE, RED);
   gotoxy(XPOS, YPOS - 2);
   if (bolo > 0 && bolo <= 15)
      printf(" BOLO: B-%d ", bolo);
   else if (bolo > 15 && bolo <= 30)
      printf(" BOLO: I-%d ", bolo);
   else if (bolo > 30 && bolo <= 45)
      printf(" BOLO: N-%d ", bolo);
   else if (bolo > 45 && bolo <= 60)
      printf(" BOLO: G-%d ", bolo);
   else
      printf(" BOLO: O-%d ", bolo);
   return;
}
/*
   Función:    getBolo
   Argumentos: int usadas[], bolos ya usados.
   Objetivo:   Lanzar un bolo sin repetición.
   Retorno:    int, 'pos' que es el bolo(numero) en cuestión.
*/
int getBolo(int usadas[])
{
   int pos;
   do
   {
      pos = randRange(1, MAXBOLO);
      if (usadas[pos] == FREE)
         usadas[pos] = USED;
      else
         pos = 0;
   } while (!pos);

   return pos;
}
/*
   Función:    setColor
   Argumentos: int TC, color del texto.
               int BC, colo del fondo
   Objetivo:   Establecer los colores elegidos en 'TC' y 'BC'.
   Retorno:    Ninguno.
*/
void setColor(int text, int background)
{
   textcolor(text);
   textbackground(background);
   return;
}
/*
   Función:    defaultColor
   Argumentos: Ninguno.
   Objetivo:   Establecer los colores de fabrica.
   Retorno:    Ninguno.
*/
void colorDefault()
{
   textcolor(LIGHTGRAY);
   textbackground(BLACK);
   return;
}
/*
   Función:    showTimeXY
   Argumentos: int seg, segundos a evaluar.
               int posX, la posición horizontal del plano.
               int posY, la posición vertical del plano.
   Objetivo:   Explicarle al jugador como jugar.
   Retorno:    Ninguno.
*/
void showTimeXY(int seg, int posX, int posY)
{
   int min, hour;

   min = (seg / 60);
   seg -= (min * 60);
   hour = (min / 60);
   seg -= (hour * 3600);
   gotoxy(posX, posY);
   setColor(RED, WHITE);
   printf("%02d:%02d:%02d", hour, min, seg);
   colorDefault();
   return;
}