import tensorflow as tf

with tf.Session() as sess:
    x = tf.constant([[1, 2, 3]])

    y = tf.tile(x, [3, 1])

    print sess.run(y)


