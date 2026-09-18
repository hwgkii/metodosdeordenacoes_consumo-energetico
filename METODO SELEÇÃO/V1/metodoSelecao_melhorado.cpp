#include <iostream>
using namespace std;

int main()
{
	int n;
	cin >> n;
	int v[n];
	
	
	for (int i = 0; i < n; i++)
	{
		cin >> v[i];
	}

	for (int i=0; i < n; i++)
	{
		int menorValor = i;
		
		for (int j = i + 1; j < n; j++)
		{
			
			if (v[j] < v[menorValor])
				menorValor = j;
		}
		
		int temp = v[i];
		v[i] = v[menorValor];
		v[menorValor] = temp;
	}
	
	for (int i = 0; i < n; i++)
	{
		cout << v[i] << " ";
	}
	
	cout << endl;

	return 0;
}
