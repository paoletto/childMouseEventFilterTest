#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QPointF>
#include <QLineF>
#include <QDebug>

class CircleMask : public QQuickItem
{
    Q_OBJECT
public:
    CircleMask(QQuickItem *parent = nullptr) : QQuickItem(parent)
    {
        setFiltersChildMouseEvents(true);
    }

    bool contains(const QPointF &point) const override {
        QPointF center(width() * 0.5, width() * 0.5);
        QLineF line(center, point);
        bool res = line.length() <= center.x();
        //qDebug() << "contains: "<<res;
        return res;
    }

protected:
    bool acceptEvent(QEvent *event) {
        return false;
    }
    bool rejectEvent(QEvent *event) {
        event->setAccepted(false);
        return true;
    }

    bool childMouseEventFilter(QQuickItem *item, QEvent *event) override
    {
        switch (event->type()) {
        case QEvent::MouseButtonPress: {
                if (!contains(static_cast<QMouseEvent*>(event)->pos())) {
                    return rejectEvent(event);
                } else {
                    return acceptEvent(event);
                }
            }
            break;
        case QEvent::MouseButtonRelease: {
                if (!contains(static_cast<QMouseEvent*>(event)->pos())) {
                    return rejectEvent(event);
                } else {
                    return acceptEvent(event);
                }
            }
            break;
        case QEvent::HoverEnter: {
                qDebug()<<"HoverEnter";
                if (contains(static_cast<QMouseEvent*>(event)->pos())) {
                    m_hoverEntered = true;
                    return acceptEvent(event);
                } else {
                    return rejectEvent(event);
                }
            }
            break;
        case QEvent::HoverLeave: {
                qDebug()<<"HoverLeave";
                if (m_hoverEntered) {
                    m_hoverEntered = false;
                    return acceptEvent(event);
                } else {
                    return rejectEvent(event);
                }
            }
            break;
        case QEvent::HoverMove: {
                qDebug()<<"HoverMove";
                // HoverMove means mouse has entered the area
                QHoverEvent* eve = static_cast<QHoverEvent*>(event);
                QPointF pos = eve->posF();
                QPointF oldPos = eve->oldPosF();
                if (m_hoverEntered) {
                    // check only if exiting
                    if (!contains(static_cast<QMouseEvent*>(event)->pos())) {
                        // Synthesizing a HoverLeave <-- Note: this apparently doesn't get delivered to this method again!
                        m_hoverEntered = false;
                        QHoverEvent *ev = new QHoverEvent(QEvent::HoverLeave, pos, oldPos, eve->modifiers());
                        QCoreApplication::postEvent(item, ev);
                        return rejectEvent(event);
                    } else {
                        return acceptEvent(event);
                    }
                } else {
                    // check only if entering
                    if (contains(static_cast<QMouseEvent*>(event)->pos())) {
                        // Synthesizing a HoverEnter <-- Note: this apparently doesn't get delivered to this method again!
                        m_hoverEntered = true;
                        QHoverEvent *ev = new QHoverEvent(QEvent::HoverEnter, pos, oldPos, eve->modifiers());
                        QCoreApplication::postEvent(item, ev);
                        return rejectEvent(event);
                    } else {
                        return rejectEvent(event);
                    }
                }
            }
            break;
        default:
            return acceptEvent(event);
        }
    }

    bool m_hoverEntered = false;
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<CircleMask>("MyModule", 1, 0, "CircleMask");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
