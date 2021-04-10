////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "StdTableModel.hpp"
#include <gui/Provider.hpp>
#include <util/Set.hpp>
#include <memory>

/*! \brief U64 ID Table Model

    This is for structures with a single "id" uint64_t data element, which are common in this project
*/
template <typename T>
class U64TableModel : public StdTableModel<T> {
public:
    using P = std::shared_ptr<Provider<T>>;

    U64TableModel(P, QObject*parent);
    ~U64TableModel();
    
    void    append(const T&) override;
    T       get(int) const;
    T       get(const QModelIndex&) const;
    bool    has(T) const;
    
    // SLOTS, but not slots due to it being a template
    void    refresh();
    void    check();
    
    void    tick(T);
    Set<T>  ticked() const;
    void    setTicked(const Set<T>&);
    bool    isTicked(T) const;
    void    untick(T);
    
protected:
    void    _refresh();
    
    P               m_source;
    Set<T>          m_current;
};
