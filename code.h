#ifndef CODE_H
#define CODE_H

#include <QDebug>
#include <QIcon>
#include <QWidget>
#include <QPainter>
#include <QSpacerItem>
#include <QByteArray>
#include <QPaintEvent>
#include "qrencode/qrencode.h"

class QRCode:public QWidget {
    Q_OBJECT
public:
    enum QR_MODE {
        MODE_NUL = QR_MODE_NUL,
        MODE_NUM = QR_MODE_NUM,
        MODE_AN = QR_MODE_AN,
        MODE_8 = QR_MODE_8,
        MODE_KANJI = QR_MODE_KANJI,
        MODE_STRUCTURE = QR_MODE_STRUCTURE,
        MODE_ECI = QR_MODE_ECI,
        MODE_FNC1FIRST = QR_MODE_FNC1FIRST,
        MODE_FNC1SECOND = QR_MODE_FNC1SECOND
    };
    
    enum QR_LEVEL {
        LEVEL_L = QR_ECLEVEL_L,
        LEVEL_M = QR_ECLEVEL_M,
        LEVEL_Q = QR_ECLEVEL_Q,
        LEVEL_H = QR_ECLEVEL_H
    };

    QRCode(const QByteArray &text, const QSize &size, QWidget *parent=NULL):QWidget(parent) {
        if (text.length () == 0) {
            this->text = QByteArray("http://github.com/chinat");
        }
        else {
            this->text = text;
        }
        setFixedSize (size);
        margin = 10;
        foreground = QColor("black");
        background = QColor("white");
        casesen = true;
        mode = MODE_8;
        level = LEVEL_Q;
        percent = 0.23;
    }
    
    void setMargin(const int &margin) {
        this->margin = margin;
        this->repaint ();
    }
    
    void setIcon(const QIcon &icon, qreal percent) {
        this->percent = percent < 0.5 ? percent : 0.3;
        this->icon = icon;
        this->repaint ();
    }
    
    void caseSensitive(bool flag) {
        this->casesen = flag;
        this->repaint ();
    }
 
protected:
    void paintEvent (QPaintEvent *event) {
        QWidget::paintEvent (event);
        QPainter painter(this);
        QRcode *qrcode = QRcode_encodeString(text.data (), 7, (QRecLevel)level, (QRencodeMode)mode, casesen ? 1 : 0);
        if(0 != qrcode) {
            unsigned char *point = qrcode->data;
            painter.setPen(Qt::NoPen);
            painter.setBrush(this->background);
            painter.drawRect(0, 0, this->width(), this->height());
            double scale = (this->width () - 2.0 * margin) / qrcode->width;
            painter.setBrush(this->foreground);
            for (int y = 0; y < qrcode->width; y ++) {
                for (int x = 0; x < qrcode->width; x ++) {
                    if (*point & 1) {
                        QRectF r(margin + x * scale, margin + y * scale, scale, scale);
                        painter.drawRects(&r, 1);
                    }
                    point ++;
                }
            }
            point = NULL;
            QRcode_free(qrcode);
            // draw icon
            // if (icon.isNull ()) {
            painter.setBrush(this->background);
            double icon_width = (this->width () - 2.0 * margin) * percent;
            double icon_height = icon_width;
            double wrap_x = (this->width () - icon_width) / 2.0;
            double wrap_y = (this->width () - icon_height) / 2.0;
            QRectF wrap(wrap_x - 5, wrap_y - 5, icon_width + 10, icon_height + 10);
            painter.drawRoundRect(wrap, 50, 50);
            QPixmap image(":/logo.png");
            QRectF target(wrap_x, wrap_y, icon_width, icon_height);
            QRectF source(0, 0, image.width (), image.height ());
            painter.drawPixmap (target, image, source);
        }
        qrcode = NULL;
        event->accept ();
    }
    
private:
    bool casesen;
    int margin;
    QIcon icon;
    qreal percent;
    QByteArray text;
    QColor foreground;
    QColor background;
    QR_MODE mode;
    QR_LEVEL level;
};
#endif // CODE_H