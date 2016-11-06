/**
linux链表示例。演示创建、遍历、删除等操作。

注：可以做到向普通链表那样前加、后加、删除等操作。
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
    list_add_tail(&devinfo->list, &my_list);
}

void show(void)
{
    struct i2c_devinfo *devinfo;
    int i = 0;

    if (list_empty(&my_list))
    {
        printf("list empty, nothing to show.\n");
        return;
    }
    
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
    
    show();
}

void list_misc(void)
{
    struct i2c_devinfo* devinfo, *tdev, *pdev;

    struct i2c_devinfo* n;
    n = list_entry(&my_list, struct i2c_devinfo, list);
    printf("current entry: %d\n", n->busnum);
    
    // 获取第一个、最后一个
    n = list_first_entry(&my_list, struct i2c_devinfo, list);
    printf("first entry: %d\n", n->busnum);
    n = list_last_entry(&my_list, struct i2c_devinfo, list);
    printf("last entry: %d\n", n->busnum);
    

    // 
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 1;
    devinfo->slave = 25;
    list_add(&devinfo->list, &my_list); // 在my_list后添加，变成在链表的头部
    
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 65535;
    devinfo->slave = 25;
    list_add_tail(&devinfo->list, &my_list); // 在末尾
    
    printf("devinfo1: %p %p %p\n", devinfo, &devinfo->busnum, &devinfo->slave);
    
    // 测试container_of
    int* aa = &devinfo->busnum;
    n = container_of(aa, struct i2c_devinfo, busnum);
    
    printf("devinfo2: %p %p %p %d\n", n, aa, &n->busnum, n->busnum);

    // 中途插入、删除
    list_for_each_entry_safe(tdev, pdev, &my_list, list)
    {
        if (tdev->busnum == 10) // 在此节点后插入
        {
            devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
            devinfo->busnum = 250;
            devinfo->slave = 25;
            list_add(&devinfo->list, &tdev->list);
        }
        if (tdev->busnum == 12) // 在此节点前插入
        {
            devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
            devinfo->busnum = 250;
            devinfo->slave = 25;
            list_add_prev(&devinfo->list, &tdev->list);
        }
        if (tdev->busnum == 13) // 删除此节点
        {
            list_del(&tdev->list);
        }
    }
    
    show();
}

void list_misc1(void)
{
    struct i2c_devinfo* devinfo = NULL;
    struct i2c_devinfo* n = NULL;
    
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 1;
    devinfo->slave = 25;
    
    printf("devinfo1: %p %p %p\n", devinfo, &devinfo->busnum, &devinfo->slave);
    
    // 测试container_of
    int* aa = &devinfo->busnum;
    n = container_of(aa, struct i2c_devinfo, busnum);
    
    printf("devinfo2: %p %p %p %d\n", n, aa, &n->busnum, n->busnum);
}

int main(void)
{
    int i = 0;
    int j = 0;
    for (i = 10; i < 15; i++, j += 2)
    {
        init(i, j);
    }

    show();

    list_misc();
    
    list_misc1();
    
    printf("after delete...\n");
    delete_list();


    return 0;
}