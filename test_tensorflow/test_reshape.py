import tensorflow as tf

with tf.Session() as sess:
    x = tf.constant([[[1., 2., 3.], [2., 3., 4.]],
                     [[0.1, 0.2, 0.3], [0.2, 0.3, 0.4]]])

    y = tf.reshape(x, [2, 2, 3, 1])
    print sess.run(y)
