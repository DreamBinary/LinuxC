import threading
import time


def hello(name):
    print("thread id", threading.get_ident())
    print("hello", name)
    time.sleep(5)
    print("hello end")


def func():
    print("thread id", threading.get_ident())
    print("func start")
    time.sleep(5)
    print("func end")


if __name__ == "__main__":
    t0 = threading.Thread(target=hello, args=("Jack",), daemon=True)
    t1 = threading.Thread(target=func)
    t0.start()
    t1.start()
    t0.join()
    t1.join()
    print("main end")

# GIL锁
# GIL锁是python解释器的一个特性，它的全称是全局解释器锁，它的作用是保证同一时刻只有一个线程在执行代码，这样就保证了线程安全，但是也带来了一些问题，比如说多线程并不能提高程序的执行效率，因为同一时刻只有一个线程在执行代码，这样就导致了多核CPU无法充分利用，所以python的多线程只适合处理I/O密集型的任务，而不适合处理CPU密集型的任务。
