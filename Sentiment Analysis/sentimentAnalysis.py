import numpy as np
import pandas as pd
import re
import matplotlib.pyplot as plt
import math
from matplotlib import rc
from sklearn.model_selection import train_test_split
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.naive_bayes import MultinomialNB
from collections import Counter, defaultdict
from sklearn.metrics import accuracy_score
from sklearn.metrics import classification_report, confusion_matrix
import nltk
from nltk.corpus import stopwords
from sklearn import metrics

nltk.download('stopwords')

"""
Clean the imported text
"""
def clean_text(text):
    text = re.sub(r'<.*?>', '', text)
    text = re.sub(r"\\", "", text)
    text = re.sub(r"\'", "", text)
    text = re.sub(r"\"", "", text)
    text = re.sub(r'\d+', '', text)
    text = text.strip().lower()
    filters = '!"\'#$%&()*+,-./:;<=>?@[\\]^_`{|}~\t\n'
    translate_dict = dict((c, " ") for c in filters)

    translate_map = str.maketrans(translate_dict)
    text = text.translate(translate_map)

    return text

def write_to_file_vocab(file_name, vocab):
    with open (file_name, 'r+') as file:
        read = file.read()
        file.seek(0,0)
        file.write(','.join(vocab) + ",\n" + read)

"""
Generate the bag of words representation of input text
"""
def generate_bow(input_text, vocabulary):
    vectorizer = CountVectorizer(analyzer = "word",
        vocabulary = vocabulary)
    # Get vocabulary, features, and word count
    bow_words = vectorizer.fit_transform(input_text).toarray()
    feature_names = vectorizer.get_feature_names()

    return bow_words, feature_names
"""
Write to file
"""
def write_to_file(file_name, b_words, b_labels,vocab):
    #Transform do the data
    transformed_label = np.array(b_labels)[np.newaxis].T
    combined_data = np.append(b_words, transformed_label, 1)
    for i in range(len(combined_data)):
        for j in range(len(combined_data[i]) - 1):
            if b_words[i][j] > 0:
                b_words[i][j] = 1;
    np.savetxt(file_name, combined_data,fmt="%d", delimiter=',')
    write_to_file_vocab(file_name, vocab)
    return combined_data
"""
Read data and seperate them in to review and labels
"""
def get_data(file_name):
    with open(file_name, "r") as file:
        review = []
        labels = []
    #read and copy every line on an array called 'lines'
        for i, col in enumerate(file):
            devide = col.strip().split('\t')
            review.append(clean_text(devide[0]))
            labels.append(int(devide[1]))
        return review, labels

"""
Process data to extract vocabulary
"""
def get_vocab(data):
    vectorizer = CountVectorizer(
        analyzer = "word",preprocessor = clean_text)
    vectorizer.fit(data)
    inv_vocab = {v: k for k, v in vectorizer.vocabulary_.items()}
    return [inv_vocab[i] for i in range(len(inv_vocab))]

class Tokenizer:
    # Clean the Data
  def clean(self, text):
      text = re.sub(r'<.*?>', '', text)
      text = re.sub(r"\\", "", text)
      text = re.sub(r"\'", "", text)
      text = re.sub(r"\"", "", text)
      text = re.sub(r'\d+', '', text)
      text = text.strip().lower()
      filters = '!"\'#$%&()*+,-./:;<=>?@[\\]^_`{|}~\t\n'
      translate_dict = dict((c, " ") for c in filters)

      translate_map = str.maketrans(translate_dict)
      text = text.translate(translate_map)
      return text
# Tokenize Bag of Words
  def tokenize(self, text):
      clean = self.clean(text).lower()
      stopwords_en = stopwords.words("english")
      butt = [w for w in re.split("\W+", clean) if not w in stopwords_en]
      return butt

class MultinomialNB:

    def __init__(self, classes, tokenizer):
      self.tokenizer = tokenizer
      self.classes = classes

    def devide_classes(self, X, y):
      data = dict()
      for c in self.classes:
        data[c] = X[np.where(y == c)]
      return data

    def fit(self, X, y):
        self.number_class = {}
        self.log_class_priors = {}
        self.wordCount = {}
        self.vocab = set()

        grouped_data = self.devide_classes(X, y)

        for c, data in grouped_data.items():
          self.number_class[c] = len(data)
          self.log_class_priors[c] = math.log(self.number_class[c] / len(X))
          self.wordCount[c] = defaultdict(lambda: 0)

          for text in data:
            counts = Counter(self.tokenizer.tokenize(text))
            for word, count in counts.items():
                if word not in self.vocab:
                    self.vocab.add(word)

                self.wordCount[c][word] += count

        return self

    def laplace_smoothing(self, word, text_class):
      # Since log(0) is undefined, add 1 to the numerator
      num = self.wordCount[text_class][word] + 1
      # add the size of our vocabulary to the denominator
      denom = self.number_class[text_class] + len(self.vocab)
      return math.log(num / denom)

    def predict(self, X):
        result = []
        for text in X:
            # log priors for positive and negative sentiment and tokenize the text
          classScores = {c: self.log_class_priors[c] for c in self.classes}

          words = set(self.tokenizer.tokenize(text))
          for word in words:
              if word not in self.vocab: continue

              for c in self.classes:
                  # add the class scores
                log_w_given_c = self.laplace_smoothing(word, c)
                classScores[c] += log_w_given_c

        #  pick the class with a max score
          result.append(max(classScores, key=classScores.get))

        return result

def main():
    # Define the required file names
    output_train = "preprocessed_train.txt"
    output_test = "preprocessed_test.txt"
    train_data_path = "trainingSet.txt"
    test_data_path = "testSet.txt"
    output_results = "results.txt"

    # PART 1 - TRAIN
    train_data = []
    train_label = []
    train_data, train_label = get_data(train_data_path)

    #Pre Process
    vocabulary_train = get_vocab(train_data)
    #Generate bag of Words
    bow_train, train_feature = generate_bow(train_data,vocabulary_train)

    # PART 1 - TEST
    test_data = []
    test_label = []
    test_data, test_label = get_data(test_data_path)

    #Pre Process
    vocabulary_test = get_vocab(test_data)
    #Generate bag of Words
    bow_test, test_feature = generate_bow(test_data,vocabulary_test)

    # Write to Preprocessed Train Data in txt file
    train_written = write_to_file(output_train, bow_train, train_label, vocabulary_train)
    # Write to Preprocessed Test Data in txt file
    test_written = write_to_file(output_test, bow_test, test_label, vocabulary_test)
    # Check if successfull
    if (test_written is not None) and (train_written is not None):
        print("Following Files:\npreprocessed_train.txt\npreprocessed_test.txt\nSuccessfully Created!!\n\n")
    else:
        print("Could NOT write the files!!")

    # Part 2
    # load the training and test data in data frames
    colnames=['reviews','labels']
    train = pd.read_csv(train_data_path, names=colnames, delimiter="\t")
    test = pd.read_csv(test_data_path, names=colnames, delimiter="\t")


    X_train = train['reviews'].values
    y_train = train['labels'].values
    X_test = test['reviews'].values
    y_test = test['labels'].values

    # Train the Data
    MNaiveBayes = MultinomialNB(
        classes=np.unique(y_train),
        tokenizer=Tokenizer()
    ).fit(X_train, y_train)

    print('Accuracy on Train Data:')
    yy_hat = MNaiveBayes.predict(X_train)
    print(accuracy_score(y_train, yy_hat))

    y_hat = MNaiveBayes.predict(X_test)
    print('Accuracy on Test Data:')
    print(accuracy_score(y_test, y_hat))

    # Writing Results to a file
    with open(output_results, "w") as file1:
        file1.write('Accuracy on Train Data:' + str(accuracy_score(y_train, yy_hat)))
        file1.write('\nAccuracy on Test Data: ' +str(accuracy_score(y_test, y_hat)) + "\n")


if __name__ == "__main__":
    main()
