#include "game_tools.h"

#include <stdlib.h>

#include "game.h"
#include "game_ext.h"
#include "game_private.h"

game game_load(char* filename) {
    // Check filename and file existance
    if (filename == NULL) {
        fprintf(stderr, "Invalid file name in game_load()\n");
        exit(5);
    }
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Unable to open file!\n");
        exit(5);
    }

    // Read size and wrapping information
    uint nb_rows, nb_cols, wrapping;
    int ret;

    ret = fscanf(file, " %d  %d %d", &nb_rows, &nb_cols, &wrapping);

    if (nb_rows > 10 || nb_cols > 10 || ret != 3) {
        fprintf(stderr, "Error when loading game!");
        exit(5);
    }

    // read unit find the first \n for the starting grid
    char c = fgetc(file);
    while (c != '\n') {
        c = fgetc(file);
    }
    square* read_data = malloc(sizeof(square) * nb_rows * nb_cols);
    for (uint i = 0; i < nb_rows * nb_cols; i++) {
        c = fgetc(file);
        switch (c) {
            case 'b':
                read_data[i] = S_BLANK;
                break;
            case '*':
                read_data[i] = S_LIGHTBULB;
                break;
            case '0':
                read_data[i] = S_BLACK0;
                break;
            case '1':
                read_data[i] = S_BLACK1;
                break;
            case '2':
                read_data[i] = S_BLACK2;
                break;
            case '3':
                read_data[i] = S_BLACK3;
                break;
            case '4':
                read_data[i] = S_BLACK4;
                break;
            case 'w':
                read_data[i] = S_BLACKU;
                break;
            case '-':
                read_data[i] = S_MARK;
                break;
            case '\n':
                i--;
                break;
            default:
                fprintf(stderr, "unable to read game");
                exit(5);
                break;
        }
    }

    // convert int to bool, wrapping
    bool bool_wrapping;
    bool_wrapping = wrapping != 0;

    game rgame = game_new_ext(nb_rows, nb_cols, read_data, bool_wrapping);

    free(read_data);
    fclose(file);
    game_update_flags(rgame);

    return rgame;
}

void game_save(cgame g, char* filename) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        fprintf(stderr, "inexistant file");
        exit(5);
    }
    uint wrap;
    if (g->wrapping) {
        wrap = 1;
    } else {
        wrap = 0;
    }
    fprintf(f, "%d ", g->nb_rows);
    fprintf(f, "%d ", g->nb_cols);
    fprintf(f, "%d\n", wrap);
    for (uint i = 0; i < game_nb_rows(g); i++) {
        for (uint j = 0; j < game_nb_cols(g); j++) {
            if (game_get_state(g, i, j) == S_LIGHTBULB) {
                fprintf(f, "*");
            }
            if (game_get_state(g, i, j) == S_BLACKU) {
                fprintf(f, "w");
            }
            if (game_is_black(g, i, j) && game_get_state(g, i, j) != S_BLACKU) {
                fprintf(f, "%d", game_get_black_number(g, i, j));
            }
            if (game_is_blank(g, i, j)) {
                fprintf(f, "b");
            }
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

bool genGame(int pos, int size, game g, bool stopAtFirstSolution, game* SolTab, int* nbSolFound ,int nbLightbulb, int posedLight){
    if(pos == size){
        if (posedLight == nbLightbulb){
            if(game_is_over(g)){
                SolTab[*nbSolFound] = game_copy(g);
                return true;
            }
        }
    }

    bool ret;
    g -> squares[pos] = S_BLANK;
    
    ret = genGame(pos+1, size, g, nbLightbulb, posedLight);
    if(stopAtFirstSolution && ret){
        return ret;
    }

    if(posedLight +1 <= nbLightbulb){
        g -> squares[pos] = S_LIGHTBULB;
        ret = genGame(pos+1, size, g, nbLightbulb, posedLight+1);
        if(stopAtFirstSolution && ret){
            return ret;
        }
    }
    return false;
}

bool game_solve(game g){
    return NULL;
}

uint game_nb_solutions(cgame g){
    return NULL;
}