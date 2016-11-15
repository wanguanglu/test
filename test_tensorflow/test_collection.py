import tensorflow as tf


with tf.Session() as sess:
    x = tf.constant(10)
    y = tf.constant(11)

    tf.add_to_collection("test", x)
    tf.add_to_collection("test", y)

    z = tf.get_collection("test")
    w = tf.get_collection("test")

    result = sess.run(z)
    print result
    result = sess.run(w)
    print result
