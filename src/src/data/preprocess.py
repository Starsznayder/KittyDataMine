import pandas as pd
import numpy as np


def transform_cat(cat):
    return np.argmax(np.equal(
        cat.reshape(-1, 1),
        np.sort(np.unique(cat))
    ), axis=1)


data = pd.read_csv('iris.arff', header=None)
for c in data.columns[:-1]:
    print('uint8_t' if data[c].dtype == 'O' else 'float', end=',\n')
data[4] = transform_cat(data[4].to_numpy())
cols = data.columns.tolist()
cols = cols[-1:] + cols[:-1]
data = data[cols]
data.to_csv('iris.csv', index=False, header=None, sep=' ')
print()

data = pd.read_csv('haberman.arff', header=None)
for c in data.columns[:-1]:
    print('uint8_t' if data[c].dtype == 'O' else 'float', end=',\n')
data[3] = transform_cat(data[3].to_numpy())
cols = data.columns.tolist()
cols = cols[-1:] + cols[:-1]
data = data[cols]
data.to_csv('haberman.csv', index=False, header=None, sep=' ')
print()

data = pd.read_csv('german.arff', header=None)
for c in data.columns[:-1]:
    print('uint8_t' if data[c].dtype == 'O' else 'float', end=',\n')
for c in data.columns:
    if data[c].dtype == 'O':
        data[c] = transform_cat(data[c].to_numpy())
cols = data.columns.tolist()
cols = cols[-1:] + cols[:-1]
data = data[cols]
data.to_csv('german.csv', index=False, header=None, sep=' ')
print()
