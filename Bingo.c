// Librerías
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <conio.c>
#include <time.h>
// Mapeo de Teclas
#define ESC 27
#define ENTER 13
// Parámetros del juego
#define INITIAL 0
#define DIF 15
#define DIST 3
#define YP 6
#define XP 20
#define FREE 0
#define USED 1
#define WIN 5
#define MAXCOL 5
#define MAXBOLO 76
#define MAXBOARDS 4
#define XPOS 35
#define YPOS 4
// Boolean
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
// Módulos int
int verifNumber(GAME *, int, int);
int randRange(int, int);
int getBolo(int[]);
int verifWin(GAME *, int);
int verifWinHor(GAME *, int);
int verifWinVer(GAME *, int);
int verifWinDiag(GAME *, int);
int verifFullWin(GAME *, int);
// Módulos void
void verifPlay(GAME *, int);
void showBoards(GAME *, int, int[]);
void fillBoards(GAME *, int);
void setColor(int, int);
void showBolo(int);
void showTimeXY(int, int, int);
void colorDefault(void);
void winner(GAME *, int, int);
void showWinners(int[], int);

int main()
{
   GAME *players;
   int quanPlayers, key, seg, bolo, bingo = 0, bingoFull = 0, bolos[MAXBOLO];
   bolo = getBolo(bolos);

   srand(time(NULL));
   _setcursortype(0);

   setColor(RED, WHITE);
   gotoxy(30, 5);
   printf("%cBIENVENIDO AL BINGO%c\n", 173, 33);
   colorDefault();

   printf("Cantidad de jugadores: ");
   scanf("%d", &quanPlayers);
   players = (GAME *)malloc(quanPlayers * sizeof(GAME));
   int winners[quanPlayers], activePlayers[quanPlayers];

   for (int pos = 0; pos < quanPlayers; pos++)
      winners[pos] = FALSE;

   do
   {
      bingo = 0, bingoFull = 0;
      for (int pos = 0; pos < quanPlayers; pos++)
         activePlayers[pos] = TRUE;
      for (int pos = 0; pos < MAXBOLO; pos++)
         bolos[pos] = FREE;

      for (int pos = 0; pos < quanPlayers; pos++)
      {
         do
         {
            printf("Cantidad de cartones jugador #%d: ", pos + 1);
            scanf("%d", &(players + pos)->quanBoards);
            if ((players + pos)->quanBoards > MAXBOARDS)
               printf("El limite de cartones es 4\n");
         } while ((players + pos)->quanBoards > MAXBOARDS);
         (players + pos)->boards = (BOARD *)malloc((players + pos)->quanBoards * sizeof(BOARD));
      }
      printf("\nControles:\n");
      printf("\n[ENTER] para sacar un bolo.\n");
      printf("[ESC]   para ver las victorias de los jugadores.\n\n");
      system("pause");
      system("cls");
      fillBoards(players, quanPlayers);
      seg = time(NULL);
      do
      {
         do
         {
            showTimeXY(time(NULL) - seg, XPOS, YPOS - 3);
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

               showBoards(players, quanPlayers, activePlayers);
               for (int pos = 0; pos < quanPlayers; pos++)
               {
                  if (!bingo)
                  {
                     if (verifWin((players + pos), pos))
                     {
                        winners[pos]++;
                        bingo = TRUE;
                        gotoxy(XPOS, YPOS);
                        printf("%cDesean jugar a carton completo%c (s)%c %c (n)o", 168, 63, 161, 162);
                        do
                        {
                           fflush(stdin);
                           key = tolower(getch());
                        } while (key != 's' && key != 'n');
                        if (key == 'n')
                        {
                           gotoxy(XPOS, YPOS);
                           printf("                                                                    ");
                           bingoFull = TRUE;
                        }
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
                              if (key == 's')
                                 activePlayers[pos] = 1;
                              else if (key == 'n')
                                 activePlayers[pos] = 0;
                           }
                        }
                        system("cls");
                     }
                  }
                  else if (bingo)
                  {
                     if (verifFullWin((players + pos), pos) && activePlayers[pos] == 1)
                     {
                        bingoFull = TRUE;
                        winners[pos]++;
                     }
                  }
               }
            }
         } while (!bingo);
      } while (!bingoFull);
      gotoxy(XPOS, YPOS);
      printf("%cDesea jugar otra vez%c (s)%c %c (n)o", 168, 63, 161, 162);
      do
      {
         fflush(stdin);
         key = tolower(getch());
      } while (key != 's' && key != 'n');
      if (key == 's')
         for (int pos = 0; pos < quanPlayers; pos++)
            free((players + pos)->boards);
      system("cls");
   } while (key != 'n');
   gotoxy(30, 10);
   setColor(YELLOW, RED);
   printf("%cGracias por jugar al bingo, vuelva pronto%c", 173, 33);
   sleep(2);
   free(players);
   return 0;
}
/*
   Función:    fillBoards
   Argumentos: GAME *players, estructura a evaluar.
               int quanPlayers, cantidad de jugadores.

   Objetivo:   Llenar los tableros con valores aleatorios.
   Retorno:    Ninguno.
*/
void fillBoards(GAME *players, int quanPlayers)
{
   int value;
   for (int pos = 0; pos < quanPlayers; pos++)
      for (int ind = 0; ind < (players + pos)->quanBoards; ind++)
         for (int arg = 0; arg < MAXCOL; arg++)
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

            (players + pos)->boards[ind].N[2].value = INITIAL;
            (players + pos)->boards[ind].N[2].status = USED;
         }
   return;
}
/*
   Función:    showBoards
   Argumentos: GAME *players, estructura a evaluar.
               int quanPlayers, cantidad de jugadores.
               int full[], verificador para imprimir jugadores.

   Objetivo:   Mostrar los tableros de todos los jugadores activos.
   Retorno:    Ninguno.
*/
void showBoards(GAME *players, int quanPlayers, int activePlayers[])
{
   int xpos, ypos = YP, def;
   for (int pos = 0; pos < quanPlayers; pos++)
   {
      if (activePlayers[pos])
      {
         for (int ind = 0; ind < (players + pos)->quanBoards; ind++)
         {
            xpos = XP * (pos + 1);
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

               xpos += DIST;
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
               xpos = def;
               ypos++;
            }
            colorDefault();
         }
         ypos = YP;
      }
   }
   return;
}
/*
   Función:    verifNumber
   Argumentos: GAME *players, estructura a evaluar.
               int value, valor del bolo actual.
               int board, tablero actual a verificar.

   Objetivo:   Verificar que los valores del tablero no se están repitiendo.
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
   Argumentos: GAME *players, estructura a evaluar.
               int bolo, bolo(numero) a verificar en los arreglos.

   Objetivo:   Verificar si el bolo que salio esta en algún tablero y cambiar el estatus.
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
   Argumentos: GAME *players, estructura a evaluar.
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
   Función:    verifWinVer
   Argumentos: GAME *players, estructura a evaluar.
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
/*
   Función:    verifWinHor
   Argumentos: GAME *players, estructura a evaluar.
               int numbPlayer, jugador especifico a evaluar.

   Objetivo:   Verificar si el jugador ganó en horizontal.
   Retorno:    int, True or False.
*/
int verifWinHor(GAME *players, int numbPlayer)
{
   for (int ind = 0; ind < players->quanBoards; ind++)
      for (int arg = 0; arg < MAXCOL; arg++)
         if (players->boards[ind].B[arg].status == USED &&
             (players->boards[ind].I[arg].status == USED && players->boards[ind].N[arg].status == USED && players->boards[ind].G[arg].status == USED && players->boards[ind].O[arg].status == USED))
         {
            winner(players, numbPlayer, ind);
            return 1;
         }
   return 0;
}
/*
   Función:    verifWinDiag
   Argumentos: GAME *players, estructura a evaluar.
               int numbPlayer, jugador especifico a evaluar.

   Objetivo:   Verificar si el jugador ganó en diagonal.
   Retorno:    int, True or False.
*/
int verifWinDiag(GAME *players, int numbPlayer)
{
   // Principal
   for (int ind = 0; ind < players->quanBoards; ind++)
      if (players->boards[ind].B[0].status == USED)
         if (players->boards[ind].I[1].status == USED)
            if (players->boards[ind].N[2].status == USED)
               if (players->boards[ind].G[3].status == USED)
                  if (players->boards[ind].O[4].status == USED)
                  {
                     winner(players, numbPlayer, ind);
                     return 1;
                  }
   // Secundaria.
   for (int ind = 0; ind < players->quanBoards; ind++)
      if (players->boards[ind].B[4].status == USED)
         if (players->boards[ind].I[3].status == USED)
            if (players->boards[ind].N[2].status == USED)
               if (players->boards[ind].G[1].status == USED)
                  if (players->boards[ind].O[0].status == USED)
                  {
                     winner(players, numbPlayer, ind);
                     return 1;
                  }
   return 0;
}
/*
   Función:    verifFullWin
   Argumentos: GAME *players, estructura a evaluar.
               int numbPlayer, jugador especifico a evaluar.

   Objetivo:   Verificar si el jugador ganó en a carton completo.
   Retorno:    int, True or False.
*/
int verifFullWin(GAME *players, int numbPlayer)
{
   int arg = 0;

   for (int ind = 0; ind < players->quanBoards; ind++)
      if (players->boards[ind].B[arg].status == USED && players->boards[ind].B[arg + 1].status == USED && players->boards[ind].B[arg + 2].status == USED &&
          players->boards[ind].B[arg + 3].status == USED && players->boards[ind].B[arg + 4].status == USED)

         if (players->boards[ind].I[arg].status == USED && players->boards[ind].I[arg + 1].status == USED && players->boards[ind].I[arg + 2].status == USED &&
             players->boards[ind].I[arg + 3].status == USED && players->boards[ind].I[arg + 4].status == USED)

            if (players->boards[ind].N[arg].status == USED && players->boards[ind].N[arg + 1].status == USED && players->boards[ind].N[arg + 2].status == USED &&
                players->boards[ind].N[arg + 3].status == USED && players->boards[ind].N[arg + 4].status == USED)

               if (players->boards[ind].G[arg].status == USED && players->boards[ind].G[arg + 1].status == USED && players->boards[ind].G[arg + 2].status == USED &&
                   players->boards[ind].G[arg + 3].status == USED && players->boards[ind].G[arg + 4].status == USED)

                  if (players->boards[ind].O[arg].status == USED && players->boards[ind].O[arg + 1].status == USED && players->boards[ind].O[arg + 2].status == USED &&
                      players->boards[ind].O[arg + 3].status == USED && players->boards[ind].O[arg + 4].status == USED)
                  {
                     winner(players, numbPlayer, ind);
                     return 1;
                  }
   return 0;
}
/*
   Función:    winner
   Argumentos: GAME *players, estructura a evaluar.
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
      printf("El jugador #%d, lleva #%d victoria(s)\n", pos + 1, winners[pos]);
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
   Argumentos: int text, color del texto.
               int background, color del fondo
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
   Función:    colorDefault
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
