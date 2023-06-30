import pandas as pd
import numpy as np
import json


def transform_cat(cat):
    return np.argmax(np.equal(
        cat.reshape(-1, 1),
        np.sort(np.unique(cat))
    ), axis=1)


def transform_num(num):
    return (num - np.mean(num))/np.std(num)


def make_table(data, A, B):
    d = data[[A, B]].value_counts()
    Au = sorted(data[A].unique())
    Bu = sorted(data[B].unique())
    return {
        int(a): [int(d.get((a, b), 0)) for b in Bu]
        for a in Au
    }


class NpEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, np.integer):
            return int(obj)
        if isinstance(obj, np.int64):
            return int(obj)
        if isinstance(obj, np.floating):
            return float(obj)
        if isinstance(obj, np.ndarray):
            return obj.tolist()
        return super(NpEncoder, self).default(obj)


def make_stats(file_name, data, categorical, target=0):
    data.columns = list(range(len(data.columns)))
    return {
        'file_name': file_name.replace('arff', 'csv'),
        'number_of_observations': data.shape[0],
        'number_of_attributes': data.shape[1] - 1,
        'decision_attribute_position': np.argmax(data.columns == target) + 1,
        'attributes': {
            name: {
                'type': ['numerical', 'categorical'][int(name) in categorical],
                'stats': ({
                    'min': float(data[name].min()),
                    'max': float(data[name].max()),
                } if int(name) not in categorical else {
                    'counts': {
                        str(value): int(count)
                        for value, count in data[name].value_counts().items()
                    },
                    'table': make_table(
                        data,
                        name,
                        target,
                    ) if name != target else None,
                }),
            }
            for name in data.columns
        }
    }


def save_json(name, data):
    with open(name, 'w') as file:
        file.write(json.dumps(data, cls=NpEncoder))


data = pd.read_csv('arff/iris.arff', header=None)
for c in range(len(data.columns)):
    if c in [4]:
        data[c] = transform_cat(data[c].to_numpy())
    else:
        data[c] = transform_num(data[c].to_numpy())
data[4] = transform_cat(data[4].to_numpy())
cols = data.columns.tolist()
cols = cols[-1:] + cols[:-1]
data = data[cols]
data.to_csv('csv/iris.csv', index=False, header=None, sep=' ')
save_json('stats/iris.json', make_stats('iris.csv', data, [0]))


data = pd.read_csv('arff/zoo.arff', header=None)
for c in data.columns:
    data[c] = transform_cat(data[c].to_numpy())
cols = data.columns.tolist()
cols = cols[-1:] + cols[:-1]
data = data[cols]
data.to_csv('csv/zoo.csv', index=False, header=None, sep=' ')
save_json('stats/zoo.json', make_stats('zoo.csv', data, list(range(20))))


data = pd.read_csv('arff/tae.arff', header=None)
for c in range(data.shape[1]):
    if c in [0, 3, 5]:
        data[c] = transform_cat(data[c].to_numpy())
    else:
        data[c] = transform_num(data[c].to_numpy())
cols = data.columns.tolist()
cols = cols[-1:] + cols[:-1]
data = data[cols]
data.to_csv('csv/tae.csv', index=False, header=None, sep=' ')
save_json('stats/tae.json', make_stats('tae.csv', data, [0, 1, 4]))


data = pd.read_csv('arff/example.arff', header=None)
for c in range(data.shape[1]):
    if c in [2, 3, 4]:
        data[c] = transform_cat(data[c].to_numpy())
    else:
        data[c] = transform_num(data[c].to_numpy())
cols = data.columns.tolist()
cols = cols[-1:] + cols[:-1]
data = data[cols]
data.to_csv('csv/example.csv', index=False, header=None, sep=' ')
save_json('stats/example.json', make_stats('example.csv', data, [0, 3, 4]))
