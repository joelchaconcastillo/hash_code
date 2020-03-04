#include <bits/stdc++.h>
using namespace std;

int B, L, D; //number of books, librearies and days....
vector<int> score_books, signup_time_l, shipping_l; // score of each book, number of days it takes to finish the library to 
vector<vector<int> > books_in_library;
vector<int> counter_books;

///the representation is by index of libraries and a mask indicating which book is active in each library..
struct solution
{
   vector< pair< int, vector<int> > > x; // the first is library ids, second id of libraries..
};

struct solution iterated_local_search(struct solution initial);
void print_solution(struct solution &sol);
long naive_eval(struct solution &Sol);
void sort_by_signup_time(struct solution &sol);
void unique_books(struct solution &sol);
solution hyper_heuristic();
void load_data();
void initialization(solution &s);
int main()
{
  srand(1);
  load_data();
  solution sol = hyper_heuristic();

//for(int i  = 0;i < B; i++) if(counter_books[i]) cout << counter_books[i]<< endl;
//for(int i = 0; i <L; i++) cout << signup_time_l[i]<<endl;

  //generating a random solution...
//  for(int i = 0; i < L; i++)
//    sol.x[i] = i; 
//  random_shuffle(sol.x.begin(), sol.x.end());
//  cout << naive_eval(sol)<<endl;
//  print_solution(sol);
//  struct solution improved = iterated_local_search(sol);
//  printf("%ld\n\n", eval(improved));
//  print_solution(improved);
  return 0;
}
void print_solution(struct solution &sol)
{
  //printf("%d\n", (int)sol.x.size());
  fprintf(stderr, "%d\n", (int)sol.x.size());
  for(int i = 0; i < (int)sol.x.size(); i++)
  {
     //printf("%d %d\n", sol.x[i], (int)sol.B_L[sol.x[i]].size());
     fprintf(stderr, "%d %d\n", sol.x[i].first, (int)sol.x[i].second.size());
     for(int j = 0; j < sol.x[i].second.size(); j++)
     {
	fprintf(stderr, "%d ", sol.x[i].second[j]);
//       printf("%d ", sol.B_L[sol.x[i]][j]);
     }
	fprintf(stderr, "\n");
//     printf("\n");
  }
}
///struct solution iterated_local_search(struct solution best)
///{
/////	return best;
///  int cont = 50000;
///  long f_best= naive_eval(best);
///
///  while(cont--)
///  {
///    struct solution current = best;
///    int a = rand()%L;
///    int b = rand()%L;
///    iter_swap(current.x.begin()+a, current.x.begin()+b);
///    random_shuffle(current.x.begin(), current.x.end());
///    long f_current = naive_eval(current);
///    if( f_current > f_best)
///    {
///	f_best= f_current;
///	best = current;
///	fprintf(stdout, "%ld %d\n", f_best, cont);
///        print_solution(best);
///	cont *=2;
///    }
///  }
/// return best;
///}
long naive_eval(struct solution &Sol)
{
 long total_score = 0;
  vector<int> cont_book_lib(L, 0);
  vector<bool> checked(B, false);
  int cont = 0;
  int current_time = signup_time_l[Sol.x[cont].first];
  for(int d = current_time; d <D; d++)
  {
     if(d >= current_time )
       current_time +=signup_time_l[Sol.x[++cont].first];
     for(int l = 0; l < cont; l++)//can be improved..
     {
	  int id_lib = Sol.x[l].first;
	  for(int t = 0; t <  shipping_l[id_lib]; t++)//required..
	  {
            if( cont_book_lib[id_lib] < books_in_library[id_lib].size())
            {
//            	      int id_book = books_in_library[id_lib][cont_book_lib[id_lib]++];
		      int id_book = Sol.x[l].second[cont_book_lib[id_lib]++];
            		 if( checked[id_book]) continue;
                             total_score += score_books[id_book];
            		 checked[id_book]=true;
            }
	  }
     }
  }
 return total_score;
}
void unique_books(struct solution &sol)
{
   //for(int i = 0;i < sol.size(); i++)
   //{
   //}
}

void sort_by_signup_time(struct solution &S)
{
   solution prev = S;
   vector<pair<int, int> > vp;
   for(int i = 0; i < S.x.size(); i++)
   vp.push_back(make_pair(signup_time_l[S.x[i].first], i));
   sort(vp.begin(), vp.end());
   for(int i = 0; i < vp.size(); i++)
     S.x[i] = prev.x[vp[i].second];
//   priority_queue<pair<int, int>>pq;
//   struct solution x_old = sol;
//   for(int i = 0; i < L; i++)
//   {
//      pq.push(make_pair(-signup_time_l[i],i));
//   }
//   int cont = 0;
//   while(!pq.empty())
//   {
//    sol.x[cont].second = x_old.x[pq.top().second].second;
//    sol.x[cont++].first = pq.top().second;
//    
//    pq.pop();
//   }
}
solution hyper_heuristic()
{
 solution S;
 initialization(S);
 sort_by_signup_time(S);
 cout << naive_eval(S);
 return S;
}
void load_data()
{
  scanf("%d %d %d\n", &B, &L, &D);
  score_books.resize(B);
  counter_books.resize(B, 0);
  for(int i = 0; i < B; i++) scanf("%d ", &score_books[i]);
  books_in_library.resize(L);
//  sol.x.resize(L);

  signup_time_l.resize(L);
  shipping_l.resize(L);
  for(int i = 0; i < L; i++)
  { 
    int size_library;
    scanf("%d %d %d\n", &size_library, &signup_time_l[i], &shipping_l[i]); 
    books_in_library[i].resize(size_library);
    //sol.x[i].first = i;
    for(int j = 0; j < books_in_library[i].size(); j++)
    {
       scanf("%d ", &books_in_library[i][j]);
   //    sol.x[i].second.push_back(books_in_library[i][j]);
       counter_books[books_in_library[i][j]]++;
    }
  }
}
void initialization(solution &s)//initialization does not takes into consideration the repeated books.
{
   s.x.resize(L);
   vector<bool> book_used(B, false);
   for(int i = 0; i <L ;i++)
   { 
     s.x[i].first = i;
     s.x[i].second.clear();
     for(int j = 0; j < books_in_library[i].size(); j++)
     {
	if( book_used[books_in_library[i][j]])continue;
	s.x[i].second.push_back(books_in_library[i][j]);
	book_used[books_in_library[i][j]] = true;
     }
   }
}
