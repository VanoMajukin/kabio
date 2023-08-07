/*
Copyright 2022 George Bartolomey

Этот файл — часть Kabio.

Kabio — свободная программа: вы можете перераспространять ее и/или изменять ее на условиях Стандартной общественной лицензии GNU в том виде, в каком она была опубликована Фондом свободного программного обеспечения; либо версии 3 лицензии, либо (по вашему выбору) любой более поздней версии.

Kabio распространяется в надежде, что она будет полезной, но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этой программой. Если это не так, см. <https://www.gnu.org/licenses/>.
*/

#pragma once

namespace ringbuffer
{
    template <typename T>
    class RingBuffer
    {
        unsigned int capacity;
        unsigned int length;
        T *buffer;
        T *iterator;
    public:
        
        RingBuffer(unsigned int capacity);
        ~RingBuffer();
        void add(T item);
        void clear();
        unsigned int size();
        unsigned int get_capacity();
        void set_capacity(unsigned int);
        void set_size(unsigned int);
        T* begin();
        T* end();
    };
};

template<typename T> void ringbuffer::RingBuffer<T>::set_size(unsigned int size)
{
    length = size > length? length : size;
}

template<typename T> void ringbuffer::RingBuffer<T>::set_capacity(unsigned int cap)
{
    capacity = cap;
    delete [] buffer;
    buffer = new T[capacity];
    length = 0;
    iterator = begin();
}



template<typename T> void ringbuffer::RingBuffer<T>::add(T item)
{
    length++;
    if (length > capacity)
        length = capacity;
    *iterator = item;
    T* tmp = iterator + 1;
    if (tmp >= buffer + capacity)
        tmp = begin();
    iterator = tmp;
    
}

template<typename T> void ringbuffer::RingBuffer<T>::clear()
{
    length = 0;
    iterator = begin();
}


template<typename T> ringbuffer::RingBuffer<T>::RingBuffer(unsigned int capacity) : capacity(capacity)
{
    buffer = new T[capacity];
    iterator = begin();
    length = 0;
}

template<typename T> ringbuffer::RingBuffer<T>::~RingBuffer()
{
    delete [] buffer;
}

template<typename T> unsigned int ringbuffer::RingBuffer<T>::size()
{
    return length;
}

template<typename T> unsigned int ringbuffer::RingBuffer<T>::get_capacity()
{
    return capacity;
}


template<typename T> T* ringbuffer::RingBuffer<T>::begin()
{
    return buffer;
}

template<typename T> T* ringbuffer::RingBuffer<T>::end()
{
    return buffer + length;
}
