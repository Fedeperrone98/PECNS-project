import numpy as np
import math
from scipy.stats import norm
import csv

# ##### Global ##### #

alpha = 0.01    # 99% CI

def compute_weighted_avg_and_CI(values, weights):
    """
    Return the weighted average and standard deviation.

    values, weights -- Numpy ndarrays with the same shape.
    """
    average = np.average(values, weights=weights)
    # Fast and numerically precise:
    variance = np.average((values - average) ** 2, weights=weights)
    std = math.sqrt(variance)
    ci = norm.ppf(1 - alpha / 2) * std / math.sqrt(len(values))
    return average, std, ci