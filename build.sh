#!/bin/bash

docker build . --tag=mv-extractor

export CIBW_BUILD=cp310-manylinux_x86_64
export CIBW_PLATFORM=linux
export CIBW_SKIP=pp*
export CIBW_ARCHS=x86_64
export CIBW_MANYLINUX_X86_64_IMAGE=mv-extractor
export CIBW_BUILD_FRONTEND=build

python3 -m cibuildwheel --output-dir wheels