import numpy as np
import math
from scipy.stats import norm
import csv
import pandas as pd

""" compute mean, stdDev, CI of each repetition individually and save all in one single file """
# modify as needed

# #### Global #### #

alpha = 0.01  # 99% CI
repeat = 29

def compute_weighted_avg_and_CI(values, weights):
    n = len(values)
    weighted_sum = 0
    total_time = weights[-1] - weights[0]

    for i in range(1, n):
        delta_time = weights[i] - weights[i - 1]
        weighted_sum += values[i] * delta_time

    average = weighted_sum / total_time
    variance = np.average((values - average) ** 2, weights=weights)
    std = math.sqrt(variance)
    ci = norm.ppf(1 - alpha / 2) * std / math.sqrt(len(values))
    return average, std, ci

# #### Main #### #
def main():
    
    directory = "csv"
    config_name = "test"

    values = []
    times = []
    dict_mean_ci = []


    df = pd.read_csv(directory + "/" + config_name + ".csv", sep=';')
    columns = df.columns
    for j in range(0, len(columns), 2):
        selected_col = columns[j:j+2]
        selected_df = df[selected_col]
        for col in selected_df.columns:
            if col.startswith('Unnamed'):
                values.extend(df[col].dropna().to_numpy())
            elif col.startswith("acPacketsInQueue"):
                times.extend(df[col].dropna().to_numpy())

        mean, stdDev, ci = compute_weighted_avg_and_CI(values, times)
        dict_mean_ci.append({"repeat": str(j/2), "mean": mean})
        print(dict_mean_ci)
        values.clear()
        times.clear()

    data = pd.DataFrame(data=dict_mean_ci)
    data.to_csv("csv/mean.csv", sep=';', index=False)


if __name__ == '__main__':
    main()
