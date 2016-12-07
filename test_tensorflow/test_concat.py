import tensorflow as tf

with tf.Session() as sess:
    trans = tf.constant([[1.0, 2.0, 3.0],
                        [2.0, 3.0, 4.0],
                        [3.0, 4.0, 5.0]])

    trans_vec = [trans]*10
    
    trans_concat = tf.concat(0, trans_vec)

    trans_reshape = tf.reshape(trans_concat, [10, 3, 3])

    print "trans vec"
    print sess.run(trans_vec)
    print

    print "trans concat"
    print sess.run(trans_concat)
    print

    print "trans reshape"
    print sess.run(trans_reshape)
    print
