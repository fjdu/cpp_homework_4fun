#include <stdio.h>
#include <stdlib.h>

#define N_DIR 8
#define BIG_NUM 9999
#define SWAP(t, x, y) do {t _TEMP_XY; _TEMP_XY = y; y = x; x =_TEMP_XY;} while(0)
#define FIND_ALL

//#define VERBOSE
//#define CLOSE_PATH

long int n_trials, nmoves, n_success;
int *moves_x, *moves_y;
int ix_s, iy_s;

class chess{
    public:
        int nx, ny;
        int npassed;
        int **passed;
        chess(int, int);
        chess(chess *);
        ~chess(void);
        void print_state(void);
        void reset_state(int ix, int iy);
        inline bool is_valid_loc(int ix, int iy);
        void copy_chess(chess *b);
        int get_n_hole(int ix, int iy);
};


class rule_horse{
    public:
        int ichoice;
        rule_horse(void);
        ~rule_horse(void);
        inline bool next_step(int *ix, int *iy);
        void set_next_steps(int ix, int iy);
        void reset(void);
        int next_x[N_DIR], next_y[N_DIR];
        static const int dx[N_DIR], dy[N_DIR];
};

const int rule_horse::dx[N_DIR] = \
    {1, 2,  2,  1, -1, -2, -2, -1};
const int rule_horse::dy[N_DIR] = \
    {2, 1, -1, -2,  2,  1, -1, -2};


rule_horse::rule_horse(void){
  ichoice = 0;
}


rule_horse::~rule_horse(void){
  return;
}


inline bool rule_horse::next_step(int *ix, int *iy){
  if (ichoice >= N_DIR || ichoice < 0){
    return false;
  }
  *ix = dx[ichoice];
  *iy = dy[ichoice];
  ichoice++;
  return true;
}


void rule_horse::set_next_steps(int ix, int iy){
  for (int i=0; i<N_DIR; i++) {
      next_x[i] = ix + dx[i];
      next_y[i] = iy + dy[i];
  }
}


void rule_horse::reset(void){
  ichoice = 0;
}


chess::chess(int _nx, int _ny){
    nx = _nx;
    ny = _ny;
    npassed = 0;
    //
    passed = new int*[nx];
    for (int i=0; i<nx; i++){
        passed[i] = new int[ny];
    }
    for (int i=0; i<nx; i++){
        for (int j=0; j<ny; j++){
            passed[i][j] = 0;
        }
    }
}


chess::chess(chess *a){
    nx = a->nx;
    ny = a->ny;
    npassed = a->npassed;
    //
    passed = new int*[nx];
    for (int i=0; i<nx; i++){
        passed[i] = new int[ny];
    }
    for (int i=0; i<nx; i++){
        for (int j=0; j<ny; j++){
            passed[i][j] = a->passed[i][j];
        }
    }
}


chess::~chess(void){
    for (int i=0; i<nx; i++){
        //printf("Deleting %d.\n", i);
        delete [] passed[i];
    }
    delete [] passed;
    //printf("Deleted!\n");
}


void chess::print_state(void){
    for (int i=0; i<nx; i++){
        for (int j=0; j<ny; j++){
            printf("%2d  ", passed[i][j]);
        }
        printf("\n");
    }
}


void chess::reset_state(int ix, int iy){
    for (int i=0; i<nx; i++){
        for (int j=0; j<ny; j++){
            passed[i][j] = 0;
        }
    }
    passed[ix][iy] = 1;
}


int chess::get_n_hole(int ix, int iy) {
  if (!is_valid_loc(ix, iy)) {
      return BIG_NUM;
  }
  int nn = 0;
  for (int i=0; i<N_DIR; i++) {
    int iix = ix + rule_horse::dx[i];
    int iiy = iy + rule_horse::dy[i];
    if (is_valid_loc(iix, iiy)) {
        if (passed[iix][iiy] == 0) {
            nn += 1;
        }
    }
  }
  if (nn == 0) {
      nn = BIG_NUM;
  }
  return nn;
}


inline bool chess::is_valid_loc(int ix, int iy){
  //return (0 <= ix < nx) && (0 <= iy < ny);
  return (0 <= ix) && (ix  < nx) && (0 <= iy) && (iy < ny);
}


void chess::copy_chess(chess *b) {
    this->npassed = b->npassed;
    for (int i=0; i < this->nx; i++){
        for (int j=0; j < this->ny; j++){
            this->passed[i][j] = b->passed[i][j];
        }
    }
}


void sort_next_steps(chess *a_chess, rule_horse *a_horse) {
    for (int i=0; i<N_DIR; i++) {
        for (int j=0; j<i; j++) {
            int ix0 = a_horse->next_x[i];
            int iy0 = a_horse->next_y[i];
            int ix1 = a_horse->next_x[j];
            int iy1 = a_horse->next_y[j];
            int nn0 = a_chess->get_n_hole(ix0, iy0);
            int nn1 = a_chess->get_n_hole(ix1, iy1);
            if (nn0 < nn1) {
                SWAP(int, a_horse->next_x[i], a_horse->next_x[j]);
                SWAP(int, a_horse->next_y[i], a_horse->next_y[j]);
            }
        }
    }
}



bool test_board(chess *a_chess, int ix, int iy) {
    extern long int n_trials, nmoves, n_success;
    //
    //chess b_chess(a_chess->nx, a_chess->ny);
    //// Copy the board state.
    //b_chess.copy_chess(a_chess);
    //
    chess b_chess(a_chess);
    //
    rule_horse a_horse;
    //a_horse.reset();
    //
    int dx1, dy1;
    for (;a_horse.next_step(&dx1, &dy1);) {
        //
        int ix1 = ix + dx1;
        int iy1 = iy + dy1;
        //
        if (b_chess.is_valid_loc(ix1, iy1)) {
            if (b_chess.passed[ix1][iy1] == 0) {
                //
                n_trials += 1;
#ifdef VERBOSE
                printf("%2d, %2d, %2d, %2d\n", \
                       ix, iy, dx1, dy1);
                b_chess.print_state();
                //
#endif
                b_chess.passed[ix1][iy1] = 1;
                b_chess.npassed += 1;
                //
                if (b_chess.npassed == (b_chess.nx * b_chess.ny - 1)) {
                    // The whole board is covered.
                    n_success += 1;
                    return true;
                } else if (test_board(&b_chess, ix1, iy1)) {
                    // Recursion: start from the new location.
#ifndef FIND_ALL
                    return true;
#endif
                } else {
                    // Didn't succeed; reset to previous state.
                    //b_chess.copy_chess(a_chess);
                    b_chess.passed[ix1][iy1] = 0;
                    b_chess.npassed -= 1;
                }
            } else {
                continue;
            }
        } else {
            continue;
        }
    }
    return false;
}


bool test_board_1(chess *a_chess, int ix, int iy, int *imove) {
    // Much faster
    extern long int n_trials, nmoves;
    extern int *moves_x, *moves_y;
    extern int ix_s, iy_s;
    //
    chess b_chess(a_chess->nx, a_chess->ny);
    //
    // Copy the board state.
    b_chess.copy_chess(a_chess);
    //
    rule_horse a_horse;
    a_horse.set_next_steps(ix, iy);
    sort_next_steps(&b_chess, &a_horse);
    //
    for (int i=0; i < N_DIR; i++) {
        //
        int ix1 = a_horse.next_x[i];
        int iy1 = a_horse.next_y[i];
        //
        if (b_chess.is_valid_loc(ix1, iy1)) {
            if (b_chess.passed[ix1][iy1] == 0) {
                //
                n_trials += 1;
#ifdef VERBOSE
                printf("%2d, %2d, %2d, %2d\n", \
                       ix, iy, ix1, iy1);
                b_chess.print_state();
                //
#endif
                b_chess.passed[ix1][iy1] = 1;
                b_chess.npassed += 1;
                //
                if (b_chess.npassed == (a_chess->nx * a_chess->ny - 1)) {
                    // The whole board is covered.
#ifdef CLOSE_PATH
                    int delx = abs(ix_s - ix1);
                    int dely = abs(iy_s - iy1);
                    if (((delx == 2) && (dely ==1)) || \
                        ((delx == 1) && (dely ==2))) {
                        *imove += 1;
                        moves_x[nmoves-*imove] = ix1;
                        moves_y[nmoves-*imove] = iy1;
                        return true;
                    } else {
                        return false;
                    }
#else
                    *imove += 1;
                    moves_x[nmoves-*imove] = ix1;
                    moves_y[nmoves-*imove] = iy1;
                    return true;
#endif
                } else if (test_board_1(&b_chess, \
                            ix1, iy1, imove)) {
                    // Recursion: start from the new location.
                    *imove += 1;
#ifdef VERBOSE
                    printf("%d\n", *imove);
#endif
                    moves_x[nmoves-*imove] = ix1;
                    moves_y[nmoves-*imove] = iy1;
                    return true;
                } else {
                    // Didn't succeed; reset to previous state.
                    b_chess.copy_chess(a_chess);
                }
            } else {
                continue;
            }
        } else {
            continue;
        }
    }
    return false;
}


int main(int argc, char *argv[]){
    extern long int n_trials, nmoves, n_success;
    extern int *moves_x, *moves_y;
    extern int ix_s, iy_s;
    int bd_nx, bd_ny, ix0, iy0, imove;
    //
    if (argc < 5) {
        printf("Usage:\n");
        printf("\t%s nx ny ix iy\n", argv[0]);
        return 1;
    }
    //
    bd_nx = atoi(argv[1]);
    bd_ny = atoi(argv[2]);
    ix0 = atoi(argv[3]);
    iy0 = atoi(argv[4]);
    nmoves = bd_nx * bd_ny;
    n_success = 0;
    //
    ix_s = ix0;
    iy_s = iy0;
    //
    moves_x = new int[nmoves];
    moves_y = new int[nmoves];
    //
    chess a_chess(bd_nx, bd_ny);
    if (!a_chess.is_valid_loc(ix0, iy0)) {
        printf("Invalid location:\n");
        printf("\t%d, %d\n", ix0, iy0);
        return 1;
    }
    //
#ifdef VERBOSE
    printf("Trying to find solution for a %d x %d board...\n", \
        bd_nx, bd_ny);
    printf("Starting from (%d, %d) (zero-based).\n", ix0, iy0);
#endif
    //
    a_chess.reset_state(ix0, iy0);
    moves_x[0] = ix0;
    moves_y[0] = iy0;
    //
    n_trials = 0;
    imove = 0;
#ifdef FIND_ALL
    int bl = test_board(&a_chess, ix0, iy0);
    printf("n_success = %lu\n", n_success);
    return 0;
#endif
    if (test_board_1(&a_chess, ix0, iy0, &imove)) {
#ifdef VERBOSE
        printf("Solution found!\n");
#endif
        for (int i=0; i < nmoves; i++) {
#ifdef VERBOSE
            printf("\t %2d, (%4d, %4d),\n", \
                i+1, moves_x[i]+1, moves_y[i]+1);
#else
            printf("\t (%4d, %4d),\n", \
                moves_x[i]+1, moves_y[i]+1);
#endif
        }
    } else {
#ifdef VERBOSE
        printf("Solution not found.\n");
#endif
    }
#ifdef VERBOSE
    printf("Number of trials = %lu\n", n_trials);
#endif
    return 0;
}
