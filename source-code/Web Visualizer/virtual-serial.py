import os, pty, serial

master, slave = pty.openpty()
s_name = os.ttyname(slave)

print master, slave

ser = serial.Serial(s_name)

# To Write to the device
ser.write('Your text')

# To read from the device
os.read(master,1000)