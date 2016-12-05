import tensorflow as tf

with tf.Session() as sess:
    x = tf.constant([[0, 1, 2], [1, 2, 3], [2, 3, 4], [3, 4, 5]])

    s1 = tf.slice(x, [0, 0], [-1, 2])
    s2 = tf.slice(x, [0, 1], [-1, 2])

    print "x is "
    print sess.run(x)

    print "slice is"
    print sess.run(s1)

    print "slice2 is"
    print sess.run(s2)

