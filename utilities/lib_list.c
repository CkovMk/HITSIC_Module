/**
 * Copyright 2018 - 2020 HITSIC
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

//
// Created by 17616 on 2019/9/26.
//
#include "lib_list.h"

/*!
 * @addtogroup clist
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

void List_Constructor(list_t *p, unsigned int SizeOfData) {
    assert(p);
    assert(SizeOfData);
    memset(p, 0, sizeof(list_t));
    p->SizeOfData = SizeOfData;
    p->size = 0;
    //生成head和tail
	p->head = (list_node_t*)LIST_MALLOC(sizeof(list_node_t));
	assert(p->head);
    p->tail = p->head;
    //互联
    p->head->next = NULL;
    p->head->prev = NULL;

    //return 0;//初始化完毕
}

void List_Destructor(list_t *p) {
    //删除所有元素
    List_Clear(p);
    LIST_FREE(p->head->data);
    LIST_FREE(p->tail->data);
    LIST_FREE(p->tail);
    LIST_FREE(p->head);
    memset(p, 0, sizeof(list_t));
}

uint32_t List_MaxSize() { return 0xFFFFU; }

bool List_IsEmpty(list_t *p) { if (p->size == 0) { return true; } else { return false; }}

uint32_t List_GetSize(list_t *p) { return p->size; }

list_node_t* List_Create(list_t* p, list_node_t* pos)
{
	assert(pos);
	//assert(src);
	list_node_t* newl;
	newl = (list_node_t*)LIST_MALLOC(sizeof(list_node_t));
	assert(newl);
	newl->data = LIST_MALLOC(p->SizeOfData);
	assert(newl->data);
	newl->prev = pos->prev;
	newl->next = pos;
	if (newl->prev != NULL)
	{
		newl->prev->next = newl;
	}
	else
	{
		p->head = newl;
	}
	pos->prev = newl;
	++p->size;
	return newl;
}

list_node_t* List_Insert(list_t *p, list_node_t *pos, void *src) {
	assert(src);
    list_node_t *newl = List_Create(p, pos);
	memcpy(newl->data, src, p->SizeOfData);    
	return newl;
}

void List_Erase(list_t *p, list_node_t *pos) {
    assert(p);
    assert(pos);
    assert(pos->next != NULL);
    pos->next->prev = pos->prev;
    if(pos->prev != NULL)
    {
        pos->prev->next = pos->next;
    }
    else
    {
        p->head = pos->next;
    }
    LIST_FREE(pos->data);
    LIST_FREE(pos);
    --p->size;
}

void List_PopFront(list_t *p) {
    if (p->size > 0) {
        List_Erase(p, p->head);
    }
}

void List_PopBack(list_t *p) {
    if (p->size > 0) {
        List_Erase(p, p->tail->prev);
    }
}

list_node_t* List_PushFront(list_t *p, void *data) {
    return List_Insert(p, p->head, data);
}

list_node_t* List_PushBack(list_t *p, void *data) {
    return List_Insert(p, p->tail, data);
}

void List_Clear(list_t *p) {
    while(p->size>0)
    {
        List_Erase(p,p->head->next);
    }
}

void* List_GetFront(list_t *p) { return p->head->data; }

void* List_GetBack(list_t *p) { return p->tail->prev->data; }

list_node_t* List_ItBegin(list_t *p) { return p->head; }

list_node_t* List_ItEnd(list_t *p) { return p->tail; }

//示例代码
#ifdef WIN32
#include <stdio.h>
#include "list.h"
typedef struct my {
	float a;
	float b;
	int c[2];
}my_t;
int main() {
	printf("Hello, World!\n");
	list_t list0;
	List_Constructor(&list0, sizeof(my_t));
	my_t buf;
	for (int i = 0; i < 4; ++i) {
		buf.a = i * 1.2;
		buf.b = i * 1.2 + 1.3;
		buf.c[0] = i;
		buf.c[1] = i + 1;
		List_PushBack(&list0, &buf);
	}
    printf("list size = %d\n", List_GetSize(&list0));
	printf("\n----take!----\n");
	for (list_node_t* j = List_ItBegin(&list0); j != list0.tail; j = j->next) {
		printf("a = %f\n", ((my_t*)j->data)->a);
		printf("b = %f\n", ((my_t*)j->data)->b);
		printf("c[0] = %d\n", ((my_t*)j->data)->c[0]);
		printf("c[1] = %d\n", ((my_t*)j->data)->c[1]);
	}
	printf("\n----pop!----\n");
    for (list_node_t* j = List_ItBegin(&list0); j != list0.tail; ) {
        j = List_ItBegin(&list0)->next;
        List_PopFront(&list0);
    }
    printf("list size = %d\n", List_GetSize(&list0));
	printf("----end!----\n");
	return 0;
}
#endif


#if defined (__cplusplus)
}
#endif

/* @} */
