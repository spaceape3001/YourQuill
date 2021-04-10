////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

SubWin::SubWin(QWidget*parent) : QWidget(parent), m_statusTimer(nullptr)
{
    connect(this, &SubWin::threadSync_status, this, &SubWin::status);
    setAttribute(Qt::WA_DeleteOnClose);
}

SubWin::~SubWin()
{
}

void    SubWin::clearStatus()
{
    if(m_statusTimer)
        m_statusTimer -> stop();
    m_status.clear();
    statusMessage(QString());
}

void    SubWin::status(const QString& txt, unsigned int kTimeout)
{
    if(isMainThread()){
        if(m_statusTimer)
            m_statusTimer -> stop();

        if(kTimeout > 0){
            if(!m_statusTimer){
                m_statusTimer   = new QTimer(this);
                connect(m_statusTimer, &QTimer::timeout, this, &SubWin::clearStatus);
                m_statusTimer -> setSingleShot(true);
            } 
            m_statusTimer -> setInterval(kTimeout);
            m_statusTimer -> start();
        }
        m_status    = txt;
        statusMessage(txt);
    } else
        threadSync_status(txt, kTimeout);
}

