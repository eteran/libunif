
#ifndef STD_FUNC_20080314_H_
#define STD_FUNC_20080314_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stddef.h>	/* for size_t */

/* some general purpose functions to trim code */
size_t display_menu(unsigned int n_values, const char *prompt, ...);
int ask_question_yn(const char *query);

#ifdef __cplusplus
}
#endif

#endif

