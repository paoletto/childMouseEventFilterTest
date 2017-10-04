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
        qDebug() << "contains: "<<res;
        return res;
    }

protected:
    bool childMouseEventFilter(QQuickItem *item, QEvent *event) override
    {
        Q_UNUSED(item)
        switch (event->type()) {
        case QEvent::MouseButtonPress: {
                if (!contains(static_cast<QMouseEvent*>(event)->pos())) {
                    event->setAccepted(false);
                    return true;
                } else {
                    return false;
                }
            }
            break;
        case QEvent::MouseButtonRelease: {
                if (!contains(static_cast<QMouseEvent*>(event)->pos())) {
                    event->setAccepted(false);
                    return true;
                } else {
                    return false;
                }
            }
            break;
        default:
            return false;
        }
    }
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
