
#ifdef _MSC_VER
#pragma warning( disable : 4127 ) 
#endif

#include "load_unif.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>


#define SAFE_FREE(x) do { free(x); (x) = 0; } while(0)

/*-----------------------------------------------------------------------------
// name: load_file_UNIF(const char *filename, unif_cart_t *cart)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE load_file_UNIF(const char *filename, unif_cart_t *cart) {
	FILE *unif_file = 0;	
	UNIF_RETURN_CODE ret_val;
	
	assert(cart != 0);
	assert(filename != 0);
	
	/* clear out out structure we use to store file info */
	memset(cart, 0, sizeof(unif_cart_t));

	/* open the file */
	ret_val = open_UNIF(filename, &unif_file, UNIF_OPEN_READ);
	
	if(ret_val == UNIF_OK) {

		/* read the header */
		/* automaticaly sets file pointer to begining of file */
		ret_val = read_header_UNIF(unif_file, &cart->header);
		if(ret_val != UNIF_OK) {
			close_UNIF(unif_file);
			return ret_val;
		}

		/* do a check to see if the header is valid */
		ret_val = check_header_UNIF(&(cart->header));
		if(ret_val != UNIF_OK) {
			close_UNIF(unif_file);
			return ret_val;
		}

		/* as long as we dont have an error, proccess tags */
		while(ret_val == UNIF_OK) {
			unif_chunk_t chunk_hdr	= { "", 0 };
			
			union {
				void 			*ptr;
				uint8_t			*ptr8;
				uint32_t		*ptr32;
				char			*str;
			} chunk_data;
			
			ret_val = read_chunk_UNIF(unif_file, &chunk_hdr, &chunk_data.ptr);

			/* did we hit the end of the file? */
			if(ret_val == UNIF_END_OF_FILE)
				break;

			/* if the read was ok, store it */
			if(ret_val == UNIF_OK) {
			
				if(memcmp(chunk_hdr.u.chunk_id, "DINF", 4) == 0) {
					cart->dumper_info = chunk_data.ptr;
				} else if(memcmp(chunk_hdr.u.chunk_id, "MAPR", 4) == 0) {
					cart->mapr_name = chunk_data.str;
				} else if(memcmp(chunk_hdr.u.chunk_id, "READ", 4) == 0) {
					cart->read_text = chunk_data.str;
				} else if(memcmp(chunk_hdr.u.chunk_id, "NAME", 4) == 0) {
					cart->name = chunk_data.str;
				} else if(memcmp(chunk_hdr.u.chunk_id, "TVCI", 4) == 0) {
					cart->tvci_byte = chunk_data.ptr8;
				} else if(memcmp(chunk_hdr.u.chunk_id, "CTRL", 4) == 0) {
					cart->ctrl_byte = chunk_data.ptr8;
				} else if(memcmp(chunk_hdr.u.chunk_id, "BATR", 4) == 0) {
					cart->batr_byte = chunk_data.ptr8;
				} else if(memcmp(chunk_hdr.u.chunk_id, "VROR", 4) == 0) {
					cart->vror_byte = chunk_data.ptr8;
				} else if(memcmp(chunk_hdr.u.chunk_id, "MIRR", 4) == 0) {
					cart->mirr_data = chunk_data.ptr8;
				} else if(memcmp(chunk_hdr.u.chunk_id, "PRG0", 4) == 0) {
					cart->prg[0] = chunk_data.ptr8;
					cart->prg_pages[0] = (uint8_t)(chunk_hdr.length >> 14);
				} else if(memcmp(chunk_hdr.u.chunk_id, "CHR0", 4) == 0) {
					cart->chr[0] = chunk_data.ptr8;
					cart->chr_pages[0] = (uint8_t)(chunk_hdr.length >> 13);
				} else if(memcmp(chunk_hdr.u.chunk_id, "PCK0", 4) == 0) {
					cart->pck[0] = chunk_data.ptr32;
				} else if(memcmp(chunk_hdr.u.chunk_id, "CCK0", 4) == 0) {
					cart->cck[0] = chunk_data.ptr32;
				} else {
					/* since we aren't storing this pointer, free it */
					SAFE_FREE(chunk_data.ptr);
				}
			} else {
				fprintf(stderr, "%s\n", get_error_string_UNIF(ret_val));
				break;
			}
		}

		close_UNIF(unif_file);
	}

	return UNIF_OK;
}

/*-----------------------------------------------------------------------------
// name: free_file_UNIF(unif_cart_t *cart)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE free_file_UNIF(unif_cart_t *cart) {
	size_t i;
	
	assert(cart != 0);
	
	SAFE_FREE(cart->mapr_name);
	SAFE_FREE(cart->dumper_info);
	SAFE_FREE(cart->read_text);
	SAFE_FREE(cart->name);

	SAFE_FREE(cart->batr_byte);
	SAFE_FREE(cart->ctrl_byte);
	SAFE_FREE(cart->mirr_data);
	SAFE_FREE(cart->tvci_byte);
	SAFE_FREE(cart->vror_byte);

	for(i = 0; i < 0x10; i++) {
		SAFE_FREE(cart->prg[i]);
		SAFE_FREE(cart->pck[i]);
		SAFE_FREE(cart->chr[i]);		
		SAFE_FREE(cart->cck[i]);
	}
	
	return UNIF_OK;
}

