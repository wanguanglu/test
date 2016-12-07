import tensorflow as tf

with tf.Session() as sess:
    a = tf.constant([1, 2, 3])
    b = tf.constant([[1],
         [2]])

    c = a + b
    
    print sess.run(a+b)


    x = tf.expand_dims(tf.range(10)*5, 1)
    y = tf.expand_dims(tf.range(5), 0)
    z = x + y

    print "x"
    print sess.run(x)
    print

    print "y"
    print sess.run(y)
    print

    print "z"
    print sess.run(z)
