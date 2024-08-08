#!/bin/bash

# Check if a job name is provided as an argument
if [ -z "$1" ]; then
  echo "Usage: $0 <job-name>"
  exit 1
fi

JOB_NAME=$1

# Get the list of job IDs with the given name
JOB_IDS=$(squeue --name="$JOB_NAME" --format=%A)

# Check if there are any jobs to cancel
if [ -z "$JOB_IDS" ]; then
  echo "No jobs found with the name $JOB_NAME"
  exit 0
fi

# Cancel each job
echo "Cancelling jobs with name $JOB_NAME"
for JOB_ID in $JOB_IDS; do
  scontrol update job=$JOB_ID TimeLimit=05:00:00
done

echo "All jobs with name $JOB_NAME have been cancelled"
