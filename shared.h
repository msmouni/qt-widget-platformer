#ifndef SHARED_H
#define SHARED_H

#include <QMutex>

template <class T>
struct SharedResource
{
private:
    mutable QMutex mutex;
    T sharedVariable;

public:
    SharedResource() : sharedVariable(0) {}

    T getSharedVariable() const
    {
        QMutexLocker locker(&mutex);
        return sharedVariable;
    }

    void setSharedVariable(T value)
    {
        QMutexLocker locker(&mutex);
        sharedVariable = value;
    }
};

#endif // SHARED_H
