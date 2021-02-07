#pragma once

/*! \brief Implementor of the safety function 
*/
template <class P>
class Safety {
public:

    Safety() : m_done(true) {}
    Safety(P p) : m_pred(p), m_done(false) {}
    ~Safety() { trigger(); }
    
    void    trigger() 
    {
        if(!m_done){
            m_pred();
            m_done    = true;
        }
    }
    
    void    clear() { m_done = true; }

private:
    P       m_pred;
    bool    m_done;
};

/*! \brief Guaranteed destruction

    Something that will trigger on a destruction, unless the flag is 
    cleared
*/
template <class P>
Safety<P> safety(P p) 
{ 
    return Safety<P>(p); 
}
