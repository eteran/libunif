
#ifdef _MSC_VER
#pragma warning( disable : 4127 ) 
#endif

#include "lib_ines.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define SAFE_FREE(x) do { free(x); (x) = 0; } while(0)

/*-----------------------------------------------------------------------------
// open_INES(const char *filename, FILE **file, UNIF_OPEN_MODE mode)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE open_INES(const char *filename, FILE **file, UNIF_OPEN_MODE mode) {

	/* check pointers */
	assert(filename != 0);
	assert(file != 0);
	
	/* open the file */
	if(mode == UNIF_OPEN_READ)	*file = fopen(filename, "rb");
	else						*file = fopen(filename, "wb");
		
	/* check if successful */
	if(*file == 0) {
		return UNIF_OPEN_FAILED;
	}

	return UNIF_OK;
}

/*-----------------------------------------------------------------------------
// close_INES(FILE *file)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE close_INES(FILE *file) {
	
	/* check pointers */
	assert(file != 0);

	/* close file */
	if(fclose(file) == EOF) {
		return UNIF_CLOSE_FAILED;
	}

	return UNIF_OK;
}

/*-----------------------------------------------------------------------------
// read_header_INES(FILE *file, ines_header_t *header)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE read_header_INES(FILE *file, ines_header_t *header) {
	
	/* check pointers */
	assert(file != 0);
	assert(header != 0);
	
	/* header is ALWAYS at begining of file */
	rewind(file);

	/* read the header data */
	if(fread(header, 1, sizeof(ines_header_t), file) != sizeof(ines_header_t)) {
		return UNIF_READ_FAILED;
	}

	return UNIF_OK;
}

/*-----------------------------------------------------------------------------
// write_header_INES(FILE *file, const ines_header_t *header)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE write_header_INES(FILE *file, const ines_header_t *header) {
	/* check pointers */
	assert(file != 0);
	assert(header != 0);

	/* header is ALWAYS at begining of file */
	rewind(file);

	/* write the header data */
	if(fwrite(header, 1, sizeof(ines_header_t), file) != sizeof(ines_header_t)) {
		return UNIF_WRITE_FAILED;
	}

	return UNIF_OK;
}

/*-----------------------------------------------------------------------------
// check_header_INES(const ines_header_t *header)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE check_header_INES(const ines_header_t *header) {

	assert(header != 0);

	if(memcmp(header->ines_signature, "NES\x1a", 4) != 0) {
		return UNIF_BAD_HEADER;
	}

	if(header->final_signature != 0) {
		return UNIF_DIRTY_HEADER;
	}

	return UNIF_OK;
}

/*-----------------------------------------------------------------------------
// read_data_INES( FILE * file, uint8_t **dst, size_t len )
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE read_data_INES(FILE *file, uint8_t **dst, size_t len) {
	assert(file != 0);
	assert(dst != 0);

	*dst = malloc(len);

	if(*dst == 0) {
		return UNIF_OUT_OF_MEMORY;
	}

	if(fread(*dst, 1, len, file) != len) {
		SAFE_FREE(*dst);
		return UNIF_READ_FAILED;
	}

	return UNIF_OK;
}
