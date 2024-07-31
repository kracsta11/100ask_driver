#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <asm/io.h>
#include "led_opr.h"
#include "led_resource.h"

static volatile unsigned int *CCM_CCGR1;
static volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
static volatile unsigned int *GPIO5_GDIR;
static volatile unsigned int *GPIO5_DR;

static PLED_RESOURCE  led_rsc;

static int board_demo_led_init(led_rsc)
{
    unsigned int val;
    if(!led_rsc){
        led_rsc = get_led_resource();
    }
    
    return 0;
}

static int board_demo_led_ctl(int which, char status)
{
    
    return 0;
}

static LED_OPR imx6ul_led_opr = {
    .num  = 1,
    .init = board_demo_led_init,
    .ctl  = board_demo_led_ctl,
};

// PLED_OPR get_board_led_opr(void){
//     return &imx6ul_led_opr;
// }
struct led_operations *get_board_led_opr(void){
    return &imx6ul_led_opr;
}