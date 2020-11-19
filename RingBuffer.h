/*
 * RingBuffer.h
 *
 *  Created on: Nov 19, 2020
 *      Author: Agapov Aleksey
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_
#include <cassert>      /* для assert() */
#include <new>     /* new, delete */

namespace ring {

#define spinlock_start(lock)  while(lock) {std::cout << "spinlock!!!\n";}; lock = true;
#define spinlock_stop(lock)  lock = false;



/*
 * Ring buffer class
*/
template <class T, size_t N = 10>
class RingBuffer {
public:
	RingBuffer() : RingBuffer(N){}
	RingBuffer(size_t _new_size) : size(_new_size) {
		assert((size>0)&&"RingBuffer size mast above zero");
		index_read = 0;
		index_write = 0;
		block_size = sizeof(T);
		array = static_cast<char *>( ::operator new (size*block_size, std::nothrow)) ;  // , std::nothrow)
		assert((array!=nullptr)&&"Failed to allocate memory");
		memset(array, 0, size*block_size);
		lock = false;
		std::cout << "RingBuffer constructor!!!\n";
	}
	virtual ~RingBuffer() {
		delete (array);
		std::cout << "RingBuffer destructor!!!\n";
	}

	bool Add(const T &item) {
		spinlock_start(lock);
		bool ret = isWrite();
		if (ret) {// nreinterpret_cast<char *>
			memcpy((array + (index_write * block_size)), reinterpret_cast<const char *>(&item), block_size);
			if (index_write < size-1) {
				index_write++;
			} else {
				index_write = 0;
			}
		}
		spinlock_stop(lock);
		return ret;
	}


	bool Take(T &item) {
		spinlock_start(lock);
		bool ret = isRead();
		if (ret) {// reinterpret_cast<char *>
			char * position = (array + (index_read * block_size));
			memcpy(reinterpret_cast<char *>(&item), position, block_size);
			memset(position, 0, block_size);

			if (index_read < size-1) {
				index_read++;
			} else {
				index_read = 0;
			}
		}
		spinlock_stop(lock);
		return ret;
	}

private:

	void memcpy(char *dest, const char *source, size_t size) {
		for (size_t count = 0; count < size; count++) {
			dest[count] = source[count];
		}
	}
	void memset(char *dest, unsigned char val, size_t size){
		for (size_t count = 0; count < size; count++) {
			dest[count] = val;
		}
	}

	bool isWrite() {
		if (((index_write+1)==size)&&(index_read == 0)) {return false;}
		return ((index_write>=index_read) || ((index_write+1)<index_read));
	}

	bool isRead() {
		if (((index_read+1)==size)&&(index_write == 0)) {return false;}
		return (index_write!=index_read);
	}

	size_t size;
	size_t block_size;
	char *array;
	volatile size_t index_read;
	volatile size_t index_write;
	volatile bool lock;
};

} /* namespace ring */

#endif /* RINGBUFFER_H_ */
