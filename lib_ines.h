
#ifndef LIB_INES_20080314_H_
#define LIB_INES_20080314_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "unif_types.h"
#include <stdio.h>

/* Layout of First Sixteen Bytes of NES Cartridge in iNES format*/
typedef struct {
	char		ines_signature[4];	/* 0x1A53454E (NES file signature) */
	uint8_t		prg_size;			/* in 32k banks */
	uint8_t		chr_size;			/* in 8k banks */
	uint8_t		ctrl1;				/* %####vTsM */
	uint8_t		ctrl2;				/* %####0000 */
	uint32_t	reserved;			/* 0x00000000 */
	uint32_t	final_signature;	/* 0x00000000 */
} ines_header_t;

/* Flags in NES_HEADER.ctrl1 */
#define INES_MIRROR			0x01
#define INES_SRAM			0x02
#define INES_TRAINER		0x04
#define INES_4SCREEN		0x08

/* function prototypes */
UNIF_RETURN_CODE open_INES(const char *filename, FILE **file, UNIF_OPEN_MODE mode);
UNIF_RETURN_CODE close_INES(FILE *file);
UNIF_RETURN_CODE read_header_INES(FILE *file, ines_header_t *header);
UNIF_RETURN_CODE write_header_INES(FILE *file, const ines_header_t *header);
UNIF_RETURN_CODE check_header_INES(const ines_header_t *header);
UNIF_RETURN_CODE read_data_INES(FILE *file, uint8_t **dst, size_t len);

#ifdef __cplusplus
}
#endif

#endif

