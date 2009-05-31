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

vector<int > look_for(const string search_term, const int pos, const vector<string > &in_array, vector<int > &indexes)
{
	vector<int > new_indexes;
	char compare_ch, ch;
	if (pos < 0)
		return new_indexes;
	for (vector<int >::iterator index = indexes.begin(); index != indexes.end(); ++index)
	{
		compare_ch = (in_array[*index])[pos];
		ch = search_term[pos];
		if (compare_ch == ch)
			new_indexes.push_back(*index);
	}
	return new_indexes;
}

/*
void look_for(int pos, char ch, vector<int > &indices)
{
	if(pos < 0 ) // if search_string is empyt (pressing backspace to begin with ), then do nothing.
		return;
	// For each character pressed, we create a new list node.
	// Each node has a vector of ints.
	// We narrow down the in_array entries that match and add the 
	// index to the vector.

	static int prev_pos = -1; // This has to be -1 to begin with.

	// If the incoming pos is less than or equal to pos, then a back-space key is pressed. 
	// Pop the stack to remove the search possibilities with the prev character pressed.
	if (pos <= prev_pos) 
		possibles.pop();

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
		char compare_ch = (in_array[*i].c_str())[pos]; // The compare_ch is the character at the 'pos' position of the input string
		if (ch == compare_ch)
			new_indices.push_back(*i);
	}
	if (pos > prev_pos)
	{
		indices = new_indices; // Do not update the indices with the new_indices if BCKSPC is pressed
		possibles.push(indices);
	}
	prev_pos = pos;
}
*/
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
	
	// Keep getting characters until esc key is pressed.
	while (1)
	{
		// Get one character
		ch = getch();
		if (ESC == ch) // If current char is ESC, break out of loop
			break;
		// If backspace is pressed then pop the last entry from the search_term
		/* Worry about backspace later
		if (BCKSPC == ch)
		{
			if (!search_term.empty()) // Do not try to pop an empty string
				search_term.resize(search_term.length()-1);  // Remove the last char from the string
			look_for(search_term.length()-1, ch, indices); // Call the search routine to narrow down the list. The narrowed down list is filled inside the indicies vector.
		}
		*/
		if ( (ch >= 'a' && ch <= 'z') || (ch >='A' && ch <= 'Z') || (ch >='0' && ch <='9') )
		{
			search_term.push_back(ch); // store the current key to the search string
			cursor_pos++;
			pos = cursor_pos - 1;
		}
		//printw("Key Pressed:%d\n", ch);
		//printw("Search Results:\n");

		while(pos < search_term.size())
		{
			indices = look_for(search_term, pos, in_array, possibles.top());
			pos++;
			if(!indices.empty())
				possibles.push(indices);
			
		}


		vector<int > cur_results;

		if (!search_term.empty())
			cur_results = indices;
		else
			cur_results = possibles.top();

		//Clear the screen before printing the results
		clear();

		// print results
		for (vector<int >::iterator it = cur_results.begin(); it != cur_results.end(); ++it )
		{
			printw("%s\n",in_array[*it].c_str());
		}

		/*if (!search_term.empty() && !indices.empty())
		{
			// Print the narrowed down list everytime a key is pressed
			clear();
			for_each(indices.begin(), indices.end(), print_results);
		}
		else if(search_term.empty())
		{
			index = 0;
			for(vector<string >::iterator str_it = in_array.begin(); str_it != in_array.end(); ++str_it)
			{
				indices.push_back(index++);
			}
			for_each(indices.begin(), indices.end(), print_results);
		}*/
	}
	endwin();
	return 0;
}
