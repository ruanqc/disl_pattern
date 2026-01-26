#!/bin/bash

# Sweep different factor values to find the one that gives 0.966 MPa
# Usage: bash run_factor_sweep.sh

FACTORS=(-0.95 -0.96 -0.965 -0.970 -0.975 -0.980)
OUTPUT_DIR="factor_sweep_results"

mkdir -p "$OUTPUT_DIR"

for factor in "${FACTORS[@]}"; do
    echo "Running simulation with factor = $factor"
    
    # Copy the template file
    cp test_ds_px.i test_ds_px_factor_${factor}.i
    
    # Replace the factor value using sed
    sed -i "s/factor = -[0-9.]*$/factor = $factor/" test_ds_px_factor_${factor}.i
    
    # Run MOOSE simulation (adjust path as needed)
    # mpirun -np 4 /path/to/moose/modules/solid_mechanics/solid_mechanics-opt -i test_ds_px_factor_${factor}.i
    
    # Move outputs
    # mv rhoe_x_out_h5_csv.csv "$OUTPUT_DIR/pk2_factor_${factor}.csv"
    
    echo "Completed factor = $factor"
done

echo ""
echo "Results saved in $OUTPUT_DIR/"
echo "Check pk2 values in each CSV file"
