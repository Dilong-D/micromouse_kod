#include "algorithms.h"


/**
 * Pozycja w labiryncie wyrażona podczas poprzedniego ruchu
 */
uint8_t prev_labposx = BEG_X;
uint8_t prev_labposy = BEG_Y;

/**
 * Służy do sprawdzania czy faktyczne polozenie robota jest zgodne z tym, 
 * ktore bylo wyznaczone z algorytmu.
 */
uint8_t labposx_real = BEG_X;
uint8_t labposy_real = BEG_Y;
uint8_t mouse_dir_real = UP;


/**
 * Kierunek, w ktorym zwrocona jest mysz
 */
uint8_t mouse_dir = UP;

/**
 * Zatrzymaj przeszukiwanie labiryntu jesli 1 (raczej ta sytuacja nie powinna nastapic)
 */
uint8_t stop = 0; 
/**
 * Tablica potrzebna do algorytmu Tremaux w celu zaznaczenia ile razy robot 
 * przeszedl dana sciezka
 */
uint8_t tr_paths[LAB_SIZE_X][LAB_SIZE_Y];



/**
 * Dlugosc tablicy path
 */
uint8_t pathlength;

/**
 * Numer kroku na ścieżce (path)
 */
uint8_t step_in_path = 0;

/**
 * Czy wyznaczona sciezka (path) przechodzi przez jakas sciane, ktora nie jest
 * jeszcze znana?
 */
uint8_t unknown_wall = 1;


/**
 * Ponizej: etapy eksploracji algorytmu, w ktorych moze znajdowac sie micromouse
 */

/**
 * Na poczatku: szacowanie najkrotszej sciezki do srodka labiryntu na podstawie
 * aktualnej wiedzy - etap konczy sie, gdy robot dojedzie do srodka. Ma na celu
 * dotarcie do celu, dlatego nazwa find_target
 */
uint8_t find_target = 1;

/**
 * Etap mapowania za pomoca algorytmu tremaux. Konczy sie, gdy oszacowanie 
 * najkrotszej drogi do srodka bedzie przechodzilo tylko przez znane sciany
 * 
 */
uint8_t mapping = 0;

/**
 * W momencie gdy zakonczony etap mapping etap polegajacy na powrocie do
 * poczatku labiryntu w celu rozpoczecia speed_run.
 */
uint8_t ret_beg = 0;

/**
 * Etap koncowy polegajacy na jezdzeniu tam i z powrotem od poczatku do
 * celu najkrotsza mozliwa droga. f pod koniec oznacza droga z poczatku do
 * srodka, r z celu do srodka labiryntu
 */
uint8_t speed_runf = 0;
uint8_t speed_runr = 0;
/**
 * Wracanie z powrotem do miejsca, gdzie robot powinien sie znajdowac wedlug
 * algorytmu - stosowane w momencie, gdy labposx_real ! = labposx lub
 * labposy_real ! = labposy
 */
uint8_t get_back_on_track = 0;

/**
 * Ustawiane w momencie odkrycia nowej sciany
 */
uint8_t new_wall_discovered = 1;
/**
 * Ustawia odpowiednie wartosci
 */

//liczba krokow
uint8_t numsteps = 0;
void initLab(void){
    
    find_target = 1;
    mapping = 0;
    ret_beg = 0;
    speed_runf = 0;
    speed_runr = 0;
    get_back_on_track = 0;
    numsteps = 0;
	labposx = BEG_X;
	labposy = BEG_Y;
	mouse_dir = BEG_DIR;
    
    uint8_t i = 0, j = 0;
    for(i = 0; i < LAB_SIZE_X; i++){
	    
        for(j = 0; j < LAB_SIZE_Y; j++){
            //Na razie nie ma zadnych scian (wirtualnie)
            
			
            labyrinth[i][j] = UP | DOWN | LEFT |RIGHT;
			
            discovered[i][j] = 0;
            tr_paths[i][j] = 0;
            //Sciany na brzegach
            if(i == 0)
                labyrinth[i][j] = labyrinth[i][j] & (~LEFT);
                
            if(j == 0){
                labyrinth[i][j] = labyrinth[i][j] & (~DOWN);
				
			}
                
            if(i == LAB_SIZE_X - 1)
                labyrinth[i][j] = labyrinth[i][j] & (~RIGHT);
            if(j == LAB_SIZE_Y - 1)
                labyrinth[i][j] = labyrinth[i][j] & (~UP);
				
        }
		
	}
}
/**
 * Mysz porusza sie o krok do przodu w kierunku, w ktorym jest zwrocona
 */
void step_forwards(void){
        
        switch(mouse_dir){
            case RIGHT:
                labposx++;
                return;
            case LEFT:
                labposx--;
                return;
            case UP:
                labposy++;
                return;
            case DOWN:
                labposy--;
                return;
       } 
}

/**
 * Mysz obraca sie w prawo lub w lewo (direction = left lub right)
 */
 void turn(uint8_t direction){
    if(direction == RIGHT)
        switch(mouse_dir){
            case UP:
                mouse_dir = RIGHT;
                return;
            case DOWN:
                mouse_dir = LEFT;
                return;
            case RIGHT:
                mouse_dir = DOWN;
                return;
            case LEFT:
                mouse_dir = UP;
                return;
        }
    if(direction == LEFT) 
        switch(mouse_dir){
            case UP:
                mouse_dir = LEFT;
                return;
            case DOWN:
                mouse_dir = RIGHT;
                return;
            case RIGHT:
                mouse_dir = UP;
                return;
            case LEFT:
                mouse_dir = DOWN;
        }
}
 
 
// --------------------------------------------------------
// Funkcje do algorytmu reka na scianie (hand on the wall) - pomocniczego
 /**
  * Sprawdza, czy po lewej stronie myszy jest sciana (przydatne do algorytmow
  * Hand on the wall i Tremaux
  */ 
uint8_t wall_on_left(void){
    uint8_t res = 0;
    turn(LEFT);
    //Po obrocie w lewo - sciana przed mysza?
    if((labyrinth[labposx][labposy] & mouse_dir) == 0)
        res = 1;
    turn(RIGHT);
    return res;
}

inline uint8_t wall_in_front(void){
    return ((labyrinth[labposx][labposy] & mouse_dir) == 0);
}
/**
 * Pojedynczy ruch z algorytmu hnd on wall
 */
void hand_on_wall_step(void) {

        prev_labposx = labposx;
        prev_labposy = labposy;
        
        //Jesli nie ma sciany po lewej, skrec w lewo i idz jeden krok do przodu
        if(!wall_on_left()){
            turn(LEFT);
        }
        //Jesli sciana przed mysza, skrec w prawo
        if(wall_in_front()){
            turn(RIGHT);
            //Jesli znowu sciana, znowu skrec w prawo (czyli zawroc).
            if(wall_in_front())
                turn(RIGHT);
        }
        step_forwards();
}
// --------------------------------------------------------
// Funkcje do algorytmu Tremaux - mapowanie

/**
 * Zwraca informacje, czy miejsce w ktorym znajduje sie mysz jest skrzyzowaniem
 * (mozna pojsc wiecej niz jedna z drog)
 */
uint8_t is_junction(uint8_t x, uint8_t y){
    if((labyrinth[x][y] & (UP | DOWN | LEFT))== (UP | DOWN | LEFT))
        return 1;
    if((labyrinth[x][y] & (UP | DOWN | RIGHT)) == (UP | DOWN | RIGHT))
        return 1;
    if((labyrinth[x][y] & (UP | LEFT | RIGHT)) == (UP | LEFT | RIGHT))
        return 1;
    if((labyrinth[x][y] & (DOWN | LEFT | RIGHT)) == (DOWN | LEFT | RIGHT))
        return 1;
    if((labyrinth[x][y]  & (UP | DOWN | LEFT | RIGHT)) == (UP | DOWN | LEFT | RIGHT))
        return 1;
    return 0;
}
/**
 * Wybiera pierwszy z dostepnych kierunkow zapisanych w av_dirs lub 0, jesli
 * w tej zmiennej nie ma zapisanego zadnego kierunku
 */
uint8_t first_dir(uint8_t av_dirs){
    uint8_t temp = av_dirs;
    uint8_t i;
    for(i = 0; i < 4; i++){
        if((temp & 0x1) != 0)
            return (0x1 << i);
        temp >>= 1;
    }
    return 0;
}

/**
 * Pojedynczy ruch z algorytmu Tremaux
 */

void tremaux_step(void){
    prev_labposx = labposx;
    prev_labposy = labposy;
    //Jesli wszedzie wokol robot albo juz szedl dwa razy, albo sa sciany - koniec mapowania
     if(0xFF == (( tr_paths[labposx][labposy]
            |(~((labyrinth[labposx][labposy] << 4) | labyrinth[labposx][labposy])))
             & 0xFF)){
        stop = 1;
        return;
    }
    /*
    Jesli nie jest skrzyzowaniem, wykonaj jeden ruch na zasadzie reki
    na scianie. 
    */
    if((!is_junction(labposx, labposy)) || (tr_paths[labposx][labposy] == 0)){
        hand_on_wall_step();
    }else {

        
         /*
        Jesli stare skrzyzowanie, do ktorego mysz doszla nowa sciezka zawroc i 
        idz jeden krok do przodu (to znaczy wroc sie jeden krok)
        Wyjasnienie: Jesli w polu tablicy tremaux_tr_paths wylaczymy bit 
        oznaczajacy kierunek, z ktorego mysz przyszla i pole to bedzie rowne 0,
        to oznacza, ze skrzyzowanie nie bylo jeszcze odwiedzone, w przeciwnym 
        razie - bylo. Drugi warunek oznacza, ze sciezka byla tylko oznaczona wiecej
        niz raz
        */
        turn(LEFT);
        turn(LEFT);
         if(((tr_paths[labposx][labposy] & (~mouse_dir)) == 0 || ((mouse_dir << 4) & tr_paths[labposx][labposy]) != 0)){
            /*
            Jesli nowe skrzyzowanie lub  stare do ktorego doprowadzila nas
            stara sciezka losuj w ktorym kierunku isc - jesli sa sciezki, ktorymi
            mysz jeszcze nie szla wybierz jedna z nich, jesli nie - wybierz jedna
            z tych ktorymi szla tylko raz (na tej sciezce nie moze byc sciana)
            */
            uint8_t new_dir = first_dir((uint8_t) ((~mouse_dir) & (~tr_paths[labposx][labposy]) & labyrinth[labposx][labposy]));

            if(new_dir == 0){
                new_dir = first_dir((uint8_t) ((~mouse_dir) & (~(tr_paths[labposx][labposy] >> 4)) & labyrinth[labposx][labposy]));

            }
            turn(RIGHT);
            turn(RIGHT);
            if(new_dir != mouse_dir){
                turn(LEFT);
                if(new_dir != mouse_dir){
                    turn(RIGHT);
                    turn(RIGHT);
                }
            }
        }
        step_forwards();
    }
    /*
    Jesli mysz sie przemiescila - zwieksz
    oznaczenie sciezki laczacej pole na ktorym byla i na ktorym sie znajdzie
    (czyli krawedzi laczacej te pola)
    */
    if(prev_labposx < labposx){
        if((tr_paths[labposx][labposy] & LEFT) == 0){
            tr_paths[labposx][labposy] |= LEFT;
            tr_paths[prev_labposx][prev_labposy] |= RIGHT;
        }else{
            tr_paths[labposx][labposy] |= SEC_LEFT;
            tr_paths[prev_labposx][prev_labposy] |= SEC_RIGHT;
        }
    }
    if(prev_labposx > labposx){
        if((tr_paths[labposx][labposy] & RIGHT) == 0){
            tr_paths[labposx][labposy] |= RIGHT;
            tr_paths[prev_labposx][prev_labposy]|= LEFT;
        }else{
            tr_paths[labposx][labposy] |= SEC_RIGHT;
            tr_paths[prev_labposx][prev_labposy] |= SEC_LEFT;
        }
    }    
    if(prev_labposy < labposy){
        if((tr_paths[labposx][labposy] & DOWN) == 0){
            tr_paths[labposx][labposy] |= DOWN;
            tr_paths[prev_labposx][prev_labposy] |= UP;
        }else{
            tr_paths[labposx][labposy] |= SEC_DOWN;
            tr_paths[prev_labposx][prev_labposy] |= SEC_UP;
        }
    }
    if(prev_labposy > labposy){
        if((tr_paths[labposx][labposy] & UP) == 0){
            tr_paths[labposx][labposy] |= UP;
            tr_paths[prev_labposx][prev_labposy] |= DOWN;
        }else{
            tr_paths[labposx][labposy] |= SEC_UP;
            tr_paths[prev_labposx][prev_labposy] |= SEC_DOWN;
        }
    }
}

//--------------------------------------------------------------------
//Funckje do algorytmow znajdujacych najkrotsza sciezke

/**
 * Ustala tymczasowa najkrotsza
 * begx, begy - wspolrzedne poczatku sciezki
 * endx1, endy1, endx2, endy2 - wsp. ktorymi ograniczony jest cel, do ktorego
 * zmierza sciezka
 * Zwraca odleglosc od celu, -1 jesli blad
 */


int flood_fill_algorithm(uint8_t begx, uint8_t begy, uint8_t endx1, uint8_t endy1,
        uint8_t endx2, uint8_t endy2){
    //zakladam, ze LAB_SIZE_X >= LAB_SIZE_Y - JESZCZE TRZEBA SIE UPEWNIC, ZE
    //TAKIE WIELKOSCI TABLIC WYSTARCZA
    uint8_t tab1[LAB_SIZE_X * 2][2];
    uint8_t tab2[LAB_SIZE_X * 2][2];
    uint8_t (*cur)[LAB_SIZE_X * 2][2];
    uint8_t (*next)[LAB_SIZE_X * 2][2];
    uint8_t (*temp)[LAB_SIZE_X * 2][2];
    uint8_t c = 0;
    uint8_t stp = 0;
    uint8_t tarx = 255, tary = 255;
    int dist = 1;
    uint8_t x, y;
    
    unknown_wall = 0;
    
    //Wypelnij tablice z odleglociami liczbami 255 , to samo z tablicami tab1 i tabe;
    int i, j, n;
    
    for(i = 0; i < LAB_SIZE_X; i++)
        for(j = 0; j < LAB_SIZE_Y; j++)
            distances[i][j] = 255;
    
    for(i = 0; i < 2* LAB_SIZE_X ; i++){
        tab1[i][0] = 255;
        tab1[i][1] = 255;
        tab2[i][0] = 255;
        tab2[i][1] = 255;
    }    
    cur = &tab1;
    next = &tab2;
    distances[begx][begy] = 0;
	
    (*cur)[0][0] = begx;
    (*cur)[0][1] = begy;
    //Sprawdz, czy poczatek nie jest zawarty w celu
     if( (endx1 <= begx) && (endx2 >= begx) 
                        && (endy1 <= begy) && (endy2 >= begy)){
                        tarx = begx;
                        tary = begy;
                        n = 0;
                        stp = 1;
                    }
				
    while(!stp){
        c = 0;
		
		
        for(i = 0; i < (LAB_SIZE_X * 2) && (*cur)[i][0] != 255; i++){
            x = (*cur)[i][0];
            y = (*cur)[i][1];
            //Jesli od gory nie ma sciany wypelnij w tamta strone (itd)
            if(labyrinth[x][y] & UP){
                if(distances[x][y+1] == 255){
					/*LcdClear();
					LcdDec(dist);
					Lcd(" ");
					LcdDec(x);
					Lcd(" UP ");
					LcdDec(y+1);
					_delay_ms(10000);*/
					
                    //Wpisz odleglosc do tablicy
                    distances[x][y+1] = dist;
                    //Wpisz do tablicy jako pole do rozwazania w kolejnej iteracji
                    (*next)[c][0] = x;
                    (*next)[c][1] = y + 1;
                    c++;
                    //Sprawdz czy to pole znajduje sie w celu
                    if( (endx1 <= x) && (endx2 >= x) 
                        && (endy1 <= y+1) && (endy2 >= y+1)){
                        tarx = x;
                        tary = y+1;
                        stp = 1;
                        break;
                    }
                } 
            }
            
            if(labyrinth[x][y] & DOWN){
	            /*LcdClear();
	            LcdDec(dist);
	            Lcd(" ");
	            LcdDec(x);
	            Lcd(" D ");
	            LcdDec(y-1);
	            _delay_ms(10000);*/
                if(distances[x][y-1] == 255){
                    distances[x][y-1] = dist;
                    (*next)[c][0] = x;
                    (*next)[c][1] = y - 1;
                    c++;
                    if( (endx1 <= x) && (endx2 >= x) 
                        && (endy1 <= y-1) && (endy2 >= y-1)){
                        tarx = x;
                        tary = y-1;
                        stp = 1;
                        break;
                    }
                } 
            }
            
            if(labyrinth[x][y] & RIGHT){
	            /*LcdClear();
	            LcdDec(dist);
	            Lcd(" ");
	            LcdDec(x+1);
	            Lcd(" R ");
	            LcdDec(y);*/
	         //   _delay_ms(10000);
                if(distances[x + 1][y] == 255){
                    distances[x + 1][y] = dist;
                    (*next)[c][0] = x + 1;
                    (*next)[c][1] = y;
                    c++;
                    if( (endx1 <= x + 1) && (endx2 >= x + 1) 
                        && (endy1 <= y) && (endy2 >= y)){
                        tarx = x + 1;
                        tary = y;
                        stp = 1;
                        break;
                    }
                } 
            }
            
            if(labyrinth[x][y] & LEFT){/*
	            LcdClear();
	            LcdDec(dist);
	            Lcd(" ");
	            LcdDec(x-1);
	            Lcd(" L ");
	            Dec(y);
	            _delay_ms(10000);*/
                if(distances[x - 1][y] == 255){
                    distances[x - 1][y] = dist;
                    (*next)[c][0] = x - 1;
                    (*next)[c][1] = y;
                    c++;
                    if( (endx1 <= x - 1) && (endx2 >= x - 1) 
                        && (endy1 <= y) && (endy2 >= y)){
                        tarx = x - 1;
                        tary = y;
                        stp = 1;
                        break;
                    }
                } 
            }
        }
        if((c == 0) && tarx == 255){
            return 255;
        }
        dist++;
        temp = cur;
        cur = next;
        next = temp;
		
        for(i = 0; i < LAB_SIZE_X; i++){
            (*next)[i][0] = 255;
            (*next)[i][1] = 255;
        }
    }
    n = distances[tarx][tary];
    //Wypelnij path libami 17
    for(i = 0; i <LAB_SIZE_X * 2; i++){
        path[i][0] = 17;
        path[i][1] = 17;
    }
    //Koniec sciezki ustaw na cel
    path[n][0] = tarx;
    path[n][1] = tary;
    x = tarx;
    y = tary;
    
    if(!discovered[x][y])
        unknown_wall = 1;
    for(i = n-1; i >= 0; i--){
        //Jesli przejscie w dana strone jest mozliwe, sprawdzm czy moze to
        //byc poprzedni elemenr sciezki - dlugosc drogi i-1 - jesli tak, 
        //wstaw go do sciezki
        if(labyrinth[x][y] & UP){
            if(distances[x][y+1] == i){
                y = y + 1;
                
                //Jesli  przechodzi przez nieznane pole - zaznacz to
                if(!discovered[x][y])
                    unknown_wall = 1;
                path[i][0] = x;
                path[i][1] = y;
                continue;
            }
        }
        
        if(labyrinth[x][y] & DOWN){
            if(distances[x][y-1] == i){
                y = y - 1;
                if(!discovered[x][y])
                    unknown_wall = 1;
                path[i][0] = x;
                path[i][1] = y;
                continue;
            }
        }
        
        if(labyrinth[x][y] & RIGHT){
            if(distances[x + 1][y] == i){
                x = x + 1;
                if(!discovered[x][y])
                    unknown_wall = 1;
                path[i][0] = x;
                path[i][1] = y;
                continue;
            }
        }
        
        if(labyrinth[x][y] & LEFT){
            if(distances[x - 1][y] == i){
                x = x - 1;
                if(!discovered[x][y])
                    unknown_wall = 1;
                path[i][0] = x;
                path[i][1] = y;
                continue;
            }
        }
    }
    pathlength = n;
    return n;
}

/*
 * Wykonywane na poczatku, robot probuje jak najszybciej dojsc do srodka lub
 * podczas powrotu na poczatek podczas mapowania
 * @return 1 jesli jest juz tam, gdzie miala dojechac, zero w przeciwnym wypadku
 */
uint8_t flood_fill_loc_step(uint8_t begx, uint8_t begy, uint8_t endx1, uint8_t endy1,
        uint8_t endx2, uint8_t endy2){
    uint8_t x, y, n = pathlength;
    if(numsteps == 0)
        new_wall_discovered = 1; 
    //Nowa odkryta sciana lub poczatek labiryntu/eksploracji
    if(new_wall_discovered){
        n = (uint8_t) flood_fill_algorithm(begx, begy, endx1, endy1,
            endx2, endy2);
            
        step_in_path = 1;
        new_wall_discovered = 0;
    }
    //Mysz znajduje sie juz u celu
    if( (path[n][0] == labposx) && (path[n][1] == labposy)){
        return 1;
    }
    
    
    x = path[step_in_path][0];
    y = path[step_in_path][1];
    //Sprawdz w ktora strone ma sie skierowac mysz.
    if(labposx < x)
        mouse_dir = RIGHT;
    else
        mouse_dir = LEFT;
        
    //W przypadku jesli labposx = x to i tak powyzsze zostanie wymazane
    
    if(labposy < y)
        mouse_dir = UP;
    else if (labposy > y)
        mouse_dir = DOWN;
    
    labposx = x;
    labposy = y;
    
    step_in_path++;
    return 0;
}


/**
 * Ogólna funkcja do wykonywania następnego kroku algorytmu
 */
void algStep(void){
    numsteps++;
    //Tu na wypadek niezgodnosci rzeczywistej pozycji i teoretycznej pozycji
    //I etap
    if(find_target){
        //Nastepny etap, jesli dotarl do celu
        if(flood_fill_loc_step(labposx, labposy, TAR_X1, TAR_Y1, TAR_X2, TAR_Y2)){
            find_target = 0;
            mapping = 1;
        }else{
            return;
			}
    }
    if(mapping){
        tremaux_step();
        flood_fill_algorithm(BEG_X, BEG_Y, TAR_X1, TAR_Y1,
            TAR_X2, TAR_Y2);
        if(!unknown_wall){
            mapping = 0;
            ret_beg = 1;
            flood_fill_algorithm(labposx, labposy, BEG_X, BEG_Y,BEG_X, BEG_Y);
            step_in_path = 1;
        }
        return;
    }
    
    
    if(ret_beg){
        if(flood_fill_loc_step(labposx, labposy, BEG_X, BEG_Y,BEG_X, BEG_Y)){
            ret_beg = 0;
            speed_runf = 1;
            flood_fill_algorithm(BEG_X, BEG_Y, TAR_X1, TAR_Y1,
                TAR_X2, TAR_Y2);
            step_in_path = 1;
        }
        else
            return;
    }
    
    if(speed_runf){
        if(flood_fill_loc_step(labposx, labposy, TAR_X1, TAR_Y1, TAR_X2, TAR_Y2) != 0){
                speed_runf = 0;
                speed_runr = 1;
                step_in_path = 1;
            flood_fill_algorithm(labposx, labposy, BEG_X, BEG_Y, BEG_X, BEG_Y);
        }else
            return;
        
    }
    
    
    if(speed_runr){
        if(flood_fill_loc_step(labposx, labposy, BEG_X, BEG_Y, BEG_X, BEG_Y) != 0){
                speed_runr = 0;
                speed_runf = 1;
                step_in_path = 1;
            flood_fill_algorithm(BEG_X, BEG_Y, TAR_X1, TAR_Y1,
                TAR_X2, TAR_Y2);
            flood_fill_loc_step(labposx, labposy, TAR_X1, TAR_Y1, TAR_X2, TAR_Y2);
        }
        return;
    }
}




//------------------------------------------------------------------
//Funkcje sluzace do bezposredniego kontaktowania sie z pozostalym 
//robota

/**
 * Funkcja zwracajaca informacje, jaka ma byc nastepna czynnosc wykonana przez 
 * robota
 * 0 - stoj w miejscu
 * 1 - jedz prosto o dwa pola
 * 2 - skrec w lewo (w efekcie jedno pole do przodu i jedno w lewo)
 * 3 - skrec w prawo (w efekcie jedno pole do przodu i jedno w prawo)
 * 4 - zawroc w miejscu 
 * 5 - zawroc i jedz o jedno pole do przodu
 * 6 - skrec w lewo w miejscu i jedz o jedno pole do przodu
 * 7 - skrec w prawo w miejscu i jedz o jedno pole do przodu 
 * 8 - jedz prosto o jedno pole
 * 9 - skrec w lewo i stoj w miejscu
 * 10- skrec w prawo i stoj w miejscu
 */
uint8_t getNextAction(void){
    
    if(stop != 0)
        return 0;
    uint8_t prev_dir = mouse_dir;
    //Jakie byly labposx i labposy na samym poczatku tej funkcji - tu zmienne
    //lokalne!
    uint8_t prev_labposx = labposx; 
    uint8_t prev_labposy = labposy;
    
    //Jakikolwiek algstep powoduje jakis obrot (lub brak obrotu) + zawsze
    //jeden krok do przodu.
    //prev_labposx
    algStep();
    
    //Zawroc + idz jeden krok w tyl (bo krok zostal na pewno wykonany)
    if( ((prev_dir == RIGHT) && (mouse_dir == LEFT)) ||
    ((prev_dir == UP) && (mouse_dir == DOWN))||
            ((prev_dir == LEFT) && (mouse_dir == RIGHT))||
    ((prev_dir == DOWN) && (mouse_dir == UP))        ){
            return 5;
    }
    
    //sytuacje, gdy najpierw musi skrecic w miejscu i potem isc krok do przodu
    //w lewo
    if( ((prev_dir == RIGHT) && (labposy > prev_labposy)) ||   
            ((prev_dir == LEFT) && (labposy < prev_labposy)) ||
            ((prev_dir == UP) && (labposx < prev_labposx))  ||
            ((prev_dir == DOWN) && (labposx > prev_labposx))) 
                    return 6;
    //w prawo
    if( ((prev_dir == RIGHT) && (labposy < prev_labposy)) ||   
            ((prev_dir == LEFT) && (labposy > prev_labposy)) ||
            ((prev_dir == UP) && (labposx > prev_labposx))  ||
            ((prev_dir == DOWN) && (labposx < prev_labposx))) 
                    return 7;
    
    //Przypadek, gdy znajdowal sie jeden krok od celu, wykonywane, aby wszedl do
    //niego -- jedyna jeszcze nie rozwazona opcja, to gdy musi isc krok do przodu
  //  if(path[pathlength][0] == labposx  && path[pathlength][1] == labposy){
        return 8;
   // }
    
    
    //Pozostaja sytuacje, gdy najpierw poszedl krok do przodu
    algStep();
    
    
    //Taki sam warunek jak poprzednio, ale tym razem przed skretem robot
    //idzie o krok do przodu 
    
    //Skret w lewo
    if( ((prev_dir == RIGHT) && (labposy > prev_labposy)) ||   
            ((prev_dir == LEFT) && (labposy < prev_labposy)) ||
            ((prev_dir == UP) && (labposx < prev_labposx))  ||
            ((prev_dir == DOWN) && (labposx > prev_labposx))) {
        return 2;
    }
    
    //Skret w prawo
    if( ((prev_dir == RIGHT) && (labposy < prev_labposy)) ||   
            ((prev_dir == LEFT) && (labposy > prev_labposy)) ||
            ((prev_dir == UP) && (labposx > prev_labposx))  ||
            ((prev_dir == DOWN) && (labposx < prev_labposx))) {
        return 3;
    }
    
    
    //Ten sam kierunek, ale ruszyl do przodu - oznacza to, ze lacznie ruszyl
    //sie o dwa pola do przodu
    if( (prev_dir == mouse_dir) && ((labposx != prev_labposx) 
            || (labposy != prev_labposy)) )
        return 1;
    
    //Pozostaly sytuacje, gdy robot zawrocil
    algStep();
    
    //Po "zawroceniu" - tylko wirtualnym - robot moglby isc w lewo (a wiec
    // w porownaniu z poczatkiem skreca w prawo, albo w prawo (czyli w lewo 
    // w porownaniu z poczatkiem) lub tez idzie na wprost
           
    //w lewo
    if( ((prev_dir == RIGHT) && (labposy > prev_labposy)) ||   
            ((prev_dir == LEFT) && (labposy < prev_labposy)) ||
            ((prev_dir == UP) && (labposx < prev_labposx))  ||
            ((prev_dir == DOWN) && (labposx > prev_labposx))) 
                    return 6;
    //w prawo
    if( ((prev_dir == RIGHT) && (labposy < prev_labposy)) ||   
            ((prev_dir == LEFT) && (labposy > prev_labposy)) ||
            ((prev_dir == UP) && (labposx > prev_labposx))  ||
            ((prev_dir == DOWN) && (labposx < prev_labposx))) 
                    return 7;
    //Robot zawrocil, wrocil na poprzednie pole i znowu zawrocil i poszedl o
    //jedno pole do przodu - w efekcie przeszedl jedno pole do przodu
    if((prev_dir == mouse_dir) && ((labposx != prev_labposx) ||  (labposy != prev_labposx)))
        return 8;
    
    //Sytuacja, gdy robot stoi w miejscu - teoretycznie
    //nie powinna zajsc
    if( (prev_dir == mouse_dir) && (labposx == prev_labposx) 
        && (labposy == prev_labposy)) {
        return 0;
    }
    
    //Robot zawrocil i ruszyl sie o jedno pole do przodu
    return 5;
}

