#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <string>
#include <sys/time.h>
#include <limits>
#include <math.h>
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

float ucb(int wi, int ni, int n){
  if(!n) return std::numeric_limits<float>::infinity();
  return (wi/ni)+1.41*sqrt(log(n)/n);
}

/*Noeud selection(Noeud &racine){
  Noeud noeud = racine;
  int n = noeud.etat.visited;
  if(!n) return noeud;
  static float max = -1;
  Noeud best(noeud.path);
  std::vector<nng_move_t> moves = noeud.etat.get_all_moves();
  int i = 0;
  for(auto m : moves){
    Noeud enfants = noeud;
    enfants.etat.play(m);
    int ni = enfants.etat.visited;
    if(!ni) return noeud;
    float eval = ucb(enfants.etat.total_score, ni, noeud.etat.visited);
    if(eval > max){
      max = eval;
      best = enfants;
    }

  }
  return best;
}

void backPropagation(Noeud &noeud){
  Noeud *current = &noeud;
  while(current != NULL){
    if(current->parent != NULL){
      current->etat.score();
      current->parent->etat.total_score += current->etat.total_score;
      current->parent->etat.visited += current->etat.visited;
    }
    current = current->parent;
  }
}

Noeud expansion(Noeud &noeud){
  std::vector<nng_move_t> moves = noeud.etat.get_all_moves();
  Noeud copy(noeud.path);
  copy.etat.copy(noeud.etat);
  Noeud copy2 = copy;
  if(!noeud.etat.visited) {
    copy.etat.play(copy.etat.get_rand_move());
    copy.parent = &noeud;
    noeud.enfants.push_back(copy);
    copy = copy2;
  }
  for(auto m : noeud.enfants){
    if(!m.etat.visited) return m;
  }
  return noeud;
}

nng_move_t MCTS(int bound){
  srand(1);
  Arbre arbre;
  nng_move_t best;
  int max = 0;
  Noeud racine = arbre.racine;
  racine.etat.fill();
  racine.etat.print_board();
  std::vector<nng_move_t> moves = racine.etat.get_all_moves();
  std::vector<std::pair<float, nng_move_t>> top;
  for(int i = 0; i < bound; i++){
    Noeud suivant = selection(racine);
    expansion(suivant);
    suivant.etat.playout();
    best = suivant.etat.move;
    backPropagation(suivant);
    //std::cout << suivant.etat.visited << "\n";
    //suivant.etat.print_board();
    if(suivant.etat.total_score) std::cout << suivant.etat.total_score << "\n";
    float score = suivant.etat.total_score / suivant.etat.visited;
    //std::cout << score << "\n";
    top.push_back(std::pair(score, best));
  }
  return best;
}*/

Noeud selection(Noeud &racine){
  Noeud noeud = racine;
  static float max = -1;
  Noeud best(noeud.path);
  std::vector<nng_move_t> moves = noeud.etat.get_all_moves();
  for(auto m : moves){
    Noeud enfants = noeud;
    enfants.etat.play(m);
    int ni = enfants.etat.visited;
    if(!ni) return enfants;
    float eval = ucb(enfants.etat.total_score, ni, noeud.etat.visited);
    if(eval > max){
      max = eval;
      best = enfants;
    }
  }
  return best;
}

Noeud expansion(Noeud &noeud){
  std::vector<nng_move_t> moves = noeud.etat.get_all_moves();
  Noeud copy(noeud.path);
  copy.etat.copy(noeud.etat);
  Noeud copy2 = copy;
  if(!noeud.etat.visited) {
    copy.etat.play(copy.etat.get_rand_move());
    copy.parent = &noeud;
    noeud.enfants.push_back(copy);
    copy = copy2;
  }
  for(auto m : noeud.enfants){
    if(!m.etat.visited) return m;
  }
  return noeud;
}

void backPropagation(Noeud &noeud){
  Noeud *current = &noeud;
  while(current != NULL){
    if(current->parent != NULL){
      current->parent->etat.total_score += current->etat.total_score;
      current->parent->etat.visited += current->etat.visited;
    }
    current = current->parent;
  }
}

nng_move_t MCTS(int bound){
  srand(1);
  Arbre arbre;
  nng_move_t best;
  int max = 0;
  Noeud racine = arbre.racine;
  racine.etat.fill();
  racine.etat.print_board();
  std::vector<nng_move_t> moves = racine.etat.get_all_moves();
  std::vector<std::pair<float, nng_move_t>> top;
  for(int i = 0; i < bound; i++){
    Noeud suivant = selection(racine);
    expansion(suivant);
    suivant.etat.playout();
    suivant.etat.score();
    best = suivant.etat.move;
    backPropagation(suivant);
    if(suivant.etat.total_score) std::cout << suivant.etat.total_score << "\n";
    float score = suivant.etat.total_score / suivant.etat.visited;
    //std::cout << score << "\n";
    top.push_back(std::pair(score, best));
  }
  return best;
}

nng_move_t MonteCarlo(int bound){
  srand(4);
  nng_t N;
  nng_move_t best, best2;
  N.load((char*)"problems/nonogram5x5_3_game.txt");
  N.print_problem_info();
  int max = 0, wi = 0;
  N.print_board();
  N.fill();
  std::vector<nng_move_t> moves = N.get_all_moves();
  N.print_board();
  for(auto m : moves){
    nng_t NN;
    NN.copy(N);
    NN.play(m);
    wi = 0;
    for(int i = 0; i < bound; i++) {
      NN.copy(N);
      NN.playout();
      if(NN.score() == 100) {
        NN.print_board();
        wi++;
      }
    }
    if(wi>max){
      //std::cout << wi << std::endl;
      best =m;
      max = wi;
    }
  }
  //std::cout << best.line << " && " << best.col << "\n";
  return best;
}


int main(int _ac, char** _av) {

  //methodeBase();
  //MonteCarlo(10000);
  MCTS(100000);

  return 0;
}
