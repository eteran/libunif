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

/*-----------------------------------------------------------------------------
// write_file_INES(const char *filename, const ines_cart_t *cart)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE write_file_INES(const char *filename, const ines_cart_t *cart) {

	UNIF_RETURN_CODE retcode = UNIF_OK;
	FILE *file = 0;
	const int prg_size = prg_size_INES(cart) * 16384;
	const int chr_size = chr_size_INES(cart) * 8192;

	assert(filename != 0);
	assert(cart != 0);

	if((retcode = open_INES(filename, &file, UNIF_OPEN_WRITE)) != UNIF_OK) {
		return retcode;
	}

	if((retcode = write_header_INES(file, cart->header)) != UNIF_OK) {
		close_INES(file);
		return retcode;
	}

	if((cart->header->ctrl1 & INES_TRAINER) != 0) {
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
// load_ptr_INES(const uint8_t *rom, size_t size, ines_cart_t *cart)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE load_ptr_INES(const uint8_t *rom, size_t size, ines_cart_t *cart) {

    UNIF_RETURN_CODE retcode = UNIF_OK;
	uint8_t *file_data     = 0;
	const uint8_t *rom_ptr = 0;
    size_t prg_size        = 0;
    size_t chr_size        = 0;
    size_t trainer_size    = 0;
	ines_header_t header;

	assert(cart != 0);
	assert(rom != 0);
	
	if(size < sizeof(ines_header_t)) {
		return UNIF_LENGTH_ERROR;
	}
	
	memcpy(&header, rom, sizeof(ines_header_t));

	cart->header  = 0;
	cart->trainer = 0;
	cart->prg_rom = 0;
	cart->chr_rom = 0;
    cart->version = ((header.ctrl2 & 0xc) == 0x8) ? 2 : 1;
	
	retcode = check_header_INES(&header, cart->version);
	if((retcode != UNIF_OK) && (retcode != UNIF_DIRTY_HEADER)) {
		return retcode;
	}
	
	/* temporarily make the cart point to our read header */
	cart->header = &header;

	if((header.ctrl1 & INES_TRAINER) != 0) {
		trainer_size = 512;
	}
	
	prg_size = prg_size_INES(cart) * 16384;
	chr_size = chr_size_INES(cart) * 8192;
	
	/* NULL the cart header pointer, it was temporary */
	cart->header = 0;
	
	if(size < sizeof(ines_header_t) + trainer_size + prg_size + chr_size) {
		return UNIF_LENGTH_ERROR;
	}
	
	/* allocate memory for the cart */
	file_data = malloc(sizeof(ines_header_t) + trainer_size + prg_size + chr_size);
	if(file_data == 0) {
		return UNIF_OUT_OF_MEMORY;
	}
	
	/* assign some pointers */
	cart->header = (void *)file_data;
	if(trainer_size != 0) {
		cart->trainer = file_data + sizeof(ines_header_t);
		cart->prg_rom = file_data + sizeof(ines_header_t) + trainer_size;
		cart->chr_rom = file_data + sizeof(ines_header_t) + trainer_size + prg_size;
	} else {
		cart->trainer = 0;
		cart->prg_rom = file_data + sizeof(ines_header_t);
		cart->chr_rom = file_data + sizeof(ines_header_t) + prg_size;	
	}
	
	rom_ptr = rom;
	
	memcpy(cart->header, rom_ptr, sizeof(ines_header_t));
	rom_ptr += sizeof(ines_header_t);
	
	if(trainer_size) {
		memcpy(cart->trainer, rom_ptr, trainer_size);
		rom_ptr += trainer_size;
	}
	
	memcpy(cart->prg_rom, rom_ptr, prg_size);
	rom_ptr += prg_size;
	
	memcpy(cart->chr_rom, rom_ptr, chr_size);
	
    return retcode;
}

/*-----------------------------------------------------------------------------
// load_file_INES(const char *filename, ines_cart_t *cart)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE load_file_INES(const char *filename, ines_cart_t *cart) {

	UNIF_RETURN_CODE retcode = UNIF_OK;

	FILE *file            = 0;
	uint8_t *file_data    = 0;
	size_t trainer_size   = 0;
	size_t prg_size       = 0;
	size_t chr_size       = 0;
	ines_header_t header;

	assert(cart != 0);
	assert(filename != 0);

	cart->header  = 0;
	cart->trainer = 0;
	cart->prg_rom = 0;
	cart->chr_rom = 0;

	if((retcode = open_INES(filename, &file, UNIF_OPEN_READ)) != UNIF_OK) {
		return retcode;
	}

	if((retcode = read_header_INES(file, &header)) != UNIF_OK) {
		close_INES(file);
		return retcode;
	}

	cart->version = ((header.ctrl2 & 0xc) == 0x8) ? 2 : 1;

	retcode = check_header_INES(&header, cart->version);
	if((retcode != UNIF_OK) && (retcode != UNIF_DIRTY_HEADER)) {
		close_INES(file);
		return retcode;
	}
	
	/* temporarily make the cart point to our read header */
	cart->header = &header;

	if((header.ctrl1 & INES_TRAINER) != 0) {
		trainer_size = 512;
	}
	
	prg_size = prg_size_INES(cart) * 16384;
	chr_size = chr_size_INES(cart) * 8192;
	
	/* NULL the cart header pointer, it was temporary */
	cart->header = 0;
	
	/* allocate memory for the cart */
	file_data = malloc(sizeof(ines_header_t) + trainer_size + prg_size + chr_size);
	if(file_data == 0) {
		return UNIF_OUT_OF_MEMORY;
	}
	
	/* assign some pointers */
	cart->header = (void *)file_data;
	if(trainer_size != 0) {
		cart->trainer = file_data + sizeof(ines_header_t);
		cart->prg_rom = file_data + sizeof(ines_header_t) + trainer_size;
		cart->chr_rom = file_data + sizeof(ines_header_t) + trainer_size + prg_size;
	} else {
		cart->trainer = 0;
		cart->prg_rom = file_data + sizeof(ines_header_t);
		cart->chr_rom = file_data + sizeof(ines_header_t) + prg_size;	
	}

	memcpy(file_data, cart->header, sizeof(ines_header_t));
	if(trainer_size != 0) {
		retcode = read_data_INES(file, cart->trainer, trainer_size);
		if(retcode != UNIF_OK) {
			goto error;
		}
	}
	
	retcode = read_data_INES(file, cart->prg_rom, prg_size);
	if(retcode != UNIF_OK) {
		goto error;
	}
	
	retcode = read_data_INES(file, cart->chr_rom, chr_size);
	if(retcode != UNIF_OK) {
		goto error;
	}

	close_INES(file);
	return retcode;
	
error:

	cart->header  = 0;
	cart->trainer = 0;
	cart->prg_rom = 0;
	cart->chr_rom = 0;

	free(file_data);
	close_INES(file);
	return retcode;
}

/*-----------------------------------------------------------------------------
// free_file_INES(ines_cart_t *cart)
//---------------------------------------------------------------------------*/
UNIF_RETURN_CODE free_file_INES(ines_cart_t *cart) {
	assert(cart != 0);
	free(cart->header);
	return UNIF_OK;
}

/*-----------------------------------------------------------------------------
// mirroring_INES(const ines_cart_t *cart)
//---------------------------------------------------------------------------*/
INES_MIRRORING mirroring_INES(const ines_cart_t *cart) {
	switch(cart->header->ctrl1 & 0x09) {
	case 0x09:
	case 0x08:
		return MIRR_4SCREEN;
	case 0x01:
		return MIRR_VERTICAL;
	case 0x00:
	default:
		return MIRR_HORIZONTAL;
	}
}

/*-----------------------------------------------------------------------------
// mapper_INES(const ines_cart_t *cart)
//---------------------------------------------------------------------------*/
uint32_t mapper_INES(const ines_cart_t *cart) {

	switch(cart->version) {
	case 2:
		return (((uint32_t)(cart->header->extended.ines2.byte8 & 0x0f)) << 8) | (cart->header->ctrl1 >> 4) | (cart->header->ctrl2 & 0xf0);
	default:
		return (cart->header->ctrl1 >> 4) | (cart->header->ctrl2 & 0xf0);
	}
}

/*-----------------------------------------------------------------------------
// mapper_INES(const ines_cart_t *cart)
//---------------------------------------------------------------------------*/
uint32_t submapper_INES(const ines_cart_t *cart) {
	switch(cart->version) {
	case 2:
		return (cart->header->extended.ines2.byte8 & 0xf0) >> 4;
	default:
		return 0;
	}
}

/*-----------------------------------------------------------------------------
// prg_size_INES(const ines_cart_t *cart)
//---------------------------------------------------------------------------*/
uint32_t prg_size_INES(const ines_cart_t *cart) {
	switch(cart->version) {
	case 2:
		return cart->header->prg_size | (((uint32_t)cart->header->extended.ines2.byte9 & 0x0f) << 8);
	default:
		return cart->header->prg_size;
	}
}

/*-----------------------------------------------------------------------------
// chr_size_INES(const ines_cart_t *cart)
//---------------------------------------------------------------------------*/
uint32_t chr_size_INES(const ines_cart_t *cart) {
	switch(cart->version) {
	case 2:
		return cart->header->chr_size | (((uint32_t)cart->header->extended.ines2.byte9 & 0xf0) << 4);
	default:
		return cart->header->chr_size;
	}
}
