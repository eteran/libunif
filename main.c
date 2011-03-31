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

#include "ines_convert.h"
#include "load_unif.h"

#include <stdlib.h>

/*-----------------------------------------------------------------------------
// Name: main(int argc, char *argv[] )
//---------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
	unif_cart_t unif;
	ines_info_t ines;
	
	const char *source;
	const char *dest;
	
	if(argc != 3) {
		fprintf(stderr, "usage: lib_unif <source.nes> <dest.unf>\n");
		return -1;
	}
	
	source	= argv[1];
	dest	= argv[2];

	make_unif_file_from_nes(dest, source);

	/* test loading of the file */
	if(load_file_UNIF(dest, &unif) != UNIF_OK) {
		fprintf(stderr, "error loading UNIF file %s\n", dest);
		return -1;
	}
	
	/* test getting info */
	get_ines_mapper(unif.mapr_name, &ines);
	free_file_UNIF(&unif);
	
	return 0;
}


