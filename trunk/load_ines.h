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

#ifndef LOAD_INES_20080314_H_
#define LOAD_INES_20080314_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "lib_ines.h"

/* description of a ines file */
typedef struct {
	ines_header_t	*header;
	uint8_t			*trainer;
	uint8_t			*prg_rom;
	uint8_t			*chr_rom;
	uint8_t			version;
} ines_cart_t;

typedef enum {
	MIRR_HORIZONTAL,
	MIRR_VERTICAL,
	MIRR_4SCREEN
} INES_MIRRORING;

/* some example implementations for loading and freeing a INES file */
UNIF_RETURN_CODE load_file_INES(const char *filename, ines_cart_t *cart);
UNIF_RETURN_CODE free_file_INES(ines_cart_t *cart);
UNIF_RETURN_CODE load_ptr_INES(const uint8_t *rom, size_t size, ines_cart_t *cart);
UNIF_RETURN_CODE write_file_INES(const char *filename, const ines_cart_t *cart);

/* API access to iNES data, works with version 2.0 ROMs as well */
INES_MIRRORING mirroring_INES(const ines_cart_t *cart);
uint32_t mapper_INES(const ines_cart_t *cart);
uint32_t submapper_INES(const ines_cart_t *cart);
uint32_t prg_size_INES(const ines_cart_t *cart);
uint32_t chr_size_INES(const ines_cart_t *cart);

#ifdef __cplusplus
}
#endif

#endif

