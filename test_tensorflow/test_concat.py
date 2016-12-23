import tensorflow as tf

with tf.Session() as sess:
    trans = tf.constant([[1.0, 2.0, 3.0],
                        [2.0, 3.0, 4.0],
                        [3.0, 4.0, 5.0]])

    trans_vec = [trans]*10
    
    trans_concat = tf.concat(0, trans_vec)

    trans_reshape = tf.reshape(trans_concat, [10, 3, 3])

    x = tf.constant([ [1.0, 1.1, 1.2],
                      [2.0, 2.1, 2.2]])
    y = tf.constant([ [1.3, 1.4, 1.5],
                     [2.3, 2.4, 2.5]])
    z = tf.concat(1, [x, y])

    print "trans vec"
    print sess.run(trans_vec)
    print

    print "trans concat"
    print sess.run(trans_concat)
    print

    print "trans reshape"
    print sess.run(trans_reshape)
    print

    print "z is"
    print sess.run(z)
    print
