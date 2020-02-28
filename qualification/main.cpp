#include <bits/stdc++.h>
using namespace std;
int B, L, D; //number of books, librearies and days....
vector<int> score_books, signup_time_l, shipping_l; // score of each book, number of days it takes to finish the library to 
vector<vector<int> > books_in_library;
vector<int> counter_library;

///the representation is by index of libraries and a mask indicating which book is active in each library..
struct x_solution
{
   vector<int> x; //indexes of the libraries...
   vector< vector<int> > B_L;
};

struct x_solution iterated_local_search(struct x_solution initial);
void print_solution(struct x_solution &sol);
long eval(struct x_solution &Sol);

int main()
{
  struct x_solution sol;
  srand(1);
  scanf("%d %d %d\n", &B, &L, &D);
  score_books.resize(B);
  counter_library.resize(B, 0);
  for(int i = 0; i < B; i++) scanf("%d ", &score_books[i]);
  books_in_library.resize(L);
  sol.B_L.resize(L); 
  sol.x.resize(L);

  signup_time_l.resize(L);
  shipping_l.resize(L);
  for(int i = 0; i < L; i++)
  { 
    int size_library;
    scanf("%d %d %d\n", &size_library, &signup_time_l[i], &shipping_l[i]); 
    books_in_library[i].resize(size_library);
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
//  cout << eval(sol)<<endl;
 // print_solution(sol);
  struct x_solution improved = iterated_local_search(sol);
//  printf("%ld\n\n", eval(improved));
  print_solution(improved);

  return 0;
}
void print_solution(struct x_solution &sol)
{
  //printf("%d\n", (int)sol.x.size());
  fprintf(stderr, "%d\n", (int)sol.x.size());
  for(int i = 0; i < sol.x.size(); i++)
  {
     //printf("%d %d\n", sol.x[i], (int)sol.B_L[sol.x[i]].size());
     fprintf(stderr, "%d %d\n", sol.x[i], (int)sol.B_L[sol.x[i]].size());
     for(int j = 0; j < sol.B_L[sol.x[i]].size(); j++)
     {
	fprintf(stderr, "%d ", sol.B_L[sol.x[i]][j]);
//       printf("%d ", sol.B_L[sol.x[i]][j]);
     }
	fprintf(stderr, "\n");
//     printf("\n");
  }
}
struct x_solution iterated_local_search(struct x_solution best)
{
//	return best;
  int cont = 50000;
  long f_best= eval(best);

  while(cont--)
  {
    struct x_solution current = best;
    int a = rand()%L;
    int b = rand()%L;
    iter_swap(current.x.begin()+a, current.x.begin()+b);
    random_shuffle(current.x.begin(), current.x.end());
    long f_current = eval(current);
    if( f_current > f_best)
    {
	f_best= f_current;
	best = current;
	fprintf(stdout, "%ld %d\n", f_best, cont);
        print_solution(best);
	cont *=2;
    }
  }
 return best;
}
long eval(struct x_solution &Sol)
{
 long total_score = 0;
  vector<int> cont_book_lib(L, 0);
  vector<bool> checked(B, false);
  int cont = 0;
  int current_time = signup_time_l[Sol.x[cont]];
  for(int d = current_time; d <D; d++)
  {
     if(d >= current_time )
       current_time +=signup_time_l[Sol.x[++cont]];
     for(int l = 0; l < cont; l++)//can be improved..
     {
	  int id_lib = Sol.x[l];
	  for(int t = 0; t <  shipping_l[id_lib]; t++)//required..
	  {
            if( cont_book_lib[id_lib] < books_in_library[id_lib].size())
            {
            	      int id_book = books_in_library[id_lib][cont_book_lib[id_lib]++];
            		 if( checked[id_book]) continue;
                             total_score += score_books[id_book];
            		 checked[id_book]=true;
            }
	  }
     }
  }
 return total_score;
}
