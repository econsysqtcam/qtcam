/*
 * Created by Sushanth S
 * This file is used to recieve QImage from another file/Class
 * and render it in another window
 * Copyright © 2015  e-con Systems India Pvt. Limited
 * This file is part of Qtcam.
 * Qtcam is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Qtcam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Qtcam. If not, see <http://www.gnu.org/licenses/>.
*/

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

    //Defines the rectangle in the plane
    QRectF bounding_rect = boundingRect();

    QImage scaled = m_image.scaledToHeight(bounding_rect.height());

    //Defines a point in the plane using floating point precision
    QPointF center = bounding_rect.center() - scaled.rect().center();

    if (center.x() < 0)
        center.setX(0);
    if (center.y() < 0)
        center.setY(0);

    //drawImage(QPointF, QImage) -> Draws the given image at the given point.
    painter->drawImage(center, scaled);
}
