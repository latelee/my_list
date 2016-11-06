/**
双向链表示例。演示创建、遍历、删除等操作。
g++ 5.8.4 & VS2010 测试通过

Late Lee <latelee@163.com> http://www.latelee.org


由hostapd的list源码修改而成 

结构示意图：

list_head
   prev ----------------------------------------------- 
      |                                                |
      -<-                                              |
   next |    node1          node2           node3      |
   ^|   |                                              |
   ||   |   data...         data...         data...    |
   ||   --- prev <--------- prev <--------- prev <-----|
   ||-----> next ---------> next ---------> next ------
   |        data...         data...         data...    |
   |-----<----------------------------------------------
   
头结点不使用，无法用list_entry拿到实际数据
list_head的prev指向最后一个结点，next指向第一个结点。
如果一直用next可遍历并循环到开始处，prev亦然。

注：list_add本身在“结点”后添加，如在list_haed“后”添加，则变成在整个链表开头。
如果在node1“后”添加，则在整个链表的node1后新加结点。

*/

#include <stdio.h>
#include <stdlib.h>

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
    //list_for_each(devinfo, &my_list, struct i2c_devinfo, list)
    list_for_each(devinfo, &my_list, struct i2c_devinfo, list)
    {
        printf("[%d] busnum: %d, slave: %d\n", i++, devinfo->busnum, devinfo->slave);
	}
}

void delete_list(void)
{
    struct i2c_devinfo *devinfo, *tdev;
    //　注：当要删除链表数据时，要使用*_safe，同时要额外增加一个临时变量
    list_for_each_safe(devinfo, tdev, &my_list, struct i2c_devinfo, list)
    {
        list_del(&devinfo->list);
        free(devinfo);
	}
}

void list_misc(void)
{
    struct i2c_devinfo* devinfo, *tdev, *tmpdev;

    struct i2c_devinfo* n;
    n = list_entry(my_list.prev, struct i2c_devinfo, list);
    printf("prev entry: %d\n", n->busnum);
    n = list_entry(my_list.next, struct i2c_devinfo, list);
    printf("next entry: %d\n", n->busnum);

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
    
    // 中途插入、删除
    //list_for_each(tdev, &my_list, struct i2c_devinfo, list)
    list_for_each_safe(tdev, tmpdev, &my_list, struct i2c_devinfo, list)
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
}

// 遍历示例
void list_misc1(void)
{
    LIST_HEAD(my_list);
    struct i2c_devinfo* devinfo;
    
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 1;
    devinfo->slave = 25;
    list_add_tail(&devinfo->list, &my_list);
    devinfo = (struct i2c_devinfo*)malloc(sizeof(struct i2c_devinfo));
    devinfo->busnum = 2;
    devinfo->slave = 25;
    list_add_tail(&devinfo->list, &my_list);
    
    
    struct i2c_devinfo* n;
    n = list_entry(my_list.next, struct i2c_devinfo, list); // 第一个结点
    printf("next1 entry: %d\n", n->busnum);
    n = list_entry(my_list.next->next, struct i2c_devinfo, list);   // 第二个结点
    printf("next2 entry: %d\n", n->busnum);
    n = list_entry(my_list.next->next->next, struct i2c_devinfo, list); // 回到my_list，此时数据无效
    printf("next3 entry: %d(0x%x)\n", n->busnum, n->busnum);
    n = list_entry(my_list.next->next->next->next, struct i2c_devinfo, list);   // 第一个结点
    printf("next4 entry: %d\n", n->busnum);
    n = list_entry(my_list.next->next->next->next->next, struct i2c_devinfo, list); // 第二个结点
    printf("next5 entry: %d\n", n->busnum);
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
    show();

    printf("after delete...\n");
    delete_list();
    // 删除链表后，已经没有内容了
    show();

    list_misc1();
    return 0;
}