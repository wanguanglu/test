import tensorflow as tf


with tf.Session() as sess:
    initializer = tf.random_uniform_initializer(-0.1, 0.1)
    val =  tf.Variable(
