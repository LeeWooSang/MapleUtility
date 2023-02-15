#pragma once
#include <QtGui/qvalidator.h>

QT_BEGIN_NAMESPACE
//namespace qdesigner_internal
//{
    class QLongLongValidator : public QValidator
    {
        Q_OBJECT
        Q_PROPERTY(qlonglong bottom READ bottom WRITE setBottom)
        Q_PROPERTY(qlonglong top READ top WRITE setTop)
    public:
        explicit QLongLongValidator(QObject* parent = nullptr);
        QLongLongValidator(qlonglong bottom, qlonglong top, QObject * parent);
        ~QLongLongValidator();
        QValidator::State validate(QString &, int &) const;
        void setBottom(qlonglong);
        void setTop(qlonglong);
        void setRange(qlonglong bottom, qlonglong top);
        qlonglong bottom() const { return b; }
        qlonglong top() const { return t; }
    private:
        Q_DISABLE_COPY_MOVE(QLongLongValidator)
        qlonglong b;
        qlonglong t;
    };

// ----------------------------------------------------------------------------
    class QULongLongValidator : public QValidator
    {
        Q_OBJECT
        Q_PROPERTY(qulonglong bottom READ bottom WRITE setBottom)
        Q_PROPERTY(qulonglong top READ top WRITE setTop)
    public:
        explicit QULongLongValidator(QObject * parent = nullptr);
        QULongLongValidator(qulonglong bottom, qulonglong top, QObject * parent);
        ~QULongLongValidator();
        QValidator::State validate(QString &, int &) const;
        void setBottom(qulonglong);
        void setTop(qulonglong);
        void setRange(qulonglong bottom, qulonglong top);
        qulonglong bottom() const { return b; }
        qulonglong top() const { return t; }
    private:
        Q_DISABLE_COPY_MOVE(QULongLongValidator)
        qulonglong b;
        qulonglong t;
    };
//}  // namespace qdesigner_internal
QT_END_NAMESPACE

