#pragma once

#ifdef __cplusplus
extern "C"{
#endif

typedef struct circbuf_t
{
    /**
     * @brief 
     * Resets the buffer, without deleting data, as though it were empty.
     */
    int (*reset)(struct circbuf_t *self);

    /**
     * @brief 
     * Returns the current number of elements in the buffer
     */
    int (*size)(struct circbuf_t *self, unsigned *size);

    /**
     * @brief 
     * Returns the max number of elements in the buffer.
     */
    int (*capacity)(struct circbuf_t *self, unsigned *capacity);

    /**
     * @brief 
     * 
     * Puts an element in the buffer.  If the buffer is full,
     * the oldest element will be replaced with the new element.
     */
    int (*put)(struct circbuf_t *self, const unsigned char *element, unsigned size);

    /**
     * @brief 
     * 
     * Puts an element in the buffer.  If the buffer is full, 
     * an error is returned and no data is added.
     */
    int (*try_put)(struct circbuf_t *self, const unsigned char *element, unsigned size);

    /**
     * @brief 
     * 
     * Returns the oldest element in the buffer and removes it
     * from the buffer
     */
    int (*get)(struct circbuf_t *self, unsigned char *element, unsigned size);

    /**
     * @brief 
     * 
     * Returns the oldest element in the buffer without removing
     * it from the buffer
     */
    int (*peek)(struct circbuf_t *self, unsigned char *element, unsigned size);

    /**
     * @brief 
     * 
     * Returns true if the buffer is empty
     */
    int (*empty)(struct circbuf_t *self, char *empty);

    /**
     * @brief 
     * 
     * Returns true if the buffer is full
     */
    int (*full)(struct circbuf_t *self, char *full);


    /** Implementation specifics, don't touch! */
    unsigned char *buffer;
    unsigned head;
    unsigned tail;
    unsigned max;
    char isFull;
}circbuf_t;


int circbuf_init(circbuf_t *obj, unsigned char *buffer, unsigned sizeInBytes);

#ifdef __cplusplus
}
#endif
