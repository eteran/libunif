/*
Copyright (C) 2000 - 2011 Evan Teran
                          eteran@alum.rit.edu
				   
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "load_ines.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>


/* a macro that will free and null a pointer */
#define SAFE_FREE(x) do { free(x); (x) = 0; } while(0)

/*-----------------------------------------------------------------------------
// write_file_INES(const char *filename, const ines_cart_t *cart)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE write_file_INES(const char *filename, const ines_cart_t *cart) {
	
	UNIF_RETURN_CODE retcode = UNIF_OK;
	FILE *file = 0;
	const int prg_size = ((cart->header).prg_size * 16384);
	const int chr_size = ((cart->header).chr_size * 8192);
	
	assert(filename != 0);
	assert(cart != 0);
	
	if((retcode = open_INES(filename, &file, UNIF_OPEN_WRITE)) != UNIF_OK) {
		return retcode;
	}
	
	if((retcode = write_header_INES(file, &cart->header)) != UNIF_OK) {
		close_INES(file);
		return retcode;
	}
	
	if(((cart->header).ctrl1 & INES_TRAINER) != 0) {
		if(fwrite(cart->trainer, 512, 1, file) != 1) {
			close_INES(file);
			return UNIF_WRITE_FAILED;
		}
	}
	
	if(prg_size > 0) {
		if(fwrite(cart->prg_rom, prg_size, 1, file) != 1) {
			close_INES(file);
			return UNIF_WRITE_FAILED;
		}
	}
	
	if(chr_size > 0) {
		if(fwrite(cart->chr_rom, chr_size, 1, file) != 1) {
			close_INES(file);
			return UNIF_WRITE_FAILED;
		}
	}
	
	close_INES(file);
	return retcode;
}

/*-----------------------------------------------------------------------------
// load_ptr_INES(const uint8_t *rom, ines_cart_t *cart)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE load_ptr_INES(const uint8_t *rom, ines_cart_t *cart) {

    UNIF_RETURN_CODE retcode = UNIF_OK;
    int prg_size = 0;
    int chr_size = 0;
    int trainer_size = 0;
    
	assert(cart != 0);
	assert(rom != 0);
           
    memcpy(&cart->header, rom, sizeof(ines_header_t));
	cart->trainer = 0;
	cart->prg_rom = 0;
	cart->chr_rom = 0;
    cart->version = ((cart->header.ctrl2 & 0xc) == 0x8) ? 2 : 1;

	if((retcode = check_header_INES(&cart->header, cart->version)) == UNIF_OK) {
        
    	if((cart->header).ctrl1 & INES_TRAINER) {
        	trainer_size = 512;
    	}

    	prg_size = ((cart->header).prg_size * 16384);
    	chr_size = ((cart->header).chr_size * 8192);

    	rom += sizeof(ines_header_t);

		if(((cart->header).ctrl1 & INES_TRAINER) != 0) {
        	if(((cart->trainer) = malloc(trainer_size)) == 0) {
            	return UNIF_OUT_OF_MEMORY;
        	}
        	memcpy(cart->trainer, rom, trainer_size);
		} 

    	rom += trainer_size;

    	if(((cart->prg_rom) = malloc(prg_size)) == 0) {
	    	SAFE_FREE(cart->trainer);    
        	return UNIF_OUT_OF_MEMORY;
    	}
    	memcpy(cart->prg_rom, rom, prg_size);

    	rom += prg_size;

    	if(chr_size != 0) {
        	if(((cart->chr_rom) = malloc(chr_size)) == 0) {
        		SAFE_FREE(cart->trainer);
        		SAFE_FREE(cart->prg_rom);
            	return UNIF_OUT_OF_MEMORY;
        	}
        	memcpy(cart->chr_rom, rom, chr_size);    
    	}
	}
            
    return retcode;
    
}

/*-----------------------------------------------------------------------------
// load_file_INES(const char *filename, ines_cart_t *cart)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE load_file_INES(const char *filename, ines_cart_t *cart) {
	
	UNIF_RETURN_CODE retcode = UNIF_OK;
	FILE *file = 0;

	assert(cart != 0);
	assert(filename != 0);
	
	memset(&cart->header, 0, sizeof(ines_header_t));
	cart->trainer = 0;
	cart->prg_rom = 0;
	cart->chr_rom = 0;
	
	if((retcode = open_INES(filename, &file, UNIF_OPEN_READ)) != UNIF_OK) {
		return retcode;
	}

	if((retcode = read_header_INES(file, &cart->header)) != UNIF_OK) {
		close_INES(file);
		return retcode;
	}
	
	cart->version = ((cart->header.ctrl2 & 0xc) == 0x8) ? 2 : 1;

	retcode = check_header_INES(&cart->header, cart->version);
	if((retcode != UNIF_OK) && (retcode != UNIF_DIRTY_HEADER)) {
		close_INES(file);
		return retcode;
	}

	if(((cart->header).ctrl1 & INES_TRAINER) != 0) {
		retcode = read_data_INES(file, &cart->trainer, 512);
		if(retcode != UNIF_OK) {
			close_INES(file);
			return retcode;
		}
	}

	retcode = read_data_INES(file, &cart->prg_rom, ((cart->header).prg_size * 16384));
	if(retcode != UNIF_OK) {
		SAFE_FREE(cart->trainer);
		close_INES(file);
		return retcode;
	}

	if(cart->header.chr_size != 0) {
		retcode = read_data_INES(file, &cart->chr_rom, ((cart->header).chr_size * 8192));
		if(retcode != UNIF_OK) {
			SAFE_FREE(cart->trainer);
			SAFE_FREE(cart->prg_rom);
			close_INES(file);
			return retcode;
		}
	}
	
	close_INES(file);
	return retcode;
}

/*-----------------------------------------------------------------------------
// free_file_INES(ines_cart_t *cart)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE free_file_INES(ines_cart_t *cart) {
	assert(cart != 0);

	SAFE_FREE(cart->trainer);
	SAFE_FREE(cart->prg_rom);
	SAFE_FREE(cart->chr_rom);
	return UNIF_OK;
}

/*-----------------------------------------------------------------------------
// mirroring_INES(const ines_cart_t *cart)
//---------------------------------------------------------------------------*/
INES_MIRRORING mirroring_INES(const ines_cart_t *cart) {
	switch(cart->header.ctrl1 & 0x09) {
	case 0x08:
	case 0x09:
		return MIRR_4SCREEN;
	case 0x00:
		return MIRR_HORIZONTAL;
	case 0x01:
		return MIRR_VERTICAL;
	}
}
