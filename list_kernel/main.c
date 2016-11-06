/**
linux链表示例。演示创建、遍历、删除等操作。

注：内核list实现，似乎无法做到方便访问某个节点。比如查找符合某条件时在此节点前/后添加新节点。
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "list.h"

struct i2c_devinfo
{
	struct list_head list;
	int busnum;
    int slave;
};

// 定义链表
LIST_HEAD(my_list);

void init(int busnum, int slave)
{
    struct i2c_devinfo* devinfo;
    // 分配空间
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    if (devinfo == NULL) return;
    
    // 赋值
    devinfo->busnum = busnum;
    devinfo->slave = slave;
    
    // 添加到链接中
    //list_add_tail(&devinfo->list, &my_list); // 插入尾部
    list_add(&devinfo->list, &my_list); // 插入头部
}

void show(void)
{
    struct i2c_devinfo *devinfo;
    int i = 0;

    printf("show info in the list.\n");
    // 解释：从全局链表my_list中拿到devinfo信息，其中list为devinfo类型的一个成员变量
    list_for_each_entry(devinfo, &my_list, list)
    {
        printf("[%d] busnum: %d, slave: %d\n", i++, devinfo->busnum, devinfo->slave);
	}
}

void delete_list(void)
{
    struct i2c_devinfo *devinfo, *tdev;
    //　注：当要删除链表数据时，要使用*_safe，同时要额外增加一个临时变量
    list_for_each_entry_safe(devinfo, tdev, &my_list, list)
    {
        list_del(&devinfo->list);
        free(devinfo);
	}
}

void add_del(void)
{
    struct i2c_devinfo* devinfo, *tdev, *pdev;
    // 分配空间
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    if (devinfo == NULL) return;
    
    // 赋值
    devinfo->busnum = 250;
    devinfo->slave = 25;
    
    list_for_each_entry_safe(tdev, pdev, &my_list, list)
    {
        if (tdev->busnum == 1)
        {
            //list_move(&tdev->list, &devinfo->list);
            break;
        }
    }
}

int main(void)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < 5; i++, j += 2)
    {
        init(i, j);
    }
    
    show();
    
    //delete_list();
add_del();
    // 删除链表后，已经没有内容了
    show();

    return 0;
}