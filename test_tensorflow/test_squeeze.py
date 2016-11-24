import tensorflow as tf


with tf.Session() as sess:
    a = tf.constant([[2, 3]])
    b = tf.shape(a)

    c = tf.squeeze(a)
    d = tf.shape(c)

    print sess.run(a)
    print sess.run(b)
    print

    print sess.run(c)
    print sess.run(d)

