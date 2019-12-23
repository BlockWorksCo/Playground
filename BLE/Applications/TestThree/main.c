
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_gpio.h"



/**@brief Application main function.
 */
int main(void)
{
    nrf_gpio_cfg_output(1);

    // Enter main loop.
    for (;;)
    {
        nrf_gpio_pin_set(1);
        nrf_gpio_pin_clear(1);
    }
}

