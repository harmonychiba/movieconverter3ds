import sys
sys.path.append(".")
import movieconverter3ds
from movieconverter3ds import Converter

import Tkinter
from Tkinter import *

revises = []
output_file_name = ""

window = Tk()
pane = PanedWindow(window,orient=HORIZONTAL)
pane.pack(fill=BOTH,expand=1)
root = Frame(pane)
#root.pack(anchor=W)
root2 = Frame(pane)
#root2.pack(anchor=E)
pane.add(root)
pane.add(root2)

print "create tk"

converter = Converter()
converter.loadFile("/Users/Yuki/Documents/sample.mov");
converter.setOutputFilePath("/Users/Yuki/Movies/sample.avi");
converter.setOutputRectSize(640, 360);
init = converter.initialize();
print "start to initialize"
print "preparing to start converter preview"
converter.doDebug(3)
print "add first frame info"
converter.addReviseFrame(0, 0.5, 0.7, 0.25, 0.68, 0.5, 0.56, 0.26, 0.55)
print "finish initializing converter"

global v
v= IntVar()
global frame
frame = 0
global flag_stop
flag_stop= False

print "define functions"

def task():
    global frame
    global flag_stop
    global fnt
    
    if not flag_stop:
        converter.previewAtFrame(frame)
        frame+=1
        fnt.delete(0, END)
        fnt.insert(0, frame)
        window.after(100,task)
    else:
        window.after(1,task)

def changeMode():
    print "change mode to"+str(v.get())
    converter.doDebug(v.get())
    converter.previewAtFrame(frame)

def pause():
    global flag_stop
    flag_stop = True

def start():
    global flag_stop
    flag_stop = False
def changeFrame():
    global fnt
    global frame
    global flag_stop
    if flag_stop:
        frame = int(fnt.get())
        converter.previewAtFrame(frame)

def prevInput():
    global ulx
    global uly
    global urx
    global ury
    global llx
    global lly
    global lrx
    global lry
    global frame
    global flag_stop
    if flag_stop:
        converter.previewInputPoints(float(ulx.get()),float(uly.get()),float(urx.get()),float(ury.get()),float(llx.get()),float(lly.get()),float(lrx.get()),float(lry.get()))
        converter.previewAtFrame(frame)
        print "preview revise!it's OK??"

def revise():
    global ulx
    global uly
    global urx
    global ury
    global llx
    global lly
    global lrx
    global lry
    global frame
    global flag_stop
    if flag_stop:
        converter.addReviseFrame(frame,float(ulx.get()),float(uly.get()),float(urx.get()),float(ury.get()),float(llx.get()),float(lly.get()),float(lrx.get()),float(lry.get()))
        converter.previewAtFrame(frame)
        revise_data = [frame,float(ulx.get()),float(uly.get()),float(urx.get()),float(ury.get()),float(llx.get()),float(lly.get()),float(lrx.get()),float(lry.get())]
        revise.append(revise_data)
def open_file():
    global inpath
    converter.loadFile(inpath.get())
def set_file_name():
    global outpath
    converter.setOutputFilePath(outpath.get())
    output_file_name = outpath.get()
def convert():
    converter.converte()
def save():
    for data in revise:
        print str(data[0])+","+str(data[1])+","+str(data[2])+","+str(data[3])+","+str(data[4])+","+str(data[5])+","+str(data[6])+","+str(data[7])+","+str(data[8])

Radiobutton(root,text="Input Image",variable=v,value=0,command=changeMode).pack(anchor=W)
Radiobutton(root,text="Current Points",variable=v,value=1,command=changeMode).pack(anchor=W)
Radiobutton(root,text="Upper Left Color Extraction Result",variable=v,value=2,command=changeMode).pack(anchor=W)
Radiobutton(root,text="Upper Right Color Extraction Result",variable=v,value=3,command=changeMode).pack(anchor=W)
Radiobutton(root,text="Lower Left Color Extraction Result",variable=v,value=4,command=changeMode).pack(anchor=W)
Radiobutton(root,text="Lower Right Color Extraction Result",variable=v,value=5,command=changeMode).pack(anchor=W)
Radiobutton(root,text="Upper Left Calcuration Result",variable=v,value=6,command=changeMode).pack(anchor=W)
Radiobutton(root,text="Upper Right Calcuration Result",variable=v,value=7,command=changeMode).pack(anchor=W)
Radiobutton(root,text="Lower Left Calcuration Result",variable=v,value=8,command=changeMode).pack(anchor=W)
Radiobutton(root,text="Lower Right Calcuration Result",variable=v,value=9,command=changeMode).pack(anchor=W)
Radiobutton(root,text="Calcuration Result",variable=v,value=10,command=changeMode).pack(anchor=W)
Radiobutton(root,text="Perspective Transformation Result",variable=v,value=-1,command=changeMode).pack(anchor=W)
Radiobutton(root,text="Preview Revise Input Point",variable=v,value=-2,command=changeMode).pack(anchor=W)
Button(root,text="stop",command=pause).pack(anchor=W)
Button(root,text="start",command=start).pack(anchor=W)
Label(root,text="frame number").pack(anchor=W)
fnt = Entry(root)
fnt.pack(anchor=W)
Button(root,text="Change Frame Number",command=changeFrame).pack(anchor=W)
ulx = Entry(root2)
uly = Entry(root2)
urx = Entry(root2)
ury = Entry(root2)
llx = Entry(root2)
lly = Entry(root2)
lrx = Entry(root2)
lry = Entry(root2)
ulx.pack(anchor=W)
uly.pack(anchor=W)
urx.pack(anchor=W)
ury.pack(anchor=W)
llx.pack(anchor=W)
lly.pack(anchor=W)
lrx.pack(anchor=W)
lry.pack(anchor=W)
Button(root2,text="check Revise",command=prevInput).pack(anchor=W)
Button(root2,text="add Revise",command=revise).pack(anchor=W)
Button(root2,text="Convert",command=convert).pack(anchor=W)
Button(root2,text="Save",command=save).pack(anchor=W)

inpath = Entry(root2)
inpath.pack(anchor=W)
Button(root2,text="open file",command=open_file).pack(anchor=W)
outpath = Entry(root2)
outpath.pack(anchor=W)
Button(root2,text="output file name",command=set_file_name).pack(anchor=W)

print "start main loop"
window.after(1,task)
window.mainloop()
