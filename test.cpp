#include<bits/stdc++.h>
#define MAXSIZE 10001
#define MAXC 1000
#define MAXR 1000
using namespace std;
int item[MAXSIZE];
int M, N; // 1x10^9 and 1x10^5
//DP solution...
int bk(int current, int remaining)
{
 if( remaining < 0) return -10000000;
 if( current == N)
    return M-remaining;
 return max(bk(current+1, remaining-item[current]), bk(current+1, remaining));
}
int T[MAXR][MAXC];
int prev[MAXR][MAXC];
int main()
{
  scanf("%d %d", &M, &N);

  for(int i = 0; i < N; i++)
    scanf("%d", &(item[i]));
  for(int i = 0; i < MAXR; i++)
     for(int j = 0 ; j < MAXC; j++)
	{
	T[i][j] = -1;
	prev[i][j]=-1;
	
	}
  
  for(int i = 0;i < N; i++)
  {	
    if( M-item[i] >=0)
       T[0][M-item[i]] = i;
  }
  bool flag=true; 
  int c_row = 1, min_r=0, min_c=1000;
  while(flag)
  {
    flag = false;
    for(int i = 0; i < M; i++)
    {
       if(T[c_row-1][i] != -1)
	{
	  for(int j = T[c_row-1][i]+1; j < N; j++)
	  {
		int new_rem = i-item[j];
		if(new_rem >=0)
		{
	           T[c_row][new_rem] = j;

	           prev[c_row][new_rem] = i;

		   if(min_c > new_rem)
		   {
		     min_r = c_row;
		     min_c = new_rem;
		   }
		}
	  }
	  flag=true;
	}
    } 
    if(!flag) break;
    c_row++;
  }
 printf("%d\n", M-min_c);
  vector<int> sol;
  while(min_r >= 0 && T[min_r][min_c]!=-1)
  {
	sol.push_back(T[min_r][min_c]);
	min_c = prev[min_r][min_c];
	min_r--;
  }
 //printf("%d\n", sol.size());
 for(int i = sol.size()-1; i>= 0; i--)
 {
  printf("%d ", sol[i]);
 }
  //printf("%d\n", bk(0,M));

  return 0;
}
