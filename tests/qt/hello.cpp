#include <QMessageBox>
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QMessageBox::information(nullptr, "Test", "Hello World");
    return 0;
}

