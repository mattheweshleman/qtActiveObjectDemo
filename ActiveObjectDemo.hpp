#ifndef ACTIVEOBJECTDEMO_HPP
#define ACTIVEOBJECTDEMO_HPP

#include <QObject>
#include <QString>

class QTimer;
class QElapsedTimer;

/**
 * @brief The ActiveObjectDemo class demonstrates
 *        a few simple Active Object patterns
 *        in the Qt environment.
 */
class ActiveObjectDemo : public QObject
{
    Q_OBJECT
public:
    explicit ActiveObjectDemo(QObject *parent = nullptr);

    void DemoApiAsync(const QString& inData);

signals:
    void DemoOutput(const QString& output);

public slots:
    void DemoPublicSlot();

private slots:
    void InternalDemoApi(const QString& inData);
    void InternalDemoSlot();
    void DemoTimedBehavior();

private:
    void TimestampAndEmit(const QString& data);

    QTimer*  mDemoTimedBehavior;
    QElapsedTimer* mElapsed;
};

#endif // ACTIVEOBJECTDEMO_HPP
