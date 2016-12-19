import tensorflow as tf

with tf.Session() as sess:
    x = tf.constant([[1., 2., 3.],
                    [2., 3., 4.],
                    [1., 2., 3.],
                    [2., 3., 4.]])

    y1 = tf.pad(x, paddings=[[1, 1], [2, 2]], mode='CONSTANT')
    print "y1 is"
    print sess.run(y1)
    print
    print

    y2 = tf.pad(x, paddings=[[1, 0], [2, 0]], mode='CONSTANT')
    print "y2 is"
    print sess.run(y2)
    print
    print

    x2 = tf.constant([
                    [[1., 2., 3.],
                    [2., 3., 4.],
                    [1., 2., 3.],
                    [2., 3., 4.]]
                    ])

    y3 = tf.pad(x2, paddings=[[0, 0], [1, 1], [0, 0]], mode='CONSTANT')
    print "y3 is"
    print sess.run(y3)
    print
    print
