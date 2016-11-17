import tensorflow as tf


with tf.Session() as sess:
    init1 = tf.random_uniform_initializer(-1.0, 1.0)
    var1  = tf.get_variable("var1",  shape=[2, 4],  initializer=init1)
    var1_ = tf.Variable(tf.random_uniform( [2, 4], -1.0, 1.0))

    init2 = tf.constant_initializer(1.0, dtype=tf.float32)
    var2  = tf.get_variable("var2",  shape=[2, 4],  initializer=init2)
    var2_ = tf.Variable(tf.constant(1.0, shape=[2, 4], dtype=tf.float32))

    init3 = tf.random_normal_initializer(1.0, dtype=tf.float32)
    var3  = tf.get_variable("var3",  shape=[2, 4],  initializer=init3)
    var3_ = tf.Variable(tf.random_normal([2, 4], 0.0, 1.0, dtype=tf.float32))

    init4 = tf.truncated_normal_initializer(0.0, 1.0, dtype=tf.float32)
    var4  = tf.get_variable("var4",  shape=[2, 4],  initializer=init4)
    var4_ = tf.Variable(tf.truncated_normal([2, 4], 0.0, 1.0, dtype=tf.float32))

    init5 = tf.zeros_initializer([2, 2], dtype=tf.float32)
    var5  = tf.get_variable("var5",  initializer=init5)
    var5_ = tf.Variable(tf.zeros(shape=[2, 4], dtype=tf.float32))

    init6 = tf.ones_initializer([2, 2], dtype=tf.float32)
    var6  = tf.get_variable("var6",  initializer=init6)
    var6_ = tf.Variable(tf.ones(shape=[2, 4], dtype=tf.float32))

    init7 = tf.uniform_unit_scaling_initializer(dtype=tf.float32)
    var7  = tf.get_variable("var7",  shape=[2, 4],  initializer=init7)

    tf.initialize_all_variables().run()
    print "random uniform initialize"
    print sess.run(var1)
    print
    print sess.run(var1_)
    print

    print "const intialize"
    print sess.run(var2)
    print
    print sess.run(var2_)
    print

    print "random normal intialize"
    print sess.run(var3)
    print
    print sess.run(var3_)
    print

    print "tuncated normal intialize"
    print sess.run(var4)
    print
    print sess.run(var4_)
    print

    print "zeros intialize"
    print sess.run(var5)
    print
    print sess.run(var5_)
    print

    print "ones intialize"
    print sess.run(var6)
    print
    print sess.run(var6_)
    print

    print "uniform unit scaling initialize"
    print sess.run(var7)
    print
