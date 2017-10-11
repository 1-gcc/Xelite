//  Alloc.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once
namespace cardac
{
	template<typename T>
	class Alloc : public std::allocator<T>
	{
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

	public:
		//    convert an Alloc<T> to Alloc<U>
		template<typename U>
		struct rebind {
			typedef Alloc<U> other;
		};

	public:
		inline explicit Alloc() {}
		inline ~Alloc() {}
		inline explicit Alloc(Alloc const&) {}
		template<typename U>
		inline explicit Alloc(Alloc<U> const&) {}

		inline pointer address(reference r) { return &r; }
		inline const_pointer address(const_reference r) { return &r; }

		inline pointer allocate(size_type cnt,
			typename std::allocator<void>::const_pointer = 0) {
			return reinterpret_cast<pointer>(malloc(cnt * sizeof(T)));
		}
		inline void deallocate(pointer p, size_type) {
			free(p);
		}

		inline size_type max_size() const {
			return 0x8fffFFFF;
		}

		inline void construct(pointer p, const T& t) { new(p) T(t); }
		inline void destroy(pointer p) { p->~T(); }

		inline bool operator==(Alloc const&) { return true; }
		inline bool operator!=(Alloc const&) { return false; }
	};
}
