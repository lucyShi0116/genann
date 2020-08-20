#!/usr/bin/python
# -*-coding:utf-8 -*-
"""
Generate data to simulate people action to turn ON or turn OFF light.

"""

import os
import csv
import numpy as np


def generate_data(num, mu, sigma):
    data = []

    np.random.seed(0)
    s = np.random.normal(mu, sigma, num)
    for i in range(num):
        data.append(int(s[i]))

    # plt.subplot(141)
    # print(s)
    # plt.hist(s, 30, normed=True)

    return data


def write_csv_file(data):
    csv_file = open("C:/Users/310249873/workspace/genann/example/simulate.csv", 'w', newline='\n', encoding='utf-8')

    try:
        writer = csv.writer(csv_file)
        for a in data:
            writer.writerow((a[0],a[1],a[2],a[3]))
    finally:
        csv_file.close()


sample_num = 10
weekday = ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"]
standard_time_morning = 390     # 6:30am
standard_time_evening = 1080     # 18:00pm
action = ["ON", "OFF"]
samples = []
wakeup_time_h_morning_1 = []
wakeup_time_m_morning_1 = []
leave_time_h_morning_2 = []
leave_time_m_morning_2 = []
back_time_h_evening_1 = []
back_time_m_evening_1 = []
sleep_time_h_evening_2 = []
sleep_time_m_evening_2 = []

data_morning = generate_data(sample_num, standard_time_morning, 20)
for i in data_morning:
    wakeup_time_h_morning_1.append(int(i / 60))
    wakeup_time_m_morning_1.append(int(i % 60))
    leave_time_h_morning_2.append(int((i + 30) / 60))
    leave_time_m_morning_2.append(int((i + 30) % 60))
# print(data_morning)
# print(wakeup_time_h_morning)
# print(wakeup_time_m_morning)

data_evening = generate_data(sample_num, standard_time_evening, 10)
for i in data_evening:
    back_time_h_evening_1.append(int(i / 60))
    back_time_m_evening_1.append(int(i % 60))
    sleep_time_h_evening_2.append(int((i + 270) / 60))
    sleep_time_m_evening_2.append(int((i + 270) % 60))
# print(data_evening)
# print(back_time_h_evening_1)
# print(back_time_m_evening_1)
# print(sleep_time_h_evening_2)
# print(sleep_time_m_evening_2)

length = int(len(wakeup_time_h_morning_1) / 7)

# sample = [weekday,hour,minute,action]
for i in range(0, length):
    l = i*len(weekday)
    for j in range(0, len(weekday)):
        # turn on light in the morning
        samples.append([weekday[j],wakeup_time_h_morning_1[l+j],wakeup_time_m_morning_1[l+j],"ON"])

        # turn off light in the morning
        samples.append([weekday[j],leave_time_h_morning_2[l+j],leave_time_m_morning_2[l+j],"OFF"])

        # turn on light in the evening
        samples.append([weekday[j],back_time_h_evening_1[l+j],back_time_h_evening_1[l+j],"ON"])

        # turn off light in the evening
        samples.append([weekday[j],sleep_time_h_evening_2[l+j],sleep_time_m_evening_2[l+j],"OFF"])

write_csv_file(samples)

os.system("set fileformat=unix")



