#include "xinu.h"

extern "C++"
{
    void *operator new(uint32 size) noexcept
    {
        return nullptr;
    }
    void *operator new[](uint32 size) noexcept
    {
        return nullptr;
    }
    void operator delete(void *ptr) noexcept
    {
    }
    void operator delete[](void *ptr) noexcept
    {
    }

    void *operator new(uint32 size, uint32 heap) noexcept
    {
        return nullptr;
    }

    void *operator new(uint32 size, uint32 size1, uint32 heap) noexcept
    {
        return nullptr;
    }

    void operator delete(void *ptr, uint32 heap) noexcept
    {
    }

    // Default placement versions of operator new.
    inline void *operator new(uint32, void *__p) noexcept
    {
        return __p;
    }
    inline void *operator new[](uint32, void *__p) noexcept
    {
        return __p;
    }

    // Default placement versions of operator delete.
    inline void operator delete(void *, void *) noexcept {}
    inline void operator delete[](void *, void *) noexcept {}
    //@}
}

extern "C" void __cxa_pure_virtual()
{
    panic("Pure virtual function called");
}

namespace
{
    void __attribute__((constructor)) _constructor_test()
    {
        kprintf("Constructor called\n");
    }
} // namespace
