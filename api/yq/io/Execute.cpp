////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Execute.hpp"

#include "Logging.hpp"
#include "yq/util/IntRange.hpp"
#include "yq/util/Utilities.hpp"
#include "yq/util/Ref.hpp"
#include "yq/util/Vector.hpp"

#include <QByteArray>
#include <QProcess>
#include <QDebug>


QByteArray      executeProcess(const QString& binExec, const QStringList& args,
    const QByteArray& stdInput, int wait, QByteArray* stdErrors)
{
    if(binExec.isEmpty())
        return QByteArray();
    QProcess proc;
    proc.setProgram(binExec);
    proc.setArguments(args);  
    proc.start();
    proc.write(stdInput.constData());
    proc.closeWriteChannel();
    proc.waitForFinished(wait);
    QByteArray errors   = proc.readAllStandardError();
    QByteArray  output  = proc.readAllStandardOutput();
    proc.close();
    if(stdErrors)
        *stdErrors += errors;
    
    return output;
}

namespace {
    struct ProcessWrapper : public ProcessDescriptor, public RefCount {
        typedef Ref<ProcessWrapper> Ptr;
        QProcess           proc;
        
        ProcessWrapper() : proc(nullptr) {}
        ProcessWrapper(const ProcessDescriptor& p) : 
            ProcessDescriptor(p), proc(nullptr)  {}
    };
}

    
QByteArray  executeProcessSeries(const Vector<ProcessDescriptor>& proclist, 
                const QByteArray& stdInput, QByteArray* stdErrors)
{
    if(proclist.empty())
        return stdInput;
        
    Vector<ProcessWrapper::Ptr> procs;
    for(auto& pd : proclist)
        procs << new ProcessWrapper(pd);
        
        // chain the standard I/O
    for(size_t n : range<size_t>(1ULL, proclist.size()))
        procs[n-1]->proc.setStandardOutputProcess(&procs[n]->proc);
        
        //  set the programs up
    for(auto p : procs){
        p->proc.setProgram(p->binExec);
        p->proc.setArguments(p->args);
    }
    
        // START!
    for(auto p : procs)
        p->proc.start(QIODevice::ReadWrite);
    
    //push data
    if(!stdInput.isEmpty()){
        //qDebug() << "Process state is " << procs.first()->proc.state();
        //qDebug() << "Process error is " << procs.first()->proc.error();
        //qDebug() << "Process executable is " << procs.first()->proc.program();
        procs.first()->proc.write(stdInput);
    }
    procs.first()->proc.closeWriteChannel();
    
    //  now wait....
    for(auto p : procs){
        p->proc.waitForFinished(p->wait);
    }

    //  read data....
    QByteArray      output  = procs.last()->proc.readAllStandardOutput();

    //  close
    for(auto p : procs){
        if(stdErrors)
            *stdErrors  += p->proc.readAllStandardError();
        p->proc.close();
    }
    return output;
}
