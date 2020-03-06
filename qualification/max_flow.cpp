#include <bits/stdc++.h>
#define MAXNODE 1000
#define INF 100000
using namespace std;

void augment(int v, int minEdge, vector<int> &p, int s)
{
  if(s==v)
  {
      f = minEdge; 
      return;
  }
  else if( p[v] != -1)
  {
     augment(p[v], min(minEdge, res[p[v]][v]));
     res[p[v]][v] -=f;
     res[v][p[v]] +=f;
  }
}
void max_flow(vector<vector<pair<int, int> > >f_adj, vector<vector<pair<int, int> > >f_adj, int s, int e)
{
  vector<int> p;
  int mf, f, s ,t;

  mf = 0;
   while(true)
   {
      f = 0;
      vector<int> dist(MAXNODE, INF);
      dist[s] = 0;
      queue<int> q;
      q.push(s);
      while(!q.empty())
      {
	int u = q.front();
        q.pop();
	if(u==t) break;	
	for(int i = 0; i < f_adj[u].size(); i++)
	{
	   pair<int, int> v = f_adj[u][i];
	   if( v.second > 0 && dist[v.first] == INF )
	   {
	      dist[v.first] = dist[u]+1;
	      q.push(v.first);
	      p[v.first] = u; 
	   }
	}
      }
      augment(t, INF, p, s);
      if(f==0) break;
      mf +=f;
   }
}
int main()
{
  return 0;
}
