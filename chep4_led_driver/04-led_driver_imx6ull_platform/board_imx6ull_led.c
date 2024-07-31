#include "led_resource.h"

static LED_RESOURCE board_imx6ull_led = {
    .pin = GROUP_PIN(5,3)
};

PLED_RESOURCE get_led_resource(void)
{
    return &board_imx6ull_led;
}