#include <stdio.h>
#include <curses.h>
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>

#define ESC 27
#define BCKSPC 127

using namespace std;

vector<string > in_array;

vector<int > look_for(const string search_term, int pos, const vector<string > &in_array, vector<int > &indexes)
{
	vector<int > new_indexes;
	char compare_ch, ch;
	if (search_term.empty())
		return new_indexes;
	if (pos < 0)
	   	pos = 0;
	for (vector<int >::iterator index = indexes.begin(); index != indexes.end(); ++index)
	{
		compare_ch = (in_array[*index])[pos];
		ch = search_term[pos];
		if (compare_ch == ch)
			new_indexes.push_back(*index);
	}
	return new_indexes;
}

int main()
{
	// Initialize the input array of strings.
	vector<string > in_array;

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

	// Stores the search_term
	string search_term;

	// Key pressed by the user
	char ch;
	// Stack of vectors. Each vector stores a list of indexes that match the current search_term.
	// Top of the stack has the vector with latest matching entries.
	// Bottom of the stack has the vector with all the indexes in in_array.
	// Each entry is the stack will be a subset of the previous entry.
	stack<vector <int > > possibles;

	// Current position of the cursor where the next character will be entered.
	int cursor_pos = 0;
	// Current position of the character to look for in the in_array's strings.
	// Initialize to -1, since no key is pressed yet. 
	int pos = -1; 

	vector<int > indices; // Vector to store the indexes of in_array that match the current search_term
	int index = 0;

	// Initialize the indices with all the entries in in_array.
	for (int i = 0 ; i < in_array.size(); i++)
	{
		indices.push_back(i);
	}

	// Add 'indices' to the stack. This will always be the bottom of the stack.
	possibles.push(indices);

	// ncurses initialization
	initscr();
	cbreak();
	noecho();

	for (vector<int >::iterator it = indices.begin(); it != indices.end(); ++it )
	{
		printw("%s\n",in_array[*it].c_str());
		//printf("%s\n",in_array[*it].c_str());
	}

	// Keep getting characters until esc key is pressed.
	while (1)
	{
		// Get one character
		ch = getch();
		if (ESC == ch) // If current char is ESC, break out of loop
			break;
		// If backspace is pressed then pop the last char from the search_term
		else if (BCKSPC == ch)
		{
			if ( !search_term.empty() ) // Do not try to erase an empty string. cursor_pos can never be less than 0
			{
				search_term.erase(cursor_pos - 1);  // Remove the char from the string
				if (cursor_pos > 0)
					cursor_pos--;
			}
			while (possibles.size() > cursor_pos+1)
				possibles.pop();
		}
		else if ( (ch >= 'a' && ch <= 'z') || (ch >='A' && ch <= 'Z') || (ch >='0' && ch <='9') )
		{
			search_term.push_back(ch); // store the current key to the search string
			cursor_pos++;
		}

		pos = cursor_pos - 1;
		while(pos < search_term.size())
		{
			indices = look_for(search_term, pos, in_array, possibles.top());
			pos++;
			possibles.push(indices);
			
		}
		
		vector<int > cur_results;

		cur_results = possibles.top();

		//Clear the screen before printing the results
		clear();

		// print results
		for (vector<int >::iterator it = cur_results.begin(); it != cur_results.end(); ++it )
		{
			printw("%s\n",in_array[*it].c_str());
			//printf("%s\n",in_array[*it].c_str());
		}

	}
	endwin();
	return 0;
}
