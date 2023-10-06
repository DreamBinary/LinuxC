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
# GIL锁