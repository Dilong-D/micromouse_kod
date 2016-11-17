
#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stddef.h>
#include "hd44780.h"
#include "libs.h"

/**
 * Kierunki w jakich moze byc zwrocona mysz, a takze strony labiryntu
 */
#define RIGHT  0x1
#define LEFT  0x2
#define DOWN  0x4
#define UP  0x8

#define LAB_SIZE_X  16
#define LAB_SIZE_Y  16

/**
 * Wspolrzedne srodka labiryntu
 */
#define BEG_X 0
#define BEG_Y 0
#define BEG_DIR RIGHT

/**
 * Wspolrzedne celu do ktorego zmierza robot (lewy dolny i prawy gorny rog
 */
#define TAR_X1 7
#define TAR_Y1 7
#define TAR_X2 8
#define TAR_Y2 8

/**
 * Do algorytmu Tremaux - zaznaczone, ze robot przeszedl sciezka znajdujacej
 * sie po danej stronie pola 2 razy
 */
#define SEC_RIGHT 0x10
#define SEC_LEFT 0x20
#define SEC_DOWN  0x40
#define SEC_UP  0x80




void initLab();
void step_forwards();
void turn(uint8_t);
uint8_t wall_on_left();
uint8_t wall_in_front();
void hand_on_wall_step();
uint8_t is_junction(uint8_t, uint8_t);
uint8_t first_dir(uint8_t);
void tremaux_step();
int flood_fill_algorithm(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
uint8_t flood_fill_loc_step(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
void algStep();
uint8_t getNextAction();



/**
 * S³u¿y do sprawdzania czy faktyczne polozenie robota jest zgodne z tym, 
 * ktore bylo wyznaczone z algorytmu.
 */
uint8_t labposx_real;
uint8_t labposy_real;
uint8_t mouse_dir_real;

uint8_t labposx;
uint8_t labposy;
uint8_t mouse_dir;
/**
 * Ustawiane w momencie odkrycia nowej sciany
 */
uint8_t new_wall_discovered;

/*
 * Tablica reprezentujaca labirynt, 1, jesli przejscie w danym kierunku jest mozliwe
 */
uint8_t labyrinth[LAB_SIZE_X][LAB_SIZE_Y];

/**
 * Kazde pole tablicy reprezentuje jedno pole labiryntu - jego wartosc wynosi 1,
 * jezeli pole zostalo odkryte (znane sa sciany)
 */
uint8_t discovered[LAB_SIZE_X][LAB_SIZE_Y];


/**
 * aktualnie wyznaczona najkrotsza sciezka (moze przechodzic przez sciany - z
 * biegiem czasu weryfikowana)
 */
uint8_t path[LAB_SIZE_X * LAB_SIZE_Y][2];

/**
 * Tablica potrzebna do algorytmu Floodfill - odleglosci od miejsca poczatkowego
 */
uint8_t distances[LAB_SIZE_X][LAB_SIZE_Y];

#endif