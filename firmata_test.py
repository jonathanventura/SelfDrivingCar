#!/usr/bin/python
# -*- coding: utf-8 -*-
# move a servo from a Tk slider - scruss 2012-10-28

import pyfirmata
import pyfirmata.util
#from Tkinter import *

# don't forget to change the serial port to suit
board = pyfirmata.Arduino('/dev/tty.usbmodem1411')

# start an iterator thread so
# serial buffer doesn't overflow
iter8 = pyfirmata.util.Iterator(board)
iter8.start()

# set up pin D9 as Analog Output
pin9 = board.get_pin('d:9:p')

pin9.write(10./255.)

#board.send_sysex(0x71, util.str_to_two_byte_iter("10\0"))

# This stops the iterator
iter8.board = None

#def move_servo(a):
    #pin9.write(int(a)+90)
    #print('writing %d'%(int(a)+90))

# set up GUI
#root = Tk()

# draw a nice big slider for servo position
#scale = Scale(root,
    #command = move_servo,
    #from_ = -10,
    #to = 10,
    #orient = HORIZONTAL,
    #length = 400,
    #label = 'Angle')
#scale.pack(anchor = CENTER)

# run Tk event loop
#root.mainloop()
