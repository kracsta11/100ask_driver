#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include "led_opr.h"

/***************************************************************
 基础驱动编写流程：
 1. 确定主设备号
 2. 定义file_ops结构体
 3. 实现file_ops结构体的各个成员 如open/read/write等
 4. 注册驱动程序 即将file_ops结构体告知内核
 5. 编写入口程序
 6. 编写出口程序
 7. 完善 提供设备信息 自动创建设备节点
***************************************************************/

#define LED_NUM  (2)

#define MIN(a,b) (a < b ? a : b)


/***************************************************************
  函数声明
 ***************************************************************/
static int      __LedOpen(struct inode *pnode, struct file *pfile);
static ssize_t  __LedRead(struct file *pfile, char __user * cbuf, size_t size, loff_t *poffset);
static ssize_t  __LedWrite(struct file *pfile, const char __user *cbuf, size_t size, loff_t *poffset);
static int      __LedClose(struct inode *pnode, struct file *pfile);

/*************************************************************** 
  全局变量定义
 ***************************************************************/
/*
 * 定义主设备号
 */
 static int _G_imajor = 0;
 /*
 * 定义file_ops结构体
 */
 static const struct file_operations led_fops = {
	.owner		= THIS_MODULE,
	.open		= __LedOpen,
	.read		= __LedRead,
    .write      = __LedWrite,
    .release    = __LedClose,
};
/*
 * 定义驱动数据缓存区
 */
 static char _G_ckbuf[BUF_SIZE];
/*
 * 定义一个类
 */
static struct class *_G_pled_class;

/*********************************************************************************************************
** 函数名称: __LedOpen
** 功能描述: 驱动打开
** 输　入  : pnode           节点指针
**           pfile           文件指针
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static int __LedOpen(struct inode *pnode, struct file *pfile)
{
    printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}
/*********************************************************************************************************
** 函数名称: __LedRead
** 功能描述: 驱动读
** 输　入  : pfile           文件指针
**           cbuf            来自用户层的数据缓存
**           size            
**           poffset         
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static ssize_t  __LedRead(struct file *pfile, char __user * cbuf, size_t size, loff_t *poffset)
{
    int iErr;
    printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
    return MIN(BUF_SIZE, size);
}
/*********************************************************************************************************
** 函数名称: __LedWrite
** 功能描述: 驱动写
** 输　入  : pfile           文件指针
**           cbuf            来自用户层的数据缓存
**           size            
**           poffset         
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static ssize_t  __LedWrite(struct file *pfile, const char __user *cbuf, size_t size, loff_t *poffset)
{
    int iErr;
    char cStatus;
    iErr = copy_from_user(&cStatus, cbuf, 1);
    
    /* 根据次设备号和status控制led */

    return 1;
}
/*********************************************************************************************************
** 函数名称: __LedClose
** 功能描述: 驱动关闭
** 输　入  : pnode           节点指针
**           pfile           文件指针
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static int      __LedClose(struct inode *pnode, struct file *pfile)
{
    printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

/*********************************************************************************************************
** 函数名称: ledInit
** 功能描述: 驱动初始化
** 输　入  : pnode           节点指针
**           pfile           文件指针
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
int ledInit(void)
{
    int iErr;
    int i;
    printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
    _G_imajor = register_chrdev(0, "my_led", &led_fops);

    _G_pled_class = class_create(THIS_MODULE, "led_class");
	if (IS_ERR(_G_pled_class)){
        printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
        unregister_chrdev(_G_imajor, "my_led");
        return -1;
    }
    for(i = 0; i < LED_NUM; i++){
        device_create(_G_pled_class, NULL, MKDEV(_G_imajor, i), NULL, "my_led%d", i);
    }
    
    return 0;
}

/*********************************************************************************************************
** 函数名称: ledExit
** 功能描述: 驱动注销
** 输　入  : pnode           节点指针
**           pfile           文件指针
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
void ledExit(void)
{
    int i;
    printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
    fir(i = 0; i < LED_NUM; i++){
        device_destroy(_G_pled_class, MKDEV(_G_imajor, i));
    }
    class_destroy(_G_pled_class);
    unregister_chrdev(_G_imajor, "my_led");

    return 0;
}

module_init(ledInit);
module_exit(ledExit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("led Driver");
MODULE_AUTHOR("Pan Yang");