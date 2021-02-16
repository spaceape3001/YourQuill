#pragma once

#include <stddef.h>
#include <atomic>


/*! \brief Reference Counted Pointer
 * 
 * 	\tparam	T	type of object (must derive from RefCount!)
 */
template <typename T>
class Ref {
public:

    Ref() : m_ptr(nullptr) {}

    Ref(T*ptr);

    Ref(const Ref& copy);
    
    Ref(Ref&&move);

    ~Ref();
    
    Ref&   operator=(const Ref& rhs);
    
    Ref&   operator=(Ref&& rhs);
    
    Ref&	operator=(T* ptr);
    
    operator T*();

    operator const T*() const;

    T*			ptr();
    const T*	ptr() const;

    T*			operator->();
    const T*	operator->() const;

    bool		valid() const;
    
    bool        invalid() const;

    bool		operator!() const;

    bool		operator==(const Ref&rhs) const;

    bool		operator==(const T* ptr) const;
    
    friend bool operator==(const T* lhs, const Ref& rhs)
    {
        return lhs == rhs.m_ptr;
    }

    bool		operator!=(const Ref&rhs) const;

    bool		operator!=(const T* ptr) const;
    
    friend bool operator!=(const T* lhs, const Ref& rhs)
    {
        return lhs != rhs.m_ptr;
    }
    
private:
    T*			m_ptr;
};

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //      REF COUNT
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
/*! \brief Basic Reference Counted class
*/
class RefCount {
public:
    bool	unique() const 
    {
        return m_count <= 1;
    }

    size_t	count() const 
    {
        return m_count;
    }
    
    /*! \brief Cause a memory leak
    
        This VIOLATES the pairity, ensuring that proper functions will never
        release the memory.  (Are you sure this is what you want to do?)
    */
    void    leak() { incRef(); }

protected:

    RefCount();
    RefCount(const RefCount& copy);
    virtual ~RefCount(){}
    
    RefCount& operator=(const RefCount&) = delete;
    
    template <typename T>  friend class ::Ref;

        //! TREAD LIGHTLY!  Increments the reference count (can lead to orphaned memory)
    void	incRef() const;
        //! TREAD LIGHTLY!  Decrements the reference count (can lead to dangling pointers)
    void	decRef() const;

private:
    mutable std::atomic<size_t>  m_count;
};



    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //      IMPLEMENTATION
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


template <typename T>
Ref<T>::Ref(T*ptr) : m_ptr(ptr)
{
    if(m_ptr)
	    m_ptr->incRef();
}

template <typename T>
Ref<T>::Ref(const Ref& copy) : m_ptr(copy.m_ptr)
{
    if(m_ptr)
	    m_ptr->incRef();
}

template <typename T>
Ref<T>::Ref(Ref&&move)
{
    m_ptr		= move.m_ptr;
    move.m_ptr	= nullptr;
}

template <typename T>
Ref<T>::~Ref()
{
    if(m_ptr){
	    m_ptr->decRef();
	
	    #ifdef _DEBUG
	    m_ptr	= nullptr;
	    #endif
    }
}

template <typename T>
Ref<T>&   Ref<T>::operator=(const Ref& rhs)
{
    if(rhs.m_ptr != m_ptr){
	    if(m_ptr)
		    m_ptr->decRef();
	    m_ptr   = rhs.m_ptr;
	    if(m_ptr)
		    m_ptr->incRef();
    }
    return *this;
}

template <typename T>
Ref<T>&   Ref<T>::operator=(Ref&& rhs)
{
    if(this != &rhs){
	    if(m_ptr)
		    m_ptr -> decRef();
	    m_ptr       = rhs.m_ptr;
	    rhs.m_ptr   = nullptr;
    }
    return *this;
}

template <typename T>
Ref<T>&	Ref<T>::operator=(T* ptr)
{
    if(m_ptr == ptr)
	    return *this;
    if(m_ptr)
	    m_ptr->decRef();
    m_ptr   = ptr;
    if(m_ptr)
	    m_ptr->incRef();
    return *this;
}

template <typename T>
Ref<T>::operator T*()
{
    return m_ptr;
}

template <typename T>
Ref<T>::operator const T*() const
{
    return m_ptr;
}
		
template <typename T>
T*			Ref<T>::ptr()
{
    return m_ptr;
}

template <typename T>
const T*	Ref<T>::ptr() const
{
    return m_ptr;
}

template <typename T>
T*			Ref<T>::operator->()
{
    return m_ptr;
}

template <typename T>
const T*	Ref<T>::operator->() const
{
    return m_ptr;
}

template <typename T>
bool		Ref<T>::valid() const
{
    return m_ptr != nullptr;
}

template <typename T>
bool        Ref<T>::invalid() const
{
    return m_ptr == nullptr;
}

template <typename T>
bool		Ref<T>::operator!() const
{
    return m_ptr == nullptr;
}

template <typename T>
bool		Ref<T>::operator==(const Ref&rhs) const
{
    return m_ptr == rhs.m_ptr;
}

template <typename T>
bool		Ref<T>::operator==(const T* ptr) const
{
    return m_ptr == ptr;
}


template <typename T>
bool		Ref<T>::operator!=(const Ref&rhs) const
{
    return m_ptr != rhs.m_ptr;
}

template <typename T>
bool		Ref<T>::operator!=(const T* ptr) const
{
    return m_ptr != ptr;
}


