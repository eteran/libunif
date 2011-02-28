
#ifndef LOAD_INES_20080314_H_
#define LOAD_INES_20080314_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "lib_ines.h"

/* description of a ines file */
typedef struct {
	ines_header_t	header;
	uint8_t			*trainer;
	uint8_t			*prg_rom;
	uint8_t			*chr_rom;
} ines_cart_t;

/* some example implementations for loading and freeing a INES file */
UNIF_RETURN_CODE load_file_INES(const char *filename, ines_cart_t *cart);
UNIF_RETURN_CODE free_file_INES(ines_cart_t *cart);
UNIF_RETURN_CODE load_ptr_INES(const uint8_t *rom, ines_cart_t *cart);
UNIF_RETURN_CODE write_file_INES(const char *filename, const ines_cart_t *cart);

#ifdef __cplusplus
}
#endif

#endif

