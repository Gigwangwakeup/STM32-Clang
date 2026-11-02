/*
 * que_buffer.c
 *
 *  Created on: 2026. 10. 30.
 *      Author: kccistc
 */

#include "que_buffer.h"
#include "string.h"

void que_buf_init(que_buf_t *que, void *storage, uint16_t elem_size, uint16_t capacity){
	que->head = 0;
	que->tail = 0;

	que->buf = (uint8_t *)storage;
	que->elem_size = elem_size;
	que->capacity = capacity;
}

static uint8_t *slot(que_buf_t *que, uint16_t n){
	return que->buf + (uint32_t)n * que->elem_size; // 저장하려고 하는 주소
}

static uint16_t next(que_buf_t *que, uint16_t i){
	return (uint16_t)((i+1)%que->capacity);
}
bool que_put(que_buf_t *que, void *elem){
	uint16_t head = que->head;
	uint16_t next_head = next(que, head);

	if(next_head == que->tail) return false; // full 상태
	memcpy(slot(que, head), elem, que->elem_size);
	que->head = next_head;
	return true;
}

bool que_get(que_buf_t *que, void*elem){

	uint16_t tail = que->tail;
	uint16_t next_tail = next(que, tail);

	if (tail == que->head) return false; // empty
	memcpy(elem, slot(que, tail), que->elem_size);
	que->tail = next_tail;

	return true;
}

bool que_is_empty(que_buf_t *que){
	return que->tail == que->head;
}

bool que_is_full(que_buf_t *que){
	return next(que, que->head) == que->tail;
}
