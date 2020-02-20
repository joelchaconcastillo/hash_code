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
struct x_solution iterated_local_search(struct x_solution initial);
void print_solution(struct x_solution &sol);
long eval(struct x_solution &Sol)
{
 long total_score = 0;  
  vector<int> start(L), cont_book_lib(L, 0);
  vector<bool> checked(B, false);
  start[0] = times_l[Sol.x[0]];
  for(int i = 1; i < L; i++) start[i] = start[i-1]+times_l[Sol.x[i]];
  for(int d = 0; d <D; d++)
  {
     for(int l = 0; l < L; l++)
     {
	int id_lib = Sol.x[l];
	if( d >= start[l] )
	{
	  for(int t = 0; t <  shipping_l[id_lib]; t++)
	  {
            if( cont_book_lib[id_lib] < books_in_library[id_lib].size())
            {
            	      int id_book = books_in_library[id_lib][cont_book_lib[id_lib]++];
            		 if( checked[id_book]) continue;
                             total_score += scores_books[id_book];
            		 checked[id_book]=true;
            }
	  }
	}
     }
  }
//  int cont = 0;
//  vector<int> current_lib, cont_book_lib(L,0);
//  current_lib.push_back(Sol.x[0]);
//  int current_day = shipping_l[0];
//  vector<bool> checked(B, false);
//  while(current_day < D)
//  {
//       cont++;
//     for(int t = 0; t < shipping_l[cont]; t++)
//     {
//        for(int l = 0; l < current_lib.size(); l++)
//        { 
//	   int id_lib = current_lib[l];
//           for(int i = 0; i < times_l[id_lib];i++)
//           {
//               if( cont_book_lib[id_lib] < books_in_library[id_lib].size())
//               {
//	         int id_book = books_in_library[id_lib][cont_book_lib[id_lib]++];
//		 if( checked[id_book]) continue;
//	         //if has not been processed....
//                 total_score += scores_books[id_book];
//		 checked[id_book]=true;
//		}
//           }
//        }
//	if(current_day >= D) break;
//	current_day++;
//	cout  << current_day<< " .. "<<D<< " ";
//     }
     //add library...
//     if(cont >= Sol.x.size()) break;
     //current_lib.push_back(Sol.x[cont]);
  //}
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
//    sol.B_L[i].resize(size_library, 1); //all the book are active....
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
  struct x_solution improved = iterated_local_search(sol);
//  printf("%d\n\n", eval(improved));
  print_solution(improved);

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

struct x_solution iterated_local_search(struct x_solution best)
{
  int cont = 500;
  long f_best= eval(best);

  while(cont--)
  {
    struct x_solution current = best;
    random_shuffle(current.x.begin(), current.x.end());
    long f_current = eval(current);
    if( f_current > f_best)
    {
	f_best= f_current;
	best = current;
    }
  }
 return best;
}
