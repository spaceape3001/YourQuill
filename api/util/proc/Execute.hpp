////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

/*! \file
    \brief Process Execution utilities
*/
#pragma once

#include <QStringList>
#include <util/collection/AllFwd.hpp>

class QByteArray;

namespace yq {


        /*! \brief Structure for executeProcessSeries
        
            This captures the arguments that would be passed to executeProcess, 
            if the processes were called individually.
        */
    struct ProcessDescriptor {
        //! Default wait for processes
        static constexpr const int  defWait = 5000;

        /*! \brief Path to the binary
        
            It's strongly advised that this be a full path....
        */
        QString     binExec;
        
        /*! \brief Command line arguments
        */
        QStringList args;
        
        /*! \brief Maximum wait time
        
            Rather than hanging the caller, this specifies the maximum wait
            time before considering the process "hung".  Note, specifying "0"
            will block forever until the program completes.
        */
        int         wait;

        ProcessDescriptor() : wait(defWait) {}
        ProcessDescriptor(const QString& be, const QStringList& a, int w=defWait) 
            : binExec(be), args(a), wait(w) {}
    };



    /*! \brief Executes specified program

        This executes the specified program, passing in the byte array as standard
        input, and BLOCKING until the progrma is finished, up to the maximum time
        specified, and returning the captured standard output.

        \param[in]  binExec     Binary (full path) of executable being ran.
        \param[in]  args        Command line arguments
        \param[in]  stdInput    Standard input to be passed to the process
        \param[in]  wait        Maximum wait time (in milliseconds) for the process to finish
        \param[out] stdErrors   If not NULL, any standard error will be appended to this.
    */
    QByteArray      executeProcess(const QString& binExec, const QStringList& args,
        const QByteArray& stdInput=QByteArray(), int wait=ProcessDescriptor::defWait,
        QByteArray* stdErrors=nullptr);



        
    /*! \brief Chain Executes 

        This executes a chain of commands, piping the output from one command as the input 
        to the next command.
        
        \param[in]  procs       Describes the processes to be executed, in order
        \param[in]  stdInput    Standard input to be passed to the FIRST process
        \param[out] stdErrors   If not NULL, any standard error will be appended to this.
    */
    QByteArray  executeProcessSeries(const Vector<ProcessDescriptor>& procs, 
                    const QByteArray& stdInput=QByteArray(), QByteArray* stdErrors=nullptr);

}
