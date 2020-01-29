#include<bits/stdc++.h>
#define MAXSIZE 100001
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
int main()
{
  scanf("%d %d", &M, &N);

  for(int i = 0; i < N; i++)
    scanf("%d", &(item[i]));
  printf("%d\n", bk(0,M));

  return 0;
}
