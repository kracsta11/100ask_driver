#include <linux/gfp.h>
#include "led_opr.h"

static int board_demo_led_init(int which)
{
    printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

static int board_demo_led_ctl(int which, char status)
{
    printk("%s %s: line%d, %s\n", __FILE__, __FUNCTION__, __LINE__, status ? "on" : "off");
    return 0;
}

static LED_OPR imx6ul_led_opr = {
    .init = board_demo_led_init,
    .ctl  = board_demo_led_ctl,
};

// PLED_OPR get_board_led_opr(void){
//     return &imx6ul_led_opr;
// }
struct led_operations *get_board_led_opr(void){
    return &imx6ul_led_opr;
}