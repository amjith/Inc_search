#include <stdio.h>
#include <iostream>
#include <vector>
#include <list>
#include <string>

#define ESC 27

using namespace std;

vector<string> in_array;

void print_results(int n)
{
	cout<<in_array[n]<<endl;
}

int main()
{
	in_array[0] = "abc";
	in_array[1] = "abde";
	in_array[2] = "abce";
	in_array[3] = "bda";
	in_array[4] = "bdecf";
	in_array[5] = "bfeda";
	in_array[6] = "cade";
	in_array[7] = "cabe";
	in_array[8] = "abcd";

	string search_term;
	char ch;
	vector<int> indeces;

	while (1)
	{
		ch = getchar();
		if (ESC == ch)
			break;
		search_term.push_back(ch);
		look_for(search_term, ch, in_array, indeces);
		cout<<"Search Results:"<<endl;
		for_each(indeces.begin(), indeces.end(), print_results());
	}

	return 0;
}
