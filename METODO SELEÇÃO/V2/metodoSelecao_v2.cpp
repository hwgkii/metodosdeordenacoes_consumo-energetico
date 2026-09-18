#include <iostream>
#include <queue>
using namespace std;

int main()
{
    int n;
    cin >> n;

    int v[n];
    int x;

    priority_queue<int> selecao;

    for (int i = 0; i < n; i++)
    {
	cin >> x;
	selecao.push(-x);
    }

    for (int i = 0; i < n; i++)
    {
        v[i] = -selecao.top();
        selecao.pop();

    }

    for (int i = 0; i < n; i++)
        cout << v[i] << " ";

    cout << endl;

    return 0;
}
