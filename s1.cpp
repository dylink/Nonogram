#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <string>
#include <sys/time.h>
#include <limits>
#include <math.h>
#include "mynng.hpp"

void methodeBase(int bound){
  //if(_ac != 2) { printf("usage: %s NONOGRAM_INPUT_FILE\n", _av[0]); return 0; }
  nng_t N;
  //N.load(_av[1]);
  N.load((char*)"problems/nonogram3x3_1_game.txt");
  //N.load((char*)"problems/nonogram5x5_1_game.txt");
  //N.load((char*)"problems/nonogram10x10_1_game.txt");
  N.print_problem_info();
  N.print_board();
  struct timeval i_time;
  struct timeval f_time;
  gettimeofday (&i_time, 0);
  int solved = 0;
  for(int i = 0; i < bound; i++) {
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

nng_move_t MonteCarlo(int bound){

  nng_t N;
  nng_move_t best, best2;
  N.load((char*)"problems/nonogram5x5_1_game.txt");
  N.print_problem_info();
  int max = 0, wi = 0;
  N.fill();
  std::vector<nng_move_t> moves = N.get_all_moves();
  N.print_board();
  nng_t NN;
  NN.copy(N);
  for(auto m : moves){
    NN.copy(N);
    NN.play(m);
    wi = 0;
    for(int i = 0; i < bound; i++) {
      nng_t NNN; NNN.copy(NN);
      //NN.copy(N);
      NNN.playout();
      if(NNN.score() == 100) {
        NNN.print_board();
        wi++;
      }
    }
    if(wi>max){
      best =m;
      max = wi;
    }
    NN.copy(N);
  }
  return best;
}

float ucb(int wi, int ni, int n){
  if(!n) return std::numeric_limits<float>::infinity();
  return (wi/ni)+1.41*sqrt(log(n)/ni);
}

Noeud& selection(Noeud &racine, nng_t &etat){
  std::vector<nng_move_t> moves = etat.get_all_moves();
  if(racine.parent != nullptr) racine.nbVisits++;
  if(racine.enfants.empty()){
    if(racine.isEnd) return racine;
    for(auto m : moves){
      Noeud copy;
      copy.move = m;
      copy.parent = &racine;
      racine.enfants.push_back(copy);
    }
  }
  if(racine.enfants.empty()){
    racine.isEnd = true;
    return racine;
  }
  static float max = -1;
  Noeud temp;
  Noeud* best = &temp;
  for(auto& m : racine.enfants){
    if(!m.nbVisits){
      etat.play(m.move);
      m.nbVisits++;
      return m;
    }
    float eval = ucb(m.nbWins, m.nbVisits, racine.nbVisits);
    if(eval > max){
      max = eval;
      best = &m;
    }
  }
  etat.play(best->move);
  if(etat.score() == 100){
    return *best;
  }
  return selection(*best, etat);
}

void backPropagation(Noeud &noeud){
  Noeud *current = &noeud;
  while(current->parent != nullptr){
    current->parent->nbVisits += current->nbVisits;
    current->parent->nbWins += current->nbWins;
    cout << current->parent->nbWins << " Oui\n";
    current = current->parent;
  }
}

<<<<<<< HEAD
Noeud MCTS(int bound, Noeud &racine, nng_t &etat){
  Noeud best;
  float max = 0;
  nng_move_t move;
  move.line = 0; move.col = 1;
  //etat.play(move);

  etat.fill();
  etat.print_board();
  //return best;
  std::vector<nng_move_t> moves = etat.get_all_moves();
  for(int i = 0; i < bound; i++){
    nng_t copy; copy.copy(etat);
    Noeud& suivant = selection(racine, copy);
    if(copy.score() == 100){
      cout << "YEEEEEEEEEEES" << "\n";
      suivant.nbWins++;
    }
    copy.playout();
    copy.print_board();
    //copy.print_board();
    if(copy.score() == 100){
      cout << "YEEEEEEEEEEES" << "\n";
      suivant.nbWins++;
      cout << suivant.nbVisits << "\n";
      return best;
    }
    cout << i << "\n";
    backPropagation(suivant);
    //if(i==20) return best;
=======
Noeud MCTS(int bound, Noeud &racine){
  Noeud best(racine.path);
  float max = 0;
  racine.etat.fill();
  racine.etat.print_board();
  std::vector<nng_move_t> moves = racine.etat.get_all_moves();
  for(int i = 0; i < bound; i++){
    Noeud suivant = selection(racine);
    Noeud suivant2 = expansion(suivant);
    suivant2.etat.playout();
    backPropagation(suivant2);
    float score = suivant2.etat.total_score / suivant2.etat.visited;
    if(score >= max){
      best = suivant2;
    }
    if(i == 10){ std::cout << racine.etat.visited << "\n"; break;}
>>>>>>> f3e9df7d43741b6227bbc8269e23432afaf4c099
  }
  for(auto m : racine.enfants){

    //float score = m.nbWins / m.nbVisits;

    if(10 >= max){
      max = 10;
      best = m;
    }

  }
  return best;
}


int main(int _ac, char** _av) {
<<<<<<< HEAD
  srand((time(NULL)));
  nng_move_t best;
  //methodeBase(1000);
  /*best = MonteCarlo(10000);
  cout << best.line << " && " << best.col << "\n";*/
  Noeud racine;
  nng_t nono; nono.load((char*)"problems/nonogram5x5_2_game.txt");
  /*nng_move_t move;
  move.line = 0; move.col = 1;
  nono.play(move);
  move.line = 1; move.col = 0;
  nono.play(move);*/
  //cout << nono.board_total_lines[0] << "\n";
  nono.print_board();
  //nono.fill();
  /*vector<nng_move_t> next = nono.get_all_moves();
  for(auto m : next){
    cout << m.line << " && " << m.col << "\n";
  }*/
  //racine.etat.print_board();
  Noeud tg;
  tg = MCTS(100, racine, nono);
=======

  //methodeBase();
  //MonteCarlo(10000);
  Noeud racine((char *)"problems/nonogram5x5_3_game.txt");
  MCTS(10, racine);

>>>>>>> f3e9df7d43741b6227bbc8269e23432afaf4c099
  return 0;
}
