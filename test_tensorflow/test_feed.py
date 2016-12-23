import tensorflow as tf
import numpy as np

with tf.Session() as sess:
    a = tf.constant([1., 2., 3.])
    b = tf.placeholder(tf.float32, [3])

    c = a + b

    print sess.run(c,
        feed_dict={b: [1., 1., 1.]})

    b_np = np.array([1., 1., 1.])
    print sess.run(c,
        feed_dict={b: b_np})


