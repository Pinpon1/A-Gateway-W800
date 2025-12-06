#include "geteway_include.h"


int main(void)
{
#if APP_CFG_TEST_RGB
    /* RGB测试任务 */
    test_rgb_init();
#endif

    while (1)
    {
        
    }
}
