#ifndef  _DROBOT_EFUSE_
#include <stdint.h>
#include <stdbool.h>

enum efuse_type {
	EFUSE_SECURE = 0,
	EFUSE_NONSECURE
};

struct efuse_info {
    enum efuse_type type;
    uint32_t bank;
    uint32_t value;
    bool lock;
};
int drobot_efuse_write(struct efuse_info *efuse);
int drobot_efuse_read(struct efuse_info *efuse);
#endif
