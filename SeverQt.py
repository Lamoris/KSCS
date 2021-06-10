import os
import sys
from PyQt5 import uic
from PyQt5.QtWidgets import QMainWindow, QApplication, QTableWidgetItem
from PyQt5.QtCore import QThread, pyqtSignal, pyqtSlot, Qt
import socket
import traceback
import time

server_window = os.path.abspath('Sever.ui')
server_window_class = uic.loadUiType(server_window)[0]
client_list = []


class Bind(QThread):
    client_msg = pyqtSignal(str)

    def __init__(self, socket, addr):
        super().__init__()
        self.client_socket = socket
        self.addr = addr

    def run(self):
        global client_list
        try:
            while True:
                data = self.client_socket.recv(4)
                length = int.from_bytes(data, "little")
                data = self.client_socket.recv(length)
                msg = data.decode()
                t_time = time.strftime('%Y-%m-%d %I:%M:%S %p', time.localtime(time.time()))
                l_msg = f"{t_time} {self.addr} ==> {msg}"
                self.client_msg.emit(l_msg)
                msg = f"{t_time} {self.addr} to All ==> {msg} "
                data = msg.encode()
                length = len(data)
                for client in client_list:
                    if client != self.client_socket:
                        client.sendall(length.to_bytes(4, byteorder="little"))
                        client.sendall(data)
                # self.client_socket.sendall(length.to_bytes(4, byteorder="little"))
                # self.client_socket.sendall(data)
        except:
            print("except : ", self.addr)
        finally:
            self.client_socket.close()


class ConnectClient(QThread):
    client_connceted = pyqtSignal(tuple)
    client_msg_toss = pyqtSignal(str)

    def __init__(self, socket):
        super().__init__()
        self.server_socket = socket

    def run(self):
        global client_list
        try:
            while True:
                client_socet, addr = self.server_socket.accept()
                client_list.append(client_socet)
                self.client_connceted.emit((client_socet, addr))
                bind = Bind(client_socet, addr)
                bind.client_msg.connect(self.toss_msg)
                bind.start()
        except:
            print("server")
            traceback.print_exc()
        finally:
            self.server_socket.close()

    @pyqtSlot(str)
    def toss_msg(self, msg):
        self.client_msg_toss.emit(msg)


class MainWindow(QMainWindow, server_window_class):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.idx = 0
        self.rowCount = 1
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_socket.bind(('', 9999))
        self.server_socket.listen()
        self.client_list = []

        self.worker = ConnectClient(self.server_socket)
        self.worker.client_connceted.connect(self.fill_table)
        self.worker.client_msg_toss.connect(self.fill_textbox)
        self.worker.start()

    @pyqtSlot(tuple)
    def fill_table(self, client):
        print(client)
        self.tableWidget.setRowCount(self.rowCount)
        self.tableWidget.setItem(self.idx, 0, self.Qitem_maker(client[1][0]))
        self.tableWidget.setItem(self.idx, 1, self.Qitem_maker(str(client[1][1])))
        self.rowCount += 1
        self.idx += 1

    def Qitem_maker(self, source):
        item = QTableWidgetItem(source)
        item.setTextAlignment(Qt.AlignmentFlag.AlignRight | Qt.AlignmentFlag.AlignVCenter)
        return item

    @pyqtSlot(str)
    def fill_textbox(self, msg):
        self.textEdit.append(msg)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    serverWindow = MainWindow()
    serverWindow.show()
    app.exec_()
