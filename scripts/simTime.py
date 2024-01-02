import pandas as pd
import matplotlib

matplotlib.use('Agg')
from matplotlib import rcParams

rcParams['font.family'] = 'serif'
import matplotlib.pyplot as plt

""" script to calibrate the simulation time, using the sample variance in sliding window """

src_dir = "csv/"
dest_dir = "plots/"
alpha = 0.01  # 99% CI
# repeat = 25


def main():
    #config_name = "kUniform_warmup_simTime"
    config_name = "kExponential_warmup_simTime"

    # delay = []
    simTime = []
    values = []
    window = 40000      # sliding window size

    df = pd.read_csv(src_dir + "/" + config_name + ".csv", sep=',')

    for col in df.columns:
        if col.startswith('Unnamed'):
            values.append(df[col].dropna().to_numpy())
        else:
            simTime.append(df[col].dropna().to_numpy())

    plt.figure(figsize=(20, 7), dpi=100)
    for x, y in zip(simTime, values):
        plt.plot(x, pd.Series(y).rolling(window, min_periods=1).var(), linewidth=0.5)  # , marker='+', markersize=8

    #plt.title("k Uniform (k_min=0s; k_max=1s) - Sim-Time ")
    plt.title("k Exponential (k_mean=0.5s) - Sim-Time ")
    plt.xlabel('Simulation Time (s)')
    plt.ylabel('Variance')
    plt.ticklabel_format(axis='x', style='sci')
    plt.grid(linestyle='--')
    #plt.savefig( dest_dir + "kUniform_simTime.png")
    plt.savefig(dest_dir + "/kExponential_simTime.png")
    plt.show()


if __name__ == "__main__":
    main()