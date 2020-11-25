#ifndef UTILITIES_TEST_EXTLIB_EASYFLASH_TEST_HPP
#define UTILITIES_TEST_EXTLIB_EASYFLASH_TEST_HPP
#include "easyFlash.h"

void EF_BasicTest(void)
{
    static char efTestStr[] = "This is EasyFlash test string.\n Lets see how it works!\n";
    uint32_t len = 0U;
    uint32_t result = 0U;

    ef_get_env_blob("efTestStr", nullptr, 0U, &len);
    printf("length is %ld\n", len);
    if(0U == len)
    {
        printf("Not present. Creating...\n", len);
        len = strlen(efTestStr) + 1U;
        result = ef_set_env_blob("efTestStr", efTestStr, len);
        printf("result is %ld\n", result);
        len = 0U;
        ef_get_env_blob("efTestStr", nullptr, 0U, &len);
        printf("Write Complete. length is %ld\n", len);
        result = ef_del_env("efTestStr");
        printf("Delete result is %ld\n", result);
    }

    printf("%ld\n", len);
}


#endif // ! UTILITIES_TEST_EXTLIB_EASYFLASH_TEST_HPP
