#pragma once

#include <stdlib.h>

namespace EGE
{
	class Allocator
	{
	public:
		Allocator(){}
		virtual ~Allocator() {}

		virtual void* allocate(size_t size, size_t align = 4) = 0;
		virtual void deallocate(void* p) = 0;
		virtual size_t getAllocatedSize(void* p) = 0;
		virtual size_t getNumAllocatedBytes() = 0;

		template<typename T, typename... Args>
		T* alloc(Args&&... args)
		{
			return new (allocate(sizeof(T), __alignof(T))) T{ std::forward<Args>(args)... };
		}

		template<typename T>
		void dealloc(T* ptr)
		{
			if (ptr)
			{
				ptr->~T();
				deallocate(ptr);
			}
		}

	private:
		Allocator(const Allocator&) = delete;
		Allocator& operator=(const Allocator&) = delete;

	};

	struct MemoryHead
	{
		size_t size;
	};

	inline void* forceAlign(void* ptr, size_t alignment)
	{
		unsigned addr = reinterpret_cast<unsigned>(ptr);
		const size_t offset = addr % alignment;
		if (offset)
		{
			addr += static_cast<unsigned>((alignment - offset));
		}
		return reinterpret_cast<void*>(addr);
	}

	class SimpleHeapAllocator : public Allocator
	{
	public:
		SimpleHeapAllocator() { m_allocatedBytes = 0; m_numAllocations = 0; }
		~SimpleHeapAllocator() {}

		virtual void* allocate(size_t size, size_t align = 4) override
		{
			// Calculate proper allocation size to include
			// allocation size, alignment, and the size of the memory header
			const size_t total = size + align + sizeof(MemoryHead);

			// Keep track of memory block header
			MemoryHead* head = static_cast<MemoryHead*>(malloc(total));
			head->size = total;

			// Keep track of actual memory data
			void* data = forceAlign(head + 1, align);

			// Fill memory
			// Set header size and pad end of data
			size_t* p = reinterpret_cast<size_t*>(head + 1);
			while (p != data)
			{
				// Max size
				*p++ = static_cast<size_t>(-1);
			}

			m_allocatedBytes += total;
			++m_numAllocations;

			return data;
		}

		virtual void deallocate(void* ptr) override
		{
			if (!ptr)
				return;

			MemoryHead* head = locateMemoryHead(ptr);
			m_allocatedBytes -= head->size;
			--m_numAllocations;
			free(head);
		}

		virtual size_t getAllocatedSize(void* ptr) override
		{
			MemoryHead* head = locateMemoryHead(ptr);
			return head->size;
		}

		virtual size_t getNumAllocatedBytes() override
		{
			return m_allocatedBytes;
		}

		inline MemoryHead* locateMemoryHead(void* ptr)
		{
			size_t* addr = reinterpret_cast<size_t*>(ptr);
			--addr;

			while (*addr == static_cast<size_t>(-1))
			{
				--addr;
			}

			return reinterpret_cast<MemoryHead*>(addr);
		}
		size_t m_allocatedBytes;
		unsigned m_numAllocations;

	};
}
