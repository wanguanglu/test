import tensorflow as tf

with tf.Session() as sess:
    x = tf.range(10)

    y = tf.expand_dims(x, 0)


    print sess.run(x)

    print sess.run(y)




