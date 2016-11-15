import tensorflow as tf
import numpy as np

with tf.Session(config=tf.ConfigProto(log_device_placement=True)) as sess:
    with tf.device("/gpu:1"):
        matrix1 = tf.constant([[3., 3.]])
        matrix2 = tf.constant([[2.],[2.]])
        product = tf.matmul(matrix1, matrix2)
    result = sess.run(product)
    print result
    print product.eval()

with tf.Session() as sess:
    with tf.device("/gpu:1"):
        matrix1 = tf.constant([[3., 3.]])
        matrix2 = tf.constant([[2.],[2.]])
        product = tf.matmul(matrix1, matrix2)
    result = sess.run(product)
    print result
    print product.eval()


sess = tf.InteractiveSession()

x = tf.constant([[3., 3.]])
y = tf.constant([[2.],[2.]])
result = tf.matmul(x, y)

print result.eval()
print sess.run(result)
