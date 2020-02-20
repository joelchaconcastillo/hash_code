#include <bits/stdc++.h>
using namespace std;
int B, L, D; //number of books, librearies and days....
vector<int> scores_books, times_l, shipping_l;
vector<vector<int> > books_in_library;
vector<int> counter_library;

///the representation is by index of libraries and a mask indicating which book is active in each library..
//struct x_solution
//{
//
//};
vector<int> x_solution; //indexes of the libraries...
vector< vector<int> > mask_books_library;
vector<int> size_scanning;
long eval()
{

}

int main()
{
  scanf("%d %d %d\n", &B, &L, &D);
  scores_books.resize(B);
  counter_library.resize(B, 0);
  for(int i = 0; i < B; i++) scanf("%d ", &scores_books[i]);
  books_in_library.resize(L);
  mask_books_library.resize(L); 
  size_scanning.resize(L);
  times_l.resize(L);
  shipping_l.resize(L);
  for(int i = 0; i < L; i++)
  { 
    int size_library;
    scanf("%d %d %d\n", &size_library, &times_l[i], &shipping_l[i]); 
    books_in_library[i].resize(size_library);
    mask_books_library[i].resize(size_library, 1); //all the book are active....
    size_scanning[i] = size_library;
    for(int j = 0; j < books_in_library[i].size(); j++)
    {
       scanf("%d ", &books_in_library[i][j]);
       counter_library[books_in_library[i][j]]++;
    }
  }

  //generating a random solution...
  vector<int> x_solution(L);
  for(int i = 0; i < L; i++)
    x_solution[i] = i; 
  random_shuffle(x_solution.begin(), x_solution.end());
  
  printf("%d\n", x_solution.size());
  for(int i = 0; i < x_solution.size(); i++)
  {
     printf("%d %d\n", x_solution[i], size_scanning[x_solution[i]]);
     for(int j = 0; j < mask_books_library[x_solution[i]].size(); j++)
     {
       if(mask_books_library[x_solution[i]][j])
       printf("%d ",books_in_library[x_solution[i]][j]);
     }
     printf("\n");
  }
  return 0;
}
