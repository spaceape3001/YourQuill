////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UndoCallback.hpp"
#include "util/Vector.hpp"
#include <QUndoCommand>

class UndoAction;


class UndoBase {
public:

    virtual ~UndoBase(){}
    virtual void    setUndoCallback(UndoCallback*);
    virtual void    setUndoName(const QString&v) { m_undoName = v; }
    
    UndoCallback*   callback() const { return m_callback; }
    QString         undoName() const { return m_undoName; }

protected:
    UndoBase() : m_callback(nullptr), m_recursive(false) {}
    void            execute(QUndoCommand*);
    
    static int      makeUndoId();
    
    
        // This list will be added to as needs dictate
    
    //template <typename T>
    //void            execAppend(Vector<T>&, const T&, const QString& undoText=QString());
    
    //template <class A, typename T>
    //void            execAppend(Vector<T>&, const T&, A*, void(A::*)(), const QString& undoText=QString());

    template <typename T>
    void            execSet(T&, const T&, const QString& undoText=QString());
    
    template <class A, typename T>
    void            execSet(T&, const T&, A*, void(A::*fnSignal)(), const QString& undoText=QString());

    template <class A, typename T>
    void            execSet(T&, const T&, A*, void(A::*fnSignal)(const T&), const QString& undoText=QString());

    //template <class A, typename T>
    //void            execSet(T&, const T&, A*, void(A::*fnSet)(const T&), T (A::*fnGet)() const, const QString& undoText=QString());

    //!     If TRUE, the "set" will recursively call children
    bool            recursive() const { return m_recursive; }
    void            setRecursive(bool f) { m_recursive = f; }

private:
    UndoCallback*   m_callback;
    QString         m_undoName;
    bool            m_recursive;
    
    template <typename T> class UndoSet;
    template <class A, typename T> class UndoSetSignal;
    template <class A, typename T> class UndoSetSignalValue;
    template <typename T> class UndoAppend;
    template <class A, typename T> class UndoAppendSignal;
};

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

template <typename T>
class UndoBase::UndoSet : public QUndoCommand {
public:
    UndoSet(T& variable, const T& value, const QString& text) : 
        QUndoCommand(text), m_variable(variable), m_new(value), m_old(variable)
    {
    }
    
    void        undo() override
    {
        m_variable  = m_old;
    }
    
    void        redo() override
    {
        m_variable  = m_new;
    }
    
    int     id() const override 
    {
        static const int    s_id    = makeUndoId();
        return s_id;
    }
    
    bool    mergeWith(const QUndoCommand* other) override
    {
        if(other->id() != id())
            return false;
        const UndoSet*  b   = static_cast<const UndoSet*>(other);
        if(&(b -> m_variable) != &m_variable)
            return false;
        m_new   = b -> m_new;
        return true;
    }

private:    
    T&      m_variable;
    T       m_new, m_old;
};

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  Simple SET, no complications

template <typename T>
void  UndoBase::execSet(T&variable, const T&newValue, const QString& undoText)
{
    if(m_callback){
        m_callback -> pushUndo(new UndoSet<T>(variable, newValue, undoText));
    } else
        variable    = newValue;
}

template <class A, typename T> 
class UndoBase::UndoSetSignal : public QUndoCommand {
public:
    UndoSetSignal(T& variable, const T& value, A* obj, 
        void(A::*fnSignal)(), const QString& undoText
    ) : QUndoCommand(undoText),
        m_variable(variable),
        m_new(value),
        m_old(variable),
        m_object(obj),
        m_signal(fnSignal)
    {
    }

    void        undo()
    {
        m_variable      = m_old;
        (m_object ->* m_signal)();
    }
    
    void        redo()
    {
        m_variable      = m_new;
        (m_object ->* m_signal)();
    }
    
    int     id() const override 
    {
        static const int    s_id    = makeUndoId();
        return s_id;
    }
    
    bool    mergeWith(const QUndoCommand* other) override
    {
        if(other->id() != id())
            return false;
        const UndoSetSignal*  b   = static_cast<const UndoSetSignal*>(other);
        if((&(b -> m_variable) != &m_variable) || (b->m_object != m_object) || (b->m_signal != m_signal))
            return false;
        m_new   = b -> m_new;
        return true;
    }

private:    
    T&      m_variable;
    T       m_new, m_old;
    A*      m_object;
    void (A::*m_signal)();
};

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  SET with a signal

template <class A, typename T>
void  UndoBase::execSet(T&variable, const T&newValue, A*object, void(A::*fnSignal)(), const QString& undoText)
{
    if(m_callback){
        m_callback -> pushUndo(new UndoSetSignal<A,T>(variable, newValue, object, fnSignal, undoText));
    } else {
        variable    = newValue;
        (object->*fnSignal)();
    }
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  SET with a signal taking a value

template <class A, typename T> 
class UndoBase::UndoSetSignalValue : public QUndoCommand {
public:
    UndoSetSignalValue(T& variable, const T& value, A* obj, 
        void(A::*fnSignal)(const T&), const QString& undoText
    ) : QUndoCommand(undoText),
        m_variable(variable),
        m_new(value),
        m_old(variable),
        m_object(obj),
        m_signal(fnSignal)
    {
    }

    void        undo()
    {
        m_variable      = m_old;
        (m_object ->* m_signal)(m_old);
    }
    
    void        redo()
    {
        m_variable      = m_new;
        (m_object ->* m_signal)(m_new);
    }
    
    int     id() const override 
    {
        static const int    s_id    = makeUndoId();
        return s_id;
    }
    
    bool    mergeWith(const QUndoCommand* other) override
    {
        if(other->id() != id())
            return false;
        const UndoSetSignalValue*  b   = static_cast<const UndoSetSignalValue*>(other);
        if((&(b -> m_variable) != &m_variable) || (b->m_object != m_object) || (b->m_signal != m_signal))
            return false;
        m_new   = b -> m_new;
        return true;
    }

private:    
    T&      m_variable;
    T       m_new, m_old;
    A*      m_object;
    void (A::*m_signal)(const T&);
};

template <class A, typename T>
void  UndoBase::execSet(T&variable, const T&newValue, A*object, void(A::*fnSignal)(const T&), const QString& undoText)
{
    if(m_callback){
        m_callback -> pushUndo(new UndoSetSignalValue<A,T>(variable, newValue, object, fnSignal, undoText));
    } else {
        variable    = newValue;
        (object->*fnSignal)(newValue);
    }
}
