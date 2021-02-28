////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <QWidget>

class SubWin : public QWidget {
    Q_OBJECT
public:
    SubWin(QWidget*parent=nullptr);
    virtual ~SubWin();
    
    const QString&  currentStatus() const { return m_status; }
    
public slots:
        //! Checks for change (not a full update, kinder)
    virtual void    check() {}
        //! Refreshes the display (equivalent to close-reopen
    virtual void    refresh() {}
    
signals:
    void    statusMessage(const QString&);
    void    threadSync_status(const QString&, unsigned int);
protected:
    void    status(const QString& msg=QString(), unsigned int kTimeout=0);
private slots:
    void    clearStatus();
private:
    QString m_status;
    QTimer* m_statusTimer;
};
