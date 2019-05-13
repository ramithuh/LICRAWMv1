# Multi Color Code Tracking Example
#
# This example shows off multi color code tracking using the OpenMV Cam.
#
# A color code is a blob composed of two or more colors. The example below will
# only track colored objects which have two or more the colors below in them.
# tuned using http://colorizer.org ##
import sensor, image, time
import pyb
from pyb import UART                     ##communication with arduino
uart = UART(3,9600, timeout_char = 1000) ##communication with arduino
#uart is
# TX = P4
# RX = P5


p_7 = pyb.Pin("P7", pyb.Pin.OUT_PP) #goes to LICRAWM 30
p_8 = pyb.Pin("P8", pyb.Pin.OUT_PP) #goes to LICRAWM 32
p_9 = pyb.Pin("P9", pyb.Pin.OUT_PP) #goes to LICRAWM 34
p_7.low()
p_8.low()
p_9.low()


def write_to_arduino(x):
    if(x=='red'):     #001
        p_7.low()
        p_8.low()
        p_9.high()
    elif(x=='green'): #010
        p_7.low()
        p_8.high()
        p_9.low()
    elif(x=='blue'):  #011
        p_7.high()
        p_8.low()
        p_9.low()
    else:            ##Blank message #000
        p_7.high()
        p_8.high()
        p_9.high()

LEDs=[pyb.LED(1),pyb.LED(2),pyb.LED(3)]


def LED_white(f):
    for i in range(0,3):
        if(f==1):
            LEDs[i].on()
        else:
            LEDs[i].off()
#0 red
#1 green
#2 blue

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green things. You may wish to tune them...
thresholds = [(0, 80, 15, 127, 15, 127), # generic_red_thresholds -> index is 0 so code == (1 << 0)
               (30, 100, -128, -15, 0, 127), # generic_green_thresholds -> index is 1 so code == (1 << 1)
              (0, 70, -128, 10, -128, -30)] # generic_blue_thresholds -> index is 2 so code == (1 << 2)
# Codes are or'ed together when "merge=True" for "find_blobs".

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.CIF)

sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. "merge=True" must be set to merge overlapping color blobs for color codes.


areas=[0,0,0] #r.g.b
#0 red
#1 green
#2 blue




pyb.LED(4).on()
i=100000;

for i in range(0,i):

    LED_white(0);
    print("running", str(50-i), areas)

    clock.tick()
    img = sensor.snapshot()

    blobs = img.find_blobs(thresholds, pixels_threshold=300, area_threshold=200)

    if(len(blobs)==0):
        write_to_arduino("nothing")


    for blob in blobs:
       # print(blob.code())

        if blob.code() == 1: # r code
            areas[0]+=blob.area()
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy())
            img.draw_string(blob.x() + 2, blob.y() + 2, "red")
            uart.write("red\n")  ##communication with arduino
            write_to_arduino("red")

        elif blob.code() ==2: # g code
            areas[1]+=blob.area()
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy())
            img.draw_string(blob.x() + 2, blob.y() + 2, "green")
            uart.write("green\n") ##communication with arduino
            write_to_arduino("green")

        elif blob.code() == 4: # b code
            areas[2]+=blob.area()
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy())
            img.draw_string(blob.x() + 2, blob.y() + 2, "blue")
            uart.write("blue\n")  ##communication with arduino
            write_to_arduino("blue")
        else:
            write_to_arduino("nothing")




    time.sleep(1)
    #print(clock.fps())
    #time.sleep(1000)

print(areas)
pyb.LED(4).off()
most=-1;

for i in range(0,3):
    if(areas[i]==max(areas)):
        most=i;
        break;


if(max(areas)==0):
    while(1):
        LED_white(1)
        time.sleep(500)
        LED_white(0)
        time.sleep(500)


else:
    while(1):
        LEDs[most].on()
        time.sleep(500)
        LEDs[most].off()
        time.sleep(500)

