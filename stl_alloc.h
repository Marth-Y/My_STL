// 不考虑多线程安全的问题

#ifndef MY_STL_ALLOC_H
#define MY_STL_ALLOC_H

#include <stdlib.h>

#define __THROW_BAD_ALLOC do { cerr << "out of memory" << endl;exit(1);}while(0)

template <int inst>
class __malloc_alloc_template {
private:
    static void* oom_malloc(size_t);
    static void* oom_realloc(void*,size_t);
    static void (* __malloc_alloc_oom_handler)();
public:
    static void* allocate(size_t n) {
        void* result = malloc(n);
        if(0 == result) {
            result = oom_malloc(n); // 内存不足调用oom_malloc()处理
        }
        return result;
    }

    static void deallocate(void* p, size_t) {
        free(p);
    }

    static void* reallocate(void* p,size_t,size_t new_size) {
        void* result = realloc(p,new_size);
        if(0 == result) result = oom_realloc(p,new_size);
        return result;
    }

    // 设置自己的oom-handler函数
    static void (* set_malloc_handler(void (*f)()))() {
        // 这个函数就相当于：void (*) set_malloc_handler(void (*f)()) {...}
        // 返回值为void (*)()类型的函数指针，参数为void (*f)()类型的函数指针
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return old;
    }
};

// 接下来实现三个私有的函数指针
// 这个函数需要set_malloc_handler指定
template <int inst>
void (* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template <int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t size) {
    void (*my_malloc_handler)();
    void* result;

    for(;;) {
        my_malloc_handler = __malloc_alloc_template;
        if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
        (*my_malloc_handler)();
        result = malloc(n);
        if (result) return result;
    }
}

template <int inst>
void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t size) {
    void (*my_malloc_handler)();
    void* result;
    while(true) {
        my_malloc_handler = __malloc_alloc_template;
        if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
        (*my_malloc_handler)();
        result = realloc(p,n);
        if (result) return result;
    }
}

typedef __malloc_alloc_template<0> malloc_alloc;

//***************以下实现二级空间配置器***************//
template <int inst>
class __default_alloc_template {
private:
    union obj {
        union obj* _free_list_link;
        char _client_data[i];
    };
    enum { __ALIGN = 8 };// 小型区块上调边界
    enum { __MAX_BYTES = 128 }; // 小型区块最大大小
    enum { __NFREELISTS = __MAX_BYTES / __ALIGN }; // 链表个数
    
private:
    static size_t ROUND_UP(size_t bytes) {
        return (((bytes) + __ALIGN - 1) & -(__ALIGN - 1));
    }

private:
    // 定义16个空闲链表
    static obj* volatile _free_list[__NFREELISTS];
    // 获取bytes大小在链表的索引
    static size_t FREELIST_INDEX(size_t bytes) {
        return (((bytes) + __ALIGN - 1) / __ALIGN - 1);
    }

    static void* refill(size_t n);
    // 配置一大块空间.
    static char* chunk_alloc(size_t size, int &nobjs);

    static char* _start_free; // 内存池起始地址
    static char* _end_free; // 内存池结束地址.以上两者皆只在chunk_alloc中发生变化
    static size_t _heap_size; 
public:
    //次级空间配置器函数定义：
    static void* allocate(size_t n) {
        obj* volatile * my_free_list;
        obj* result;
        // 如果超过128，则调用一级空间配置器
        if(n > __MAX_BYTES) {
            return (malloc_alloc::allocate(n));
        }
        my_free_list = _free_list + FREELIST_INDEX(n);
        result = *my_free_list;
        if(0 == result) {
            // 空闲链表中没有可用的区块，需要去内存池申请
            void* r = refill(ROUND_UP(n));
            return r;
        }
        // 空闲链表有可用区块，就需要调整链表返回区块
        *my_free_list = result->_free_list_link;
        return result;
    }
    static void deallocate(void* p, size_t n) {
        obj* q = (obj*)p;
        obj* volatile * my_free_list;
        if(n > __MAX_BYTES) {
            malloc_alloc::deallocate(p,n);
            return;
        }
        my_free_list = _free_list + FREELIST_INDEX(n);
        q->_free_list_link = *my_free_list;
        *my_free_list = q;
    }
};
// 初始化静态成员数据:
template <int inst>
char* __default_alloc_template<inst>::_start_free = 0;

template <int inst>
char* __default_alloc_template<inst>::_end_free = 0;

template <int inst>
size_t __default_alloc_template<inst>::_heap_size = 0;

template <int inst>
__default_alloc_template<inst>::obj* volatile
__default_alloc_template<inst>::_free_list[__NFREELISTS] = 
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

// refill实现
// refill接受的参数 n ，已经上调至8的倍数
template <int inst>
void* __default_alloc_template<inst>::refill(size_t n) {
    int nobjs = 20; // 缺省为20个区块
    // 调用chunk_alloc，尝试取得nobjs个区块作为free list的新节点
    char* chunk = chunk_alloc(n,nobjs);
    obj* volatile * my_free_list;
    obj* result;
    obj *current_obj,*next_obj;
    int i;

    // 如果只取得一个区块，则直接返回，不需要调整空闲链表
    if (1 == nobjs) return chunk;
    // 获取了多个区块，那么就把多余的区块插入空闲链表对应的位置
    my_free_list = _free_list + FREELIST_INDEX(n);
    result = (obj*)chunk;// 准备返回的区块
    // 这里注意：这里已经是refill函数中了，那么说明空闲链表上对应索引的链表上没有区块可用，
    // 因此可以直接更改二级指针my_free_list的指向，对于chunk，它是char*类型，因此可以直接 + n跳过返回的第一个区块
    *my_free_list = next_obj = (obj*)(chunk + n);
    for(i = 1;;++i) {
        current_obj = next_obj;
        next_obj = (obj*)((char*)next_obj + n);
        if (nobjs - 1 == i) {
            current_obj->_free_list_link = 0;
            break;
        }
        else {
            current_obj->_free_list_link = next_obj;
        }
    }
    return result;
}

// chunk_alloc实现
// chunk_alloc接受的参数 n ，已经上调至8的倍数
template <int inst>
char* __default_alloc_template<inst>::chunk_alloc(size_t size,int& nobjs) {
    char* result;
    size_t total_bytes = size * nobjs;
    size_t bytes_left = _end_free - _start_free; // 内存池剩余的空间

    if (bytes_left > total_bytes) {
        result = _start_free;
        _start_free += total_bytes;
        return result;
    } else if (bytes_left >= size) {
        nobjs = bytes_left / size;
        total_bytes = size * nobjs;
        _start_free += total_bytes;
        return result;
    } else {
        // 新的申请的空间的大小为需求量的两倍加上随着申请次数增加而越来越大的附加量。
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(_heap_size >> 4);
        if (bytes_left > 0) {
            // 将内存池中剩余的零头分配出去，对内存池进行充分的利用
            // 要想清楚，已经执行到这一步了，那么说明bytes_left的大小也就只够一个区块的大小了
            // (而且按上调至8的大小来分配，那么最后剩余的一定刚好够一个区块的大小)。
            // 因此可以直接用_start_free转为obj*来进行操作了。
            obj* volatile * my_free_list = _free_list + FREELIST_INDEX(bytes_left);
            ((obj*)_start_free)->_free_list_link = *my_free_list;
            *my_free_list = (obj*)_start_free;
        }

        //从heap空间重新申请空间，补充内存池
        _start_free = (char*)malloc(bytes_to_get);
        if (0 == _start_free) {
            //堆空间内存不足，malloc失败
            int i;
            obj* volatile * my_free_list,*p;
            // 此时去寻找空闲链表上，索引更大的链表上是否有可用区块，将其分配出去.
            for (i = size;i < __MAX_BYTES;i += __ALIGN) {
                my_free_list = _free_list + FREELIST_INDEX(i);
                p = *my_free_list;
                if (0 != p) {
                    // 尚有比当前区块大的可用区块，摘下来放入内存池
                    *my_free_list = p->_free_list_link;
                    // chunk_alloc是不返回给客端的，而是改变内存池的大小，因此返回给内存池一个区块
                    // 需要修改指针.
                    _start_free = (char*)p;
                    _end_free = _start_free + i;
                    return (chunk_alloc(size,nobjs));
                }
            }
            // 没有更大的空闲区块可用,此时就只能看一级空间配置器的oom函数的处理了
            _end_free = 0;
            _start_free = (char*)malloc_alloc::allocate(bytes_to_get);
        }
        // malloc申请到了足够的空间，修改内存池变量
        _heap_size += bytes_to_get;
        _end_free = _start_free + bytes_to_get;
        return (chunk_alloc(size,nobjs)); // 递归调用自己，为了修正nobjs
    }
}

#define USE_DEFAULT_ALLOC
#ifdef USE_DEFAULT_ALLOC
typedef __default_alloc_template<0> alloc;
#else
typedef malloc_alloc alloc;
#endif

template <class T,class Alloc>
class simple_alloc {
public:
    static T* allocate(size_t n)
    { return 0 == n?0:(T*)Alloc::allocate(n * sizeof(T)); }

    static T* allocate(void)
    { return (T*)Alloc::allocate(sizeof(T)); }

    static void deallocate(T* p,size_t n)
    { if(0 != n) Alloc::deallocate(p,n * sizeof(T)); }

    static void deallocate(T* p)
    { Alloc::deallocate(p, sizeof(T)); }
};


#endif