import numpy as np

import keras
from keras.datasets import imdb
from keras.utils import to_categorical
from keras import models
from keras import layers


# 评论数据向量化
def vectorize_sequences(seqs, dim):
  new_seqs = np.zeros((seqs.shape[0], dim))
  for i, seq in enumerate(seqs):
    new_seqs[i, seq] = 1.
  return new_seqs


def imdb_data():
  word_num = 10000
  (train_seqs, train_lbls), (test_seqs, test_lbls) = imdb.load_data(num_words=word_num)
  train_seqs = vectorize_sequences(train_seqs, word_num)
  test_seqs = vectorize_sequences(test_seqs, word_num)
  return (train_seqs, train_lbls), (test_seqs, test_lbls)


def main():
  (train_seqs, train_lbls), (test_seqs, test_lbls) = imdb_data()
  test_num = 10000
  test_seqs, test_lbls = train_seqs[:test_num], train_lbls[:test_num]
  train_seqs, train_lbls = train_seqs[test_num:], train_lbls[test_num:]
  model = models.Sequential()
  model.add(layers.Dense(16, activation='relu', input_shape=(train_seqs.shape[1],)))
  model.add(layers.Dense(16, activation='relu'))
  model.add(layers.Dense(1, activation='sigmoid'))
  model.compile(optimizer='rmsprop', loss='binary_crossentropy', metrics=['accuracy'])
  model.fit(train_seqs, train_lbls, epochs=5, batch_size=512, validation_data=(test_seqs, test_lbls))


if __name__ == '__main__':
  main()