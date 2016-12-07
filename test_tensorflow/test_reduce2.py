import tensorflow as tf

with tf.Session() as sess:
    x = tf.constant([ [ [1., 2., 3., 4], [2., 3., 4., 5], [3., 4., 5., 6] ],
          [ [1., 2., 3., 4], [2., 3., 4., 5], [3., 4., 5., 6] ] ])

    y0 = tf.reduce_sum(x, 0)
    y1 = tf.reduce_sum(x, 1)

    l  = tf.reduce_logsumexp(x)
    l0 = tf.reduce_logsumexp(x, 0)
    l1 = tf.reduce_logsumexp(x, 1)

    print "x"
    print sess.run(x)

    print "y0"
    print sess.run(y0)

    print "y1"
    print sess.run(y1)

    print "l"
    print sess.run(l)

    print "l0"
    print sess.run(l0)

    print "l1"
    print sess.run(l1)

