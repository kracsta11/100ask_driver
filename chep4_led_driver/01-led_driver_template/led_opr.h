#ifndef _LED_OPR_H
#define _LED_OPR_H

typedef struct led_operations {
    int (*init) (int which);                   /* 初始化哪一个led */
    int (*ctl) (int which, char status);        /* led控制函数，which-哪一个led，status-灯的状态，0：暗，1：亮 */
} LED_OPR;

typedef LED_OPR *PLED_OPR;

PLED_OPR get_board_led_opr(void);

#endif 