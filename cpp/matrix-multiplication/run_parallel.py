import subprocess
import time
import argparse
from io import StringIO

# Set up argument parser
parser = argparse.ArgumentParser(description="Run N instances of ./main.exe async.")
parser.add_argument("N", type=int, help="Number of instances to run")

args = parser.parse_args()
N = args.N
command = ".\\build\\main.exe"

start_time = time.time()

# Store output from each process
outputs = []

# Start N processes asynchronously and redirect their output to StringIO buffers
processes = []
for _ in range(N):
    proc = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    processes.append(proc)

# Wait for all processes to finish and collect their output
for process in processes:
    stdout, stderr = process.communicate()
    outputs.append((stdout, stderr))

end_time = time.time()
elapsed_time = end_time - start_time


# Optionally print the output from all processes after they finish
for idx, (stdout, stderr) in enumerate(outputs):
    print(f"Output from process {idx + 1}:")
    for line in stdout.splitlines():
        print(f"  {line}")

    if stderr:
        print(f"Errors from process {idx + 1}:\n{stderr}")
    print("\n-----------------------------------------------------")

# Print total elapsed time
print(f"Total time spent: {elapsed_time:.2f} seconds")
