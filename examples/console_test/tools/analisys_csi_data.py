#!/usr/bin/env python3
# -*-coding:utf-8-*-

# Copyright 2021 Espressif Systems (Shanghai) PTE LTD
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# WARNING: we don't check for Python build-time dependencies until
# check_environment() function below. If possible, avoid importing
# any external libraries here - put in external script, or import in
# their specific function instead.

import sys
import json
import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


# only plot useful subcarrier channel
select_list = []
select_list += [i for i in range(5, 31)]
select_list += [i for i in range(33, 58)]
select_list += [i for i in range(66, 122)]
select_list += [i for i in range(123, 191)]
select_list.remove(128)

if __name__ == '__main__':
    if sys.version_info < (3, 6):
        print(" Python version should >= 3.6")
        exit()

    parser = argparse.ArgumentParser(description="analisys csv_data and plot it")
    parser.add_argument('-S', '--src', dest='src_file', action='store', required=True,
                        help="csi_data file, generated by filter_csidata.py")
    args = parser.parse_args()
    file_name = args.src_file

    df = pd.read_csv(file_name)

    df_rssi = df.loc[:, ['rssi']]
    df_rssi.plot(y=['rssi'])
    plt.axis([0, len(df_rssi.index), -100, 0])

    df_csi = df.loc[:, ['len', 'data']]
    size_x = len(df_csi.index)
    size_y = df_csi.iloc[0]['len'] // 2
    array_csi = np.zeros([size_x, size_y], dtype=np.complex64)
    for x, csi in enumerate(df_csi.iloc):
        csi_raw_data = json.loads(csi['data'])
        for y in range(0, len(csi_raw_data), 2):
            array_csi[x][y //
                         2] = complex(csi_raw_data[y + 1], csi_raw_data[y])

    array_csi_modulus = abs(array_csi)
    columns = [f"subcarrier{i}" for i in range(0, size_y)]
    df_csi_modulus = pd.DataFrame(array_csi_modulus, columns=columns)

    df_csi_modulus.plot(y = [f"subcarrier{i}" for i in select_list])
    plt.show()
