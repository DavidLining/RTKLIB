#include "vendor_storage_rw.h"

#include <fcntl.h>
#include <sys/ioctl.h>

typedef		unsigned short	    uint16;
typedef		unsigned long	    uint32;
typedef		unsigned char	    uint8;

#define VENDOR_REQ_TAG		0x56524551
#define VENDOR_READ_IO		_IOW('v', 0x01, unsigned int)
#define VENDOR_WRITE_IO		_IOW('v', 0x02, unsigned int)

#define VENDOR_RTK_INFO_ID  17
#define VENDOR_DATA_SIZE (3 * 1024) 
#define VERDOR_DEVICE "/dev/vendor_storage"

typedef struct _RK_VERDOR_REQ {
    uint32_t tag;
    uint16_t id;
    uint16_t len;
    uint8_t data[VENDOR_DATA_SIZE];
} RK_VERDOR_REQ;

static rtk_param rtk_parameter;

int parameter_vendor_write(int buffer_size, uint8_t* buffer, uint16_t vendor_id)
{
    int ret;
    RK_VERDOR_REQ req;

    int sys_fd = open(VERDOR_DEVICE, O_RDWR, 0);
    if (sys_fd < 0) {
        printf("vendor_storage open fail\n");
        return -1;
    }

    req.tag = VENDOR_REQ_TAG;
    req.id = vendor_id;
    req.len = buffer_size > VENDOR_DATA_SIZE ? VENDOR_DATA_SIZE : buffer_size;
    memcpy(req.data, buffer, req.len);
    ret = ioctl(sys_fd, VENDOR_WRITE_IO, &req);
    if (ret) {
        printf("vendor write error\n");
        close(sys_fd);
        return -1;
    }

    close(sys_fd);

    return 0;
}

int parameter_vendor_read(int buffer_size, uint8_t* buffer, uint16_t vendor_id)
{
    int ret;
    RK_VERDOR_REQ req;

    int sys_fd = open(VERDOR_DEVICE, O_RDWR, 0);
    if (sys_fd < 0) {
        printf("vendor_storage open fail\n");
        return -2;
    }

    req.tag = VENDOR_REQ_TAG;
    req.id = vendor_id;
    /* max read length to read */
    req.len = buffer_size > VENDOR_DATA_SIZE ? VENDOR_DATA_SIZE : buffer_size;

    ret = ioctl(sys_fd, VENDOR_READ_IO, &req);
    /* return req->len is the real data length stored in the NV-storage */
    if (ret) {
        /*  printf("vendor read error ret = %d\n", ret); */
        close(sys_fd);
        return -1;
    }
    memcpy(buffer, req.data, req.len);
    /* rknand_print_hex_data("vendor read:", (uint32_t *)(& req), req.len + 8); */
    close(sys_fd);

    return 0;
}

int parameter_rtk_save(uint8_t* pdata)
{
	int ret = 0;
    ret = parameter_vendor_write(sizeof(rtk_param),
                                (uint8_t*)pdata, VENDOR_RTK_INFO_ID);
    if(ret == 0)
    {
        memcpy((uint8_t*)&rtk_parameter, pdata, sizeof(rtk_param));  
    }
    return ret;
}

int parameter_rtk_read(uint8_t* parameter)
{
	int ret = 0;
    ret = parameter_vendor_read(sizeof(rtk_param), parameter, VENDOR_RTK_INFO_ID);
	return ret;
}
