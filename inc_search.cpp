#include <stdio.h>
#include <curses.h>
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>

#define ESC 27

using namespace std;

vector<string > in_array;

void print_results(int n)
{
	printw("%s\n",in_array[n].c_str());
}

void look_for(string search_term, char ch, vector<int > &indices)
{
	// For each character pressed, we create a new list node.
	// Each node has a vector of ints.
	// We narrow down the in_array entries that match and add the 
	// index to the vector.

	static stack<vector <int > > possibles;

	// A vector to store the new indicies after we narrowed it down using the latest char.
	vector<int > new_indices;

	// Last node's vector has the most latest narrowed down indexes.
	// For each index in this last node's vector, we look at the corresponding
	// in_array entry and see if it matches the new character. 
	// Matching entries' indexes are added to a new vector which will then 
	// become the latest linked list node.
	if (!possibles.empty())
		indices = possibles.top();
	for( vector<int >::iterator i = indices.begin(); i !=indices.end(); ++i)
	{
		char compare_ch = (in_array[*i].c_str())[search_term.length()-1];
		if (ch == compare_ch)
			new_indices.push_back(*i);
	}
	indices = new_indices;
	possibles.push(indices);
}

int main()
{

	// Initialize the input array of strings.
	/*in_array[0] = "abc";
	in_array[1] = "abde";
	in_array[2] = "abce";
	in_array[3] = "bda";
	in_array[4] = "bdecf";
	in_array[5] = "bfeda";
	in_array[6] = "cade";
	in_array[7] = "cabe";
	in_array[8] = "abcd";*/

	in_array.push_back("abde");
	in_array.push_back("abce");
	in_array.push_back("bda");
	in_array.push_back("bdecf");
	in_array.push_back("bfeda");
	in_array.push_back("cade");
	in_array.push_back("cabe");
	in_array.push_back("abcd");
	in_array.push_back("abc");
	in_array.push_back("bdac");

	string search_term;
	char ch;
	vector<int > indices;
	int index = 0;
	initscr();
	cbreak();
	noecho();
	for (vector< string>::iterator st_i = in_array.begin(); st_i != in_array.end(); ++st_i)
	{
		printw("%s\n",(*st_i).c_str());
		indices.push_back(index++);
	}
	// Keep getting characters until esc key is pressed.
	while (1)
	{
		// Get one character
		ch = getch();
		if (ESC == ch) // If current char is ESC, break out of loop
			break;
		if (ch >= 'a' && ch <= 'z')
		{
			search_term.push_back(ch); // store the current key to the search string
			look_for(search_term, ch, indices); // Call the search routine to narrow down the list. The narrowed down list is filled inside the indicies vector.
		}
		printw("Search Results:\n");
		clear();
	   	// Print the narrowed down list everytime a key is pressed
		for_each(indices.begin(), indices.end(), print_results);
	}
	endwin();
	return 0;
}
