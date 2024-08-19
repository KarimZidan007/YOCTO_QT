#include "mainwindow.h"

#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFile>
#include <QDir>
#include <QTimer>
#include <QDebug>
#include <unistd.h>
#include <QThread>

class CombinedApp : public QMainWindow {
    Q_OBJECT

public:
    CombinedApp(QWidget *parent = nullptr) : QMainWindow(parent) {
        initUI();
        PIN_NUMB = 17;  // Set your GPIO pin number
        init_gpio(PIN_NUMB);
    }

private slots:
    void gpio_on() {
        QString gpio_path = QString("/sys/class/gpio/gpio%1/value").arg(PIN_NUMB);
        QFile value_file(gpio_path);
        if (value_file.open(QIODevice::WriteOnly)) {
            value_file.write("1");
            value_file.close();
        }
        QThread::msleep(1000);  // Keep the pin high for a while
    }

    void gpio_off() {
        QString gpio_path = QString("/sys/class/gpio/gpio%1/value").arg(PIN_NUMB);
        QFile value_file(gpio_path);
        if (value_file.open(QIODevice::WriteOnly)) {
            value_file.write("0");
            value_file.close();
        }
        QThread::msleep(1000);  // Keep the pin low for a while
    }

private:
    int PIN_NUMB;
    QWidget *centralWidget;
    QPushButton *pushButton;
    QPushButton *pushButton2;
    QGraphicsView *graphicsView;

    void initUI() {
        this->setWindowTitle("LED CONTROL");
        this->setGeometry(100, 100, 800, 600);

        centralWidget = new QWidget(this);
        centralWidget->setStyleSheet("background-color: rgb(0, 0, 0);");

        pushButton = new QPushButton("LED ON", centralWidget);
        pushButton->setGeometry(100, 300, 100, 100);
        pushButton->setStyleSheet("background-color: rgb(26, 95, 180); color: white;");
        connect(pushButton, &QPushButton::clicked, this, &CombinedApp::gpio_on);

        pushButton2 = new QPushButton("LED OFF", centralWidget);
        pushButton2->setGeometry(600, 300, 100, 100);
        pushButton2->setStyleSheet("background-color: rgb(26, 95, 180); color: white;");
        connect(pushButton2, &QPushButton::clicked, this, &CombinedApp::gpio_off);

        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setGeometry(290, 10, 171, 201);
        QGraphicsScene *scene = new QGraphicsScene();
        graphicsView->setScene(scene);
        QPixmap pixmap("file:///media/karim/Tools/img/iti.png");  // Update with the correct image path
        scene->addPixmap(pixmap);

        this->setCentralWidget(centralWidget);
    }

    void init_gpio(int pin_number) {
        QString gpio_path = QString("/sys/class/gpio/gpio%1").arg(pin_number);
        QString export_path = "/sys/class/gpio/export";

        if (!QDir(gpio_path).exists()) {
            QFile export_file(export_path);
            if (export_file.open(QIODevice::WriteOnly)) {
                export_file.write(QByteArray::number(pin_number));
                export_file.close();
            }
            QThread::msleep(1000);  // Wait for the pin to be exported
        }

        QString direction_path = gpio_path + "/direction";
        QFile direction_file(direction_path);
        if (direction_file.open(QIODevice::WriteOnly)) {
            direction_file.write("out");
            direction_file.close();
        }
        QThread::msleep(1000);  // Wait for the direction to be set
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    CombinedApp MainWindow;
    MainWindow.show();
    return app.exec();
}

#include "main.moc"

