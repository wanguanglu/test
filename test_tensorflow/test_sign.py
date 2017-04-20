import tensorflow as tf

with tf.Session() as sess:
    x  = tf.constant([2, 3, 4, 0, 0])
    x2 = tf.constant([2, 2, 3, 3, 3])
    y = tf.sign(x)
    y2 = x * x2

    print "y"
    print sess.run(y)

    print "y2"
    print sess.run(y2)
