#include <bits/stdc++.h>
#define MAXNODE 1000
#define INF 100000
using namespace std;
vector<vector<int>>edge;
vector<int> p;
int res[MAXNODE][MAXNODE], mf, f, s ,t;

void augment(int v, int minEdge)
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
int max_flow()
{
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
	for(int i = 0; i < edge[u].size(); i++)
	{
	   int v = edge[u][i];
	   if( res[u][v] > 0 && dist[v] == INF )
	   {
	      dist[v] = dist[u]+1;
	      q.push(v);
	      p[v] = u; 
	   }
	}
      }
      augment(t, INF);
      if(f==0) break;
      mf +=f;
   }
}
int main()
{
  return 0;
}
