#ifndef _LED_RESOURCE_H
#define _LED_RESOURCE_H

/* GPIO5_IO03 */
/* bit[31:16] = group 
 * bit[15:0]  = pin
 * 高16位表示属于哪一组GPIO
 * 低16位表示该组中那一个引脚
*/
#define GROUP(x)       (x >> 16)
#define PIN(x)         (x & 0xFFFF)
#define GROUP_PIN(g,p) ((g << 16) | (p))

typedef struct led_resource {
    unsigned int pin;
} LED_RESOURCE;

typedef LED_RESOURCE *PLED_RESOURCE;

PLED_RESOURCE get_led_resource(void);

#endif 