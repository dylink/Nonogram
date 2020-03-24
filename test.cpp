#include <iostream>
#include <string.h>
#include <math.h>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <list>
#include <typeinfo>

#define WHITE 0
#define BLACK 1

using namespace std;

struct nng_move_t {
  int line, col;
};

class Nonogram {

public:
  vector<vector<int>> plateau, plateau_c_lines, plateau_c_cols, plateau_lines_id, plateau_cols_id,
  problem_c_lines, problem_c_cols, problem_display_lines, problem_display_cols;

  vector<int> problem_nb_lines, problem_nb_cols, problem_max_nbc_lines, problem_max_nbc_cols,
  plateau_nb_c_lines, plateau_nb_c_cols, plateau_max_nbc_lines, plateau_max_nbc_cols;

  int problem_nb_cases = 0, problem_maxval_lines = 0, problem_maxval_col = 0, nb_val_set = 0;


  void loadPlateau(string file){
    this->plateau.clear();
    string line;
    ifstream myfile (file);
    getline(myfile, line);
    int nbl, nbc, max_l, max_c;
    sscanf(line.c_str(), "%d %d %d %d", &nbl, &nbc, &max_l, &max_c);
    vector<int> vect(nbc, 0);
    vector<vector<int>> plateau(nbl, vect);
    this->problem_maxval_lines = max_l, this->problem_maxval_col = max_c;
    this->plateau = plateau;
    int mode = 0;
    if(!myfile.is_open()){
      cout << file << " : " << "This file does not exists or cannot be open.\n";
      exit(1);
    }
    vect.clear();
    vector<int> copy;
    while(getline(myfile, line)){
      if(line.compare("lines") == 0) mode = 1;
      else if(line.compare("cols") == 0) mode = 2;
      else{
        int nbval = ceil((float)line.size()/2);
        if(mode == 1){
          this->problem_nb_lines.push_back(nbval);
          for(int i = 0; i < line.size(); i++){
            if(line[i] != ' ') {
              int val = (int)line[i] - 48;
              vect.push_back(val);
              this->problem_nb_cases += val;
            }
          }
          copy = vect;
          if(vect.size() < this->problem_maxval_lines){
            int size = vect.size();
            for(int i = 0; i<this->problem_maxval_lines - size; i++){
              vect.insert(vect.begin(), 0);
            }
          }
          this->problem_display_lines.push_back(vect);
          this->problem_c_lines.push_back(copy);
          vect.clear();
        }
        else if(mode == 2){
          this->problem_nb_cols.push_back(nbval);
          for(int i = 0; i < line.size(); i++){
            if(line[i] != ' ') {
              int val = (int)line[i] - 48;
              vect.push_back(val);
            }
          }
          copy = vect;
          if(vect.size() < this->problem_maxval_col){
            int size = vect.size();
            for(int i = 0; i<this->problem_maxval_col - size; i++){
              vect.insert(vect.begin(), 0);
            }
          }
          this->problem_display_cols.push_back(vect);
          this->problem_c_cols.push_back(copy);
          vect.clear();
        }
      }
    }
    myfile.close();
    line.clear();
    set_max();
    vector<int> vect1(this->plateau[0].size(), 0);
    this->plateau_max_nbc_cols = vect1, this->plateau_nb_c_cols = vect1;
    vector<vector<int>> vect2(this->plateau.size(), vect1);
    vector<int> vect3(this->plateau.size(), 0);
    this->plateau_max_nbc_lines = vect3, this->plateau_nb_c_lines = vect3;
    vector<vector<int>> vect4(this->plateau.size(), vect3);
    this->plateau_c_lines = vect2;
    this->plateau_c_cols = vect4;
  }

  void affichePlateau(){
    printf("problem_nb_cases: %d\n", problem_nb_cases);
    printf("nb_val_set: %d\n\n", nb_val_set);
    for(int i = 0; i<this->problem_maxval_col; i++){
      for(int j = 0; j<this->problem_maxval_lines; j++){
        printf("  ");
      }
      for(int j = 0; j<this->plateau[0].size(); j++){
        if(!this->problem_display_cols[j][i]){
          printf("  ");
        }
        else {
          printf("%d ", this->problem_display_cols[j][i]);
        }
      }
      printf("\n");
    }
    for(int i = 0; i<this->plateau.size(); i++){
      for(int j = 0; j<this->problem_maxval_lines; j++){
        if(!this->problem_display_lines[i][j]){
          printf("  ");
        }
        else{
          printf("%d ", this->problem_display_lines[i][j]);
        }
      }
      for(int j = 0; j<this->plateau[i].size(); j++){
        switch(this->plateau[i][j]){
          case 0: printf("\033[38m▢ \033[0m"); break;
          case 1: printf("\033[38m▣ \033[0m"); break;
        }
      }
      printf("\n");
    }
  }

  void set_max() {
    vector<int> vect(this->plateau.size(), 0), vect2(this->plateau[0].size(), 0);
    this->problem_max_nbc_lines = vect;
    this->problem_max_nbc_cols = vect2;
    for(int i = 0; i < this->problem_nb_lines.size(); i++) {
      for(int j = 0; j < this->problem_c_lines[i].size(); j++) {
        if(this->problem_c_lines[i][j] > this->problem_max_nbc_lines[i])
          this->problem_max_nbc_lines[i] = this->problem_c_lines[i][j];
      }
    }
    for(int i = 0; i < this->problem_nb_cols.size(); i++) {
      for(int j = 0; j < this->problem_c_cols[i].size(); j++) {
        if(this->problem_c_cols[i][j] > this->problem_max_nbc_cols[i])
          this->problem_max_nbc_cols[i] = this->problem_c_cols[i][j];
      }
    }
  }

  void set_line_id(int _lid) {
    int curr_gid = 0, curr_val = this->plateau[_lid][0], best_lines_size = 0, curr_lines_size = 0;
    if(this->plateau[_lid][0] == BLACK) {
      best_lines_size = 1;
      curr_lines_size = 1;
    }
    for(int i = 1; i < this->plateau[0].size(); i++) {
      if(this->plateau[_lid][i] == curr_val) {
        curr_lines_size ++;
      }
      else {
        if(this->plateau[_lid][i] == WHITE) {
          if(best_lines_size < curr_lines_size){
            best_lines_size = curr_lines_size;
          }
          this->plateau_c_lines[_lid][curr_gid] = curr_lines_size;
          curr_gid++;
          this->plateau_nb_c_lines[_lid] = curr_gid;
        }
        curr_val = this->plateau[_lid][i];
        curr_lines_size = 1;
      }
    }
    if(this->plateau[_lid][this->plateau[0].size()-1] == BLACK) {
      if(best_lines_size < curr_lines_size)
        best_lines_size = curr_lines_size;
      this->plateau_c_lines[_lid][curr_gid] = curr_lines_size;
      this->plateau_nb_c_lines[_lid] = curr_gid+1;
    }
    this->plateau_max_nbc_lines[_lid] = best_lines_size;
  }

  void set_col_id(int _cid) {
    int curr_gid = 0, curr_val = this->plateau[0][_cid], best_cols_size = 0, curr_cols_size = 0;
    if(this->plateau[0][_cid] == BLACK) {
      best_cols_size = 1;
      curr_cols_size = 1;
    }
    for(int i = 1; i < this->plateau.size(); i++) {
      if(this->plateau[i][_cid] == curr_val) {
        curr_cols_size ++;
      }
      else {
        if(this->plateau[i][_cid] == WHITE) {
          if(best_cols_size < curr_cols_size){
            best_cols_size = curr_cols_size;
          }
          this->plateau_c_cols[_cid][curr_gid] = curr_cols_size;
          curr_gid++;
          this->plateau_nb_c_cols[_cid] = curr_gid;
        }
        curr_val = this->plateau[i][_cid];
        curr_cols_size = 1;
      }
    }
    if(this->plateau[this->plateau.size()-1][_cid] == BLACK) {
      if(best_cols_size < curr_cols_size)
        best_cols_size = curr_cols_size;
      this->plateau_c_cols[_cid][curr_gid] = curr_cols_size;
      this->plateau_nb_c_cols[_cid] = curr_gid+1;
    }
    this->plateau_max_nbc_cols[_cid] = best_cols_size;
  }

  void play(nng_move_t _m) {
    if(this->plateau[_m.line][_m.col] != BLACK){
      this->plateau[_m.line][_m.col] = BLACK;
      set_line_id(_m.line);
      set_col_id(_m.col);
      this->nb_val_set++;
    }
  }

  nng_move_t get_rand_move() {
    nng_move_t ret;
    int nbc = this->plateau[0].size(), nbl = this->plateau.size();
    int r = ((int)rand())%((nbl*nbc)-this->nb_val_set);
    //cout << "Nb_Val " << r << "\n";
    for(int i = 0; i < nbl; i++) {
      for(int j = 0; j < nbc; j++) {
        if(r == 0 && this->plateau[i][j] == WHITE) {
          ret.line = i; ret.col = j; return ret;
        }
        if(this->plateau[i][j] == WHITE) r--;
      }
    }
    printf("ERROR : NO RAND MOVE\n"); exit(0);
    return ret;
  }

  vector<nng_move_t> get_all_moves() {
    vector<nng_move_t> ret;
    int nbc = this->plateau[0].size(), nbl = this->plateau.size();
    for(int i = 0; i < nbl; i++) {
      if(this->plateau_max_nbc_lines[i] == this->problem_max_nbc_lines[i]) continue;
      for(int j = 0; j < nbc; j++) {
        if(this->plateau_max_nbc_cols[j] == this->problem_max_nbc_cols[j]) continue;
        if(this->plateau[i][j] == WHITE) {
          nng_move_t mm;
          mm.line = i; mm.col = j;
          ret.push_back(mm);
        }
      }
    }
    return ret;
  }

  bool terminal() {
    // all are checked
    int nbc = this->plateau[0].size(), nbl = this->plateau.size();
    if(this->nb_val_set == nbl*nbc) return true;
    // one is bigger => true
    for(int i = 0; i < nbc; i++)
      if(this->plateau_max_nbc_lines[i] > this->problem_max_nbc_lines[i]) return true;
    for(int i = 0; i < nbl; i++)
      if(this->plateau_max_nbc_cols[i] > this->problem_max_nbc_cols[i]) return true;
    // one is not equal => false;
    for(int i = 0; i < nbc; i++)
      if(this->plateau_max_nbc_lines[i] != this->problem_max_nbc_lines[i]) return false;
    for(int i = 0; i < nbl; i++)
      if(this->plateau_max_nbc_cols[i] != this->problem_max_nbc_cols[i]) return false;
    // sizes of groups are different => false
    for(int i = 0; i < nbl; i++) {
      if(this->plateau_nb_c_lines[i] != this->problem_nb_lines[i]) return false;
      for(int j = 0; j < this->problem_nb_lines[i]; j++)
        if(this->plateau_c_lines[i][j] != this->problem_c_lines[i][j]) return false;
    }
    for(int i = 0; i < nbc; i++) {
      if(this->plateau_nb_c_cols[i] != this->problem_nb_cols[i]) return false;
      for(int j = 0; j < this->problem_nb_cols[i]; j++)
        if(this->plateau_c_cols[i][j] != this->problem_c_cols[i][j]) return false;
    }
    // all are equals... so potentially the solution
    return true;
  }

  int score() {
    int nbc = this->plateau[0].size(), nbl = this->plateau.size();
    for(int i = 0; i < nbl; i++) {
      if(this->plateau_nb_c_lines[i] != this->problem_nb_lines[i]) return 0;
      for(int j = 0; j < this->problem_nb_lines[i]; j++)
        if(this->plateau_c_lines[i][j] != this->problem_c_lines[i][j]) return 0;
    }
    for(int i = 0; i < nbc; i++) {
      if(this->plateau_nb_c_cols[i] != this->problem_nb_cols[i]) return 0;
      for(int j = 0; j < this->problem_nb_cols[i]; j++)
        if(this->plateau_c_cols[i][j] != this->problem_c_cols[i][j]) return 0;
    }
    return 100;
  }

  void playout() {
    while( ! terminal()) {
      nng_move_t m = get_rand_move();
      //move = m;
      //cout << m.line << " && " << m.col << "\n";
      play(m);
    }
    /*total_score = score();
    visited++;*/
  }

  void fill(){
    int nbc = this->plateau[0].size(), nbl = this->plateau.size();
    nng_move_t m;
    for(int i = 0; i < nbl; i++){
      if(this->problem_max_nbc_lines[i] == nbl){
        for(int j = 0; j < nbc; j++){
          m.line = i; m.col = j;
          play(m);
        }
        continue;
      }
      if(this->problem_nb_lines[i] == 2){
        if(this->problem_c_lines[i][0] + this->problem_c_lines[i][1] == nbl-1){
          int step = 0;
          for(int j = 0; j < 2; j++){
            for(int x = 0; x < this->problem_c_lines[i][j]; x++, step++){
              m.line = i; m.col = step;
              play(m);
            }
            step++;
          }
          continue;
        }
      }
      if(this->problem_max_nbc_lines[i] > (nbc/2)){
        for(int j = nbc - this->problem_max_nbc_lines[i]; j < nbc - (nbc - this->problem_max_nbc_lines[i]); j++){
          m.line = i; m.col = j;
          play(m);
        }
      }
    }
    for(int i = 0; i < nbc; i++){
      if(this->problem_max_nbc_cols[i] == nbc){
        for(int j = 0; j<nbl; j++){
          m.line = j; m.col = i;
          play(m);
        }
        continue;
      }
      if(this->problem_nb_cols[i] == 2){
        if(this->problem_c_cols[i][0] + this->problem_c_cols[i][1] == nbc-1){
          int step = 0;
          for(int j = 0; j < 2; j++){
            for(int x = 0; x < this->problem_c_cols[i][j]; x++, step++){
              m.line = step; m.col = i;
              play(m);
            }
            step++;
          }
          continue;
        }
      }
      if(this->problem_max_nbc_cols[i] > (nbl/2)){
        for(int j = nbl - this->problem_max_nbc_cols[i]; j < nbl - (nbl - this->problem_max_nbc_cols[i]); j++){
          m.line = j; m.col = i;
          play(m);
        }
      }
    }
  }

  void copy(Nonogram &nono){
    memcpy(this, &nono, sizeof(Nonogram));
  }


  nng_move_t MonteCarlo(int bound, string file){
    nng_move_t best, best2;
    loadPlateau(file);
    int max = 0, wi = 0;
    std::vector<nng_move_t> moves = get_all_moves();
    fill();
    affichePlateau();
    Nonogram nCopy;
    nCopy = *this;
    for(auto m : moves){
      nCopy.play(m);
      wi = 0;
      for(int i = 0; i < bound; i++) {
        Nonogram nCopy2;
        nCopy2 = nCopy;
        nCopy2.playout();
        if(nCopy2.score() == 100) {
          nCopy2.affichePlateau();
          wi++;
        }
      }
      if(wi>max){
        //std::cout << wi << std::endl;
        best =m;
        max = wi;
      }
      nCopy = *this;
    }
    //std::cout << best.line << " && " << best.col << "\n";
    return best;
  }


};


int main(){
  srand(1);
  Nonogram b;
  nng_move_t best;
  best = b.MonteCarlo(10000, "problems/nonogram5x5_2_game.txt");
  cout << best.line << " && " << best.col << "\n";

  return 0;
}
