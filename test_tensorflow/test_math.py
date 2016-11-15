import tensorflow as tf


with tf.Session() as sess:
    x = tf.constant([3., 4.])
    y = tf.square(x)
    z = tf.constant(3.0)
    w = tf.mul(x, z)

    x_add = tf.add_n([x, y, w])

    print y.eval()
    print w.eval()
    print x_add.eval()
    
    result = sess.run(y)

print result
print type(result)
