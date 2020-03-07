#include <bits/stdc++.h>
#define INF 1000000000
using namespace std;

int B, L, D; //number of books, librearies and days....
vector<int> score_books, signup_time_l, shipping_l; // score of each book, number of days it takes to finish the library to 
vector<vector<int> > books_in_library;
vector<int> counter_books;


///the representation is by index of libraries and a mask indicating which book is active in each library..
struct solution
{
   vector<int> lib_order;
   vector< vector<int> > > edgeLB; // the first is library ids, second id of books..
   vector<pair< int, vector<pair<int, int> > > > edgeBL; //books in libraries, < library index,  < active index, <id_library, index in id_library> > >

   vector<int> start_day;
   vector<int> disabled_libraries;
   vector< vector< int> > accumulative_score;
};


void augment(int v, int minEdge, vector<int> &p, int s, unordered_map<int, unordered_map<int, int> > &adj, int &f);
void max_flow(unordered_map<int, unordered_map<int, int> > &adj, int s, int t);

solution local_search_1(struct solution initial, int cont);// swapping libraries..
void print_solution(struct solution &sol);
long fast_eval(struct solution &Sol);
void sort_by_signup_time(struct solution &sol);
void unique_books(struct solution &sol);
void modeling_MBMP(solution &S);
solution hyper_heuristic();
void load_data();
void initialization(solution &s);
int main()
{
  int s =1;//time(NULL);
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
	for(auto j = sol.edge[*i].begin(); j != sol.edge[*i].end() ; j++)
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
   for(int i = 0; i < S.edge.size(); i++)
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
// modeling_MBMP(S);
// cout << fast_eval(S);
//exit(0);
 int ite=1000;
 while(ite--)
 {
    //local search 1  by libraries..
     S = local_search_1(S, 100);
     S = local_search_2(S, 100);
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
   s.edge.resize(L);
   s.idxBL.resize(B);
   vector<bool> book_used(B, false);
   for(int i = 0; i <L ;i++)
   { 
     s.lib_order[i] = i;
     s.edge[i].clear();
     for(int j = 0; j < books_in_library[i].size(); j++)
     {
	s.idxBL.second[books_in_library[i][j]].push_back(make_pair(i,s.edge[i].size()));
	s.edge[i].push_back(make_pair(books_in_library[i][j], !book_used[books_in_library[i][j]]));
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
      for(auto j = s.edge[*i].begin();  j != s.edge[*i].end(); j++)
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
    for(auto i_book = S.edge[*i_lib].begin(); i_book != S.edge[*i_lib].end(); i_book++)
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
  S.disabled_libraries.clear(); 
  S.start_day.clear();
  S.start_day.resize(L);
  S.accumulative_score.clear();
  S.accumulative_score.resize(L);
  for(auto i_lib = S.lib_order.begin(); i_lib != S.lib_order.end(); i_lib++)
  {
    vector<int> tmp; 
    int partial_score = 0;
    for(auto i_book = S.edge[*i_lib].begin(); i_book != S.edge[*i_lib].end(); i_book++)
    {
	if( !i_book->second) continue;
        partial_score += score_books[i_book->first]; 
	tmp.push_back(partial_score);
    }
    S.accumulative_score[*i_lib] = tmp;
    if(!tmp.empty())
      day += signup_time_l[*i_lib];
    if( day + tmp.size() >= D) S.disabled_libraries.push_back(*i_lib);
    S.start_day[*i_lib] = day;
//    if( (D-day) < S.accumulative_score.size()/shipping_l[*i_lib]) S.out_libraries.push_back(*i_lib);
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
//long incremental_evaluation_libraries(solution &s)
//{
// long total_score = 0;
// return total_score;
//}
solution local_search_1(solution best, int cont)// swapping libraries..
{
  long f_best= fast_eval(best);
  bool improved = true;
  while(improved)
  {
    improved = false;
    //build neighbour..
    vector< pair<int, int> > vp_neighbour;
    update_information(best);
    solution current = best;
    
    //defining neighbour neighbour... 
    for(int i = 0; i < current.disabled_libraries.size(); i++)
       for(int j = 0 ; j < current.lib_order.size(); j++)
 	 vp_neighbour.push_back(make_pair(current.disabled_libraries[i], current.lib_order[j])); 
    random_shuffle(vp_neighbour.begin(), vp_neighbour.end());
   
    for(auto i = vp_neighbour.begin(); i != vp_neighbour.end(); i++)
    {
       iter_swap(current.lib_order.begin() + i->first, current.lib_order.begin()+i->second);
       long f_current = fast_eval(current);
       if( f_current > f_best)
       {
	   improved = true;
           iter_swap(best.lib_order.begin()+i->first, best.lib_order.begin()+i->second);
           f_best= f_current;
           best = current;
           print_solution(best);
           cout << f_best<<endl;
       }
       else
         iter_swap(current.lib_order.begin()+i->first, current.lib_order.begin()+i->second);
       }
    }
 return best;
}

solution local_search_2(solution best, int cont)// swapping libraries..
{
  long f_best= fast_eval(best);
  bool improved = true;
  
  while(improved)
  {
    improved = false;
    //build neighbour..
    solution current = best;
    //defining neighbour neighbour... 
    for(int i = 0; i < cont ; i++) 
    {
       int id_book = rand()%current.idxBL.size();
       while(current.idxBL[id_book].size()<=1) id_book = rand()%current.idxBL.size();

       int idx_lib_old = current.idxBL[id_book].first;
       int idx_lib_new = rand()%current.idxBL[id_book].size();
       while(id_lib_new == id_lib_old) idx_lib_new = rand()%current.idxBL[id_book].size()
       current.idxBL[id_book].first = id_lib_new;
       
       for(int j = 0; j < current.idxBL[idxa].size(); j++)
	current[]
              
       long f_current = fast_eval(current);
       if( f_current > f_best)
       {
	   improved = true;
           f_best= f_current;
           best = current;
           print_solution(best);
           cout << f_best<<endl;
       }
       else
         iter_swap(current.lib_order.begin()+i->first, current.lib_order.begin()+i->second);
       }
    }
 return best;
}
void  modeling_MBMP(solution &S) //build a Maximum Bipartite Matching Problem
{

//  //getting the active libraries...
//  //compute the number of book per library given the current order..
  unordered_map<int, unordered_map<int, int> > adj;
  int s, t;

//  for(int i = 0 ; i < S.edge.size(); i++)
  for(auto i = S.lib_order.begin(); i != S.lib_order.end(); i++)
  {
     for(auto j = S.edge[*i].begin(); j != S.edge[*i].end(); j++)
     {
	int a = *i, b = (j->first+L);
	adj[a][b] = INF; 
	adj[b][a] = 0; //link between libreries and books...
     }
  }
  s = L+B;
  t = L+B+1;
  adj[s][s] = -1;
  adj[t][t] = -1;
  for(auto i = S.lib_order.begin(); i != S.lib_order.end(); i++) adj[s][*i] = S.edge[*i].size();
  for(int j = 0; j < B; j++) adj[j+L][t] = 1;
  max_flow(adj, s, t);
   for(auto i = S.lib_order.begin(); i != S.lib_order.end(); i++)
  {
     for(int j = 0;  j < S.edge[*i].size(); j++)
     { 
        S.edge[*i][j].second = false;
	int a = *i, b = (S.edge[*i][j].first+L);

	if(adj[b][a] == 1 ){
	cout << *i << " " <<j <<endl;
//	cout << "aa"<<endl;
	S.edge[*i][j].second = true;
	}
     }
  }
 
}

//////////////////////7
void augment(int v, int minEdge, vector<int> &p, int s, unordered_map<int, unordered_map<int, int> > &adj, int &f)
{
  if(s==v)
  {
      f = minEdge; 
      return;
  }
  else if( p[v] != -1)
  {
     augment(p[v], min(minEdge, adj[p[v]][v]),p,s, adj, f);
     adj[p[v]][v] -=f;
//     adj[v][p[v]] +=f;
  }
}
void max_flow(unordered_map<int, unordered_map<int, int> > &adj, int s, int t)
{
  int mf = 0, f;
   while(true)
   {
      vector<int> p(adj.size(), -1);
//	cout << "\n"<< p.size() <<endl;
      f = 0;
      vector<int> dist(adj.size(), INF);
      dist[s] = 0;
      queue<int> q;
      q.push(s);
      while(!q.empty())
      {
	int u = q.front();
        q.pop();
//	cout << u <<":"<< t <<endl;
	if(u==t) break;	
        for(auto i = adj[u].begin(); i != adj[u].end(); i++)
	{
	//cout << u << "_"<<i->first << "_"<<i->second<<" w: "<< dist[i->first] <<endl;
	   if( i->second > 0 && dist[i->first] == INF)// && u != i->first)
	   {
	      dist[i->first] = dist[u]+1;
	      q.push(i->first);
	      p[i->first] = u; 
	   }
	}
      }
      augment(t, INF, p, s, adj, f);
      if(f==0) break;
	cout << mf<<endl;
      mf +=f;
   }
	cout << mf <<endl;
}
