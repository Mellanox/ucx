#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Error: NVCC path must be first argument"
    exit 1
fi

nvcc="$1"
shift
output=""
src=""
args=""

# Parse remaining arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -o)
            output="$2"
            shift 2
            ;;
        -c)
            shift
            ;;
        -I*)
            args="$args $1"
            shift
            ;;
        -D*)
            args="$args $1"
            shift
            ;;
        -*)
            # Skip other options
            shift
            ;;
        *)
            src="$1"
            shift
            ;;
    esac
done

if [ -z "$output" ] || [ -z "$src" ]; then
    echo "Error: Missing output or source file"
    exit 1
fi

# Create output directory if it doesn't exist
mkdir -p $(dirname "$output")

# Call NVCC
cmd="$nvcc --verbose -arch=sm_80 -Xcompiler -fPIC $NVCC_FLAGS -c $src -o $output $args"
echo $cmd
$cmd
#$nvcc -Xcompiler "-fPIC" $NVCC_FLAGS -c "$src" -o "$output" $args 