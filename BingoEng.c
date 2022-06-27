// Includes
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <conio.c>
#include <time.h>
// Key Bindings
#define ESC 27
#define ENTER 13
// Game parameters
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
// int modules
int verifNumber(GAME *, int, int);
int randRange(int, int);
int getBolo(int[]);
int verifWin(GAME *, int);
int verifWinHor(GAME *, int);
int verifWinVer(GAME *, int);
int verifWinDiag(GAME *, int);
int verifFullWin(GAME *, int);
// Void modules
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
   printf("%cWELCOME TO THE BINGO%c\n", 173, 33);
   colorDefault();

   printf("Amount of players: ");
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
            printf("Amount of boards of player #%d: ", pos + 1);
            scanf("%d", &(players + pos)->quanBoards);
            if ((players + pos)->quanBoards > MAXBOARDS)
               printf("The max amount of boards is %d\n", MAXBOARDS);
         } while ((players + pos)->quanBoards > MAXBOARDS);
         (players + pos)->boards = (BOARD *)malloc((players + pos)->quanBoards * sizeof(BOARD));
      }
      printf("\nControls:\n");
      printf("\n[ENTER] to throw.\n");
      printf("[ESC]   to see the statistics.\n\n");
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
                        printf("Would you like to play full board mode%c (yes) or (n)o", 63);
                        do
                        {
                           fflush(stdin);
                           key = tolower(getch());
                        } while (key != 'y' && key != 'n');
                        if (key == 'n')
                        {
                           gotoxy(XPOS, YPOS);
                           printf("                                                                    ");
                           bingoFull = TRUE;
                        }
                        if (key == 'y')
                        {
                           gotoxy(XPOS, YPOS);
                           printf("                                                                    ");
                           for (int pos = 0; pos < quanPlayers; pos++)
                           {
                              gotoxy(XPOS, YPOS);
                              printf("Jugador #%d%c (y)es or (n)o", pos + 1, 63);
                              do
                              {
                                 fflush(stdin);
                                 key = tolower(getch());
                              } while (key != 'y' && key != 'n');
                              if (key == 'y')
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
                     if (verifFullWin((players + pos), pos))
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
      printf("Would you like to play again%c (y)es or (n)o", 63);
      do
      {
         fflush(stdin);
         key = tolower(getch());
      } while (key != 'y' && key != 'n');
      if (key == 'y')
         for (int pos = 0; pos < quanPlayers; pos++)
            free((players + pos)->boards);
      system("cls");
   } while (key != 'n');
   gotoxy(30, 10);
   setColor(YELLOW, RED);
   printf("%cThanks for playing the game%c", 173, 33);
   sleep(2);
   free(players);
   return 0;
}
/*
   Function:   fillBoards
   Arguments:  GAME *players, struct to be evaluated.
               int quanPlayers, quantity of players.

   Objective:  to fill the boards with random numbers (between 1-75)
   Return:     None.
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
   Function:   showBoards
   Arguments:  GAME *players, struct to be evaluated.
               int quanPlayers, quantity of players.
               int full[], players to be printed.

   Objective:  to show all the active player's boards.
   Return:     None.
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
   Function:   verifNumber
   Arguments:  GAME *players, struct to be evaluated.
               int value, current value of the bolo.
               int board, current board to be emulated.

   Objective:  to check that the value is not already used or contain in the board.
   Return:     int True or False.
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
   Function:   verifPlay
   Arguments:  GAME *players, struct to be evaluated.
               int bolo, bolo(numero) a verificar en los arreglos.

   Objective:  to check if the current bolo exist on any boar, and change the status.
   Return:     None.
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
   Function:   verifWin
   Arguments:  GAME *players, struct to be evaluated.
               int numbPlayer, specific player to be evaluated.

   Objective:  to check if the player won.
   Return:     int, True or False.
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
   Function:   verifWinVer
   Arguments:  GAME *players, struct to be evaluated.
               int numbPlayer, specific player to be evaluated.

   Objective:  to check if the player won in vertical.
   Return:    int, True or False.
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
   Function:   verifWinHor
   Arguments:  GAME *players, struct to be evaluated.
               int numbPlayer, specific player to be evaluated.

   Objective:  to check if the player won in horizontal
   Return:     int, True or False.
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
   Function:   verifWinDiag
   Arguments:  GAME *players, struct to be evaluated.
               int numbPlayer, specific player to be evaluated.

   Objective:  to check if the player won in diagonal
   Return:     int, True or False.
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
   Function:   verifFullWin
   Arguments:  GAME *players, struct to be evaluated.
               int numbPlayer, specific player to be evaluated.

   Objective:  to check if the player won in full board mode.
   Return:     int, True or False.
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
   Function:   winner
   Arguments:  GAME *players, struct to be evaluated.
               int numbPlayer, specific player to be evaluated.
               int board, current board to be emulated.

   Objective:  to print the player who won the game.
   Return:     None.
*/
void winner(GAME *players, int numbPlayer, int board)
{
   setColor(RED, WHITE);
   gotoxy(XPOS, YPOS - 1);
   printf("The player #%d won in board #%d (%d-%d).\n", numbPlayer + 1, board + 1, numbPlayer + 1, board + 1);
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
   Function:   showWinners
   Arguments:  int winners[], Arreglos que guarda cuantas veces ha ganado un jugador.
               int quanPlayers, quantity of players.

   Objective:  Imprimir en pantalla las estadísticas de los jugadores.
   Return:     None.
*/
void showWinners(int winners[], int quanPlayers)
{
   for (int pos = 0; pos < quanPlayers; pos++)
      printf("The player #%d, has #%d victories\n", pos + 1, winners[pos]);
   return;
}
/*
   Function:   randRange
   Arguments:  int minLimit, inferior limit.
               int maxLimit, superior limit.

   Objective:  to get a random number.
   Return:     int, a random number.
*/
int randRange(int minLimit, int maxLimit)
{
   return rand() % (maxLimit - minLimit + 1) + minLimit;
}
/*
   Function:   showBolo
   Arguments:  int bolo, the number to be printed.
   Objective:  to show the number with it's representative letter.
   Return:     None.
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
   Function:   getBolo
   Arguments:  int used[], string with used numbers (bolos).
   Objective:  to throw a random number with no repetition.
   Return:     int pos, the number of the bolo.
*/
int getBolo(int used[])
{
   int pos;
   do
   {
      pos = randRange(1, MAXBOLO);
      if (used[pos] == FREE)
         used[pos] = USED;
      else
         pos = 0;
   } while (!pos);

   return pos;
}
/*
   Function:   setColor
   Arguments:  int TC, text's color.
               int BC, background's color.

   Objective:  to set text color 'TC' and background 'BC'.
   Return:     None.
*/
void setColor(int text, int background)
{
   textcolor(text);
   textbackground(background);
   return;
}
/*
   Function:   defaultColor
   Arguments:  None.
   Objective:  to set default colors.
   Return:     None.
*/
void colorDefault()
{
   textcolor(LIGHTGRAY);
   textbackground(BLACK);
   return;
}
/*
   Function:   showTimeXY
   Arguments:  int seg, seconds to be evaluated.
               int posX, horizontal plane's position.
               int posY, vertical plane's position.

   Objective:  to get and show the current time lapsed.
   Return:     None.
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
