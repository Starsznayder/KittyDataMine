import pandas as pd
import numpy as np
import json


def transform_cat(cat):
    return np.argmax(np.equal(
        cat.reshape(-1, 1),
        np.sort(np.unique(cat))
    ), axis=1)


def make_table(data, A, B):
    d = data[[A, B]].value_counts()
    Au = sorted(data[A].unique())
    Bu = sorted(data[B].unique())
    return {
        a: [d.get((a, b), 0) for b in Bu]
        for a in Au
    }


def make_stats(file_name, data, target=0):
    return {
        'file_name': file_name.replace('arff', 'csv'),
        'number_of_observations': data.shape[0],
        'number_of_attributes': data.shape[1] - 1,
        'decision_attribute_position': np.argmax(data.columns == target) + 1,
        'attributes': {
            name: {
                'type': ['categorical', 'numerical'][data[name].dtype == 'object'],
                'stats': {
                    'min': data[name].min(),
                    'max': data[name].max(),
                } if data[name].dtype != 'object' else {
                    **{
                        str(value): count
                        for value, count in data[name].value_counts().items()
                    },
                    'table': make_table(
                        data,
                        name,
                        target,
                    ),
                },
            }
            for name in data.columns
        }
    }


def save_json(name, data):
    with open(name, 'w') as file:
        file.write(json.dumps(data))


data = pd.read_csv('iris.arff', header=None)
for c in data.columns:
    print(data[c].dtype)
data[4] = transform_cat(data[4].to_numpy())
cols = data.columns.tolist()
cols = cols[-1:] + cols[:-1]
data = data[cols]
data.to_csv('iris.csv', index=False, header=None, sep=' ')

data = pd.read_csv('haberman.arff', header=None)
data[3] = transform_cat(data[3].to_numpy())
cols = data.columns.tolist()
cols = cols[-1:] + cols[:-1]
data = data[cols]
data.to_csv('haberman.csv', index=False, header=None, sep=' ')

data = pd.read_csv('german.arff', header=None)

for c in data.columns:
    if data[c].dtype == 'O':
        data[c] = transform_cat(data[c].to_numpy())

cols = data.columns.tolist()
cols = cols[-1:] + cols[:-1]
data = data[cols]
data.to_csv('german.csv', index=False, header=None, sep=' ')
