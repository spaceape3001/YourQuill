////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


GridTickModel::GridTickModel(QObject* parent) : 
        QObject(parent)
{
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

StandardGridTickModel::StandardGridTickModel(QObject* parent) :
    GridTickModel(parent)
{
    m_tickToUnitPitch		= 0;
    m_posFormat				= "+%g";
    m_negFormat				= "-%g";
    
}

/*! TRUE if this index is a grid stop */
bool			StandardGridTickModel::hasStop(int64_t s) const
{
    for(uint64_t k : m_ticks.keys())
        if(!(s % k))
            return true;
    return false;
}

Vector<int64_t>			StandardGridTickModel::stops  (int64_t from, int64_t to, uint64_t minPitch) const
{
    Map<uint64_t, QColor>::const_iterator		start, j, end;
    Vector<int64_t>		r;
    int64_t				i;

    start			= m_ticks.lower_bound(minPitch);
    end				= m_ticks.cend();

    uint64_t     s   = start->first;	
    from            = from - from % (int64_t) s;
    
        /*
            We are doing an optimization with the assumption that
            the smallest pitch is a common factor to all of the
            bigger pitches.  This permits for a much faster
            lookup
        */
    for(i=from;i<=to; i += s)
        for(j=start;j!=end;++j)
            if(!(i % (int64_t) j -> first)){
                r << i;
                break; /* continue the outer for loop */
            }
    return r;
}

uint64_t	StandardGridTickModel::pitch  (int64_t h) const
{
    uint64_t    r   = 0;	
    for(auto & j : m_ticks)
        if(!(h % (int64_t) j.first))
            r       = j.first;
    return r;
}

QString			StandardGridTickModel::label  (int64_t i) const
{
    double  v   = ((double) i) / ((double) m_tickToUnitPitch);
    if(m_negFormat.isEmpty())
        return QString::number(v);
    else if(i < 0)
        return m_negFormat + QString::number(fabs(v));
    else if(i > 0)
        return m_posFormat + QString::number(fabs(v));
    else
        return "0";
}

QColor			StandardGridTickModel::color  (int64_t i) const
{
    uint64_t		p	= pitch(i);
    if(!p)
        return QColor("white");
    return m_ticks.get(p);
}

void	StandardGridTickModel::clearAllTicks()
{
    m_ticks.clear();
    emit modelChanged();
}

void	StandardGridTickModel::setTickToUnitPitchRatio(uint64_t tup)
{
    m_tickToUnitPitch		= tup;
    emit modelChanged();
}

void	StandardGridTickModel::setPositiveFormat(const QString&s)
{
    m_posFormat		= s;
    emit modelChanged();
}

void	StandardGridTickModel::setNegativeFormat(const QString&s)
{
    m_negFormat		= s;
    emit modelChanged();
}

void	StandardGridTickModel::addTick(uint64_t p, const QColor&c)
{
    m_ticks[p]		= c;
    emit modelChanged();
}

