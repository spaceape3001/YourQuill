////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GridTickModel.hpp"
#include "util/Map.hpp"


class StandardGridTickModel : public GridTickModel {
    Q_OBJECT
public:
    
    StandardGridTickModel(QObject* parent=0);
    
    /*! TRUE if this index is a grid stop */
    virtual bool			hasStop(int64_t) const;
    virtual Vector<int64_t>	stops  (int64_t from, int64_t to, uint64_t minPitch) const;
    virtual uint64_t		pitch  (int64_t) const;
    virtual QString			label  (int64_t) const;
    virtual QColor			color  (int64_t) const;
    
    void	                clearAllTicks();
    void	                setTickToUnitPitchRatio(uint64_t);
    virtual double          tickToUnitPitchRatio() const { return m_tickToUnitPitch; }
    void	                setPositiveFormat(const QString&);
    void	                setNegativeFormat(const QString&);
    
    void	                addTick(uint64_t, const QColor&color=QColor("black"));
    
    uint64_t				    tickToUnitPitch() const { return m_tickToUnitPitch; }
    QString					positiveFormat() const { return m_posFormat; }
    QString					negativeFormat() const { return m_negFormat; }

//signals:
//	virtual void        modelChanged();
    
private:
    Map<uint64_t, QColor> m_ticks;
    QString					    m_posFormat;
    QString					    m_negFormat;
    uint64_t					m_tickToUnitPitch;
};

