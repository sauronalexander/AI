#!/usr/bin/python3.4

import sys
import math
import heapq
import glob
import codecs
import re
import copy
from queue import PriorityQueue as PQ

import numpy
from matplotlib import pyplot
# from wordcloud import WordCloud


class Message():
    def __init__(self):
        self.bag_of_words = {}
        self.label = None


def get_data(messages):
    message_list = []
    vocabulary = set()
    for line in messages:
        line = line.replace('\r', '').replace('\n', '')
        words = line.split(' ')
        message = Message()
        for each in words:
            try:
                message.label = int(each)
            except:
                temp = each.split(':')
                vocabulary.add(temp[0])
                message.bag_of_words[temp[0]] = int(temp[1])
        message_list.append(message)
    return message_list, vocabulary


def get_likelihoods(message_list, vocabulary, class_num, k):
    total_class_count = [0 for i in range(class_num)]
    total_word_count = [0 for i in range(class_num)]

    likelihoods_word_y = [{} for i in range(class_num)]
    likelihoods_word_n = [{} for i in range(class_num)]
    likelihoods_class = [0 for i in range(class_num)]

    for index in range(len(message_list)):
        total_class_count[message_list[index].label] += 1
        for word in vocabulary:
            if word in message_list[index].bag_of_words:
                total_word_count[message_list[index].label] += message_list[index].bag_of_words[word]
                try:
                    likelihoods_word_y[message_list[index].label][word] += message_list[index].bag_of_words[word]
                except:
                    likelihoods_word_y[message_list[index].label][word] = message_list[index].bag_of_words[word]
            else:
                likelihoods_word_n[message_list[index].label][word] = 0

    for num in range(class_num):
        likelihoods_class[num] = total_class_count[num] / float(sum(total_class_count))
        for word in likelihoods_word_y[num]:
            likelihoods_word_y[num][word] = (likelihoods_word_y[num][word] + k) / float(total_word_count[num] + k * len(vocabulary))
        for word in likelihoods_word_n[num]:
            likelihoods_word_n[num][word] = (likelihoods_word_n[num][word] + k) / float(total_word_count[num] + k * len(vocabulary))

    return likelihoods_word_y, likelihoods_word_n, likelihoods_class


def get_likelihoods_tfidf(message_list, vocabulary, class_num, k):
    total_class_count = [0 for i in range(class_num)]
    total_word_count = [0 for i in range(class_num)]
    total_word_occurrence = [{} for i in range(class_num)]

    tfidf_weight = [{} for i in range(class_num)]
    likelihoods_word_y = [{} for i in range(class_num)]
    likelihoods_word_n = [{} for i in range(class_num)]
    likelihoods_class = [0 for i in range(class_num)]

    for index in range(len(message_list)):
        total_class_count[message_list[index].label] += 1
        for word in vocabulary:
            if word in message_list[index].bag_of_words:
                total_word_count[message_list[index].label] += message_list[index].bag_of_words[word]
                try:
                    total_word_occurrence[message_list[index].label][word] += 1
                except:
                    total_word_occurrence[message_list[index].label][word] = 1

                try:
                    likelihoods_word_y[message_list[index].label][word] += message_list[index].bag_of_words[word]
                except:
                    likelihoods_word_y[message_list[index].label][word] = message_list[index].bag_of_words[word]
            else:
                likelihoods_word_n[message_list[index].label][word] = 0

    for index in range(len(message_list)):
        for word in message_list[index].bag_of_words:
            try:
                tfidf_weight[message_list[index].label][word] += message_list[index].bag_of_words[word] * math.log10(total_class_count[message_list[index].label] / total_word_occurrence[message_list[index].label][word])
            except:
                tfidf_weight[message_list[index].label][word] = message_list[index].bag_of_words[word] * math.log10(total_class_count[message_list[index].label] / total_word_occurrence[message_list[index].label][word])

    for num in range(class_num):
        likelihoods_class[num] = total_class_count[num] / float(sum(total_class_count))
        for word in likelihoods_word_y[num]:
            likelihoods_word_y[num][word] = (tfidf_weight[num][word] + k) / float(total_word_count[num] + k * len(vocabulary))
        for word in likelihoods_word_n[num]:
            likelihoods_word_n[num][word] = k / float(total_word_count[num] + k * len(vocabulary))

    return likelihoods_word_y, likelihoods_word_n, likelihoods_class


def get_decisions(message_list, likelihoods_word_y, likelihoods_word_n, likelihoods_class, class_num, k):
    map_decisions = []
    ml_decisions = []
    map_correct_count = 0.0
    ml_correct_count = 0.0
    for index in range(len(message_list)):
        map_decision = []
        ml_decision = []
        for num in range(class_num):
            p = 0.0
            for word in message_list[index].bag_of_words:
                try:
                    p += (math.log(likelihoods_word_y[num][word]) * message_list[index].bag_of_words[word])
                except:
                    try:
                        p += (math.log(likelihoods_word_n[num][word]) * message_list[index].bag_of_words[word])
                    except:
                        'do nothing'

            map_decision.append(p + math.log(likelihoods_class[num]))
            ml_decision.append(p)

        map_decisions.append([index, message_list[index].label, map_decision.index(max(map_decision)), max(map_decision)])
        ml_decisions.append([index, message_list[index].label, ml_decision.index(max(ml_decision)), max(ml_decision)])

        if map_decisions[-1][1] == map_decisions[-1][2]:
            map_correct_count += 1.0
        if ml_decisions[-1][1] == ml_decisions[-1][2]:
            ml_correct_count += 1.0

    return map_correct_count / len(message_list), map_decisions, ml_correct_count / len(message_list), ml_decisions


def get_evaluation(decisions, class_num):
    classification_rate = numpy.zeros(shape=(class_num), dtype='float64')
    class_count = numpy.zeros(shape=(class_num))
    confusion_matrix = numpy.zeros(shape=(class_num, class_num), dtype='float64')

    for decision in decisions:
        class_count[decision[1]] += 1
        confusion_matrix[decision[1]][decision[2]] += 1

    for index in range(class_num):
        classification_rate[index] = confusion_matrix[index][index] / class_count[index]
        confusion_matrix[index] = confusion_matrix[index] / class_count[index]

    return classification_rate, confusion_matrix


def odds_ratios(decisions, likelihoods_word_y, likelihoods_word_n, confusion_matrix, top_four_size):
    top_four = []
    pq = PQ()
    for row in range(len(confusion_matrix)):
        for col in range(len(confusion_matrix)):
            if row == col:
                continue
            pq.put((-confusion_matrix[row][col], (row, col)))

    for i in range(top_four_size):
        top_four.append(pq.get()[1])

    top_twenty = []
    for pair in top_four:
        pq = PQ()
        for word in likelihoods_word_y[pair[0]]:
            try:
                log_ratio = math.log(likelihoods_word_y[pair[0]][word] / likelihoods_word_y[pair[1]][word])
            except:
                log_ratio = math.log(likelihoods_word_y[pair[0]][word] / likelihoods_word_n[pair[1]][word])

            pq.put((-log_ratio, word))

        top_twenty.append([])
        for i in range(20):
            top_twenty[-1].append(pq.get()[1])

    return top_four, top_twenty


def create_train_test_file(read_file_list, write_file_name, news_class):
    f_write = codecs.open(write_file_name, 'w', 'utf-8')
    for file in read_file_list:
        f_read = codecs.open(file, 'r', 'cp1252')
        lines = f_read.readlines()
        f_read.close()
        bag_of_words = {}
        for line in lines:
            line = re.sub(r'[\W]', ' ', line)
            line = line.split()
            for word in line:
                try:
                    bag_of_words[word] += 1
                except:
                    bag_of_words[word] = 1
        start = file.find('\\') + 1
        end = file.find('\\', start)
        words = str(news_class[file[start:end]]) + ' ' + str(bag_of_words).replace('{', '').replace('}', '').replace(': ', ':').replace(',', '').replace('\'', '').replace('\"', '').replace('_', '') + '\n'
        words = re.sub(r' :\d+', '', words)
        f_write.write(words)
    f_write.close()


def get_20news_files():
    news_class = {}
    folder_list_train = glob.glob('20news-bydate-train/*')
    file_list_train = []
    news_class_label = 0
    for folder in folder_list_train:
        news_class[folder.replace('20news-bydate-train\\', '')] = news_class_label
        news_class_label += 1
        file_list_train.extend(glob.glob(folder + '/*'))

    folder_list_test = glob.glob('20news-bydate-test/*')
    file_list_test = []
    for folder in folder_list_test:
        file_list_test.extend(glob.glob(folder + '/*'))

    create_train_test_file(file_list_train, '20news_train.txt', news_class)
    create_train_test_file(file_list_test, '20news_test.txt', news_class)
    return


def get_results(train_file_name, test_file_name, class_num, k, top_four_size):
    f = open(train_file_name, 'r')
    train_data = f.readlines()
    f.close()

    f = open(test_file_name, 'r')
    test_data = f.readlines()
    f.close()

    message_train, vocabulary_train = get_data(train_data)
    message_test, vocabulary_test = get_data(test_data)
    likelihoods_word_y, likelihoods_word_n, likelihoods_class = get_likelihoods(message_train, vocabulary_train, class_num, k)
    map_accuracy, map_decisions, ml_accuracy, ml_decisions = get_decisions(message_test, likelihoods_word_y, likelihoods_word_n, likelihoods_class, class_num, k)

    print('k = {0}, map accuracy = {1:.3f}%'.format(k, map_accuracy * 100))

    classification_rate, confusion_matrix = get_evaluation(map_decisions, class_num)
    print('map classification rate:', classification_rate)
    print(numpy.matrix(confusion_matrix))
    for i in range(class_num):
        pq = PQ()
        for word in likelihoods_word_y[i]:
            pq.put((-likelihoods_word_y[i][word], word))
        highest_likelihood = []
        for j in range(20):
            highest_likelihood.append(pq.get()[1])
        print('class =', i, highest_likelihood)

    top_four, top_twenty = odds_ratios(map_decisions, likelihoods_word_y, likelihoods_word_n, confusion_matrix, top_four_size)
    for i in range(len(top_four)):
        print(top_four[i], top_twenty[i])

    # print('k = {0}, ml accuracy = {1:.3f}%'.format(k, ml_accuracy * 100))
    #
    # classification_rate, confusion_matrix = get_evaluation(ml_decisions, class_num)
    # print('ml classification rate:', classification_rate)
    # print(numpy.matrix(confusion_matrix))
    # for i in range(class_num):
    #     pq = PQ()
    #     for word in likelihoods_word_y[i]:
    #         pq.put((-likelihoods_word_y[i][word], word))
    #     highest_likelihood = []
    #     for j in range(20):
    #         highest_likelihood.append(pq.get())
    #     print('class =', i, highest_likelihood[1])
    #
    # top_four, top_twenty = odds_ratios(ml_decisions, likelihoods_word_y, likelihoods_word_n, confusion_matrix, top_four_size)
    # for i in range(len(top_four)):
    #     print(top_four[i], top_twenty[i])

    return


def get_results_tfidf(train_file_name, test_file_name, class_num, k, top_four_size):
    f = open(train_file_name, 'r')
    train_data = f.readlines()
    f.close()

    f = open(test_file_name, 'r')
    test_data = f.readlines()
    f.close()

    message_train, vocabulary_train = get_data(train_data)
    message_test, vocabulary_test = get_data(test_data)

    likelihoods_word_y, likelihoods_word_n, likelihoods_class = get_likelihoods_tfidf(message_train, vocabulary_train, class_num, k)
    map_accuracy, map_decisions, ml_accuracy, ml_decisions = get_decisions(message_test, likelihoods_word_y, likelihoods_word_n, likelihoods_class, class_num, k)

    print('k = {0}, map accuracy = {1:.3f}%'.format(k, map_accuracy * 100))

    classification_rate, confusion_matrix = get_evaluation(map_decisions, class_num)
    print('map classification rate:', classification_rate)
    print(numpy.matrix(confusion_matrix))
    for i in range(class_num):
        pq = PQ()
        for word in likelihoods_word_y[i]:
            pq.put((-likelihoods_word_y[i][word], word))
        highest_likelihood = []
        for j in range(20):
            highest_likelihood.append(pq.get()[1])
        print('class =', i, highest_likelihood)

    top_four, top_twenty = odds_ratios(map_decisions, likelihoods_word_y, likelihoods_word_n, confusion_matrix, top_four_size)
    for i in range(len(top_four)):
        print(top_four[i], top_twenty[i])

    return


def get_word_cloud(file_name):
    f = open(file_name, 'r')
    lines = f.readlines()
    f.close()

    bag_of_words = ''
    for line in lines:
        line = line.replace('\r', '').replace('\n', '')
        words = line.split(' ')
        for word in words:
            try:
                label = int(word)
            except:
                temp = word.split(':')
                for i in range(int(temp[1])):
                    bag_of_words += (temp[0] + ' ')

    wordcloud = WordCloud().generate(bag_of_words)

    pyplot.imshow(wordcloud)
    pyplot.axis("off")
    pyplot.show()

    return


def main():
    numpy.set_printoptions(linewidth=200, precision=3)

    # get_20news_files()
    # get_results('train_email.txt', 'test_email.txt', 2, 1, 2)
    # get_results('8category.training.txt', '8category.testing.txt', 8, 1, 4)
    get_results('20news_train.txt', '20news_test.txt', 20, 1, 4)

    # get_results_tfidf('train_email.txt', 'test_email.txt', 2, 1, 2)
    # get_results_tfidf('8category.training.txt', '8category.testing.txt', 8, 1, 4)
    get_results_tfidf('20news_train.txt', '20news_test.txt', 20, 1, 4)

    # get_word_cloud('train_email.txt')
    # get_word_cloud('8category.training.txt')
    # get_word_cloud('20news_train.txt')

    sys.exit(0)


if __name__ == '__main__':
    main()
