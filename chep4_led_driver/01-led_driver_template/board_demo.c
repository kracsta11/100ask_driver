#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <asm/io.h>
#include "led_opr.h"

static volatile unsigned int *CCM_CCGR1;
static volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
static volatile unsigned int *GPIO5_GDIR;
static volatile unsigned int *GPIO5_DR;

static int board_demo_led_init(int which)
{
    unsigned int val;
    if(which == 0){
        if(!CCM_CCGR1){
            CCM_CCGR1 = ioremap(0x20C406C, 4);
            IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x2290014, 4);
            GPIO5_GDIR = ioremap(0x020AC000 + 0x4, 4);
            GPIO5_DR = ioremap(0x020AC000, 4);
        }
       /* GPIO5_IO03 */
       /* a. 使能 GPIO5
        * set CCM to enable GPIO5
        * CCM_CCGR1[CG15] 0x20C406C
        * bit[31:30] = 0b11
        */
       printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
        *CCM_CCGR1 |= (3 << 30); 

       /* b. 设置 GPIO5_IO03 用于 GPIO
        * set IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3
        * to configure GPIO5_IO03 as GPIO
        * IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 0x2290014
        * bit[3:0] = 0b0101 alt5
        */
       printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
		val = *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
		val &= ~(0xf);
		val |= (5);
		*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = val;

       /* c. 设置 GPIO5_IO03 作为 output 引脚
        * set GPIO5_GDIR to configure GPIO5_IO03 as output
        * GPIO5_GDIR 0x020AC000 + 0x4
        * bit[3] = 0b1
        */
       printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
        *GPIO5_GDIR |= (1 << 3);
    }
    return 0;
}

static int board_demo_led_ctl(int which, char status)
{
    printk("%s %s: line%d, %s\n", __FILE__, __FUNCTION__, __LINE__, status ? "on" : "off");
        if(status){
            /* d. 设置 GPIO5_DR 输出低电平
            * set GPIO5_DR to configure GPIO5_IO03 output 0
            * GPIO5_DR 0x020AC000 + 0
            * bit[3] = 0b0
            */  
            printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
            *GPIO5_DR &= ~(1 << 3);
        } else {
            /* e. 设置 GPIO5_IO3 输出高电平
            * set GPIO5_DR to configure GPIO5_IO03 output 1
            * GPIO5_DR 0x020AC000 + 0
            * bit[3] = 0b1
            */
            printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
            *GPIO5_DR |= (1 << 3);  
        }
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