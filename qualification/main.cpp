#include <bits/stdc++.h>
using namespace std;

int B, L, D; //number of books, librearies and days....
vector<int> score_books, signup_time_l, shipping_l; // score of each book, number of days it takes to finish the library to 
vector<vector<int> > books_in_library;
vector<int> counter_books;

///the representation is by index of libraries and a mask indicating which book is active in each library..
struct solution
{
   vector<int> lib_order;
   vector< vector<pair<int, bool> > > x; // the first is library ids, second id of libraries..
   vector<vector<pair<int, int> > > idxBL; //indexes of books in libraries...
};

struct solution iterated_local_search(struct solution initial);
void print_solution(struct solution &sol);
//long naive_eval(struct solution &Sol);
long fast_eval(struct solution &Sol);
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
  return 0;
}
void print_solution(struct solution &sol)
{

  vector<int> libs;
  vector< vector<int> > l_b;
  for(auto i = sol.lib_order.begin(); i != sol.lib_order.end(); i++)
  {
	vector<int> tmp;
	for(auto j = sol.x[*i].begin(); j != sol.x[*i].end() ; j++)
	   if( j->second) tmp.push_back(j->first);
       if(!tmp.empty())
       {
	l_b.push_back(tmp);
	libs.push_back(*i);
       }
  }

  //printf("%d\n", (int)sol.x.size());
  fprintf(stderr, "%d\n", (int)libs.size());
  //for(auto i = sol.lib_order.begin(); i != sol.lib_order.end(); i++)
  for(int i = 0; i < l_b.size(); i++)
  {
     //printf("%d %d\n", sol.x[i], (int)sol.B_L[sol.x[i]].size());
     fprintf(stderr, "%d %d\n", libs[i], (int)l_b[i].size());
     for(int j = 0; j < l_b[i].size(); j++)
     {
	fprintf(stderr, "%d ", l_b[i][j]);
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
//long naive_eval(struct solution &Sol)
//{
// long total_score = 0;
//  vector<int> cont_book_lib(L, 0);
//  vector<bool> checked(B, false);
//  int cont = 0;
//  int current_time = signup_time_l[Sol.x[cont].first];
//  for(int d = current_time; d <D; d++)
//  {
//	cout <<cont <<endl;
//     if(d >= current_time )
//       current_time +=signup_time_l[Sol.x[++cont].first];
//     for(int l = 0; l < cont; l++)//can be improved..
//     {
//	  int id_lib = Sol.x[l].first;
//	  for(int t = 0; t <  shipping_l[id_lib]; t++)//required..
//	  {
//            if( cont_book_lib[id_lib] < Sol.x[l].second.size())
//            {
////            	      int id_book = books_in_library[id_lib][cont_book_lib[id_lib]++];
//		      int id_book = Sol.x[l].second[cont_book_lib[id_lib]++];
//            		 if( checked[id_book]) continue;
//                             total_score += score_books[id_book];
//            		 checked[id_book]=true;
//            }
//	  }
//     }
//  }
// return total_score;
//}


void sort_by_signup_time(struct solution &S)
{
   vector<pair<int, int> > vp;
   for(int i = 0; i < S.x.size(); i++)
   vp.push_back(make_pair(signup_time_l[S.lib_order[i]], i));
   sort(vp.begin(), vp.end());
   for(int i = 0; i < vp.size(); i++) S.lib_order[i] = vp[i].second;
}
solution hyper_heuristic()
{
 solution S;
 initialization(S);
 sort_by_signup_time(S);
 unique_books(S);
 print_solution(S);
 cout << fast_eval(S);
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
   s.lib_order.resize(L);
   s.x.resize(L);
   s.idxBL.resize(B);
   vector<bool> book_used(B, false);
   for(int i = 0; i <L ;i++)
   { 
     s.lib_order[i] = i;
     s.x[i].clear();
     for(int j = 0; j < books_in_library[i].size(); j++)
     {
	s.idxBL[books_in_library[i][j]].push_back(make_pair(i,s.x[i].size()));
	s.x[i].push_back(make_pair(books_in_library[i][j], !book_used[books_in_library[i][j]]));
	book_used[books_in_library[i][j]] = true;
     }
   }
}
void unique_books(struct solution &s)
{
   vector<bool> u_books(B, false);
   //for(int i = 0;i < s.x.size(); i++)
   for(auto i = s.lib_order.begin(); i != s.lib_order.end() ; i++)
   {
      for(auto j = s.x[*i].begin();  j != s.x[*i].end(); j++)
      {
	  j->second = !u_books[j->first];
          u_books[j->first] = true;
      }
   }
}

long fast_eval(struct solution &S)
{
 long total_score = 0;
 vector< vector<int > > accum;
 vector<int> lib;
 for(auto i_lib = S.lib_order.begin(); i_lib != S.lib_order.end(); i_lib++)
 {
    int partial_score = 0;
    vector<int> tmp;
    for(auto i_book = S.x[*i_lib].begin(); i_book != S.x[*i_lib].end(); i_book++)
    {
	if( !i_book->second) continue;
        partial_score += score_books[i_book->first]; 
	tmp.push_back(partial_score);
    }
    if(tmp.empty()) continue;
    accum.push_back(tmp);
    lib.push_back(*i_lib);
 }
 for(int i = 0, day = 0; i < lib.size(); i++)
 {
    int id_lib = lib[i];
    day += signup_time_l[id_lib];
    if(day >=D) break;
    int time_d;
    if( (D-day) < (double)accum[i].size()/(double)shipping_l[id_lib]  ) //warning overflow.....
      time_d =  (D - day)*shipping_l[id_lib];
    else
       time_d = accum[i].size();
    total_score +=  accum[i][time_d-1];
 }
 return total_score;
}
