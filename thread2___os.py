from threading import *
import time
import sys
print(sys.getswitchinterval())

lock = Lock()
Count =0

def task():
    lock.acquire()
    global Count 
    for i in range(1000000):
        Count +=1
    lock.release()
    
if __name__=='__main__':
    t1=Thread(target=task) 
    t2=Thread(target=task) 
    
    
    t1.start()
    t2.start()
    t1.join()
    t2.join()
    print(Count)