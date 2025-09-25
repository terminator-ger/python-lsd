from bdb import set_trace
import ctypes as CT
import os.path
import numpy as np
import pdb
import cv2
# Find and load the library

libpath = os.path.join(os.path.dirname(__file__), 'liblsd.so')
print("Loading {}".format(libpath))
liblsd = CT.cdll.LoadLibrary(libpath)

class lines_t(CT.Structure):
    _fields_ = [("lines_v", CT.POINTER(CT.c_double)),
                ("lines_h", CT.POINTER(CT.c_double)),
                ("len_v", CT.c_int),
                ("len_h", CT.c_int)]

# Set the argument types
liblsd.lsd.argtypes = [CT.POINTER(CT.c_int), 
                       CT.POINTER(CT.c_double), 
                       CT.c_int, CT.c_int]
liblsd.lsd.restype = CT.POINTER(CT.c_double)

liblsd.lsd_with_line_merge.argtypes = [CT.POINTER(CT.c_double), 
                                       CT.c_int, CT.c_int]
liblsd.lsd_with_line_merge.restype = CT.POINTER(lines_t)

liblsd.free_memory.argtypes = [CT.c_void_p]
liblsd.free_memory.restype = None

def convert(img):
    if isinstance(img, np.ndarray):
        if img.shape[-1] == 3:
            img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        if img.dtype == np.uint8:
            img = img.astype(np.double)
    return img

def img_to_c(img):
    y,x = img.shape
    img_pix = (CT.c_double * (x*y))(*img.reshape(-1))
    return img_pix, x, y
    
 
def lsd(img):
    img = convert(img)
    n_out = CT.c_int() 
    img_pix, x, y = img_to_c(img) 

    segs = liblsd.lsd(CT.byref(n_out), img_pix, x, y)

    ret = [(segs[7*i+0], 
                segs[7*i+1], 
                segs[7*i+2], 
                segs[7*i+3], 
                segs[7*i+4], 
                segs[7*i+5],
                segs[7*i+6]) for i in range(n_out.value)]
    liblsd.free_memory(segs)
    return ret


def lsd_with_line_merge(img):
    img = convert(img)
    img_pix, x, y = img_to_c(img)
    segs = liblsd.lsd_with_line_merge(img_pix,
                                     x,
                                     y)
    w = 4
    lines_v = [(segs.contents.lines_v[w*i+0],
                segs.contents.lines_v[w*i+1], 
                segs.contents.lines_v[w*i+2],
                segs.contents.lines_v[w*i+3]) for i in range(segs.contents.len_v)]

    lines_h = [(segs.contents.lines_h[w*i+0],
                segs.contents.lines_h[w*i+1], 
                segs.contents.lines_h[w*i+2],
                segs.contents.lines_h[w*i+3]) for i in range(segs.contents.len_h)]

    liblsd.free_memory(segs)
    return lines_v, lines_h



    ret = [(segs[w*i+0], 
            segs[w*i+1], 
            segs[w*i+2], 
            segs[w*i+3]) for i in range(n_out.value)]
    return ret

