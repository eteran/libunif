
#include "ines_convert.h"
#include "load_unif.h"

#include <stdlib.h>

/*-----------------------------------------------------------------------------
// Name: main(int argc, char *argv[] )
//---------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
	unif_cart_t dta;
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

	if(load_file_UNIF(dest, &dta) != UNIF_OK) {
		fprintf(stderr, "error loading UNIF file %s\n", dest);
		return -1;
	}
	
	/* test getting info */
	get_ines_mapper(dta.mapr_name, &ines);
	free_file_UNIF(&dta);
	return 0;
}


