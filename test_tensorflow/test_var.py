import tensorflow as tf

with tf.Session() as sess:
    var = tf.get_variable("test", shape=[2, 3], 
                initializer=tf.constant_initializer(1.0),
                 dtype=tf.float32)
    var2 = tf.Variable(0, trainable=False)

    init = tf.initialize_all_variables()

    sess.run(init)

    #tf.initialize_all_variables().run()
    print 'the value of var'
    print sess.run(var)
    print

    print 'all trainable vars'
    print sess.run(tf.trainable_variables())
    print

    print 'all vars'
    print sess.run(tf.global_variables())
    print

