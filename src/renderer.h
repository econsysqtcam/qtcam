#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QImage>
#include <QQuickPaintedItem>

class ImageItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
public:
    ImageItem(QQuickItem *parent = NULL);
    QImage image() const;
    void setImage(const QImage &image);

    void paint(QPainter *painter);
signals:
    void imageChanged();
private:
    QImage m_image;
};


class Helper: public QObject{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
public:
    QImage image() const{ return m_image; }
    void setImage(const QImage &image){
        m_image = image;
        emit imageChanged();
    }
signals:
    void imageChanged();
private:
    QImage m_image;
};
#endif // IMAGEITEM_H

