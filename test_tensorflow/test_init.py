import tensorflow as tf


with tf.Session() as sess:
    init1 = tf.random_uniform_initializer(-1.0, 1.0)
    var1 = tf.get_variable("var1",  shape=[2, 4],  initializer=init1)

    init2 = tf.constant_initializer(1.0, dtype=tf.float32)
    var2  = tf.get_variable("var2",  shape=[2, 4],  initializer=init2)

    init3 = tf.random_normal_initializer(1.0, dtype=tf.float32)
    var3  = tf.get_variable("var3",  shape=[2, 4],  initializer=init3)

    tf.initialize_all_variables().run()
    print "random uniform initialize"
    print sess.run(var1)
    print

    print "const intialize"
    print sess.run(var2)
    print

    print "random normal intialize"
    print sess.run(var3)
    print

