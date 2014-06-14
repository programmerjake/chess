#ifndef STATIC_VECTOR_H_INCLUDED
#define STATIC_VECTOR_H_INCLUDED

#include <iterator>
#include <new>
#include <initializer_list>
#include <stdexcept>
#include <cstring>
#include <cstddef>

template <typename T, size_t maxSize>
class static_vector final
{
    static_assert(sizeof(T) > 0, "can't make a static_vector of empty types");
    typedef char Item[sizeof(T)];
    Item storage[maxSize];
    size_t used = 0;
public:
    inline constexpr size_t capacity() const
    {
        return maxSize;
    }
    typedef T value_type;
    typedef value_type &reference;
    typedef value_type *pointer;
    typedef const value_type &const_reference;
    typedef const value_type *const_pointer;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    class iterator final : public std::iterator<std::random_access_iterator_tag, value_type>
    {
        T *ptr;
        friend class static_vector;
        friend class const_iterator;
        iterator(T *ptr)
            : ptr(ptr)
        {
        }
    public:
        iterator()
        {
        }
        bool operator ==(iterator rt) const
        {
            return ptr == rt.ptr;
        }
        bool operator !=(iterator rt) const
        {
            return ptr != rt.ptr;
        }
        bool operator >(iterator rt) const
        {
            return ptr > rt.ptr;
        }
        bool operator <(iterator rt) const
        {
            return ptr < rt.ptr;
        }
        bool operator >=(iterator rt) const
        {
            return ptr >= rt.ptr;
        }
        bool operator <=(iterator rt) const
        {
            return ptr <= rt.ptr;
        }
        T & operator *() const
        {
            return *ptr;
        }
        T * operator ->() const
        {
            return ptr;
        }
        iterator operator++()
        {
            return iterator(++ptr);
        }
        iterator operator++(int)
        {
            return iterator(ptr++);
        }
        iterator operator--()
        {
            return iterator(--ptr);
        }
        iterator operator--(int)
        {
            return iterator(ptr--);
        }
        friend iterator operator +(ptrdiff_t a, iterator b)
        {
            return iterator(a + b.ptr);
        }
        friend iterator operator +(iterator a, ptrdiff_t b)
        {
            return iterator(a.ptr + b);
        }
        friend ptrdiff_t operator -(iterator a, iterator b)
        {
            return (ptrdiff_t)(a.ptr - b.ptr);
        }
        friend iterator operator -(iterator a, ptrdiff_t b)
        {
            return a.ptr - b;
        }
        iterator operator +=(ptrdiff_t v)
        {
            return iterator(ptr += v);
        }
        iterator operator -=(ptrdiff_t v)
        {
            return iterator(ptr -= v);
        }
        T & operator [](ptrdiff_t v) const
        {
            return *&ptr[v];
        }
    };

    class const_iterator final : public std::iterator<std::random_access_iterator_tag, const value_type>
    {
        const T *ptr;
        friend class static_vector;
        const_iterator(const T *ptr)
            : ptr(ptr)
        {
        }
    public:
        const_iterator()
        {
        }
        const_iterator(iterator v)
            : ptr(v.ptr)
        {
        }
        friend bool operator ==(const_iterator l, const_iterator r)
        {
            return l.ptr == r.ptr;
        }
        friend bool operator !=(const_iterator l, const_iterator r)
        {
            return l.ptr != r.ptr;
        }
        friend bool operator >(const_iterator l, const_iterator r)
        {
            return l.ptr > r.ptr;
        }
        friend bool operator <(const_iterator l, const_iterator r)
        {
            return l.ptr < r.ptr;
        }
        friend bool operator >=(const_iterator l, const_iterator r)
        {
            return l.ptr >= r.ptr;
        }
        friend bool operator <=(const_iterator l, const_iterator r)
        {
            return l.ptr <= r.ptr;
        }
        const T & operator *() const
        {
            return *ptr;
        }
        const T * operator ->() const
        {
            return ptr;
        }
        const_iterator operator ++()
        {
            return const_iterator(++ptr);
        }
        const_iterator operator ++(int)
        {
            return const_iterator(ptr++);
        }
        const_iterator operator --()
        {
            return const_iterator(--ptr);
        }
        const_iterator operator --(int)
        {
            return const_iterator(ptr--);
        }
        friend const_iterator operator +(ptrdiff_t a, const_iterator b)
        {
            return const_iterator(a + b.ptr);
        }
        friend const_iterator operator +(const_iterator a, ptrdiff_t b)
        {
            return const_iterator(a.ptr + b);
        }
        friend ptrdiff_t operator -(const_iterator a, const_iterator b)
        {
            return (ptrdiff_t)(a.ptr - b.ptr);
        }
        friend const_iterator operator -(const_iterator a, ptrdiff_t b)
        {
            return a.ptr - b;
        }
        const_iterator operator +=(ptrdiff_t v)
        {
            return const_iterator(ptr += v);
        }
        const_iterator operator -=(ptrdiff_t v)
        {
            return const_iterator(ptr -= v);
        }
        const T & operator [](ptrdiff_t v) const
        {
            return *&ptr[v];
        }
    };

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    static_vector()
        : used(0)
    {
    }

    static_vector(size_t used)
        : used(used)
    {
        assert(used <= maxSize);
        for(size_t i = 0; i < used; i++)
        {
            new((void *)&storage[i]) T();
        }
    }

    static_vector(size_t used, const T & v)
        : used(used)
    {
        assert(used <= maxSize);
        for(size_t i = 0; i < used; i++)
        {
            new((void *)&storage[i]) T(v);
        }
    }

    template <typename InputIterator>
    static_vector(InputIterator first, InputIterator last)
    {
        used = 0;
        while(first != last)
        {
            assert(used < maxSize);
            new((void *)&storage[used++]) T(*first++);
        }
    }

    static_vector(const static_vector & rt)
        : used(rt.used)
    {
        for(size_t i = 0; i < used; i++)
            new((void *)&storage[i]) T(*(const T *)&rt.storage[i]);
    }

    static_vector(static_vector && rt)
        : used(rt.used)
    {
        for(size_t i = 0; i < used; i++)
            new((void *)&storage[i]) T(std::move(*(const T *)&rt.storage[i]));
    }

    static_vector(std::initializer_list<T> il)
        : static_vector(il.begin(), il.end())
    {
    }

    ~static_vector()
    {
        for(size_t i = 0; i < used; i++)
            ((T *)&storage[i])->~T();
    }

    const static_vector & operator =(const static_vector & rt)
    {
        for(size_t i = rt.used; i < used; i++)
        {
            ((T *)&storage[i])->~T();
        }
        for(size_t i = used; i < rt.used; i++)
        {
            new((void *)&storage[i]) T(*(const T *)&rt.storage[i]);
        }
        for(size_t i = 0; i < used && i < rt.used; i++)
        {
            *(T *)&storage[i] = *(const T *)&rt.storage[i];
        }
        used = rt.used;
        return *this;
    }

    const static_vector & operator =(static_vector && rt)
    {
        for(size_t i = rt.used; i < used; i++)
        {
            ((T *)&storage[i])->~T();
        }
        for(size_t i = used; i < rt.used; i++)
        {
            new((void *)&storage[i]) T(std::move(*(const T *)&rt.storage[i]));
        }
        for(size_t i = 0; i < used && i < rt.used; i++)
        {
            *(T *)&storage[i] = std::move(*(const T *)&rt.storage[i]);
        }
        used = rt.used;
        return *this;
    }

    const static_vector & operator =(std::initializer_list<T> il)
    {
        assert(il.size() <= maxSize);
        for(size_t i = il.size(); i < used; i++)
        {
            ((T *)&storage[i])->~T();
        }
        auto iter = il.begin();
        for(size_t i = 0; i < used && i < il.size(); i++)
        {
            *(T *)&storage[i] = *iter++;
        }
        for(size_t i = used; i < il.size(); i++)
        {
            new((void *)&storage[i]) T(*iter++);
        }
        used = il.size();
        return *this;
    }

    iterator begin()
    {
        return iterator((T *)&storage[0]);
    }

    const_iterator begin() const
    {
        return const_iterator((const T *)&storage[0]);
    }

    const_iterator cbegin() const
    {
        return const_iterator((const T *)&storage[0]);
    }

    iterator end()
    {
        return iterator((T *)&storage[used]);
    }

    const_iterator end() const
    {
        return const_iterator((const T *)&storage[used]);
    }

    const_iterator cend() const
    {
        return const_iterator((const T *)&storage[used]);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(cend());
    }

    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(cbegin());
    }

    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(cbegin());
    }

    size_t size() const
    {
        return used;
    }

    void resize(size_t n)
    {
        assert(n <= maxSize);
        if(n < used)
        {
            for(size_t i = n; i < used; i++)
            {
                ((T *)&storage[i])->~T();
            }
        }
        else if(n > used)
        {
            while(used < n)
            {
                new((void *)&storage[used++]) T();
            }
        }
        used = n;
    }

    void resize(size_t n, const T & v)
    {
        assert(n <= maxSize);
        if(n < used)
        {
            for(size_t i = n; i < used; i++)
            {
                ((T *)&storage[i])->~T();
            }
        }
        else if(n > used)
        {
            while(used < n)
            {
                new((void *)&storage[used++]) T(v);
            }
        }
        used = n;
    }

    bool empty() const
    {
        return used == 0;
    }

    T & operator [](size_t n)
    {
        return *(T *)&storage[n];
    }

    const T & operator [](size_t n) const
    {
        return *(const T *)&storage[n];
    }

    T & at(size_t n)
    {
        if(n >= used)
            throw std::out_of_range("static_vector::at");
        return *(T *)&storage[n];
    }

    const T & at(size_t n) const
    {
        if(n >= used)
            throw std::out_of_range("static_vector::at");
        return *(const T *)&storage[n];
    }

    T & front()
    {
        return *(T *)&storage[0];
    }

    const T & front() const
    {
        return *(const T *)&storage[0];
    }

    T & back()
    {
        return *(T *)&storage[used - 1];
    }

    const T & back() const
    {
        return *(const T *)&storage[used - 1];
    }

    T * data()
    {
        return (T *)&storage[0];
    }

    const T * data() const
    {
        return (const T *)&storage[0];
    }

    void clear()
    {
        for(size_t i = 0; i < used; i++)
            ((T *)&storage[i])->~T();
        used = 0;
    }

    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last)
    {
        for(size_t i = 0; i < used; i++)
            ((T *)&storage[i])->~T();
        used = 0;
        while(first != last)
        {
            assert(used < maxSize);
            new((void *)&storage[used++]) T(*first++);
        }
    }

    void assign(size_t n, const T &v)
    {
        assert(n <= maxSize);
        for(size_t i = 0; i < used; i++)
            ((T *)&storage[i])->~T();
        used = n;
        for(size_t i = 0; i < used; i++)
            new((void *)&storage[i]) T(v);
    }

    void assign(std::initializer_list<T> il)
    {
        assign(il.begin(), il.end());
    }

    void push_back(const T & v)
    {
        assert(used < maxSize);
        new((void *)&storage[used++]) T(v);
    }

    void push_back(T && v)
    {
        assert(used < maxSize);
        new((void *)&storage[used++]) T(std::move(v));
    }

    void pop_back()
    {
        assert(used > 0);
        ((T *)&storage[--used])->~T();
    }

    iterator insert(const_iterator position, const T &v)
    {
        assert(used < maxSize);
        std::memmove((void *)(position + 1).ptr, (const void *)position.ptr, (end() - position) * sizeof(T));
        new((void *)position.ptr) T(v);
        used++;
        return iterator((T *)position.ptr);
    }

    iterator insert(const_iterator position, T &&v)
    {
        assert(used < maxSize);
        std::memmove((void *)(position + 1).ptr, (const void *)position.ptr, (end() - position) * sizeof(T));
        new((void *)position.ptr) T(std::move(v));
        used++;
        return iterator((T *)position.ptr);
    }

    iterator erase(const_iterator position)
    {
        assert(used > 0 && position != end());
        ((T *)position.ptr)->~T();
        std::memmove((void *)position.ptr, (const void *)(position + 1).ptr, ((end() - position) - 1) * sizeof(T));
        used--;
        return iterator((T *)position.ptr);
    }
};

#endif // STATIC_VECTOR_H_INCLUDED
