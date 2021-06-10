import os
import sys
from PyQt5 import uic
from PyQt5.QtWidgets import QMainWindow, QApplication
from PyQt5.QtCore import QThread, pyqtSignal, pyqtSlot
import socket
import traceback

client_window = os.path.abspath('client.ui')
client_window_class = uic.loadUiType(client_window)[0]
HOST = '127.0.0.1'
PORT = 9999

class Worker(QThread):
    recv_msg = pyqtSignal(str)

    def __init__(self, socket):
        super().__init__()
        self.client_socket = socket

    def run(self):
        print("Thread Start")
        self.receive_msg()

    def receive_msg(self):
        while True:
            data = self.client_socket.recv(4)
            length = int.from_bytes(data, "little")
            data = self.client_socket.recv(length)
            msg = data.decode()
            self.recv_msg.emit(msg)

class MainWindow(QMainWindow, client_window_class):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client_socket.connect((HOST, PORT))
        self.worker = Worker(self.client_socket)
        self.worker.recv_msg.connect(self.fill_textbox)
        self.worker.start()
        self.pushButton.clicked.connect(lambda : self.send_msg(self.client_socket))

    def send_msg(self, socket):
        try:
            msg = self.lineEdit.text()
            data = msg.encode()
            length = len(data)
            socket.sendall(length.to_bytes(4, byteorder="little"))
            socket.sendall(data)
            self.lineEdit.setText("")
        except:
            traceback.print_exc()

    @pyqtSlot(str)
    def fill_textbox(self, msg):
        self.textEdit.append(msg)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    loginwindow = MainWindow()
    loginwindow.show()
    app.exec_()
    
