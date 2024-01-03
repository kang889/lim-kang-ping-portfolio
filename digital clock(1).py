from tkinter import *
from time import strftime

mywindow = Tk()
mywindow.title('my clock')

def time():
    mytime = strftime('%H:%M:%S %p')
    clock.config(text= mytime)
    clock.after(1000, time)

clock = Label(mywindow, font =('arial', 40, 'bold'), background = 'dark green', foreground = 'white')

clock.pack(anchor='center')
time()
mainloop()


