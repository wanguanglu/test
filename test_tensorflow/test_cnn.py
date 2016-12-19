import tensorflow as tf

with tf.Session() as sess:
   x = tf.constant([
                    [[1., 2., 3.],
                    [2., 3., 4.],
                    [1., 2., 3.],
                    [2., 3., 4.]]
                    ])

   x1 = tf.expand_dims(x, -1)

   print sess.run(x1)
   print
   print

   w = tf.constant([ [1., 1., 0.],
                     [1., 0., 0.] ])

   w1 = tf.expand_dims(w, -1)
   w2 = tf.expand_dims(w1, -1)

   y = tf.nn.conv2d(x1, w2, strides=[1, 1, 1, 1], padding="VALID")
   print "y is"
   print sess.run(y)
   print
   print

   y1 = tf.nn.conv2d(x1, w2, strides=[1, 1, 1, 1], padding="SAME")
   print "y1 is"
   print sess.run(y1)
   print
   print

   w3 = tf.constant([ [1., 0],
                     [0., 1.] ])
   w4 = tf.expand_dims(tf.expand_dims(w3, -1), -1)

   y2 = tf.nn.conv2d(x1, w4, strides=[1, 1, 1, 1], padding="VALID")
   print "y2 is"
   print sess.run(y2)
   print
   print

   y3 = tf.nn.conv2d(x1, w4, strides=[1, 1, 1, 1], padding="SAME")
   print "y3 is"
   print sess.run(y3)
   print
   print
