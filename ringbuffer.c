#include "ringbuffer.h"

/**
 * @file
 * Implementation of ring buffer functions.
 * 
 * From https://github.com/AndersKaloer/Ring-Buffer
 */

void ring_buffer_init(ring_buffer_t *buffer) {
  buffer->tail_index = 0;
  buffer->head_index = 0;
}

void ring_buffer_queue(ring_buffer_t *buffer, char data) {
  /* Is buffer full? */
  if(ring_buffer_is_full(buffer)) {
    /* Is going to overwrite the oldest byte */
    /* Increase tail index */
    buffer->tail_index = ((buffer->tail_index + 1) & RING_BUFFER_MASK);
  }

  /* Place data in buffer */
  buffer->buffer[buffer->head_index] = data;
  buffer->head_index = ((buffer->head_index + 1) & RING_BUFFER_MASK);
}

uint8_t ring_buffer_dequeue(ring_buffer_t *buffer, char *data) {
  if(ring_buffer_is_empty(buffer)) {
    /* No items */
    return 0;
  }
  
  *data = buffer->buffer[buffer->tail_index];
  buffer->tail_index = ((buffer->tail_index + 1) & RING_BUFFER_MASK);
  return 1;
}
