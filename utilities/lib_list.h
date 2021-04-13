/**
 * Copyright 2018 - 2021 HITSIC
 * All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,哈工大智能车创新俱乐部
 * All rights reserved.
 *
 * @brief           单向链表
 * @file       		lib_list.h
 * @company	        哈工大智能车创新俱乐部
 * @author     		肖日涛 (QQ:1761690868); C.M. (QQ:905497173)
 * @version    		v1.1
 * @Software 		
 * @Target_core		any platform
 * @date       		ver1.0 2019.09.26 by 肖日涛
 * @date            ver1.1 2019.09.28 by C.M.
 *
 * @note：哈尔滨工业大学智能车创新俱乐部专用，请勿泄露
***************************************************************************************************************************/

#pragma once
#ifndef UTILITIES_LIB_LIST_H
#define UTILITIES_LIB_LIST_H
#include <inc_stdlib.h>
#include "hitsic_common.h"

/*!
 * @addtogroup clist
 * @{
 */

/** @brief : 软件版本 */
#define LIB_LIST_VERSION (HITSIC_MAKE_VERSION(0U, 1U, 1U))

#ifdef __cplusplus
extern "C" {
#endif


// Library version
#define LIST_VERSION "0.1.5"

// Memory management macros
#ifndef LIST_MALLOC
#define LIST_MALLOC malloc
#endif
#ifndef LIST_FREE
#define LIST_FREE free
#endif

/*
* list_t node struct.
*/
typedef struct list_node {
    void *data;
    struct list_node *prev;
    struct list_node *next;
} list_node_t;

/*
 * list_t struct.
 */
typedef struct list_struct {
    list_node_t *head;
    list_node_t *tail;
    unsigned int SizeOfData;
    unsigned int size;
} list_t;

/**
 * @brief   链表的构造函数。
 * 
 * @param  {list_t*} p    : list对象句柄。
 * @param  {unsigned} int : 每个node存储的数据多大。
 */
void List_Constructor(list_t *p, unsigned int SizeOfData_);

/**
 * @brief   链表的析构函数。
 *
 * @param  {list_t*} p    : list对象句柄。
 */
void List_Destructor(list_t *p);

/**
 * @brief   返回链表能容纳的最大节点数量。
 */
uint32_t List_MaxSize(void);

/**
 * @brief   检查链表是否为空。
 *
 * @param  {list_t*} p    : list对象句柄。
 * @return {bool}         : 空链表返回true，非空链表返回false。
 */
bool List_IsEmpty(list_t *p);

/**
 * @brief  获得 链表内有效节点的个数。
 *
 * @param  {list_t*} p    : list对象句柄。
 * @return {uint32_t}     : 链表内节点的个数。空链表返回0。
 */
uint32_t List_GetSize(list_t *p);

/**
 * @brief                     : 向链表内新建节点。
 *
 * @param  {list_t*} p        : list对象句柄。
 * @param  {list_node_t*} pos : 要插入节点的位置。新节点总是插入到此位置之前。
 * @return {list_node_t*}     : 返回指向创建的节点的指针。
 */
list_node_t* List_Create(list_t* p, list_node_t* pos);

/**
 * @brief                     : 向链表内插入节点。
 *
 * @param  {list_t*} p        : list对象句柄。
 * @param  {list_node_t*} pos : 要插入节点的位置。新节点总是插入到此位置之前。
 * @param  {void*} data       : 要插入节点内的数据。数据大小应与构造链表时一致。
 * @return {list_node_t*}     : 返回指向插入的节点的指针。
 */
list_node_t* List_Insert(list_t *p, list_node_t *pos, void *data);

/**
 * @brief   删除链表的一个节点
 *
 * @param  {list_t*} p        : list对象句柄
 * @param  {list_node_t*} pos : 要删除的节点
 * @return {int}              : 错误代码
 */
void List_Erase(list_t *p, list_node_t *pos);

/**
 * @brief   删除链表内第一个元素
 *
 * @param  {list_t*} p    : list对象句柄
 */
void List_PopFront(list_t *p);

/**
 * @brief   删除链表内最后一个元素
 *
 * @param  {list_t*} p    : list对象句柄
 */
void List_PopBack(list_t *p);

/**
 * @brief   向链表头部插入节点。
 *
 * @param  {list_t*} p    : list对象句柄
 * @param  {void*} data   : 要插入节点内的数据。数据大小应与构造链表时一致。
 * @return {list_node_t*} : 返回指向插入的节点的指针。
 */
list_node_t* List_PushFront(list_t *p, void *data);

/**
 * @brief   向链表尾部插入节点。
 *
 * @param  {list_t*} p    : list对象句柄
 * @param  {void*} data   : 要插入节点内的数据。数据大小应与构造链表时一致。
 * @return {list_node_t*} : 返回指向插入的节点的指针。
 */
list_node_t* List_PushBack(list_t *p, void *data);

/**
 * @brief   清空链表。
 *
 * @param  {list_t*} p    : list对象句柄
 */
void List_Clear(list_t *p);

/**
 * @brief   返回指向链表内第一个节点所存储元素的指针。
 *
 * 用户需要自行检查链表是否为空。
 *
 * @param  {list_t*} p    : list对象句柄
 */
void *List_GetFront(list_t *p);

/**
 * @brief   返回指向链表内最后一个有效节点所存储元素的指针。
 *
 * @param  {list_t*} p    : list对象句柄。
 */
void *List_GetBack(list_t *p);

/**
 * @brief   返回指向链表第一个节点的指针。
 *
 * @param  {list_t*} p    : list对象句柄。
 */
list_node_t* List_ItBegin(list_t *p);

/**
 * @brief   返回指向链表最后一个有效节点之后的节点的指针。
 *
 * @param  {list_t*} p    : list对象句柄。
 */
list_node_t* List_ItEnd(list_t *p);



#ifdef __cplusplus
}
#endif

/* @} */

#endif //UTILITIES_LIB_LIST_H
