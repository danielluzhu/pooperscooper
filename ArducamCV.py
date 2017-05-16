import time
import serial
import optparse
import logging
import cv2
import numpy as np

parser = optparse.OptionParser('arducam_read')
parser.add_option(
    '--device', type='string', default='/dev/cu.usbmodemFD121',
    help='serial device to read from')
parser.add_option('--baudrate', type='int', default=921600, help='baud rate')
parser.add_option(
    '--rtscts', action='store_true', default=False, help='enable rtscts')
parser.add_option(
    '--dsrdtr', action='store_true', default=False, help='enable dsrdtr')
parser.add_option(
    '--xonxoff', action='store_true', default=False, help='enable xonxoff')

def main(opts):
    logging.basicConfig(level=logging.INFO)
    '''
    colorLow1 = np.array([12, 30, 19], dtype='uint8')
    colorHigh1 = np.array([12, 100, 54], dtype='uint8')
    colorLow2 = np.array([24, 55, 30], dtype='uint8')
    colorHigh2 = np.array([28, 65, 66], dtype='uint8')
    colorLow3 = np.array([9, 18, 21], dtype='uint8')
    colorHigh3 = np.array([19, 35, 69], dtype='uint8')
    colorLow4 = np.array([0, 0, 0], dtype='uint8')
    colorHigh4 = np.array([179, 255, 255], dtype='uint8')
    '''
    '''
    colorLow1 = np.array([17, 18, 40], dtype='uint8')
    colorHigh1 = np.array([56, 71, 141], dtype='uint8')
    colorLow2 = np.array([33, 27, 66], dtype='uint8')
    colorHigh2 = np.array([25, 56, 139], dtype='uint8')
    notcolorLow = np.array([43, 40, 44], dtype='uint8')
    notcolorHigh = np.array([58, 59, 55], dtype='uint8')
    '''
    colorLow1 = np.array([143, 31, 0], dtype='uint8')
    colorHigh1 = np.array([190, 255, 255], dtype='uint8')
    colorLow2 = np.array([0, 25, 0], dtype='uint8')
    colorHigh2 = np.array([50, 255, 255], dtype='uint8')
 
    port = serial.Serial(
        opts.device, opts.baudrate,
        dsrdtr=opts.dsrdtr, rtscts=opts.rtscts,
        xonxoff=opts.xonxoff)
    s = port.readline()
    print "ArduCAM Start!:", s
    #assert(s.startswith('ArduCAM Start!'))
    s = port.readline()
    print "OV5642 detected.:", s
    #assert(s.startswith("OV5642 detected."))
    time.sleep(0.2)
    port.write(chr(0x04))
    s = port.readline()
    print "s=", s
    s = port.readline()
    print "s=", s
    port.write(chr(0x20))
    last = None
    #f = open("test.jpg", "w")
    while(1):
        res = []
        write = False
        while(True):
            r = port.read(1)
            if last:
                if ord(r) == 0xd8 and ord(last) == 0xff:
                    #print "Saw header"
                    #f.write(last)
                    #res += str(last)
                    res.append(np.uint8(ord(last)))
                    write = True

                if write:
                    #f.write(r)
                    #res += str(r)
                    res.append(np.uint8(ord(r)))
                if ord(r) == 0xd9 and ord(last) == 0xff:
                    #print "Saw footer"
                    break
            last = r
        #f.close()
        #img = cv2.imdecode(np.fromstring(res, dtype='uint8'), 1)
        img = cv2.imdecode(np.array(res), 1)
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        #mask = cv2.inRange(img, colorLow, colorHigh)
        ''' mask1 = cv2.inRange(hsv, colorLow1, colorHigh1);
        mask2 = cv2.inRange(hsv, colorLow2, colorHigh2);
        mask3 = cv2.inRange(hsv, colorLow3, colorHigh3);
        mask4 = cv2.inRange(hsv, colorLow4, colorHigh4);'''
        #mask = cv2.max(mask1, mask2, mask4)
        mask1 = cv2.inRange(img, colorLow1, colorHigh1);
        mask2 = cv2.inRange(img, colorLow2, colorHigh2);
        #mask3 = cv2.inRange(img, notcolorLow, notcolorHigh);
        mask = (mask1 | mask2)
        mask = cv2.erode(mask, None, iterations=2)
        mask = cv2.dilate(mask, None, iterations=2)
        output = cv2.bitwise_and(img, img, mask=mask)
        cv2.imshow('image', np.hstack([img, output]))
        k = cv2.waitKey(5) & 0xFF
        if k == 27:
            break
    
if __name__ == '__main__':
  opts, args = parser.parse_args()
  main(opts)

