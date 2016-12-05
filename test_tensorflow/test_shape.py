import tensorflow as tf

with tf.Session() as sess:
    matrix1 = tf.constant([[3., 3.]])

    s1 = tf.shape(matrix1)

    print sess.run(s1)
   
