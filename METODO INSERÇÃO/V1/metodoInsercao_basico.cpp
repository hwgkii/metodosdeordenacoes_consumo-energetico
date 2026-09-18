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
        for (int i = 1; i < n; i++)
	{
    		for (int j = i; j > 0 && v[j] < v[j - 1]; j--)
    		{
        		int temp = v[j];
        		v[j] = v[j - 1];
      			v[j - 1] = temp;
    		}
	}


        for(i =0; i < n; i++)
                cout << v[i] << " ";

        cout << endl;
        return 0;
}
