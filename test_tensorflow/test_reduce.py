import tensorflow as tf

with tf.Session() as sess:
    x = tf.constant([[1., 2., 3.],
                     [2., 3., 4.]])

    s = tf.reduce_sum(x)
    s2 = tf.reduce_sum(x, 0)
    s3 = tf.reduce_sum(x, 1)

    m = tf.reduce_mean(x, 0)

    p = tf.reduce_prod(x, 0)

    rmax = tf.reduce_max(x)
    rmin = tf.reduce_min(x)

    print "x"
    print sess.run(x)

    print "reduce_sum"
    print sess.run(s)

    print "reduce_sum(x, 0)"
    print sess.run(s2)

    print "reduce_sum(x, 1)"
    print sess.run(s3)

    print "reduce_mean(x, 0)"
    print sess.run(m)

    print "reduce_prod(x, 0)"
    print sess.run(p)

    print "reduce_max(x, 0)"
    print sess.run(rmax)

    print "reduce_min(x, 0)"
    print sess.run(rmin)
