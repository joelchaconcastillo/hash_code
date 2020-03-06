#include <bits/stdc++.h>
#define MAXNODE 1000
#define INF 100000
using namespace std;

void augment(int v, int minEdge, vector<int> &p, int s, unordered_map<int, unordered_map<int, int> > adj, int &f)
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
     adj[v][p[v]] +=f;
  }
}
void max_flow(unordered_map<int, unordered_map<int, int> > adj, int s, int t)
{
  vector<int> p;
  int mf, f;
   mf = 0;
   while(true)
   {
      f = 0;
      vector<int> dist(adj.size(), INF);
      dist[s] = 0;
      queue<int> q;
      q.push(s);
      while(!q.empty())
      {
	int u = q.front();
        q.pop();
	if(u==t) break;	
        for(auto i = adj[u].begin(); i != adj[u].end(); i++)
	{
	   if( i->second > 0 && dist[i->first] == INF )
	   {
	      dist[i->first] = dist[u]+1;
	      q.push(i->first);
	      p[i->first] = u; 
	   }
	}
      }
      augment(t, INF, p, s, adj, f);
      if(f==0) break;
      mf +=f;
   }
}
int main()
{
  return 0;
}
