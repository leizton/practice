import numpy as np

import keras
from keras.datasets import mnist
from keras.utils import to_categorical
from keras import models
from keras import layers


def norm_imgs(imgs):
  img_num = imgs.shape[0]
  return imgs.reshape(img_num, 28 * 28).astype('float32') / 255


def mnist_data():
  (train_imgs, train_lbls), (test_imgs, test_lbls) = mnist.load_data()
  train_imgs, train_lbls = norm_imgs(train_imgs), to_categorical(train_lbls)
  test_imgs, test_lbls = norm_imgs(test_imgs), to_categorical(test_lbls)
  return (train_imgs, train_lbls), (test_imgs, test_lbls)


def main():
  (train_imgs, train_lbls), (test_imgs, test_lbls) = mnist_data()
  network = models.Sequential()
  network.add(layers.Dense(512, activation='relu', input_shape=(28 * 28,)))
  network.add(layers.Dense(10, activation='softmax'))
  network.compile(optimizer='rmsprop', loss='categorical_crossentropy', metrics=['accuracy'])
  network.fit(train_imgs, train_lbls, epochs=5, batch_size=128)
  test_loss, test_acc = network.evaluate(test_imgs, test_lbls)
  print('loss: {}, acc: {}'.format(test_loss, test_acc))


if __name__ == '__main__':
  main()