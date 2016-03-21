#/usr/bin/python
#encoding=gbk

import logging

def _init_log():
    # 生成log对象
    logger = logging.getLogger("test_logging")
    # 日志打印到磁盘；默认输出模式为'a'(文件末尾追加写)
    # 除了FileHandler，还支持SocketHandler, SMTPHandler等
    console=logging.StreamHandler()
    # 设置日志格式
    formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
    console.setFormatter(formatter)
    logger.addHandler(console)
    logger.setLevel(logging.DEBUG)
    return logger

# 下面是使用logger打印日志
def run():
    logger = _init_log()
    logger.debug('debug info')
    logger.info("info message")
    print(__file__)
    print(__line__)

run()
