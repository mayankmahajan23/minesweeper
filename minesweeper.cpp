#include <iostream>
#include <ctime>
#include <iomanip>
#include <conio.h>
#include <cstdlib>
using namespace std;

struct double_node {
  int node[2];
};
  template <typename t>
class array_two {
public:
  t **a;
  int n, m;
  array_two () {
    a = NULL;
    n = 0;
    m = 0;
  }
  array_two (int x, int y) {
    a = give_memory(x,y);
    n = x;
    m = y;
  }
  t **give_memory (int x, int y) {
    t **temp = new t*[x];
    for(int i = 0; i < x; i++)
      temp[i] = new t[y];
    return temp;
  }
  void display (int start1, int end1, int start2, int end2, int gap) {
    for (int i = start1; i < end1; i++) {
      for (int j = start2; j < end2; j++)
        cout << setw(gap) << a[i][j];
      cout << setw(0) << endl;
    }
    cout<< endl;
  }
  void displayDouble (int start1, int end1, int start2, int end2) {
    for (int i = start1; i < end1; i++) {
      for (int j = start2; j < end2; j++)
        cout<< a[i][j].node[0] << setw(3) << a[i][j].node[1] << setw(5);
      cout<< setw(0) << endl << endl;
    }
    cout<< endl;
  }
  void delete_array () {
    for (int i = 0; i < n; i++)
      delete a[i];
    delete a;
  }
    template <typename datatype>
  void setEdges (datatype x) {
    for (int i = 0; i < min(n,m); i++) {
      a[0][i] = x;
      a[n-1][i] = x;
      a[i][0] = x;
      a[i][m-1] = x;
    }
    if ( n < m )
      for (int i = n; i < m; i++) {
        a[0][i] = x;
        a[n-1][i] = x;
      }
    else
      for (int i = m; i < n; i++) {
        a[i][0] = x;
        a[m-1][0] = x;
      }
  }
    template <typename datatype>
  void setMatrix (datatype value, int start1, int end1, int start2, int end2) {
    for (int i = start1; i < end1; i++)
      for (int j = start2; j < end2; j++)
        a[i][j] = value;
    }
    template <typename datatype>
  void setMatrixDouble (datatype value, int start1, int end1, int start2, int end2) {
    for (int i = start1; i < end1; i++)
      for (int j = start2; j < end2; j++) {
        a[i][j].node[0] = value;
        a[i][j].node[1] = value;
      }
  }
};

bool binary_search(int *A, int n, int B) {
    int start = 0, end = n - 1, mid;

    while (start <= end) {
        mid = (start + end)/2;

        if (A[mid] == B)
            return true;
        else if (A[mid] > B)
            end = mid - 1;
        else
            start = mid + 1;
    }
    return false; //not found
}

#define mine -9;
#define free 0;
#define height 5;
#define width 5;
#define total_mines 3; //should be less than to (height * width)

class GAME {
  array_two <int> *land;
  array_two <char> *ans;
  int n, m, N, M, t, minesMarked, xCord, yCord;

  void new_land () {
    //setting inner matrix to zero
    land->setMatrix <int> (0, 1, n+1, 1, m+1);

    //planting mines
    int k = 0, total = n*m, list[t], length = 0;
    for (int i = 0; i < t; i++) {
      k = rand() % (total); //note : this can give same number even if rand doesn't repeat

      //dbt: is the following necessary (how to uniquely generate the numbers without checking)
      while (true) {//check if number already used
        if ( binary_search(list, length, k) )
          k = rand() % (total);
        else {
          list[length] = k;
          length++;
          break;
        }
      }

      //working logic
        land->a[ k / m + 1 ][ k % m + 1 ] = mine;
      //dbt - (alternate code) this should work for numbers n and m iff their lcm is n * m, but doesn't
        //land->a[(k % n) + 1][(k % m) + 1] = -9; //panga / dbt - unpredictable behaviour!
    }

    //marking numbers
    for (int i = 1; i < n+1; i++)
      for (int j = 1; j < m+1; j++)
        if ( land->a[i][j] < 0 ) {
          land->a[i-1][j-1] += 1;
          land->a[i-1][j] += 1;
          land->a[i-1][j+1] += 1;
          land->a[i][j-1] += 1;
          land->a[i][j+1] += 1;
          land->a[i+1][j-1] += 1;
          land->a[i+1][j] += 1;
          land->a[i+1][j+1] += 1;
        }

    //setting the boundary elements zero (that two extra rows and columns)
    land->setEdges <int> (0);
  }
  void displayLand () {
    cout<< "the land : " << endl;
    land->display(1, n+1, 1, m+1, 2);
  }
  int getRandomPosition () {
    return rand() % (n*m);
  }
  bool open_up (int x, int y) {//opens the blocks
    if ( ans->a[x][y] == '&' ) {
      if ( land->a[x][y] < 0 ) {
        cout<< "mine hit. you died" << endl;
        displayLand();
        return 0;
      }
      else if ( land->a[x][y] > 0 )
        ans->a[x][y] = land->a[x][y] + 48;
      else {
        ans->a[x][y] = '0';
        for (int i = -1; i <= 1; i++)
          for (int j = -1; j <= 1; j++)
            open_up(x+i, y+j);
      }
    }
    return 1; //success
  }
  void initializeGame (int x, int y, int T) {
    //initializing stuff
    srand( time(NULL) ); //because time(NULL) varies everytime

    minesMarked = 0;

    land = new array_two <int> (N, M);
    new_land();
    land->display(1, n+1, 1, m+1, 3);

    ans = new array_two <char> (N,M);
    ans->setMatrix <char> ('&', 1, n+1, 1, m+1);
    ans->setEdges <char> ('0');
  }
  bool checkAnswer () {//returns 1 if complete
    if (minesMarked == t) {//checks completion
      int mineCount = 0;
      for (int x = 1; x < n+1; x++)
        for (int y = 1; y < m+1; y++)
          if (ans->a[x][y] == '*' && land->a[x][y] < 0)
            mineCount++;
      if (mineCount == t) {
        cout<< "minefield solved. congratulations." << endl;
        displayLand();
        return true;
      }
      else
        cout<< "marks not correct. try again" << endl;
    }
    else if (minesMarked > t) {
      cout<< "marked mines capacity increased. unmark some." << endl;
    }
    //else (minesMarked < t) return false; //not needed

    return false;
  }
  bool makeMove (int x, int y, int move) {
    switch (move) {
      case 0: //hit
        if ( !open_up(x,y) )
          return 0; //got mine
        break;
      case 1: //mark
        ans->a[x][y] = '*';
        minesMarked++;
        break;
      case 2: //unmark
        ans->a[x][y] = '&';
        minesMarked--;
        break;
    }
    displayAnswer();
    return 1;
  }

public:
  GAME (int x, int y, int T) {
    t = T;
    n = x;
    N = x + 2;
    m = y;
    M = y + 2;

    initializeGame(x,y,T);
  }
  bool convertMove (int x, int y, int move) { //returns 0 if hit a mine
    return makeMove(++x, ++y, move);
  }
  void play () {
    int move;
    while (true) {
      cout<< "enter play - row, column, move: ";
      cin>> xCord >> yCord >> move;
      cout<< n << m << xCord << yCord << move << endl;
      if (xCord < n && yCord < m && xCord >= 0 && yCord >= 0 && (move == 0 || move == 1 || move == 2)) {
        if ( !convertMove(xCord, yCord, move) )
          return;
        if ( checkAnswer() )
          break;
      }
      else
        cout<< "wrong inputs, try again.\n";
    }
  }
  void displayAnswer () {
    cout<< "your ans : " << endl;
    ans->display(1, n+1, 1, m+1, 2);
  }
};

int main () {
  GAME g(5,6,3);
  g.play();

  return 0;
}
