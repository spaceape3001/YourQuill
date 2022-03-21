////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/Deque.hpp>
#include <QThread>

class QTimer;
class Root;
//struct FragmentReport {
    //Vector<Fragment>   modified, created, removed;
//};

//Q_DECLARE_METATYPE(FragmentReport)


class Scanner : public QThread {
    Q_OBJECT
public:
    Scanner();
    ~Scanner();
    
    
        // ONLY CALL DURING STARTUP (not thread safe otherwise) and keep this list SMALL (exact too)
    static void     watch(const QString&, void(*)());

        //  Primes the scanner so files/documents are all registered.
    void            prime();

signals:
        // Emitted once per cycle...  (queue, because this will happen async)
    void            cycled();

public slots:

private slots:

    void            timeout();

//signals:
    //void            report(const FragmentReport&);

protected:
    virtual void    run() override;

private:

    Scanner(const Scanner&) = delete;
    Scanner(Scanner&&) = delete;
    Scanner& operator=(const Scanner&) = delete;
    Scanner& operator=(Scanner&&) = delete;
    
    
    struct D;
    struct F;
    struct W;
    struct I;

    QTimer*             m_timer;
    Deque<D>            m_scan;
    
    /*
        Startup sequence
        1)  Simple file/directory inventory 
                This phase is internal-ONLY
        2)  Specialized inventory
                This phase allows specialized handlers to do the same (file/directory/document level binding only)
        3)  Root specific Scans (ie, first for them)
        4)  First (startup) scan
        5)  Normal running
        
    
    */
    
    
        
        //INVENTORY,  //  Prescan inventory, courtesy to callbacks
        //SCAN,       //  Root init scan
        //FIRST,      //  STARTUP/First pass in new thread
        //RUNNING     //  Normal running
    //};
    
    bool                m_primed;
    bool                m_first;
    bool                m_startup;
    double              m_time;
    unsigned            m_nIter;
    uint64_t            m_nFragments;
    
    
    //D                   next();
    void            scan();
    static void     checkWatch();
    static I&       sImpl();
};


