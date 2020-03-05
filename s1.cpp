#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <string>
#include <sys/time.h>
#include "mynng.hpp"

void methodeBase(){
  //if(_ac != 2) { printf("usage: %s NONOGRAM_INPUT_FILE\n", _av[0]); return 0; }
  srand(4);
  nng_t N;
  //N.load(_av[1]);
  N.load((char*)"problems/nonogram5x5_1_game.txt");
  //N.load((char*)"problems/nonogram5x5_1_game.txt");
  //N.load((char*)"problems/nonogram10x10_1_game.txt");
  N.print_problem_info();
  N.print_board();
  struct timeval i_time;
  struct timeval f_time;
  gettimeofday (&i_time, 0);
  int solved = 0;
  for(int i = 0; i < 1000000; i++) {
    nng_t NN;
    NN.copy(N);
    NN.playout();
    if(NN.score() == 100) { // a binary game score
      gettimeofday (&f_time, 0);
      float time = ((float)(f_time.tv_sec - i_time.tv_sec)) +
        ((float)(f_time.tv_usec - i_time.tv_usec))/1000000.0;
      printf("solved after %d iterations time %.2f\n", i, time);
      //NN.print_board_info();
      NN.print_board();
      solved = 1;
      break;
    }
  }
  if(solved == 0) printf("not solved\n");
}

nng_move_t MonteCarlo(){
  srand(1);
  nng_t N;
  nng_move_t best;
  N.load((char*)"problems/nonogram5x5_3_game.txt");
  N.print_problem_info();
  N.print_board();
  int max = 0, wi = 0;
  std::vector<nng_move_t> moves = N.get_all_moves();
  for(auto m : moves){
    nng_t NN;
    NN.copy(N);
    NN.play(m);
    wi = 0;
    for(int i = 0; i < 1000; i++) {
      NN.copy(N);
      NN.playout();
      if(NN.score() == 100) {
        //NN.print_board();
        wi++;
      }
    }
    if(wi>max){
      std::cout << wi << std::endl;
      best =m;
      max = wi;
    }
  }
  std::cout << best.line << " && " << best.col << "\n";
  return best;
}


int main(int _ac, char** _av) {

  //methodeBase();
  MonteCarlo();

  return 0;
}
