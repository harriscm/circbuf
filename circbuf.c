#include "circbuf.h"

#include <stdio.h>

static unsigned advance_head_tail_value(unsigned value, unsigned max)
{
    return (value +1)%max;
}

static void advance_head(circbuf_t *self)
{
    if(self->isFull)
    {
        self->tail = advance_head_tail_value(self->tail, self->max);
    }

    self->head = advance_head_tail_value(self->head, self->max);
    self->isFull = (self->head == self->tail);
}

static char put_single_element(circbuf_t *self, unsigned char data, char try)
{
    char putValue = 0;
    if(!try || !self->isFull)
    {
        putValue = 1;
        self->buffer[self->head] = data;
        advance_head(self);
    }

    return putValue;
}

static int reset(circbuf_t *self)
{
    int err = !self;
    if(!err)
    {
        self->head = 0;
        self->tail = 0;
        self->isFull = 0;
    }
    return err;
}

static int size(circbuf_t *self, unsigned *size)
{
    int err = !self && !size;

    if(!err)
    {
        *size = self->max;
        if(!self->isFull)
        {
            if(self->head >= self->tail)
            {
                *size = (self->head - self->tail);
            }
            else
            {
                *size = (self->max + self->head);
            }
        }
    }
    return err;
}

static int capacity(circbuf_t *self, unsigned *capacity)
{
    int err = !self || !capacity;

    if(!err)
    {
        *capacity = self->max; 
    }
    return err;
}

static int put(circbuf_t *self, const unsigned char *data, unsigned size)
{
    int err = !self || !data || !size;
    int i = 0;
    
    if(!err)
    {
        for(i = 0; i < size; i++)
        {
            put_single_element(self, data[i], 0);
        }
    }
    return err;
}

static int try_put(circbuf_t *self, const unsigned char *data, unsigned size)
{
    int err = !self || !data || !size;

    if(!err)
    {
        unsigned available;
        self->size(self, &available);
        if(available >= size)
        {
            err = self->put(self, data, size);
        }
        else
        {
            err = -1;
        }
    }

    return err;
}

int get(circbuf_t *self, unsigned char *data, unsigned size)
{
    int err = !self || !data || !size;

    if(!err)
    {
        char empty;
        self->empty(self, &empty);
        if(!empty)
        {
            unsigned i = 0;
            for (i = 0; i < size; i++)
            {
                data[i] = self->buffer[self->tail];
                self->tail = advance_head_tail_value(self->tail, self->max);
                self->full = 0;
            }
        }
        else
        {
            err = -1;
        }
    }
    return err;

}

static int peek(circbuf_t *self, unsigned char *data, unsigned size)
{
    int err = !self || !data || !size;

    if(!err)
    {
        unsigned available;
        char empty;
        self->size(self, &available);
        self->empty(self, &empty);
        if(empty || size > available)
        {
            err = -1;
        }
        else
        {
            unsigned pos = self->tail;
            unsigned i = 0;
            for(i = 0; i < size; i++)
            {
                data[i] = self->buffer[pos];
                pos = advance_head_tail_value(pos, self->max);
            }
        }
    }

    return err;
}

int empty(circbuf_t *self, char *empty)
{
    int err = !self || !empty;

    if(!err)
    {
        *empty = self->isFull && (self->head == self->tail);
    }

    return err;
}

int full(circbuf_t *self, char *full)
{
    int err = !self || !full;
    
    if(!err)
    {
        *full = self->isFull;
    }

    return err;
}

int circbuf_init(circbuf_t *obj, unsigned char *buffer, unsigned max)
{
    int err = !obj || !buffer || !max;

    if(!err)
    {
        obj->buffer = buffer;
        obj->max = max;
        reset(obj);

        obj->reset = reset;
        obj->size = size;
        obj->capacity = capacity;
        obj->put = put;
        obj->try_put = try_put;
        obj->get = get;
        obj->peek = peek;
        obj->empty = empty;
        obj->full = full;
    }

    return err;
}
