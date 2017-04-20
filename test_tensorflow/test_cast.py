import tensorflow as tf

with tf.Session() as sess:
    x = tf.constant([1, 2, 3])
    y = tf.cast(x, tf.float32)

    print sess.run(y)


