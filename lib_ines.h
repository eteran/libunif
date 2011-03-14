
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
	uint8_t		ctrl1;				/* %####FTBM */
	uint8_t		ctrl2;				/* %####00PV */
	
	/* in iNES 2.0, these mean something, otherwise, should be 0 */
	union {
		struct ines2_t {
			uint8_t byte8;	/* %SSSSMMMM */
			uint8_t byte9;	/* %CCCCPPPP */
			uint8_t byte10;	/* %ppppPPPP */
			uint8_t byte11;	/* %ccccCCCC */
			uint8_t byte12;	/* %xxxxxxBP */
			uint8_t byte13;	/* %MMMMPPPP */
			uint8_t byte14;
			uint8_t byte15;	
		} ines2;
		
		struct ines1_t {
			uint32_t reserved_1;
			uint32_t reserved_2;
		} ines1;
	} extended;
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
UNIF_RETURN_CODE check_header_INES(const ines_header_t *header, int version);
UNIF_RETURN_CODE read_data_INES(FILE *file, uint8_t **dst, size_t len);

#ifdef __cplusplus
}
#endif

#endif
