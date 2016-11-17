import tensorflow as tf

with tf.Session() as sess:
    var  = tf.Variable(tf.ones(shape=[5, 10], dtype=tf.float32))
    var2 = tf.nn.dropout(var, 0.5, [5, 10])
    var3 = tf.nn.dropout(var, 0.5, [1, 10])

    tf.initialize_all_variables().run()

    print sess.run(var)
    print
    print sess.run(var2)
    print
    print sess.run(var3)
    print

