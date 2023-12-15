import pandas as pd
import matplotlib
import numpy as np

matplotlib.use('Agg')
from matplotlib import rcParams

rcParams['font.family'] = 'serif'
import matplotlib.pyplot as plt

""" calibrate the warmup time given a configuration filename """

src_dir = "csv/"
dest_dir = "plots/"
alpha = 0.01  # 99% CI


def main():
    #config_name = "kUniform_warmup_simTime"
    config_name = "kExponential_warmup_simTime"

    # delay = []
    simTime = []
    values = []
    window = 20000

    df = pd.read_csv(src_dir + "/" + config_name + ".csv", sep=',')

    for col in df.columns:
        if col.startswith('Unnamed'):
            values.append(df[col].dropna().to_numpy())
        else:
            simTime.append(df[col].dropna().to_numpy())

    plt.figure(figsize=(10, 8), dpi=100)
    for x, y in zip(simTime, values):
        plt.plot(x, pd.Series(y).rolling(window, 1).mean(), linewidth=0.5)      # , marker='+', markersize=8

    #plt.title("k Uniform (k_min=0s; k_max=1s) - Warm-up")
    plt.title("k Exponential (k_mean=0.5s) - Warm-up")
    plt.xlabel('Simulation Time (s)')
    plt.xticks(np.arange(0, 105000, step=5000), rotation='vertical')
    plt.ylabel('End-to-End Delay (s)')
    plt.ticklabel_format(axis='x', style='sci')
    plt.grid(linestyle='--')
    #plt.savefig(dest_dir + "/kUniform_warmup.png")
    plt.savefig(dest_dir + "/kExponential_warmup.png")
    plt.show()


if __name__ == "__main__":
    main()