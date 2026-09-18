#include <iostream>
using namespace std;

int main()
{
	int n;
	cin >> n;
	int v[n];
	
	for (int i = 0; i < n; i++)
		cin >> v[i];
	
	int temp, i, j;
	for ( i = 1; i < n; i++)
	{
		temp = v[i];
		
		for( j = i - 1; j >= 0 && temp <v[j]; j--)
			v[j + 1] = v[j];
			
		v[j + 1] = temp;
	}
	
	for(i =0; i < n; i++)
		cout << v[i] << " ";
		
	cout << endl;
	return 0;
}
