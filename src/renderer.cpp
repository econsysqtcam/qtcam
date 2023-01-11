#include "renderer.h"
#include <QDebug>
#include <QPainter>

ImageItem::ImageItem(QQuickItem *parent):QQuickPaintedItem(parent)
{
}

QImage ImageItem::image() const
{
    return m_image;
}

void ImageItem::setImage(const QImage &image)
{
    m_image = image;
    emit imageChanged();
    update();
}

void ImageItem::paint(QPainter *painter)
{
    if(m_image.isNull())
        return;

    QRectF bounding_rect = boundingRect();
    QImage scaled = m_image.scaledToHeight(bounding_rect.height());
    QPointF center = bounding_rect.center() - scaled.rect().center();

    if (center.x() < 0)
        center.setX(0);
    if (center.y() < 0)
        center.setY(0);
    painter->drawImage(center, scaled);
}
