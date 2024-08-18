import sys
from PyQt5 import QtCore, QtGui, QtWidgets
import os
import time
import resources_rc 

class CombinedApp(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.initUI()
        self.PIN_NUMB = 17  # Set your GPIO pin number
        self.init_gpio(self.PIN_NUMB)

    def initUI(self):
        self.setWindowTitle("LED CONTROL")
        self.setGeometry(100, 100, 800, 600)
        self.centralwidget = QtWidgets.QWidget(self)
        self.centralwidget.setObjectName("centralwidget")
        self.centralwidget.setStyleSheet("background-color: rgb(0, 0, 0);")

        self.pushButton = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton.setGeometry(QtCore.QRect(100, 300, 100, 100))
        self.pushButton.setText("LED ON")
        self.pushButton.setStyleSheet("background-color: rgb(26, 95, 180); color: white;")
        self.pushButton.clicked.connect(self.gpio_on)

        self.pushButton2 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton2.setGeometry(QtCore.QRect(600, 300, 100, 100))
        self.pushButton2.setText("LED OFF")
        self.pushButton2.setStyleSheet("background-color: rgb(26, 95, 180); color: white;")
        self.pushButton2.clicked.connect(self.gpio_off)

        self.graphicsView = QtWidgets.QGraphicsView(self.centralwidget)
        self.graphicsView.setGeometry(QtCore.QRect(290, 10, 171, 201))
        self.graphicsView.setStyleSheet("border-image: url(:/img/iti.png);")

        self.setCentralWidget(self.centralwidget)

    def init_gpio(self, pin_number):
        gpio_path = f"/sys/class/gpio/gpio{pin_number}"
        export_path = "/sys/class/gpio/export"
        
        if not os.path.isdir(gpio_path):
            with open(export_path, 'w') as f:
                f.write(str(pin_number))
            time.sleep(1)  # Wait for the pin to be exported

        direction_path = f"{gpio_path}/direction"
        with open(direction_path, 'w') as f:
            f.write("out")
        time.sleep(1)  # Wait for the direction to be set    

    def gpio_on(self):
        gpio_path = f"/sys/class/gpio/gpio{self.PIN_NUMB}"
        value_path = f"{gpio_path}/value"
        with open(value_path, 'w') as f:
            f.write("1")
        time.sleep(1)  # Keep the pin high for a while

    def gpio_off(self):
        gpio_path = f"/sys/class/gpio/gpio{self.PIN_NUMB}"
        value_path = f"{gpio_path}/value"
        with open(value_path, 'w') as f:
            f.write("0")
        time.sleep(1)  # Keep the pin low for a while

def main():
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = CombinedApp()
    MainWindow.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()

