#include "tetris.h"
#include "print_table.h"
#include "colors.h"
#include "tetris.h"
#include "types.h"

const int PER = 3000000;
const char FILLED[7] = {'0', '1', '2', '3', '4', '5', '6'};
const char EMPTY = ' ';
static char screen[22][12];
char answer[3];
int save_i, save_j;
int version, result;
int figure, key, r;
int new_operation = 1;
const int ver[7] = {1, 4, 4, 2, 2, 2, 4};
const char digits[10] = {
    '0', '1', '2', '3', '4',
    '5', '6', '7', '8', '9'};



/*
 * выводит состояние screen на данный момент - ОК
 */
static char print_table[22][80];
void full_print(int mode) {
    clear_screen();
    for (int i=0;i<22;i++){
        for (int j=0;j<12;j++){
            print_table[i][j]=(char)(screen[i][j]);
        }
    }
	if (mode == 0) {
		print_colored_symbols(print_table, (size_t)22, (size_t)12);
	} else {
		print_monotonic_table(print_table, (size_t)22, (size_t)12, WHITE);
	}

}


/*
 * задает начальное состояние screen - OK
 */
void start_screen() {
    for (int j = 1; j < 11; ++j) {
        screen[0][j] = (char)'-';
    }
    for (int j = 1; j < 11; ++j) {
        screen[21][j] = (char)'-';
    }
    for (int i = 1; i < 21; ++i) {
        screen[i][0] = (char)'|';
    }
    for (int i = 1; i < 21; ++i) {
        screen[i][11] = (char)'|';
    }
    screen[0][0] = (char)'+';
    screen[21][0] = (char)'+';
    screen[0][11] = (char)'+';
    screen[21][11] = (char)'+';
    for (int i = 1; i < 21; ++i) {
        for (int j = 1; j < 11; ++j) {
            screen[i][j] = EMPTY;
        }
    }
    
}

/*
 * создает новое состояние при движении вниз, если может создать, и возвращает 1 (иначе - вернет 0) - OK
 */
int next_state() {
    switch(figure) {
        case 0:
            if (screen[save_i + 2][save_j] != EMPTY || screen[save_i + 2][save_j + 1] != EMPTY) {
                return 0;
            }
            screen[save_i + 2][save_j] = screen[save_i + 2][save_j + 1] = FILLED[0];
            screen[save_i][save_j] = screen[save_i][save_j + 1] = EMPTY;
            break;
        case 1:
            switch (version) {
                case 0 :
                    if (screen[save_i][save_j - 1] != EMPTY || screen[save_i + 2][save_j] != EMPTY) {
                        return 0;
                    }
                    screen[save_i][save_j - 1] = screen[save_i + 2][save_j] = FILLED[1];
                    screen[save_i - 1][save_j - 1] = screen[save_i - 1][save_j] = EMPTY;
                    break;
                case 1:
                    if (screen[save_i + 1][save_j - 1] != EMPTY || screen[save_i + 1][save_j] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return 0;
                    }
                    screen[save_i + 1][save_j - 1] = screen[save_i + 1][save_j] = screen[save_i + 1][save_j + 1] = FILLED[1];
                    screen[save_i][save_j - 1] = screen[save_i][save_j] = screen[save_i - 1][save_j + 1] = EMPTY;
                    break;
                case 2:
                    if (screen[save_i + 2][save_j] != EMPTY || screen[save_i + 2][save_j + 1] != EMPTY) {
                        return 0;
                    }
                    screen[save_i + 2][save_j] = screen[save_i + 2][save_j + 1] = FILLED[1];
                    screen[save_i - 1][save_j] = screen[save_i + 1][save_j + 1] = EMPTY;
                    break;
                default:
                    if (screen[save_i + 2][save_j - 1] != EMPTY || screen[save_i + 1][save_j] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return 0;
                    }
                    screen[save_i + 2][save_j - 1] = screen[save_i + 1][save_j] = screen[save_i + 1][save_j + 1] = FILLED[1];
                    screen[save_i][save_j - 1] = screen[save_i][save_j] = screen[save_i][save_j + 1]  = EMPTY;
            }
            break;
        case 2:
            switch (version) {
                case 0 :
                    if (screen[save_i][save_j + 1] != EMPTY || screen[save_i + 2][save_j] != EMPTY) {
                        return 0;
                    }
                    screen[save_i][save_j + 1] = screen[save_i + 2][save_j] = FILLED[2];
                    screen[save_i - 1][save_j + 1] = screen[save_i - 1][save_j] = EMPTY;
                    break;
                case 1:
                    if (screen[save_i + 2][save_j + 1] != EMPTY || screen[save_i + 1][save_j] != EMPTY || screen[save_i + 1][save_j - 1] != EMPTY) {
                        return 0;
                    }
                    screen[save_i + 2][save_j + 1] = screen[save_i + 1][save_j] = screen[save_i + 1][save_j - 1] = FILLED[2];
                    screen[save_i][save_j - 1] = screen[save_i][save_j] = screen[save_i][save_j + 1]  = EMPTY;
                    break;
                case 2:
                    if (screen[save_i + 2][save_j] != EMPTY || screen[save_i + 2][save_j - 1] != EMPTY) {
                        return 0;
                    }
                    screen[save_i + 2][save_j] = screen[save_i + 2][save_j - 1] = FILLED[2];
                    screen[save_i - 1][save_j] = screen[save_i + 1][save_j - 1] = EMPTY;
                    break;
                default:
                    if (screen[save_i + 1][save_j - 1] != EMPTY || screen[save_i + 1][save_j] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return 0;
                    }
                    screen[save_i + 1][save_j - 1] = screen[save_i + 1][save_j] = screen[save_i + 1][save_j + 1] = FILLED[2];
                    screen[save_i][save_j + 1] = screen[save_i][save_j] = screen[save_i - 1][save_j - 1] = EMPTY;
            }
            break;
        case 3:
            switch (version) {
                case 0 :
                    if (screen[save_i + 1][save_j] != EMPTY || screen[save_i + 2][save_j - 1] != EMPTY) {
                        return 0;
                    }
                    screen[save_i + 1][save_j] = screen[save_i + 2][save_j - 1] = FILLED[3];
                    screen[save_i - 1][save_j] = screen[save_i][save_j - 1] = EMPTY;
                    break;
                default:
                    if (screen[save_i][save_j - 1] != EMPTY || screen[save_i + 1][save_j] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return 0;
                    }
                    screen[save_i][save_j - 1] = screen[save_i + 1][save_j] = screen[save_i + 1][save_j + 1] = FILLED[3];
                    screen[save_i - 1][save_j - 1] = screen[save_i - 1][save_j] = screen[save_i][save_j + 1] = EMPTY;
            }
            break;
        case 4:
            switch (version) {
                case 0 :
                    if (screen[save_i + 1][save_j] != EMPTY || screen[save_i + 2][save_j + 1] != EMPTY) {
                        return 0;
                    }
                    screen[save_i + 1][save_j] = screen[save_i + 2][save_j + 1] = FILLED[4];
                    screen[save_i - 1][save_j] = screen[save_i][save_j + 1] = EMPTY;
                    break;
                default:
                    if (screen[save_i + 1][save_j + 1] != EMPTY || screen[save_i + 2][save_j] != EMPTY || screen[save_i + 2][save_j - 1] != EMPTY) {
                        return 0;
                    }
                    screen[save_i + 1][save_j + 1] = screen[save_i + 2][save_j] = screen[save_i + 2][save_j - 1] = FILLED[4];
                    screen[save_i][save_j + 1] = screen[save_i][save_j] = screen[save_i + 1][save_j - 1] = EMPTY;
            }
            break;
        case 5:
            switch (version) {
                case 0 :
                    for (int k = -1; k < 3; ++k) {
                        if (screen[save_i + 1][save_j + k] != EMPTY) {
                            return 0;
                        }
                    }
                    for (int k = -1; k < 3; ++k) {
                        screen[save_i + 1][save_j + k] = FILLED[5];
                        screen[save_i][save_j + k] = EMPTY;
                    }
                    break;
                default:
                    if (screen[save_i + 3][save_j] != EMPTY) {
                        return 0;
                    }
                    screen[save_i + 3][save_j] = FILLED[5];
                    screen[save_i - 1][save_j] = EMPTY;
            }
            break;
        default:
            switch (version) {
                case 0 :
                    if (screen[save_i + 2][save_j] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return 0;
                    }
                    screen[save_i + 2][save_j] = screen[save_i + 1][save_j + 1] = FILLED[6];
                    screen[save_i - 1][save_j] = screen[save_i][save_j + 1] = EMPTY;
                    break;
                case 1:
                    if (screen[save_i + 1][save_j - 1] != EMPTY || screen[save_i + 2][save_j] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return 0;
                    }
                    screen[save_i + 1][save_j - 1] = screen[save_i + 2][save_j] = screen[save_i + 1][save_j + 1] = FILLED[6];
                    screen[save_i][save_j - 1] = screen[save_i][save_j] = screen[save_i][save_j + 1]  = EMPTY;
                    break;
                case 2:
                    if (screen[save_i + 2][save_j] != EMPTY || screen[save_i + 1][save_j - 1] != EMPTY) {
                        return 0;
                    }
                    screen[save_i + 2][save_j] = screen[save_i + 1][save_j - 1] = FILLED[6];
                    screen[save_i - 1][save_j] = screen[save_i][save_j - 1] = EMPTY;
                    break;
                default:
                    if (screen[save_i + 1][save_j - 1] != EMPTY || screen[save_i + 1][save_j] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return 0;
                    }
                    screen[save_i + 1][save_j - 1] = screen[save_i + 1][save_j] = screen[save_i + 1][save_j + 1] = FILLED[6];
                    screen[save_i][save_j - 1] = screen[save_i - 1][save_j] = screen[save_i][save_j + 1] = EMPTY;
            }
            
    }
    ++save_i;
    return 1;
}

/*
 * поворачивает фигуру, если может - ОК
 */
void change_version() {
    switch(figure) {
        case 0:
            return;
        case 1:
            switch(version) {
                case 0:
                    if (screen[save_i - 1][save_j + 1] != EMPTY || screen[save_i][save_j + 1] != EMPTY || screen[save_i][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j + 1] = screen[save_i][save_j + 1] = screen[save_i][save_j - 1] = FILLED[1];
                    screen[save_i + 1][save_j] = screen[save_i - 1][save_j] = screen[save_i - 1][save_j - 1] = EMPTY;
                    break;
                case 1:
                    if (screen[save_i - 1][save_j] != EMPTY || screen[save_i + 1][save_j] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j] = screen[save_i + 1][save_j] = screen[save_i + 1][save_j + 1] = FILLED[1];
                    screen[save_i - 1][save_j + 1] = screen[save_i][save_j + 1] = screen[save_i][save_j - 1] = EMPTY;
                    break;
                case 2:
                    if (screen[save_i][save_j - 1] != EMPTY || screen[save_i + 1][save_j - 1] != EMPTY || screen[save_i][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j - 1] = screen[save_i + 1][save_j - 1] = screen[save_i][save_j + 1] = FILLED[1];
                    screen[save_i - 1][save_j] = screen[save_i + 1][save_j] = screen[save_i + 1][save_j + 1] = EMPTY;
                    break;
                default:
                    if (screen[save_i + 1][save_j] != EMPTY || screen[save_i - 1][save_j] != EMPTY || screen[save_i - 1][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i + 1][save_j] = screen[save_i - 1][save_j] = screen[save_i - 1][save_j - 1] = FILLED[1];
                    screen[save_i][save_j - 1] = screen[save_i + 1][save_j - 1] = screen[save_i][save_j + 1] = EMPTY;
            }
            break;
        case 2:
            switch (version) {
                case 0:
                    if (screen[save_i][save_j + 1] != EMPTY || screen[save_i][save_j - 1] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j + 1] = screen[save_i][save_j - 1] = screen[save_i + 1][save_j + 1] = FILLED[2];
                    screen[save_i + 1][save_j] = screen[save_i - 1][save_j] = screen[save_i - 1][save_j + 1] = EMPTY;
                    break;
                case 1:
                    if (screen[save_i + 1][save_j] != EMPTY || screen[save_i - 1][save_j] != EMPTY || screen[save_i + 1][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i + 1][save_j] = screen[save_i - 1][save_j] = screen[save_i + 1][save_j - 1] = FILLED[2];
                    screen[save_i][save_j + 1] = screen[save_i][save_j - 1] = screen[save_i + 1][save_j + 1] = EMPTY;
                    break;
                case 2:
                    if (screen[save_i][save_j - 1] != EMPTY || screen[save_i][save_j + 1] != EMPTY || screen[save_i - 1][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j + 1] = screen[save_i][save_j - 1] = screen[save_i - 1][save_j - 1] = FILLED[2];
                    screen[save_i + 1][save_j] = screen[save_i - 1][save_j] = screen[save_i + 1][save_j - 1] = EMPTY;
                    break;
                default:
                    if (screen[save_i + 1][save_j] != EMPTY || screen[save_i - 1][save_j] != EMPTY || screen[save_i - 1][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i + 1][save_j] = screen[save_i - 1][save_j] = screen[save_i - 1][save_j + 1] = FILLED[2];
                    screen[save_i][save_j + 1] = screen[save_i][save_j - 1] = screen[save_i - 1][save_j - 1] = EMPTY;
            }
            break;
        case 3:
            switch (version) {
                case 0:
                    if (screen[save_i - 1][save_j - 1] != EMPTY || screen[save_i][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j - 1] = screen[save_i][save_j + 1] = FILLED[3];
                    screen[save_i][save_j - 1] = screen[save_i + 1][save_j - 1] = EMPTY;
                    break;
                default:
                    if (screen[save_i][save_j - 1] != EMPTY || screen[save_i + 1][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j - 1] = screen[save_i + 1][save_j - 1] = FILLED[3];
                    screen[save_i - 1][save_j - 1] = screen[save_i][save_j + 1] = EMPTY;
            }
            break;
        case 4:
            switch (version) {
                case 0:
                    if (screen[save_i + 1][save_j - 1] != EMPTY || screen[save_i + 1][save_j] != EMPTY) {
                        return;
                    }
                    screen[save_i + 1][save_j - 1] = screen[save_i + 1][save_j] = FILLED[4];
                    screen[save_i - 1][save_j] = screen[save_i + 1][save_j + 1] = EMPTY;
                    break;
                default:
                    if (screen[save_i - 1][save_j] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j] = screen[save_i + 1][save_j + 1] = FILLED[4];
                    screen[save_i + 1][save_j - 1] = screen[save_i + 1][save_j] = EMPTY;
            }
            break;
        case 5:
            switch (version) {
                case 0:
                    if (screen[save_i - 1][save_j] != EMPTY || screen[save_i + 1][save_j] != EMPTY || screen[save_i + 2][save_j] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j] = screen[save_i + 1][save_j] = screen[save_i + 2][save_j] = FILLED[5];
                    screen[save_i][save_j - 1] = screen[save_i][save_j + 1] = screen[save_i][save_j + 2] = EMPTY;
                    break;
                default:
                    if (screen[save_i][save_j - 1] != EMPTY || screen[save_i][save_j + 1] != EMPTY || screen[save_i][save_j + 2] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j - 1] = screen[save_i][save_j + 1] = screen[save_i][save_j + 2] = FILLED[5];
                    screen[save_i - 1][save_j] = screen[save_i + 1][save_j] = screen[save_i + 2][save_j] = EMPTY;
            }
            break;
        default:
            switch (version) {
                case 0:
                    if (screen[save_i][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j - 1] = FILLED[6];
                    screen[save_i - 1][save_j] = EMPTY;
                    break;
                case 1:
                    if (screen[save_i - 1][save_j] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j] = FILLED[6];
                    screen[save_i][save_j + 1] = EMPTY;
                    break;
                case 2:
                    if (screen[save_i][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j + 1] = FILLED[6];
                    screen[save_i + 1][save_j] = EMPTY;
                    break;
                default:
                    if (screen[save_i + 1][save_j] != EMPTY) {
                        return;
                    }
                    screen[save_i + 1][save_j] = FILLED[6];
                    screen[save_i][save_j - 1] = EMPTY;
            }
    }
    version = (version + 1) % ver[figure];
    return;
}

/*
 * сдвигает фигуру влево, если может - ОК
 */
void try_left() {
    switch (figure) {
        case 0:
            if (screen[save_i][save_j - 1] != EMPTY || screen[save_i + 1][save_j - 1] != EMPTY) {
                return;
            }
            screen[save_i][save_j - 1] = screen[save_i + 1][save_j - 1] = FILLED[0];
            screen[save_i][save_j + 1] = screen[save_i + 1][save_j + 1] = EMPTY;
            break;
        case 1:
            switch(version) {
                case 0:
                    if (screen[save_i - 1][save_j - 2] != EMPTY || screen[save_i][save_j - 1] != EMPTY || screen[save_i + 1][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j - 2] = screen[save_i][save_j - 1] = screen[save_i + 1][save_j - 1] = FILLED[1];
                    screen[save_i - 1][save_j] = screen[save_i][save_j] = screen[save_i + 1][save_j] = EMPTY;
                    break;
                case 1:
                    if (screen[save_i][save_j - 2] != EMPTY || screen[save_i - 1][save_j] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j - 2] = screen[save_i - 1][save_j] = FILLED[1];
                    screen[save_i][save_j + 1] = screen[save_i - 1][save_j + 1] = EMPTY;
                    break;
                case 2:
                    if (screen[save_i - 1][save_j - 1] != EMPTY || screen[save_i][save_j - 1] != EMPTY || screen[save_i + 1][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j - 1] = screen[save_i][save_j - 1] = screen[save_i + 1][save_j - 1] = FILLED[1];
                    screen[save_i + 1][save_j + 1] = screen[save_i][save_j] = screen[save_i - 1][save_j] = EMPTY;
                    break;
                default:
                    if (screen[save_i][save_j - 2] != EMPTY || screen[save_i + 1][save_j - 2] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j - 2] = screen[save_i + 1][save_j - 2] = FILLED[1];
                    screen[save_i + 1][save_j - 1] = screen[save_i][save_j + 1] = EMPTY;
            }
            break;
        case 2:
            switch(version) {
                case 0:
                    if (screen[save_i - 1][save_j - 1] != EMPTY || screen[save_i][save_j - 1] != EMPTY || screen[save_i + 1][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j - 1] = screen[save_i][save_j - 1] = screen[save_i + 1][save_j - 1] = FILLED[2];
                    screen[save_i - 1][save_j + 1] = screen[save_i][save_j] = screen[save_i + 1][save_j] = EMPTY;
                    break;
                case 1:
                    if (screen[save_i][save_j - 2] != EMPTY || screen[save_i + 1][save_j] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j - 2] = screen[save_i + 1][save_j] = FILLED[2];
                    screen[save_i][save_j + 1] = screen[save_i + 1][save_j + 1] = EMPTY;
                    break;
                case 2:
                    if (screen[save_i - 1][save_j - 1] != EMPTY || screen[save_i][save_j - 1] != EMPTY || screen[save_i + 1][save_j - 2] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j - 1] = screen[save_i][save_j - 1] = screen[save_i + 1][save_j - 2] = FILLED[2];
                    screen[save_i - 1][save_j] = screen[save_i][save_j] = screen[save_i + 1][save_j] = EMPTY;
                    break;
                default:
                    if (screen[save_i][save_j - 2] != EMPTY || screen[save_i - 1][save_j - 2] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j - 2] = screen[save_i - 1][save_j - 2] = FILLED[2];
                    screen[save_i - 1][save_j - 1] = screen[save_i][save_j + 1] = EMPTY;
            }
            break;
        case 3:
            switch (version) {
                case 0:
                    if (screen[save_i][save_j - 2] != EMPTY || screen[save_i + 1][save_j - 2] != EMPTY || screen[save_i - 1][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j - 2] = screen[save_i + 1][save_j - 2] = screen[save_i - 1][save_j - 1] = FILLED[3];
                    screen[save_i][save_j] = screen[save_i - 1][save_j] = screen[save_i + 1][save_j - 1] = EMPTY;
                    break;
                default:
                    if (screen[save_i - 1][save_j - 2] != EMPTY || screen[save_i][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j - 2] = screen[save_i][save_j - 1] = FILLED[3];
                    screen[save_i - 1][save_j] = screen[save_i][save_j + 1] = EMPTY;
            }
            break;
        case 4:
            switch (version) {
                case 0:
                    if (screen[save_i][save_j - 1] != EMPTY || screen[save_i - 1][save_j - 1] != EMPTY || screen[save_i + 1][save_j] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j - 1] = screen[save_i - 1][save_j - 1] = screen[save_i + 1][save_j] = FILLED[4];
                    screen[save_i][save_j + 1] = screen[save_i + 1][save_j + 1] = screen[save_i - 1][save_j] = EMPTY;
                    break;
                default:
                    if (screen[save_i + 1][save_j - 2] != EMPTY || screen[save_i][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i + 1][save_j - 2] = screen[save_i][save_j - 1] = FILLED[4];
                    screen[save_i + 1][save_j] = screen[save_i][save_j + 1] = EMPTY;
            }
            break;
        case 5:
            switch (version) {
                case 0:
                    if (screen[save_i][save_j - 2] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j - 2] = FILLED[5];
                    screen[save_i][save_j + 2] = EMPTY;
                    break;
                default:
                    if (screen[save_i - 1][save_j - 1] != EMPTY || screen[save_i][save_j - 1] != EMPTY || screen[save_i + 1][save_j - 1] != EMPTY || screen[save_i + 2][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j - 1] = screen[save_i][save_j - 1] = screen[save_i + 1][save_j - 1] = screen[save_i + 2][save_j - 1] = FILLED[5];
                    screen[save_i - 1][save_j] = screen[save_i][save_j] = screen[save_i + 1][save_j] = screen[save_i + 2][save_j] = EMPTY;
            }
            break;
        default:
            switch (version) {
                case 0:
                    if (screen[save_i - 1][save_j - 1] != EMPTY || screen[save_i][save_j - 1] != EMPTY || screen[save_i + 1][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j - 1] = screen[save_i][save_j - 1] = screen[save_i + 1][save_j - 1] = FILLED[6];
                    screen[save_i - 1][save_j] = screen[save_i][save_j + 1] = screen[save_i + 1][save_j] = EMPTY;
                    break;
                case 1:
                    if (screen[save_i][save_j - 2] != EMPTY || screen[save_i + 1][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j - 2] = screen[save_i + 1][save_j - 1] = FILLED[6];
                    screen[save_i][save_j + 1] = screen[save_i + 1][save_j] = EMPTY;
                    break;
                case 2:
                    if (screen[save_i - 1][save_j - 1] != EMPTY || screen[save_i][save_j - 2] != EMPTY || screen[save_i + 1][save_j - 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j - 1] = screen[save_i][save_j - 2] = screen[save_i + 1][save_j - 1] = FILLED[6];
                    screen[save_i - 1][save_j] = screen[save_i][save_j] = screen[save_i + 1][save_j] = EMPTY;
                    break;
                default:
                    if (screen[save_i - 1][save_j - 1] != EMPTY || screen[save_i][save_j - 2] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j - 1] = screen[save_i][save_j - 2] = FILLED[6];
                    screen[save_i - 1][save_j] = screen[save_i][save_j + 1] = EMPTY;
            }
    }
    --save_j;
    return;
}

/*
 * сдвигает фигуру вправо, если может - ОК
 */
void try_right() {
    switch (figure) {
        case 0:
            if (screen[save_i][save_j + 2] != EMPTY || screen[save_i + 1][save_j + 2] != EMPTY) {
                return;
            }
            screen[save_i][save_j + 2] = screen[save_i + 1][save_j + 2] = FILLED[0];
            screen[save_i][save_j] = screen[save_i + 1][save_j] = EMPTY;
            break;
        case 1:
            switch (version) {
                case 0:
                    if (screen[save_i - 1][save_j + 1] != EMPTY || screen[save_i][save_j + 1] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j +1] = screen[save_i][save_j + 1] = screen[save_i + 1][save_j + 1] = FILLED[1];
                    screen[save_i - 1][save_j - 1] = screen[save_i][save_j] = screen[save_i + 1][save_j] = EMPTY;
                    break;
                case 1:
                    if (screen[save_i][save_j + 2] != EMPTY || screen[save_i - 1][save_j + 2] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j + 2] = screen[save_i - 1][save_j + 2] = FILLED[1];
                    screen[save_i - 1][save_j + 1] = screen[save_i][save_j - 1] = EMPTY;
                    break;
                case 2:
                    if (screen[save_i - 1][save_j + 1] != EMPTY || screen[save_i][save_j + 1] != EMPTY || screen[save_i + 1][save_j + 2] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j + 1] = screen[save_i][save_j + 1] = screen[save_i + 1][save_j + 2] = FILLED[1];
                    screen[save_i - 1][save_j] = screen[save_i][save_j] = screen[save_i + 1][save_j] = EMPTY;
                    break;
                default:
                    if (screen[save_i][save_j + 2] != EMPTY || screen[save_i + 1][save_j] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j + 2] = screen[save_i + 1][save_j] = FILLED[1];
                    screen[save_i][save_j - 1] = screen[save_i + 1][save_j - 1] = EMPTY;
            }
            break;
        case 2:
            switch (version) {
                case 0:
                    if (screen[save_i - 1][save_j + 2] != EMPTY || screen[save_i][save_j + 1] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j + 2] = screen[save_i][save_j + 1] = screen[save_i + 1][save_j + 1] = FILLED[2];
                    screen[save_i - 1][save_j] = screen[save_i][save_j] = screen[save_i + 1][save_j] = EMPTY;
                    break;
                case 1:
                    if (screen[save_i][save_j + 2] != EMPTY || screen[save_i + 1][save_j + 2] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j + 2] = screen[save_i + 1][save_j + 2] = FILLED[2];
                    screen[save_i + 1][save_j + 1] = screen[save_i][save_j - 1] = EMPTY;
                    break;
                case 2:
                    if (screen[save_i - 1][save_j + 1] != EMPTY || screen[save_i][save_j + 1] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j + 1] = screen[save_i][save_j + 1] = screen[save_i + 1][save_j + 1] = FILLED[2];
                    screen[save_i + 1][save_j - 1] = screen[save_i][save_j] = screen[save_i - 1][save_j] = EMPTY;
                    break;
                default:
                    if (screen[save_i][save_j + 2] != EMPTY || screen[save_i - 1][save_j] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j + 2] = screen[save_i - 1][save_j] = FILLED[2];
                    screen[save_i][save_j - 1] = screen[save_i - 1][save_j - 1] = EMPTY;
            }
            break;
        case 3:
            switch (version) {
                case 0:
                    if (screen[save_i][save_j + 1] != EMPTY || screen[save_i - 1][save_j + 1] != EMPTY || screen[save_i + 1][save_j] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j + 1] = screen[save_i - 1][save_j + 1] = screen[save_i + 1][save_j] = FILLED[3];
                    screen[save_i][save_j - 1] = screen[save_i + 1][save_j - 1] = screen[save_i - 1][save_j] = EMPTY;
                    break;
                default:
                    if (screen[save_i][save_j + 2] != EMPTY || screen[save_i - 1][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j + 2] = screen[save_i - 1][save_j + 1] = FILLED[3];
                    screen[save_i][save_j] = screen[save_i - 1][save_j - 1] = EMPTY;
            }
            break;
        case 4:
            switch (version) {
                case 0:
                    if (screen[save_i][save_j + 2] != EMPTY || screen[save_i + 1][save_j + 2] != EMPTY || screen[save_i - 1][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j + 2] = screen[save_i + 1][save_j + 2] = screen[save_i - 1][save_j + 1] = FILLED[4];
                    screen[save_i][save_j] = screen[save_i - 1][save_j] = screen[save_i + 1][save_j + 1] = EMPTY;
                    break;
                default:
                    if (screen[save_i][save_j + 2] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j + 2] = screen[save_i + 1][save_j + 1] = FILLED[4];
                    screen[save_i][save_j] = screen[save_i + 1][save_j - 1] = EMPTY;
            }
            break;
        case 5:
            switch (version) {
                case 0:
                    if (screen[save_i][save_j + 3] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j + 3] = FILLED[5];
                    screen[save_i][save_j - 1] = EMPTY;
                    break;
                default:
                    if (screen[save_i - 1][save_j + 1] != EMPTY || screen[save_i][save_j + 1] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY || screen[save_i + 2][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j + 1] = screen[save_i][save_j + 1] = screen[save_i + 1][save_j + 1] = screen[save_i + 2][save_j + 1] = FILLED[5];
                    screen[save_i - 1][save_j] = screen[save_i][save_j] = screen[save_i + 1][save_j] = screen[save_i + 2][save_j] = EMPTY;
            }
            break;
        default:
            switch (version) {
                case 0:
                    if (screen[save_i - 1][save_j + 1] != EMPTY || screen[save_i][save_j + 2] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j + 1] = screen[save_i][save_j + 2] = screen[save_i + 1][save_j + 1] = FILLED[6];
                    screen[save_i - 1][save_j] = screen[save_i][save_j] = screen[save_i + 1][save_j] = EMPTY;
                    break;
                case 1:
                    if (screen[save_i][save_j + 2] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i][save_j + 2] = screen[save_i + 1][save_j + 1] = FILLED[6];
                    screen[save_i][save_j - 1] = screen[save_i + 1][save_j] = EMPTY;
                    break;
                case 2:
                    if (screen[save_i - 1][save_j + 1] != EMPTY || screen[save_i][save_j + 1] != EMPTY || screen[save_i + 1][save_j + 1] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j + 1] = screen[save_i][save_j + 1] = screen[save_i + 1][save_j + 1] = FILLED[6];
                    screen[save_i - 1][save_j] = screen[save_i][save_j - 1] = screen[save_i + 1][save_j] = EMPTY;
                    break;
                default:
                    if (screen[save_i - 1][save_j + 1] != EMPTY || screen[save_i][save_j + 2] != EMPTY) {
                        return;
                    }
                    screen[save_i - 1][save_j + 1] = screen[save_i][save_j + 2] = FILLED[6];
                    screen[save_i - 1][save_j] = screen[save_i][save_j - 1] = EMPTY;
            }
    }
    ++save_j;
    return;
}

/*
 * проверяет законченные уровни - ОК
 */
void check_level() {
    int start_levels = 20;
    int ok = 1;
    while (start_levels > 0) {
        for (int j = 1; j < 11; ++j) {
            if (screen[start_levels][j] == EMPTY) {
                ok = 0;
            }
        }
        if (ok) {
            break;
        } else {
            --start_levels;
            ok = 1;
        }
    }
    if (start_levels == 0) {
        return;
    }
    ok = 1;
    int end_levels = start_levels;
    while (end_levels > 0) {
        for (int j = 1; j < 11; ++j) {
            if (screen[end_levels][j] == EMPTY) {
                ok = 0;
            }
        }
        if (ok) {
            --end_levels;
            ok = 1;
        } else {
            break;
        }
    }
    int dif = start_levels - end_levels;
    for (int i = end_levels; i > 0; --i) {
        for (int j = 1; j < 11; ++j) {
            screen[i + dif][j] = screen[i][j];
        }
    }
    for (int i = 1; i < dif; ++i) {
        for (int j = 1; j < 11; ++j) {
            screen[i][j] = EMPTY;
        }
    }
    result += dif;
}

/*
 * возвращает код нажатой кнопки - ОК
 */
unsigned char cur_button() {
    unsigned char res;
    asm ("inb %1, %0" : "=a"(res) : "Nd"(0x60));
    return res;
}

/*
 * переводит итоговый результат в массив символов - OK
 */
int create_answer() {
    if (result == 0) {
        answer[0] = '0';
        return 1;
    }
    int digit;
    int pointer = 0;
    while (result != 0) {
        digit = result % 10;
        answer[pointer] = digits[digit];
        ++pointer;
        result /= 10;
    }
    return pointer;
}

/*
* выводит сообщение о конце игры - OK
*/
void game_over() {
	/*
	* GAME OVER!
	* score: result
	* restart: w
	*/
	start_screen();
	char buffer1[10] = {'G', 'A', 'M', 'E', ' ', 'O', 'V', 'E', 'R', '!'};
	for (int j = 1; j < 11; ++j) {
		screen[8][j] = buffer1[j - 1];
	}
	char buffer2[7] = {'s', 'c', 'o', 'r', 'e', ':', ' '};
	for (int j = 1; j < 8; ++j) {
		screen[10][j] = buffer2[j - 1];
	}
	int length = create_answer();
	for (int j = 0; j < length; ++j) {
		screen[10][8 + j] = answer[j];
	}
	char buffer3[10] = {'r', 'e', 's', 't', 'a', 'r', 't', ':', ' ', 'w'};
	for (int j = 1; j < 11; ++j) {
		screen[12][j] = buffer3[j - 1];
	}
	full_print(1);
}


/*
 * запуск тетриса - ОК
 */
void play_tetris(){
    while (1) {
        start_screen();
        int fail = 0;
        while (1) {
            figure = r % 7;
            ++r;
            version = 0;
            switch (figure) {
                case 0:
                    if (screen[1][5] != EMPTY || screen[1][6] != EMPTY || screen[2][5] != EMPTY || screen[2][6] != EMPTY) {
                        fail = 1;
                    }
                    screen[1][5] = screen[1][6] = screen[2][5] = screen[2][6] = FILLED[0];
                    save_i = 1;
                    save_j = 5;
                    break;
                case 1:
                    if (screen[1][5] != EMPTY || screen[1][6] != EMPTY || screen[2][6] != EMPTY || screen[3][6] != EMPTY) {
                        fail = 1;
                    }
                    screen[1][5] = screen[1][6] = screen[2][6] = screen[3][6] = FILLED[1];
                    save_i = 2;
                    save_j = 6;
                    break;
                case 2:
                    if (screen[1][5] != EMPTY || screen[1][6] != EMPTY || screen[2][5] != EMPTY || screen[3][5] != EMPTY) {
                        fail = 1;
                    }
                    screen[1][5] = screen[1][6] = screen[2][5] = screen[3][5] = FILLED[2];
                    save_i = 2;
                    save_j = 5;
                    break;
                case 3:
                    if (screen[1][6] != EMPTY || screen[2][6] != EMPTY || screen[2][5] != EMPTY || screen[3][5] != EMPTY) {
                        fail = 1;
                    }
                    screen[1][6] = screen[2][6] = screen[2][5] = screen[3][5] = FILLED[3];
                    save_i = 2;
                    save_j = 6;
                    break;
                case 4:
                    if (screen[1][5] != EMPTY || screen[2][5] != EMPTY || screen[2][6] != EMPTY || screen[3][6] != EMPTY) {
                        fail = 1;
                    }
                    screen[1][5] = screen[2][5] = screen[2][6] = screen[3][6] = FILLED[4];
                    save_i = 2;
                    save_j = 5;
                    break;
                case 5:
                    if (screen[1][4] != EMPTY || screen[1][5] != EMPTY || screen[1][6] != EMPTY || screen[1][7] != EMPTY) {
                        fail = 1;
                    }
                    screen[1][4] = screen[1][5] = screen[1][6] = screen[1][7] = FILLED[5];
                    save_i = 1;
                    save_j = 5;
                    break;
                default:
                    if (screen[1][5] != EMPTY || screen[2][5] != EMPTY || screen[2][6] != EMPTY || screen[3][5] != EMPTY) {
                        fail = 1;
                    }
                    screen[1][5] = screen[2][5] = screen[2][6] = screen[3][5] = FILLED[6];
                    save_i = 2;
                    save_j = 5;
            }
            full_print(0);
            if (fail) {
                int unused = 0;
                for (int i = 0; i < PER; ++i) {
                    ++unused;
                }
                break;
            }
            while (1) {
                for (int i = 0; i < PER; ++i) {
                    key = cur_button();
                    if (key != 0x11 && key != 0x1f && key != 0x20 && key != 0x1e) {
                        new_operation = 1;
                    } else {
                        if (new_operation) {
                            if (key == 0x11) { //вверх
                                change_version();
                            } else if (key == 0x1f) { //вниз
                                while (next_state()) {};
                            } else if (key == 0x20) { //вправо
                                try_right();
                            } else if (key == 0x1e){ //влево
                                try_left();
                            }
                            r += i;
                            r %= 7;
                            new_operation = 0;
                            full_print(0);
                        }
                    }
                }
                if (next_state()) {
                    full_print(0);
                } else {
                    check_level();
                    break;
                }
            }
        }
        game_over();
        while (1) {
            key = cur_button();
            if (key == 0x11) {
                break;
            }
        }
    }
}
