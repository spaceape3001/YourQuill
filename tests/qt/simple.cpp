#include <QWidget>
#include <QLabel>
#include <QApplication>
#include <QVBoxLayout>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QWidget*     base   = new QWidget;
    QVBoxLayout* lay    = new QVBoxLayout(base);
    lay -> addWidget(new QLabel("Hello"));
    base -> show();
    return app.exec();
}
