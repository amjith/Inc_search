#include <stdio.h>

#define MAXSTR 25    /* Maximum number of strings in array */
#define MAXLEN 30     /* Maximum length of each string */
#define ESC 27        /* ASCII code for Escape */

/* if MIXCASE is nonzero, comparisons are case-sensitive */
#define MIXCASE 0
#if MIXCASE
#define CASEMOD(x) x
#else
#define CASEMOD toupper
#endif

typedef char STR[MAXLEN+1];             /* Definition of type STR */
typedef STR STARRAY[MAXSTR];            /* ... of type STARRAY    */

int get_range(search_char,array,last_index,ptr_first,ptr_last)
	char search_char;
	STR *array;
	int last_index, *ptr_first, *ptr_last;
{
	int j, k;     /* Loop variables */
	int found;    /* Indicates if a match has been found */

	search_char = CASEMOD(search_char);
	for (j=0; j<=last_index; j++)  /* Seek first match */
		if (found = CASEMOD(array[j][0]) == search_char)
		{                          /* A match has been found at index j */
			k = j;                    /* Store value in k */
			break;
		}
	if (found)
	{          /* A match has been found at index k; determine its extent */
		for (j=k; CASEMOD(array[j+1][0]) == search_char; j++)
			;      /* Set j to last matching index */
		*ptr_first = k;
		*ptr_last = j;
		return(1);
	}
	return(0);
}

int getkey(void)
{ char ch;

	while (1)
	{
		if ((ch = getchar()) == '\'' || ch == '\"')
			return(0);
		if (ch == ESC || ch == '\r' || ch >= 32 && ch <= 125)
		{ putchar(ch);
			return(ch);
		}
		if (ch == 0) getchar();  /* Discard function key input */
	}
}
int inc_search(arr,imax,ptr_start,ptr_end)
	STARRAY arr;
	int *ptr_start, *ptr_end;
{  int first,last,ch;    /* Parameters for GET_RANGE call */
	int OK = 1;           /* Return value of GET_RANGE; defaults to one */
	int start,end;         /* First and last indices that yield a match */
		STR *base;            /* Starting location for search */

	end = imax;
	base = arr; start = 0;
	while ( (ch = getkey()) != '\r' && ch != ESC
			&& (OK = get_range(ch,base,imax, &first, &last)))
	{   
		/* No CR or ESC has been entered,
		 * and the list of
		 * matches is
		 * nonempty */
		end = start + last;      /* Adjust values of end and start */
		start += first;
		if (strcmp(arr[start],arr[end])==0)
			break;           /* Abort loop if there's a unique match */
		else               /* A match has been found, but it's not unique */
		{               /* Adjust start location for search */
			base = &base[first][1];
			imax = last - first;  /* Adjust max search index */
		}
	}
	if (ch == ESC)
		return(-1);               /* Return -1 if search was aborted */
	/* Else return values as described above */
	*ptr_start = start;
	*ptr_end = end;
	return(OK);

}

/* The code that appears from this point on is used solely to */
/* demonstrate the incremental search facility. */

STARRAY arr =
{ "Anderson","Bradley","Burke","Federico","Fikar",
	"Frazer","Geary","Grando","Haas","Lehr","Lemke","Mahoney",
	"Martin","Martino","Milne","Milnor","Montgomery","Rivera",
	"Rivers"};

int do_again(void)
{ 
	char ch;

	printf("\n Do again ? (Y/N) ");
	while ((ch = toupper(getchar())) != 'Y' && ch != 'N')

		;
	putchar(ch);
	return(ch);
}
main()
{  int k,maxindex,start,end;

	/* Print the array to be searched */
	for (k=0; k<=MAXSTR-1; k++)
		if (arr[k][0] == 0)
		{ maxindex = k-1;
			printf("\n");
			break;
		}
		else printf("%-20s",arr[k]);
	for (k=1; k<=80; k++)
		putchar('=');
	/* Perform incremental search */
	do  {
		printf("\n Enter a search string : ");
		switch (inc_search(arr,maxindex, &start, &end)) {
			case -1 : printf ("\n Aborted by user.");
					  break;
			case  0 : printf("\n No matches found.");
					  break;
			case  1 : printf("\n Matches are : \n");
					  for (k=start; k<=end; k++)
						  printf("%-20s",arr[k]);
					  printf("\n");
		}
	}
	while (do_again() == 'Y');
}

