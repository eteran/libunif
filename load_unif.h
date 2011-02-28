
#ifndef LOAD_UNIF_20080314_H_
#define LOAD_UNIF_20080314_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "lib_unif.h"

typedef struct {
	uint32_t		*pck[0x10];
	uint32_t		*cck[0x10];
	uint8_t			*prg[0x10];
	uint8_t			*chr[0x10];
	uint8_t			prg_pages[0x10];
	uint8_t			chr_pages[0x10];
	unif_header_t	header;
	dumper_info_t	*dumper_info;
	char			*mapr_name;
	char			*read_text;
	char			*name;
	uint8_t			*tvci_byte;
	uint8_t			*ctrl_byte;
	uint8_t			*batr_byte;
	uint8_t			*vror_byte;
	uint8_t			*mirr_data;	
} unif_cart_t;

/* some example implementations for loading and freeing a UNIF file */
UNIF_RETURN_CODE load_file_UNIF(const char *filename, unif_cart_t *cart);
UNIF_RETURN_CODE free_file_UNIF(unif_cart_t *cart);

#ifdef __cplusplus
}
#endif

#endif


