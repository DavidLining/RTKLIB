#include <stdint.h>

typedef struct _rtk_param
{
	uint32_t state	: 8;
	uint32_t base_alt	: 24;	
	uint32_t base_lat;
	uint32_t base_lon;
}rtk_param;

int parameter_rtk_read(uint8_t* parameter);