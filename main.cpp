
#include "code.h"
#include <QApplication>


int main(int argc, char **argv) {
   QApplication app(argc, argv);
   QByteArray text("hello, world");
   QRCode code(text, QSize(300, 300));
   code.setMargin (10);
   code.show ();
   return app.exec ();
}
