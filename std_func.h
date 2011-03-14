
#ifndef STD_FUNC_20080314_H_
#define STD_FUNC_20080314_H_

#ifdef __cplusplus
extern "C"{
#endif

/* some general purpose functions to trim code */
unsigned int display_menu(unsigned int n, const char *prompt, ...);
int ask_question_yn(const char *query);

#ifdef __cplusplus
}
#endif

#endif

