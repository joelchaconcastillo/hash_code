#include <bits/stdc++.h>
using namespace std;
int B, L, D; //number of books, librearies and days....
vector<int> scores_books, times_l, shipping_l;
vector<vector<int> > books_in_library;

int main()
{
  scanf("%d %d %d\n", &B, &L, &D);
  scores_books.resize(B);
  for(int i = 0; i < B; i++) scanf("%d ", &scores_books[i]);
  books_in_library.resize(L);
  times_l.resize(L);
  shipping_l.resize(L);
  for(int i = 0; i < L; i++)
  { 
    int size_library;
    scanf("%d %d %d", &size_library, &times_l[i], &shipping_l[i]); 
    books_in_library[i].resize(size_library);
    for(int j = 0; j < books_in_library[i].size(); j++)
    {
       scanf("%d ", &books_in_library[i][j]);
    }
  }

 
}
