#include "ActiveObjectDemo.hpp"
#include <QTimer>
#include <QElapsedTimer>
#include <QThread>
#include <QDebug>

static void DebugOutputFuncAndCurrentThreadObjName(const char * functionName)
{
    qDebug() << functionName << QThread::currentThread()->objectName();
}

ActiveObjectDemo::ActiveObjectDemo(QObject *parent) :
    QObject(parent),
    mDemoTimedBehavior(new QTimer(this)),
    mElapsed(new QElapsedTimer())
{
    //in theory Qt will ensure Queued Connections where appropriate.
    //but I prefer to be explicit.
    connect(mDemoTimedBehavior, &QTimer::timeout, this, &ActiveObjectDemo::DemoTimedBehavior,
            Qt::QueuedConnection);
    mDemoTimedBehavior->setInterval(std::chrono::seconds(1));
    mDemoTimedBehavior->start();
    mElapsed->start();
}

void ActiveObjectDemo::DemoApiAsync(const QString &inData)
{
    DebugOutputFuncAndCurrentThreadObjName(__FUNCTION__);

    //this public API implementation assumes thread context
    //is incorrect, and invoke the internal method with a QueuedConnection.
    QMetaObject::invokeMethod(this,
                              [this, inData](){ InternalDemoApi(inData); },
                              Qt::QueuedConnection);
}

void ActiveObjectDemo::DemoPublicSlot()
{
    DebugOutputFuncAndCurrentThreadObjName(__FUNCTION__);

    //ensure thread context, as user's of this
    //class could still manually execute this slot method.
    if (QThread::currentThread() == thread())
    {
        InternalDemoSlot();
    }
    else
    {
        QMetaObject::invokeMethod(this,
                                  [this](){ InternalDemoSlot(); },
                                  Qt::QueuedConnection);
    }
}

void ActiveObjectDemo::InternalDemoApi(const QString &inData)
{
    DebugOutputFuncAndCurrentThreadObjName(__FUNCTION__);

    TimestampAndEmit(QString("DemoApi(%1)").arg(inData));
}

void ActiveObjectDemo::InternalDemoSlot()
{
    DebugOutputFuncAndCurrentThreadObjName(__FUNCTION__);

    TimestampAndEmit("DemoSlot");
}

void ActiveObjectDemo::DemoTimedBehavior()
{
    DebugOutputFuncAndCurrentThreadObjName(__FUNCTION__);

    TimestampAndEmit("TimerBehavior");
}

void ActiveObjectDemo::TimestampAndEmit(const QString &data)
{
    DebugOutputFuncAndCurrentThreadObjName(__FUNCTION__);

    emit DemoOutput(QString("%1: %2")
                    .arg(mElapsed->elapsed())
                    .arg(data));
}
