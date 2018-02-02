/* adlist.h - A generic doubly linked list implementation
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ADLIST_H__
#define __ADLIST_H__

/*
Redis链表实现的特性总结：
- 双端
- 无环
- 带表头指针和表尾指针
- 带链表长度计数器
- 多态
*/

/* Node, List, and Iterator are the only data structures used currently. */

// 链表节点，用于形成链表并保存值
typedef struct listNode {
    // 前置节点
    struct listNode *prev;
    // 后置节点
    struct listNode *next;
    // 节点的值
    void *value;
} listNode;

typedef struct listIter {
    listNode *next;
    int direction;
} listIter;

// 链表，用于描述这个链表的属性
typedef struct list {
    // 表头节点
    listNode *head;
    // 表尾节点
    listNode *tail;
    // 节点值复制函数
    void *(*dup)(void *ptr);
    // 节点值释放函数
    void (*free)(void *ptr);
    // 节点值对比函数
    int (*match)(void *ptr, void *key);
    // 链表所包含的节点数量
    unsigned long len;
} list;

/* Functions implemented as macros */
// 返回链表的长度 - O(1)
#define listLength(l) ((l)->len)
// 返回链表的表头节点 - O(1)
#define listFirst(l) ((l)->head)
// 返回链表的表尾节点 - O(1)
#define listLast(l) ((l)->tail)
// 返回给定节点的前置节点 - O(1)
#define listPrevNode(n) ((n)->prev)
// 返回给定节点的后置节点 - O(1)
#define listNextNode(n) ((n)->next)
// 返回给定节点保存的值 - O(1)
#define listNodeValue(n) ((n)->value)

// 将给定的函数设置为链表的节点值复制函数
#define listSetDupMethod(l,m) ((l)->dup = (m))
// 将给定的函数设置为链表的节点值释放函数
#define listSetFreeMethod(l,m) ((l)->free = (m))
// 将给定的函数设置为链表的节点值对比函数
#define listSetMatchMethod(l,m) ((l)->match = (m))

#define listGetDupMethod(l) ((l)->dup)
#define listGetFree(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)

/* Prototypes */
// 创建一个不包含任何节点的新链表 - O(1)
list *listCreate(void);
// 释放给定链表，以及链表中的所有节点 - O(N)
void listRelease(list *list);
// 将一个包含给定值的新节点添加到给定链表的表头 - O(1)
list *listAddNodeHead(list *list, void *value);
// 将一个包含给定值的新节点添加到给定链表的表尾 - O(1)
list *listAddNodeTail(list *list, void *value);
// 将一个包含给定值的新节点添加到给定节点的之前或之后 - O(1)
list *listInsertNode(list *list, listNode *old_node, void *value, int after);
// 从链表中删除给定节点 - O(N)
void listDelNode(list *list, listNode *node);
listIter *listGetIterator(list *list, int direction);
listNode *listNext(listIter *iter);
void listReleaseIterator(listIter *iter);
// 复制一个给定链表的副本
list *listDup(list *orig);
// 查找并返回链表中包含给定值的节点
listNode *listSearchKey(list *list, void *key);
// 返回链表在给定索引上的节点
listNode *listIndex(list *list, long index);
void listRewind(list *list, listIter *li);
void listRewindTail(list *list, listIter *li);
void listRotate(list *list);

/* Directions for iterators */
#define AL_START_HEAD 0
#define AL_START_TAIL 1

#endif /* __ADLIST_H__ */
