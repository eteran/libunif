
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
		if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
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
// Name: display_menu(unsigned int n_values, const char *prompt, ...)
//---------------------------------------------------------------------------*/
size_t display_menu(unsigned int n_values, const char *prompt, ...) {
	
	unsigned int user_selection;
	
	assert(prompt != 0);
	
	do {
		va_list			var_arg;
		unsigned int	count;
		char			trash_data;

		/* print the menu */
		va_start(var_arg, prompt);

		printf("%s\n", prompt);

		for(count = 0; count < n_values; count++) {
			printf("\t%u - %s\n", count, va_arg(var_arg, const char *));
		}

		va_end(var_arg);
		
		/* read the user selection */
		scanf("%u", &user_selection);

		/* get rid of the return */
		scanf("%c", &trash_data);
	} while(user_selection >= n_values);
	
	return user_selection;
}

