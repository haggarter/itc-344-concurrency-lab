import subprocess
import sys
import os
import time
import threading
import socket

score = 0

def spawn_server(path):
    return subprocess.Popen([path, '8080'])
    
def spawn_client():
    global score
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(('127.0.0.1', 8080))
    message = ("Hello server! I am " + str(sock.getsockname()[1]) + ".\r\n\r\n").encode('utf-8')
    sock.sendall(message)
    resp = sock.recv(len(message))
    if resp == message:
        score += 1
    sock.close()
    
def run_test(num_clients) -> bool:
    global score
    score = 0
    clients = []
    for i in range(0, num_clients):
        client = threading.Thread(target=spawn_client)
        client.start()
        clients.append(client)
    for client in clients:
        client.join()
    return score == num_clients

def main():
    path = sys.argv[1]
    server = spawn_server(path)
    
    time.sleep(1)
    
    tests = [1, 8, 16, 32]
    
    for i, client_num in enumerate(tests):
        print('Test ' + str(i) + '...')
        passed = run_test(client_num)
        if passed:
            print("Passed")
        else:
            print("Failed")
    
    server.terminate()
    server.wait()
    
if __name__ == '__main__':
    main()