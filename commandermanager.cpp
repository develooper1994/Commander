#include "commandermanager.h"

#include <QtConcurrent>

namespace Utilities{

CommanderManager::CommanderManager(QObject *parent)
    : QObject{parent}
{
    init();
}

CommanderManager::~CommanderManager(){
    quit();
}

void CommanderManager::init(){
}
void CommanderManager::connections(CommanderManager* manager, Commander* worker){
    connect(manager,&CommanderManager::start, worker, &Commander::loopStart); // Qt::QueuedConnection
    connect(manager,&CommanderManager::stop, worker, &Commander::loopStop); // Qt::QueuedConnection
    connect(worker, &Commander::complete, [this](bool success){
        emit complete(success);  // üst katmana worker sonucunu gönder
    });
}

//slots
void CommanderManager::process(){
    //QFuture<void> value = QtConcurrent::run([this](){this->createWorker(this);});
    this->createWorker(this);
}

void CommanderManager::process(
    QStringList arguments,
    QString program,
    QString endl,
    int time,
    int waitStartedMsec,
    int waitWritenMsec,
    int waitFinishedMsec)
{
    QFuture<void> value = QtConcurrent::run(
        [this,
         arguments,
         program,
         endl,
         time,
         waitStartedMsec,
         waitWritenMsec,
         waitFinishedMsec]()
        {
        this->createWorker(
                this,
                arguments,
                program,
                endl,
                time,
                waitStartedMsec,
                waitWritenMsec,
                waitFinishedMsec);
    });
    //this->createWorker(this, arguments, timeoutCallback, time, program, endl, waitStartedMsec, waitWritenMsec, waitFinishedMsec);
}

void CommanderManager::quit(){
    qInfo() << "Stopping CommanderManager";
    emit stop();
}

//private
void CommanderManager::createWorker(CommanderManager *manager){
    // Worker creation factory!(not periodic)
    qInfo() << "Creating Worker";
    Commander* worker = new Commander();
    connections(manager, worker);
    emit start();
}


void CommanderManager::createWorker(
    CommanderManager* manager,
    QStringList arguments,
    const QString& program,
    const QString& endl,
    int time,
    int waitStartedMsec,
    int waitWritenMsec,
    int waitFinishedMsec){
    // Worker creation factory!(periodic)
    qInfo() << "Creating Worker";
    Commander* worker = new Commander();

    setWorker(worker, arguments, program, endl, time, waitStartedMsec, waitWritenMsec, waitFinishedMsec);

    connections(manager, worker);
    emit start();
}


void Utilities::CommanderManager::setWorker(Commander* worker, QStringList arguments, const QString &program, const QString &endl, int time, int waitStartedMsec, int waitWritenMsec, int waitFinishedMsec)
{
    worker->setEndl(std::forward<decltype(endl)>(endl));
    worker->setProgram(std::forward<decltype(program)>(program));
    worker->setArguments(std::forward<decltype(arguments)>(arguments));
    worker->setWaitStartedMsec(std::forward<decltype(waitStartedMsec)>(waitStartedMsec));
    worker->setWaitWritenMsec(std::forward<decltype(waitStartedMsec)>(waitStartedMsec));
    worker->setWaitFinishedMsec(std::forward<decltype(waitFinishedMsec)>(waitFinishedMsec));
    worker->setTime(std::forward<decltype(time)>(time));
}



}
