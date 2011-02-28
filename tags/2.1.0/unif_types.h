
#ifndef UNIF_TYPES_20080314_H_
#define UNIF_TYPES_20080314_H_

#ifdef __cplusplus
extern "C"{
#endif

#if defined(WIN32) || defined(WIN64)
  typedef unsigned __int8	uint8_t;
  typedef unsigned __int16	uint16_t;
  typedef unsigned __int32	uint32_t;
  typedef unsigned __int64	uint64_t;
  
  typedef __int8			int8_t;
  typedef __int16			int16_t;
  typedef __int32			int32_t;
  typedef __int64			int64_t;

#elif defined (__BEOS__)
	#include <support/SupportDefs.h>
	typedef int8	int8_t;
	typedef int16	int16_t;
	typedef int32	int32_t;
	typedef int64	int64_t;

	typedef uint8	uint8_t;
	typedef uint16	uint16_t;
	typedef uint32	uint32_t;
	typedef uint64	uint64_t;
#elif defined (Linux)
	#include <stdint.h>
#elif defined(SunOS)
	#include <sys/types.h>
#endif
	
/* enum for return info */
typedef enum {
	UNIF_OK,
	UNIF_END_OF_FILE,
	UNIF_BAD_HEADER,
	UNIF_BAD_REVISION_NUMBER,
	UNIF_DIRTY_HEADER,
	UNIF_OPEN_FAILED,
	UNIF_CLOSE_FAILED,
	UNIF_READ_FAILED,
	UNIF_WRITE_FAILED,
	UNIF_OUT_OF_MEMORY,
	UNIF_INPUT_FAIL
} UNIF_RETURN_CODE;

/* enum for open type */
typedef enum {
	UNIF_OPEN_READ,
	UNIF_OPEN_WRITE
} UNIF_OPEN_MODE;
 
#ifdef __cplusplus
}
#endif

#endif

