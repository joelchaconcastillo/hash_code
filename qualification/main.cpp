#include <bits/stdc++.h>
using namespace std;
int B, L, D; //number of books, librearies and days....
vector<int> scores_books, times_l, shipping_l;
vector<vector<int> > books_in_library;
vector<int> counter_library;

///the representation is by index of libraries and a mask indicating which book is active in each library..
struct x_solution
{
   vector<int> x; //indexes of the libraries...
   vector< vector<int> > B_L;
};

void print_solution(struct x_solution &sol);
long eval(struct x_solution &Sol)
{
 long total_score = 0;  
  //get the start processing of each library.....
  vector<int> start(L);
//  for(int i = 0; i start.size(); i++)
//  {
//     c_time += times_l[i]
//     start[i] = c_time;
//  }

  int cont = 0;
  vector<int> c_lib(Sol.x[cont]), c_book_lib(L);
  int current_day = shipping_l[cont]+1;

  while(current_day < D)
  {
     for(int t = 0; t < shipping_l[cont]; t++)
     {
	current_day++;
        for(int l = 0; l < c_lib.size(); l++)
        { 
           for(int i = 0; i < times_l[c_lib[l]];i++)
           {
              total_score += scores_books[ books_in_library[c_lib[l]][ c_book_lib[c_lib[l]] ]];
              c_book_lib[c_lib[l]]++;
           }
        }
     }
     //add library...
     c_lib.push_back(Sol.x[cont++]);
  }
 return total_score;
}

int main()
{
  struct x_solution sol;
  srand(1);
  scanf("%d %d %d\n", &B, &L, &D);
  scores_books.resize(B);
  counter_library.resize(B, 0);
  for(int i = 0; i < B; i++) scanf("%d ", &scores_books[i]);
  books_in_library.resize(L);
  sol.B_L.resize(L); 
  sol.x.resize(L);

  times_l.resize(L);
  shipping_l.resize(L);
  for(int i = 0; i < L; i++)
  { 
    int size_library;
    scanf("%d %d %d\n", &size_library, &times_l[i], &shipping_l[i]); 
    //cout << size_library<< " " << times_l[i] << " " <<shipping_l[i]<<endl;
    books_in_library[i].resize(size_library);
    sol.B_L[i].resize(size_library, 1); //all the book are active....
    for(int j = 0; j < books_in_library[i].size(); j++)
    {
       scanf("%d ", &books_in_library[i][j]);
       sol.B_L[i].push_back(books_in_library[i][j]);
       counter_library[books_in_library[i][j]]++;
    }
  }

  //generating a random solution...
  for(int i = 0; i < L; i++)
    sol.x[i] = i; 
  random_shuffle(sol.x.begin(), sol.x.end());

  printf("%d\n\n", eval(sol));
  print_solution(sol);

  return 0;
}
void print_solution(struct x_solution &sol)
{
  printf("%d\n", sol.x.size());
  for(int i = 0; i < sol.x.size(); i++)
  {
     printf("%d %d\n", sol.x[i], sol.B_L[sol.x[i]].size());
     for(int j = 0; j < sol.B_L[sol.x[i]].size(); j++)
     {
       printf("%d ", sol.B_L[sol.x[i]][j]);
     }
     printf("\n");
  }
}
