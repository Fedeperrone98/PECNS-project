import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import math
from scipy.stats import uniform as unif
from scipy.stats import linregress as regr
import matplotlib
matplotlib.rcParams['font.family'] = "serif"
plt.style.use('ggplot')

M = 25000
k = 1.6*10**(-9)
T = 1*10**(-9)
nameOrder = ['interarrival', 'serviceTime',
             'queueLength', 'responseTime', 'waitingTime']


'''
    Sort every column of dataframe and perform a mean per row
'''
def meanPerRow(df, stat):
    orderedStat = df.apply(lambda x: x.sort_values().values)
    orderedStat = orderedStat.apply(lambda x: x.dropna())
    return pd.DataFrame(orderedStat.mean(axis=1), columns=[stat])


def histogram(df, nbin, path, name, k):
    plt.figure()
    n, bins, patches = plt.hist(
        df['Mean_' + name], nbin, density=True, facecolor='g', alpha=0.75)
    plt.title('Histogram of ' + name + ' ' + k + 's')
    plt.grid(True)
    plt.savefig(path + '.png')


'''
    Service Time Cumulative Distribution Function F(s)
'''
def serviceTimeCDF(s):
    if s < 0:
        Fs = 0
    elif s >=0 and s <= (T*M*math.sqrt(2))/2:
        Fs = ((k * math.pi * (s**2)) /(T**2))
    #elif s >= T*M/2 and s <= T*M/2**0.5:
        #Fs = k * (M/T * (4*s**2-M**2*T**2)**0.5 + (math.pi*s**2)/(T**2)- ((4*s**2)/(T**2)* math.acos((M*T)/(2*s))) - (math.pi*M**2)/(4))
    else:
        Fs = 1.0
    return Fs


'''
    Distance Cumulative Distribution Function F(d)
'''
def distanceCDF(d):
    if d < 0:
        Fd = 0
    #elif d>=0 and d <= M/2:
    elif d>=0 and d <= (M*math.sqrt(2))/2:
        Fd = math.pi*d**2 * k
    #elif d>M/2 and d <= (M*math.sqrt(d))/2:
        #Fd = k * ( (M * math.sqrt((4*(d**2))-(M**2)))  + (math.pi * (d**2)) - ((4*(d**2))*math.acos(M/(2*d))) - (math.pi*(M**2))/(4) )
    else:
        Fd = 1.0
    return Fd


'''
    look for x such that F(x) = P{X < x} = quantile, with an error of maxError
'''
def findQuantile(quantile, name, maxError):
    x = 0.0
    if name == 'serviceTime':
        error = quantile - serviceTimeCDF(x)
        while error > maxError:
            x += (10**(-5))*error
            error = quantile - serviceTimeCDF(x)
    elif name == 'distance':
        error = quantile - distanceCDF(x)
        while error > maxError:
            x += 0.1*error
            error = quantile - distanceCDF(x)
    return x


'''
     find every quantile for both theoretical and sample distribution
'''
def fitDistribution(df, name, maxError):
    theoreticalQ = []
    sampleQ = []
    for i in range(1, len(df)):
        quantile = (i-0.5)/len(df)
        sq = df[name].quantile(quantile)
        tq = findQuantile(quantile, name, maxError)
        sampleQ.append(sq)
        theoreticalQ.append(tq)
        print(quantile, tq, sq)
    return [theoreticalQ, sampleQ]


'''
     draw a qq plot
'''
def qqPlot(theoreticalQ, sampleQ, name):
    slope, intercept, r_value, p_value, std_err = regr(theoreticalQ, sampleQ)

    plt.figure()
    plt.scatter(theoreticalQ, sampleQ, s=0.8, label=name, c='blue')
    y = [x*slope + intercept for x in theoreticalQ]
    plt.plot(theoreticalQ, y, 'r', label='Trend line')
    plt.text(0, max(sampleQ)*0.6, '\n\n$R^2$ = ' + str('%.6f' % r_value**2))
    if intercept > 0:
        plt.text(0, max(sampleQ)*0.55, 'y = ' + str('%.6f' %
                                                    slope) + 'x + ' + str('%.6f' % intercept))
    else:
        plt.text(0, max(sampleQ)*0.55, 'y = ' + str('%.6f' %
                                                    slope) + 'x ' + str('%.6f' % intercept))

    plt.xlabel('Theoretical Quantile')
    plt.ylabel('Sample Quantile')
    plt.title('QQ plot ' + name)
    plt.grid(True)
    plt.legend()


def main():
    '''
        Service Time fitting
    '''
    serviceTime10Rep = pd.read_csv('csv/service_time.csv')
    names = []
    for i in range(10):
        names.append('time')
        names.append('serviceTime' + str(i))
    serviceTime10Rep.columns = names
    serviceTime10Rep = serviceTime10Rep.drop(columns='time')
    serviceTime = meanPerRow(serviceTime10Rep, 'serviceTime')
    serviceTime = serviceTime.sample(n=1000)

    theoreticalQ, sampleQ = fitDistribution(serviceTime, 'serviceTime', 0.0001)
    qqPlot(theoreticalQ, sampleQ, 'serviceTime' )

    plt.savefig(fname="plots/serviceTime_qq_plot")

    '''
        Distance Time fitting
    # '''
    distance10Rep = pd.read_csv('csv/distance.csv')
    names = []
    for i in range(10):
        names.append('time')
        names.append('distance' + str(i))

    distance10Rep.columns = names
    distance10Rep = distance10Rep.drop(columns='time')
    distance = meanPerRow(distance10Rep, 'distance')
    distance = distance.sample(n=1000)

    theoreticalQ, sampleQ = fitDistribution(distance, 'distance', 0.0001)
    qqPlot(theoreticalQ, sampleQ, 'distance')

    plt.savefig(fname="plots/distance_qq_plot")


if __name__ == "__main__":
    main()
