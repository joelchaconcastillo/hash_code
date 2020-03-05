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
   vector< vector< int> > accumulative_score;
   vector<int> start_day;
   vector<vector<pair<int, int> > > idxBL; //indexes of books in libraries...
   vector<int> out_libraries;
};

solution local_search_1(struct solution initial, int cont);// swapping libraries..
void print_solution(struct solution &sol);
long fast_eval(struct solution &Sol);
void sort_by_signup_time(struct solution &sol);
void unique_books(struct solution &sol);
solution hyper_heuristic();
void load_data();
void initialization(solution &s);
int main()
{
  int s = 1;//time(NULL);
  cout <<s <<endl;
  srand(s);
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

void sort_by_signup_time(struct solution &S)
{
   vector<pair<int, int> > vp;
   for(int i = 0; i < S.x.size(); i++)
   vp.push_back(make_pair( -shipping_l[S.lib_order[i]] + 2*signup_time_l[S.lib_order[i]], i));
   sort(vp.begin(), vp.end());
   for(int i = 0; i < vp.size(); i++) S.lib_order[i] = vp[i].second;
}
solution hyper_heuristic()
{
 solution S;
 initialization(S);
 sort_by_signup_time(S);
 unique_books(S);
 int ite=1000;
 while(ite--)
 {
    //local search 1  by libraries..
     S = local_search_1(S, 1000);
 } 
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
 int day = 0;
 for(auto i_lib = S.lib_order.begin(); i_lib != S.lib_order.end(); i_lib++)
 {
    day +=  signup_time_l[*i_lib];
    if(day >=D) break;
    double lib_cont=0;
    bool updated = false;
    for(auto i_book = S.x[*i_lib].begin(); i_book != S.x[*i_lib].end(); i_book++)
    {
	if( !i_book->second) continue;
        lib_cont++;
	if( ((double)lib_cont/(double)shipping_l[*i_lib])  > (D-day)  )break;
        total_score += score_books[i_book->first]; 
	updated =true;
    }
    if(!updated) day -= signup_time_l[*i_lib];
 }
 return total_score;
}
void update_information(solution &S)
{
  int day = 0;
  S.out_libraries.clear();
  S.start_day.clear();
  S.start_day.resize(L);
  S.accumulative_score.clear();
  S.accumulative_score.resize(L);
  for(auto i_lib = S.lib_order.begin(); i_lib != S.lib_order.end(); i_lib++)
  {
    vector<int> tmp; 
    int partial_score = 0;
    for(auto i_book = S.x[*i_lib].begin(); i_book != S.x[*i_lib].end(); i_book++)
    {
	if( !i_book->second) continue;
        partial_score += score_books[i_book->first]; 
	tmp.push_back(partial_score);
    }
    S.accumulative_score[*i_lib] = tmp;
    if(!tmp.empty())
      day += signup_time_l[*i_lib];
    S.start_day[*i_lib] = day;
    if( (D-day) < S.accumulative_score.size()) S.out_libraries.push_back(*i_lib);
  } 
// for(int i = 0, day = 0; i < lib.size(); i++)
// {
//    int id_lib = lib[i];
//    day += signup_time_l[id_lib];
//    if(day >=D) break;
//    int time_d;
//    if( (D-day) < (double)accum[i].size()/(double)shipping_l[id_lib]  ) //warning overflow.....
//      time_d =  (D - day)*shipping_l[id_lib];
//    else
//       time_d = accum[i].size();
//    total_score +=  accum[i][time_d-1];
// }

}
long incremental_evaluation_libraries(solution &s)
{
 long total_score = 0;
 return total_score;
}
solution local_search_1(solution best, int cont)// swapping libraries..
{
  long f_best= fast_eval(best);
  update_information(best);
  cout << best.out_libraries.size()<<endl;
  cout << f_best<<endl;

  solution current = best;
  while(cont--)
  {
    int idx1 = current.out_libraries[rand()% (int)current.out_libraries.size()], idx2 = rand()%L;
    for(auto i = current.out_libraries.begin(); i != current.out_libraries.end(); i++)
   {
    int idx1 = *i;
    //int idx1 = rand()%L, idx2 = rand()%L;
    iter_swap(current.lib_order.begin()+idx1, current.lib_order.begin()+idx2);
    long f_current = fast_eval(current);
    if( f_current > f_best)
    {
        iter_swap(best.lib_order.begin()+idx1, best.lib_order.begin()+idx2);
        update_information(current);
	f_best= f_current;
	best = current;
        print_solution(best);
	cout << f_best<<endl;
	i = current.out_libraries.begin();
    }
    else
      iter_swap(current.lib_order.begin()+idx1, current.lib_order.begin()+idx2);
    }
  }
 return best;
}

//
//solution local_search_1(solution best, int cont)// swapping libraries..
//{
//  long f_best= fast_eval(best);
////  update_information(best);
//  cout << f_best<<endl;
//
//  solution current = best;
//  while(cont--)
//  {
//    //int idx1 = current.out_libraries[rand()% (int)current.out_libraries.size()], idx2 = rand()%L;
//    int idx1 = rand()%L, idx2 = rand()%L;
//    iter_swap(current.lib_order.begin()+idx1, current.lib_order.begin()+idx2);
//    long f_current = fast_eval(current);
//    if( f_current > f_best)
//    {
//        iter_swap(best.lib_order.begin()+idx1, best.lib_order.begin()+idx2);
//	f_best= f_current;
//	best = current;
//        print_solution(best);
//	cout << f_best<<endl;
//    }
//    else
//      iter_swap(current.lib_order.begin()+idx1, current.lib_order.begin()+idx2);
//  }
// return best;
//}
