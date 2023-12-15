import multiprocess
import os
import time


def hello(name):
    print("pid", os.getpid())
    print("pid", multiprocess.current_process().pid)
    print("hello", name)
    time.sleep(5)
    print("hello end")


def func():
    print("func start")
    time.sleep(5)
    print("func end")


if __name__ == "__main__":
    p0 = multiprocess.Process(target=hello, args=("Jack",))
    p1 = multiprocess.Process(target=func)
    p0.start()
    p1.start()
    p0.join()
    p1.join()
    print("main end")
