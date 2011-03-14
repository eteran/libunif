
#include "std_func.h"

#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>

/*-----------------------------------------------------------------------------
// Name: ask_question_yn(const char *query)
// Desc: presents the user with a yes or no question and waits for a 'y' or 'n'
// Note: treats no input as 'n' 
//---------------------------------------------------------------------------*/
int ask_question_yn(const char *query) {

	char buffer[256];
	
	assert(query != 0);

	do {
		/* print the question */
		printf("%s", query);
		
		/* get the answer */
		if(fgets(buffer, sizeof(buffer), stdin) == 0) {
			return 0;
		}
		
		buffer[0] = tolower(buffer[0]);
		
		
		/* we only can possibly read one character at a time, so only
		 * need to check the first */
		if(buffer[0] == '\n') {
			return 0;
		}
		
	} while(buffer[0] != 'y' && buffer[0] != 'n');
	
	return buffer[0] == 'y';
}

/*-----------------------------------------------------------------------------
// Name: display_menu(unsigned int n, const char *prompt, ...)
// Note: returns (unsigned int)-1 on error
//---------------------------------------------------------------------------*/
unsigned int display_menu(unsigned int n, const char *prompt, ...) {
	
	unsigned int user_selection;
	char buffer[256];
	
	assert(prompt != 0);
	
	do {
		va_list			var_arg;
		unsigned int	count;

		/* print the menu */
		va_start(var_arg, prompt);

		printf("%s\n", prompt);

		for(count = 0; count < n; ++count) {
			printf("\t%u - %s\n", count, va_arg(var_arg, const char *));
		}

		va_end(var_arg);
		
		if(fgets(buffer, sizeof(buffer), stdin) == 0) {
			return (unsigned int)-1;
		}
		
		/* read the user selection */
		if(sscanf(buffer, "%u", &user_selection) == EOF) {
			continue;
		}
	} while(user_selection >= n);
	
	return user_selection;
}

