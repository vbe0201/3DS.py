#!/bin/bash -ex

source /etc/profile.d/devkit-env.sh

apt-get update
apt-get install -y patch

make -C 3DS.py
