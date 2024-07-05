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

#define BUF_SIZE (1024)
#define MIN(a,b) (a < b ? a : b)


/***************************************************************
  函数声明
 ***************************************************************/
static int      __HelloOpen(struct inode *pnode, struct file *pfile);
static ssize_t  __HelloRead(struct file *pfile, char __user * cbuf, size_t size, loff_t *poffset);
static ssize_t  __HelloWrite(struct file *pfile, const char __user *cbuf, size_t size, loff_t *poffset);
static int      __HelloClose(struct inode *pnode, struct file *pfile);

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
 static const struct file_operations hello_fops = {
	.owner		= THIS_MODULE,
	.open		= __HelloOpen,
	.read		= __HelloRead,
    .write      = __HelloWrite,
    .release    = __HelloClose,
};
/*
 * 定义驱动数据缓存区
 */
 static char _G_ckbuf[BUF_SIZE];
/*
 * 定义一个类
 */
static struct class *_G_phello_class;

/*********************************************************************************************************
** 函数名称: __HelloOpen
** 功能描述: 驱动打开
** 输　入  : pnode           节点指针
**           pfile           文件指针
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static int __HelloOpen(struct inode *pnode, struct file *pfile)
{
    printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}
/*********************************************************************************************************
** 函数名称: __HelloRead
** 功能描述: 驱动读
** 输　入  : pfile           文件指针
**           cbuf            来自用户层的数据缓存
**           size            
**           poffset         
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static ssize_t  __HelloRead(struct file *pfile, char __user * cbuf, size_t size, loff_t *poffset)
{
    printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
    copy_to_user(cbuf, _G_ckbuf, MIN(BUF_SIZE, size));
    return MIN(BUF_SIZE, size);
}
/*********************************************************************************************************
** 函数名称: __HelloWrite
** 功能描述: 驱动写
** 输　入  : pfile           文件指针
**           cbuf            来自用户层的数据缓存
**           size            
**           poffset         
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static ssize_t  __HelloWrite(struct file *pfile, const char __user *cbuf, size_t size, loff_t *poffset)
{
    printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
    copy_from_user(_G_ckbuf, cbuf, MIN(BUF_SIZE, size));
    return MIN(BUF_SIZE, size);
}
/*********************************************************************************************************
** 函数名称: __HelloClose
** 功能描述: 驱动关闭
** 输　入  : pnode           节点指针
**           pfile           文件指针
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static int      __HelloClose(struct inode *pnode, struct file *pfile)
{
    printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

/*********************************************************************************************************
** 函数名称: HelloInit
** 功能描述: 驱动初始化
** 输　入  : pnode           节点指针
**           pfile           文件指针
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
int HelloInit(void)
{
    printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
    _G_imajor = register_chrdev(0, "hello", &hello_fops);

    _G_phello_class = class_create(THIS_MODULE, "hello_class");
	if (IS_ERR(_G_phello_class)){
        printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
        unregister_chrdev(_G_imajor, "hello");
        return -1;
    }
    device_create(_G_phello_class, NULL, MKDEV(_G_imajor, 0), NULL, "hello");
    return 0;
}

/*********************************************************************************************************
** 函数名称: HelloExit
** 功能描述: 驱动注销
** 输　入  : pnode           节点指针
**           pfile           文件指针
** 输　出  : 
** 全局变量:
** 调用模块:
*********************************************************************************************************/
void HelloExit(void)
{
    printk("%s %s: line%d.\n", __FILE__, __FUNCTION__, __LINE__);
    device_destroy(_G_phello_class, MKDEV(_G_imajor, 0));
    class_destroy(_G_phello_class);
    unregister_chrdev(_G_imajor, "hello");

    return 0;
}

module_init(HelloInit);
module_exit(HelloExit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Driver");
MODULE_AUTHOR("Pan Yang");